/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CEcoGaloisLFSR1_66391FF5Factory
 * </summary>
 *
 * <description>
 *   This header describes the implementation of the factory for the component
 * </description>
 *
 * <author>
 *   Copyright (c) 2026 . All rights reserved.
 * </author>
 *
 */

#ifndef __C_ECOGALOISLFSR1_FACTORY_H__
#define __C_ECOGALOISLFSR1_FACTORY_H__

#include "IEcoSystem1.h"

typedef struct CEcoGaloisLFSR1_66391FF5Factory {

    /* IEcoComponentFactory interface function table */
    IEcoComponentFactoryVTbl* m_pVTblICF;

    /* Reference counter */
    uint32_t m_cRef;

    /* Component data for the factory */
    char_t m_Name[64];
    char_t m_Version[16];
    char_t m_Manufacturer[64];

} CEcoGaloisLFSR1_66391FF5Factory;

#endif /* __C_ECOGALOISLFSR1_FACTORY_H__ */
