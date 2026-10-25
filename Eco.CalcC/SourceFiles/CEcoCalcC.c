/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CEcoCalcC_DE118C94
 * </summary>
 *
 * <description>
 *   This source code describes the implementation of the interfaces for CEcoCalcC_DE118C94 (Calculator C)
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
#include "CEcoCalcC.h"

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
static int16_t CEcoCalcC_DE118C94_QueryCommon(/* in */ CEcoCalcC_DE118C94* pCMe, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv) {
    if (pCMe == 0 || riid == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        *ppv = &pCMe->m_pVTblIEcoCalculatorX;
        ++pCMe->m_cRef;
    }
    else if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = &pCMe->m_pVTblIEcoCalculatorY;
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
static int16_t ECOCALLMETHOD CEcoCalcC_DE118C94_X_QueryInterface(/* in */ IEcoCalculatorXPtr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv) {
    if (me == 0) {
        return ERR_ECO_POINTER;
    }
    return CEcoCalcC_DE118C94_QueryCommon((CEcoCalcC_DE118C94*)me, riid, ppv);
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
static uint32_t ECOCALLMETHOD CEcoCalcC_DE118C94_X_AddRef(/* in */ IEcoCalculatorXPtr_t me) {
    CEcoCalcC_DE118C94* pCMe = 0;

    if (me == 0) {
        return (uint32_t)-1; /* ERR_ECO_POINTER */
    }
    pCMe = (CEcoCalcC_DE118C94*)me;
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
static uint32_t ECOCALLMETHOD CEcoCalcC_DE118C94_X_Release(/* in */ IEcoCalculatorXPtr_t me) {
    CEcoCalcC_DE118C94* pCMe = 0;

    if (me == 0) {
        return (uint32_t)-1; /* ERR_ECO_POINTER */
    }
    pCMe = (CEcoCalcC_DE118C94*)me;
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
 *   QueryInterface Function (IEcoCalculatorY)
 * </summary>
 *
 * <description>
 *   QueryInterface function for the IEcoCalculatorY interface
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoCalcC_DE118C94_Y_QueryInterface(/* in */ IEcoCalculatorYPtr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv) {
    if (me == 0) {
        return ERR_ECO_POINTER;
    }
    return CEcoCalcC_DE118C94_QueryCommon((CEcoCalcC_DE118C94*)((char_t*)me - 1 * sizeof(voidptr_t)), riid, ppv);
}

/*
 *
 * <summary>
 *   AddRef Function (IEcoCalculatorY)
 * </summary>
 *
 * <description>
 *   AddRef function for the IEcoCalculatorY interface (one counter for all interfaces)
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoCalcC_DE118C94_Y_AddRef(/* in */ IEcoCalculatorYPtr_t me) {
    CEcoCalcC_DE118C94* pCMe = 0;

    if (me == 0) {
        return (uint32_t)-1; /* ERR_ECO_POINTER */
    }
    pCMe = (CEcoCalcC_DE118C94*)((char_t*)me - 1 * sizeof(voidptr_t));
    return ++pCMe->m_cRef;
}

/*
 *
 * <summary>
 *   Release Function (IEcoCalculatorY)
 * </summary>
 *
 * <description>
 *   Release function for the IEcoCalculatorY interface (one counter for all interfaces)
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoCalcC_DE118C94_Y_Release(/* in */ IEcoCalculatorYPtr_t me) {
    CEcoCalcC_DE118C94* pCMe = 0;

    if (me == 0) {
        return (uint32_t)-1; /* ERR_ECO_POINTER */
    }
    pCMe = (CEcoCalcC_DE118C94*)((char_t*)me - 1 * sizeof(voidptr_t));
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
static int32_t ECOCALLMETHOD CEcoCalcC_DE118C94_Addition(/* in */ IEcoCalculatorXPtr_t me, /* in */ int16_t a, /* in */ int16_t b) {
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
 *   On overflow of the 16 bit result the method returns 0.
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoCalcC_DE118C94_Subtraction(/* in */ IEcoCalculatorXPtr_t me, /* in */ int16_t a, /* in */ int16_t b) {
    int32_t diff = 0;

    if (me == 0) {
        return 0;
    }
    diff = (int32_t)a - (int32_t)b;
    if (diff < -32768 || diff > 32767) {
        diff = 0;
    }
    return (int16_t)diff;
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
static int32_t ECOCALLMETHOD CEcoCalcC_DE118C94_Multiplication(/* in */ IEcoCalculatorYPtr_t me, /* in */ int16_t a, /* in */ int16_t b) {
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
 *   Division by zero returns -1. Returns 0 for me == NULL.
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoCalcC_DE118C94_Division(/* in */ IEcoCalculatorYPtr_t me, /* in */ int16_t a, /* in */ int16_t b) {
    int32_t quot = 0;

    if (me == 0) {
        return 0;
    }
    if (b == 0) {
        return (int16_t)(-1);
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
static int16_t ECOCALLMETHOD initCEcoCalcC_DE118C94(/*in*/ CEcoCalcC_DE118C94Ptr_t me, /* in */ IEcoUnknownPtr_t pIUnkSystem) {
    CEcoCalcC_DE118C94* pCMe = (CEcoCalcC_DE118C94*)me;
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
static int16_t ECOCALLMETHOD createCEcoCalcC_DE118C94(/* in */ CEcoCalcC_DE118C94Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter) {

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
static void ECOCALLMETHOD deleteCEcoCalcC_DE118C94(/* in */ CEcoCalcC_DE118C94Ptr_t pCMe) {
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
IEcoCalculatorXVTbl g_xIEcoCalculatorXVTbl_DE118C94 = {
    CEcoCalcC_DE118C94_X_QueryInterface,
    CEcoCalcC_DE118C94_X_AddRef,
    CEcoCalcC_DE118C94_X_Release,
    CEcoCalcC_DE118C94_Addition,
    CEcoCalcC_DE118C94_Subtraction
};

/* IEcoCalculatorY Virtual Table */
IEcoCalculatorYVTbl g_xIEcoCalculatorYVTbl_DE118C94 = {
    CEcoCalcC_DE118C94_Y_QueryInterface,
    CEcoCalcC_DE118C94_Y_AddRef,
    CEcoCalcC_DE118C94_Y_Release,
    CEcoCalcC_DE118C94_Multiplication,
    CEcoCalcC_DE118C94_Division
};

/* Object Instance */
CEcoCalcC_DE118C94 g_xCEcoCalcC_DE118C94 = {
    &g_xIEcoCalculatorXVTbl_DE118C94,
    &g_xIEcoCalculatorYVTbl_DE118C94,

    initCEcoCalcC_DE118C94,
    createCEcoCalcC_DE118C94,
    deleteCEcoCalcC_DE118C94,
    1, /* m_cRef */
    0, /* m_pIMem */
    0, /* m_pISys */
    0 /* m_Name */
};
