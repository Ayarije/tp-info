#ifndef PNG_LIB_H
#define PNG_LIB_H

struct image_s {
  int h;
  int w;
  int** pixels;
};
typedef struct image_s image_t;

image_t* new_image(int h, int w);
void free_image(image_t* image);

image_t* image_read(char *filename);
void image_save(image_t *im, char *filename);

void horizontal_flip(image_t* im);
void vertical_flip(image_t* im);
void inverse_image(image_t* im);

image_t* subsampling(image_t* im, double factor);

#endif
