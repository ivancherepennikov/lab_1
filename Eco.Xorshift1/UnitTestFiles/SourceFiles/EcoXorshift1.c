/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   EcoXorshift1 unit test client
 * </summary>
 *
 * <description>
 *   EcoMain returns 0 when every check passes, otherwise 1 (Service tests),
 *   2 (Object tests) or 3 (QueryInterface/AddRef rules); the section and the
 *   1-based index of the first failed check are printed with printf.
 *   The test client is allowed to use libc output; the components are not.
 * </description>
 *
 * <author>
 *   Copyright (c) 2026 . All rights reserved.
 * </author>
 *
 */

#include <stdio.h>
#include "IEcoSystem1.h"
#include "IdEcoInterfaceBus1.h"
#include "IdEcoXorshift1.h"
#include "PseudoRandomTests.h"

int16_t EcoMain(IEcoUnknown* pIUnk) {
    int16_t result = -1;
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoPseudoRandomService1* pSvc = 0;
    IEcoPseudoRandom1* pObj = 0;
    IEcoPseudoRandom1* pFresh = 0;

    result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem, (void **)&pISys);
    if (result != 0 || pISys == 0) {
        printf("FAIL: cannot get system interface, result=%d\n", (int)result);
        goto Release;
    }
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    if (result != 0 || pIBus == 0) {
        printf("FAIL: cannot get interface bus, result=%d\n", (int)result);
        goto Release;
    }
#ifdef ECO_LIB
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoXorshift1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_EB9A5A10589745BE90FEDB27479BCC71);
    if (result != 0) {
        printf("FAIL: RegisterComponent, result=%d\n", (int)result);
        goto Release;
    }
#endif
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoXorshift1, 0, &IID_IEcoPseudoRandomService1, (void**) &pSvc);
    if (result != 0 || pSvc == 0) {
        printf("FAIL: cannot get service interface, result=%d\n", (int)result);
        goto Release;
    }
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoXorshift1, 0, &IID_IEcoPseudoRandom1, (void**) &pObj);
    if (result != 0 || pObj == 0) {
        printf("FAIL: cannot get object interface, result=%d\n", (int)result);
        goto Release;
    }
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoXorshift1, 0, &IID_IEcoPseudoRandom1, (void**) &pFresh);
    if (result != 0 || pFresh == 0) {
        printf("FAIL: cannot get fresh object, result=%d\n", (int)result);
        goto Release;
    }

    result = PRNGTestService(pSvc);
    if (result != 0) {
        printf("FAIL: service check #%d\n", (int)result);
        result = 1;
        goto Release;
    }
    result = PRNGTestObject(pObj, pFresh);
    if (result != 0) {
        printf("FAIL: object check #%d\n", (int)result);
        result = 2;
        goto Release;
    }
    result = PRNGTestIdentity(pSvc);
    if (result != 0) {
        printf("FAIL: identity check #%d\n", (int)result);
        result = 3;
        goto Release;
    }
    printf("PASS: all %lu contract checks passed\n", g_PRNGChecks);

Release:
    if (pFresh != 0) {
        pFresh->pVTbl->Release(pFresh);
    }
    if (pObj != 0) {
        pObj->pVTbl->Release(pObj);
    }
    if (pSvc != 0) {
        pSvc->pVTbl->Release(pSvc);
    }
    if (pIBus != 0) {
        pIBus->pVTbl->Release(pIBus);
    }
    if (pISys != 0) {
        pISys->pVTbl->Release(pISys);
    }
    return result;
}
