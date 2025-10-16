#include <png_lib.h>


int main() {
  image_t* img = new_image(100, 100);

  image_save(img, "TP_seam_carving/black.png");

  image_t* broadway = image_read("TP_seam_carving/broadway_seam.png");

  vertical_flip(broadway);
  image_save(broadway, "TP_seam_carving/broadway_seam_v_flipped.png");
  vertical_flip(broadway);

  horizontal_flip(broadway);
  image_save(broadway, "TP_seam_carving/broadway_seam_h_flipped.png");
  horizontal_flip(broadway);

  inverse_image(broadway);
  image_save(broadway, "TP_seam_carving/broadway_seam_inv.png");
  inverse_image(broadway);

  image_t* reduced_broadway = subsampling(broadway, 0.5);
  image_save(reduced_broadway, "TP_seam_carving/broadway_seam_subsampling.png");

  free_image(reduced_broadway);
  free_image(broadway);
  free_image(img);
  return 0;
}
