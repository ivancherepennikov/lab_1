"""Runs the unit tests of the assignment 2 projects and stores their results for the report (data_lab2.json)."""
import json, os, re, subprocess, sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.environ.get("ECO_DEV") or os.path.abspath(os.path.join(HERE, "..", ".."))
TESTS = os.path.join(ROOT, "Eco.PseudoRandom", "tests", "run_unittest.sh")
out = {"tests": {}}
for name in ("CalcA", "CalcB", "CalcC", "CalcD", "CalcE", "Lab1Outer", "Lab2"):
    r = subprocess.run([TESTS, os.path.join(ROOT, "Eco." + name)], capture_output=True, text=True, timeout=900)
    text = r.stdout + r.stderr
    m = re.search(r"PASS: all (\d+) checks passed", text)
    out["tests"][name] = {"checks": int(m.group(1)) if m else None, "exit": 0 if "TEST EXIT CODE: 0" in text else 1}
    if name == "Lab2":
        demo = text.split("BUILD OK")[-1].split("\n", 1)[-1]
        out["demo_lab2"] = [l.rstrip() for l in demo.splitlines() if not l.startswith("PASS") and not l.startswith("TEST EXIT")]
    if name == "Lab1Outer":
        demo = text.split("BUILD OK")[-1].split("\n", 1)[-1]
        out["demo_lab1outer"] = [l.rstrip() for l in demo.splitlines() if not l.startswith("PASS") and not l.startswith("TEST EXIT")]
json.dump(out, open(os.path.join(HERE, "data_lab2.json"), "w"), indent=1, ensure_ascii=False)
print({k: v for k, v in out["tests"].items()})
