/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CEcoLab1Outer_540B2191
 * </summary>
 *
 * <description>
 *   This header describes the implementation of the CEcoLab1Outer_540B2191 component
 * </description>
 *
 * <author>
 *   Copyright (c) 2026 . All rights reserved.
 * </author>
 *
 */

#ifndef __C_ECOLAB1OUTER_H__
#define __C_ECOLAB1OUTER_H__

#include "IEcoLab1Outer.h"
#include "IEcoCalculatorX.h"
#include "IEcoCalculatorY.h"
#include "IEcoPseudoRandomService1.h"
#include "IEcoPseudoRandom1.h"
#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "IdEcoMemoryManager1.h"

typedef struct CEcoLab1Outer_540B2191* CEcoLab1Outer_540B2191Ptr_t;

typedef struct CEcoLab1Outer_540B2191 {

    /* IEcoLab1Outer interface function table (interface 0) */
    IEcoLab1OuterVTbl* m_pVTblIEcoLab1Outer;

    /* IEcoCalculatorX interface function table (interface 1) */
    IEcoCalculatorXVTbl* m_pVTblIEcoCalculatorX;

    /* IEcoCalculatorY interface function table (interface 2) */
    IEcoCalculatorYVTbl* m_pVTblIEcoCalculatorY;

    /* IEcoPseudoRandomService1 interface function table (interface 3) */
    IEcoPseudoRandomService1VTbl* m_pVTblIEcoPseudoRandomService1;

    /* IEcoPseudoRandom1 interface function table (interface 4) */
    IEcoPseudoRandom1VTbl* m_pVTblIEcoPseudoRandom1;

    /* Non-delegating IEcoUnknown function table (interface 5) */
    IEcoUnknownVTbl* m_pVTblINondelegatingUnk;

    /* Instance initialization */
    int16_t (ECOCALLMETHOD *Init)(/*in*/ CEcoLab1Outer_540B2191Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem);
    /* Instance creation */
    int16_t (ECOCALLMETHOD *Create)(/*in*/ CEcoLab1Outer_540B2191Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter);
    /* Deletion */
    void (ECOCALLMETHOD *Delete)(/*in*/ CEcoLab1Outer_540B2191Ptr_t pCMe);


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

    /* Contained lab 1 generator (component of lab 1, used unchanged) */
    IEcoPseudoRandomService1* m_pGenSvc;
    IEcoPseudoRandom1* m_pGenObj;
    uint16_t m_kindGen;

} CEcoLab1Outer_540B2191;

#endif /* __C_ECOLAB1OUTER_H__ */
