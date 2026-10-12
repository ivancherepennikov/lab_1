/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CEcoXorshift1_479BCC71
 * </summary>
 *
 * <description>
 *   This source code describes the implementation of the IEcoPseudoRandomService1 (stateless)
 *   and IEcoPseudoRandom1 (stateful) interfaces of CEcoXorshift1_479BCC71 (Xorshift, G. Marsaglia)
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
#include "CEcoXorshift1.h"

/* Algorithm name returned by get_Name; static, so it is never freed through m_Name */
static char_t g_XorshiftName[] = "Xorshift (Marsaglia)";

/*
 *
 * <summary>
 *   XorshiftWidthMask Function
 * </summary>
 *
 * <description>
 *   Returns the mask of the lowest bits bits. For bits >= 64 all bits are set;
 *   the shift is never done by the full width of the type.
 * </description>
 *
 */
static uint64_t XorshiftWidthMask(/* in */ uint16_t bits) {
    if (bits >= 64) {
        return ~((uint64_t)0);
    }
    return ((uint64_t)1 << bits) - 1;
}

/*
 *
 * <summary>
 *   XorshiftCore Function
 * </summary>
 *
 * <description>
 *   The only place where the Xorshift formula is written. bits selects the variant:
 *   32 -> (13, 17, 5), 64 -> (13, 7, 17). The state is truncated to the width before
 *   the zero check, and the result of every left shift is truncated at once.
 *   Returns ERR_ECO_POINTER (out is NULL), ERR_ECO_INVALIDARG (bad width or zero state).
 *   The result is written to out only on success.
 * </description>
 *
 */
static int16_t XorshiftCore(/* in */ uint64_t state, /* in */ uint16_t bits, /* out */ uint64_t* out) {
    uint64_t wmask = 0;
    uint64_t x = 0;

    /* Pointer Validation */
    if (out == 0) {
        return ERR_ECO_POINTER;
    }
    if (bits != 32 && bits != 64) {
        return ERR_ECO_INVALIDARG;
    }

    wmask = XorshiftWidthMask(bits);
    x = state & wmask;
    if (x == 0) {
        return ERR_ECO_INVALIDARG;
    }

    if (bits == 32) {
        x = x ^ ((x << 13) & wmask);
        x = x ^ (x >> 17);
        x = x ^ ((x << 5) & wmask);
    }
    else {
        x = x ^ ((x << 13) & wmask);
        x = x ^ (x >> 7);
        x = x ^ ((x << 17) & wmask);
    }

    *out = x & wmask;
    return ERR_ECO_SUCCESS;
}

/*
 *
 * <summary>
 *   QueryInterfaceCommon Function
 * </summary>
 *
 * <description>
 *   Shared branching for both QueryInterface entry points. pCMe is the object start.
 *   Supports IEcoPseudoRandomService1, IEcoPseudoRandom1 and IEcoUnknown.
 *   On success increments the single reference counter.
 * </description>
 *
 */
static int16_t QueryInterfaceCommon(/* in */ CEcoXorshift1_479BCC71* pCMe, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv) {
    /* Pointer Validation */
    if (pCMe == 0 || riid == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    if ( IsEqualUGUID(riid, &IID_IEcoPseudoRandomService1) ) {
        *ppv = &pCMe->m_pVTblIEcoPseudoRandomService1;
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoPseudoRandom1) ) {
        *ppv = &pCMe->m_pVTblIEcoPseudoRandom1;
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIEcoPseudoRandomService1;
    }
    else {
        *ppv = 0;
        return ERR_ECO_NOINTERFACE;
    }

    ++pCMe->m_cRef;
    return ERR_ECO_SUCCESS;
}

/*
 *
 * <summary>
 *   ReleaseCommon Function
 * </summary>
 *
 * <description>
 *   Decrements the single reference counter and deletes the instance at zero.
 * </description>
 *
 */
static uint32_t ReleaseCommon(/* in */ CEcoXorshift1_479BCC71* pCMe) {
    --pCMe->m_cRef;
    if ( pCMe->m_cRef == 0 ) {
        pCMe->Delete(pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

/*
 *
 * <summary>
 *   Service_QueryInterface Function
 * </summary>
 *
 * <description>
 *   QueryInterface through IEcoPseudoRandomService1 (interface 0): the object starts at me.
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoXorshift1_479BCC71_Service_QueryInterface(/* in */ IEcoPseudoRandomService1Ptr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv) {
    CEcoXorshift1_479BCC71* pCMe = 0;

    /* Pointer Validation */
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    pCMe = (CEcoXorshift1_479BCC71*)me;
    return QueryInterfaceCommon(pCMe, riid, ppv);
}

/*
 *
 * <summary>
 *   Service_AddRef Function
 * </summary>
 *
 * <description>
 *   AddRef through IEcoPseudoRandomService1 (interface 0).
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoXorshift1_479BCC71_Service_AddRef(/* in */ IEcoPseudoRandomService1Ptr_t me) {
    CEcoXorshift1_479BCC71* pCMe = 0;

    /* Pointer Validation */
    if (me == 0) {
        return -1; /* ERR_ECO_POINTER */
    }

    pCMe = (CEcoXorshift1_479BCC71*)me;
    return ++pCMe->m_cRef;
}

/*
 *
 * <summary>
 *   Service_Release Function
 * </summary>
 *
 * <description>
 *   Release through IEcoPseudoRandomService1 (interface 0).
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoXorshift1_479BCC71_Service_Release(/* in */ IEcoPseudoRandomService1Ptr_t me) {
    CEcoXorshift1_479BCC71* pCMe = 0;

    /* Pointer Validation */
    if (me == 0) {
        return -1; /* ERR_ECO_POINTER */
    }

    pCMe = (CEcoXorshift1_479BCC71*)me;
    return ReleaseCommon(pCMe);
}

/*
 *
 * <summary>
 *   Service_Step32 Function
 * </summary>
 *
 * <description>
 *   Stateless 32 bit Xorshift step. param is ignored. newState receives the new value only on success.
 *   Returns ERR_ECO_POINTER, ERR_ECO_INVALIDARG (zero state after truncation to 32 bits).
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoXorshift1_479BCC71_Service_Step32(/* in */ IEcoPseudoRandomService1Ptr_t me, /* in */ uint32_t state, /* in */ uint32_t param, /* out */ uint32_t* newState) {
    uint64_t result = 0;
    int16_t rc = ERR_ECO_POINTER;

    /* Pointer Validation */
    if (me == 0 || newState == 0) {
        return ERR_ECO_POINTER;
    }

    (void)param; /* Xorshift has no feedback mask */
    rc = XorshiftCore((uint64_t)state, 32, &result);
    if (rc == ERR_ECO_SUCCESS) {
        *newState = (uint32_t)result;
    }
    return rc;
}

/*
 *
 * <summary>
 *   Service_Step64 Function
 * </summary>
 *
 * <description>
 *   Stateless 64 bit Xorshift step. param is ignored. newState receives the new value only on success.
 *   Returns ERR_ECO_POINTER, ERR_ECO_INVALIDARG (zero state).
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoXorshift1_479BCC71_Service_Step64(/* in */ IEcoPseudoRandomService1Ptr_t me, /* in */ uint64_t state, /* in */ uint64_t param, /* out */ uint64_t* newState) {
    int16_t rc = ERR_ECO_POINTER;

    /* Pointer Validation */
    if (me == 0 || newState == 0) {
        return ERR_ECO_POINTER;
    }

    (void)param; /* Xorshift has no feedback mask */
    rc = XorshiftCore(state, 64, newState);
    return rc;
}

/*
 *
 * <summary>
 *   PRS1_QueryInterface Function
 * </summary>
 *
 * <description>
 *   QueryInterface through IEcoPseudoRandom1 (interface 1): the object starts sizeof(voidptr_t) bytes before me.
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoXorshift1_479BCC71_PRS1_QueryInterface(/* in */ IEcoPseudoRandom1Ptr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv) {
    CEcoXorshift1_479BCC71* pCMe = 0;

    /* Pointer Validation */
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    pCMe = (CEcoXorshift1_479BCC71*)((char_t*)me - sizeof(voidptr_t));
    return QueryInterfaceCommon(pCMe, riid, ppv);
}

/*
 *
 * <summary>
 *   PRS1_AddRef Function
 * </summary>
 *
 * <description>
 *   AddRef through IEcoPseudoRandom1 (interface 1).
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoXorshift1_479BCC71_PRS1_AddRef(/* in */ IEcoPseudoRandom1Ptr_t me) {
    CEcoXorshift1_479BCC71* pCMe = 0;

    /* Pointer Validation */
    if (me == 0) {
        return -1; /* ERR_ECO_POINTER */
    }

    pCMe = (CEcoXorshift1_479BCC71*)((char_t*)me - sizeof(voidptr_t));
    return ++pCMe->m_cRef;
}

/*
 *
 * <summary>
 *   PRS1_Release Function
 * </summary>
 *
 * <description>
 *   Release through IEcoPseudoRandom1 (interface 1).
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoXorshift1_479BCC71_PRS1_Release(/* in */ IEcoPseudoRandom1Ptr_t me) {
    CEcoXorshift1_479BCC71* pCMe = 0;

    /* Pointer Validation */
    if (me == 0) {
        return -1; /* ERR_ECO_POINTER */
    }

    pCMe = (CEcoXorshift1_479BCC71*)((char_t*)me - sizeof(voidptr_t));
    return ReleaseCommon(pCMe);
}

/*
 *
 * <summary>
 *   PRS1_Init Function
 * </summary>
 *
 * <description>
 *   Starts (or restarts) the sequence. bits must be 32 or 64. seed is truncated to bits;
 *   a zero seed is ERR_ECO_INVALIDARG. param is ignored. The object state is changed only on success.
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoXorshift1_479BCC71_PRS1_Init(/* in */ IEcoPseudoRandom1Ptr_t me, /* in */ uint64_t seed, /* in */ uint64_t param, /* in */ uint16_t bits) {
    CEcoXorshift1_479BCC71* pCMe = 0;
    uint64_t state = 0;

    /* Pointer Validation */
    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    pCMe = (CEcoXorshift1_479BCC71*)((char_t*)me - sizeof(voidptr_t));
    (void)param; /* Xorshift has no feedback mask */

    if (bits != 32 && bits != 64) {
        return ERR_ECO_INVALIDARG;
    }
    state = seed & XorshiftWidthMask(bits);
    if (state == 0) {
        return ERR_ECO_INVALIDARG;
    }

    pCMe->m_state = state;
    pCMe->m_bits = bits;
    return ERR_ECO_SUCCESS;
}

/*
 *
 * <summary>
 *   PRS1_Next Function
 * </summary>
 *
 * <description>
 *   Advances the register and returns the new state in value (zero-extended for 32 bit width).
 *   Returns ERR_ECO_POINTER, ERR_ECO_FAIL (Init not called), ERR_ECO_INVALIDARG.
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoXorshift1_479BCC71_PRS1_Next(/* in */ IEcoPseudoRandom1Ptr_t me, /* out */ uint64_t* value) {
    CEcoXorshift1_479BCC71* pCMe = 0;
    uint64_t result = 0;
    int16_t rc = ERR_ECO_POINTER;

    /* Pointer Validation */
    if (me == 0 || value == 0) {
        return ERR_ECO_POINTER;
    }

    pCMe = (CEcoXorshift1_479BCC71*)((char_t*)me - sizeof(voidptr_t));
    if (pCMe->m_bits == 0) {
        return ERR_ECO_FAIL;
    }

    rc = XorshiftCore(pCMe->m_state, pCMe->m_bits, &result);
    if (rc == ERR_ECO_SUCCESS) {
        pCMe->m_state = result;
        *value = result;
    }
    return rc;
}

/*
 *
 * <summary>
 *   PRS1_get_Name Function
 * </summary>
 *
 * <description>
 *   Returns the algorithm name, a static string owned by the component. Returns 0 if me is NULL.
 * </description>
 *
 */
static char_t* ECOCALLMETHOD CEcoXorshift1_479BCC71_PRS1_get_Name(/* in */ IEcoPseudoRandom1Ptr_t me) {
    /* Pointer Validation */
    if (me == 0) {
        return 0; /* ERR_ECO_POINTER */
    }

    return g_XorshiftName;
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
static int16_t ECOCALLMETHOD initCEcoXorshift1_479BCC71(/*in*/ CEcoXorshift1_479BCC71Ptr_t me, /* in */ IEcoUnknownPtr_t pIUnkSystem) {
    CEcoXorshift1_479BCC71* pCMe = (CEcoXorshift1_479BCC71*)me;
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
 * </description>
 *
 */
static int16_t ECOCALLMETHOD createCEcoXorshift1_479BCC71(/* in */ CEcoXorshift1_479BCC71Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter) {
    int16_t result = ERR_ECO_POINTER;

    /* Pointer Validation */
    if (pCMe == 0) {
        return result; /* ERR_ECO_POINTER */
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
static void ECOCALLMETHOD deleteCEcoXorshift1_479BCC71(/* in */ CEcoXorshift1_479BCC71Ptr_t pCMe) {
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

/* IEcoPseudoRandomService1 Virtual Table (interface 0) */
IEcoPseudoRandomService1VTbl g_xIEcoPseudoRandomService1VTbl_479BCC71 = {
    CEcoXorshift1_479BCC71_Service_QueryInterface,
    CEcoXorshift1_479BCC71_Service_AddRef,
    CEcoXorshift1_479BCC71_Service_Release,
    CEcoXorshift1_479BCC71_Service_Step32,
    CEcoXorshift1_479BCC71_Service_Step64
};

/* IEcoPseudoRandom1 Virtual Table (interface 1) */
IEcoPseudoRandom1VTbl g_xIEcoPseudoRandom1VTbl_479BCC71 = {
    CEcoXorshift1_479BCC71_PRS1_QueryInterface,
    CEcoXorshift1_479BCC71_PRS1_AddRef,
    CEcoXorshift1_479BCC71_PRS1_Release,
    CEcoXorshift1_479BCC71_PRS1_Init,
    CEcoXorshift1_479BCC71_PRS1_Next,
    CEcoXorshift1_479BCC71_PRS1_get_Name
};



/* Object Instance */
CEcoXorshift1_479BCC71 g_xCEcoXorshift1_479BCC71 = {
    &g_xIEcoPseudoRandomService1VTbl_479BCC71,
    &g_xIEcoPseudoRandom1VTbl_479BCC71,

    initCEcoXorshift1_479BCC71,
    createCEcoXorshift1_479BCC71,
    deleteCEcoXorshift1_479BCC71,
    1, /* m_cRef */
    0, /* m_pIMem */
    0, /* m_pISys */
    0, /* m_Name */
    0, /* m_state */
    0  /* m_bits */
};
