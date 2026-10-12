/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   EcoLab1 console client "Calculator" of pseudo random number generators
 * </summary>
 *
 * <description>
 *   Loads one of three independent components (Fibonacci LFSR, Galois LFSR, Xorshift)
 *   by its CID and works with it through two interfaces of one contract:
 *     - IEcoPseudoRandomService1: stateless service (SOA), the client keeps the state;
 *     - IEcoPseudoRandom1: stateful object (OOA), the component keeps the state.
 *   First a demonstration for all algorithms is printed (SOA and OOA must give the same
 *   sequence), then an interactive menu starts; at the end of the input it exits.
 *   The client uses libc input and output (stdio); the components do not use libc.
 * </description>
 *
 * <author>
 *   Copyright (c) 2026 . All rights reserved.
 * </author>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "IEcoSystem1.h"
#include "IdEcoInterfaceBus1.h"
#include "IEcoPseudoRandomService1.h"
#include "IEcoPseudoRandom1.h"
#include "IdEcoFibonacciLFSR1.h"
#include "IdEcoGaloisLFSR1.h"
#include "IdEcoXorshift1.h"

#define ALGO_COUNT 3
#define DEMO_STEPS 8

typedef struct ClientAlgo {
    const char* name;
    const UGUID* cid;
    IEcoComponentFactory* factory;
} ClientAlgo;

static ClientAlgo g_Algos[ALGO_COUNT];

static void PrintValue(uint64_t value, uint16_t bits) {
    if (bits == 32) {
        printf("%08lX", (unsigned long)(value & 0xFFFFFFFFUL));
    } else {
        printf("%08lX%08lX", (unsigned long)(value >> 32), (unsigned long)(value & 0xFFFFFFFFUL));
    }
}

static uint64_t ParseHex(const char* s) {
    uint64_t value = 0;
    char c = 0;

    if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
        s += 2;
    }
    while ((c = *s) != 0) {
        if (c >= '0' && c <= '9') {
            value = (value << 4) | (uint64_t)(c - '0');
        } else if (c >= 'a' && c <= 'f') {
            value = (value << 4) | (uint64_t)(c - 'a' + 10);
        } else if (c >= 'A' && c <= 'F') {
            value = (value << 4) | (uint64_t)(c - 'A' + 10);
        } else {
            break;
        }
        s++;
    }
    return value;
}

static int16_t RegisterAlgorithms(IEcoInterfaceBus1* pIBus) {
    int16_t result = ERR_ECO_SUCCESS;
    uint16_t i = 0;

    g_Algos[0].name = "Fibonacci LFSR";
    g_Algos[0].cid = &CID_EcoFibonacciLFSR1;
    g_Algos[0].factory = GetIEcoComponentFactoryPtr_A0F26A7CE65B43D8B022EFFE919D0053;
    g_Algos[1].name = "Galois LFSR";
    g_Algos[1].cid = &CID_EcoGaloisLFSR1;
    g_Algos[1].factory = GetIEcoComponentFactoryPtr_09232D5025AE4DC5B11844D166391FF5;
    g_Algos[2].name = "Xorshift";
    g_Algos[2].cid = &CID_EcoXorshift1;
    g_Algos[2].factory = GetIEcoComponentFactoryPtr_EB9A5A10589745BE90FEDB27479BCC71;

    for (i = 0; i < ALGO_COUNT; i++) {
        result = pIBus->pVTbl->RegisterComponent(pIBus, g_Algos[i].cid, (IEcoUnknown*)g_Algos[i].factory);
        if (result != ERR_ECO_SUCCESS) {
            printf("Cannot register component %s, result=%d\n", g_Algos[i].name, (int)result);
            return result;
        }
    }
    return ERR_ECO_SUCCESS;
}

/* Generates count values with the stateless service; the client keeps the state. */
static int16_t GenerateService(IEcoInterfaceBus1* pIBus, uint16_t algo, uint16_t bits, uint64_t seed, uint64_t param, uint16_t count, uint64_t* values) {
    IEcoPseudoRandomService1* pSvc = 0;
    int16_t result = ERR_ECO_SUCCESS;
    uint16_t i = 0;
    uint32_t next32 = 0;
    uint64_t next64 = 0;
    uint64_t state = seed;

    result = pIBus->pVTbl->QueryComponent(pIBus, g_Algos[algo].cid, 0, &IID_IEcoPseudoRandomService1, (void**)&pSvc);
    if (result != ERR_ECO_SUCCESS || pSvc == 0) {
        return result;
    }
    for (i = 0; i < count; i++) {
        if (bits == 32) {
            result = pSvc->pVTbl->Step32(pSvc, (uint32_t)state, (uint32_t)param, &next32);
            state = (uint64_t)next32;
        } else {
            result = pSvc->pVTbl->Step64(pSvc, state, param, &next64);
            state = next64;
        }
        if (result != ERR_ECO_SUCCESS) {
            break;
        }
        values[i] = state;
    }
    pSvc->pVTbl->Release(pSvc);
    return result;
}

/* Generates count values with the stateful object; the component keeps the state. */
static int16_t GenerateObject(IEcoInterfaceBus1* pIBus, uint16_t algo, uint16_t bits, uint64_t seed, uint64_t param, uint16_t count, uint64_t* values) {
    IEcoPseudoRandom1* pObj = 0;
    int16_t result = ERR_ECO_SUCCESS;
    uint16_t i = 0;

    result = pIBus->pVTbl->QueryComponent(pIBus, g_Algos[algo].cid, 0, &IID_IEcoPseudoRandom1, (void**)&pObj);
    if (result != ERR_ECO_SUCCESS || pObj == 0) {
        return result;
    }
    result = pObj->pVTbl->Init(pObj, seed, param, bits);
    for (i = 0; i < count && result == ERR_ECO_SUCCESS; i++) {
        result = pObj->pVTbl->Next(pObj, &values[i]);
    }
    pObj->pVTbl->Release(pObj);
    return result;
}

static void PrintSequence(const char* title, uint16_t bits, const uint64_t* values, uint16_t count) {
    uint16_t i = 0;

    printf("    %-4s", title);
    for (i = 0; i < count; i++) {
        printf(" ");
        PrintValue(values[i], bits);
    }
    printf("\n");
}

/* Prints the name reported by the loaded component. */
static void PrintComponentName(IEcoInterfaceBus1* pIBus, uint16_t algo) {
    IEcoPseudoRandom1* pObj = 0;
    int16_t result = ERR_ECO_SUCCESS;

    result = pIBus->pVTbl->QueryComponent(pIBus, g_Algos[algo].cid, 0, &IID_IEcoPseudoRandom1, (void**)&pObj);
    if (result != ERR_ECO_SUCCESS || pObj == 0) {
        printf("    component is not available, result=%d\n", (int)result);
        return;
    }
    printf("    loaded component reports: %s\n", pObj->pVTbl->get_Name(pObj));
    pObj->pVTbl->Release(pObj);
}

/* Shows that both architectures give the same sequence for every algorithm and width. */
static int16_t Demonstration(IEcoInterfaceBus1* pIBus) {
    uint64_t soa[DEMO_STEPS];
    uint64_t ooa[DEMO_STEPS];
    uint16_t algo = 0;
    uint16_t width = 0;
    uint16_t i = 0;
    uint16_t bits = 0;
    uint64_t seed = 0;
    int16_t result = ERR_ECO_SUCCESS;
    int16_t status = ERR_ECO_SUCCESS;
    int same = 1;

    printf("=== Demonstration: stateless service (SOA) and stateful object (OOA) ===\n");
    for (algo = 0; algo < ALGO_COUNT; algo++) {
        for (width = 0; width < 2; width++) {
            bits = (uint16_t)(width == 0 ? 32 : 64);
            seed = (uint64_t)1;
            printf("%s, %u bit, seed 1, default polynomial\n", g_Algos[algo].name, (unsigned int)bits);
            PrintComponentName(pIBus, algo);
            result = GenerateService(pIBus, algo, bits, seed, 0, DEMO_STEPS, soa);
            if (result == ERR_ECO_SUCCESS) {
                result = GenerateObject(pIBus, algo, bits, seed, 0, DEMO_STEPS, ooa);
            }
            if (result != ERR_ECO_SUCCESS) {
                printf("    error, result=%d\n", (int)result);
                status = result;
                continue;
            }
            PrintSequence("SOA", bits, soa, DEMO_STEPS);
            PrintSequence("OOA", bits, ooa, DEMO_STEPS);
            same = 1;
            for (i = 0; i < DEMO_STEPS; i++) {
                if (soa[i] != ooa[i]) {
                    same = 0;
                }
            }
            printf("    SOA == OOA: %s\n", same ? "yes" : "NO");
            if (!same) {
                status = ERR_ECO_FAIL;
            }
        }
    }
    printf("\n");
    return status;
}

#define STAT_COUNT 1000000UL

/* Counts the bits set to one in a 32 bit value. */
static unsigned int PopCount32(uint32_t v) {
    unsigned int n = 0;

    while (v != 0) {
        n += (unsigned int)(v & 1);
        v >>= 1;
    }
    return n;
}

/* Simple quality and speed figures of one algorithm in the object mode (OOA). */
static void StatisticsOne(IEcoInterfaceBus1* pIBus, uint16_t algo, uint16_t bits) {
    IEcoPseudoRandom1* pObj = 0;
    unsigned long bins[256];
    unsigned long i = 0;
    unsigned long ones = 0;
    unsigned long lowOnes = 0;
    unsigned int k = 0;
    unsigned int bytesPerValue = (unsigned int)(bits / 8);
    uint64_t value = 0;
    double expected = 0.0;
    double chi = 0.0;
    double diff = 0.0;
    double seconds = 0.0;
    double secondsSoa = 0.0;
    IEcoPseudoRandomService1* pSvc = 0;
    uint32_t state32 = 0;
    uint64_t state64 = 0;
    uint32_t next32 = 0;
    clock_t start = 0;
    clock_t stop = 0;
    int16_t result = ERR_ECO_SUCCESS;

    result = pIBus->pVTbl->QueryComponent(pIBus, g_Algos[algo].cid, 0, &IID_IEcoPseudoRandom1, (void**)&pObj);
    if (result != ERR_ECO_SUCCESS || pObj == 0) {
        printf("%-15s %2u bit: component is not available\n", g_Algos[algo].name, (unsigned int)bits);
        return;
    }
    result = pObj->pVTbl->Init(pObj, (uint64_t)0x2545F491UL, 0, bits);
    for (k = 0; k < 256; k++) {
        bins[k] = 0;
    }
    start = clock();
    for (i = 0; i < STAT_COUNT && result == ERR_ECO_SUCCESS; i++) {
        result = pObj->pVTbl->Next(pObj, &value);
        ones += PopCount32((uint32_t)(value & 0xFFFFFFFFUL));
        if (bits == 64) {
            ones += PopCount32((uint32_t)(value >> 32));
        }
        lowOnes += (unsigned long)(value & 1);
        for (k = 0; k < bytesPerValue; k++) {
            bins[(unsigned int)((value >> (8 * k)) & 0xFF)]++;
        }
    }
    /* clean timing of the object mode: only Next() in the loop */
    if (result == ERR_ECO_SUCCESS) {
        result = pObj->pVTbl->Init(pObj, (uint64_t)0x2545F491UL, 0, bits);
    }
    start = clock();
    for (i = 0; i < STAT_COUNT && result == ERR_ECO_SUCCESS; i++) {
        result = pObj->pVTbl->Next(pObj, &value);
    }
    stop = clock();
    pObj->pVTbl->Release(pObj);
    if (result != ERR_ECO_SUCCESS) {
        printf("%-15s %2u bit: error %d\n", g_Algos[algo].name, (unsigned int)bits, (int)result);
        return;
    }
    expected = (double)STAT_COUNT * (double)bytesPerValue / 256.0;
    for (k = 0; k < 256; k++) {
        diff = (double)bins[k] - expected;
        chi += diff * diff / expected;
    }
    seconds = (double)(stop - start) / (double)CLOCKS_PER_SEC;

    result = pIBus->pVTbl->QueryComponent(pIBus, g_Algos[algo].cid, 0, &IID_IEcoPseudoRandomService1, (void**)&pSvc);
    if (result == ERR_ECO_SUCCESS && pSvc != 0) {
        state32 = (uint32_t)0x2545F491UL;
        state64 = (uint64_t)0x2545F491UL;
        start = clock();
        for (i = 0; i < STAT_COUNT && result == ERR_ECO_SUCCESS; i++) {
            if (bits == 32) {
                result = pSvc->pVTbl->Step32(pSvc, state32, 0, &next32);
                state32 = next32;
            } else {
                result = pSvc->pVTbl->Step64(pSvc, state64, 0, &state64);
            }
        }
        stop = clock();
        secondsSoa = (double)(stop - start) / (double)CLOCKS_PER_SEC;
        pSvc->pVTbl->Release(pSvc);
    }
    printf("%-15s %2u bit: ones %.5f  low bit %.5f  chi2(bytes) %8.1f  OOA %.2f  SOA %.2f Mvalues/s\n",
           g_Algos[algo].name, (unsigned int)bits,
           (double)ones / ((double)STAT_COUNT * (double)bits),
           (double)lowOnes / (double)STAT_COUNT,
           chi,
           seconds > 0.0 ? (double)STAT_COUNT / seconds / 1000000.0 : 0.0,
           secondsSoa > 0.0 ? (double)STAT_COUNT / secondsSoa / 1000000.0 : 0.0);
}

/* Quality (share of ones, uniformity of bytes) and speed of all algorithms. */
static void Statistics(IEcoInterfaceBus1* pIBus) {
    uint16_t algo = 0;

    printf("=== Statistics: %lu values of the object mode, seed 0x2545F491, default polynomial ===\n", STAT_COUNT);
    printf("ones = share of one bits (ideal 0.5), chi2 over 256 byte values (ideal about 255)\n");
    for (algo = 0; algo < ALGO_COUNT; algo++) {
        StatisticsOne(pIBus, algo, 32);
        StatisticsOne(pIBus, algo, 64);
    }
    printf("\n");
}

#define CMP_COUNT 1000000UL

/* Quality figures over the low 24 bits of a value: share of ones and byte uniformity of bytes 0 and 1. */
typedef struct CmpAcc {
    unsigned long ones;
    unsigned long b0[256];
    unsigned long b1[256];
} CmpAcc;

static void CmpAccAdd(CmpAcc* acc, uint32_t v) {
    acc->ones += PopCount32(v & 0xFFFFFFUL);
    acc->b0[v & 0xFF]++;
    acc->b1[(v >> 8) & 0xFF]++;
}

static double CmpChi2(const unsigned long* bins) {
    double expected = (double)CMP_COUNT / 256.0;
    double chi = 0.0;
    double diff = 0.0;
    unsigned int k = 0;

    for (k = 0; k < 256; k++) {
        diff = (double)bins[k] - expected;
        chi += diff * diff / expected;
    }
    return chi;
}

static void CmpPrint(const char* name, const CmpAcc* acc, double seconds) {
    printf("%-26s ones24 %.5f  chi2(byte0) %7.1f  chi2(byte1) %7.1f  %7.2f Mvalues/s\n", name,
           (double)acc->ones / ((double)CMP_COUNT * 24.0), CmpChi2(acc->b0), CmpChi2(acc->b1),
           seconds > 0.0 ? (double)CMP_COUNT / seconds / 1000000.0 : 0.0);
}

/* libc generators: kind 0 = rand, 1 = random, 2 = lrand48 (all return 31 bit values). */
static unsigned long CmpLibcNext(int kind) {
    if (kind == 0) {
        return (unsigned long)rand();
    }
    if (kind == 1) {
        return (unsigned long)random();
    }
    return (unsigned long)lrand48();
}

static void CmpLibc(const char* name, int kind) {
    CmpAcc acc;
    unsigned long i = 0;
    unsigned long sink = 0;
    unsigned int k = 0;
    clock_t start = 0;
    clock_t stop = 0;

    acc.ones = 0;
    for (k = 0; k < 256; k++) {
        acc.b0[k] = 0;
        acc.b1[k] = 0;
    }
    if (kind == 0) { srand(0x2545F491U); } else if (kind == 1) { srandom(0x2545F491U); } else { srand48(0x2545F491L); }
    for (i = 0; i < CMP_COUNT; i++) {
        CmpAccAdd(&acc, (uint32_t)CmpLibcNext(kind));
    }
    if (kind == 0) { srand(0x2545F491U); } else if (kind == 1) { srandom(0x2545F491U); } else { srand48(0x2545F491L); }
    start = clock();
    for (i = 0; i < CMP_COUNT; i++) {
        sink += CmpLibcNext(kind);
    }
    stop = clock();
    CmpPrint(name, &acc, (double)(stop - start) / (double)CLOCKS_PER_SEC);
    if (sink == 1) {
        printf(" ");
    }
}

static void CmpOurs(IEcoInterfaceBus1* pIBus, uint16_t algo) {
    CmpAcc acc;
    IEcoPseudoRandom1* pObj = 0;
    uint64_t value = 0;
    unsigned long i = 0;
    unsigned int k = 0;
    clock_t start = 0;
    clock_t stop = 0;
    int16_t result = ERR_ECO_SUCCESS;
    char title[40];

    result = pIBus->pVTbl->QueryComponent(pIBus, g_Algos[algo].cid, 0, &IID_IEcoPseudoRandom1, (void**)&pObj);
    if (result != ERR_ECO_SUCCESS || pObj == 0) {
        return;
    }
    acc.ones = 0;
    for (k = 0; k < 256; k++) {
        acc.b0[k] = 0;
        acc.b1[k] = 0;
    }
    result = pObj->pVTbl->Init(pObj, (uint64_t)0x2545F491UL, 0, 32);
    for (i = 0; i < CMP_COUNT && result == ERR_ECO_SUCCESS; i++) {
        result = pObj->pVTbl->Next(pObj, &value);
        CmpAccAdd(&acc, (uint32_t)(value & 0xFFFFFFFFUL));
    }
    result = pObj->pVTbl->Init(pObj, (uint64_t)0x2545F491UL, 0, 32);
    start = clock();
    for (i = 0; i < CMP_COUNT && result == ERR_ECO_SUCCESS; i++) {
        result = pObj->pVTbl->Next(pObj, &value);
    }
    stop = clock();
    pObj->pVTbl->Release(pObj);
    k = 0;
    while (g_Algos[algo].name[k] != 0 && k < 28) {
        title[k] = g_Algos[algo].name[k];
        k++;
    }
    title[k] = 0;
    if (result == ERR_ECO_SUCCESS) {
        CmpPrint(title, &acc, (double)(stop - start) / (double)CLOCKS_PER_SEC);
    }
}

/* Comparison with the C standard library generators on identical tests (low 24 bits, 1 000 000 values). */
static void ComparisonWithLibc(IEcoInterfaceBus1* pIBus) {
    uint16_t algo = 0;

    printf("=== Comparison with libc generators: %lu values, seed 0x2545F491, low 24 bits ===\n", CMP_COUNT);
    printf("ones24 = share of ones in the low 24 bits (ideal 0.5), chi2 per byte (ideal about 255)\n");
    for (algo = 0; algo < ALGO_COUNT; algo++) {
        CmpOurs(pIBus, algo);
    }
    CmpLibc("libc rand()", 0);
    CmpLibc("libc random()", 1);
    CmpLibc("libc lrand48()", 2);
    printf("\n");
}

static void InteractiveMenu(IEcoInterfaceBus1* pIBus) {
    uint64_t values[64];
    char text[64];
    int algoChoice = 0;
    int modeChoice = 0;
    int bitsChoice = 0;
    int count = 0;
    uint64_t seed = 0;
    uint64_t param = 0;
    int16_t result = ERR_ECO_SUCCESS;

    for (;;) {
        printf("Algorithm: 1) Fibonacci LFSR  2) Galois LFSR  3) Xorshift  8) comparison with libc  9) statistics  0) exit > ");
        if (scanf("%d", &algoChoice) != 1 || algoChoice == 0) {
            break;
        }
        if (algoChoice == 8) {
            ComparisonWithLibc(pIBus);
            continue;
        }
        if (algoChoice == 9) {
            Statistics(pIBus);
            continue;
        }
        if (algoChoice < 1 || algoChoice > ALGO_COUNT) {
            printf("Unknown algorithm\n");
            continue;
        }
        printf("Mode: 1) service SOA  2) object OOA > ");
        if (scanf("%d", &modeChoice) != 1) {
            break;
        }
        printf("Register width, 32 or 64 > ");
        if (scanf("%d", &bitsChoice) != 1) {
            break;
        }
        printf("Seed (hex, non-zero) > ");
        if (scanf("%63s", text) != 1) {
            break;
        }
        seed = ParseHex(text);
        printf("Polynomial mask (hex, 0 = default, ignored by Xorshift) > ");
        if (scanf("%63s", text) != 1) {
            break;
        }
        param = ParseHex(text);
        printf("How many values (1..64) > ");
        if (scanf("%d", &count) != 1) {
            break;
        }
        if (count < 1 || count > 64) {
            printf("Wrong count\n");
            continue;
        }
        if (modeChoice == 1) {
            result = GenerateService(pIBus, (uint16_t)(algoChoice - 1), (uint16_t)bitsChoice, seed, param, (uint16_t)count, values);
        } else {
            result = GenerateObject(pIBus, (uint16_t)(algoChoice - 1), (uint16_t)bitsChoice, seed, param, (uint16_t)count, values);
        }
        if (result != ERR_ECO_SUCCESS) {
            printf("Component returned error %d\n", (int)result);
            continue;
        }
        PrintSequence(modeChoice == 1 ? "SOA" : "OOA", (uint16_t)bitsChoice, values, (uint16_t)count);
    }
}

int16_t EcoMain(IEcoUnknown* pIUnk) {
    int16_t result = -1;
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;

    result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem, (void **)&pISys);
    if (result != 0 || pISys == 0) {
        goto Release;
    }
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    if (result != 0 || pIBus == 0) {
        goto Release;
    }
    result = RegisterAlgorithms(pIBus);
    if (result != ERR_ECO_SUCCESS) {
        goto Release;
    }

    result = Demonstration(pIBus);
    InteractiveMenu(pIBus);

Release:
    if (pIBus != 0) {
        pIBus->pVTbl->Release(pIBus);
    }
    if (pISys != 0) {
        pISys->pVTbl->Release(pISys);
    }
    return result;
}
