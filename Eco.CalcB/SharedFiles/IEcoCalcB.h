/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IEcoCalcB
 * </summary>
 *
 * <description>
 *   This header describes the interface IEcoCalcB
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

#ifndef __I_ECOCALCB_H__
#define __I_ECOCALCB_H__

#include "IEcoBase1.h"

/* IEcoCalcB IID = {0B2CBFAF-15F4-4C3A-8EFB-1AE806F582EC} */
#ifndef __IID_IEcoCalcB
static const UGUID IID_IEcoCalcB = {0x01, 0x10, {0x0B, 0x2C, 0xBF, 0xAF, 0x15, 0xF4, 0x4C, 0x3A, 0x8E, 0xFB, 0x1A, 0xE8, 0x06, 0xF5, 0x82, 0xEC}};
#endif /* __IID_IEcoCalcB */

typedef struct IEcoCalcB* IEcoCalcBPtr_t;

typedef struct IEcoCalcBVTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoCalcBPtr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoCalcBPtr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoCalcBPtr_t me);

    /* IEcoCalcB */
    int16_t (ECOCALLMETHOD *MyFunction)(/* in */ IEcoCalcBPtr_t me, /* in */ char_t* Name, /* out */ char_t** CopyName);

} IEcoCalcBVTbl, *IEcoCalcBVTblPtr_t;

interface IEcoCalcB {
    struct IEcoCalcBVTbl *pVTbl;
} IEcoCalcB;


#endif /* __I_ECOCALCB_H__ */

