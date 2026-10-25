/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   OuterDemo
 * </summary>
 *
 * <description>
 *   Console demonstration for assignment 2: the client asks the outer component for the calculator interfaces with
 *   QueryInterface (containment and aggregation of A-E), shows that any interface pointer gives any other interface,
 *   and calls the generators of lab 1 through a pointer obtained from a calculator interface.
 *   The demonstration uses libc output; the components do not.
 * </description>
 *
 * <author>
 *   Copyright (c) 2026 . All rights reserved.
 * </author>
 *
 */

#ifndef __OUTERDEMO_H__
#define __OUTERDEMO_H__

#include "OuterTests.h"

static const char* g_DemoKindX[3] = { "A (containment)", "B (aggregation)", "C (containment)" };
static const char* g_DemoKindY[3] = { "D (containment)", "E (aggregation)", "C (containment)" };

static void OuterDemo(IEcoInterfaceBus1* pIBus) {
    OUTER_OWN_TYPE* pOwn = 0;
    IEcoUnknown* pOwnU = 0;
    IEcoCalculatorX* pX = 0;
    IEcoCalculatorY* pY = 0;
    IEcoLab1Outer* pL1 = 0;
    IEcoPseudoRandom1* pObj = 0;
    OuterIfaceSet set;
    const char* names[OUTER_MAX_IFACES];
    voidptr_t q = 0;
    uint16_t kx = 0;
    uint16_t ky = 0;
    uint16_t g = 0;
    int i = 0;
    int j = 0;
    int k = 0;
    int same = 0;
    uint64_t value = 0;
    uint64_t seed[3];
    uint64_t param[3];

    seed[0] = 0xACE1UL; param[0] = 0xB400UL;
    seed[1] = 0xACE1UL; param[1] = 0xB400UL;
    seed[2] = 1UL;      param[2] = 0UL;

    if (pIBus->pVTbl->QueryComponent(pIBus, OUTER_CID, 0, &OUTER_OWN_IID, (void**)&pOwn) != ERR_ECO_SUCCESS || pOwn == 0) {
        printf("Demonstration: cannot create %s\n", OUTER_NAME);
        return;
    }
    pOwnU = OUNK(pOwn);

    printf("=== %s: calculators X and Y requested with QueryInterface, every combination of the inner components ===\n", OUTER_NAME);
    for (kx = 0; kx < 3; kx++) {
        for (ky = 0; ky < 3; ky++) {
            if (pOwn->pVTbl->SelectCalculators(pOwn, kx, ky) != ERR_ECO_SUCCESS) {
                printf("  SelectCalculators(%u, %u) failed\n", (unsigned int)kx, (unsigned int)ky);
                continue;
            }
            pX = 0;
            pY = 0;
            pOwnU->pVTbl->QueryInterface(pOwnU, &IID_IEcoCalculatorX, (voidptr_t*)&pX);
            pOwnU->pVTbl->QueryInterface(pOwnU, &IID_IEcoCalculatorY, (voidptr_t*)&pY);
            if (pX == 0 || pY == 0) {
                printf("  QueryInterface of X or Y failed\n");
                continue;
            }
            printf("  X from %-16s Y from %-16s  3+5=%ld  7-9=%d  (-32768)-1=%d  |  6*7=%ld  17/5=%d  1/0=%d\n",
                   g_DemoKindX[kx], g_DemoKindY[ky],
                   (long)pX->pVTbl->Addition(pX, 3, 5), (int)pX->pVTbl->Subtraction(pX, 7, 9), (int)pX->pVTbl->Subtraction(pX, -32768, 1),
                   (long)pY->pVTbl->Multiplication(pY, 6, 7), (int)pY->pVTbl->Division(pY, 17, 5), (int)pY->pVTbl->Division(pY, 1, 0));
            pX->pVTbl->Release(pX);
            pY->pVTbl->Release(pY);
        }
    }

    printf("\n=== %s: any interface pointer gives any other interface (ok = obtained, the same pointer every time) ===\n", OUTER_NAME);
    OuterFillIids(&set);
    names[0] = "IEcoUnknown";
    names[1] = OUTER_NAME_IFACE;
    names[2] = "IEcoCalculatorX";
    names[3] = "IEcoCalculatorY";
    names[4] = "IEcoPseudoRandomService1";
    names[5] = "IEcoPseudoRandom1";
#if OUTER_IS_LAB2
    names[6] = "IEcoLab1Outer";
#endif
    pOwn->pVTbl->SelectCalculators(pOwn, ECO_CALC_X_B, ECO_CALC_Y_E);
    for (j = 0; j < set.count; j++) {
        pOwnU->pVTbl->QueryInterface(pOwnU, set.iid[j], &set.ptr[j]);
    }
    printf("  %-26s", "from \\ to");
    for (j = 0; j < set.count; j++) {
        printf(" %-4d", j);
    }
    printf("\n");
    for (i = 0; i < set.count; i++) {
        printf("  %d %-24s", i, names[i]);
        for (j = 0; j < set.count; j++) {
            q = 0;
            same = (set.ptr[i] != 0 && OUNK(set.ptr[i])->pVTbl->QueryInterface(OUNK(set.ptr[i]), set.iid[j], &q) == ERR_ECO_SUCCESS && q == set.ptr[j]);
            if (q != 0) {
                OUNK(q)->pVTbl->Release(OUNK(q));
            }
            printf(" %-4s", same ? "ok" : "FAIL");
        }
        printf("\n");
    }
    for (j = 0; j < set.count; j++) {
        if (set.ptr[j] != 0) {
            OUNK(set.ptr[j])->pVTbl->Release(OUNK(set.ptr[j]));
        }
    }
    (void)k;

    printf("\n=== %s: the generators of lab 1 are called through the pointer of calculator X (aggregated component B) ===\n", OUTER_NAME);
    pOwn->pVTbl->SelectCalculators(pOwn, ECO_CALC_X_B, ECO_CALC_Y_E);
    pOwnU->pVTbl->QueryInterface(pOwnU, &IID_IEcoCalculatorX, (voidptr_t*)&pX);
    pOwnU->pVTbl->QueryInterface(pOwnU, &IID_IEcoLab1Outer, (voidptr_t*)&pL1);
    if (pX != 0 && pL1 != 0 && pX->pVTbl->QueryInterface(pX, &IID_IEcoPseudoRandom1, (voidptr_t*)&pObj) == ERR_ECO_SUCCESS && pObj != 0) {
        printf("  3+5 through the same pointer = %ld, then QueryInterface(IEcoPseudoRandom1) from it succeeded\n", (long)pX->pVTbl->Addition(pX, 3, 5));
        for (g = 0; g < 3; g++) {
            pL1->pVTbl->SelectGenerator(pL1, g);
            pObj->pVTbl->Init(pObj, seed[g], param[g], 32);
            printf("  %-24s first values:", pObj->pVTbl->get_Name(pObj));
            for (i = 0; i < 4; i++) {
                value = 0;
                pObj->pVTbl->Next(pObj, &value);
                printf(" %08lX", (unsigned long)(value & 0xFFFFFFFFUL));
            }
            printf("\n");
        }
    } else {
        printf("  QueryInterface chain failed\n");
    }
    if (pObj != 0) {
        pObj->pVTbl->Release(pObj);
    }
    if (pL1 != 0) {
        pL1->pVTbl->Release(pL1);
    }
    if (pX != 0) {
        pX->pVTbl->Release(pX);
    }
    printf("\n");
    pOwnU->pVTbl->Release(pOwnU);
}

#endif /* __OUTERDEMO_H__ */
