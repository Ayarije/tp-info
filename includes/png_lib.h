#ifndef PNG_LIB_H
#define PNG_LIB_H

struct image_s {
  int h;
  int w;
  int** pixels;
};
typedef struct image_s image_t;

struct path_s {
  int size;
  int array_size;
  int* x_coords;
  int* y_coords;
};
typedef struct path_s path_t;

path_t* new_path();
void free_path(path_t* path);
path_t* clone_path(path_t* path);

void add_xy_to_path(path_t* path, int x, int y);

void print_path(path_t* path, image_t* im);

image_t* new_image(int h, int w);
void free_image(image_t* image);

image_t* image_read(char *filename);
void image_save(image_t *im, char *filename);

void horizontal_flip(image_t* im);
void vertical_flip(image_t* im);
void inverse_image(image_t* im);

image_t* subsampling(image_t* im, double factor);

image_t* image_gradient(image_t* im);
image_t* atomic_horizontal_shrink(image_t* im);
image_t* horizontal_shrink(image_t* im, int nb_pixels_removed, image_t* (*shrink_func)(image_t*));
image_t* atomic_horizontal_column_shrink(image_t* im);

path_t* greedy_best_path(image_t* im);
image_t* atomic_horizontal_greedy_shrink(image_t* im);

path_t* dp_best_path(image_t* im);

#endif
