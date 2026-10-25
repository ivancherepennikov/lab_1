/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IEcoCalcE
 * </summary>
 *
 * <description>
 *   This header describes the interface IEcoCalcE
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

#ifndef __I_ECOCALCE_H__
#define __I_ECOCALCE_H__

#include "IEcoBase1.h"

/* IEcoCalcE IID = {53E803FD-44CA-4432-8D56-9C7E9E86129E} */
#ifndef __IID_IEcoCalcE
static const UGUID IID_IEcoCalcE = {0x01, 0x10, {0x53, 0xE8, 0x03, 0xFD, 0x44, 0xCA, 0x44, 0x32, 0x8D, 0x56, 0x9C, 0x7E, 0x9E, 0x86, 0x12, 0x9E}};
#endif /* __IID_IEcoCalcE */

typedef struct IEcoCalcE* IEcoCalcEPtr_t;

typedef struct IEcoCalcEVTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoCalcEPtr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoCalcEPtr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoCalcEPtr_t me);

    /* IEcoCalcE */
    int16_t (ECOCALLMETHOD *MyFunction)(/* in */ IEcoCalcEPtr_t me, /* in */ char_t* Name, /* out */ char_t** CopyName);

} IEcoCalcEVTbl, *IEcoCalcEVTblPtr_t;

interface IEcoCalcE {
    struct IEcoCalcEVTbl *pVTbl;
} IEcoCalcE;


#endif /* __I_ECOCALCE_H__ */

