/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IdEcoLab1
 * </summary>
 *
 * <description>
 *   This header describes the interface IdEcoLab1
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

#ifndef __ID_ECOLAB1_H__
#define __ID_ECOLAB1_H__

#include "IEcoBase1.h"
#include "IEcoLab1.h"

/* EcoLab1 CID = {48C2391D-9EEB-4512-833D-BAA69B8059BF} */
#ifndef __CID_EcoLab1
static const UGUID CID_EcoLab1 = {0x01, 0x10, {0x48, 0xC2, 0x39, 0x1D, 0x9E, 0xEB, 0x45, 0x12, 0x83, 0x3D, 0xBA, 0xA6, 0x9B, 0x80, 0x59, 0xBF}};
#endif /* __CID_EcoLab1 */

/* Component factory for dynamic and static layout */
#ifdef ECO_DLL
ECO_EXPORT IEcoComponentFactory* ECOCALLMETHOD GetIEcoComponentFactoryPtr();
#elif ECO_LIB
extern IEcoComponentFactory* GetIEcoComponentFactoryPtr_48C2391D9EEB4512833DBAA69B8059BF;
#endif

#endif /* __ID_ECOLAB1_H__ */

