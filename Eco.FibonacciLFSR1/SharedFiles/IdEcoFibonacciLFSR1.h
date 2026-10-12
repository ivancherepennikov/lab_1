/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IdEcoFibonacciLFSR1
 * </summary>
 *
 * <description>
 *   This header describes the interface IdEcoFibonacciLFSR1
 * </description>
 *
 * <reference>
 *
 * </reference>
 *
 * <author>
 *   Copyright (c) 2026 . All rights reserved.
 * </author>
 *
 */

#ifndef __ID_ECOFIBONACCILFSR1_H__
#define __ID_ECOFIBONACCILFSR1_H__

#include "IEcoBase1.h"
#include "IEcoPseudoRandomService1.h"
#include "IEcoPseudoRandom1.h"

/* EcoFibonacciLFSR1 CID = {A0F26A7C-E65B-43D8-B022-EFFE919D0053} */
#ifndef __CID_EcoFibonacciLFSR1
static const UGUID CID_EcoFibonacciLFSR1 = {0x01, 0x10, {0xA0, 0xF2, 0x6A, 0x7C, 0xE6, 0x5B, 0x43, 0xD8, 0xB0, 0x22, 0xEF, 0xFE, 0x91, 0x9D, 0x00, 0x53}};
#endif /* __CID_EcoFibonacciLFSR1 */

/* Component factory for dynamic and static layout */
#ifdef ECO_DLL
ECO_EXPORT IEcoComponentFactory* ECOCALLMETHOD GetIEcoComponentFactoryPtr();
#elif ECO_LIB
extern IEcoComponentFactory* GetIEcoComponentFactoryPtr_A0F26A7CE65B43D8B022EFFE919D0053;
#endif

#endif /* __ID_ECOFIBONACCILFSR1_H__ */

