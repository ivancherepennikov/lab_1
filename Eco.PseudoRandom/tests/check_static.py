"""Static rules check of lines ADDED relative to the pristine template.
usage: check_static.py <project dir> <pristine template dir>"""
import difflib, os, re, sys

proj, base = sys.argv[1], sys.argv[2]
SKIP = ("IEcoPseudoRandom1.h", "IEcoPseudoRandomService1.h")
RULES = [
    ("libc include", re.compile(r"#\s*include\s*<")),
    ("libc/forbidden call", re.compile(r"\b(malloc|calloc|realloc|free|memcpy|memset|memmove|memcmp|strlen|strcpy|strcmp|printf|sprintf|puts|assert|rand|srand|abort|exit)\s*\(")),
    ("C99 // comment", re.compile(r"(?<![:\"'])//")),
    ("std C type", re.compile(r"\b(int|char|short|long|unsigned|signed|float|double)\b(?!\s*\*?\s*\()")),
    ("bare void*", re.compile(r"\bvoid\s*\*")),
    ("stdint 64 literal LL", re.compile(r"\b0x[0-9A-Fa-f]+(ULL|LL)\b")),
]
bad = []
for sub in ("SourceFiles", "HeaderFiles", "SharedFiles"):
    d = os.path.join(proj, sub)
    for f in sorted(os.listdir(d)):
        if f in SKIP or not f.endswith((".c", ".h")):
            continue
        new = open(os.path.join(d, f), encoding="utf-8-sig").read().splitlines()
        bp = os.path.join(base, sub, f)
        old = open(bp, encoding="utf-8-sig").read().splitlines() if os.path.exists(bp) else []
        for line in difflib.unified_diff(old, new, lineterm="", n=0):
            if not line.startswith("+") or line.startswith("+++"):
                continue
            text = re.sub(r'"(\\.|[^"\\])*"', '""', line[1:])
            text = re.sub(r"/\*.*?\*/", "", text)
            if text.strip().startswith("*") or not text.strip():
                continue
            for rule, rx in RULES:
                if rx.search(text):
                    bad.append("%s/%s: [%s] %s" % (sub, f, rule, line[1:].strip()))
print("\n".join(bad) if bad else "static rules: no violations in added lines")
print("violations: %d" % len(bad))
