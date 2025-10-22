#include <png_lib.h>
#include <stdlib.h>

#include "string.h"

char** char_buffer;
int char_buffer_cursor;

char* build_path(char** paths, int nb_paths) {
  int len = 0;
  for (int i = 0; i < nb_paths; i++) { len += strlen(paths[i]); }
  char* path = malloc(sizeof(char) * (len + 1));
  
  int cursor = 0;

  for (int i = 0; i < nb_paths; i++) {
    for (int j = 0; j < strlen(paths[i]); j++) {
      path[cursor] = paths[i][j];
      cursor++;
    }
  }
  path[len] = '\0';

  char_buffer[char_buffer_cursor] = path;
  char_buffer_cursor++;
  printf("Done : %s\n", path);
  return path;
}

void test_all_functions_on(char* image_name, char* result_dir) {
  image_t* im = image_read(build_path((char*[]) {"TP_seam_carving/", image_name, ".png"}, 3));

  vertical_flip(im);
  image_save(
    im,
    build_path((char*[]) {"TP_seam_carving/", result_dir, image_name, "_v_flipped.png"}, 4)
  );
  vertical_flip(im);

  horizontal_flip(im);
  image_save(
    im,
    build_path((char*[]) {"TP_seam_carving/", result_dir, image_name, "_h_flipped.png"}, 4)
  );
  horizontal_flip(im);

  inverse_image(im);
  image_save(
    im,
    build_path((char*[]) {"TP_seam_carving/", result_dir, image_name, "_inv.png"}, 4)
  );
  inverse_image(im);

  image_t* reduced_im = subsampling(im, 0.01);
  image_save(
    reduced_im,
    build_path((char*[]) {"TP_seam_carving/", result_dir, image_name, "_r_subsampling.png"}, 4)
  );

  image_t* augmented_im = subsampling(im, 1.5);
  image_save(
    augmented_im,
    build_path((char*[]) {"TP_seam_carving/", result_dir, image_name, "_a_subsampling.png"}, 4)
  );

  image_t* gradient_im = image_gradient(im);
  image_save(
    gradient_im,
    build_path((char*[]) {"TP_seam_carving/", result_dir, image_name, "_gradient.png"}, 4)
  );

  image_t* naive_im = horizontal_shrink(im, 50, atomic_horizontal_shrink);
  image_save(
    naive_im,
    build_path((char*[]) {"TP_seam_carving/", result_dir, image_name, "_naive_reduction.png"}, 4)
  );

  image_t* column_im = horizontal_shrink(im, 50, atomic_horizontal_column_shrink);
  image_save(
    column_im,
    build_path((char*[]) {"TP_seam_carving/", result_dir, image_name, "_column_reduction.png"}, 4)
  );

  image_t* greedy_im = horizontal_shrink(im, 50, atomic_horizontal_greedy_shrink);
  image_save(
    greedy_im,
    build_path((char*[]) {"TP_seam_carving/", result_dir, image_name, "_greedy_reduction.png"}, 4)
  );

  free_image(greedy_im);
  free_image(column_im);
  free_image(naive_im);
  free_image(gradient_im);
  free_image(augmented_im);
  free_image(reduced_im);
  free_image(im);
}


int main() {
  char_buffer = malloc(sizeof(char*) * 1000);

  image_t* img = new_image(100, 100);
  image_save(img, "TP_seam_carving/black.png");

  /*
  test_all_functions_on("bird", "bird/");
  test_all_functions_on("broadway_seam", "broadway/");
  test_all_functions_on("boat", "boat/");
  */

  image_t* bird_low = image_read("TP_seam_carving/bird_low.png");
  image_t* grad_bird_low = image_gradient(bird_low);
  image_save(grad_bird_low, "TP_seam_carving/bird_low/gradient_bird_low.png");

  for (int y = 0; y < grad_bird_low->h; y++) {
    for (int x = 0; x < grad_bird_low->w; x++) {
      int p = grad_bird_low->pixels[y][x];
      printf("%d", p);
      if (p < 10) {
        printf("   ");
      } else if (p < 100) {
        printf("  ");
      } else {
        printf(" ");
      }
    }
    printf("\n");
  }

  path_t* best_dp_path = dp_best_path(grad_bird_low);
  print_path(best_dp_path, grad_bird_low);

  free_path(best_dp_path);
  free_image(bird_low);
  free_image(grad_bird_low);

  for (int i = 0; i < char_buffer_cursor; i++) {
    free(char_buffer[i]);
  }
  free(char_buffer);

  free_image(img);
  return 0;
}
