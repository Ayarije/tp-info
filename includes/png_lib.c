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
#include "matrices.h"

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
  free(image->pixels);
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

    result[y] = im->pixels[flip_y];
  }
  
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

  // Simplification de la fraction
  int d = pgcd(num, denom);
  while (d != 1) {
    num = num / d;
    denom = denom / d;
    d = pgcd(num, denom);
  }

  return num;
}

image_t* reduce_sub(image_t* im, double factor) {
  int old_im_p_size = get_denom(factor);
  int new_im_p_size = get_num(factor);

  int new_h = im->h*factor;
  int new_w = im->w*factor;

  image_t* result = new_image(new_h, new_w);

  int offset_y = 0;
  int offset_x = 0;

  for (int y = 0; y < im->h; y++) {
    if (y%old_im_p_size < old_im_p_size - new_im_p_size) {
      offset_y++;
      continue;
    }

    for (int x = 0; x < im->w; x++) {
      if (x%old_im_p_size < old_im_p_size - new_im_p_size) {
        offset_x++;
        continue;
      }
      
      result->pixels[y - offset_y][x - offset_x] = im->pixels[y][x];

    }

    offset_x = 0;
  }

  return result;
}

image_t* augment_sub(image_t* im, double factor) {
  int old_im_p_size = get_denom(factor);
  int new_im_p_size = get_num(factor);

  int new_h = im->h*factor;
  int new_w = im->w*factor;

  image_t* result = new_image(new_h, new_w);

  int offset_y = 0;
  int offset_x = 0;

  int line_skip = 0;

  for (int y = 0; y < new_h; y++) {
    if (y - offset_y >= im->h) { break; }
    if (y%new_im_p_size >= old_im_p_size) {
      offset_y++;
      line_skip = 1;
    }

    for (int x = 0; x < new_w; x++) {
      if (line_skip) {
	result->pixels[y][x] = result->pixels[y - 1][x];// TODO: verifier le -2
	continue;
      }
      
      if (x%new_im_p_size >= old_im_p_size) {
        result->pixels[y][x] = im->pixels[y - offset_y][x - offset_x - 1];
        offset_x++;
        continue;
      }
      
      if (x - offset_x >= im->w) { break; }
      result->pixels[y][x] = im->pixels[y - offset_y][x - offset_x];
    }

    offset_x = 0;
    line_skip = 0;
  }

  return result;
}

image_t* subsampling(image_t* im, double factor) {
  if (factor <= 1) {
    return reduce_sub(im, factor);
  } else {
    return augment_sub(im, factor);
  }
}

image_t* image_gradient(image_t* im) {
  image_t* result = malloc(sizeof(image_t));
  result->h = im->h;
  result->w = im->w;

  result->pixels = malloc(sizeof(int*) * im->h);

  int** p = im->pixels;
  
  for (int y = 0; y < im->h; y++) {
    result->pixels[y] = malloc(sizeof(int) * im->w);

    int prev_y = y - 1;
    int next_y = y + 1;
    
    if (y == 0) {
      prev_y = y;
    } else if (y == im->h - 1) {
      next_y = y;
    }

    for (int x = 0; x < im->w; x++) {
      int prev_x = x - 1;
      int next_x = x + 1;
      
      if (x == 0) {
        prev_x = x;
      } else if (x == im->w - 1) {
        next_x = x;
      }
      int p1 = abs(p[y][prev_x] - p[y][next_x]);
      int p2 = abs(p[prev_y][x] - p[next_y][x]);
      
      result->pixels[y][x] = (
        abs(p[y][prev_x] - p[y][next_x]) +
        abs(p[prev_y][x] - p[next_y][x])
      )/2;
      
    }
  }

  return result;
}

image_t* atomic_horizontal_shrink(image_t* im) {
  image_t* gradient = image_gradient(im);
  image_t* reduced_im = malloc(sizeof(image_t));
  reduced_im->h = im->h;
  reduced_im->w = im->w - 1;

  reduced_im->pixels = malloc(sizeof(int*) * reduced_im->h);

  for (int y = 0; y < im->h; y++) {
    int min_grad_i = 0;
    int x_offset = 0;

    for (int x = 1; x < im->w; x++) {
      if (gradient->pixels[y][x] < gradient->pixels[y][min_grad_i]) { min_grad_i = x; }
    }

    reduced_im->pixels[y] = malloc(sizeof(int) * reduced_im->w);

    for (int x = 0; x < reduced_im->w; x++) {
      if (x == min_grad_i) { x_offset = 1; }
      reduced_im->pixels[y][x] = im->pixels[y][x + x_offset];
    }
  }
  free_image(gradient);
  return reduced_im;
}

image_t* horizontal_shrink(image_t* im, int nb_pixels_removed, image_t* (*shrink_func)(image_t*)) {
  image_t* result = shrink_func(im);
  image_t* temp;

  for (int i = 1; i < nb_pixels_removed; i++) {
    temp = shrink_func(result);
    free_image(result);
    result = temp;
  }

  return result;
}

image_t* atomic_horizontal_column_shrink(image_t* im) {
  image_t* gradient = image_gradient(im);
  image_t* reduced_im = malloc(sizeof(image_t));
  reduced_im->h = im->h;
  reduced_im->w = im->w - 1;

  reduced_im->pixels = malloc(sizeof(int*) * reduced_im->h);

  int min_col = 0;
  int min_sum = INT_MAX;

  for (int x = 0; x < gradient->w; x++) {
    int sum = 0;

    for (int y = 0; y < gradient->h; y++) {
      sum += gradient->pixels[y][x];
    }

    if (sum < min_sum) {
      min_col = x;
      min_sum = sum;
    }
  }

  int x_offset = 0;

  for (int y = 0; y < reduced_im->h; y++) {
    reduced_im->pixels[y] = malloc(sizeof(int) * reduced_im->w);

    for (int x = 0; x < reduced_im->w; x++) {
      if (x == min_col) {
        x_offset = 1;
      }
      
      reduced_im->pixels[y][x] = im->pixels[y][x + x_offset];
    }
  }

  free_image(gradient);
  return reduced_im;
}

path_t* greedy_best_path(image_t* im) {
  path_t* best_path = new_path();
  
  int min_x = 0;
  for (int x = 1; x < im->w; x++) {
    if (im->pixels[0][x] < im->pixels[0][min_x]) {
      min_x = x;
    }
  }

  add_xy_to_path(best_path, min_x, 0);

  int px = min_x;

  for (int y = 1; y < im->h; y++) {
    // Get all valid pixels coords and values (if not valid put INT_MAX)
    int pixels[3];
    int x_coords[3];

    for (int offset_x = -1; offset_x <= 1; offset_x++) {
      if (px + offset_x >= 0 && px + offset_x < im->w) {
        pixels[offset_x + 1] = im->pixels[y][px + offset_x];
        x_coords[offset_x + 1] = px + offset_x;
      } else {
        pixels[offset_x + 1] = INT_MAX;
        x_coords[offset_x + 1] = INT_MAX;
      }
    }

    // Find minimum value
    min_x = INT_MAX;
    int min_p = INT_MAX;

    for (int i = 0; i < 3; i++) {
      if (pixels[i] < min_p) {
        min_p = pixels[i];
        min_x = x_coords[i];
      }
    }

    // add to path the minimum value
    add_xy_to_path(best_path, min_x, y);
    px = min_x;
  }

  return best_path;
}

image_t* atomic_horizontal_greedy_shrink(image_t* im) {
  path_t* path = greedy_best_path(im);
  image_t* reduced_im = malloc(sizeof(image_t));

  reduced_im->h = im->h;
  reduced_im->w = im->w - 1;

  reduced_im->pixels = malloc(sizeof(int*) * reduced_im->h);

  for (int y = 0; y < reduced_im->h; y++) {
    int offset_x = 0;
    reduced_im->pixels[y] = malloc(sizeof(int) * reduced_im->w);

    for (int x = 0; x < reduced_im->w; x++) {
      if (x == path->x_coords[y]) {
        offset_x = 1;
      }
      reduced_im->pixels[y][x] = im->pixels[y][x + offset_x];
    }
  }

  free_path(path);
  return reduced_im;
}

path_t* dp_best_path(image_t* im) {
  image_t* gradient = image_gradient(im);

  matrix* cost_matrix = matrix_create(im->w, im->h, sizeof(int));
  matrix* predecessors = matrix_create(im->w, im->h, sizeof(int));

  const int empty_pred = -1;

  for (int y = 0; y < im->h; y++) {
    for (int x = 0; x < im->w; x++) {
      if (y == 0) {
        matrix_set(cost_matrix, x, y, &gradient->pixels[y][x]);
        matrix_set(predecessors, x, y, &empty_pred);
        continue;
      }

      int best_col = x;
      int best_value = 256 * (y+1);

      for (int offset_x = -1; offset_x <= 1; offset_x++) {
        if (offset_x + x < 0 || offset_x + x >= im->w) { continue; }

        int mat_value = *(int*)matrix_get(cost_matrix, x + offset_x, y - 1);

        if (mat_value < best_value) {
          best_value = mat_value;
          best_col = x + offset_x;
        }
      }

      int new_cost = best_value + gradient->pixels[y][x];
      matrix_set(cost_matrix, x, y, &new_cost);
      matrix_set(predecessors, x, y, &best_col);
    }
  }

  path_t* best_path = new_path();
  int curr_x = 0;
  const int last_y = matrix_get_height(cost_matrix) - 1;

  for (int x = 1; x < im->w; x++) {
    int min_mat_value = *(int*)matrix_get(cost_matrix, curr_x, last_y);
    int mat_value = *(int*)matrix_get(cost_matrix, x, last_y);

    if (mat_value < min_mat_value) {
      curr_x = x;
    }
  }

  for (int y = im->h - 1; y >= 0; y--) {
    add_xy_to_path(best_path, curr_x, y);
    
    curr_x = *(int*)matrix_get(predecessors, curr_x, y);
  }
  
  matrix_destroy(predecessors);
  matrix_destroy(cost_matrix);
  free_image(gradient);

  return best_path;
}

image_t* atomic_horizontal_dp_shrink(image_t* im) {
  path_t* path = dp_best_path(im);
  image_t* reduced_im = malloc(sizeof(image_t));

  reduced_im->h = im->h;
  reduced_im->w = im->w - 1;

  reduced_im->pixels = malloc(sizeof(int*) * reduced_im->h);

  for (int y = 0; y < reduced_im->h; y++) {
    int offset_x = 0;
    reduced_im->pixels[y] = malloc(sizeof(int) * reduced_im->w);

    for (int x = 0; x < reduced_im->w; x++) {
      if (x == path->x_coords[y]) {
        offset_x = 1;
      }
      reduced_im->pixels[y][x] = im->pixels[y][x + offset_x];
    }
  }

  free_path(path);
  return reduced_im;
}

matrix* get_matrix_from_im(image_t* im) {
  matrix* result = matrix_create(im->w, im->h, sizeof(int));

  for (int y = 0; y < im->h; y++) {
    for (int x = 0; x < im->w; x++) {
      matrix_set(result, x, y, &im->pixels[y][x]);
    }
  }

  return result;
}

image_t* new_image_from_matrix(matrix* mat) {
  image_t* result = new_image(matrix_get_height(mat), matrix_get_width(mat));

  for (int y = 0; y < result->h; y++) {
    for (int x = 0; x < result->w; x++) {
      result->pixels[y][x] = *(int*)matrix_get(mat, x, y);
    }
  }

  return result;
}

path_t* new_path() {
  path_t* result = malloc(sizeof(path_t));

  result->array_size = 50;
  result->x_coords = malloc(sizeof(int) * result->array_size);
  result->y_coords = malloc(sizeof(int) * result->array_size);
  result->size = 0;

  return result;
}

path_t* clone_path(path_t* path) {
  if (!path) { return NULL; }

  path_t* result = malloc(sizeof(path_t));

  result->array_size = path->array_size;
  result->x_coords = malloc(sizeof(int) * result->array_size);
  result->y_coords = malloc(sizeof(int) * result->array_size);
  result->size = path->size;

  for (int i = 0; i < path->size; i++) {
    result->x_coords[i] = path->x_coords[i];
    result->y_coords[i] = path->y_coords[i];
  }

  return result;
}

void free_path(path_t* path) {
  free(path->x_coords);
  free(path->y_coords);
  free(path);
}

void double_array_length(path_t* path) {
  int* r_x_coords = malloc(sizeof(int) * path->array_size * 2);
  int* r_y_coords = malloc(sizeof(int) * path->array_size * 2);
  for (int i = 0; i < path->array_size; i++) {
    r_x_coords[i] = path->x_coords[i];
    r_y_coords[i] = path->y_coords[i];
  }
  path->array_size *= 2;
  free(path->x_coords);
  free(path->y_coords);
  path->x_coords = r_x_coords;
  path->y_coords = r_y_coords;
}

void add_xy_to_path(path_t* path, int x, int y) {
  if (path->size == path->array_size) {
    double_array_length(path);
  }
  path->x_coords[path->size] = x;
  path->y_coords[path->size] = y;
  path->size++;
}

void print_path(path_t* path, image_t* im) {
  printf("====[ %p ]====\n", path);
  int sum = 0;
  for (int i = 0; i < path->size; i++) {
    printf("%d / %d\n", path->x_coords[i], path->y_coords[i]);
    sum += im->pixels[path->y_coords[i]][path->x_coords[i]];
  }
  
  printf("Total : %d\n", sum);
}

image_t* atomic_vertical_dp_shrink(image_t* im) {
  matrix* im_mat = get_matrix_from_im(im);
  matrix* rotated_mat = matrix_rotate_90_right(im_mat);

  image_t* rotated_im = new_image_from_matrix(rotated_mat);
  image_t* rotated_result = atomic_horizontal_dp_shrink(rotated_im);

  matrix* rotated_result_mat = get_matrix_from_im(rotated_result);
  matrix* result_mat = matrix_rotate_90_left(rotated_result_mat);

  image_t* result = new_image_from_matrix(result_mat);

  free_image(rotated_im);
  free_image(rotated_result);
  matrix_destroy(im_mat);
  matrix_destroy(rotated_mat);
  matrix_destroy(rotated_result_mat);
  matrix_destroy(result_mat);
  
  return result;
}

image_t* shrink(image_t* im, double factor) {
  if (factor >= 1) { 
    printf("Error: you can only shrink images (factor > 1)");
    return NULL;
  }

  int new_w = im->w*factor;
  int new_h = im->h*factor;
  
  image_t* result = atomic_vertical_dp_shrink(im);
  image_t* temp;

  int nb_pixels_to_remove_width = im->w - new_w;
  int nb_pixels_to_remove_height = im->w - new_w;

  for (int i = 0; i < nb_pixels_to_remove_height + nb_pixels_to_remove_width; i++) {
    if (i/2 > nb_pixels_to_remove_height) {
      temp = result;
      result = atomic_horizontal_dp_shrink(result);
      free_image(temp);
    } else if (i/2 > nb_pixels_to_remove_width) {
      temp = result;
      result = atomic_vertical_dp_shrink(result);
      free_image(temp);
    } else if (i%2 == 1) {
      temp = result;
      result = atomic_vertical_dp_shrink(result);
      free_image(temp);
    } else {
      temp = result;
      result = atomic_horizontal_dp_shrink(result);
      free_image(temp);
    }
  }

  return result;
}