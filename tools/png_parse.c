#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

void process_image(const char *path) {
  int width, height, bits_per_pixel;
  uint8_t *data = stbi_load(path, &width, &height, &bits_per_pixel, 3);
  
  if (!data) {
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
  
  printf("const int g_%s[] = {", name);
  
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (i > 0 || j > 0) {
        printf(", ");
      }
      
      printf("%d, %d, %d", data[j * 3 + i * width * 3 + 0], data[j * 3 + i * width * 3 + 1], data[j * 3 + i * width * 3 + 2]);
    }
  }
  
  printf("};\n\n");
}

void process_font(const char *path, int font_width, int font_height) {
  int width, height, bits_per_pixel;
  uint8_t *data = stbi_load(path, &width, &height, &bits_per_pixel, 1);
  
  if (!data) {
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
  
  printf("const int g_%s[] = {", name);
  
  for (int i = 0; i * font_height < height; i++) {
    for (int j = 0; j * font_width < width; j++) {
      for (int k = 0; k < font_height; k++) {
        uint8_t byte = 0;
        
        for (int l = 0; l < font_width; l++) {
          byte |= ((data[l + j * font_width + (k + i * font_height) * width] >> 7) << l);
        }
        
        if (i > 0 || j > 0 || k > 0) {
          printf(", ");
        }
        
        printf("%d", byte);
      }
    }
  }
  
  printf("};\n\n");
}

int main_images(int argc, const char **argv) {
  printf("#ifndef __G_IMAGES_IJ__\n");
  printf("#define __G_IMAGES_IJ__\n\n");
  printf("// === g_images.ij ===\n\n");
  
  for (int i = 1; i < argc; i++) {
    const char *path = argv[i];
    
    while (*path == '.' || *path == '/') {
      path++;
    }
    
    process_image(path);
  }
  
  printf("// ^^^ g_images.ij ^^^\n\n");
  printf("#endif\n");
  
  return 0;
}

int main_fonts(int argc, const char **argv) {
  printf("#ifndef __G_FONTS_IJ__\n");
  printf("#define __G_FONTS_IJ__\n\n");
  printf("// === g_fonts.ij ===\n\n");
  
  for (int i = 1; i < argc; i++) {
    const char *path = argv[i];
    
    while (*path == '.' || *path == '/') {
      path++;
    }
    
    process_font(path, 5, 5);
  }
  
  printf("// ^^^ g_fonts.ij ^^^\n\n");
  printf("#endif\n");
  
  return 0;
}

int main(int argc, const char **argv) {
  if (argv[1][0] == 'i') {
    return main_images(argc - 1, argv + 1);
  } else if (argv[1][0] == 'f') {
    return main_fonts(argc - 1, argv + 1);
  }
  
  return 1;
}
