/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CEcoLab2_BBF941C7
 * </summary>
 *
 * <description>
 *   This source code describes the implementation of the interfaces for CEcoLab2_BBF941C7
 *   Inner components are used through containment (they are called by this component) and
 *   through aggregation (their interface pointers are handed to the client directly).
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
#include "CEcoLab2.h"
#include "IdEcoCalcA.h"
#include "IdEcoCalcB.h"
#include "IdEcoCalcC.h"
#include "IdEcoCalcD.h"
#include "IdEcoCalcE.h"
#include "IdEcoLab1Outer.h"

/*
 *
 * <summary>
 *   ReleaseCalculators Function
 * </summary>
 *
 * <description>
 *   Releases the inner calculator components: contained ones with Release, aggregated ones with the non-delegating IEcoUnknown.
 * </description>
 *
 */
static void CEcoLab2_BBF941C7_ReleaseCalculators(/* in */ CEcoLab2_BBF941C7* pCMe) {
    if (pCMe->m_pIXc != 0) {
        pCMe->m_pIXc->pVTbl->Release(pCMe->m_pIXc);
        pCMe->m_pIXc = 0;
    }
    if (pCMe->m_pIYc != 0) {
        pCMe->m_pIYc->pVTbl->Release(pCMe->m_pIYc);
        pCMe->m_pIYc = 0;
    }
    /* releasing the non-delegating IEcoUnknown destroys an aggregated inner component */
    if (pCMe->m_pXInner != 0) {
        pCMe->m_pXInner->pVTbl->Release(pCMe->m_pXInner);
        pCMe->m_pXInner = 0;
    }
    if (pCMe->m_pYInner != 0) {
        pCMe->m_pYInner->pVTbl->Release(pCMe->m_pYInner);
        pCMe->m_pYInner = 0;
    }
}

/*
 *
 * <summary>
 *   SelectCalcInternal Function
 * </summary>
 *
 * <description>
 *   Creates the inner calculator components for the selected kinds.
 *   The controlling unknown given to an aggregated inner component is the delegating IEcoUnknown of this component.
 * </description>
 *
 */
static int16_t CEcoLab2_BBF941C7_SelectCalcInternal(/* in */ CEcoLab2_BBF941C7* pCMe, /* in */ uint16_t kindX, /* in */ uint16_t kindY) {
    IEcoUnknown* pOuterUnk = (IEcoUnknown*)&pCMe->m_pVTblIEcoLab2;
    int16_t result = ERR_ECO_SUCCESS;

    if (pCMe->m_pIBus == 0) {
        return ERR_ECO_FAIL;
    }
    if (kindX > 2 || kindY > 2) {
        return ERR_ECO_INVALIDARG;
    }
    CEcoLab2_BBF941C7_ReleaseCalculators(pCMe);

    /* IEcoCalculatorX: A or C by containment, B by aggregation */
    if (kindX == 1) {
        result = pCMe->m_pIBus->pVTbl->QueryComponent(pCMe->m_pIBus, &CID_EcoCalcB, pOuterUnk, &IID_IEcoUnknown, (void**)&pCMe->m_pXInner);
    }
    else {
        result = pCMe->m_pIBus->pVTbl->QueryComponent(pCMe->m_pIBus, (kindX == 0) ? &CID_EcoCalcA : &CID_EcoCalcC, 0, &IID_IEcoCalculatorX, (void**)&pCMe->m_pIXc);
    }
    if (result != ERR_ECO_SUCCESS) {
        CEcoLab2_BBF941C7_ReleaseCalculators(pCMe);
        return result;
    }

    /* IEcoCalculatorY: D or C by containment, E by aggregation */
    if (kindY == 1) {
        result = pCMe->m_pIBus->pVTbl->QueryComponent(pCMe->m_pIBus, &CID_EcoCalcE, pOuterUnk, &IID_IEcoUnknown, (void**)&pCMe->m_pYInner);
    }
    else {
        result = pCMe->m_pIBus->pVTbl->QueryComponent(pCMe->m_pIBus, (kindY == 0) ? &CID_EcoCalcD : &CID_EcoCalcC, 0, &IID_IEcoCalculatorY, (void**)&pCMe->m_pIYc);
    }
    if (result != ERR_ECO_SUCCESS) {
        CEcoLab2_BBF941C7_ReleaseCalculators(pCMe);
        return result;
    }
    pCMe->m_kindX = kindX;
    pCMe->m_kindY = kindY;
    return ERR_ECO_SUCCESS;
}

/*
 *
 * <summary>
 *   Delegating IEcoUnknown (IEcoLab2)
 * </summary>
 *
 * <description>
 *   QueryInterface, AddRef and Release of every interface delegate to the controlling unknown.
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoLab2_BBF941C7_Own_QueryInterface(/* in */ IEcoLab2Ptr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv) {
    CEcoLab2_BBF941C7* pCMe = 0;

    if (me == 0 || riid == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }
    pCMe = (CEcoLab2_BBF941C7*)me;
    return pCMe->m_pIUnkOuter->pVTbl->QueryInterface(pCMe->m_pIUnkOuter, riid, ppv);
}

static uint32_t ECOCALLMETHOD CEcoLab2_BBF941C7_Own_AddRef(/* in */ IEcoLab2Ptr_t me) {
    CEcoLab2_BBF941C7* pCMe = 0;

    if (me == 0) {
        return (uint32_t)-1; /* ERR_ECO_POINTER */
    }
    pCMe = (CEcoLab2_BBF941C7*)me;
    return pCMe->m_pIUnkOuter->pVTbl->AddRef(pCMe->m_pIUnkOuter);
}

static uint32_t ECOCALLMETHOD CEcoLab2_BBF941C7_Own_Release(/* in */ IEcoLab2Ptr_t me) {
    CEcoLab2_BBF941C7* pCMe = 0;

    if (me == 0) {
        return (uint32_t)-1; /* ERR_ECO_POINTER */
    }
    pCMe = (CEcoLab2_BBF941C7*)me;
    return pCMe->m_pIUnkOuter->pVTbl->Release(pCMe->m_pIUnkOuter);
}

/*
 *
 * <summary>
 *   Delegating IEcoUnknown (IEcoCalculatorX)
 * </summary>
 *
 * <description>
 *   QueryInterface, AddRef and Release of every interface delegate to the controlling unknown.
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoLab2_BBF941C7_X_QueryInterface(/* in */ IEcoCalculatorXPtr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv) {
    CEcoLab2_BBF941C7* pCMe = 0;

    if (me == 0 || riid == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }
    pCMe = (CEcoLab2_BBF941C7*)((char_t*)me - 1 * sizeof(voidptr_t));
    return pCMe->m_pIUnkOuter->pVTbl->QueryInterface(pCMe->m_pIUnkOuter, riid, ppv);
}

static uint32_t ECOCALLMETHOD CEcoLab2_BBF941C7_X_AddRef(/* in */ IEcoCalculatorXPtr_t me) {
    CEcoLab2_BBF941C7* pCMe = 0;

    if (me == 0) {
        return (uint32_t)-1; /* ERR_ECO_POINTER */
    }
    pCMe = (CEcoLab2_BBF941C7*)((char_t*)me - 1 * sizeof(voidptr_t));
    return pCMe->m_pIUnkOuter->pVTbl->AddRef(pCMe->m_pIUnkOuter);
}

static uint32_t ECOCALLMETHOD CEcoLab2_BBF941C7_X_Release(/* in */ IEcoCalculatorXPtr_t me) {
    CEcoLab2_BBF941C7* pCMe = 0;

    if (me == 0) {
        return (uint32_t)-1; /* ERR_ECO_POINTER */
    }
    pCMe = (CEcoLab2_BBF941C7*)((char_t*)me - 1 * sizeof(voidptr_t));
    return pCMe->m_pIUnkOuter->pVTbl->Release(pCMe->m_pIUnkOuter);
}

/*
 *
 * <summary>
 *   Delegating IEcoUnknown (IEcoCalculatorY)
 * </summary>
 *
 * <description>
 *   QueryInterface, AddRef and Release of every interface delegate to the controlling unknown.
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoLab2_BBF941C7_Y_QueryInterface(/* in */ IEcoCalculatorYPtr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv) {
    CEcoLab2_BBF941C7* pCMe = 0;

    if (me == 0 || riid == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }
    pCMe = (CEcoLab2_BBF941C7*)((char_t*)me - 2 * sizeof(voidptr_t));
    return pCMe->m_pIUnkOuter->pVTbl->QueryInterface(pCMe->m_pIUnkOuter, riid, ppv);
}

static uint32_t ECOCALLMETHOD CEcoLab2_BBF941C7_Y_AddRef(/* in */ IEcoCalculatorYPtr_t me) {
    CEcoLab2_BBF941C7* pCMe = 0;

    if (me == 0) {
        return (uint32_t)-1; /* ERR_ECO_POINTER */
    }
    pCMe = (CEcoLab2_BBF941C7*)((char_t*)me - 2 * sizeof(voidptr_t));
    return pCMe->m_pIUnkOuter->pVTbl->AddRef(pCMe->m_pIUnkOuter);
}

static uint32_t ECOCALLMETHOD CEcoLab2_BBF941C7_Y_Release(/* in */ IEcoCalculatorYPtr_t me) {
    CEcoLab2_BBF941C7* pCMe = 0;

    if (me == 0) {
        return (uint32_t)-1; /* ERR_ECO_POINTER */
    }
    pCMe = (CEcoLab2_BBF941C7*)((char_t*)me - 2 * sizeof(voidptr_t));
    return pCMe->m_pIUnkOuter->pVTbl->Release(pCMe->m_pIUnkOuter);
}

/*
 *
 * <summary>
 *   Non-delegating IEcoUnknown
 * </summary>
 *
 * <description>
 *   Real reference counting and interface lookup of the component itself (interface 3 of the object).
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoLab2_BBF941C7_Nondelegating_QueryInterface(/* in */ IEcoUnknownPtr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv) {
    CEcoLab2_BBF941C7* pCMe = 0;

    if (me == 0 || riid == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }
    pCMe = (CEcoLab2_BBF941C7*)((char_t*)me - 3 * sizeof(voidptr_t));

    if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &pCMe->m_pVTblINondelegatingUnk;
        ++pCMe->m_cRef;
        return ERR_ECO_SUCCESS;
    }
    if (IsEqualUGUID(riid, &IID_IEcoLab2)) {
        *ppv = &pCMe->m_pVTblIEcoLab2;
        pCMe->m_pIUnkOuter->pVTbl->AddRef(pCMe->m_pIUnkOuter);
        return ERR_ECO_SUCCESS;
    }
    /* IEcoCalculatorX: the interface of the aggregated inner component goes to the client directly, otherwise this component's own */
    if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        if (pCMe->m_pXInner != 0) {
            return pCMe->m_pXInner->pVTbl->QueryInterface(pCMe->m_pXInner, riid, ppv);
        }
        if (pCMe->m_pIXc == 0) {
            *ppv = 0;
            return ERR_ECO_NOINTERFACE;
        }
        *ppv = &pCMe->m_pVTblIEcoCalculatorX;
        pCMe->m_pIUnkOuter->pVTbl->AddRef(pCMe->m_pIUnkOuter);
        return ERR_ECO_SUCCESS;
    }
    if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        if (pCMe->m_pYInner != 0) {
            return pCMe->m_pYInner->pVTbl->QueryInterface(pCMe->m_pYInner, riid, ppv);
        }
        if (pCMe->m_pIYc == 0) {
            *ppv = 0;
            return ERR_ECO_NOINTERFACE;
        }
        *ppv = &pCMe->m_pVTblIEcoCalculatorY;
        pCMe->m_pIUnkOuter->pVTbl->AddRef(pCMe->m_pIUnkOuter);
        return ERR_ECO_SUCCESS;
    }
    /* every other interface comes from the aggregated Eco.Lab1Outer (its pointers are handed to the client directly) */
    if (pCMe->m_pLab1Inner != 0) {
        return pCMe->m_pLab1Inner->pVTbl->QueryInterface(pCMe->m_pLab1Inner, riid, ppv);
    }
    *ppv = 0;
    return ERR_ECO_NOINTERFACE;
}

static uint32_t ECOCALLMETHOD CEcoLab2_BBF941C7_Nondelegating_AddRef(/* in */ IEcoUnknownPtr_t me) {
    CEcoLab2_BBF941C7* pCMe = 0;

    if (me == 0) {
        return (uint32_t)-1; /* ERR_ECO_POINTER */
    }
    pCMe = (CEcoLab2_BBF941C7*)((char_t*)me - 3 * sizeof(voidptr_t));
    return ++pCMe->m_cRef;
}

static uint32_t ECOCALLMETHOD CEcoLab2_BBF941C7_Nondelegating_Release(/* in */ IEcoUnknownPtr_t me) {
    CEcoLab2_BBF941C7* pCMe = 0;

    if (me == 0) {
        return (uint32_t)-1; /* ERR_ECO_POINTER */
    }
    pCMe = (CEcoLab2_BBF941C7*)((char_t*)me - 3 * sizeof(voidptr_t));
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
 *   Addition Function (containment)
 * </summary>
 *
 * <description>
 *   Forwards the call to the contained inner component; returns 0 when none is contained.
 * </description>
 *
 */
static int32_t ECOCALLMETHOD CEcoLab2_BBF941C7_X_Addition(/* in */ IEcoCalculatorXPtr_t me, /* in */ int16_t a, /* in */ int16_t b) {
    CEcoLab2_BBF941C7* pCMe = 0;

    if (me == 0) {
        return 0;
    }
    pCMe = (CEcoLab2_BBF941C7*)((char_t*)me - 1 * sizeof(voidptr_t));
    if (pCMe->m_pIXc == 0) {
        return 0;
    }
    return pCMe->m_pIXc->pVTbl->Addition(pCMe->m_pIXc, a, b);
}

/*
 *
 * <summary>
 *   Subtraction Function (containment)
 * </summary>
 *
 * <description>
 *   Forwards the call to the contained inner component; returns 0 when none is contained.
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoLab2_BBF941C7_X_Subtraction(/* in */ IEcoCalculatorXPtr_t me, /* in */ int16_t a, /* in */ int16_t b) {
    CEcoLab2_BBF941C7* pCMe = 0;

    if (me == 0) {
        return 0;
    }
    pCMe = (CEcoLab2_BBF941C7*)((char_t*)me - 1 * sizeof(voidptr_t));
    if (pCMe->m_pIXc == 0) {
        return 0;
    }
    return pCMe->m_pIXc->pVTbl->Subtraction(pCMe->m_pIXc, a, b);
}

/*
 *
 * <summary>
 *   Multiplication Function (containment)
 * </summary>
 *
 * <description>
 *   Forwards the call to the contained inner component; returns 0 when none is contained.
 * </description>
 *
 */
static int32_t ECOCALLMETHOD CEcoLab2_BBF941C7_Y_Multiplication(/* in */ IEcoCalculatorYPtr_t me, /* in */ int16_t a, /* in */ int16_t b) {
    CEcoLab2_BBF941C7* pCMe = 0;

    if (me == 0) {
        return 0;
    }
    pCMe = (CEcoLab2_BBF941C7*)((char_t*)me - 2 * sizeof(voidptr_t));
    if (pCMe->m_pIYc == 0) {
        return 0;
    }
    return pCMe->m_pIYc->pVTbl->Multiplication(pCMe->m_pIYc, a, b);
}

/*
 *
 * <summary>
 *   Division Function (containment)
 * </summary>
 *
 * <description>
 *   Forwards the call to the contained inner component; returns 0 when none is contained.
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoLab2_BBF941C7_Y_Division(/* in */ IEcoCalculatorYPtr_t me, /* in */ int16_t a, /* in */ int16_t b) {
    CEcoLab2_BBF941C7* pCMe = 0;

    if (me == 0) {
        return 0;
    }
    pCMe = (CEcoLab2_BBF941C7*)((char_t*)me - 2 * sizeof(voidptr_t));
    if (pCMe->m_pIYc == 0) {
        return 0;
    }
    return pCMe->m_pIYc->pVTbl->Division(pCMe->m_pIYc, a, b);
}

/*
 *
 * <summary>
 *   IEcoLab2 methods
 * </summary>
 *
 * <description>
 *   SelectCalculators chooses the inner calculator components.
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoLab2_BBF941C7_Own_SelectCalculators(/* in */ IEcoLab2Ptr_t me, /* in */ uint16_t kindX, /* in */ uint16_t kindY) {
    if (me == 0) {
        return ERR_ECO_POINTER;
    }
    return CEcoLab2_BBF941C7_SelectCalcInternal((CEcoLab2_BBF941C7*)me, kindX, kindY);
}

/*
 *
 * <summary>
 *   Init Function
 * </summary>
 *
 * <description>
 *   Instance initialization: gets the bus and the allocator (as in the template) and creates the default inner components.
 * </description>
 *
 */
static int16_t ECOCALLMETHOD initCEcoLab2_BBF941C7(/*in*/ CEcoLab2_BBF941C7Ptr_t me, /* in */ IEcoUnknownPtr_t pIUnkSystem) {
    CEcoLab2_BBF941C7* pCMe = (CEcoLab2_BBF941C7*)me;
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
    if (result != 0 || pIBus == 0) {
        return ERR_ECO_NOBUS;
    }

    /* Getting the component ID for working with memory */
    result = pIBus->pVTbl->QueryInterface(pIBus, &IID_IEcoInterfaceBus1MemExt, (void**)&pIMemExt);
    if (result == 0 && pIMemExt != 0) {
        rcid = (UGUID*)pIMemExt->pVTbl->get_Manager(pIMemExt);
        pIMemExt->pVTbl->Release(pIMemExt);
    }

    /* Getting the memory allocator interface */
    result = pIBus->pVTbl->QueryComponent(pIBus, rcid, 0, &IID_IEcoMemoryAllocator1, (void**) &pCMe->m_pIMem);
    if (result != 0 || pCMe->m_pIMem == 0) {
        pIBus->pVTbl->Release(pIBus);
        return ERR_ECO_GET_MEMORY_ALLOCATOR;
    }

    /* The bus is kept to create the inner components; it is released in Delete */
    pCMe->m_pIBus = pIBus;

    result = CEcoLab2_BBF941C7_SelectCalcInternal(pCMe, 0, 0);
    if (result != ERR_ECO_SUCCESS) {
        return result;
    }
    /* Eco.Lab1Outer is aggregated: it receives the delegating IEcoUnknown of this component as its controlling unknown */
    result = pCMe->m_pIBus->pVTbl->QueryComponent(pCMe->m_pIBus, &CID_EcoLab1Outer, (IEcoUnknown*)&pCMe->m_pVTblIEcoLab2, &IID_IEcoUnknown, (void**)&pCMe->m_pLab1Inner);
    if (result != ERR_ECO_SUCCESS) {
        return result;
    }
    return ERR_ECO_SUCCESS;
}

/*
 *
 * <summary>
 *   Create Function
 * </summary>
 *
 * <description>
 *   Instance creation: an outer IEcoUnknown (aggregation) becomes the controlling unknown, otherwise the component controls itself.
 * </description>
 *
 */
static int16_t ECOCALLMETHOD createCEcoLab2_BBF941C7(/* in */ CEcoLab2_BBF941C7Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter) {

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
 *   Instance freeing: the inner components and the bus are released first (the counter is stabilised so that a re-entrant Release cannot destroy the object twice).
 * </description>
 *
 */
static void ECOCALLMETHOD deleteCEcoLab2_BBF941C7(/* in */ CEcoLab2_BBF941C7Ptr_t pCMe) {
    IEcoMemoryAllocator1* pIMem = 0;

    if (pCMe != 0 ) {
        pCMe->m_cRef = 1;
        CEcoLab2_BBF941C7_ReleaseCalculators(pCMe);
        if (pCMe->m_pLab1Inner != 0) {
            pCMe->m_pLab1Inner->pVTbl->Release(pCMe->m_pLab1Inner);
            pCMe->m_pLab1Inner = 0;
        }
        if (pCMe->m_pIBus != 0) {
            pCMe->m_pIBus->pVTbl->Release(pCMe->m_pIBus);
            pCMe->m_pIBus = 0;
        }
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

/* IEcoLab2 Virtual Table */
IEcoLab2VTbl g_xIEcoLab2VTbl_BBF941C7 = {
    CEcoLab2_BBF941C7_Own_QueryInterface,
    CEcoLab2_BBF941C7_Own_AddRef,
    CEcoLab2_BBF941C7_Own_Release,
    CEcoLab2_BBF941C7_Own_SelectCalculators
};

/* IEcoCalculatorX Virtual Table */
IEcoCalculatorXVTbl g_xIEcoCalculatorXVTbl_BBF941C7 = {
    CEcoLab2_BBF941C7_X_QueryInterface,
    CEcoLab2_BBF941C7_X_AddRef,
    CEcoLab2_BBF941C7_X_Release,
    CEcoLab2_BBF941C7_X_Addition,
    CEcoLab2_BBF941C7_X_Subtraction
};

/* IEcoCalculatorY Virtual Table */
IEcoCalculatorYVTbl g_xIEcoCalculatorYVTbl_BBF941C7 = {
    CEcoLab2_BBF941C7_Y_QueryInterface,
    CEcoLab2_BBF941C7_Y_AddRef,
    CEcoLab2_BBF941C7_Y_Release,
    CEcoLab2_BBF941C7_Y_Multiplication,
    CEcoLab2_BBF941C7_Y_Division
};

/* Non-delegating IEcoUnknown Virtual Table */
IEcoUnknownVTbl g_xINondelegatingUnkVTbl_BBF941C7 = {
    CEcoLab2_BBF941C7_Nondelegating_QueryInterface,
    CEcoLab2_BBF941C7_Nondelegating_AddRef,
    CEcoLab2_BBF941C7_Nondelegating_Release
};

/* Object Instance */
CEcoLab2_BBF941C7 g_xCEcoLab2_BBF941C7 = {
    &g_xIEcoLab2VTbl_BBF941C7,
    &g_xIEcoCalculatorXVTbl_BBF941C7,
    &g_xIEcoCalculatorYVTbl_BBF941C7,
    &g_xINondelegatingUnkVTbl_BBF941C7,

    initCEcoLab2_BBF941C7,
    createCEcoLab2_BBF941C7,
    deleteCEcoLab2_BBF941C7,
    1, /* m_cRef */
    0, /* m_pIMem */
    0, /* m_pISys */
    0, /* m_Name */
    0, /* m_pIUnkOuter */
    0, /* m_pIBus */
    0, /* m_kindX */
    0, /* m_kindY */
    0, /* m_pIXc */
    0, /* m_pIYc */
    0, /* m_pXInner */
    0, /* m_pYInner */
    0  /* m_pLab1Inner */
};
