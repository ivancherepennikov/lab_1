"""Connects sibling component projects to a wizard-created Xcode project.

usage: link_projects.py <repo root> <project name> <dependency project names...>

For the library project (EcoXxx.xcodeproj) only the header search paths of the dependencies are added.
For the unit test project (EcoXxxUnitTest.xcodeproj) the header paths, the library search paths and
the -l<CID> linker flags of the dependencies (and of the project's own library) are added.
"""
import os
import re
import sys

root = os.path.abspath(sys.argv[1])
proj = sys.argv[2]
deps = sys.argv[3:]
short = proj.split(".", 1)[1]


def cid_of(p):
    s = p.split(".", 1)[1]
    t = open(os.path.join(root, p, "SharedFiles", "IdEco%s.h" % s), encoding="utf-8-sig").read()
    return re.search(r"GetIEcoComponentFactoryPtr_([0-9A-F]{32});", t).group(1)


xdir = os.path.join(root, proj, "AssemblyFiles", "Mac", "Xcode_v123")
hdr = "".join("\t\t\t\t../../../../%s/SharedFiles,\n" % d for d in deps)

# --- library project: header paths only
lib = os.path.join(xdir, "Eco%s.xcodeproj" % short, "project.pbxproj")
t = open(lib, encoding="utf-8").read()
marker = "\t\t\t\t../../../SharedFiles,\n"
assert marker in t, lib
if "../../../../%s/SharedFiles," % deps[0] not in t:
    t = t.replace(marker, marker + hdr)
open(lib, "w", encoding="utf-8").write(t)

# --- unit test project: header paths, library paths and linker flags
ut = os.path.join(xdir, "Eco%sUnitTest.xcodeproj" % short, "project.pbxproj")
t = open(ut, encoding="utf-8").read()
assert marker in t, ut
if "../../../../%s/SharedFiles," % deps[0] not in t:
    t = t.replace(marker, marker + hdr)
    libp = "".join("\t\t\t\t\"'../../../../%s/BuildFiles/Mac/$(ARCHS)/$(TARGET_NAME)$(CONFIGURATION)'\",\n" % d for d in deps)
    t = t.replace("\t\t\t\tLIBRARY_SEARCH_PATHS = (\n", "\t\t\t\tLIBRARY_SEARCH_PATHS = (\n" + libp)
    flags = "OTHER_LDFLAGS = (\n" + "".join("\t\t\t\t\"-l%s\",\n" % cid_of(d) for d in deps) + "\t\t\t);"
    n = t.count('OTHER_LDFLAGS = "";')
    assert n >= 1, "OTHER_LDFLAGS placeholder not found"
    t = t.replace('OTHER_LDFLAGS = "";', flags)
open(ut, "w", encoding="utf-8").write(t)
print("linked %s -> %s" % (proj, ", ".join(deps)))
