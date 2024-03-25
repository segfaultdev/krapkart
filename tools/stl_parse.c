#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

void process_file(const char *path) {
  FILE *file = fopen(path, "rb");
  
  if (!file) {
    exit(1);
  }
  
  uint32_t count = 0;
  
  fseek(file, 80, SEEK_SET);
  fread(&count, 4, 1, file);
  
  char name[256];
  strcpy(name, path);
  
  for (int i = 0; name[i]; i++) {
    if (isalnum(name[i])) {
      name[i] = tolower(name[i]);
    } else {
      name[i] = '_';
    }
  }
  
  printf("const face_t g_%s[] = {\n", name);
  
  for (uint32_t i = 0; i < count; i++) {
    printf("  new face_t(255.0, 255.0, 255.0, ");
    
    for (int j = 0; j < 4; j++) {
      if (j) {
        printf(", ");
      }
      
      float values[3];
      fread(values, 4, 3, file);
      
      printf("new vector_t(%.2f, %.2f, %.2f)", values[0], values[2], values[1]);
    }
    
    fseek(file, 2, SEEK_CUR);
    printf("),\n");
  }
  
  printf("};\n\n");
  fclose(file);
}

int main(int argc, const char **argv) {
  printf("#ifndef __G_MODELS_IJ__\n");
  printf("#define __G_MODELS_IJ__\n\n");
  printf("// === g_models.ij ===\n\n");
  
  printf("#include <vector.ij>\n");
  printf("#include <face.ij>\n\n");
  
  for (int i = 1; i < argc; i++) {
    const char *path = argv[i];
    
    while (*path == '.' || *path == '/') {
      path++;
    }
    
    process_file(path);
  }
  
  printf("// ^^^ g_models.ij ^^^\n\n");
  printf("#endif\n");
  return 0;
}
