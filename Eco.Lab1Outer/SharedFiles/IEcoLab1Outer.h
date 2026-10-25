/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IEcoLab1Outer
 * </summary>
 *
 * <description>
 *   IEcoLab1Outer: interface of the outer component.
 *   SelectCalculators(kindX, kindY): choose the inner components that provide IEcoCalculatorX and IEcoCalculatorY,
 *     kinds ECO_CALC_X_* / ECO_CALC_Y_* (defaults: A and D). Interface pointers of an aggregated inner component
 *     obtained before the call become invalid after it.
 *   SelectGenerator(kind): the lab 1 generator contained in the component, kind = ECO_LAB1_GEN_*;
 *     the pseudo random interfaces of the component forward to it (default: Xorshift).
 *   Return codes: ERR_ECO_SUCCESS, ERR_ECO_POINTER, ERR_ECO_INVALIDARG (unknown kind), or the error of the bus.
 * </description>
 *
 * <author>
 *   Copyright (c) 2026 . All rights reserved.
 * </author>
 *
 */

#ifndef __I_ECOLAB1OUTER_H__
#define __I_ECOLAB1OUTER_H__

#include "IEcoBase1.h"

/* IEcoLab1Outer IID = {C06104EE-BE11-479E-AE92-2EE734B67798} */
#ifndef __IID_IEcoLab1Outer
static const UGUID IID_IEcoLab1Outer = {0x01, 0x10, {0xC0, 0x61, 0x04, 0xEE, 0xBE, 0x11, 0x47, 0x9E, 0xAE, 0x92, 0x2E, 0xE7, 0x34, 0xB6, 0x77, 0x98}};
#endif /* __IID_IEcoLab1Outer */

/* Kinds for SelectCalculators: the inner component providing the interface and the way it is used */
#ifndef ECO_CALC_KINDS_DEFINED
#define ECO_CALC_KINDS_DEFINED
#define ECO_CALC_X_A 0 /* IEcoCalculatorX from component A, containment */
#define ECO_CALC_X_B 1 /* IEcoCalculatorX from component B, aggregation */
#define ECO_CALC_X_C 2 /* IEcoCalculatorX from component C, containment */
#define ECO_CALC_Y_D 0 /* IEcoCalculatorY from component D, containment */
#define ECO_CALC_Y_E 1 /* IEcoCalculatorY from component E, aggregation */
#define ECO_CALC_Y_C 2 /* IEcoCalculatorY from component C, containment */
#endif /* ECO_CALC_KINDS_DEFINED */
/* Kinds for SelectGenerator: the unchanged lab 1 component that is contained */
#ifndef ECO_LAB1_GEN_DEFINED
#define ECO_LAB1_GEN_DEFINED
#define ECO_LAB1_GEN_FIBONACCI 0
#define ECO_LAB1_GEN_GALOIS 1
#define ECO_LAB1_GEN_XORSHIFT 2
#endif /* ECO_LAB1_GEN_DEFINED */

typedef struct IEcoLab1Outer* IEcoLab1OuterPtr_t;

typedef struct IEcoLab1OuterVTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoLab1OuterPtr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoLab1OuterPtr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoLab1OuterPtr_t me);

    /* IEcoLab1Outer */
    int16_t (ECOCALLMETHOD *SelectGenerator)(/* in */ IEcoLab1OuterPtr_t me, /* in */ uint16_t kind);
    int16_t (ECOCALLMETHOD *SelectCalculators)(/* in */ IEcoLab1OuterPtr_t me, /* in */ uint16_t kindX, /* in */ uint16_t kindY);

} IEcoLab1OuterVTbl, *IEcoLab1OuterVTblPtr_t;

interface IEcoLab1Outer {
    struct IEcoLab1OuterVTbl *pVTbl;
} IEcoLab1Outer;


#endif /* __I_ECOLAB1OUTER_H__ */
