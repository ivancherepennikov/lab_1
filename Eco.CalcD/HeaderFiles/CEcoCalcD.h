/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CEcoCalcD_A8852E48
 * </summary>
 *
 * <description>
 *   This header describes the implementation of the CEcoCalcD_A8852E48 component
 * </description>
 *
 * <author>
 *   Copyright (c) 2026 . All rights reserved.
 * </author>
 *
 */

#ifndef __C_ECOCALCD_H__
#define __C_ECOCALCD_H__

#include "IEcoCalculatorY.h"
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"

typedef struct CEcoCalcD_A8852E48* CEcoCalcD_A8852E48Ptr_t;

typedef struct CEcoCalcD_A8852E48 {

    /* IEcoCalculatorY interface function table (interface 0) */
    IEcoCalculatorYVTbl* m_pVTblIEcoCalculatorY;

    /* Instance initialization */
    int16_t (ECOCALLMETHOD *Init)(/*in*/ CEcoCalcD_A8852E48Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem);
    /* Instance creation */
    int16_t (ECOCALLMETHOD *Create)(/*in*/ CEcoCalcD_A8852E48Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter);
    /* Deletion */
    void (ECOCALLMETHOD *Delete)(/*in*/ CEcoCalcD_A8852E48Ptr_t pCMe);


    /* Reference counter */
    uint32_t m_cRef;

    /* Interface for memory operations */
    IEcoMemoryAllocator1* m_pIMem;

    /* System interface */
    IEcoSystem1* m_pISys;

    /* Instance data */
    char_t* m_Name;

} CEcoCalcD_A8852E48;

#endif /* __C_ECOCALCD_H__ */
