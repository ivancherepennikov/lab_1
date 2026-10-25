/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IEcoCalculatorX
 * </summary>
 *
 * <description>
 *   Calculator interface X: addition (32 bit result) and subtraction (16 bit result).
 *   Contract: all methods work with 16 bit operands. A method called with me == NULL returns 0.
 *   Cases the interface cannot report through its return type (overflow of a 16 bit result,
 *   division by zero) are implementation-defined: each component documents its own policy.
 * </description>
 *
 * <author>
 *   Copyright (c) 2026 . All rights reserved.
 * </author>
 *
 */

#ifndef __I_ECOCALCULATORX_H__
#define __I_ECOCALCULATORX_H__

#include "IEcoBase1.h"

/* IEcoCalculatorX IID = {93221116-2248-4742-AE06-82819447843D} */
#ifndef __IID_IEcoCalculatorX
static const UGUID IID_IEcoCalculatorX = {0x01, 0x10, {0x93, 0x22, 0x11, 0x16, 0x22, 0x48, 0x47, 0x42, 0xAE, 0x06, 0x82, 0x81, 0x94, 0x47, 0x84, 0x3D}};
#endif /* __IID_IEcoCalculatorX */

typedef struct IEcoCalculatorX* IEcoCalculatorXPtr_t;

typedef struct IEcoCalculatorXVTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoCalculatorXPtr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoCalculatorXPtr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoCalculatorXPtr_t me);

    /* IEcoCalculatorX */
    int32_t (ECOCALLMETHOD *Addition)(/* in */ IEcoCalculatorXPtr_t me, /* in */ int16_t a, /* in */ int16_t b);
    int16_t (ECOCALLMETHOD *Subtraction)(/* in */ IEcoCalculatorXPtr_t me, /* in */ int16_t a, /* in */ int16_t b);

} IEcoCalculatorXVTbl, *IEcoCalculatorXVTblPtr_t;

interface IEcoCalculatorX {
    struct IEcoCalculatorXVTbl *pVTbl;
} IEcoCalculatorX;


#endif /* __I_ECOCALCULATORX_H__ */
