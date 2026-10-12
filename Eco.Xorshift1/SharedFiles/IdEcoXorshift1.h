/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IdEcoXorshift1
 * </summary>
 *
 * <description>
 *   This header describes the interface IdEcoXorshift1
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

#ifndef __ID_ECOXORSHIFT1_H__
#define __ID_ECOXORSHIFT1_H__

#include "IEcoBase1.h"
#include "IEcoPseudoRandomService1.h"
#include "IEcoPseudoRandom1.h"

/* EcoXorshift1 CID = {EB9A5A10-5897-45BE-90FE-DB27479BCC71} */
#ifndef __CID_EcoXorshift1
static const UGUID CID_EcoXorshift1 = {0x01, 0x10, {0xEB, 0x9A, 0x5A, 0x10, 0x58, 0x97, 0x45, 0xBE, 0x90, 0xFE, 0xDB, 0x27, 0x47, 0x9B, 0xCC, 0x71}};
#endif /* __CID_EcoXorshift1 */

/* Component factory for dynamic and static layout */
#ifdef ECO_DLL
ECO_EXPORT IEcoComponentFactory* ECOCALLMETHOD GetIEcoComponentFactoryPtr();
#elif ECO_LIB
extern IEcoComponentFactory* GetIEcoComponentFactoryPtr_EB9A5A10589745BE90FEDB27479BCC71;
#endif

#endif /* __ID_ECOXORSHIFT1_H__ */
