/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IEcoCalcD
 * </summary>
 *
 * <description>
 *   This header describes the interface IEcoCalcD
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

#ifndef __I_ECOCALCD_H__
#define __I_ECOCALCD_H__

#include "IEcoBase1.h"

/* IEcoCalcD IID = {E252AE4A-D585-4083-838C-9975007EDED3} */
#ifndef __IID_IEcoCalcD
static const UGUID IID_IEcoCalcD = {0x01, 0x10, {0xE2, 0x52, 0xAE, 0x4A, 0xD5, 0x85, 0x40, 0x83, 0x83, 0x8C, 0x99, 0x75, 0x00, 0x7E, 0xDE, 0xD3}};
#endif /* __IID_IEcoCalcD */

typedef struct IEcoCalcD* IEcoCalcDPtr_t;

typedef struct IEcoCalcDVTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoCalcDPtr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoCalcDPtr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoCalcDPtr_t me);

    /* IEcoCalcD */
    int16_t (ECOCALLMETHOD *MyFunction)(/* in */ IEcoCalcDPtr_t me, /* in */ char_t* Name, /* out */ char_t** CopyName);

} IEcoCalcDVTbl, *IEcoCalcDVTblPtr_t;

interface IEcoCalcD {
    struct IEcoCalcDVTbl *pVTbl;
} IEcoCalcD;


#endif /* __I_ECOCALCD_H__ */

