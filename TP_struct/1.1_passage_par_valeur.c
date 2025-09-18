#include <stdlib.h>
#include <stdio.h>

// Fraction(frac) methods

typedef struct frac {
  int num;
  int denom;
} fraction;

void frac_print(fraction frac) {
  printf("(%d / %d)\n", frac.num, frac.denom);
}

fraction frac_sum(fraction q1, fraction q2) {
  fraction result;
  result.num = q1.denom*q2.num + q2.denom*q1.num;
  result.denom = q1.denom * q2.denom;
  return result;
}

void frac_opposite(fraction* frac) {
  frac->num *= -1;
  frac->denom *= -1;
}

fraction* frac_inverse(fraction* frac) {
  fraction* result = malloc(sizeof(fraction));
  result->num = frac->denom;
  result->denom = frac->num;
  return result;
}

// Complex(co) numbers methods

typedef struct complex {
  double real;
  double im;
} Complex;

Complex* init_complex(double real, double im) {
  Complex* nb = malloc(sizeof(Complex));
  nb->real = real;
  nb->im = im;
  return nb;
}

void destroy_complex(Complex* nb) {
  free(nb);
}

void co_print(Complex* nb) {
  printf("%f + i(%f)\n", nb->real, nb->im);
}

int main() {
  fraction q1 = { .num=5, .denom=8 };
  fraction q2 = { .num=5, .denom=7 };

  fraction sum = frac_sum(q1, q2);
  
  printf("(%d / %d) + (%d / %d) = ", q1.num, q1.denom, q2.num, q2.denom);
  frac_print(sum);

  frac_opposite(&sum);

  frac_print(sum);

  fraction* inv_sum = frac_inverse(&sum);
  
  frac_print(*inv_sum);

  Complex* complex_nb = init_complex(21, 3.21);
  co_print(complex_nb);
  
  destroy_complex(complex_nb);
  free(inv_sum);
  return 0;
}
