/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IEcoCalcA
 * </summary>
 *
 * <description>
 *   This header describes the interface IEcoCalcA
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

#ifndef __I_ECOCALCA_H__
#define __I_ECOCALCA_H__

#include "IEcoBase1.h"

/* IEcoCalcA IID = {6880A69A-8EE6-405A-9A76-2927BD332F4C} */
#ifndef __IID_IEcoCalcA
static const UGUID IID_IEcoCalcA = {0x01, 0x10, {0x68, 0x80, 0xA6, 0x9A, 0x8E, 0xE6, 0x40, 0x5A, 0x9A, 0x76, 0x29, 0x27, 0xBD, 0x33, 0x2F, 0x4C}};
#endif /* __IID_IEcoCalcA */

typedef struct IEcoCalcA* IEcoCalcAPtr_t;

typedef struct IEcoCalcAVTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoCalcAPtr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoCalcAPtr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoCalcAPtr_t me);

    /* IEcoCalcA */
    int16_t (ECOCALLMETHOD *MyFunction)(/* in */ IEcoCalcAPtr_t me, /* in */ char_t* Name, /* out */ char_t** CopyName);

} IEcoCalcAVTbl, *IEcoCalcAVTblPtr_t;

interface IEcoCalcA {
    struct IEcoCalcAVTbl *pVTbl;
} IEcoCalcA;


#endif /* __I_ECOCALCA_H__ */

