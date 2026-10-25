/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CEcoCalcA_06878ABA
 * </summary>
 *
 * <description>
 *   This source code describes the implementation of the interfaces for CEcoCalcA_06878ABA (Calculator A)
 * </description>
 *
 * <author>
 *   Copyright (c) 2026 . All rights reserved.
 * </author>
 *
 */


#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "IEcoInterfaceBus1MemExt.h"
#include "CEcoCalcA.h"

/*
 *
 * <summary>
 *   QueryInterface Function
 * </summary>
 *
 * <description>
 *   Common QueryInterface of all interfaces of the component: every interface can return every other one.
 * </description>
 *
 */
static int16_t CEcoCalcA_06878ABA_QueryCommon(/* in */ CEcoCalcA_06878ABA* pCMe, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv) {
    if (pCMe == 0 || riid == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        *ppv = &pCMe->m_pVTblIEcoCalculatorX;
        ++pCMe->m_cRef;
    }
    else if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &pCMe->m_pVTblIEcoCalculatorX;
        ++pCMe->m_cRef;
    }
    else {
        *ppv = 0;
        return ERR_ECO_NOINTERFACE;
    }
    return ERR_ECO_SUCCESS;
}

/*
 *
 * <summary>
 *   QueryInterface Function (IEcoCalculatorX)
 * </summary>
 *
 * <description>
 *   QueryInterface function for the IEcoCalculatorX interface
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoCalcA_06878ABA_X_QueryInterface(/* in */ IEcoCalculatorXPtr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv) {
    if (me == 0) {
        return ERR_ECO_POINTER;
    }
    return CEcoCalcA_06878ABA_QueryCommon((CEcoCalcA_06878ABA*)me, riid, ppv);
}

/*
 *
 * <summary>
 *   AddRef Function (IEcoCalculatorX)
 * </summary>
 *
 * <description>
 *   AddRef function for the IEcoCalculatorX interface (one counter for all interfaces)
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoCalcA_06878ABA_X_AddRef(/* in */ IEcoCalculatorXPtr_t me) {
    CEcoCalcA_06878ABA* pCMe = 0;

    if (me == 0) {
        return (uint32_t)-1; /* ERR_ECO_POINTER */
    }
    pCMe = (CEcoCalcA_06878ABA*)me;
    return ++pCMe->m_cRef;
}

/*
 *
 * <summary>
 *   Release Function (IEcoCalculatorX)
 * </summary>
 *
 * <description>
 *   Release function for the IEcoCalculatorX interface (one counter for all interfaces)
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoCalcA_06878ABA_X_Release(/* in */ IEcoCalculatorXPtr_t me) {
    CEcoCalcA_06878ABA* pCMe = 0;

    if (me == 0) {
        return (uint32_t)-1; /* ERR_ECO_POINTER */
    }
    pCMe = (CEcoCalcA_06878ABA*)me;
    --pCMe->m_cRef;
    if (pCMe->m_cRef == 0) {
        pCMe->Delete(pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

/*
 *
 * <summary>
 *   Addition Function
 * </summary>
 *
 * <description>
 *   32 bit sum of two 16 bit operands. Returns 0 for me == NULL.
 * </description>
 *
 */
static int32_t ECOCALLMETHOD CEcoCalcA_06878ABA_Addition(/* in */ IEcoCalculatorXPtr_t me, /* in */ int16_t a, /* in */ int16_t b) {
    if (me == 0) {
        return 0;
    }
    return (int32_t)a + (int32_t)b;
}

/*
 *
 * <summary>
 *   Subtraction Function
 * </summary>
 *
 * <description>
 *   16 bit difference a - b. Returns 0 for me == NULL.
 *   On overflow of the 16 bit result the value is saturated to -32768 or 32767.
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoCalcA_06878ABA_Subtraction(/* in */ IEcoCalculatorXPtr_t me, /* in */ int16_t a, /* in */ int16_t b) {
    int32_t diff = 0;

    if (me == 0) {
        return 0;
    }
    diff = (int32_t)a - (int32_t)b;
    if (diff < -32768 || diff > 32767) {
        diff = (diff < 0) ? -32768 : 32767;
    }
    return (int16_t)diff;
}

/*
 *
 * <summary>
 *   Init Function
 * </summary>
 *
 * <description>
 *   Instance initialization function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD initCEcoCalcA_06878ABA(/*in*/ CEcoCalcA_06878ABAPtr_t me, /* in */ IEcoUnknownPtr_t pIUnkSystem) {
    CEcoCalcA_06878ABA* pCMe = (CEcoCalcA_06878ABA*)me;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoInterfaceBus1MemExt* pIMemExt = 0;
    int16_t result = ERR_ECO_POINTER;
    UGUID* rcid = (UGUID*)&CID_EcoMemoryManager1;

    /* Pointer Validation */
    if (me == 0 ) {
        return result;
    }

    /* Storing the pointer to the system interface */
    pCMe->m_pISys = (IEcoSystem1*)pIUnkSystem;

    /* Getting the interface for working with the interface bus */
    result = pCMe->m_pISys->pVTbl->QueryInterface(pCMe->m_pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);

    /* Getting the component ID for working with memory */
    result = pIBus->pVTbl->QueryInterface(pIBus, &IID_IEcoInterfaceBus1MemExt, (void**)&pIMemExt);
    if (result == 0 && pIMemExt != 0) {
        rcid = (UGUID*)pIMemExt->pVTbl->get_Manager(pIMemExt);
        pIMemExt->pVTbl->Release(pIMemExt);
    }

    /* Getting the memory allocator interface */
    result = pIBus->pVTbl->QueryComponent(pIBus, rcid, 0, &IID_IEcoMemoryAllocator1, (void**) &pCMe->m_pIMem);
    /* Check */
    if (result != 0 || pCMe->m_pIMem == 0) {
        result = ERR_ECO_GET_MEMORY_ALLOCATOR;
    }

    /* Freeing */
    pIBus->pVTbl->Release(pIBus);

    return result;
}

/*
 *
 * <summary>
 *   Create Function
 * </summary>
 *
 * <description>
 *   Instance creation function
 *   The component supports containment only: an outer IEcoUnknown is refused with ERR_ECO_NOAGGREGATION.
 * </description>
 *
 */
static int16_t ECOCALLMETHOD createCEcoCalcA_06878ABA(/* in */ CEcoCalcA_06878ABAPtr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter) {

    /* Pointer Validation */
    if (pCMe == 0) {
        return ERR_ECO_POINTER;
    }
    if (pIUnkOuter != 0) {
        return ERR_ECO_NOAGGREGATION;
    }
    return ERR_ECO_SUCCESS;
}

/*
 *
 * <summary>
 *   Delete Function
 * </summary>
 *
 * <description>
 *   Instance freeing function
 * </description>
 *
 */
static void ECOCALLMETHOD deleteCEcoCalcA_06878ABA(/* in */ CEcoCalcA_06878ABAPtr_t pCMe) {
    IEcoMemoryAllocator1* pIMem = 0;

    if (pCMe != 0 ) {
        pIMem = pCMe->m_pIMem;
        /* Freeing */
        if ( pCMe->m_Name != 0 ) {
            pIMem->pVTbl->Free(pIMem, pCMe->m_Name);
        }
        if ( pCMe->m_pISys != 0 ) {
            pCMe->m_pISys->pVTbl->Release(pCMe->m_pISys);
        }
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
}

/* IEcoCalculatorX Virtual Table */
IEcoCalculatorXVTbl g_xIEcoCalculatorXVTbl_06878ABA = {
    CEcoCalcA_06878ABA_X_QueryInterface,
    CEcoCalcA_06878ABA_X_AddRef,
    CEcoCalcA_06878ABA_X_Release,
    CEcoCalcA_06878ABA_Addition,
    CEcoCalcA_06878ABA_Subtraction
};

/* Object Instance */
CEcoCalcA_06878ABA g_xCEcoCalcA_06878ABA = {
    &g_xIEcoCalculatorXVTbl_06878ABA,

    initCEcoCalcA_06878ABA,
    createCEcoCalcA_06878ABA,
    deleteCEcoCalcA_06878ABA,
    1, /* m_cRef */
    0, /* m_pIMem */
    0, /* m_pISys */
    0 /* m_Name */
};
