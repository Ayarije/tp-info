#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Nécessaire pour utiliser le code de lecture/écriture du format png
#define STBI_NO_FAILURE_STRINGS
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stb_image.h"
#include "stb_image_write.h"

#include "png_lib.h"

image_t* new_image(int h, int w){
  image_t* img = malloc(sizeof(image_t));
  img->h = h;
  img->w = w;

  img->pixels = malloc(sizeof(int*) * h);
  for (int y = 0; y < h; y++) {
    img->pixels[y] = malloc(sizeof(int) * w);
    for (int x = 0; x < w; x++) {
      img->pixels[y][x] = 0;
    }
  }  
  return img;
}

void free_image(image_t* image){
  for (int y = 0; y < image->h; y++) {
    for (int x = 0; x < image->w; x++) {
      free(image->pixels[y][x]);
    }
    free(image->pixels[y]);
  }
  free(image);
}

image_t* image_read(char *filename){
    int w, h, channels;
    uint8_t *data = stbi_load(filename, &w, &h, &channels, 0);
    if (!data) {
        fprintf(stderr, "Erreur de lecture.\n");
        stbi_failure_reason();
        exit(EXIT_FAILURE);
    }
    if (channels != 1){
        fprintf(stdout, "Pas une image en niveaux de gris.\n");
        exit(EXIT_FAILURE);
    }
    image_t* result_image = new_image(h, w);
    for (int i = 0; i < h; i++){
        for (int j = 0; j < w; j++){
            result_image->pixels[i][j] = (int)data[j + i * w];
        }
    }
    free(data);
    return result_image;
}

void image_save(image_t* image, char *filename){
    int h = image->h;
    int w = image->w;
    int stride_length = w;
    uint8_t *data = malloc(w * h * sizeof(uint8_t));
    for (int i = 0; i < h; i++){
        for (int j = 0; j < w; j++){
            data[j + i * w] = image->pixels[i][j];
        }
    }
    if (!stbi_write_png(filename, w, h, 1, data, stride_length)){
        fprintf(stderr, "Erreur d'écriture.\n");
        free_image(image);
        free(data);
        exit(EXIT_FAILURE);
    }
    free(data);
}
