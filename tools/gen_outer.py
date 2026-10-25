"""Generates the outer components of assignment 2: Eco.Lab1Outer and Eco.Lab2.

usage: gen_outer.py <repo root>

Eco.Lab1Outer ("the Eco.Lab1 component with aggregation support" of the assignment, built around the unchanged
lab 1 generators): own interface IEcoLab1Outer, the calculator interfaces X and Y (provided by the inner
components A..E through containment or aggregation) and the pseudo random interfaces of lab 1 (provided by a
contained lab 1 component). It is aggregatable itself.
Eco.Lab2: own interface IEcoLab2, the calculator interfaces X and Y (inner components A..E) and the aggregated
Eco.Lab1Outer; every other interface is requested from the aggregated Eco.Lab1Outer.
"""
import os
import re
import sys

ROOT = os.path.abspath(sys.argv[1]) if len(sys.argv) > 1 else os.getcwd()


def write(path, text):
    with open(path, "w", encoding="utf-8-sig", newline="") as f:
        f.write(text.replace("\r\n", "\n").replace("\n", "\r\n"))


def read(path):
    return open(path, encoding="utf-8-sig").read().replace("\r\n", "\n")


def fheader(summary, desc):
    return "/*\n * <character encoding>\n *   Cyrillic (UTF-8 with signature) - Codepage 65001\n * </character encoding>\n *\n * <summary>\n *   %s\n * </summary>\n *\n * <description>\n%s\n * </description>\n *\n * <author>\n *   Copyright (c) 2026 . All rights reserved.\n * </author>\n *\n */\n" % (summary, "\n".join(" *   " + l for l in desc))


def mheader(summary, desc):
    return "/*\n *\n * <summary>\n *   %s\n * </summary>\n *\n * <description>\n%s\n * </description>\n *\n */\n" % (summary, "\n".join(" *   " + l for l in desc))


OUTERS = {
    "Lab1Outer": dict(
        own="IEcoLab1Outer",
        ifaces=[("IEcoLab1Outer", "Own"), ("IEcoCalculatorX", "X"), ("IEcoCalculatorY", "Y"),
                ("IEcoPseudoRandomService1", "Svc"), ("IEcoPseudoRandom1", "Obj")],
        gen=True, lab1=False,
        deps=["CalcA", "CalcB", "CalcC", "CalcD", "CalcE", "FibonacciLFSR1", "GaloisLFSR1", "Xorshift1"],
        extra_ifaces=["IEcoPseudoRandomService1", "IEcoPseudoRandom1"]),
    "Lab2": dict(
        own="IEcoLab2",
        ifaces=[("IEcoLab2", "Own"), ("IEcoCalculatorX", "X"), ("IEcoCalculatorY", "Y")],
        gen=False, lab1=True,
        deps=["CalcA", "CalcB", "CalcC", "CalcD", "CalcE", "Lab1Outer"],
        extra_ifaces=[]),
}

KIND_MACROS = """/* Kinds for SelectCalculators: the inner component providing the interface and the way it is used */
#ifndef ECO_CALC_KINDS_DEFINED
#define ECO_CALC_KINDS_DEFINED
#define ECO_CALC_X_A 0 /* IEcoCalculatorX from component A, containment */
#define ECO_CALC_X_B 1 /* IEcoCalculatorX from component B, aggregation */
#define ECO_CALC_X_C 2 /* IEcoCalculatorX from component C, containment */
#define ECO_CALC_Y_D 0 /* IEcoCalculatorY from component D, containment */
#define ECO_CALC_Y_E 1 /* IEcoCalculatorY from component E, aggregation */
#define ECO_CALC_Y_C 2 /* IEcoCalculatorY from component C, containment */
#endif /* ECO_CALC_KINDS_DEFINED */
"""


def gen_interface(name, spec, stub):
    own = spec["own"]
    iid_line = re.search(r"/\* %s IID = \{([0-9A-F-]+)\} \*/" % own, stub).group(1)
    iid_bytes = re.search(r"static const UGUID IID_%s = \{0x01, 0x10, \{([^}]*)\}\};" % own, stub).group(1)
    gen_m = ""
    gen_decl = ""
    gen_doc = []
    if spec["gen"]:
        gen_decl = "    int16_t (ECOCALLMETHOD *SelectGenerator)(/* in */ @OWN@Ptr_t me, /* in */ uint16_t kind);\n"
        gen_doc = ["SelectGenerator(kind): the lab 1 generator contained in the component, kind = ECO_LAB1_GEN_*;",
                   "  the pseudo random interfaces of the component forward to it (default: Xorshift)."]
        gen_m = """/* Kinds for SelectGenerator: the unchanged lab 1 component that is contained */
#ifndef ECO_LAB1_GEN_DEFINED
#define ECO_LAB1_GEN_DEFINED
#define ECO_LAB1_GEN_FIBONACCI 0
#define ECO_LAB1_GEN_GALOIS 1
#define ECO_LAB1_GEN_XORSHIFT 2
#endif /* ECO_LAB1_GEN_DEFINED */
"""
    doc = ["%s: interface of the outer component." % own,
           "SelectCalculators(kindX, kindY): choose the inner components that provide IEcoCalculatorX and IEcoCalculatorY,",
           "  kinds ECO_CALC_X_* / ECO_CALC_Y_* (defaults: A and D). Interface pointers of an aggregated inner component",
           "  obtained before the call become invalid after it."] + gen_doc + [
           "Return codes: ERR_ECO_SUCCESS, ERR_ECO_POINTER, ERR_ECO_INVALIDARG (unknown kind), or the error of the bus."]
    tpl = """
#ifndef __I_@UPPER@_H__
#define __I_@UPPER@_H__

#include "IEcoBase1.h"

/* @OWN@ IID = {@IIDLINE@} */
#ifndef __IID_@OWN@
static const UGUID IID_@OWN@ = {0x01, 0x10, {@IIDBYTES@}};
#endif /* __IID_@OWN@ */

@KINDS@@GENM@
typedef struct @OWN@* @OWN@Ptr_t;

typedef struct @OWN@VTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ @OWN@Ptr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ @OWN@Ptr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ @OWN@Ptr_t me);

    /* @OWN@ */
@GENDECL@    int16_t (ECOCALLMETHOD *SelectCalculators)(/* in */ @OWN@Ptr_t me, /* in */ uint16_t kindX, /* in */ uint16_t kindY);

} @OWN@VTbl, *@OWN@VTblPtr_t;

interface @OWN@ {
    struct @OWN@VTbl *pVTbl;
} @OWN@;


#endif /* __I_@UPPER@_H__ */
"""
    tpl = (tpl.replace("@KINDS@", KIND_MACROS).replace("@GENM@", gen_m).replace("@GENDECL@", gen_decl)
              .replace("@IIDLINE@", iid_line).replace("@IIDBYTES@", iid_bytes)
              .replace("@UPPER@", own.upper().replace("IECO", "ECO")).replace("@OWN@", own))
    return fheader(own, doc) + tpl


def cid_of(p):
    s = p
    t = read(os.path.join(ROOT, "Eco." + p, "SharedFiles", "IdEco%s.h" % s))
    return re.search(r"GetIEcoComponentFactoryPtr_([0-9A-F]{32});", t).group(1)


def build(name, spec):
    pdir = os.path.join(ROOT, "Eco." + name)
    cid = cid_of(name)
    sfx = cid[-8:]
    cn = "CEco%s_%s" % (name, sfx)
    own = spec["own"]
    ifaces = spec["ifaces"]
    nd_idx = len(ifaces)
    has_gen, has_lab1 = spec["gen"], spec["lab1"]
    own_vt = "m_pVTbl" + own  # first table field (the delegating unknown handed to inner components)

    def fix(idx):
        return "(%s*)me" % cn if idx == 0 else "(%s*)((char_t*)me - %d * sizeof(voidptr_t))" % (cn, idx)

    # ---------------------------------------------------------------- header
    incs = "".join('#include "%s.h"\n' % i for i, _ in ifaces)
    fields = ""
    for idx, (it, tag) in enumerate(ifaces):
        fields += "    /* %s interface function table (interface %d) */\n    %sVTbl* m_pVTbl%s;\n\n" % (it, idx, it, it)
    fields += "    /* Non-delegating IEcoUnknown function table (interface %d) */\n    IEcoUnknownVTbl* m_pVTblINondelegatingUnk;\n\n" % nd_idx
    data = """
    /* Controlling IEcoUnknown: the outer component when aggregated, the non-delegating IEcoUnknown otherwise */
    IEcoUnknown* m_pIUnkOuter;

    /* Interface bus used to create the inner components */
    IEcoInterfaceBus1* m_pIBus;

    /* Selected variants */
    uint16_t m_kindX;
    uint16_t m_kindY;

    /* Contained calculator components (containment: the outer component calls them) */
    IEcoCalculatorX* m_pIXc;
    IEcoCalculatorY* m_pIYc;

    /* Aggregated calculator components: their NON-delegating IEcoUnknown (aggregation: their interfaces go to the client directly) */
    IEcoUnknown* m_pXInner;
    IEcoUnknown* m_pYInner;
"""
    if has_gen:
        data += """
    /* Contained lab 1 generator (component of lab 1, used unchanged) */
    IEcoPseudoRandomService1* m_pGenSvc;
    IEcoPseudoRandom1* m_pGenObj;
    uint16_t m_kindGen;
"""
    if has_lab1:
        data += """
    /* Aggregated Eco.Lab1Outer: its NON-delegating IEcoUnknown */
    IEcoUnknown* m_pLab1Inner;
"""
    header = fheader(cn, ["This header describes the implementation of the %s component" % cn]) + """
#ifndef __C_ECO%s_H__
#define __C_ECO%s_H__

%s#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "IdEcoMemoryManager1.h"

typedef struct %s* %sPtr_t;

typedef struct %s {

%s    /* Instance initialization */
    int16_t (ECOCALLMETHOD *Init)(/*in*/ %sPtr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem);
    /* Instance creation */
    int16_t (ECOCALLMETHOD *Create)(/*in*/ %sPtr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter);
    /* Deletion */
    void (ECOCALLMETHOD *Delete)(/*in*/ %sPtr_t pCMe);


    /* Reference counter */
    uint32_t m_cRef;

    /* Interface for memory operations */
    IEcoMemoryAllocator1* m_pIMem;

    /* System interface */
    IEcoSystem1* m_pISys;

    /* Instance data */
    char_t* m_Name;
%s
} %s;

#endif /* __C_ECO%s_H__ */
""" % (name.upper(), name.upper(), incs, cn, cn, cn, fields, cn, cn, cn, data, cn, name.upper())

    # ---------------------------------------------------------------- source
    s = fheader(cn, ["This source code describes the implementation of the interfaces for %s" % cn,
                     "Inner components are used through containment (they are called by this component) and",
                     "through aggregation (their interface pointers are handed to the client directly)."]) + "\n"
    s += '#include "IEcoSystem1.h"\n#include "IEcoInterfaceBus1.h"\n#include "IEcoInterfaceBus1MemExt.h"\n#include "CEco%s.h"\n' % name
    for d in spec["deps"]:
        s += '#include "IdEco%s.h"\n' % d
    s += "\n"

    # --- helpers
    rel = """static void %s_ReleaseCalculators(/* in */ %s* pCMe) {
    if (pCMe->m_pIXc != 0) {
        pCMe->m_pIXc->pVTbl->Release(pCMe->m_pIXc);
        pCMe->m_pIXc = 0;
    }
    if (pCMe->m_pIYc != 0) {
        pCMe->m_pIYc->pVTbl->Release(pCMe->m_pIYc);
        pCMe->m_pIYc = 0;
    }
    /* releasing the non-delegating IEcoUnknown destroys an aggregated inner component */
    if (pCMe->m_pXInner != 0) {
        pCMe->m_pXInner->pVTbl->Release(pCMe->m_pXInner);
        pCMe->m_pXInner = 0;
    }
    if (pCMe->m_pYInner != 0) {
        pCMe->m_pYInner->pVTbl->Release(pCMe->m_pYInner);
        pCMe->m_pYInner = 0;
    }
}

""" % (cn, cn)
    s += mheader("ReleaseCalculators Function", ["Releases the inner calculator components: contained ones with Release, aggregated ones with the non-delegating IEcoUnknown."]) + rel
    sel = """static int16_t %(cn)s_SelectCalcInternal(/* in */ %(cn)s* pCMe, /* in */ uint16_t kindX, /* in */ uint16_t kindY) {
    IEcoUnknown* pOuterUnk = (IEcoUnknown*)&pCMe->%(own_vt)s;
    int16_t result = ERR_ECO_SUCCESS;

    if (pCMe->m_pIBus == 0) {
        return ERR_ECO_FAIL;
    }
    if (kindX > 2 || kindY > 2) {
        return ERR_ECO_INVALIDARG;
    }
    %(cn)s_ReleaseCalculators(pCMe);

    /* IEcoCalculatorX: A or C by containment, B by aggregation */
    if (kindX == 1) {
        result = pCMe->m_pIBus->pVTbl->QueryComponent(pCMe->m_pIBus, &CID_EcoCalcB, pOuterUnk, &IID_IEcoUnknown, (void**)&pCMe->m_pXInner);
    }
    else {
        result = pCMe->m_pIBus->pVTbl->QueryComponent(pCMe->m_pIBus, (kindX == 0) ? &CID_EcoCalcA : &CID_EcoCalcC, 0, &IID_IEcoCalculatorX, (void**)&pCMe->m_pIXc);
    }
    if (result != ERR_ECO_SUCCESS) {
        %(cn)s_ReleaseCalculators(pCMe);
        return result;
    }

    /* IEcoCalculatorY: D or C by containment, E by aggregation */
    if (kindY == 1) {
        result = pCMe->m_pIBus->pVTbl->QueryComponent(pCMe->m_pIBus, &CID_EcoCalcE, pOuterUnk, &IID_IEcoUnknown, (void**)&pCMe->m_pYInner);
    }
    else {
        result = pCMe->m_pIBus->pVTbl->QueryComponent(pCMe->m_pIBus, (kindY == 0) ? &CID_EcoCalcD : &CID_EcoCalcC, 0, &IID_IEcoCalculatorY, (void**)&pCMe->m_pIYc);
    }
    if (result != ERR_ECO_SUCCESS) {
        %(cn)s_ReleaseCalculators(pCMe);
        return result;
    }
    pCMe->m_kindX = kindX;
    pCMe->m_kindY = kindY;
    return ERR_ECO_SUCCESS;
}

""" % {"cn": cn, "own_vt": own_vt}
    s += mheader("SelectCalcInternal Function", ["Creates the inner calculator components for the selected kinds.",
                                                 "The controlling unknown given to an aggregated inner component is the delegating IEcoUnknown of this component."]) + sel
    if has_gen:
        s += mheader("ReleaseGenerator / SelectGenInternal Functions", ["Contained lab 1 generator: the unchanged component of lab 1 is created by its CID and both its interfaces are kept."])
        s += """static void %(cn)s_ReleaseGenerator(/* in */ %(cn)s* pCMe) {
    if (pCMe->m_pGenSvc != 0) {
        pCMe->m_pGenSvc->pVTbl->Release(pCMe->m_pGenSvc);
        pCMe->m_pGenSvc = 0;
    }
    if (pCMe->m_pGenObj != 0) {
        pCMe->m_pGenObj->pVTbl->Release(pCMe->m_pGenObj);
        pCMe->m_pGenObj = 0;
    }
}

static int16_t %(cn)s_SelectGenInternal(/* in */ %(cn)s* pCMe, /* in */ uint16_t kind) {
    int16_t result = ERR_ECO_SUCCESS;

    if (pCMe->m_pIBus == 0) {
        return ERR_ECO_FAIL;
    }
    if (kind > 2) {
        return ERR_ECO_INVALIDARG;
    }
    %(cn)s_ReleaseGenerator(pCMe);
    result = pCMe->m_pIBus->pVTbl->QueryComponent(pCMe->m_pIBus,
                 (kind == 0) ? &CID_EcoFibonacciLFSR1 : ((kind == 1) ? &CID_EcoGaloisLFSR1 : &CID_EcoXorshift1),
                 0, &IID_IEcoPseudoRandom1, (void**)&pCMe->m_pGenObj);
    if (result == ERR_ECO_SUCCESS) {
        result = pCMe->m_pGenObj->pVTbl->QueryInterface(pCMe->m_pGenObj, &IID_IEcoPseudoRandomService1, (voidptr_t*)&pCMe->m_pGenSvc);
    }
    if (result != ERR_ECO_SUCCESS) {
        %(cn)s_ReleaseGenerator(pCMe);
        return result;
    }
    pCMe->m_kindGen = kind;
    return ERR_ECO_SUCCESS;
}

""" % {"cn": cn}

    # --- delegating IEcoUnknown for every interface
    for idx, (it, tag) in enumerate(ifaces):
        ptr = it + "Ptr_t"
        s += mheader("Delegating IEcoUnknown (%s)" % it, ["QueryInterface, AddRef and Release of every interface delegate to the controlling unknown."])
        s += """static int16_t ECOCALLMETHOD %(cn)s_%(tag)s_QueryInterface(/* in */ %(ptr)s me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv) {
    %(cn)s* pCMe = 0;

    if (me == 0 || riid == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }
    pCMe = %(fix)s;
    return pCMe->m_pIUnkOuter->pVTbl->QueryInterface(pCMe->m_pIUnkOuter, riid, ppv);
}

static uint32_t ECOCALLMETHOD %(cn)s_%(tag)s_AddRef(/* in */ %(ptr)s me) {
    %(cn)s* pCMe = 0;

    if (me == 0) {
        return (uint32_t)-1; /* ERR_ECO_POINTER */
    }
    pCMe = %(fix)s;
    return pCMe->m_pIUnkOuter->pVTbl->AddRef(pCMe->m_pIUnkOuter);
}

static uint32_t ECOCALLMETHOD %(cn)s_%(tag)s_Release(/* in */ %(ptr)s me) {
    %(cn)s* pCMe = 0;

    if (me == 0) {
        return (uint32_t)-1; /* ERR_ECO_POINTER */
    }
    pCMe = %(fix)s;
    return pCMe->m_pIUnkOuter->pVTbl->Release(pCMe->m_pIUnkOuter);
}

""" % {"cn": cn, "tag": tag, "ptr": ptr, "fix": fix(idx)}

    # --- non-delegating IEcoUnknown
    own_iid = "IID_" + own
    q = ""
    q += """    if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &pCMe->m_pVTblINondelegatingUnk;
        ++pCMe->m_cRef;
        return ERR_ECO_SUCCESS;
    }
    if (IsEqualUGUID(riid, &%(own_iid)s)) {
        *ppv = &pCMe->%(own_vt)s;
        pCMe->m_pIUnkOuter->pVTbl->AddRef(pCMe->m_pIUnkOuter);
        return ERR_ECO_SUCCESS;
    }
    /* IEcoCalculatorX: the interface of the aggregated inner component goes to the client directly, otherwise this component's own */
    if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        if (pCMe->m_pXInner != 0) {
            return pCMe->m_pXInner->pVTbl->QueryInterface(pCMe->m_pXInner, riid, ppv);
        }
        if (pCMe->m_pIXc == 0) {
            *ppv = 0;
            return ERR_ECO_NOINTERFACE;
        }
        *ppv = &pCMe->m_pVTblIEcoCalculatorX;
        pCMe->m_pIUnkOuter->pVTbl->AddRef(pCMe->m_pIUnkOuter);
        return ERR_ECO_SUCCESS;
    }
    if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        if (pCMe->m_pYInner != 0) {
            return pCMe->m_pYInner->pVTbl->QueryInterface(pCMe->m_pYInner, riid, ppv);
        }
        if (pCMe->m_pIYc == 0) {
            *ppv = 0;
            return ERR_ECO_NOINTERFACE;
        }
        *ppv = &pCMe->m_pVTblIEcoCalculatorY;
        pCMe->m_pIUnkOuter->pVTbl->AddRef(pCMe->m_pIUnkOuter);
        return ERR_ECO_SUCCESS;
    }
""" % {"own_iid": own_iid, "own_vt": own_vt}
    if has_gen:
        q += """    /* the pseudo random interfaces of lab 1 are re-implemented here and forwarded to the contained generator */
    if (IsEqualUGUID(riid, &IID_IEcoPseudoRandomService1)) {
        *ppv = &pCMe->m_pVTblIEcoPseudoRandomService1;
        pCMe->m_pIUnkOuter->pVTbl->AddRef(pCMe->m_pIUnkOuter);
        return ERR_ECO_SUCCESS;
    }
    if (IsEqualUGUID(riid, &IID_IEcoPseudoRandom1)) {
        *ppv = &pCMe->m_pVTblIEcoPseudoRandom1;
        pCMe->m_pIUnkOuter->pVTbl->AddRef(pCMe->m_pIUnkOuter);
        return ERR_ECO_SUCCESS;
    }
"""
    if has_lab1:
        q += """    /* every other interface comes from the aggregated Eco.Lab1Outer (its pointers are handed to the client directly) */
    if (pCMe->m_pLab1Inner != 0) {
        return pCMe->m_pLab1Inner->pVTbl->QueryInterface(pCMe->m_pLab1Inner, riid, ppv);
    }
"""
    q += "    *ppv = 0;\n    return ERR_ECO_NOINTERFACE;\n"
    s += mheader("Non-delegating IEcoUnknown", ["Real reference counting and interface lookup of the component itself (interface %d of the object)." % nd_idx])
    s += """static int16_t ECOCALLMETHOD %(cn)s_Nondelegating_QueryInterface(/* in */ IEcoUnknownPtr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv) {
    %(cn)s* pCMe = 0;

    if (me == 0 || riid == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }
    pCMe = %(fix)s;

%(q)s}

static uint32_t ECOCALLMETHOD %(cn)s_Nondelegating_AddRef(/* in */ IEcoUnknownPtr_t me) {
    %(cn)s* pCMe = 0;

    if (me == 0) {
        return (uint32_t)-1; /* ERR_ECO_POINTER */
    }
    pCMe = %(fix)s;
    return ++pCMe->m_cRef;
}

static uint32_t ECOCALLMETHOD %(cn)s_Nondelegating_Release(/* in */ IEcoUnknownPtr_t me) {
    %(cn)s* pCMe = 0;

    if (me == 0) {
        return (uint32_t)-1; /* ERR_ECO_POINTER */
    }
    pCMe = %(fix)s;
    --pCMe->m_cRef;
    if (pCMe->m_cRef == 0) {
        pCMe->Delete(pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

""" % {"cn": cn, "fix": fix(nd_idx), "q": q}

    # --- X / Y forwarding (containment)
    for iface, tag, idx, a, b in (("X", "X", 1, ("Addition", "int32_t"), ("Subtraction", "int16_t")), ("Y", "Y", 2, ("Multiplication", "int32_t"), ("Division", "int16_t"))):
        for (mn, rt) in (a, b):
            s += mheader("%s Function (containment)" % mn, ["Forwards the call to the contained inner component; returns 0 when none is contained."])
            s += """static %(rt)s ECOCALLMETHOD %(cn)s_%(tag)s_%(mn)s(/* in */ IEcoCalculator%(iface)sPtr_t me, /* in */ int16_t a, /* in */ int16_t b) {
    %(cn)s* pCMe = 0;

    if (me == 0) {
        return 0;
    }
    pCMe = %(fix)s;
    if (pCMe->m_pI%(iface)sc == 0) {
        return 0;
    }
    return pCMe->m_pI%(iface)sc->pVTbl->%(mn)s(pCMe->m_pI%(iface)sc, a, b);
}

""" % {"rt": rt, "cn": cn, "tag": tag, "mn": mn, "iface": iface, "fix": fix(idx)}

    if has_gen:
        s += mheader("Step32 / Step64 Functions (containment of lab 1)", ["Forward the stateless call to the contained lab 1 generator."])
        for mn, tp in (("Step32", "uint32_t"), ("Step64", "uint64_t")):
            s += """static int16_t ECOCALLMETHOD %(cn)s_Svc_%(mn)s(/* in */ IEcoPseudoRandomService1Ptr_t me, /* in */ %(tp)s state, /* in */ %(tp)s param, /* out */ %(tp)s* newState) {
    %(cn)s* pCMe = 0;

    if (me == 0 || newState == 0) {
        return ERR_ECO_POINTER;
    }
    pCMe = %(fix)s;
    if (pCMe->m_pGenSvc == 0) {
        return ERR_ECO_FAIL;
    }
    return pCMe->m_pGenSvc->pVTbl->%(mn)s(pCMe->m_pGenSvc, state, param, newState);
}

""" % {"cn": cn, "mn": mn, "tp": tp, "fix": fix(3)}
        s += mheader("Init / Next / get_Name Functions (containment of lab 1)", ["Forward the stateful call to the contained lab 1 generator."])
        s += """static int16_t ECOCALLMETHOD %(cn)s_Obj_Init(/* in */ IEcoPseudoRandom1Ptr_t me, /* in */ uint64_t seed, /* in */ uint64_t param, /* in */ uint16_t bits) {
    %(cn)s* pCMe = 0;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }
    pCMe = %(fix)s;
    if (pCMe->m_pGenObj == 0) {
        return ERR_ECO_FAIL;
    }
    return pCMe->m_pGenObj->pVTbl->Init(pCMe->m_pGenObj, seed, param, bits);
}

static int16_t ECOCALLMETHOD %(cn)s_Obj_Next(/* in */ IEcoPseudoRandom1Ptr_t me, /* out */ uint64_t* value) {
    %(cn)s* pCMe = 0;

    if (me == 0 || value == 0) {
        return ERR_ECO_POINTER;
    }
    pCMe = %(fix)s;
    if (pCMe->m_pGenObj == 0) {
        return ERR_ECO_FAIL;
    }
    return pCMe->m_pGenObj->pVTbl->Next(pCMe->m_pGenObj, value);
}

static char_t* ECOCALLMETHOD %(cn)s_Obj_get_Name(/* in */ IEcoPseudoRandom1Ptr_t me) {
    %(cn)s* pCMe = 0;

    if (me == 0) {
        return 0;
    }
    pCMe = %(fix)s;
    if (pCMe->m_pGenObj == 0) {
        return 0;
    }
    return pCMe->m_pGenObj->pVTbl->get_Name(pCMe->m_pGenObj);
}

""" % {"cn": cn, "fix": fix(4)}

    # --- own interface
    s += mheader("%s methods" % own, ["SelectCalculators chooses the inner calculator components" + (", SelectGenerator the contained lab 1 generator." if has_gen else ".")])
    if has_gen:
        s += """static int16_t ECOCALLMETHOD %(cn)s_Own_SelectGenerator(/* in */ %(own)sPtr_t me, /* in */ uint16_t kind) {
    if (me == 0) {
        return ERR_ECO_POINTER;
    }
    return %(cn)s_SelectGenInternal((%(cn)s*)me, kind);
}

""" % {"cn": cn, "own": own}
    s += """static int16_t ECOCALLMETHOD %(cn)s_Own_SelectCalculators(/* in */ %(own)sPtr_t me, /* in */ uint16_t kindX, /* in */ uint16_t kindY) {
    if (me == 0) {
        return ERR_ECO_POINTER;
    }
    return %(cn)s_SelectCalcInternal((%(cn)s*)me, kindX, kindY);
}

""" % {"cn": cn, "own": own}

    # --- Init / Create / Delete
    defaults = "    result = %s_SelectCalcInternal(pCMe, 0, 0);\n    if (result != ERR_ECO_SUCCESS) {\n        return result;\n    }\n" % cn
    if has_gen:
        defaults += "    result = %s_SelectGenInternal(pCMe, 2);\n    if (result != ERR_ECO_SUCCESS) {\n        return result;\n    }\n" % cn
    if has_lab1:
        defaults += """    /* Eco.Lab1Outer is aggregated: it receives the delegating IEcoUnknown of this component as its controlling unknown */
    result = pCMe->m_pIBus->pVTbl->QueryComponent(pCMe->m_pIBus, &CID_EcoLab1Outer, (IEcoUnknown*)&pCMe->%s, &IID_IEcoUnknown, (void**)&pCMe->m_pLab1Inner);
    if (result != ERR_ECO_SUCCESS) {
        return result;
    }
""" % own_vt
    s += mheader("Init Function", ["Instance initialization: gets the bus and the allocator (as in the template) and creates the default inner components."])
    s += """static int16_t ECOCALLMETHOD init%(cn)s(/*in*/ %(cn)sPtr_t me, /* in */ IEcoUnknownPtr_t pIUnkSystem) {
    %(cn)s* pCMe = (%(cn)s*)me;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoInterfaceBus1MemExt* pIMemExt = 0;
    int16_t result = ERR_ECO_POINTER;
    UGUID* rcid = (UGUID*)&CID_EcoMemoryManager1;

    /* Pointer Validation */
    if (me == 0 ) {
        return result;
    }

    /* Storing the pointer to the system interface */
    pCMe->m_pISys = (IEcoSystem1*)pIUnkSystem;

    /* Getting the interface for working with the interface bus */
    result = pCMe->m_pISys->pVTbl->QueryInterface(pCMe->m_pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    if (result != 0 || pIBus == 0) {
        return ERR_ECO_NOBUS;
    }

    /* Getting the component ID for working with memory */
    result = pIBus->pVTbl->QueryInterface(pIBus, &IID_IEcoInterfaceBus1MemExt, (void**)&pIMemExt);
    if (result == 0 && pIMemExt != 0) {
        rcid = (UGUID*)pIMemExt->pVTbl->get_Manager(pIMemExt);
        pIMemExt->pVTbl->Release(pIMemExt);
    }

    /* Getting the memory allocator interface */
    result = pIBus->pVTbl->QueryComponent(pIBus, rcid, 0, &IID_IEcoMemoryAllocator1, (void**) &pCMe->m_pIMem);
    if (result != 0 || pCMe->m_pIMem == 0) {
        pIBus->pVTbl->Release(pIBus);
        return ERR_ECO_GET_MEMORY_ALLOCATOR;
    }

    /* The bus is kept to create the inner components; it is released in Delete */
    pCMe->m_pIBus = pIBus;

%(defaults)s    return ERR_ECO_SUCCESS;
}

""" % {"cn": cn, "defaults": defaults}
    s += mheader("Create Function", ["Instance creation: an outer IEcoUnknown (aggregation) becomes the controlling unknown, otherwise the component controls itself."])
    s += """static int16_t ECOCALLMETHOD create%(cn)s(/* in */ %(cn)sPtr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter) {

    /* Pointer Validation */
    if (pCMe == 0) {
        return ERR_ECO_POINTER;
    }
    if (pIUnkOuter != 0) {
        pCMe->m_pIUnkOuter = pIUnkOuter;
    }
    else {
        pCMe->m_pIUnkOuter = (IEcoUnknown*)&pCMe->m_pVTblINondelegatingUnk;
    }
    return ERR_ECO_SUCCESS;
}

""" % {"cn": cn}
    gen_rel = "        %s_ReleaseGenerator(pCMe);\n" % cn if has_gen else ""
    lab1_rel = "        if (pCMe->m_pLab1Inner != 0) {\n            pCMe->m_pLab1Inner->pVTbl->Release(pCMe->m_pLab1Inner);\n            pCMe->m_pLab1Inner = 0;\n        }\n" if has_lab1 else ""
    s += mheader("Delete Function", ["Instance freeing: the inner components and the bus are released first (the counter is stabilised so that a re-entrant Release cannot destroy the object twice)."])
    s += """static void ECOCALLMETHOD delete%(cn)s(/* in */ %(cn)sPtr_t pCMe) {
    IEcoMemoryAllocator1* pIMem = 0;

    if (pCMe != 0 ) {
        pCMe->m_cRef = 1;
        %(cn)s_ReleaseCalculators(pCMe);
%(gen_rel)s%(lab1_rel)s        if (pCMe->m_pIBus != 0) {
            pCMe->m_pIBus->pVTbl->Release(pCMe->m_pIBus);
            pCMe->m_pIBus = 0;
        }
        pIMem = pCMe->m_pIMem;
        /* Freeing */
        if ( pCMe->m_Name != 0 ) {
            pIMem->pVTbl->Free(pIMem, pCMe->m_Name);
        }
        if ( pCMe->m_pISys != 0 ) {
            pCMe->m_pISys->pVTbl->Release(pCMe->m_pISys);
        }
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
}

""" % {"cn": cn, "gen_rel": gen_rel, "lab1_rel": lab1_rel}

    # --- tables and the global object
    meth = {
        "Own": ([("Own_SelectGenerator" if has_gen else None), "Own_SelectCalculators"]),
        "X": ["X_Addition", "X_Subtraction"],
        "Y": ["Y_Multiplication", "Y_Division"],
        "Svc": ["Svc_Step32", "Svc_Step64"],
        "Obj": ["Obj_Init", "Obj_Next", "Obj_get_Name"],
    }
    for idx, (it, tag) in enumerate(ifaces):
        s += "/* %s Virtual Table */\n%sVTbl g_x%sVTbl_%s = {\n    %s_%s_QueryInterface,\n    %s_%s_AddRef,\n    %s_%s_Release" % (it, it, it, sfx, cn, tag, cn, tag, cn, tag)
        for m in meth[tag]:
            if m:
                s += ",\n    %s_%s" % (cn, m)
        s += "\n};\n\n"
    s += "/* Non-delegating IEcoUnknown Virtual Table */\nIEcoUnknownVTbl g_xINondelegatingUnkVTbl_%s = {\n    %s_Nondelegating_QueryInterface,\n    %s_Nondelegating_AddRef,\n    %s_Nondelegating_Release\n};\n\n" % (sfx, cn, cn, cn)
    s += "/* Object Instance */\n%s g_x%s = {\n" % (cn, cn)
    for it, tag in ifaces:
        s += "    &g_x%sVTbl_%s,\n" % (it, sfx)
    s += "    &g_xINondelegatingUnkVTbl_%s,\n\n    init%s,\n    create%s,\n    delete%s,\n    1, /* m_cRef */\n    0, /* m_pIMem */\n    0, /* m_pISys */\n    0, /* m_Name */\n    0, /* m_pIUnkOuter */\n    0, /* m_pIBus */\n    0, /* m_kindX */\n    0, /* m_kindY */\n    0, /* m_pIXc */\n    0, /* m_pIYc */\n    0, /* m_pXInner */\n    0%s /* m_pYInner */\n" % (sfx, cn, cn, cn, "," if (has_gen or has_lab1) else "")
    if has_gen:
        s += "    0, /* m_pGenSvc */\n    0, /* m_pGenObj */\n    0%s /* m_kindGen */\n" % ("," if has_lab1 else "")
    if has_lab1:
        s += "    0  /* m_pLab1Inner */\n"
    s += "};\n"

    # --- write files
    write(os.path.join(pdir, "HeaderFiles", "CEco%s.h" % name), header)
    write(os.path.join(pdir, "SourceFiles", "CEco%s.c" % name), s)

    # interface header of the component itself (keeps the IID of the wizard stub, parsed from the current header)
    cur = read(os.path.join(pdir, "SharedFiles", "IEco%s.h" % name))
    write(os.path.join(pdir, "SharedFiles", "IEco%s.h" % name), gen_interface(name, spec, cur))

    # factory: the same patch as for the aggregatable calculators (non-delegating QueryInterface, Create result)
    path = os.path.join(pdir, "SourceFiles", "CEco%sFactory.c" % name)
    t = read(path)
    if "Create result" not in t and "NON-delegating" not in t:
        a = t.index("    /* Component creation */")
        b = t.index("    return result;\n}", a)
        new = """    /* Component creation */
    result = pCObj->Create(pCObj, pISystem, pIUnknownOuter);
    if (result != ERR_ECO_SUCCESS) {
        pIMem->pVTbl->Free(pIMem, pCObj);
        pIMem->pVTbl->Release(pIMem);
        pIBus->pVTbl->Release(pIBus);
        pISys->pVTbl->Release(pISys);
        return result;
    }

    /* Component initialization: on failure the half-built object is destroyed */
    result = pCObj->Init(pCObj, pISystem);
    if (result != ERR_ECO_SUCCESS) {
        pCObj->m_cRef = 1;
        pCObj->Delete(pCObj);
        return result;
    }

    /* Getting a pointer to the interface through the NON-delegating IEcoUnknown (required for aggregation) */
    pIUnk = (IEcoUnknown*)&pCObj->m_pVTblINondelegatingUnk;
    result = pIUnk->pVTbl->QueryInterface(pIUnk, riid, ppv);

    /* Decrementing the reference requested by the Component Factory */
    pIUnk->pVTbl->Release(pIUnk);

"""
        t = t[:a] + new + t[b:]
        write(path, t)
    print("generated Eco.%s (CID suffix %s)" % (name, sfx))


for name, spec in OUTERS.items():
    build(name, spec)
