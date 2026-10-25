/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IdEcoCalcC
 * </summary>
 *
 * <description>
 *   This header describes the interface IdEcoCalcC
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

#ifndef __ID_ECOCALCC_H__
#define __ID_ECOCALCC_H__

#include "IEcoBase1.h"
#include "IEcoCalculatorX.h"
#include "IEcoCalculatorY.h"

/* EcoCalcC CID = {F34299EC-0AC3-4EAA-9739-9B54DE118C94} */
#ifndef __CID_EcoCalcC
static const UGUID CID_EcoCalcC = {0x01, 0x10, {0xF3, 0x42, 0x99, 0xEC, 0x0A, 0xC3, 0x4E, 0xAA, 0x97, 0x39, 0x9B, 0x54, 0xDE, 0x11, 0x8C, 0x94}};
#endif /* __CID_EcoCalcC */

/* Component factory for dynamic and static layout */
#ifdef ECO_DLL
ECO_EXPORT IEcoComponentFactory* ECOCALLMETHOD GetIEcoComponentFactoryPtr();
#elif ECO_LIB
extern IEcoComponentFactory* GetIEcoComponentFactoryPtr_F34299EC0AC34EAA97399B54DE118C94;
#endif

#endif /* __ID_ECOCALCC_H__ */

