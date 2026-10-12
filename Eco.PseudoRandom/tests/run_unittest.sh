#!/bin/sh
# Build the component library and its unit test with xcodebuild and run it.
# usage: run_unittest.sh <project dir>   -> prints BUILD result and the test exit code
export ECO_FRAMEWORK="${ECO_FRAMEWORK:-$HOME/Dev/Eco.Framework}"
PROJ=$(cd "$1" && pwd)
X="$PROJ/AssemblyFiles/Mac/Xcode_v123"
LIB=$(ls "$X" | grep -v UnitTest | grep '\.xcodeproj$' | head -1)
UT=$(ls "$X" | grep UnitTest | grep '\.xcodeproj$' | head -1)
DD="$PROJ/.dd"
cd "$X" || exit 2
xcodebuild -project "$LIB" -scheme Static -configuration Debug -derivedDataPath "$DD/lib" build > "$PROJ/.build_lib.log" 2>&1
if ! grep -q "BUILD SUCCEEDED" "$PROJ/.build_lib.log"; then echo "LIB BUILD FAILED"; grep -E "error:" "$PROJ/.build_lib.log" | head -15; exit 3; fi
xcodebuild -project "$UT" -scheme Static -configuration Debug -derivedDataPath "$DD/ut" build > "$PROJ/.build_ut.log" 2>&1
if ! grep -q "BUILD SUCCEEDED" "$PROJ/.build_ut.log"; then echo "UNITTEST BUILD FAILED"; grep -E "error:|undefined|Undefined" "$PROJ/.build_ut.log" | head -15; exit 4; fi
echo "BUILD OK (warnings: lib=$(grep -c 'warning:' "$PROJ/.build_lib.log") ut=$(grep -c 'warning:' "$PROJ/.build_ut.log"))"
"$DD/ut/Build/Products/Debug/Static"
echo "TEST EXIT CODE: $?"
