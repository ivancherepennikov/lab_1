/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CEcoGaloisLFSR1_66391FF5
 * </summary>
 *
 * <description>
 *   Galois LFSR (shift right) pseudo random generator. Interface 0 is the stateless
 *   IEcoPseudoRandomService1 (Step32, Step64), interface 1 is the stateful IEcoPseudoRandom1
 *   (Init, Next, get_Name). Both interfaces share one object, one reference counter
 *   and one step formula (Core).
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
#include "CEcoGaloisLFSR1.h"

/* Algorithm name returned by get_Name (static, not stored in m_Name) */
static char_t g_GaloisLFSR1Name[] = "GaloisLFSR1";

/*
 *
 * <summary>
 *   EffectiveParam Function
 * </summary>
 *
 * <description>
 *   Returns the tap mask to use: param itself, or the default maximal-length mask
 *   for the given width (32 bit: 0x80200003, 64 bit: 0xD800000000000000) when param == 0.
 * </description>
 *
 */
static uint64_t EffectiveParam(/* in */ uint64_t param, /* in */ uint16_t callBits) {
    if (param != 0) {
        return param;
    }
    if (callBits == 32) {
        return (uint64_t)0x80200003UL;
    }
    return (((uint64_t)0xD8000000UL) << 32) | (uint64_t)0x00000000UL;
}

/*
 *
 * <summary>
 *   Degree Function
 * </summary>
 *
 * <description>
 *   Returns the register width: index of the highest set bit of param plus one (0 for param == 0).
 * </description>
 *
 */
static uint16_t Degree(/* in */ uint64_t param) {
    uint16_t n = 0;

    while (param != 0) {
        param = param >> 1;
        n++;
    }
    return n;
}

/*
 *
 * <summary>
 *   WidthMask Function
 * </summary>
 *
 * <description>
 *   Returns a mask of the lowest n bits. For n >= 64 all bits are set (no shift by 64).
 * </description>
 *
 */
static uint64_t WidthMask(/* in */ uint16_t n) {
    if (n >= 64) {
        return ~((uint64_t)0);
    }
    return (((uint64_t)1) << n) - 1;
}

/*
 *
 * <summary>
 *   Core Function
 * </summary>
 *
 * <description>
 *   One Galois step, the only place where the step formula is written.
 *   callBits (32 or 64) selects only the default mask. The register width is the degree of
 *   the mask. The state is truncated to the width before the check for zero, and the result
 *   is truncated after the step. Returns ERR_ECO_INVALIDARG for a zero state within the width.
 * </description>
 *
 */
static int16_t Core(/* in */ uint64_t state, /* in */ uint64_t param, /* in */ uint16_t callBits, /* out */ uint64_t* out) {
    uint64_t wmask = 0;
    uint64_t lsb = 0;
    uint64_t r = 0;

    param = EffectiveParam(param, callBits);
    wmask = WidthMask(Degree(param));

    state = state & wmask;
    if (state == 0) {
        return ERR_ECO_INVALIDARG;
    }

    lsb = state & 1;
    r = state >> 1;
    if (lsb != 0) {
        r = r ^ param;
    }

    *out = r & wmask;
    return ERR_ECO_SUCCESS;
}

/*
 *
 * <summary>
 *   Common QueryInterface Function
 * </summary>
 *
 * <description>
 *   Shared branch logic for both interface tables. Any of the three interfaces
 *   (IEcoPseudoRandomService1, IEcoPseudoRandom1, IEcoUnknown) can be requested from any table.
 *   IEcoUnknown always returns the table of interface 0. Returns ERR_ECO_NOINTERFACE and
 *   *ppv = 0 for an unknown IID.
 * </description>
 *
 */
static int16_t CommonQueryInterface(/* in */ CEcoGaloisLFSR1_66391FF5* pCMe, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv) {
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
 *   Common AddRef Function
 * </summary>
 *
 * <description>
 *   Increments the single reference counter of the object.
 * </description>
 *
 */
static uint32_t CommonAddRef(/* in */ CEcoGaloisLFSR1_66391FF5* pCMe) {
    return ++pCMe->m_cRef;
}

/*
 *
 * <summary>
 *   Common Release Function
 * </summary>
 *
 * <description>
 *   Decrements the single reference counter; deletes the object when it reaches zero.
 * </description>
 *
 */
static uint32_t CommonRelease(/* in */ CEcoGaloisLFSR1_66391FF5* pCMe) {
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
 *   QueryInterface Function (IEcoPseudoRandomService1, interface 0)
 * </summary>
 *
 * <description>
 *   me is the start of the object. Returns the requested interface through *ppv.
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoGaloisLFSR1_66391FF5_QueryInterface(/* in */ IEcoPseudoRandomService1Ptr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv) {
    CEcoGaloisLFSR1_66391FF5* pCMe = 0;

    /* Pointer Validation */
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }
    pCMe = (CEcoGaloisLFSR1_66391FF5*)me;

    return CommonQueryInterface(pCMe, riid, ppv);
}

/*
 *
 * <summary>
 *   AddRef Function (IEcoPseudoRandomService1, interface 0)
 * </summary>
 *
 * <description>
 *   me is the start of the object.
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoGaloisLFSR1_66391FF5_AddRef(/* in */ IEcoPseudoRandomService1Ptr_t me) {
    CEcoGaloisLFSR1_66391FF5* pCMe = 0;

    /* Pointer Validation */
    if (me == 0) {
        return -1; /* ERR_ECO_POINTER */
    }
    pCMe = (CEcoGaloisLFSR1_66391FF5*)me;

    return CommonAddRef(pCMe);
}

/*
 *
 * <summary>
 *   Release Function (IEcoPseudoRandomService1, interface 0)
 * </summary>
 *
 * <description>
 *   me is the start of the object.
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoGaloisLFSR1_66391FF5_Release(/* in */ IEcoPseudoRandomService1Ptr_t me) {
    CEcoGaloisLFSR1_66391FF5* pCMe = 0;

    /* Pointer Validation */
    if (me == 0) {
        return -1; /* ERR_ECO_POINTER */
    }
    pCMe = (CEcoGaloisLFSR1_66391FF5*)me;

    return CommonRelease(pCMe);
}

/*
 *
 * <summary>
 *   Step32 Function
 * </summary>
 *
 * <description>
 *   Stateless 32 bit step. The register width is the degree of param (default mask 0x80200003).
 *   On success *newState receives the new state. Returns ERR_ECO_POINTER, ERR_ECO_INVALIDARG
 *   (zero state within the width).
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoGaloisLFSR1_66391FF5_Step32(/* in */ IEcoPseudoRandomService1Ptr_t me, /* in */ uint32_t state, /* in */ uint32_t param, /* out */ uint32_t* newState) {
    uint64_t r = 0;
    int16_t result = ERR_ECO_POINTER;

    /* Pointer Validation */
    if (me == 0 || newState == 0) {
        return result;
    }

    result = Core((uint64_t)state, (uint64_t)param, 32, &r);
    if (result != ERR_ECO_SUCCESS) {
        return result;
    }

    *newState = (uint32_t)r;
    return ERR_ECO_SUCCESS;
}

/*
 *
 * <summary>
 *   Step64 Function
 * </summary>
 *
 * <description>
 *   Stateless 64 bit step. The register width is the degree of param (default mask 0xD800000000000000).
 *   On success *newState receives the new state. Returns ERR_ECO_POINTER, ERR_ECO_INVALIDARG
 *   (zero state within the width).
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoGaloisLFSR1_66391FF5_Step64(/* in */ IEcoPseudoRandomService1Ptr_t me, /* in */ uint64_t state, /* in */ uint64_t param, /* out */ uint64_t* newState) {
    int16_t result = ERR_ECO_POINTER;

    /* Pointer Validation */
    if (me == 0 || newState == 0) {
        return result;
    }

    return Core(state, param, 64, newState);
}

/*
 *
 * <summary>
 *   QueryInterface Function (IEcoPseudoRandom1, interface 1)
 * </summary>
 *
 * <description>
 *   me points to the interface 1 table field, so the object start is one pointer earlier.
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoGaloisLFSR1_66391FF5_QueryInterface1(/* in */ IEcoPseudoRandom1Ptr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv) {
    CEcoGaloisLFSR1_66391FF5* pCMe = 0;

    /* Pointer Validation */
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }
    pCMe = (CEcoGaloisLFSR1_66391FF5*)((char_t*)me - sizeof(voidptr_t));

    return CommonQueryInterface(pCMe, riid, ppv);
}

/*
 *
 * <summary>
 *   AddRef Function (IEcoPseudoRandom1, interface 1)
 * </summary>
 *
 * <description>
 *   me points to the interface 1 table field, so the object start is one pointer earlier.
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoGaloisLFSR1_66391FF5_AddRef1(/* in */ IEcoPseudoRandom1Ptr_t me) {
    CEcoGaloisLFSR1_66391FF5* pCMe = 0;

    /* Pointer Validation */
    if (me == 0) {
        return -1; /* ERR_ECO_POINTER */
    }
    pCMe = (CEcoGaloisLFSR1_66391FF5*)((char_t*)me - sizeof(voidptr_t));

    return CommonAddRef(pCMe);
}

/*
 *
 * <summary>
 *   Release Function (IEcoPseudoRandom1, interface 1)
 * </summary>
 *
 * <description>
 *   me points to the interface 1 table field, so the object start is one pointer earlier.
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoGaloisLFSR1_66391FF5_Release1(/* in */ IEcoPseudoRandom1Ptr_t me) {
    CEcoGaloisLFSR1_66391FF5* pCMe = 0;

    /* Pointer Validation */
    if (me == 0) {
        return -1; /* ERR_ECO_POINTER */
    }
    pCMe = (CEcoGaloisLFSR1_66391FF5*)((char_t*)me - sizeof(voidptr_t));

    return CommonRelease(pCMe);
}

/*
 *
 * <summary>
 *   Init Function (IEcoPseudoRandom1)
 * </summary>
 *
 * <description>
 *   Sets the seed, tap mask and width (32 or 64) and marks the object as initialized.
 *   Restarts the sequence when called again. The mask is replaced by the default one when param == 0
 *   and the seed is truncated to the register width. Returns ERR_ECO_POINTER, ERR_ECO_INVALIDARG
 *   (bad width, mask wider than the width, or zero seed within the width).
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoGaloisLFSR1_66391FF5_Init1(/* in */ IEcoPseudoRandom1Ptr_t me, /* in */ uint64_t seed, /* in */ uint64_t param, /* in */ uint16_t bits) {
    CEcoGaloisLFSR1_66391FF5* pCMe = 0;
    uint64_t wmask = 0;

    /* Pointer Validation */
    if (me == 0) {
        return ERR_ECO_POINTER;
    }
    pCMe = (CEcoGaloisLFSR1_66391FF5*)((char_t*)me - sizeof(voidptr_t));

    if (bits != 32 && bits != 64) {
        return ERR_ECO_INVALIDARG;
    }

    param = EffectiveParam(param, bits);
    if (Degree(param) > bits) {
        return ERR_ECO_INVALIDARG;
    }
    wmask = WidthMask(Degree(param));

    seed = seed & wmask;
    if (seed == 0) {
        return ERR_ECO_INVALIDARG;
    }

    pCMe->m_state = seed;
    pCMe->m_param = param;
    pCMe->m_bits = bits;
    pCMe->m_inited = 1;

    return ERR_ECO_SUCCESS;
}

/*
 *
 * <summary>
 *   Next Function (IEcoPseudoRandom1)
 * </summary>
 *
 * <description>
 *   Advances the internal register and returns the new state through *value
 *   (zero-extended for the 32 bit width). Returns ERR_ECO_POINTER, ERR_ECO_FAIL (Init not called).
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoGaloisLFSR1_66391FF5_Next1(/* in */ IEcoPseudoRandom1Ptr_t me, /* out */ uint64_t* value) {
    CEcoGaloisLFSR1_66391FF5* pCMe = 0;
    uint64_t r = 0;
    int16_t result = ERR_ECO_POINTER;

    /* Pointer Validation */
    if (me == 0 || value == 0) {
        return result;
    }
    pCMe = (CEcoGaloisLFSR1_66391FF5*)((char_t*)me - sizeof(voidptr_t));

    if (pCMe->m_inited == 0) {
        return ERR_ECO_FAIL;
    }

    result = Core(pCMe->m_state, pCMe->m_param, pCMe->m_bits, &r);
    if (result != ERR_ECO_SUCCESS) {
        return result;
    }

    pCMe->m_state = r;
    *value = r;
    return ERR_ECO_SUCCESS;
}

/*
 *
 * <summary>
 *   get_Name Function (IEcoPseudoRandom1)
 * </summary>
 *
 * <description>
 *   Returns the algorithm name as a static string owned by the component, or 0 if me is NULL.
 * </description>
 *
 */
static char_t* ECOCALLMETHOD CEcoGaloisLFSR1_66391FF5_get_Name1(/* in */ IEcoPseudoRandom1Ptr_t me) {
    /* Pointer Validation */
    if (me == 0) {
        return 0; /* ERR_ECO_POINTER */
    }

    return g_GaloisLFSR1Name;
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
static int16_t ECOCALLMETHOD initCEcoGaloisLFSR1_66391FF5(/*in*/ CEcoGaloisLFSR1_66391FF5Ptr_t me, /* in */ IEcoUnknownPtr_t pIUnkSystem) {
    CEcoGaloisLFSR1_66391FF5* pCMe = (CEcoGaloisLFSR1_66391FF5*)me;
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
static int16_t ECOCALLMETHOD createCEcoGaloisLFSR1_66391FF5(/* in */ CEcoGaloisLFSR1_66391FF5Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter) {
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
static void ECOCALLMETHOD deleteCEcoGaloisLFSR1_66391FF5(/* in */ CEcoGaloisLFSR1_66391FF5Ptr_t pCMe) {
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
IEcoPseudoRandomService1VTbl g_xIEcoPseudoRandomService1VTbl_66391FF5 = {
    CEcoGaloisLFSR1_66391FF5_QueryInterface,
    CEcoGaloisLFSR1_66391FF5_AddRef,
    CEcoGaloisLFSR1_66391FF5_Release,
    CEcoGaloisLFSR1_66391FF5_Step32,
    CEcoGaloisLFSR1_66391FF5_Step64
};

/* IEcoPseudoRandom1 Virtual Table (interface 1) */
IEcoPseudoRandom1VTbl g_xIEcoPseudoRandom1VTbl_66391FF5 = {
    CEcoGaloisLFSR1_66391FF5_QueryInterface1,
    CEcoGaloisLFSR1_66391FF5_AddRef1,
    CEcoGaloisLFSR1_66391FF5_Release1,
    CEcoGaloisLFSR1_66391FF5_Init1,
    CEcoGaloisLFSR1_66391FF5_Next1,
    CEcoGaloisLFSR1_66391FF5_get_Name1
};


/* Object Instance */
CEcoGaloisLFSR1_66391FF5 g_xCEcoGaloisLFSR1_66391FF5 = {
    &g_xIEcoPseudoRandomService1VTbl_66391FF5,
    &g_xIEcoPseudoRandom1VTbl_66391FF5,

    initCEcoGaloisLFSR1_66391FF5,
    createCEcoGaloisLFSR1_66391FF5,
    deleteCEcoGaloisLFSR1_66391FF5,
    1, /* m_cRef */
    0, /* m_pIMem */
    0, /* m_pISys */
    0, /* m_Name */
    0, /* m_state */
    0, /* m_param */
    0, /* m_bits */
    0  /* m_inited */
};
