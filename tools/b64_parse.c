#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

const char *b64_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void process_file(const char *path) {
  FILE *file = fopen(path, "rb");
  
  if (!file) {
    exit(1);
  }
  
  char name[256];
  strcpy(name, path);
  
  for (int i = 0; name[i]; i++) {
    if (isalnum(name[i])) {
      name[i] = tolower(name[i]);
    } else {
      name[i] = '_';
    }
  }
  
  printf("const String g_%s = \"", name);
  
  uint8_t mask = 0;
  int count = 0;
  
  int c;
  
  while ((c = fgetc(file)) != EOF) {
    for (int i = 7; i >= 0; i--) {
      mask = (mask << 1) | ((c >> i) & 1);
      count++;
      
      if (count == 6) {
        putchar(b64_table[mask]);
        
        mask = 0;
        count = 0;
      }
    }
  }
  
  if (count == 2) {
    printf("%c==", b64_table[mask << 4]);
  } else if (count == 4) {
    printf("%c=", b64_table[mask << 2]);
  }
  
  printf("\";\n\n");
  fclose(file);
}

int main(int argc, const char **argv) {
  printf("#ifndef __G_BASE_64_IJ__\n");
  printf("#define __G_BASE_64_IJ__\n\n");
  printf("// === g_base_64.ij ===\n\n");
  
  for (int i = 1; i < argc; i++) {
    const char *path = argv[i];
    
    while (*path == '.' || *path == '/') {
      path++;
    }
    
    process_file(path);
  }
  
  printf("// ^^^ g_base_64.ij ^^^\n\n");
  printf("#endif\n");
  return 0;
}
