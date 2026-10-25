/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CEcoCalcB_6A5670F9
 * </summary>
 *
 * <description>
 *   This header describes the implementation of the CEcoCalcB_6A5670F9 component
 * </description>
 *
 * <author>
 *   Copyright (c) 2026 . All rights reserved.
 * </author>
 *
 */

#ifndef __C_ECOCALCB_H__
#define __C_ECOCALCB_H__

#include "IEcoCalculatorX.h"
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"

typedef struct CEcoCalcB_6A5670F9* CEcoCalcB_6A5670F9Ptr_t;

typedef struct CEcoCalcB_6A5670F9 {

    /* IEcoCalculatorX interface function table (interface 0) */
    IEcoCalculatorXVTbl* m_pVTblIEcoCalculatorX;

    /* Non-delegating IEcoUnknown function table (interface 1) */
    IEcoUnknownVTbl* m_pVTblINondelegatingUnk;

    /* Instance initialization */
    int16_t (ECOCALLMETHOD *Init)(/*in*/ CEcoCalcB_6A5670F9Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem);
    /* Instance creation */
    int16_t (ECOCALLMETHOD *Create)(/*in*/ CEcoCalcB_6A5670F9Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter);
    /* Deletion */
    void (ECOCALLMETHOD *Delete)(/*in*/ CEcoCalcB_6A5670F9Ptr_t pCMe);


    /* Reference counter */
    uint32_t m_cRef;

    /* Interface for memory operations */
    IEcoMemoryAllocator1* m_pIMem;

    /* System interface */
    IEcoSystem1* m_pISys;

    /* Instance data */
    char_t* m_Name;

    /* Controlling IEcoUnknown: the outer component when aggregated, the non-delegating IEcoUnknown otherwise */
    IEcoUnknown* m_pIUnkOuter;

} CEcoCalcB_6A5670F9;

#endif /* __C_ECOCALCB_H__ */
