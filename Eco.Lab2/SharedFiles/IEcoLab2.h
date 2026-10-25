/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IEcoLab2
 * </summary>
 *
 * <description>
 *   IEcoLab2: interface of the outer component.
 *   SelectCalculators(kindX, kindY): choose the inner components that provide IEcoCalculatorX and IEcoCalculatorY,
 *     kinds ECO_CALC_X_* / ECO_CALC_Y_* (defaults: A and D). Interface pointers of an aggregated inner component
 *     obtained before the call become invalid after it.
 *   Return codes: ERR_ECO_SUCCESS, ERR_ECO_POINTER, ERR_ECO_INVALIDARG (unknown kind), or the error of the bus.
 * </description>
 *
 * <author>
 *   Copyright (c) 2026 . All rights reserved.
 * </author>
 *
 */

#ifndef __I_ECOLAB2_H__
#define __I_ECOLAB2_H__

#include "IEcoBase1.h"

/* IEcoLab2 IID = {3CB9FF49-5C70-4891-893A-B8D61E930D8D} */
#ifndef __IID_IEcoLab2
static const UGUID IID_IEcoLab2 = {0x01, 0x10, {0x3C, 0xB9, 0xFF, 0x49, 0x5C, 0x70, 0x48, 0x91, 0x89, 0x3A, 0xB8, 0xD6, 0x1E, 0x93, 0x0D, 0x8D}};
#endif /* __IID_IEcoLab2 */

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

typedef struct IEcoLab2* IEcoLab2Ptr_t;

typedef struct IEcoLab2VTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoLab2Ptr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoLab2Ptr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoLab2Ptr_t me);

    /* IEcoLab2 */
    int16_t (ECOCALLMETHOD *SelectCalculators)(/* in */ IEcoLab2Ptr_t me, /* in */ uint16_t kindX, /* in */ uint16_t kindY);

} IEcoLab2VTbl, *IEcoLab2VTblPtr_t;

interface IEcoLab2 {
    struct IEcoLab2VTbl *pVTbl;
} IEcoLab2;


#endif /* __I_ECOLAB2_H__ */
