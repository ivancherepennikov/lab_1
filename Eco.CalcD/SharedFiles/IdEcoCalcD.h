/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IdEcoCalcD
 * </summary>
 *
 * <description>
 *   This header describes the interface IdEcoCalcD
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

#ifndef __ID_ECOCALCD_H__
#define __ID_ECOCALCD_H__

#include "IEcoBase1.h"
#include "IEcoCalculatorY.h"

/* EcoCalcD CID = {F2363DF3-F1CD-41CA-9B15-C660A8852E48} */
#ifndef __CID_EcoCalcD
static const UGUID CID_EcoCalcD = {0x01, 0x10, {0xF2, 0x36, 0x3D, 0xF3, 0xF1, 0xCD, 0x41, 0xCA, 0x9B, 0x15, 0xC6, 0x60, 0xA8, 0x85, 0x2E, 0x48}};
#endif /* __CID_EcoCalcD */

/* Component factory for dynamic and static layout */
#ifdef ECO_DLL
ECO_EXPORT IEcoComponentFactory* ECOCALLMETHOD GetIEcoComponentFactoryPtr();
#elif ECO_LIB
extern IEcoComponentFactory* GetIEcoComponentFactoryPtr_F2363DF3F1CD41CA9B15C660A8852E48;
#endif

#endif /* __ID_ECOCALCD_H__ */

