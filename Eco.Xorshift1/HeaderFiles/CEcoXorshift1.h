/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CEcoXorshift1_479BCC71
 * </summary>
 *
 * <description>
 *   This header describes the implementation of the CEcoXorshift1_479BCC71 component
 * </description>
 *
 * <author>
 *   Copyright (c) 2018 . All rights reserved.
 * </author>
 *
 */

#ifndef __C_ECOXORSHIFT1_H__
#define __C_ECOXORSHIFT1_H__

#include "IEcoPseudoRandomService1.h"
#include "IEcoPseudoRandom1.h"
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"

typedef struct CEcoXorshift1_479BCC71* CEcoXorshift1_479BCC71Ptr_t;

typedef struct CEcoXorshift1_479BCC71 {

    /* IEcoPseudoRandomService1 interface function table (interface 0, first field) */
    IEcoPseudoRandomService1VTbl* m_pVTblIEcoPseudoRandomService1;
    /* IEcoPseudoRandom1 interface function table (interface 1, second field) */
    IEcoPseudoRandom1VTbl* m_pVTblIEcoPseudoRandom1;


    /* Instance initialization */
    int16_t (ECOCALLMETHOD *Init)(/*in*/ CEcoXorshift1_479BCC71Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem);
    /* Instance creation */
    int16_t (ECOCALLMETHOD *Create)(/*in*/ CEcoXorshift1_479BCC71Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter);
    /* Deletion */
    void (ECOCALLMETHOD *Delete)(/*in*/ CEcoXorshift1_479BCC71Ptr_t pCMe);


    /* Reference counter */
    uint32_t m_cRef;

    /* Interface for memory operations */
    IEcoMemoryAllocator1* m_pIMem;

    /* System interface */
    IEcoSystem1* m_pISys;

    /* Instance data */
    char_t* m_Name;

    /* Register state of IEcoPseudoRandom1 */
    uint64_t m_state;
    /* Register width of IEcoPseudoRandom1: 32 or 64, 0 = not initialized */
    uint16_t m_bits;

} CEcoXorshift1_479BCC71;

#endif /* __C_ECOXORSHIFT1_H__ */
