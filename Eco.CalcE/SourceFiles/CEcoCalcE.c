/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CEcoCalcE_8C776B06
 * </summary>
 *
 * <description>
 *   This source code describes the implementation of the interfaces for CEcoCalcE_8C776B06 (Calculator E)
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
#include "CEcoCalcE.h"

/*
 *
 * <summary>
 *   Delegating IEcoUnknown (IEcoCalculatorY)
 * </summary>
 *
 * <description>
 *   The IEcoUnknown methods of the calculator interface always delegate to the controlling unknown:
 *   to the outer component when the component is aggregated, otherwise to its own non-delegating IEcoUnknown.
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoCalcE_8C776B06_Y_QueryInterface(/* in */ IEcoCalculatorYPtr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv) {
    CEcoCalcE_8C776B06* pCMe = (CEcoCalcE_8C776B06*)me;

    if (me == 0 || riid == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }
    return pCMe->m_pIUnkOuter->pVTbl->QueryInterface(pCMe->m_pIUnkOuter, riid, ppv);
}

static uint32_t ECOCALLMETHOD CEcoCalcE_8C776B06_Y_AddRef(/* in */ IEcoCalculatorYPtr_t me) {
    CEcoCalcE_8C776B06* pCMe = (CEcoCalcE_8C776B06*)me;

    if (me == 0) {
        return (uint32_t)-1; /* ERR_ECO_POINTER */
    }
    return pCMe->m_pIUnkOuter->pVTbl->AddRef(pCMe->m_pIUnkOuter);
}

static uint32_t ECOCALLMETHOD CEcoCalcE_8C776B06_Y_Release(/* in */ IEcoCalculatorYPtr_t me) {
    CEcoCalcE_8C776B06* pCMe = (CEcoCalcE_8C776B06*)me;

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
static int16_t ECOCALLMETHOD CEcoCalcE_8C776B06_Nondelegating_QueryInterface(/* in */ IEcoUnknownPtr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv) {
    CEcoCalcE_8C776B06* pCMe = 0;

    if (me == 0 || riid == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }
    pCMe = (CEcoCalcE_8C776B06*)((char_t*)me - 1 * sizeof(voidptr_t));

    if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &pCMe->m_pVTblINondelegatingUnk;
        ++pCMe->m_cRef;
    }
    else if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = &pCMe->m_pVTblIEcoCalculatorY;
        pCMe->m_pIUnkOuter->pVTbl->AddRef(pCMe->m_pIUnkOuter);
    }
    else {
        *ppv = 0;
        return ERR_ECO_NOINTERFACE;
    }
    return ERR_ECO_SUCCESS;
}

static uint32_t ECOCALLMETHOD CEcoCalcE_8C776B06_Nondelegating_AddRef(/* in */ IEcoUnknownPtr_t me) {
    CEcoCalcE_8C776B06* pCMe = 0;

    if (me == 0) {
        return (uint32_t)-1; /* ERR_ECO_POINTER */
    }
    pCMe = (CEcoCalcE_8C776B06*)((char_t*)me - 1 * sizeof(voidptr_t));
    return ++pCMe->m_cRef;
}

static uint32_t ECOCALLMETHOD CEcoCalcE_8C776B06_Nondelegating_Release(/* in */ IEcoUnknownPtr_t me) {
    CEcoCalcE_8C776B06* pCMe = 0;

    if (me == 0) {
        return (uint32_t)-1; /* ERR_ECO_POINTER */
    }
    pCMe = (CEcoCalcE_8C776B06*)((char_t*)me - 1 * sizeof(voidptr_t));
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
 *   Multiplication Function
 * </summary>
 *
 * <description>
 *   32 bit product of two 16 bit operands. Returns 0 for me == NULL.
 * </description>
 *
 */
static int32_t ECOCALLMETHOD CEcoCalcE_8C776B06_Multiplication(/* in */ IEcoCalculatorYPtr_t me, /* in */ int16_t a, /* in */ int16_t b) {
    if (me == 0) {
        return 0;
    }
    return (int32_t)a * (int32_t)b;
}

/*
 *
 * <summary>
 *   Division Function
 * </summary>
 *
 * <description>
 *   16 bit quotient a / b truncated toward zero; the only overflowing quotient (-32768 / -1) is saturated to 32767.
 *   Division by zero returns 32767. Returns 0 for me == NULL.
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoCalcE_8C776B06_Division(/* in */ IEcoCalculatorYPtr_t me, /* in */ int16_t a, /* in */ int16_t b) {
    int32_t quot = 0;

    if (me == 0) {
        return 0;
    }
    if (b == 0) {
        return (int16_t)(32767);
    }
    quot = (int32_t)a / (int32_t)b;
    if (quot > 32767) {
        quot = 32767;
    }
    return (int16_t)quot;
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
static int16_t ECOCALLMETHOD initCEcoCalcE_8C776B06(/*in*/ CEcoCalcE_8C776B06Ptr_t me, /* in */ IEcoUnknownPtr_t pIUnkSystem) {
    CEcoCalcE_8C776B06* pCMe = (CEcoCalcE_8C776B06*)me;
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
static int16_t ECOCALLMETHOD createCEcoCalcE_8C776B06(/* in */ CEcoCalcE_8C776B06Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter) {

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
static void ECOCALLMETHOD deleteCEcoCalcE_8C776B06(/* in */ CEcoCalcE_8C776B06Ptr_t pCMe) {
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

/* IEcoCalculatorY Virtual Table */
IEcoCalculatorYVTbl g_xIEcoCalculatorYVTbl_8C776B06 = {
    CEcoCalcE_8C776B06_Y_QueryInterface,
    CEcoCalcE_8C776B06_Y_AddRef,
    CEcoCalcE_8C776B06_Y_Release,
    CEcoCalcE_8C776B06_Multiplication,
    CEcoCalcE_8C776B06_Division
};

/* Non-delegating IEcoUnknown Virtual Table */
IEcoUnknownVTbl g_xINondelegatingUnkVTbl_8C776B06 = {
    CEcoCalcE_8C776B06_Nondelegating_QueryInterface,
    CEcoCalcE_8C776B06_Nondelegating_AddRef,
    CEcoCalcE_8C776B06_Nondelegating_Release
};

/* Object Instance */
CEcoCalcE_8C776B06 g_xCEcoCalcE_8C776B06 = {
    &g_xIEcoCalculatorYVTbl_8C776B06,
    &g_xINondelegatingUnkVTbl_8C776B06,

    initCEcoCalcE_8C776B06,
    createCEcoCalcE_8C776B06,
    deleteCEcoCalcE_8C776B06,
    1, /* m_cRef */
    0, /* m_pIMem */
    0, /* m_pISys */
    0, /* m_Name */
    0  /* m_pIUnkOuter */
};
