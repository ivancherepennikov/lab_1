/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   EcoLab2 unit test client
 * </summary>
 *
 * <description>
 *   Registers the static components needed by EcoLab2 on the interface bus and runs the tests. Prints PASS and the
 *   number of checks or the index of the first failed check. The test client uses libc output; the components do not.
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
#include "OuterTests.h"
#include "OuterDemo.h"

#define REG_COUNT 10

typedef struct RegEntry {
    const UGUID* cid;
    IEcoComponentFactory* factory;
} RegEntry;

static RegEntry g_Reg[REG_COUNT];

int16_t EcoMain(IEcoUnknown* pIUnk) {
    int16_t result = -1;
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;
    int i = 0;

    result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem, (void **)&pISys);
    if (result != 0 || pISys == 0) {
        goto Release;
    }
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    if (result != 0 || pIBus == 0) {
        goto Release;
    }
    g_Reg[0].cid = &CID_EcoCalcA;
    g_Reg[0].factory = GetIEcoComponentFactoryPtr_BD77682199BD4A97826514C606878ABA;
    g_Reg[1].cid = &CID_EcoCalcB;
    g_Reg[1].factory = GetIEcoComponentFactoryPtr_6DE9CA943778466FBCD881EE6A5670F9;
    g_Reg[2].cid = &CID_EcoCalcC;
    g_Reg[2].factory = GetIEcoComponentFactoryPtr_F34299EC0AC34EAA97399B54DE118C94;
    g_Reg[3].cid = &CID_EcoCalcD;
    g_Reg[3].factory = GetIEcoComponentFactoryPtr_F2363DF3F1CD41CA9B15C660A8852E48;
    g_Reg[4].cid = &CID_EcoCalcE;
    g_Reg[4].factory = GetIEcoComponentFactoryPtr_A143D4A712654E93ABCEABEA8C776B06;
    g_Reg[5].cid = &CID_EcoFibonacciLFSR1;
    g_Reg[5].factory = GetIEcoComponentFactoryPtr_A0F26A7CE65B43D8B022EFFE919D0053;
    g_Reg[6].cid = &CID_EcoGaloisLFSR1;
    g_Reg[6].factory = GetIEcoComponentFactoryPtr_09232D5025AE4DC5B11844D166391FF5;
    g_Reg[7].cid = &CID_EcoXorshift1;
    g_Reg[7].factory = GetIEcoComponentFactoryPtr_EB9A5A10589745BE90FEDB27479BCC71;
    g_Reg[8].cid = &CID_EcoLab1Outer;
    g_Reg[8].factory = GetIEcoComponentFactoryPtr_44F480AF1FFA4822A895BB9C540B2191;
    g_Reg[9].cid = &CID_EcoLab2;
    g_Reg[9].factory = GetIEcoComponentFactoryPtr_6242527A642E4FD2933FD40ABBF941C7;

#ifdef ECO_LIB
    for (i = 0; i < REG_COUNT; i++) {
        result = pIBus->pVTbl->RegisterComponent(pIBus, g_Reg[i].cid, (IEcoUnknown*)g_Reg[i].factory);
        if (result != 0) {
            printf("FAIL: RegisterComponent #%d, result=%d\n", i, (int)result);
            goto Release;
        }
    }
#endif
    OuterDemo(pIBus);
    result = OuterTestAll(pIBus, pIUnk);
    if (result != 0) {
        printf("FAIL: check #%d\n", (int)result);
        result = 1;
    } else {
        printf("PASS: all %lu checks passed\n", g_OuterChecks);
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
