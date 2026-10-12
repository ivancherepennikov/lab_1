/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CEcoFibonacciLFSR1_919D0053
 * </summary>
 *
 * <description>
 *   Implementation of the Fibonacci LFSR pseudo random generator. One object implements two
 *   interfaces: IEcoPseudoRandomService1 (stateless, interface 0) and IEcoPseudoRandom1
 *   (stateful, interface 1). Both use the single step function FibCore().
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
#include "CEcoFibonacciLFSR1.h"

/* Default polynomials used when param == 0 (32 bit and 64 bit width) */
#define FIB_DEFAULT_PARAM_32 0x80200003UL
#define FIB_DEFAULT_PARAM_64 ((((uint64_t)0xD8000000UL) << 32) | (uint64_t)0x00000000UL)

/* Algorithm name returned by get_Name, owned by the component */
static char_t g_Name[] = "Fibonacci LFSR";

/*
 *
 * <summary>
 *   FibDegree function
 * </summary>
 *
 * <description>
 *   Returns the register width n = (index of the highest set bit of param) + 1, or 0 for param == 0
 * </description>
 *
 */
static uint16_t FibDegree(uint64_t param) {
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
 *   FibWidthMask function
 * </summary>
 *
 * <description>
 *   Returns a mask of the n low bits; n >= 64 gives all ones (avoids a shift by the type width)
 * </description>
 *
 */
static uint64_t FibWidthMask(uint16_t n) {
    if (n >= 64) {
        return ~((uint64_t)0);
    }
    return (((uint64_t)1) << n) - 1;
}

/*
 *
 * <summary>
 *   FibParity function
 * </summary>
 *
 * <description>
 *   Returns the XOR of all bits of x (0 or 1)
 * </description>
 *
 */
static uint64_t FibParity(uint64_t x) {
    x = x ^ (x >> 32);
    x = x ^ (x >> 16);
    x = x ^ (x >> 8);
    x = x ^ (x >> 4);
    x = x ^ (x >> 2);
    x = x ^ (x >> 1);
    return x & 1;
}

/*
 *
 * <summary>
 *   FibResolveParam function
 * </summary>
 *
 * <description>
 *   Substitutes the default polynomial for param == 0; callBits (32 or 64) selects the default
 * </description>
 *
 */
static uint64_t FibResolveParam(uint64_t param, uint16_t callBits) {
    if (param != 0) {
        return param;
    }
    if (callBits == 32) {
        return FIB_DEFAULT_PARAM_32;
    }
    return FIB_DEFAULT_PARAM_64;
}

/*
 *
 * <summary>
 *   FibCore function
 * </summary>
 *
 * <description>
 *   Single Fibonacci LFSR step shared by all methods. The register width n is taken from param
 *   (not from callBits). The state is truncated to n bits; a zero state gives ERR_ECO_INVALIDARG.
 *   The new state is ((state << 1) | fb) truncated to n bits, fb = parity(state & param).
 * </description>
 *
 */
static int16_t FibCore(uint64_t state, uint64_t param, uint16_t callBits, uint64_t* out) {
    uint16_t n;
    uint64_t wmask;
    uint64_t fb;
    uint64_t r;

    param = FibResolveParam(param, callBits);
    n = FibDegree(param);
    wmask = FibWidthMask(n);

    state = state & wmask;
    if (state == 0) {
        return ERR_ECO_INVALIDARG;
    }

    fb = FibParity(state & param);
    r = ((state << 1) | fb) & wmask;

    *out = r;
    return ERR_ECO_SUCCESS;
}

/*
 *
 * <summary>
 *   QueryInterface common branch
 * </summary>
 *
 * <description>
 *   Returns the table field of the requested interface of the object (IEcoUnknown maps to interface 0).
 *   Increments the single reference counter.
 * </description>
 *
 */
static int16_t CEcoFibonacciLFSR1_919D0053_QueryCommon(/* in */ CEcoFibonacciLFSR1_919D0053* pCMe, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv) {
    if ( IsEqualUGUID(riid, &IID_IEcoPseudoRandomService1) ) {
        *ppv = (voidptr_t)&pCMe->m_pVTblIEcoPseudoRandomService1;
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoPseudoRandom1) ) {
        *ppv = (voidptr_t)&pCMe->m_pVTblIEcoPseudoRandom1;
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = (voidptr_t)&pCMe->m_pVTblIEcoPseudoRandomService1;
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
 *   Release common branch
 * </summary>
 *
 * <description>
 *   Decrements the reference counter and deletes the object when it reaches zero
 * </description>
 *
 */
static uint32_t CEcoFibonacciLFSR1_919D0053_ReleaseCommon(/* in */ CEcoFibonacciLFSR1_919D0053* pCMe) {
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
 *   QueryInterface function (interface 0, IEcoPseudoRandomService1)
 * </summary>
 *
 * <description>
 *   me is the object start, no pointer correction
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoFibonacciLFSR1_919D0053_QueryInterface_Service(/* in */ IEcoPseudoRandomService1Ptr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv) {
    CEcoFibonacciLFSR1_919D0053* pCMe;

    /* Pointer Validation */
    if (me == 0 || riid == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    pCMe = (CEcoFibonacciLFSR1_919D0053*)me;
    return CEcoFibonacciLFSR1_919D0053_QueryCommon(pCMe, riid, ppv);
}

/*
 *
 * <summary>
 *   AddRef function (interface 0, IEcoPseudoRandomService1)
 * </summary>
 *
 * <description>
 *   Increments the reference counter
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoFibonacciLFSR1_919D0053_AddRef_Service(/* in */ IEcoPseudoRandomService1Ptr_t me) {
    CEcoFibonacciLFSR1_919D0053* pCMe;

    /* Pointer Validation */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    pCMe = (CEcoFibonacciLFSR1_919D0053*)me;
    return ++pCMe->m_cRef;
}

/*
 *
 * <summary>
 *   Release function (interface 0, IEcoPseudoRandomService1)
 * </summary>
 *
 * <description>
 *   Decrements the reference counter, deletes the object at zero
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoFibonacciLFSR1_919D0053_Release_Service(/* in */ IEcoPseudoRandomService1Ptr_t me) {
    CEcoFibonacciLFSR1_919D0053* pCMe;

    /* Pointer Validation */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    pCMe = (CEcoFibonacciLFSR1_919D0053*)me;
    return CEcoFibonacciLFSR1_919D0053_ReleaseCommon(pCMe);
}

/*
 *
 * <summary>
 *   Step32 function
 * </summary>
 *
 * <description>
 *   Stateless 32 bit step. newState receives the result only on success.
 *   Returns ERR_ECO_POINTER, ERR_ECO_INVALIDARG (zero state after truncation).
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoFibonacciLFSR1_919D0053_Step32(/* in */ IEcoPseudoRandomService1Ptr_t me, /* in */ uint32_t state, /* in */ uint32_t param, /* out */ uint32_t* newState) {
    uint64_t value = 0;
    int16_t result;

    /* Pointer Validation */
    if (me == 0 || newState == 0) {
        return ERR_ECO_POINTER;
    }

    result = FibCore((uint64_t)state, (uint64_t)param, 32, &value);
    if (result == ERR_ECO_SUCCESS) {
        *newState = (uint32_t)value;
    }
    return result;
}

/*
 *
 * <summary>
 *   Step64 function
 * </summary>
 *
 * <description>
 *   Stateless 64 bit step. newState receives the result only on success.
 *   Returns ERR_ECO_POINTER, ERR_ECO_INVALIDARG (zero state after truncation).
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoFibonacciLFSR1_919D0053_Step64(/* in */ IEcoPseudoRandomService1Ptr_t me, /* in */ uint64_t state, /* in */ uint64_t param, /* out */ uint64_t* newState) {
    uint64_t value = 0;
    int16_t result;

    /* Pointer Validation */
    if (me == 0 || newState == 0) {
        return ERR_ECO_POINTER;
    }

    result = FibCore(state, param, 64, &value);
    if (result == ERR_ECO_SUCCESS) {
        *newState = value;
    }
    return result;
}

/*
 *
 * <summary>
 *   QueryInterface function (interface 1, IEcoPseudoRandom1)
 * </summary>
 *
 * <description>
 *   me points to the second table field, so the object starts one pointer earlier
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoFibonacciLFSR1_919D0053_QueryInterface_PR1(/* in */ IEcoPseudoRandom1Ptr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv) {
    CEcoFibonacciLFSR1_919D0053* pCMe;

    /* Pointer Validation */
    if (me == 0 || riid == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    pCMe = (CEcoFibonacciLFSR1_919D0053*)((char_t*)me - sizeof(voidptr_t));
    return CEcoFibonacciLFSR1_919D0053_QueryCommon(pCMe, riid, ppv);
}

/*
 *
 * <summary>
 *   AddRef function (interface 1, IEcoPseudoRandom1)
 * </summary>
 *
 * <description>
 *   Increments the reference counter (object pointer corrected as in QueryInterface_PR1)
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoFibonacciLFSR1_919D0053_AddRef_PR1(/* in */ IEcoPseudoRandom1Ptr_t me) {
    CEcoFibonacciLFSR1_919D0053* pCMe;

    /* Pointer Validation */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    pCMe = (CEcoFibonacciLFSR1_919D0053*)((char_t*)me - sizeof(voidptr_t));
    return ++pCMe->m_cRef;
}

/*
 *
 * <summary>
 *   Release function (interface 1, IEcoPseudoRandom1)
 * </summary>
 *
 * <description>
 *   Decrements the reference counter, deletes the object at zero
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoFibonacciLFSR1_919D0053_Release_PR1(/* in */ IEcoPseudoRandom1Ptr_t me) {
    CEcoFibonacciLFSR1_919D0053* pCMe;

    /* Pointer Validation */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    pCMe = (CEcoFibonacciLFSR1_919D0053*)((char_t*)me - sizeof(voidptr_t));
    return CEcoFibonacciLFSR1_919D0053_ReleaseCommon(pCMe);
}

/*
 *
 * <summary>
 *   Init function (IEcoPseudoRandom1)
 * </summary>
 *
 * <description>
 *   Starts (or restarts) the sequence. bits must be 32 or 64. param == 0 selects the default polynomial.
 *   The seed is truncated to the register width n; a zero seed gives ERR_ECO_INVALIDARG.
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoFibonacciLFSR1_919D0053_Init_PR1(/* in */ IEcoPseudoRandom1Ptr_t me, /* in */ uint64_t seed, /* in */ uint64_t param, /* in */ uint16_t bits) {
    CEcoFibonacciLFSR1_919D0053* pCMe;
    uint64_t wmask;

    /* Pointer Validation */
    if (me == 0 ) {
        return ERR_ECO_POINTER;
    }

    pCMe = (CEcoFibonacciLFSR1_919D0053*)((char_t*)me - sizeof(voidptr_t));

    if (bits != 32 && bits != 64) {
        return ERR_ECO_INVALIDARG;
    }

    param = FibResolveParam(param, bits);
    wmask = FibWidthMask(FibDegree(param));
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
 *   Next function (IEcoPseudoRandom1)
 * </summary>
 *
 * <description>
 *   Advances the register and returns the new state in value. ERR_ECO_FAIL if Init was not called.
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoFibonacciLFSR1_919D0053_Next_PR1(/* in */ IEcoPseudoRandom1Ptr_t me, /* out */ uint64_t* value) {
    CEcoFibonacciLFSR1_919D0053* pCMe;
    uint64_t next = 0;
    int16_t result;

    /* Pointer Validation */
    if (me == 0 || value == 0) {
        return ERR_ECO_POINTER;
    }

    pCMe = (CEcoFibonacciLFSR1_919D0053*)((char_t*)me - sizeof(voidptr_t));

    if (pCMe->m_inited == 0) {
        return ERR_ECO_FAIL;
    }

    result = FibCore(pCMe->m_state, pCMe->m_param, pCMe->m_bits, &next);
    if (result == ERR_ECO_SUCCESS) {
        pCMe->m_state = next;
        *value = next;
    }
    return result;
}

/*
 *
 * <summary>
 *   get_Name function (IEcoPseudoRandom1)
 * </summary>
 *
 * <description>
 *   Returns the static algorithm name (not stored in m_Name, which stays 0)
 * </description>
 *
 */
static char_t* ECOCALLMETHOD CEcoFibonacciLFSR1_919D0053_get_Name_PR1(/* in */ IEcoPseudoRandom1Ptr_t me) {
    /* Pointer Validation */
    if (me == 0 ) {
        return 0; /* ERR_ECO_POINTER */
    }

    return g_Name;
}

/*
 *
 * <summary>
 *   Init function
 * </summary>
 *
 * <description>
 *   Instance initialization function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD initCEcoFibonacciLFSR1_919D0053(/*in*/ CEcoFibonacciLFSR1_919D0053Ptr_t me, /* in */ IEcoUnknownPtr_t pIUnkSystem) {
    CEcoFibonacciLFSR1_919D0053* pCMe = (CEcoFibonacciLFSR1_919D0053*)me;
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
static int16_t ECOCALLMETHOD createCEcoFibonacciLFSR1_919D0053(/* in */ CEcoFibonacciLFSR1_919D0053Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter) {
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
static void ECOCALLMETHOD deleteCEcoFibonacciLFSR1_919D0053(/* in */ CEcoFibonacciLFSR1_919D0053Ptr_t pCMe) {
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
IEcoPseudoRandomService1VTbl g_xIEcoPseudoRandomService1VTbl_919D0053 = {
    CEcoFibonacciLFSR1_919D0053_QueryInterface_Service,
    CEcoFibonacciLFSR1_919D0053_AddRef_Service,
    CEcoFibonacciLFSR1_919D0053_Release_Service,
    CEcoFibonacciLFSR1_919D0053_Step32,
    CEcoFibonacciLFSR1_919D0053_Step64
};

/* IEcoPseudoRandom1 Virtual Table (interface 1) */
IEcoPseudoRandom1VTbl g_xIEcoPseudoRandom1VTbl_919D0053 = {
    CEcoFibonacciLFSR1_919D0053_QueryInterface_PR1,
    CEcoFibonacciLFSR1_919D0053_AddRef_PR1,
    CEcoFibonacciLFSR1_919D0053_Release_PR1,
    CEcoFibonacciLFSR1_919D0053_Init_PR1,
    CEcoFibonacciLFSR1_919D0053_Next_PR1,
    CEcoFibonacciLFSR1_919D0053_get_Name_PR1
};



/* Object Instance */
CEcoFibonacciLFSR1_919D0053 g_xCEcoFibonacciLFSR1_919D0053 = {
    &g_xIEcoPseudoRandomService1VTbl_919D0053,
    &g_xIEcoPseudoRandom1VTbl_919D0053,

    initCEcoFibonacciLFSR1_919D0053,
    createCEcoFibonacciLFSR1_919D0053,
    deleteCEcoFibonacciLFSR1_919D0053,
    1, /* m_cRef */
    0, /* m_pIMem */
    0, /* m_pISys */
    0, /* m_Name */
    0, /* m_state */
    0, /* m_param */
    0, /* m_bits */
    0  /* m_inited */
};
