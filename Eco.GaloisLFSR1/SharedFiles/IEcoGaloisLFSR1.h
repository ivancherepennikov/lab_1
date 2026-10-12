/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IEcoGaloisLFSR1
 * </summary>
 *
 * <description>
 *   This header describes the interface IEcoGaloisLFSR1
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

#ifndef __I_ECOGALOISLFSR1_H__
#define __I_ECOGALOISLFSR1_H__

#include "IEcoBase1.h"

/* IEcoGaloisLFSR1 IID = {F0B776FA-6EAE-4DEE-8DE0-0E26773305DB} */
#ifndef __IID_IEcoGaloisLFSR1
static const UGUID IID_IEcoGaloisLFSR1 = {0x01, 0x10, {0xF0, 0xB7, 0x76, 0xFA, 0x6E, 0xAE, 0x4D, 0xEE, 0x8D, 0xE0, 0x0E, 0x26, 0x77, 0x33, 0x05, 0xDB}};
#endif /* __IID_IEcoGaloisLFSR1 */

typedef struct IEcoGaloisLFSR1* IEcoGaloisLFSR1Ptr_t;

typedef struct IEcoGaloisLFSR1VTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoGaloisLFSR1Ptr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoGaloisLFSR1Ptr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoGaloisLFSR1Ptr_t me);

    /* IEcoGaloisLFSR1 */
    int16_t (ECOCALLMETHOD *MyFunction)(/* in */ IEcoGaloisLFSR1Ptr_t me, /* in */ char_t* Name, /* out */ char_t** CopyName);

} IEcoGaloisLFSR1VTbl, *IEcoGaloisLFSR1VTblPtr_t;

interface IEcoGaloisLFSR1 {
    struct IEcoGaloisLFSR1VTbl *pVTbl;
} IEcoGaloisLFSR1;


#endif /* __I_ECOGALOISLFSR1_H__ */

