/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CEcoCalcA_06878ABA
 * </summary>
 *
 * <description>
 *   This header describes the implementation of the CEcoCalcA_06878ABA component
 * </description>
 *
 * <author>
 *   Copyright (c) 2026 . All rights reserved.
 * </author>
 *
 */

#ifndef __C_ECOCALCA_H__
#define __C_ECOCALCA_H__

#include "IEcoCalculatorX.h"
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"

typedef struct CEcoCalcA_06878ABA* CEcoCalcA_06878ABAPtr_t;

typedef struct CEcoCalcA_06878ABA {

    /* IEcoCalculatorX interface function table (interface 0) */
    IEcoCalculatorXVTbl* m_pVTblIEcoCalculatorX;

    /* Instance initialization */
    int16_t (ECOCALLMETHOD *Init)(/*in*/ CEcoCalcA_06878ABAPtr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem);
    /* Instance creation */
    int16_t (ECOCALLMETHOD *Create)(/*in*/ CEcoCalcA_06878ABAPtr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter);
    /* Deletion */
    void (ECOCALLMETHOD *Delete)(/*in*/ CEcoCalcA_06878ABAPtr_t pCMe);


    /* Reference counter */
    uint32_t m_cRef;

    /* Interface for memory operations */
    IEcoMemoryAllocator1* m_pIMem;

    /* System interface */
    IEcoSystem1* m_pISys;

    /* Instance data */
    char_t* m_Name;

} CEcoCalcA_06878ABA;

#endif /* __C_ECOCALCA_H__ */
