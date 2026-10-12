/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IEcoXorshift1
 * </summary>
 *
 * <description>
 *   This header describes the interface IEcoXorshift1
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

#ifndef __I_ECOXORSHIFT1_H__
#define __I_ECOXORSHIFT1_H__

#include "IEcoBase1.h"

/* IEcoXorshift1 IID = {5DD125B0-65E3-4E42-B889-1F79B6C588F4} */
#ifndef __IID_IEcoXorshift1
static const UGUID IID_IEcoXorshift1 = {0x01, 0x10, {0x5D, 0xD1, 0x25, 0xB0, 0x65, 0xE3, 0x4E, 0x42, 0xB8, 0x89, 0x1F, 0x79, 0xB6, 0xC5, 0x88, 0xF4}};
#endif /* __IID_IEcoXorshift1 */

typedef struct IEcoXorshift1* IEcoXorshift1Ptr_t;

typedef struct IEcoXorshift1VTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoXorshift1Ptr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoXorshift1Ptr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoXorshift1Ptr_t me);

    /* IEcoXorshift1 */
    int16_t (ECOCALLMETHOD *MyFunction)(/* in */ IEcoXorshift1Ptr_t me, /* in */ char_t* Name, /* out */ char_t** CopyName);

} IEcoXorshift1VTbl, *IEcoXorshift1VTblPtr_t;

interface IEcoXorshift1 {
    struct IEcoXorshift1VTbl *pVTbl;
} IEcoXorshift1;


#endif /* __I_ECOXORSHIFT1_H__ */

