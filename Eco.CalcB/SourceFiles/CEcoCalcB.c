/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CEcoCalcB_6A5670F9
 * </summary>
 *
 * <description>
 *   This source code describes the implementation of the interfaces for CEcoCalcB_6A5670F9 (Calculator B)
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
#include "CEcoCalcB.h"

/*
 *
 * <summary>
 *   Delegating IEcoUnknown (IEcoCalculatorX)
 * </summary>
 *
 * <description>
 *   The IEcoUnknown methods of the calculator interface always delegate to the controlling unknown:
 *   to the outer component when the component is aggregated, otherwise to its own non-delegating IEcoUnknown.
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoCalcB_6A5670F9_X_QueryInterface(/* in */ IEcoCalculatorXPtr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv) {
    CEcoCalcB_6A5670F9* pCMe = (CEcoCalcB_6A5670F9*)me;

    if (me == 0 || riid == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }
    return pCMe->m_pIUnkOuter->pVTbl->QueryInterface(pCMe->m_pIUnkOuter, riid, ppv);
}

static uint32_t ECOCALLMETHOD CEcoCalcB_6A5670F9_X_AddRef(/* in */ IEcoCalculatorXPtr_t me) {
    CEcoCalcB_6A5670F9* pCMe = (CEcoCalcB_6A5670F9*)me;

    if (me == 0) {
        return (uint32_t)-1; /* ERR_ECO_POINTER */
    }
    return pCMe->m_pIUnkOuter->pVTbl->AddRef(pCMe->m_pIUnkOuter);
}

static uint32_t ECOCALLMETHOD CEcoCalcB_6A5670F9_X_Release(/* in */ IEcoCalculatorXPtr_t me) {
    CEcoCalcB_6A5670F9* pCMe = (CEcoCalcB_6A5670F9*)me;

    if (me == 0) {
        return (uint32_t)-1; /* ERR_ECO_POINTER */
    }
    return pCMe->m_pIUnkOuter->pVTbl->Release(pCMe->m_pIUnkOuter);
}

/*
 *
 * <summary>
 *   Non-delegating IEcoUnknown
 * </summary>
 *
 * <description>
 *   Real reference counting and interface lookup of the component itself (interface 1 of the object).
 *   QueryInterface of any interface other than IEcoUnknown adds the reference through the controlling unknown.
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoCalcB_6A5670F9_Nondelegating_QueryInterface(/* in */ IEcoUnknownPtr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv) {
    CEcoCalcB_6A5670F9* pCMe = 0;

    if (me == 0 || riid == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }
    pCMe = (CEcoCalcB_6A5670F9*)((char_t*)me - 1 * sizeof(voidptr_t));

    if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &pCMe->m_pVTblINondelegatingUnk;
        ++pCMe->m_cRef;
    }
    else if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        *ppv = &pCMe->m_pVTblIEcoCalculatorX;
        pCMe->m_pIUnkOuter->pVTbl->AddRef(pCMe->m_pIUnkOuter);
    }
    else {
        *ppv = 0;
        return ERR_ECO_NOINTERFACE;
    }
    return ERR_ECO_SUCCESS;
}

static uint32_t ECOCALLMETHOD CEcoCalcB_6A5670F9_Nondelegating_AddRef(/* in */ IEcoUnknownPtr_t me) {
    CEcoCalcB_6A5670F9* pCMe = 0;

    if (me == 0) {
        return (uint32_t)-1; /* ERR_ECO_POINTER */
    }
    pCMe = (CEcoCalcB_6A5670F9*)((char_t*)me - 1 * sizeof(voidptr_t));
    return ++pCMe->m_cRef;
}

static uint32_t ECOCALLMETHOD CEcoCalcB_6A5670F9_Nondelegating_Release(/* in */ IEcoUnknownPtr_t me) {
    CEcoCalcB_6A5670F9* pCMe = 0;

    if (me == 0) {
        return (uint32_t)-1; /* ERR_ECO_POINTER */
    }
    pCMe = (CEcoCalcB_6A5670F9*)((char_t*)me - 1 * sizeof(voidptr_t));
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
static int32_t ECOCALLMETHOD CEcoCalcB_6A5670F9_Addition(/* in */ IEcoCalculatorXPtr_t me, /* in */ int16_t a, /* in */ int16_t b) {
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
 *   On overflow of the 16 bit result the value wraps modulo 65536 (two's complement).
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoCalcB_6A5670F9_Subtraction(/* in */ IEcoCalculatorXPtr_t me, /* in */ int16_t a, /* in */ int16_t b) {
    int32_t diff = 0;

    if (me == 0) {
        return 0;
    }
    diff = (int32_t)a - (int32_t)b;
    if (diff < -32768 || diff > 32767) {
        diff = (int32_t)(int16_t)((uint16_t)((uint16_t)a - (uint16_t)b));
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
static int16_t ECOCALLMETHOD initCEcoCalcB_6A5670F9(/*in*/ CEcoCalcB_6A5670F9Ptr_t me, /* in */ IEcoUnknownPtr_t pIUnkSystem) {
    CEcoCalcB_6A5670F9* pCMe = (CEcoCalcB_6A5670F9*)me;
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
 *   The component supports aggregation: an outer IEcoUnknown becomes the controlling unknown, otherwise the component controls itself.
 * </description>
 *
 */
static int16_t ECOCALLMETHOD createCEcoCalcB_6A5670F9(/* in */ CEcoCalcB_6A5670F9Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter) {

    /* Pointer Validation */
    if (pCMe == 0) {
        return ERR_ECO_POINTER;
    }
    if (pIUnkOuter != 0) {
        pCMe->m_pIUnkOuter = pIUnkOuter;
    }
    else {
        pCMe->m_pIUnkOuter = (IEcoUnknown*)&pCMe->m_pVTblINondelegatingUnk;
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
static void ECOCALLMETHOD deleteCEcoCalcB_6A5670F9(/* in */ CEcoCalcB_6A5670F9Ptr_t pCMe) {
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
IEcoCalculatorXVTbl g_xIEcoCalculatorXVTbl_6A5670F9 = {
    CEcoCalcB_6A5670F9_X_QueryInterface,
    CEcoCalcB_6A5670F9_X_AddRef,
    CEcoCalcB_6A5670F9_X_Release,
    CEcoCalcB_6A5670F9_Addition,
    CEcoCalcB_6A5670F9_Subtraction
};

/* Non-delegating IEcoUnknown Virtual Table */
IEcoUnknownVTbl g_xINondelegatingUnkVTbl_6A5670F9 = {
    CEcoCalcB_6A5670F9_Nondelegating_QueryInterface,
    CEcoCalcB_6A5670F9_Nondelegating_AddRef,
    CEcoCalcB_6A5670F9_Nondelegating_Release
};

/* Object Instance */
CEcoCalcB_6A5670F9 g_xCEcoCalcB_6A5670F9 = {
    &g_xIEcoCalculatorXVTbl_6A5670F9,
    &g_xINondelegatingUnkVTbl_6A5670F9,

    initCEcoCalcB_6A5670F9,
    createCEcoCalcB_6A5670F9,
    deleteCEcoCalcB_6A5670F9,
    1, /* m_cRef */
    0, /* m_pIMem */
    0, /* m_pISys */
    0, /* m_Name */
    0  /* m_pIUnkOuter */
};
