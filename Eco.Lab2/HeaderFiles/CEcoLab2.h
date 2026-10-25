/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CEcoLab2_BBF941C7
 * </summary>
 *
 * <description>
 *   This header describes the implementation of the CEcoLab2_BBF941C7 component
 * </description>
 *
 * <author>
 *   Copyright (c) 2026 . All rights reserved.
 * </author>
 *
 */

#ifndef __C_ECOLAB2_H__
#define __C_ECOLAB2_H__

#include "IEcoLab2.h"
#include "IEcoCalculatorX.h"
#include "IEcoCalculatorY.h"
#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "IdEcoMemoryManager1.h"

typedef struct CEcoLab2_BBF941C7* CEcoLab2_BBF941C7Ptr_t;

typedef struct CEcoLab2_BBF941C7 {

    /* IEcoLab2 interface function table (interface 0) */
    IEcoLab2VTbl* m_pVTblIEcoLab2;

    /* IEcoCalculatorX interface function table (interface 1) */
    IEcoCalculatorXVTbl* m_pVTblIEcoCalculatorX;

    /* IEcoCalculatorY interface function table (interface 2) */
    IEcoCalculatorYVTbl* m_pVTblIEcoCalculatorY;

    /* Non-delegating IEcoUnknown function table (interface 3) */
    IEcoUnknownVTbl* m_pVTblINondelegatingUnk;

    /* Instance initialization */
    int16_t (ECOCALLMETHOD *Init)(/*in*/ CEcoLab2_BBF941C7Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem);
    /* Instance creation */
    int16_t (ECOCALLMETHOD *Create)(/*in*/ CEcoLab2_BBF941C7Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter);
    /* Deletion */
    void (ECOCALLMETHOD *Delete)(/*in*/ CEcoLab2_BBF941C7Ptr_t pCMe);


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

    /* Interface bus used to create the inner components */
    IEcoInterfaceBus1* m_pIBus;

    /* Selected variants */
    uint16_t m_kindX;
    uint16_t m_kindY;

    /* Contained calculator components (containment: the outer component calls them) */
    IEcoCalculatorX* m_pIXc;
    IEcoCalculatorY* m_pIYc;

    /* Aggregated calculator components: their NON-delegating IEcoUnknown (aggregation: their interfaces go to the client directly) */
    IEcoUnknown* m_pXInner;
    IEcoUnknown* m_pYInner;

    /* Aggregated Eco.Lab1Outer: its NON-delegating IEcoUnknown */
    IEcoUnknown* m_pLab1Inner;

} CEcoLab2_BBF941C7;

#endif /* __C_ECOLAB2_H__ */
