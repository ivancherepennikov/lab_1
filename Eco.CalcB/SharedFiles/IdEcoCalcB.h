/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IdEcoCalcB
 * </summary>
 *
 * <description>
 *   This header describes the interface IdEcoCalcB
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

#ifndef __ID_ECOCALCB_H__
#define __ID_ECOCALCB_H__

#include "IEcoBase1.h"
#include "IEcoCalculatorX.h"

/* EcoCalcB CID = {6DE9CA94-3778-466F-BCD8-81EE6A5670F9} */
#ifndef __CID_EcoCalcB
static const UGUID CID_EcoCalcB = {0x01, 0x10, {0x6D, 0xE9, 0xCA, 0x94, 0x37, 0x78, 0x46, 0x6F, 0xBC, 0xD8, 0x81, 0xEE, 0x6A, 0x56, 0x70, 0xF9}};
#endif /* __CID_EcoCalcB */

/* Component factory for dynamic and static layout */
#ifdef ECO_DLL
ECO_EXPORT IEcoComponentFactory* ECOCALLMETHOD GetIEcoComponentFactoryPtr();
#elif ECO_LIB
extern IEcoComponentFactory* GetIEcoComponentFactoryPtr_6DE9CA943778466FBCD881EE6A5670F9;
#endif

#endif /* __ID_ECOCALCB_H__ */

