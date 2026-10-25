#!/bin/sh
# Builds the components of assignment 2 in dependency order (Debug and Release libraries) and runs all their unit tests.
#
#   export ECO_FRAMEWORK=/path/to/Eco.Framework
#   tools/build_lab2.sh              # assignment 2 only (the prebuilt lab 1 libraries from BuildFiles are used as they are)
#   tools/build_lab2.sh --with-lab1  # additionally rebuilds the three lab 1 libraries first
#
# Order: calculators A-E -> Eco.Lab1Outer -> Eco.Lab2. The unit tests of the outer components link the libraries
# of the calculators and of the lab 1 generators, so those libraries must exist before the outer tests run.
ROOT=$(cd "$(dirname "$0")/.." && pwd)
if [ -z "$ECO_FRAMEWORK" ]; then echo "ECO_FRAMEWORK is not set (path to Eco.Framework)"; exit 2; fi
export ECO_FRAMEWORK

build_lib() { # project configuration
    X="$ROOT/$1/AssemblyFiles/Mac/Xcode_v123"
    L=$(ls "$X" | grep -v UnitTest | grep '\.xcodeproj$' | head -1)
    printf '  %-20s %-8s ' "$1" "$2"
    OUT=$(cd "$X" && xcodebuild -project "$L" -scheme Static -configuration "$2" build 2>&1)
    if echo "$OUT" | grep -q "BUILD SUCCEEDED"; then echo "BUILD SUCCEEDED"; else echo "BUILD FAILED"; echo "$OUT" | grep "error:" | head -5; FAIL=1; fi
}

FAIL=0
LAB1="Eco.FibonacciLFSR1 Eco.GaloisLFSR1 Eco.Xorshift1"
LAB2="Eco.CalcA Eco.CalcB Eco.CalcC Eco.CalcD Eco.CalcE Eco.Lab1Outer Eco.Lab2"

echo "== libraries"
if [ "$1" = "--with-lab1" ]; then for p in $LAB1; do for c in Debug Release; do build_lib "$p" "$c"; done; done; fi
for p in $LAB2; do for c in Debug Release; do build_lib "$p" "$c"; done; done

echo "== unit tests (each builds its own unit test project and runs it)"
for p in $LAB2; do
    printf '  %-20s ' "$p"
    OUT=$("$ROOT/Eco.PseudoRandom/tests/run_unittest.sh" "$ROOT/$p" 2>&1)
    LINE=$(echo "$OUT" | grep -E "^(PASS|FAIL)" | tail -1)
    if echo "$OUT" | grep -q "TEST EXIT CODE: 0"; then echo "$LINE" | sed 's/ (request.*//'; else echo "FAILED: ${LINE:-no result}"; FAIL=1; fi
done
if [ "$FAIL" = "0" ]; then echo "all assignment 2 components built and passed their tests"; else echo "SOMETHING FAILED"; fi
exit $FAIL
