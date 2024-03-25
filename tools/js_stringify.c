#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

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
  
  printf("const String g_%s = \"`", name);
  int c;
  
  while ((c = fgetc(file)) != EOF) {
    loop_start:
    
    if (c < ' ') {
      continue;
    }
    
    if (c == '/') {
      int d = fgetc(file);
      
      if (d == '/') {
        while (d != EOF && d != '\n') {
          d = fgetc(file);
        }
        
        continue;
      } else {
        putchar(c);
        c = d;
        
        goto loop_start;
      }
    }
    
    if (c == '\"') {
      printf("\\\"");
    } else if (c == '\\') {
      printf("\\\\\\\\");
    } else {
      putchar(c);
    }
  }
  
  printf("`\";\n\n");
  fclose(file);
}

int main(int argc, const char **argv) {
  printf("#ifndef __G_SCRIPTS_IJ__\n");
  printf("#define __G_SCRIPTS_IJ__\n\n");
  printf("// === g_scripts.ij ===\n\n");
  
  for (int i = 1; i < argc; i++) {
    const char *path = argv[i];
    
    while (*path == '.' || *path == '/') {
      path++;
    }
    
    process_file(path);
  }
  
  printf("// ^^^ g_scripts.ij ^^^\n\n");
  printf("#endif\n");
  return 0;
}
