/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IEcoCalcC
 * </summary>
 *
 * <description>
 *   This header describes the interface IEcoCalcC
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

#ifndef __I_ECOCALCC_H__
#define __I_ECOCALCC_H__

#include "IEcoBase1.h"

/* IEcoCalcC IID = {A98F2E38-38C6-470A-BB77-BA33EF8073EC} */
#ifndef __IID_IEcoCalcC
static const UGUID IID_IEcoCalcC = {0x01, 0x10, {0xA9, 0x8F, 0x2E, 0x38, 0x38, 0xC6, 0x47, 0x0A, 0xBB, 0x77, 0xBA, 0x33, 0xEF, 0x80, 0x73, 0xEC}};
#endif /* __IID_IEcoCalcC */

typedef struct IEcoCalcC* IEcoCalcCPtr_t;

typedef struct IEcoCalcCVTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoCalcCPtr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoCalcCPtr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoCalcCPtr_t me);

    /* IEcoCalcC */
    int16_t (ECOCALLMETHOD *MyFunction)(/* in */ IEcoCalcCPtr_t me, /* in */ char_t* Name, /* out */ char_t** CopyName);

} IEcoCalcCVTbl, *IEcoCalcCVTblPtr_t;

interface IEcoCalcC {
    struct IEcoCalcCVTbl *pVTbl;
} IEcoCalcC;


#endif /* __I_ECOCALCC_H__ */

