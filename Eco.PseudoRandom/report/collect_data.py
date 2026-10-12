"""Collects the numbers for the report: component output vs reference model, statistics, object sizes."""
import json, os, re, subprocess, sys
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "reference"))
from reference import sequence

HOME = os.path.expanduser("~")
DEV = os.environ.get("ECO_DEV") or os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
CLIENT = DEV + "/Eco.Lab1/.dd_client/Build/Products/Release/Static"
ALGOS = [(1, "fibonacci", "Fibonacci LFSR"), (2, "galois", "Galois LFSR"), (3, "xorshift", "Xorshift")]
CASES = [  # (bits, seed, mask) mask only for LFSR
    (32, 0x1, 0x0), (32, 0xACE1, 0xB400), (32, 0xDEADBEEF, 0x0),
    (64, 0x1, 0x0), (64, 0xDEADBEEFCAFEBABE, 0x0),
]
COUNT = 5


def run_client(algo_no, mode, bits, seed, mask, count):
    stdin = "%d\n%d\n%d\n%X\n%X\n%d\n0\n" % (algo_no, mode, bits, seed, mask, count)
    out = subprocess.run([CLIENT], input=stdin, capture_output=True, text=True, timeout=60).stdout
    found = re.findall(r"(?:SOA|OOA)\s{2}((?:[0-9A-F]{8,16} ?)+)$", out, re.M)
    return [int(v, 16) for v in found[-1].split()]


rows = []
for no, key, title in ALGOS:
    for bits, seed, mask in CASES:
        if key == "xorshift":
            if mask:
                continue
            if bits == 32 and seed == 0x1:
                pass
        ref = sequence(key, seed, mask, bits, COUNT)
        soa = run_client(no, 1, bits, seed, mask, COUNT)
        ooa = run_client(no, 2, bits, seed, mask, COUNT)
        w = bits // 4
        fmt = lambda v: format(v, "0%dX" % w)
        rows.append(dict(algo=title, bits=bits, seed=format(seed, "X"), mask=format(mask, "X"),
                         ref=[fmt(v) for v in ref], soa=[fmt(v) for v in soa], ooa=[fmt(v) for v in ooa],
                         soa_ok=(soa == ref), ooa_ok=(ooa == ref), same=(soa == ooa)))

# Marsaglia published first values of xorshift32 for seed 1
out = subprocess.run([CLIENT], input="9\n0\n", capture_output=True, text=True, timeout=120).stdout
stats = []
for l in out.splitlines():
    m = re.match(r"(.+?)\s+(32|64) bit: ones ([\d.]+)\s+low bit ([\d.]+)\s+chi2\(bytes\)\s+([\d.]+)\s+OOA ([\d.]+)\s+SOA ([\d.]+) Mvalues/s", l)
    if m:
        stats.append(dict(algo=m.group(1).strip(), bits=int(m.group(2)), ones=float(m.group(3)), low=float(m.group(4)),
                          chi2=float(m.group(5)), ooa=float(m.group(6)), soa=float(m.group(7))))

# object sizes
sizes = {}
fw = os.environ.get("ECO_FRAMEWORK") or HOME + "/Dev/Eco.Framework"
def cls(proj):
    short = proj.split(".")[1]
    idh = open("%s/%s/SharedFiles/IdEco%s.h" % (DEV, proj, short), encoding="utf-8-sig").read()
    cid = re.search(r"GetIEcoComponentFactoryPtr_([0-9A-F]{32});", idh).group(1)
    return "CEco%s_%s" % (short, cid[-8:])


for proj, name in ((p_, cls(p_)) for p_ in ("Eco.FibonacciLFSR1", "Eco.GaloisLFSR1", "Eco.Xorshift1")):
    hdr = "CEco%s.h" % proj.split(".")[1]
    src = '#include <stdio.h>\n#include "%s"\nint main(void){printf("%%lu\\n",(unsigned long)sizeof(%s));return 0;}\n' % (hdr, name)
    tmp = os.environ.get("TMPDIR", "/tmp") + "/sz.c"
    open(tmp, "w").write(src)
    cmd = ["clang", "-DECO_LIB", "-DECO_APPLE", "-DECO_MAC", "-DUGUID_UTILITY", "-DECO_AARCH64", "-w", "-o", tmp[:-2],
           "-I%s/%s/HeaderFiles" % (DEV, proj), "-I%s/%s/SharedFiles" % (DEV, proj)]
    for d in ("Eco.Core1", "Eco.System1", "Eco.MemoryManager1", "Eco.InterfaceBus1", "Eco.FileSystemManagement1"):
        cmd.append("-I%s/%s/SharedFiles" % (fw, d))
    cmd.append(tmp)
    subprocess.check_call(cmd)
    sizes[name] = int(subprocess.check_output([tmp[:-2]], text=True).strip())

json.dump(dict(rows=rows, stats=stats, sizes=sizes), open("data.json", "w"), indent=1)
print(json.dumps(sizes))
print("rows:", len(rows), " stats:", len(stats))
bad = [r for r in rows if not (r["soa_ok"] and r["ooa_ok"] and r["same"])]
print("mismatches vs reference:", len(bad))
