#!/bin/bash

set -e

cd tools
  gcc b64_parse.c -o b64_parse
  gcc js_stringify.c -o js_stringify
  gcc png_parse.c -lm -Iinclude -o png_parse
  gcc stl_parse.c -o stl_parse
cd ..

cd base_64
  ../tools/b64_parse $(find . -type f) > ../g_base_64.ij
cd ..

cd fonts
  ../tools/png_parse f $(find . -name "*.png") > ../g_fonts.ij
cd ..

cd images
  ../tools/png_parse i $(find . -name "*.png") > ../g_images.ij
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
