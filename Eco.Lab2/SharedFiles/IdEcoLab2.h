/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IdEcoLab2
 * </summary>
 *
 * <description>
 *   This header describes the interface IdEcoLab2
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

#ifndef __ID_ECOLAB2_H__
#define __ID_ECOLAB2_H__

#include "IEcoBase1.h"
#include "IEcoLab2.h"

/* EcoLab2 CID = {6242527A-642E-4FD2-933F-D40ABBF941C7} */
#ifndef __CID_EcoLab2
static const UGUID CID_EcoLab2 = {0x01, 0x10, {0x62, 0x42, 0x52, 0x7A, 0x64, 0x2E, 0x4F, 0xD2, 0x93, 0x3F, 0xD4, 0x0A, 0xBB, 0xF9, 0x41, 0xC7}};
#endif /* __CID_EcoLab2 */

/* Component factory for dynamic and static layout */
#ifdef ECO_DLL
ECO_EXPORT IEcoComponentFactory* ECOCALLMETHOD GetIEcoComponentFactoryPtr();
#elif ECO_LIB
extern IEcoComponentFactory* GetIEcoComponentFactoryPtr_6242527A642E4FD2933FD40ABBF941C7;
#endif

#endif /* __ID_ECOLAB2_H__ */

