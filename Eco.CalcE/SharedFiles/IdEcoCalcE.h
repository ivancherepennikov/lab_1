/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IdEcoCalcE
 * </summary>
 *
 * <description>
 *   This header describes the interface IdEcoCalcE
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

#ifndef __ID_ECOCALCE_H__
#define __ID_ECOCALCE_H__

#include "IEcoBase1.h"
#include "IEcoCalculatorY.h"

/* EcoCalcE CID = {A143D4A7-1265-4E93-ABCE-ABEA8C776B06} */
#ifndef __CID_EcoCalcE
static const UGUID CID_EcoCalcE = {0x01, 0x10, {0xA1, 0x43, 0xD4, 0xA7, 0x12, 0x65, 0x4E, 0x93, 0xAB, 0xCE, 0xAB, 0xEA, 0x8C, 0x77, 0x6B, 0x06}};
#endif /* __CID_EcoCalcE */

/* Component factory for dynamic and static layout */
#ifdef ECO_DLL
ECO_EXPORT IEcoComponentFactory* ECOCALLMETHOD GetIEcoComponentFactoryPtr();
#elif ECO_LIB
extern IEcoComponentFactory* GetIEcoComponentFactoryPtr_A143D4A712654E93ABCEABEA8C776B06;
#endif

#endif /* __ID_ECOCALCE_H__ */

