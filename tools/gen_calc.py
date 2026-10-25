"""Generates the sources of the calculator components A-E (assignment 2) into wizard-created projects.

usage: gen_calc.py <repo root>

Each component implements IEcoCalculatorX and/or IEcoCalculatorY. Components B and E support aggregation
(non-delegating IEcoUnknown); A, C and D support containment only and refuse an outer IEcoUnknown.
The implementation policies differ on purpose (documented in every file), so a test can tell which
component answered through any interface pointer.
"""
import os
import re
import sys

ROOT = os.path.abspath(sys.argv[1]) if len(sys.argv) > 1 else os.getcwd()

# name: (interfaces in table order, aggregatable, subtraction-overflow policy, division-by-zero value, display name)
SPEC = {
    "CalcA": (["X"], False, "saturate", None, "Calculator A"),
    "CalcB": (["X"], True, "wrap", None, "Calculator B"),
    "CalcC": (["X", "Y"], False, "zero", -1, "Calculator C"),
    "CalcD": (["Y"], False, "saturate", 0, "Calculator D"),
    "CalcE": (["Y"], True, "saturate", 32767, "Calculator E"),
}
IFACE_METHODS = {"X": ("Addition", "Subtraction"), "Y": ("Multiplication", "Division")}


def write(path, text):
    with open(path, "w", encoding="utf-8-sig", newline="") as f:
        f.write(text.replace("\r\n", "\n").replace("\n", "\r\n"))


def read(path):
    return open(path, encoding="utf-8-sig").read().replace("\r\n", "\n")


def file_header(summary, description):
    return """/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   %s
 * </summary>
 *
 * <description>
%s
 * </description>
 *
 * <author>
 *   Copyright (c) 2026 . All rights reserved.
 * </author>
 *
 */
""" % (summary, "\n".join(" *   " + l for l in description))


def method_header(summary, description):
    return """/*
 *
 * <summary>
 *   %s
 * </summary>
 *
 * <description>
%s
 * </description>
 *
 */
""" % (summary, "\n".join(" *   " + l for l in description))


def project_info(name):
    pdir = os.path.join(ROOT, "Eco." + name)
    idh = read(os.path.join(pdir, "SharedFiles", "IdEco%s.h" % name))
    cid = re.search(r"GetIEcoComponentFactoryPtr_([0-9A-F]{32});", idh).group(1)
    return pdir, cid, cid[-8:]


def gen_header(name, sfx, ifaces, aggr):
    cn = "CEco%s_%s" % (name, sfx)
    incs = "".join('#include "IEcoCalculator%s.h"\n' % i for i in ifaces)
    tables = ""
    for n, i in enumerate(ifaces):
        tables += "    /* IEcoCalculator%s interface function table (interface %d) */\n    IEcoCalculator%sVTbl* m_pVTblIEcoCalculator%s;\n\n" % (i, n, i, i)
    if aggr:
        tables += "    /* Non-delegating IEcoUnknown function table (interface %d) */\n    IEcoUnknownVTbl* m_pVTblINondelegatingUnk;\n\n" % len(ifaces)
    outer = "\n    /* Controlling IEcoUnknown: the outer component when aggregated, the non-delegating IEcoUnknown otherwise */\n    IEcoUnknown* m_pIUnkOuter;\n" if aggr else ""
    return file_header(cn, ["This header describes the implementation of the %s component" % cn]) + """
#ifndef __C_ECO%s_H__
#define __C_ECO%s_H__

%s#include "IEcoSystem1.h"
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
""" % (name.upper(), name.upper(), incs, cn, cn, cn, tables, cn, cn, cn, outer, cn, name.upper())


def method_code(name, sfx, iface, idx, policy, divzero):
    """C code of the two methods of one calculator interface; idx is the interface index inside the object."""
    cn = "CEco%s_%s" % (name, sfx)
    me_fix = "(%s*)me" % cn if idx == 0 else "(%s*)((char_t*)me - %d * sizeof(voidptr_t))" % (cn, idx)
    out = ""
    m1, m2 = IFACE_METHODS[iface]
    ptr = "IEcoCalculator%sPtr_t" % iface
    if iface == "X":
        out += method_header("%s Function" % m1, ["32 bit sum of two 16 bit operands. Returns 0 for me == NULL."])
        out += """static int32_t ECOCALLMETHOD %s_%s(/* in */ %s me, /* in */ int16_t a, /* in */ int16_t b) {
    if (me == 0) {
        return 0;
    }
    return (int32_t)a + (int32_t)b;
}

""" % (cn, m1, ptr)
        if policy == "saturate":
            fix = "        diff = (diff < 0) ? -32768 : 32767;"
            doc = "On overflow of the 16 bit result the value is saturated to -32768 or 32767."
        elif policy == "wrap":
            fix = "        diff = (int32_t)(int16_t)((uint16_t)((uint16_t)a - (uint16_t)b));"
            doc = "On overflow of the 16 bit result the value wraps modulo 65536 (two's complement)."
        else:
            fix = "        diff = 0;"
            doc = "On overflow of the 16 bit result the method returns 0."
        out += method_header("%s Function" % m2, ["16 bit difference a - b. Returns 0 for me == NULL.", doc])
        out += """static int16_t ECOCALLMETHOD %s_%s(/* in */ %s me, /* in */ int16_t a, /* in */ int16_t b) {
    int32_t diff = 0;

    if (me == 0) {
        return 0;
    }
    diff = (int32_t)a - (int32_t)b;
    if (diff < -32768 || diff > 32767) {
%s
    }
    return (int16_t)diff;
}

""" % (cn, m2, ptr, fix)
    else:
        out += method_header("%s Function" % m1, ["32 bit product of two 16 bit operands. Returns 0 for me == NULL."])
        out += """static int32_t ECOCALLMETHOD %s_%s(/* in */ %s me, /* in */ int16_t a, /* in */ int16_t b) {
    if (me == 0) {
        return 0;
    }
    return (int32_t)a * (int32_t)b;
}

""" % (cn, m1, ptr)
        out += method_header("%s Function" % m2, ["16 bit quotient a / b truncated toward zero; the only overflowing quotient (-32768 / -1) is saturated to 32767.",
                                                    "Division by zero returns %d. Returns 0 for me == NULL." % divzero])
        out += """static int16_t ECOCALLMETHOD %s_%s(/* in */ %s me, /* in */ int16_t a, /* in */ int16_t b) {
    int32_t quot = 0;

    if (me == 0) {
        return 0;
    }
    if (b == 0) {
        return (int16_t)(%d);
    }
    quot = (int32_t)a / (int32_t)b;
    if (quot > 32767) {
        quot = 32767;
    }
    return (int16_t)quot;
}

""" % (cn, m2, ptr, divzero)
    return out


def gen_source(name, sfx, ifaces, aggr, policy, divzero, display):
    cn = "CEco%s_%s" % (name, sfx)
    s = file_header(cn, ["This source code describes the implementation of the interfaces for %s (%s)" % (cn, display)])
    s += """

#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "IEcoInterfaceBus1MemExt.h"
#include "%s.h"

""" % ("CEco" + name)
    n_if = len(ifaces)
    # ---- IEcoUnknown parts
    if not aggr:
        s += method_header("QueryInterface Function", ["Common QueryInterface of all interfaces of the component: every interface can return every other one."])
        s += "static int16_t %s_QueryCommon(/* in */ %s* pCMe, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv) {\n" % (cn, cn)
        s += "    if (pCMe == 0 || riid == 0 || ppv == 0) {\n        return ERR_ECO_POINTER;\n    }\n\n"
        first = True
        for n, i in enumerate(ifaces):
            s += "    %sif (IsEqualUGUID(riid, &IID_IEcoCalculator%s)) {\n        *ppv = &pCMe->m_pVTblIEcoCalculator%s;\n        ++pCMe->m_cRef;\n    }\n" % ("" if first else "else ", i, i)
            first = False
        s += "    else if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {\n        *ppv = &pCMe->m_pVTblIEcoCalculator%s;\n        ++pCMe->m_cRef;\n    }\n" % ifaces[0]
        s += "    else {\n        *ppv = 0;\n        return ERR_ECO_NOINTERFACE;\n    }\n    return ERR_ECO_SUCCESS;\n}\n\n"
        for n, i in enumerate(ifaces):
            ptr = "IEcoCalculator%sPtr_t" % i
            fix = "(%s*)me" % cn if n == 0 else "(%s*)((char_t*)me - %d * sizeof(voidptr_t))" % (cn, n)
            s += method_header("QueryInterface Function (IEcoCalculator%s)" % i, ["QueryInterface function for the IEcoCalculator%s interface" % i])
            s += "static int16_t ECOCALLMETHOD %s_%s_QueryInterface(/* in */ %s me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv) {\n    if (me == 0) {\n        return ERR_ECO_POINTER;\n    }\n    return %s_QueryCommon(%s, riid, ppv);\n}\n\n" % (cn, i, ptr, cn, fix)
            s += method_header("AddRef Function (IEcoCalculator%s)" % i, ["AddRef function for the IEcoCalculator%s interface (one counter for all interfaces)" % i])
            s += "static uint32_t ECOCALLMETHOD %s_%s_AddRef(/* in */ %s me) {\n    %s* pCMe = 0;\n\n    if (me == 0) {\n        return (uint32_t)-1; /* ERR_ECO_POINTER */\n    }\n    pCMe = %s;\n    return ++pCMe->m_cRef;\n}\n\n" % (cn, i, ptr, cn, fix)
            s += method_header("Release Function (IEcoCalculator%s)" % i, ["Release function for the IEcoCalculator%s interface (one counter for all interfaces)" % i])
            s += "static uint32_t ECOCALLMETHOD %s_%s_Release(/* in */ %s me) {\n    %s* pCMe = 0;\n\n    if (me == 0) {\n        return (uint32_t)-1; /* ERR_ECO_POINTER */\n    }\n    pCMe = %s;\n    --pCMe->m_cRef;\n    if (pCMe->m_cRef == 0) {\n        pCMe->Delete(pCMe);\n        return 0;\n    }\n    return pCMe->m_cRef;\n}\n\n" % (cn, i, ptr, cn, fix)
    else:
        i = ifaces[0]
        ptr = "IEcoCalculator%sPtr_t" % i
        s += method_header("Delegating IEcoUnknown (IEcoCalculator%s)" % i, [
            "The IEcoUnknown methods of the calculator interface always delegate to the controlling unknown:",
            "to the outer component when the component is aggregated, otherwise to its own non-delegating IEcoUnknown."])
        s += "static int16_t ECOCALLMETHOD %s_%s_QueryInterface(/* in */ %s me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv) {\n    %s* pCMe = (%s*)me;\n\n    if (me == 0 || riid == 0 || ppv == 0) {\n        return ERR_ECO_POINTER;\n    }\n    return pCMe->m_pIUnkOuter->pVTbl->QueryInterface(pCMe->m_pIUnkOuter, riid, ppv);\n}\n\n" % (cn, i, ptr, cn, cn)
        s += "static uint32_t ECOCALLMETHOD %s_%s_AddRef(/* in */ %s me) {\n    %s* pCMe = (%s*)me;\n\n    if (me == 0) {\n        return (uint32_t)-1; /* ERR_ECO_POINTER */\n    }\n    return pCMe->m_pIUnkOuter->pVTbl->AddRef(pCMe->m_pIUnkOuter);\n}\n\n" % (cn, i, ptr, cn, cn)
        s += "static uint32_t ECOCALLMETHOD %s_%s_Release(/* in */ %s me) {\n    %s* pCMe = (%s*)me;\n\n    if (me == 0) {\n        return (uint32_t)-1; /* ERR_ECO_POINTER */\n    }\n    return pCMe->m_pIUnkOuter->pVTbl->Release(pCMe->m_pIUnkOuter);\n}\n\n" % (cn, i, ptr, cn, cn)
        s += method_header("Non-delegating IEcoUnknown", [
            "Real reference counting and interface lookup of the component itself (interface %d of the object)." % n_if,
            "QueryInterface of any interface other than IEcoUnknown adds the reference through the controlling unknown."])
        fix = "(%s*)((char_t*)me - %d * sizeof(voidptr_t))" % (cn, n_if)
        s += "static int16_t ECOCALLMETHOD %s_Nondelegating_QueryInterface(/* in */ IEcoUnknownPtr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv) {\n    %s* pCMe = 0;\n\n    if (me == 0 || riid == 0 || ppv == 0) {\n        return ERR_ECO_POINTER;\n    }\n    pCMe = %s;\n\n    if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {\n        *ppv = &pCMe->m_pVTblINondelegatingUnk;\n        ++pCMe->m_cRef;\n    }\n    else if (IsEqualUGUID(riid, &IID_IEcoCalculator%s)) {\n        *ppv = &pCMe->m_pVTblIEcoCalculator%s;\n        pCMe->m_pIUnkOuter->pVTbl->AddRef(pCMe->m_pIUnkOuter);\n    }\n    else {\n        *ppv = 0;\n        return ERR_ECO_NOINTERFACE;\n    }\n    return ERR_ECO_SUCCESS;\n}\n\n" % (cn, cn, fix, i, i)
        s += "static uint32_t ECOCALLMETHOD %s_Nondelegating_AddRef(/* in */ IEcoUnknownPtr_t me) {\n    %s* pCMe = 0;\n\n    if (me == 0) {\n        return (uint32_t)-1; /* ERR_ECO_POINTER */\n    }\n    pCMe = %s;\n    return ++pCMe->m_cRef;\n}\n\n" % (cn, cn, fix)
        s += "static uint32_t ECOCALLMETHOD %s_Nondelegating_Release(/* in */ IEcoUnknownPtr_t me) {\n    %s* pCMe = 0;\n\n    if (me == 0) {\n        return (uint32_t)-1; /* ERR_ECO_POINTER */\n    }\n    pCMe = %s;\n    --pCMe->m_cRef;\n    if (pCMe->m_cRef == 0) {\n        pCMe->Delete(pCMe);\n        return 0;\n    }\n    return pCMe->m_cRef;\n}\n\n" % (cn, cn, fix)
    # ---- calculator methods
    for n, i in enumerate(ifaces):
        s += method_code(name, sfx, i, n, policy, divzero)
    # ---- Init / Create / Delete (template logic)
    s += method_header("Init Function", ["Instance initialization function"])
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

    /* Getting the component ID for working with memory */
    result = pIBus->pVTbl->QueryInterface(pIBus, &IID_IEcoInterfaceBus1MemExt, (void**)&pIMemExt);
    if (result == 0 && pIMemExt != 0) {
        rcid = (UGUID*)pIMemExt->pVTbl->get_Manager(pIMemExt);
        pIMemExt->pVTbl->Release(pIMemExt);
    }

    /* Getting the memory allocator interface */
    result = pIBus->pVTbl->QueryComponent(pIBus, rcid, 0, &IID_IEcoMemoryAllocator1, (void**) &pCMe->m_pIMem);
    /* Check */
    if (result != 0 || pCMe->m_pIMem == 0) {
        result = ERR_ECO_GET_MEMORY_ALLOCATOR;
    }

    /* Freeing */
    pIBus->pVTbl->Release(pIBus);

    return result;
}

""" % {"cn": cn}
    s += method_header("Create Function", ["Instance creation function"] + (
        ["The component supports aggregation: an outer IEcoUnknown becomes the controlling unknown, otherwise the component controls itself."] if aggr else
        ["The component supports containment only: an outer IEcoUnknown is refused with ERR_ECO_NOAGGREGATION."]))
    if aggr:
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
    else:
        s += """static int16_t ECOCALLMETHOD create%(cn)s(/* in */ %(cn)sPtr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter) {

    /* Pointer Validation */
    if (pCMe == 0) {
        return ERR_ECO_POINTER;
    }
    if (pIUnkOuter != 0) {
        return ERR_ECO_NOAGGREGATION;
    }
    return ERR_ECO_SUCCESS;
}

""" % {"cn": cn}
    s += method_header("Delete Function", ["Instance freeing function"])
    s += """static void ECOCALLMETHOD delete%(cn)s(/* in */ %(cn)sPtr_t pCMe) {
    IEcoMemoryAllocator1* pIMem = 0;

    if (pCMe != 0 ) {
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

""" % {"cn": cn}
    # ---- tables and the global object
    for n, i in enumerate(ifaces):
        m1, m2 = IFACE_METHODS[i]
        s += "/* IEcoCalculator%s Virtual Table */\nIEcoCalculator%sVTbl g_xIEcoCalculator%sVTbl_%s = {\n    %s_%s_QueryInterface,\n    %s_%s_AddRef,\n    %s_%s_Release,\n    %s_%s,\n    %s_%s\n};\n\n" % (i, i, i, sfx, cn, i, cn, i, cn, i, cn, m1, cn, m2)
    if aggr:
        s += "/* Non-delegating IEcoUnknown Virtual Table */\nIEcoUnknownVTbl g_xINondelegatingUnkVTbl_%s = {\n    %s_Nondelegating_QueryInterface,\n    %s_Nondelegating_AddRef,\n    %s_Nondelegating_Release\n};\n\n" % (sfx, cn, cn, cn)
    s += "/* Object Instance */\n%s g_x%s = {\n" % (cn, cn)
    for n, i in enumerate(ifaces):
        s += "    &g_xIEcoCalculator%sVTbl_%s,\n" % (i, sfx)
    if aggr:
        s += "    &g_xINondelegatingUnkVTbl_%s,\n" % sfx
    s += "\n    init%s,\n    create%s,\n    delete%s,\n    1, /* m_cRef */\n    0, /* m_pIMem */\n    0, /* m_pISys */\n    0%s /* m_Name */\n" % (cn, cn, cn, "," if aggr else "")
    if aggr:
        s += "    0  /* m_pIUnkOuter */\n"
    s += "};\n"
    return s


def patch_factory(pdir, name, sfx, aggr):
    path = os.path.join(pdir, "SourceFiles", "CEco%sFactory.c" % name)
    t = read(path)
    a = t.index("    /* Component creation */")
    b = t.index("    return result;\n}", a)
    new = """    /* Component creation: a component that does not support aggregation refuses the outer unknown */
    result = pCObj->Create(pCObj, pISystem, pIUnknownOuter);
    if (result != ERR_ECO_SUCCESS) {
        pIMem->pVTbl->Free(pIMem, pCObj);
        pIMem->pVTbl->Release(pIMem);
        pIBus->pVTbl->Release(pIBus);
        pISys->pVTbl->Release(pISys);
        return result;
    }

    /* Component initialization */
    result = pCObj->Init(pCObj, pISystem);

"""
    if aggr:
        new += """    /* Getting a pointer to the interface through the NON-delegating IEcoUnknown (required for aggregation) */
    pIUnk = (IEcoUnknown*)&pCObj->m_pVTblINondelegatingUnk;
    result = pIUnk->pVTbl->QueryInterface(pIUnk, riid, ppv);

    /* Decrementing the reference requested by the Component Factory */
    pIUnk->pVTbl->Release(pIUnk);

"""
    else:
        new += """    /* Getting a pointer to the interface */
    pIUnk = (IEcoUnknown*)pCObj;
    result = pIUnk->pVTbl->QueryInterface(pIUnk, riid, ppv);

    /* Decrementing the reference requested by the Component Factory */
    pIUnk->pVTbl->Release(pIUnk);

"""
    t = t[:a] + new + t[b:]
    write(path, t)


for name, (ifaces, aggr, policy, divzero, display) in SPEC.items():
    pdir, cid, sfx = project_info(name)
    write(os.path.join(pdir, "HeaderFiles", "CEco%s.h" % name), gen_header(name, sfx, ifaces, aggr))
    write(os.path.join(pdir, "SourceFiles", "CEco%s.c" % name), gen_source(name, sfx, ifaces, aggr, policy, divzero, display))
    patch_factory(pdir, name, sfx, aggr)
    idp = os.path.join(pdir, "SharedFiles", "IdEco%s.h" % name)
    idt = read(idp)
    idt = idt.replace('#include "IEco%s.h"' % name, "".join('#include "IEcoCalculator%s.h"\n' % i for i in ifaces).rstrip("\n"))
    write(idp, idt)
    print("generated Eco.%s  (CID suffix %s, interfaces %s, aggregation %s)" % (name, sfx, "+".join(ifaces), aggr))
