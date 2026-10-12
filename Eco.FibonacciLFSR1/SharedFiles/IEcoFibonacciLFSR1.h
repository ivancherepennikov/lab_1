/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IEcoFibonacciLFSR1
 * </summary>
 *
 * <description>
 *   This header describes the interface IEcoFibonacciLFSR1
 * </description>
 *
 * <reference>
 *
 * </reference>
 *
 * <author>
 *   Copyright (c) 2026 . All rights reserved.
 * </author>
 *
 */

#ifndef __I_ECOFIBONACCILFSR1_H__
#define __I_ECOFIBONACCILFSR1_H__

#include "IEcoBase1.h"

/* IEcoFibonacciLFSR1 IID = {64435E55-C2EB-4EDF-892C-7D6C73C0A955} */
#ifndef __IID_IEcoFibonacciLFSR1
static const UGUID IID_IEcoFibonacciLFSR1 = {0x01, 0x10, {0x64, 0x43, 0x5E, 0x55, 0xC2, 0xEB, 0x4E, 0xDF, 0x89, 0x2C, 0x7D, 0x6C, 0x73, 0xC0, 0xA9, 0x55}};
#endif /* __IID_IEcoFibonacciLFSR1 */

typedef struct IEcoFibonacciLFSR1* IEcoFibonacciLFSR1Ptr_t;

typedef struct IEcoFibonacciLFSR1VTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoFibonacciLFSR1Ptr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoFibonacciLFSR1Ptr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoFibonacciLFSR1Ptr_t me);

    /* IEcoFibonacciLFSR1 */
    int16_t (ECOCALLMETHOD *MyFunction)(/* in */ IEcoFibonacciLFSR1Ptr_t me, /* in */ char_t* Name, /* out */ char_t** CopyName);

} IEcoFibonacciLFSR1VTbl, *IEcoFibonacciLFSR1VTblPtr_t;

interface IEcoFibonacciLFSR1 {
    struct IEcoFibonacciLFSR1VTbl *pVTbl;
} IEcoFibonacciLFSR1;


#endif /* __I_ECOFIBONACCILFSR1_H__ */

