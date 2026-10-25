/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CalcTests
 * </summary>
 *
 * <description>
 *   Unit tests of one calculator component (A-E). The component is described by CalcConfig.h:
 *   CALC_CID, CALC_HAS_X, CALC_HAS_Y, CALC_AGGR and the policy constants. The test checks
 *   the arithmetic, the policy that identifies the component, the QueryInterface rules and the
 *   aggregation behaviour against a mock outer component. A test function returns 0 on success
 *   or the 1-based index of the first failed check.
 * </description>
 *
 * <author>
 *   Copyright (c) 2026 . All rights reserved.
 * </author>
 *
 */

#ifndef __CALCTESTS_H__
#define __CALCTESTS_H__

#include "IEcoBase1.h"
#include "ErrEcoCodes.h"
#include "IEcoSystem1.h"
#include "IdEcoInterfaceBus1.h"
#include "IEcoCalculatorX.h"
#include "IEcoCalculatorY.h"
#include "CalcConfig.h"

static const UGUID IID_CalcUnsupported = {0x01, 0x10, {0xDE, 0xAD, 0xBE, 0xEF, 0x00, 0x00, 0x40, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02}};
static unsigned long g_CalcChecks = 0;
static int16_t g_CalcAggResult = 0; /* result of requesting the component with an outer unknown */
#define CK(cond) do { ++n; ++g_CalcChecks; if (!(cond)) { return n; } } while (0)

/* Mock outer component: counts the calls that an aggregated component delegates to it. */
typedef struct CalcMockOuter {
    IEcoUnknownVTbl* pVTbl;
    unsigned long qi;
    unsigned long addref;
    unsigned long release;
} CalcMockOuter;

static int16_t ECOCALLMETHOD CalcMockQueryInterface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    CalcMockOuter* pMock = (CalcMockOuter*)me;

    pMock->qi++;
    if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = me;
        pMock->addref++;
        return ERR_ECO_SUCCESS;
    }
    *ppv = 0;
    return ERR_ECO_NOINTERFACE;
}

static uint32_t ECOCALLMETHOD CalcMockAddRef(IEcoUnknownPtr_t me) {
    CalcMockOuter* pMock = (CalcMockOuter*)me;

    pMock->addref++;
    return (uint32_t)pMock->addref;
}

static uint32_t ECOCALLMETHOD CalcMockRelease(IEcoUnknownPtr_t me) {
    CalcMockOuter* pMock = (CalcMockOuter*)me;

    pMock->release++;
    return (uint32_t)pMock->release;
}

static IEcoUnknownVTbl g_CalcMockVTbl = { CalcMockQueryInterface, CalcMockAddRef, CalcMockRelease };

#if CALC_HAS_X
/* returns 0 when every operation of the IEcoCalculatorX pointer is correct */
static int CalcCheckX(IEcoCalculatorX* p) {
    if (p->pVTbl->Addition(p, 3, 5) != 8) { return 1; }
    if (p->pVTbl->Addition(p, -32768, -32768) != -65536L) { return 2; }
    if (p->pVTbl->Addition(p, 32767, 32767) != 65534L) { return 3; }
    if (p->pVTbl->Subtraction(p, 5, 3) != 2) { return 4; }
    if (p->pVTbl->Subtraction(p, -7, -9) != 2) { return 5; }
    if (p->pVTbl->Subtraction(p, 32767, -1) != CALC_SUB_OVF_PLUS) { return 6; }
    if (p->pVTbl->Subtraction(p, -32768, 1) != CALC_SUB_OVF_MINUS) { return 7; }
    return 0;
}
#endif

#if CALC_HAS_Y
/* returns 0 when every operation of the IEcoCalculatorY pointer is correct */
static int CalcCheckY(IEcoCalculatorY* p) {
    if (p->pVTbl->Multiplication(p, 3, 5) != 15) { return 1; }
    if (p->pVTbl->Multiplication(p, -32768, -32768) != 1073741824L) { return 2; }
    if (p->pVTbl->Multiplication(p, -4, 6) != -24) { return 3; }
    if (p->pVTbl->Division(p, 15, 3) != 5) { return 4; }
    if (p->pVTbl->Division(p, -15, 3) != -5) { return 5; }
    if (p->pVTbl->Division(p, 7, 2) != 3) { return 6; }
    if (p->pVTbl->Division(p, -7, 2) != -3) { return 7; }
    if (p->pVTbl->Division(p, -32768, -1) != 32767) { return 8; }
    if (p->pVTbl->Division(p, 1, 0) != CALC_DIV0) { return 9; }
    return 0;
}
#endif

static int16_t CalcTestAll(IEcoInterfaceBus1* pIBus, IEcoUnknown* pISystem) {
    int16_t n = 0;
    int16_t res = 0;
    IEcoUnknown* pU1 = 0;
    IEcoUnknown* pU2 = 0;
    IEcoUnknown* pInner = 0;
    IEcoUnknown* pInnerU = 0;
    IEcoUnknown* pOuterBack = 0;
    voidptr_t pBad = 0;
    uint32_t r1 = 0;
    uint32_t r2 = 0;
    unsigned long before = 0;
    CalcMockOuter mock;
#if CALC_HAS_X
    IEcoCalculatorX* pX = 0;
    IEcoCalculatorX* pXa = 0;
    IEcoCalculatorX* pX2 = 0;
#endif
#if CALC_HAS_Y
    IEcoCalculatorY* pY = 0;
    IEcoCalculatorY* pYa = 0;
    IEcoCalculatorY* pY2 = 0;
#endif

    /* 1. creation through the bus and the arithmetic */
#if CALC_HAS_X
    res = pIBus->pVTbl->QueryComponent(pIBus, CALC_CID, 0, &IID_IEcoCalculatorX, (void**)&pX);
    CK(res == ERR_ECO_SUCCESS && pX != 0);
    CK(CalcCheckX(pX) == 0);
    CK(pX->pVTbl->Addition(0, 1, 2) == 0);
    CK(pX->pVTbl->Subtraction(0, 1, 2) == 0);
#endif
#if CALC_HAS_Y
#if CALC_HAS_X
    /* the second interface of the SAME instance comes from QueryInterface, not from a second creation */
    res = pX->pVTbl->QueryInterface(pX, &IID_IEcoCalculatorY, (voidptr_t*)&pY);
#else
    res = pIBus->pVTbl->QueryComponent(pIBus, CALC_CID, 0, &IID_IEcoCalculatorY, (void**)&pY);
#endif
    CK(res == ERR_ECO_SUCCESS && pY != 0);
    CK(CalcCheckY(pY) == 0);
    CK(pY->pVTbl->Multiplication(0, 1, 2) == 0);
    CK(pY->pVTbl->Division(0, 1, 2) == 0);
#endif

    /* 2. QueryInterface rules (lecture 3): one IEcoUnknown, return to the start, any interface from any */
#if CALC_HAS_X
    CK(pX->pVTbl->QueryInterface(pX, &IID_IEcoUnknown, (voidptr_t*)&pU1) == ERR_ECO_SUCCESS);
    CK(pX->pVTbl->QueryInterface(pX, &IID_IEcoCalculatorX, (voidptr_t*)&pX2) == ERR_ECO_SUCCESS);
    CK(pX2 == pX);
    pX2->pVTbl->Release(pX2);
    CK(pX->pVTbl->QueryInterface(pX, &IID_CalcUnsupported, &pBad) == ERR_ECO_NOINTERFACE);
    CK(pBad == 0);
    CK(pX->pVTbl->QueryInterface(pX, &IID_IEcoUnknown, 0) == ERR_ECO_POINTER);
    CK(pX->pVTbl->QueryInterface(0, &IID_IEcoUnknown, (voidptr_t*)&pU2) == ERR_ECO_POINTER);
    r1 = pX->pVTbl->AddRef(pX);
    r2 = pX->pVTbl->Release(pX);
    CK(r1 == r2 + 1);
#endif
#if CALC_HAS_X && CALC_HAS_Y
    CK(pY->pVTbl->QueryInterface(pY, &IID_IEcoUnknown, (voidptr_t*)&pU2) == ERR_ECO_SUCCESS);
    CK(pU1 == pU2);
    CK(pU1->pVTbl->QueryInterface(pU1, &IID_IEcoCalculatorY, (voidptr_t*)&pY2) == ERR_ECO_SUCCESS);
    CK(pY2 == pY);
    CK(pY2->pVTbl->QueryInterface(pY2, &IID_IEcoCalculatorX, (voidptr_t*)&pX2) == ERR_ECO_SUCCESS);
    CK(pX2 == pX);
    pX2->pVTbl->Release(pX2);
    pY2->pVTbl->Release(pY2);
    pU2->pVTbl->Release(pU2);
#endif
#if CALC_HAS_X
    pU1->pVTbl->Release(pU1);
    pX->pVTbl->Release(pX);
#endif
#if CALC_HAS_Y
#if !CALC_HAS_X
    CK(pY->pVTbl->QueryInterface(pY, &IID_IEcoUnknown, (voidptr_t*)&pU1) == ERR_ECO_SUCCESS);
    CK(pY->pVTbl->QueryInterface(pY, &IID_CalcUnsupported, &pBad) == ERR_ECO_NOINTERFACE);
    CK(pBad == 0);
    CK(pY->pVTbl->QueryInterface(pY, &IID_IEcoUnknown, 0) == ERR_ECO_POINTER);
    r1 = pY->pVTbl->AddRef(pY);
    r2 = pY->pVTbl->Release(pY);
    CK(r1 == r2 + 1);
    pU1->pVTbl->Release(pU1);
#endif
    pY->pVTbl->Release(pY);
#endif

    /* 3. aggregation with a mock outer component */
    mock.pVTbl = &g_CalcMockVTbl;
    mock.qi = 0;
    mock.addref = 0;
    mock.release = 0;
    res = pIBus->pVTbl->QueryComponent(pIBus, CALC_CID, (IEcoUnknown*)&mock, &IID_IEcoUnknown, (void**)&pInner);
    g_CalcAggResult = res;
#if CALC_AGGR
    CK(res == ERR_ECO_SUCCESS && pInner != 0);
    /* the pointer is the NON-delegating IEcoUnknown: it answers for itself */
    CK(pInner->pVTbl->QueryInterface(pInner, &IID_IEcoUnknown, (voidptr_t*)&pInnerU) == ERR_ECO_SUCCESS);
    CK(pInnerU == pInner);
    pInnerU->pVTbl->Release(pInnerU);
    CK(mock.qi == 0 && mock.addref == 0);
#if CALC_HAS_X
    CK(pInner->pVTbl->QueryInterface(pInner, &IID_IEcoCalculatorX, (voidptr_t*)&pXa) == ERR_ECO_SUCCESS);
    CK(mock.addref == 1);                       /* the reference went to the controlling unknown */
    CK(CalcCheckX(pXa) == 0);
    before = mock.qi;
    CK(pXa->pVTbl->QueryInterface(pXa, &IID_IEcoUnknown, (voidptr_t*)&pOuterBack) == ERR_ECO_SUCCESS);
    CK(mock.qi == before + 1);                  /* the call was delegated to the outer component */
    CK(pOuterBack == (IEcoUnknown*)&mock);
    pOuterBack->pVTbl->Release(pOuterBack);
    before = mock.release;
    pXa->pVTbl->Release(pXa);
    CK(mock.release == before + 1);
#endif
#if CALC_HAS_Y
    CK(pInner->pVTbl->QueryInterface(pInner, &IID_IEcoCalculatorY, (voidptr_t*)&pYa) == ERR_ECO_SUCCESS);
    CK(mock.addref == 1);
    CK(CalcCheckY(pYa) == 0);
    before = mock.qi;
    CK(pYa->pVTbl->QueryInterface(pYa, &IID_IEcoUnknown, (voidptr_t*)&pOuterBack) == ERR_ECO_SUCCESS);
    CK(mock.qi == before + 1);
    CK(pOuterBack == (IEcoUnknown*)&mock);
    pOuterBack->pVTbl->Release(pOuterBack);
    before = mock.release;
    pYa->pVTbl->Release(pYa);
    CK(mock.release == before + 1);
#endif
    /* releasing the non-delegating reference destroys the aggregated component */
    CK(pInner->pVTbl->Release(pInner) == 0);
#else
    /* a component without aggregation support refuses an outer unknown: the bus reports the failed creation ... */
    CK(res != ERR_ECO_SUCCESS);
    CK(pInner == 0);
    CK(mock.qi == 0 && mock.addref == 0 && mock.release == 0);
#ifdef ECO_LIB
    /* ... and the component factory itself answers ERR_ECO_NOAGGREGATION */
    res = CALC_FACTORY->pVTbl->Alloc(CALC_FACTORY, pISystem, (IEcoUnknown*)&mock, &IID_IEcoUnknown, (void**)&pInner);
    CK(res == ERR_ECO_NOAGGREGATION);
    CK(pInner == 0);
    CK(mock.qi == 0 && mock.addref == 0 && mock.release == 0);
#endif
#endif
    return 0;
}

#endif /* __CALCTESTS_H__ */
