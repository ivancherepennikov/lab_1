"""Install the contract tests into a component project: install_tests.py <algo> <project dir>"""
import os, re, shutil, subprocess, sys

HERE = os.path.dirname(os.path.abspath(__file__))
algo, proj = sys.argv[1], os.path.abspath(sys.argv[2])
name = [f[len("IdEco"):-2] for f in os.listdir(os.path.join(proj, "SharedFiles")) if f.startswith("IdEco")][0]
idh = open(os.path.join(proj, "SharedFiles", "IdEco%s.h" % name), encoding="utf-8-sig").read()
cid_hex = re.search(r"GetIEcoComponentFactoryPtr_([0-9A-F]{32})", idh).group(1)

hdr_dir = os.path.join(proj, "UnitTestFiles", "HeaderFiles")
os.makedirs(hdr_dir, exist_ok=True)
shutil.copy(os.path.join(HERE, "PseudoRandomTests.h"), hdr_dir)
vec = subprocess.check_output([sys.executable, os.path.join(HERE, "..", "reference", "gen_vectors.py"), algo],
                              cwd=os.path.join(HERE, "..", "reference"), text=True)
open(os.path.join(hdr_dir, "PRNGVectors.h"), "w", encoding="utf-8").write(vec)

main = open(os.path.join(HERE, "EcoMain.c.in"), encoding="utf-8").read()
main = main.replace("@NAME@", name).replace("@CIDHEX@", cid_hex)
open(os.path.join(proj, "UnitTestFiles", "SourceFiles", "Eco%s.c" % name), "w", encoding="utf-8-sig").write(main)
print("installed tests for Eco%s (CID %s)" % (name, cid_hex))
