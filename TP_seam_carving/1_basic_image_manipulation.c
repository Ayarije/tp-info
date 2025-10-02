#include <png_lib.h>


int main() {
  image_t* img = new_image(100, 100);

  image_save(img, "TP_seam_carving/black.png");

  free_image(img);
  return 0;
}
