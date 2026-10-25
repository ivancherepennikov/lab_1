/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CEcoCalcE_8C776B06
 * </summary>
 *
 * <description>
 *   This header describes the implementation of the CEcoCalcE_8C776B06 component
 * </description>
 *
 * <author>
 *   Copyright (c) 2026 . All rights reserved.
 * </author>
 *
 */

#ifndef __C_ECOCALCE_H__
#define __C_ECOCALCE_H__

#include "IEcoCalculatorY.h"
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"

typedef struct CEcoCalcE_8C776B06* CEcoCalcE_8C776B06Ptr_t;

typedef struct CEcoCalcE_8C776B06 {

    /* IEcoCalculatorY interface function table (interface 0) */
    IEcoCalculatorYVTbl* m_pVTblIEcoCalculatorY;

    /* Non-delegating IEcoUnknown function table (interface 1) */
    IEcoUnknownVTbl* m_pVTblINondelegatingUnk;

    /* Instance initialization */
    int16_t (ECOCALLMETHOD *Init)(/*in*/ CEcoCalcE_8C776B06Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem);
    /* Instance creation */
    int16_t (ECOCALLMETHOD *Create)(/*in*/ CEcoCalcE_8C776B06Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter);
    /* Deletion */
    void (ECOCALLMETHOD *Delete)(/*in*/ CEcoCalcE_8C776B06Ptr_t pCMe);


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

} CEcoCalcE_8C776B06;

#endif /* __C_ECOCALCE_H__ */
