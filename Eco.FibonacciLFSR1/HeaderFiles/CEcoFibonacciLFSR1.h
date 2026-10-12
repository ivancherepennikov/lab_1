/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CEcoFibonacciLFSR1_919D0053
 * </summary>
 *
 * <description>
 *   This header describes the implementation of the CEcoFibonacciLFSR1_919D0053 component.
 *   Interface 0 is IEcoPseudoRandomService1 (first table pointer), interface 1 is
 *   IEcoPseudoRandom1 (second table pointer). The state fields are appended at the end.
 * </description>
 *
 * <author>
 *   Copyright (c) 2018 . All rights reserved.
 * </author>
 *
 */

#ifndef __C_ECOFIBONACCILFSR1_H__
#define __C_ECOFIBONACCILFSR1_H__

#include "IEcoPseudoRandomService1.h"
#include "IEcoPseudoRandom1.h"
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"

typedef struct CEcoFibonacciLFSR1_919D0053* CEcoFibonacciLFSR1_919D0053Ptr_t;

typedef struct CEcoFibonacciLFSR1_919D0053 {

    /* IEcoPseudoRandomService1 interface function table (interface 0) */
    IEcoPseudoRandomService1VTbl* m_pVTblIEcoPseudoRandomService1;
    /* IEcoPseudoRandom1 interface function table (interface 1) */
    IEcoPseudoRandom1VTbl* m_pVTblIEcoPseudoRandom1;


    /* Instance initialization */
    int16_t (ECOCALLMETHOD *Init)(/*in*/ CEcoFibonacciLFSR1_919D0053Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem);
    /* Instance creation */
    int16_t (ECOCALLMETHOD *Create)(/*in*/ CEcoFibonacciLFSR1_919D0053Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter);
    /* Deletion */
    void (ECOCALLMETHOD *Delete)(/*in*/ CEcoFibonacciLFSR1_919D0053Ptr_t pCMe);


    /* Reference counter */
    uint32_t m_cRef;

    /* Interface for memory operations */
    IEcoMemoryAllocator1* m_pIMem;

    /* System interface */
    IEcoSystem1* m_pISys;

    /* Instance data */
    char_t* m_Name;

    /* LFSR register state (IEcoPseudoRandom1 only) */
    uint64_t m_state;
    /* Feedback tap mask, already resolved (never 0 after Init) */
    uint64_t m_param;
    /* Register width requested in Init: 32 or 64 */
    uint16_t m_bits;
    /* Non-zero after a successful Init */
    uint16_t m_inited;

} CEcoFibonacciLFSR1_919D0053;

#endif /* __C_ECOFIBONACCILFSR1_H__ */
