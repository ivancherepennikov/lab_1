/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IdEcoGaloisLFSR1
 * </summary>
 *
 * <description>
 *   This header describes the interface IdEcoGaloisLFSR1
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

#ifndef __ID_ECOGALOISLFSR1_H__
#define __ID_ECOGALOISLFSR1_H__

#include "IEcoBase1.h"
#include "IEcoGaloisLFSR1.h"

/* EcoGaloisLFSR1 CID = {09232D50-25AE-4DC5-B118-44D166391FF5} */
#ifndef __CID_EcoGaloisLFSR1
static const UGUID CID_EcoGaloisLFSR1 = {0x01, 0x10, {0x09, 0x23, 0x2D, 0x50, 0x25, 0xAE, 0x4D, 0xC5, 0xB1, 0x18, 0x44, 0xD1, 0x66, 0x39, 0x1F, 0xF5}};
#endif /* __CID_EcoGaloisLFSR1 */

/* Component factory for dynamic and static layout */
#ifdef ECO_DLL
ECO_EXPORT IEcoComponentFactory* ECOCALLMETHOD GetIEcoComponentFactoryPtr();
#elif ECO_LIB
extern IEcoComponentFactory* GetIEcoComponentFactoryPtr_09232D5025AE4DC5B11844D166391FF5;
#endif

#endif /* __ID_ECOGALOISLFSR1_H__ */

