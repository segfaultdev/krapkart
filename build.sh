#!/bin/bash

set -e

cd tools
  gcc stl_parse.c -o stl_parse
  gcc js_stringify.c -o js_stringify
cd ..

cd models
  # ../tools/stl_parse $(find . -name "*.stl") > ../g_models.ij
  blender -b models.blend -P ../tools/blend_parse.py
cd ..

cd scripts
  ../tools/js_stringify $(find . -name "*.js") > ../g_scripts.ij
cd ..

cpp-13 -D bool=boolean -D const=final -I. -nostdinc -trigraphs -CC main.ij | grep -v "^#" | cat -s > krapkart.txt
rm g_*.ij
