/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IdEcoCalcA
 * </summary>
 *
 * <description>
 *   This header describes the interface IdEcoCalcA
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

#ifndef __ID_ECOCALCA_H__
#define __ID_ECOCALCA_H__

#include "IEcoBase1.h"
#include "IEcoCalculatorX.h"

/* EcoCalcA CID = {BD776821-99BD-4A97-8265-14C606878ABA} */
#ifndef __CID_EcoCalcA
static const UGUID CID_EcoCalcA = {0x01, 0x10, {0xBD, 0x77, 0x68, 0x21, 0x99, 0xBD, 0x4A, 0x97, 0x82, 0x65, 0x14, 0xC6, 0x06, 0x87, 0x8A, 0xBA}};
#endif /* __CID_EcoCalcA */

/* Component factory for dynamic and static layout */
#ifdef ECO_DLL
ECO_EXPORT IEcoComponentFactory* ECOCALLMETHOD GetIEcoComponentFactoryPtr();
#elif ECO_LIB
extern IEcoComponentFactory* GetIEcoComponentFactoryPtr_BD77682199BD4A97826514C606878ABA;
#endif

#endif /* __ID_ECOCALCA_H__ */

