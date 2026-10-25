/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CEcoCalcC_DE118C94
 * </summary>
 *
 * <description>
 *   This header describes the implementation of the CEcoCalcC_DE118C94 component
 * </description>
 *
 * <author>
 *   Copyright (c) 2026 . All rights reserved.
 * </author>
 *
 */

#ifndef __C_ECOCALCC_H__
#define __C_ECOCALCC_H__

#include "IEcoCalculatorX.h"
#include "IEcoCalculatorY.h"
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"

typedef struct CEcoCalcC_DE118C94* CEcoCalcC_DE118C94Ptr_t;

typedef struct CEcoCalcC_DE118C94 {

    /* IEcoCalculatorX interface function table (interface 0) */
    IEcoCalculatorXVTbl* m_pVTblIEcoCalculatorX;

    /* IEcoCalculatorY interface function table (interface 1) */
    IEcoCalculatorYVTbl* m_pVTblIEcoCalculatorY;

    /* Instance initialization */
    int16_t (ECOCALLMETHOD *Init)(/*in*/ CEcoCalcC_DE118C94Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem);
    /* Instance creation */
    int16_t (ECOCALLMETHOD *Create)(/*in*/ CEcoCalcC_DE118C94Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter);
    /* Deletion */
    void (ECOCALLMETHOD *Delete)(/*in*/ CEcoCalcC_DE118C94Ptr_t pCMe);


    /* Reference counter */
    uint32_t m_cRef;

    /* Interface for memory operations */
    IEcoMemoryAllocator1* m_pIMem;

    /* System interface */
    IEcoSystem1* m_pISys;

    /* Instance data */
    char_t* m_Name;

} CEcoCalcC_DE118C94;

#endif /* __C_ECOCALCC_H__ */
