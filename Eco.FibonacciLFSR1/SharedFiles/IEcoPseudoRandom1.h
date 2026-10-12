/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IEcoPseudoRandom1
 * </summary>
 *
 * <description>
 *   Stateful (object-oriented) interface of a pseudo random number generator.
 *   The object encapsulates the register state and the feedback polynomial.
 *   Both interfaces of the component produce identical sequences for identical
 *   seed, param and width.
 *
 *   - Init    seed: initial register state, must be non-zero within the width.
 *             param: same meaning as in IEcoPseudoRandomService1 (0 = default
 *             maximal-length polynomial of the given width).
 *             bits: register width, 32 or 64, otherwise ERR_ECO_INVALIDARG.
 *             Init may be called again to restart the object.
 *   - Next    Advances the register and returns the new state (zero-extended
 *             for 32 bit width). ERR_ECO_FAIL if Init has not been called.
 *   - get_Name  Human readable algorithm name, owned by the component.
 *
 *   Return codes: ERR_ECO_SUCCESS, ERR_ECO_POINTER, ERR_ECO_INVALIDARG, ERR_ECO_FAIL.
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

#ifndef __I_ECOPSEUDORANDOM1_H__
#define __I_ECOPSEUDORANDOM1_H__

#include "IEcoBase1.h"

/* IEcoPseudoRandom1 IID = {36BD4863-DE93-40D6-B1FB-319B64A37722} */
#ifndef __IID_IEcoPseudoRandom1
static const UGUID IID_IEcoPseudoRandom1 = {0x01, 0x10, {0x36, 0xBD, 0x48, 0x63, 0xDE, 0x93, 0x40, 0xD6, 0xB1, 0xFB, 0x31, 0x9B, 0x64, 0xA3, 0x77, 0x22}};
#endif /* __IID_IEcoPseudoRandom1 */

typedef struct IEcoPseudoRandom1* IEcoPseudoRandom1Ptr_t;

typedef struct IEcoPseudoRandom1VTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoPseudoRandom1Ptr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoPseudoRandom1Ptr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoPseudoRandom1Ptr_t me);

    /* IEcoPseudoRandom1 */
    int16_t (ECOCALLMETHOD *Init)(/* in */ IEcoPseudoRandom1Ptr_t me, /* in */ uint64_t seed, /* in */ uint64_t param, /* in */ uint16_t bits);
    int16_t (ECOCALLMETHOD *Next)(/* in */ IEcoPseudoRandom1Ptr_t me, /* out */ uint64_t* value);
    char_t* (ECOCALLMETHOD *get_Name)(/* in */ IEcoPseudoRandom1Ptr_t me);

} IEcoPseudoRandom1VTbl, *IEcoPseudoRandom1VTblPtr_t;

interface IEcoPseudoRandom1 {
    struct IEcoPseudoRandom1VTbl *pVTbl;
} IEcoPseudoRandom1;


#endif /* __I_ECOPSEUDORANDOM1_H__ */
