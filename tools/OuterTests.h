/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   OuterTests
 * </summary>
 *
 * <description>
 *   Unit tests of an outer component (Eco.Lab1Outer or Eco.Lab2): containment and aggregation of the calculator
 *   components A-E, the lab 1 generators called through interface pointers of the calculators, the QueryInterface
 *   rules for every pair of interfaces, balanced reference counting and aggregation of the outer component itself.
 *   The component is described by OuterConfig.h. A test function returns 0 on success or the 1-based index of the
 *   first failed check.
 * </description>
 *
 * <author>
 *   Copyright (c) 2026 . All rights reserved.
 * </author>
 *
 */

#ifndef __OUTERTESTS_H__
#define __OUTERTESTS_H__

#include "IEcoBase1.h"
#include "ErrEcoCodes.h"
#include "IEcoSystem1.h"
#include "IdEcoInterfaceBus1.h"
#include "IEcoCalculatorX.h"
#include "IEcoCalculatorY.h"
#include "IEcoPseudoRandomService1.h"
#include "IEcoPseudoRandom1.h"
#include "IEcoLab1Outer.h"
#include "OuterConfig.h"

static const UGUID IID_OuterUnsupported = {0x01, 0x10, {0xDE, 0xAD, 0xBE, 0xEF, 0x00, 0x00, 0x40, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03}};
static unsigned long g_OuterChecks = 0;
#define OCK(cond) do { ++n; ++g_OuterChecks; if (!(cond)) { return n; } } while (0)

typedef struct OuterMock {
    IEcoUnknownVTbl* pVTbl;
    unsigned long qi;
    unsigned long addref;
    unsigned long release;
} OuterMock;

static int16_t ECOCALLMETHOD OuterMockQI(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    OuterMock* pMock = (OuterMock*)me;

    pMock->qi++;
    if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = me;
        pMock->addref++;
        return ERR_ECO_SUCCESS;
    }
    *ppv = 0;
    return ERR_ECO_NOINTERFACE;
}

static uint32_t ECOCALLMETHOD OuterMockAddRef(IEcoUnknownPtr_t me) {
    OuterMock* pMock = (OuterMock*)me;

    pMock->addref++;
    return (uint32_t)pMock->addref;
}

static uint32_t ECOCALLMETHOD OuterMockRelease(IEcoUnknownPtr_t me) {
    OuterMock* pMock = (OuterMock*)me;

    pMock->release++;
    return (uint32_t)pMock->release;
}

static IEcoUnknownVTbl g_OuterMockVTbl = { OuterMockQI, OuterMockAddRef, OuterMockRelease };

/* Any interface pointer starts with the same three IEcoUnknown methods: QueryInterface can be called on any of them. */
#define OUNK(p) ((IEcoUnknown*)(p))

/* Operations of the calculator pointers must be correct and identify the inner component through its policy. */
static int OuterCheckX(IEcoCalculatorX* p, uint16_t kind) {
    long plus = (kind == ECO_CALC_X_A) ? 32767L : ((kind == ECO_CALC_X_B) ? -32768L : 0L);
    long minus = (kind == ECO_CALC_X_A) ? -32768L : ((kind == ECO_CALC_X_B) ? 32767L : 0L);

    if (p->pVTbl->Addition(p, 3, 5) != 8) { return 1; }
    if (p->pVTbl->Addition(p, -32768, -32768) != -65536L) { return 2; }
    if (p->pVTbl->Subtraction(p, 5, 3) != 2) { return 3; }
    if (p->pVTbl->Subtraction(p, 32767, -1) != plus) { return 4; }
    if (p->pVTbl->Subtraction(p, -32768, 1) != minus) { return 5; }
    return 0;
}

static int OuterCheckY(IEcoCalculatorY* p, uint16_t kind) {
    long div0 = (kind == ECO_CALC_Y_D) ? 0L : ((kind == ECO_CALC_Y_E) ? 32767L : -1L);

    if (p->pVTbl->Multiplication(p, 3, 5) != 15) { return 1; }
    if (p->pVTbl->Multiplication(p, -32768, -32768) != 1073741824L) { return 2; }
    if (p->pVTbl->Division(p, 15, 3) != 5) { return 3; }
    if (p->pVTbl->Division(p, -7, 2) != -3) { return 4; }
    if (p->pVTbl->Division(p, 1, 0) != div0) { return 5; }
    return 0;
}

/* Interfaces of the outer component for the QueryInterface matrix. */
#define OUTER_MAX_IFACES 8
typedef struct OuterIfaceSet {
    const UGUID* iid[OUTER_MAX_IFACES];
    voidptr_t ptr[OUTER_MAX_IFACES];
    int count;
} OuterIfaceSet;

static void OuterFillIids(OuterIfaceSet* set) {
    set->count = 0;
    set->iid[set->count++] = &IID_IEcoUnknown;
    set->iid[set->count++] = &OUTER_OWN_IID;
    set->iid[set->count++] = &IID_IEcoCalculatorX;
    set->iid[set->count++] = &IID_IEcoCalculatorY;
    set->iid[set->count++] = &IID_IEcoPseudoRandomService1;
    set->iid[set->count++] = &IID_IEcoPseudoRandom1;
#if OUTER_IS_LAB2
    set->iid[set->count++] = &IID_IEcoLab1Outer;
#endif
}

/*
 * QueryInterface matrix: from every interface pointer every other interface is obtained; the pointer obtained for an
 * IID is always the same, and every interface gives the same IEcoUnknown. The reference counter stays balanced.
 */
static int16_t OuterTestMatrix(IEcoUnknown* pOwn) {
    int16_t n = 0;
    OuterIfaceSet set;
    voidptr_t q = 0;
    voidptr_t u = 0;
    int i = 0;
    int j = 0;
    uint32_t base = 0;
    uint32_t now = 0;

    OuterFillIids(&set);
    for (j = 0; j < set.count; j++) {
        OCK(pOwn->pVTbl->QueryInterface(pOwn, set.iid[j], &set.ptr[j]) == ERR_ECO_SUCCESS);
        OCK(set.ptr[j] != 0);
    }
    for (i = 0; i < set.count; i++) {
        /* one IEcoUnknown whatever interface it is asked from */
        OCK(OUNK(set.ptr[i])->pVTbl->QueryInterface(OUNK(set.ptr[i]), &IID_IEcoUnknown, &u) == ERR_ECO_SUCCESS);
        OCK(u == set.ptr[0]);
        OUNK(u)->pVTbl->Release(OUNK(u));
        for (j = 0; j < set.count; j++) {
            q = 0;
            OCK(OUNK(set.ptr[i])->pVTbl->QueryInterface(OUNK(set.ptr[i]), set.iid[j], &q) == ERR_ECO_SUCCESS);
            OCK(q == set.ptr[j]);
            OUNK(q)->pVTbl->Release(OUNK(q));
        }
        /* an unsupported interface is refused from every pointer */
        q = set.ptr[i];
        OCK(OUNK(set.ptr[i])->pVTbl->QueryInterface(OUNK(set.ptr[i]), &IID_OuterUnsupported, &q) == ERR_ECO_NOINTERFACE);
        OCK(q == 0);
    }
    /* all interfaces share one reference counter */
    base = OUNK(set.ptr[0])->pVTbl->AddRef(OUNK(set.ptr[0]));
    for (i = 1; i < set.count; i++) {
        now = OUNK(set.ptr[i])->pVTbl->AddRef(OUNK(set.ptr[i]));
        OCK(now == base + (uint32_t)i);
    }
    for (i = set.count - 1; i >= 0; i--) {
        now = OUNK(set.ptr[i])->pVTbl->Release(OUNK(set.ptr[i]));
        OCK(now == base + (uint32_t)i - 1);
    }
    for (j = 0; j < set.count; j++) {
        OUNK(set.ptr[j])->pVTbl->Release(OUNK(set.ptr[j]));
    }
    return 0;
}

/* The lab 1 generators are called through interface pointers obtained with QueryInterface from a calculator pointer. */
static int16_t OuterTestGenerators(IEcoUnknown* pOwn, uint16_t kindX) {
    int16_t n = 0;
    IEcoLab1Outer* pL1 = 0;
    IEcoCalculatorX* pX = 0;
    IEcoPseudoRandom1* pObj = 0;
    IEcoPseudoRandomService1* pSvc = 0;
    uint64_t value = 0;
    uint32_t value32 = 0;
    uint16_t g = 0;
    uint32_t expected[3];
    uint64_t seed[3];
    uint64_t param[3];
    char_t* names[3];

    expected[0] = 0x000059C3UL; seed[0] = 0xACE1UL; param[0] = 0xB400UL;  /* Fibonacci LFSR, 16 bit polynomial */
    expected[1] = 0x0000E270UL; seed[1] = 0xACE1UL; param[1] = 0xB400UL;  /* Galois LFSR, 16 bit polynomial */
    expected[2] = 0x00042021UL; seed[2] = 1UL;      param[2] = 0UL;       /* Xorshift32, first value of Marsaglia */

    OCK(pOwn->pVTbl->QueryInterface(pOwn, &IID_IEcoLab1Outer, (voidptr_t*)&pL1) == ERR_ECO_SUCCESS);
    OCK(pOwn->pVTbl->QueryInterface(pOwn, &IID_IEcoCalculatorX, (voidptr_t*)&pX) == ERR_ECO_SUCCESS);
    OCK(OuterCheckX(pX, kindX) == 0);
    /* from the calculator pointer to the generator interfaces of lab 1 */
    OCK(pX->pVTbl->QueryInterface(pX, &IID_IEcoPseudoRandom1, (voidptr_t*)&pObj) == ERR_ECO_SUCCESS);
    OCK(pObj->pVTbl->QueryInterface(pObj, &IID_IEcoPseudoRandomService1, (voidptr_t*)&pSvc) == ERR_ECO_SUCCESS);
    for (g = 0; g < 3; g++) {
        OCK(pL1->pVTbl->SelectGenerator(pL1, g) == ERR_ECO_SUCCESS);
        names[g] = pObj->pVTbl->get_Name(pObj);
        OCK(names[g] != 0 && names[g][0] != 0);
        OCK(pObj->pVTbl->Init(pObj, seed[g], param[g], 32) == ERR_ECO_SUCCESS);
        OCK(pObj->pVTbl->Next(pObj, &value) == ERR_ECO_SUCCESS);
        OCK(value == (uint64_t)expected[g]);
        OCK(pSvc->pVTbl->Step32(pSvc, (uint32_t)seed[g], (uint32_t)param[g], &value32) == ERR_ECO_SUCCESS);
        OCK(value32 == expected[g]);
        OCK(pObj->pVTbl->Init(pObj, 0, 0, 32) == ERR_ECO_INVALIDARG);           /* the contract of lab 1 is preserved */
    }
    OCK(names[0] != names[1] && names[1] != names[2]);
    OCK(pL1->pVTbl->SelectGenerator(pL1, 3) == ERR_ECO_INVALIDARG);
    pSvc->pVTbl->Release(pSvc);
    pObj->pVTbl->Release(pObj);
    pX->pVTbl->Release(pX);
    pL1->pVTbl->Release(pL1);
    return 0;
}

static int16_t OuterTestAll(IEcoInterfaceBus1* pIBus, IEcoUnknown* pISystem) {
    int16_t n = 0;
    int16_t res = 0;
    OUTER_OWN_TYPE* pOwn = 0;
    IEcoUnknown* pOwnU = 0;
    IEcoCalculatorX* pX = 0;
    IEcoCalculatorY* pY = 0;
    IEcoUnknown* pInner = 0;
    IEcoUnknown* pInnerU = 0;
    IEcoUnknown* pBack = 0;
    IEcoUnknown* pIdent = 0;
    uint16_t kx = 0;
    uint16_t ky = 0;
    uint32_t r = 0;
    unsigned long before = 0;
    OuterMock mock;

    /* 1. creation, wrong kinds */
    res = pIBus->pVTbl->QueryComponent(pIBus, OUTER_CID, 0, &OUTER_OWN_IID, (void**)&pOwn);
    OCK(res == ERR_ECO_SUCCESS && pOwn != 0);
    pOwnU = OUNK(pOwn);
    /* identity of the component: the IEcoUnknown that QueryInterface returns, not the cast of an interface pointer (lecture 3) */
    OCK(pOwnU->pVTbl->QueryInterface(pOwnU, &IID_IEcoUnknown, (voidptr_t*)&pIdent) == ERR_ECO_SUCCESS);
    OCK(pOwn->pVTbl->SelectCalculators(pOwn, 3, 0) == ERR_ECO_INVALIDARG);
    OCK(pOwn->pVTbl->SelectCalculators(pOwn, 0, 3) == ERR_ECO_INVALIDARG);
    OCK(pOwn->pVTbl->SelectCalculators(0, 0, 0) == ERR_ECO_POINTER);

    /* 2. every combination of the inner calculator components: containment (A, C, D) and aggregation (B, E) */
    for (kx = 0; kx < 3; kx++) {
        for (ky = 0; ky < 3; ky++) {
            OCK(pOwn->pVTbl->SelectCalculators(pOwn, kx, ky) == ERR_ECO_SUCCESS);
            OCK(pOwnU->pVTbl->QueryInterface(pOwnU, &IID_IEcoCalculatorX, (voidptr_t*)&pX) == ERR_ECO_SUCCESS);
            OCK(pOwnU->pVTbl->QueryInterface(pOwnU, &IID_IEcoCalculatorY, (voidptr_t*)&pY) == ERR_ECO_SUCCESS);
            OCK(OuterCheckX(pX, kx) == 0);
            OCK(OuterCheckY(pY, ky) == 0);
            OCK(pX->pVTbl->Addition(0, 1, 2) == 0);
            /* the pointers belong to the same component whichever inner component serves them */
            OCK(pX->pVTbl->QueryInterface(pX, &IID_IEcoUnknown, (voidptr_t*)&pBack) == ERR_ECO_SUCCESS);
            OCK(pBack == pIdent);
            pBack->pVTbl->Release(pBack);
            OCK(pY->pVTbl->QueryInterface(pY, &IID_IEcoUnknown, (voidptr_t*)&pBack) == ERR_ECO_SUCCESS);
            OCK(pBack == pIdent);
            pBack->pVTbl->Release(pBack);
            /* reference counting through X, Y and the component itself uses one counter */
            r = pX->pVTbl->AddRef(pX);
            OCK(pY->pVTbl->AddRef(pY) == r + 1);
            OCK(pOwnU->pVTbl->AddRef(pOwnU) == r + 2);
            OCK(pOwnU->pVTbl->Release(pOwnU) == r + 1);
            OCK(pY->pVTbl->Release(pY) == r);
            OCK(pX->pVTbl->Release(pX) == r - 1);
            pX->pVTbl->Release(pX);
            pY->pVTbl->Release(pY);
        }
    }

    /* 3. QueryInterface matrix in the default composition and in the all-aggregated composition */
    OCK(pOwn->pVTbl->SelectCalculators(pOwn, ECO_CALC_X_A, ECO_CALC_Y_D) == ERR_ECO_SUCCESS);
    OCK(OuterTestMatrix(pOwnU) == 0);
    OCK(pOwn->pVTbl->SelectCalculators(pOwn, ECO_CALC_X_B, ECO_CALC_Y_E) == ERR_ECO_SUCCESS);
    OCK(OuterTestMatrix(pOwnU) == 0);
    OCK(pOwn->pVTbl->SelectCalculators(pOwn, ECO_CALC_X_C, ECO_CALC_Y_C) == ERR_ECO_SUCCESS);
    OCK(OuterTestMatrix(pOwnU) == 0);

    /* 4. the lab 1 generators are called through the calculator pointers (containment A, aggregation B, containment C) */
    for (kx = 0; kx < 3; kx++) {
        OCK(pOwn->pVTbl->SelectCalculators(pOwn, kx, ECO_CALC_Y_E) == ERR_ECO_SUCCESS);
        OCK(OuterTestGenerators(pOwnU, kx) == 0);
    }

    /* 5. every reference taken above was returned: the last Release destroys the component */
    pIdent->pVTbl->Release(pIdent);
    OCK(pOwnU->pVTbl->Release(pOwnU) == 0);

    /* 6. the outer component itself supports aggregation */
    mock.pVTbl = &g_OuterMockVTbl;
    mock.qi = 0;
    mock.addref = 0;
    mock.release = 0;
    res = pIBus->pVTbl->QueryComponent(pIBus, OUTER_CID, (IEcoUnknown*)&mock, &IID_IEcoUnknown, (void**)&pInner);
    OCK(res == ERR_ECO_SUCCESS && pInner != 0);
    OCK(pInner->pVTbl->QueryInterface(pInner, &IID_IEcoUnknown, (voidptr_t*)&pInnerU) == ERR_ECO_SUCCESS);
    OCK(pInnerU == pInner);
    pInnerU->pVTbl->Release(pInnerU);
    OCK(mock.qi == 0 && mock.addref == 0);
    OCK(pInner->pVTbl->QueryInterface(pInner, &OUTER_OWN_IID, (voidptr_t*)&pOwn) == ERR_ECO_SUCCESS);
    OCK(mock.addref == 1);
    OCK(pOwn->pVTbl->SelectCalculators(pOwn, ECO_CALC_X_B, ECO_CALC_Y_E) == ERR_ECO_SUCCESS);
    OCK(pInner->pVTbl->QueryInterface(pInner, &IID_IEcoCalculatorX, (voidptr_t*)&pX) == ERR_ECO_SUCCESS);
    OCK(OuterCheckX(pX, ECO_CALC_X_B) == 0);
    before = mock.qi;
    OCK(pX->pVTbl->QueryInterface(pX, &IID_IEcoUnknown, (voidptr_t*)&pBack) == ERR_ECO_SUCCESS);
    OCK(mock.qi > before);                      /* the chain of delegating calls ends at the outer component */
    OCK(pBack == (IEcoUnknown*)&mock);
    pBack->pVTbl->Release(pBack);
    pX->pVTbl->Release(pX);
    pOwn->pVTbl->Release(pOwn);
    OCK(mock.addref == 3 && mock.release == 3);   /* every reference given to the outer component was returned */
    OCK(pInner->pVTbl->Release(pInner) == 0);
    return 0;
}

#endif /* __OUTERTESTS_H__ */
