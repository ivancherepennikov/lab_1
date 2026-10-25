/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   EcoCalcD unit test client
 * </summary>
 *
 * <description>
 *   Console test client of the component: prints PASS and the number of checks or the index of
 *   the first failed check. The test client uses libc output; the component does not.
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
#include "IdEcoCalcD.h"
#include "CalcTests.h"

int16_t EcoMain(IEcoUnknown* pIUnk) {
    int16_t result = -1;
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;

    result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem, (void **)&pISys);
    if (result != 0 || pISys == 0) {
        goto Release;
    }
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    if (result != 0 || pIBus == 0) {
        goto Release;
    }
#ifdef ECO_LIB
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoCalcD, (IEcoUnknown*)GetIEcoComponentFactoryPtr_F2363DF3F1CD41CA9B15C660A8852E48);
    if (result != 0) {
        printf("FAIL: RegisterComponent, result=%d\n", (int)result);
        goto Release;
    }
#endif
    result = CalcTestAll(pIBus, pIUnk);
    if (result != 0) {
        printf("FAIL: check #%d\n", (int)result);
        result = 1;
    } else {
        printf("PASS: all %lu checks passed (request with an outer unknown: result %d)\n", g_CalcChecks, (int)g_CalcAggResult);
    }

Release:
    if (pIBus != 0) {
        pIBus->pVTbl->Release(pIBus);
    }
    if (pISys != 0) {
        pISys->pVTbl->Release(pISys);
    }
    return result;
}
