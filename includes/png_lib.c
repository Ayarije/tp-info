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

void vertical_flip(image_t* im) {
  int* result;
  for (int y = 0; y < im->h; y++) {
    result = malloc(sizeof(int) * im->w);
    for (int x = 0; x < im->w; x++) {
      int flip_x = x - im->w + 1;
      if (flip_x < 0) { flip_x = flip_x * -1; }
      result[x] = im->pixels[y][flip_x];
    }
    free(im->pixels[y]);
    im->pixels[y] = result;
  }
}

void horizontal_flip(image_t* im) {
  int** result = malloc(sizeof(int*) * im->h);

  for (int y = 0; y < im->h; y++) {
    int flip_y = y - im->h + 1;
    if (flip_y < 0) { flip_y = flip_y * -1; }

    result[y] = malloc(sizeof(int) * im->w);
    for (int x = 0; x < im->w; x++) { result[y][x] = im->pixels[flip_y][x]; }
  }
  
  for (int y = 0; y < im->h; y++) { free(im->pixels[y]); }
  free(im->pixels);
  im->pixels = result;
}

void inverse_image(image_t* im) {
  int inv_color;
  for (int y = 0; y < im->h; y++) {
    for (int x = 0; x < im->w; x++) {
      inv_color = im->pixels[y][x] - 255;
      if (inv_color < 0) { inv_color = inv_color * -1; }
      im->pixels[y][x] = inv_color;
    }
  }
}

double modulo(double a, int b) {
  int q = a/b;
  return a - q;
}

int pgcd(int a, int b) {
  while (b != 0) {
    int r = a % b;
    a = b;
    b = r;
  }
  return a;
}

int get_denom(double nb) { // Get the denominator of the most simplified fraction of a double
  int denom = 1000000;
  int num = nb * denom;
  printf("%f : %d/%d\n", nb, num, denom);

  // Simplification de la fraction
  int d = pgcd(num, denom);
  while (d != 1) {
    num = num / d;
    denom = denom / d;
    d = pgcd(num, denom);
  }

  return denom;
}

int get_num(double nb) { // Get the denominator of the most simplified fraction of a double
  int denom = 1000000;
  int num = nb * denom;
  printf("%f : %d/%d\n", nb, num, denom);

  // Simplification de la fraction
  int d = pgcd(num, denom);
  while (d != 1) {
    num = num / d;
    denom = denom / d;
    d = pgcd(num, denom);
  }

  return num;
}

void subsampling(image_t* im, double factor) {
  
  int old_im_p_size = get_denom(factor);

  printf("%d\n", old_im_p_size);

}
