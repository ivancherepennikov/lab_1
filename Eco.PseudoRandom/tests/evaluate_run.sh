#!/bin/bash
# usage: evaluate_run.sh <run dir> <algo> <project folder name> <pristine template dir>
T=$(cd "$(dirname "$0")" && pwd)
DEV="${ECO_DEV:-$(cd "$T/../.." && pwd)}"
R="$1/$3"
python3 "$T/install_tests.py" "$2" "$R" > /dev/null
python3 "$T/check_static.py" "$R" "$4" | tail -2
"$T/run_unittest.sh" "$R" 2>&1 | tail -3
# Link-time check: global symbols must not clash with the other (already accepted) components.
LIB=$(ls "$R"/BuildFiles/Mac/arm64/StaticDebug/lib*.a 2>/dev/null | head -1)
if [ -n "$LIB" ]; then
  SELF=$(basename "$LIB")
  nm -g --defined-only "$LIB" 2>/dev/null | awk 'NF==3{print $3}' | sort -u > "${TMPDIR:-/tmp}/self_syms.txt"
  CLASH=""
  for other in Eco.FibonacciLFSR1 Eco.GaloisLFSR1 Eco.Xorshift1; do
    OL=$(ls "$DEV/$other"/BuildFiles/Mac/arm64/StaticDebug/lib*.a 2>/dev/null | head -1)
    [ -z "$OL" ] && continue
    [ "$(basename "$OL")" = "$SELF" ] && continue
    nm -g --defined-only "$OL" 2>/dev/null | awk 'NF==3{print $3}' | sort -u > "${TMPDIR:-/tmp}/other_syms.txt"
    CLASH="$CLASH $(comm -12 "${TMPDIR:-/tmp}/self_syms.txt" "${TMPDIR:-/tmp}/other_syms.txt" | tr '\n' ' ')"
  done
  if [ -n "$(echo $CLASH)" ]; then echo "SYMBOL CLASH with other components: $CLASH"; else echo "link check: global symbols are unique"; fi
fi
