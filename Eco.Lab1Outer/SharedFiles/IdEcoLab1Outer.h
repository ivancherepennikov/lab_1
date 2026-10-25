/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IdEcoLab1Outer
 * </summary>
 *
 * <description>
 *   This header describes the interface IdEcoLab1Outer
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

#ifndef __ID_ECOLAB1OUTER_H__
#define __ID_ECOLAB1OUTER_H__

#include "IEcoBase1.h"
#include "IEcoLab1Outer.h"

/* EcoLab1Outer CID = {44F480AF-1FFA-4822-A895-BB9C540B2191} */
#ifndef __CID_EcoLab1Outer
static const UGUID CID_EcoLab1Outer = {0x01, 0x10, {0x44, 0xF4, 0x80, 0xAF, 0x1F, 0xFA, 0x48, 0x22, 0xA8, 0x95, 0xBB, 0x9C, 0x54, 0x0B, 0x21, 0x91}};
#endif /* __CID_EcoLab1Outer */

/* Component factory for dynamic and static layout */
#ifdef ECO_DLL
ECO_EXPORT IEcoComponentFactory* ECOCALLMETHOD GetIEcoComponentFactoryPtr();
#elif ECO_LIB
extern IEcoComponentFactory* GetIEcoComponentFactoryPtr_44F480AF1FFA4822A895BB9C540B2191;
#endif

#endif /* __ID_ECOLAB1OUTER_H__ */

