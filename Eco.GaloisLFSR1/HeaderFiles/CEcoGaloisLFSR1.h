/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CEcoGaloisLFSR1_66391FF5
 * </summary>
 *
 * <description>
 *   This header describes the implementation of the CEcoGaloisLFSR1_66391FF5 component.
 *   One instance implements two interfaces with one reference counter:
 *   interface 0 - IEcoPseudoRandomService1 (stateless), interface 1 - IEcoPseudoRandom1 (stateful).
 * </description>
 *
 * <author>
 *   Copyright (c) 2026 . All rights reserved.
 * </author>
 *
 */

#ifndef __C_ECOGALOISLFSR1_H__
#define __C_ECOGALOISLFSR1_H__

#include "IEcoPseudoRandomService1.h"
#include "IEcoPseudoRandom1.h"
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"

typedef struct CEcoGaloisLFSR1_66391FF5* CEcoGaloisLFSR1_66391FF5Ptr_t;

typedef struct CEcoGaloisLFSR1_66391FF5 {

    /* IEcoPseudoRandomService1 interface function table (interface 0) */
    IEcoPseudoRandomService1VTbl* m_pVTblIEcoPseudoRandomService1;

    /* IEcoPseudoRandom1 interface function table (interface 1) */
    IEcoPseudoRandom1VTbl* m_pVTblIEcoPseudoRandom1;


    /* Instance initialization */
    int16_t (ECOCALLMETHOD *Init)(/*in*/ CEcoGaloisLFSR1_66391FF5Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem);
    /* Instance creation */
    int16_t (ECOCALLMETHOD *Create)(/*in*/ CEcoGaloisLFSR1_66391FF5Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter);
    /* Deletion */
    void (ECOCALLMETHOD *Delete)(/*in*/ CEcoGaloisLFSR1_66391FF5Ptr_t pCMe);


    /* Reference counter */
    uint32_t m_cRef;

    /* Interface for memory operations */
    IEcoMemoryAllocator1* m_pIMem;

    /* System interface */
    IEcoSystem1* m_pISys;

    /* Instance data (algorithm name is a static string, m_Name stays 0) */
    char_t* m_Name;

    /* LFSR state of IEcoPseudoRandom1: register state, effective tap mask, width, init flag */
    uint64_t m_state;
    uint64_t m_param;
    uint16_t m_bits;
    uint16_t m_inited;

} CEcoGaloisLFSR1_66391FF5;

#endif /* __C_ECOGALOISLFSR1_H__ */
