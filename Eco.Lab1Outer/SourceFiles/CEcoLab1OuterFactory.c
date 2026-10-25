/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CEcoLab1Outer_540B2191Factory
 * </summary>
 *
 * <description>
 *   This header describes the implementation of the factory for the component
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

#include "CEcoLab1Outer.h"
#include "CEcoLab1OuterFactory.h"

extern CEcoLab1Outer_540B2191 g_xCEcoLab1Outer_540B2191;

/*
 *
 * <summary>
 *   QueryInterface function
 * </summary>
 *
 * <description>
 *   The function returns a pointer to the interface
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoLab1Outer_540B2191Factory_QueryInterface(IEcoComponentFactory* me, const UGUID* riid, void** ppv) {
    if ( IsEqualUGUID(riid, &IID_IEcoUnknown) || IsEqualUGUID(riid, &IID_IEcoComponentFactory) ) {
        *ppv = me;
    }
    else {
        *ppv = 0;
        return ERR_ECO_NOINTERFACE;
    }
    ((IEcoUnknown*)(*ppv))->pVTbl->AddRef((IEcoUnknown*)*ppv);

    return ERR_ECO_SUCCESS;
}

/*
 *
 * <summary>
 *   AddRef function
 * </summary>
 *
 * <description>
 *   The function increments the reference count for the interface
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoLab1Outer_540B2191Factory_AddRef(/* in */ IEcoComponentFactory* me) {
    CEcoLab1Outer_540B2191Factory* pCMe = (CEcoLab1Outer_540B2191Factory*)me;

    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    return ++pCMe->m_cRef;
}

/*
 *
 * <summary>
 *   Release function
 * </summary>
 *
 * <description>
 *   The function decrements the reference count for the interface
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoLab1Outer_540B2191Factory_Release(/* in */ IEcoComponentFactory* me) {
    CEcoLab1Outer_540B2191Factory* pCMe = (CEcoLab1Outer_540B2191Factory*)me;

    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    /* Decrementing the component reference counter */
    --pCMe->m_cRef;

    /* If the counter is zeroed, free the instance data */
    if ( pCMe->m_cRef == 0 ) {
        return 0;
    }
    return pCMe->m_cRef;
}

/*
 *
 * <summary>
 *   Init function
 * </summary>
 *
 * <description>
 *   The function initializes the component with parameters
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoLab1Outer_540B2191Factory_Init(/* in */ struct IEcoComponentFactory* me, /* in */ struct IEcoUnknown *pIUnkSystem, /* in */ void* pv) {
    CEcoLab1Outer_540B2191Factory* pCMe = (CEcoLab1Outer_540B2191Factory*)me;
    int16_t result = ERR_ECO_POINTER;

    if (me == 0 ) {
        return result;
    }

    /* Initializing the component with parameters */

    return result;
}

/*
 *
 * <summary>
 *   Alloc function
 * </summary>
 *
 * <description>
 *   The function creates a component
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoLab1Outer_540B2191Factory_Alloc(/* in */ struct IEcoComponentFactory* me, /* in */ struct IEcoUnknown *pISystem, /* in */ struct IEcoUnknown *pIUnknownOuter, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoLab1Outer_540B2191Factory* pCMe = (CEcoLab1Outer_540B2191Factory*)me;
    IEcoUnknown* pIUnk = 0;
    int16_t result = ERR_ECO_POINTER;
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoInterfaceBus1MemExt* pIMemExt = 0;
    IEcoMemoryAllocator1* pIMem = 0;
    CEcoLab1Outer_540B2191* pCObj = 0;
    UGUID* rcid = (UGUID*)&CID_EcoMemoryManager1;

    if (me == 0 || pISystem == 0 ) {
        return result; /* ERR_ECO_POINTER */
    }

    /* Aggregation provided that IID is IID_IEcoUnknown */
    if ( ( pIUnknownOuter != 0 ) && !IsEqualUGUID(riid, &IID_IEcoUnknown ) ) {
        /* aggregation not supported */
        return ERR_ECO_NOAGGREGATION;
    }

    /* Getting the application system interface */
    result = pISystem->pVTbl->QueryInterface(pISystem, &GID_IEcoSystem, (void **)&pISys);
    /* Check */
    if (result != 0 || pISys == 0) {
        return ERR_ECO_NOSYSTEM;
    }

    /* Getting the interface for working with the interface bus */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    /* Check */
    if (result != 0 || pIBus == 0) {
        pISys->pVTbl->Release(pISys);
        return ERR_ECO_NOBUS;
    }

    /* Getting the component ID for memory operations */
    result = pIBus->pVTbl->QueryInterface(pIBus, &IID_IEcoInterfaceBus1MemExt, (void**)&pIMemExt);
    if (result == 0 && pIMemExt != 0) {
        rcid = (UGUID*)pIMemExt->pVTbl->get_Manager(pIMemExt);
        pIMemExt->pVTbl->Release(pIMemExt);
    }

    /* Getting the memory allocator interface */
    pIBus->pVTbl->QueryComponent(pIBus, rcid, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);
    /* Check */
    if (result != 0 || pIMem == 0) {
        /* Freeing in case of an error */
        pIBus->pVTbl->Release(pIBus);
        pISys->pVTbl->Release(pISys);
        return ERR_ECO_GET_MEMORY_ALLOCATOR;
    }

    /* Allocating memory for instance data */
    pCObj = (CEcoLab1Outer_540B2191*)pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoLab1Outer_540B2191));
    if (pCObj == 0) {
        /* Freeing in case of an error */
        pIBus->pVTbl->Release(pIBus);
        pISys->pVTbl->Release(pISys);
        return ERR_ECO_OUTOFMEMORY;
    }

    /* Forming instance data */
    pCObj = (CEcoLab1Outer_540B2191*)pIMem->pVTbl->Copy(pIMem, pCObj, &g_xCEcoLab1Outer_540B2191, sizeof(CEcoLab1Outer_540B2191));

    /* Component creation */
    result = pCObj->Create(pCObj, pISystem, pIUnknownOuter);
    if (result != ERR_ECO_SUCCESS) {
        pIMem->pVTbl->Free(pIMem, pCObj);
        pIMem->pVTbl->Release(pIMem);
        pIBus->pVTbl->Release(pIBus);
        pISys->pVTbl->Release(pISys);
        return result;
    }

    /* Component initialization: on failure the half-built object is destroyed */
    result = pCObj->Init(pCObj, pISystem);
    if (result != ERR_ECO_SUCCESS) {
        pCObj->m_cRef = 1;
        pCObj->Delete(pCObj);
        return result;
    }

    /* Getting a pointer to the interface through the NON-delegating IEcoUnknown (required for aggregation) */
    pIUnk = (IEcoUnknown*)&pCObj->m_pVTblINondelegatingUnk;
    result = pIUnk->pVTbl->QueryInterface(pIUnk, riid, ppv);

    /* Decrementing the reference requested by the Component Factory */
    pIUnk->pVTbl->Release(pIUnk);

    return result;
}
/*
 *
 * <summary>
 *   get_Name function
 * </summary>
 *
 * <description>
 *   The function returns the name of the component
 * </description>
 *
 */
static char_t* ECOCALLMETHOD CEcoLab1Outer_540B2191Factory_get_Name(/* in */ struct IEcoComponentFactory* me) {
    CEcoLab1Outer_540B2191Factory* pCMe = (CEcoLab1Outer_540B2191Factory*)me;

    if (me == 0 ) {
        return 0; /* ERR_ECO_POINTER */
    }

    return pCMe->m_Name;
}

/*
 *
 * <summary>
 *   get_Version function
 * </summary>
 *
 * <description>
 *   The function returns the version of the component
 * </description>
 *
 */
static char_t* ECOCALLMETHOD CEcoLab1Outer_540B2191Factory_get_Version(/* in */ struct IEcoComponentFactory* me) {
    CEcoLab1Outer_540B2191Factory* pCMe = (CEcoLab1Outer_540B2191Factory*)me;

    if (me == 0 ) {
        return 0; /* ERR_ECO_POINTER */
    }

    return pCMe->m_Version;
}

/*
 *
 * <summary>
 *   get_Manufacturer function
 * </summary>
 *
 * <description>
 *   The function returns the name of the component manufacturer
 * </description>
 *
 */
static char_t* ECOCALLMETHOD CEcoLab1Outer_540B2191Factory_get_Manufacturer(/* in */ struct IEcoComponentFactory* me) {
    CEcoLab1Outer_540B2191Factory* pCMe = (CEcoLab1Outer_540B2191Factory*)me;

    if (me == 0 ) {
        return 0; /* ERR_ECO_POINTER */
    }

    return pCMe->m_Manufacturer;
}

/* Create Virtual Table */
IEcoComponentFactoryVTbl g_x44F480AF1FFA4822A895BB9C540B2191FactoryVTbl = {
    CEcoLab1Outer_540B2191Factory_QueryInterface,
    CEcoLab1Outer_540B2191Factory_AddRef,
    CEcoLab1Outer_540B2191Factory_Release,
    CEcoLab1Outer_540B2191Factory_Alloc,
    CEcoLab1Outer_540B2191Factory_Init,
    CEcoLab1Outer_540B2191Factory_get_Name,
    CEcoLab1Outer_540B2191Factory_get_Version,
    CEcoLab1Outer_540B2191Factory_get_Manufacturer
};

/*
 *
 * <summary>
 *   Create function
 * </summary>
 *
 * <description>
 *   The function 
 * </description>
 *
 */
CEcoLab1Outer_540B2191Factory g_x44F480AF1FFA4822A895BB9C540B2191Factory = {
    &g_x44F480AF1FFA4822A895BB9C540B2191FactoryVTbl,
    0,
    "EcoLab1Outer\0",
    "1.0.0.0\0",
    "\0"
};

#ifdef ECO_DLL
ECO_EXPORT IEcoComponentFactory* ECOCALLMETHOD GetIEcoComponentFactoryPtr() {
    return (IEcoComponentFactory*)&g_x44F480AF1FFA4822A895BB9C540B2191Factory;
};
#elif ECO_LIB
IEcoComponentFactory* GetIEcoComponentFactoryPtr_44F480AF1FFA4822A895BB9C540B2191 = (IEcoComponentFactory*)&g_x44F480AF1FFA4822A895BB9C540B2191Factory;
#endif
