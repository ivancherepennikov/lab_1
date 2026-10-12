#!/bin/sh
# Overlay our modules on a scratch copy of the course prompts and build P1 (the course files are not modified).
set -e
HERE=$(cd "$(dirname "$0")" && pwd)
W=${TMPDIR:-/tmp}/eco_promts_build
rm -rf "$W"; mkdir -p "$W"; cp -R "${ECO_PROMTS:-$HOME/Dev/Eco.AI.Promts}" "$W/promts"
cp "$HERE/ours/Stack/ru/eco-prng.md" "$W/promts/Stack/ru/eco-prng.md"
cp "$HERE/ours/Agents/eco_prng_coder.md" "$W/promts/Agents/eco_prng_coder.md"
cd "$W/promts" && uv run --quiet --with python-frontmatter python builder.py eco_prng_coder.md --lang ru
cp Builds/build_eco_prng_coder_ru_*.md "$HERE/${1:-P1}_ours.md"
echo "written $HERE/${1:-P1}_ours.md"
