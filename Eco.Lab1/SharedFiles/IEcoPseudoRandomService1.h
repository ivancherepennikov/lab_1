/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IEcoPseudoRandomService1
 * </summary>
 *
 * <description>
 *   Stateless (service-oriented) interface of a pseudo random number generator.
 *   The component stores no data between calls: the client passes the current
 *   register state and receives the next one.
 *
 *   Contract shared by every algorithm component (Fibonacci LFSR, Galois LFSR,
 *   Xorshift), so the client may switch the algorithm by changing only the CID:
 *
 *   - param   LFSR: feedback tap mask, bit i is the tap of x^(i+1). The register
 *             width is the degree of the polynomial = (index of the highest set
 *             bit of param) + 1. param == 0 selects the default maximal-length
 *             polynomial of the full width (32 for Step32, 64 for Step64).
 *             Xorshift: param is ignored.
 *   - state   Must be non-zero within the register width (zero is a fixed point
 *             of every supported algorithm), otherwise ERR_ECO_INVALIDARG.
 *   - newState  The generated value. It is also the next state to pass back.
 *
 *   Return codes: ERR_ECO_SUCCESS, ERR_ECO_POINTER (me or newState is NULL),
 *   ERR_ECO_INVALIDARG (zero state).
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

#ifndef __I_ECOPSEUDORANDOMSERVICE1_H__
#define __I_ECOPSEUDORANDOMSERVICE1_H__

#include "IEcoBase1.h"

/* IEcoPseudoRandomService1 IID = {C13EFEC0-379D-4374-9B7F-F4E3815D6C31} */
#ifndef __IID_IEcoPseudoRandomService1
static const UGUID IID_IEcoPseudoRandomService1 = {0x01, 0x10, {0xC1, 0x3E, 0xFE, 0xC0, 0x37, 0x9D, 0x43, 0x74, 0x9B, 0x7F, 0xF4, 0xE3, 0x81, 0x5D, 0x6C, 0x31}};
#endif /* __IID_IEcoPseudoRandomService1 */

typedef struct IEcoPseudoRandomService1* IEcoPseudoRandomService1Ptr_t;

typedef struct IEcoPseudoRandomService1VTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoPseudoRandomService1Ptr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoPseudoRandomService1Ptr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoPseudoRandomService1Ptr_t me);

    /* IEcoPseudoRandomService1 */
    int16_t (ECOCALLMETHOD *Step32)(/* in */ IEcoPseudoRandomService1Ptr_t me, /* in */ uint32_t state, /* in */ uint32_t param, /* out */ uint32_t* newState);
    int16_t (ECOCALLMETHOD *Step64)(/* in */ IEcoPseudoRandomService1Ptr_t me, /* in */ uint64_t state, /* in */ uint64_t param, /* out */ uint64_t* newState);

} IEcoPseudoRandomService1VTbl, *IEcoPseudoRandomService1VTblPtr_t;

interface IEcoPseudoRandomService1 {
    struct IEcoPseudoRandomService1VTbl *pVTbl;
} IEcoPseudoRandomService1;


#endif /* __I_ECOPSEUDORANDOMSERVICE1_H__ */
