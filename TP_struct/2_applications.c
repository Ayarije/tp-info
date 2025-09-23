// Voir dans ../includes/vector.h
#include <stdlib.h>
#include <stdio.h>

#include "vector.h"

int main() {
  Vector* a1 = InitVectorWithValue(5, 0);
  Vector* a2 = InitVectorWithValue(5, 0);
  
  v_print(a1);
  v_set(a1, 1, 1);
  v_print(a1);

  printf("a1[3] = %d\n", *v_get(a1, 3));

  v_print(a2);
  v_set(a2, 4, 3);
  v_print(a2);

  printf("a2[4] = %d\n", *v_get(a2, 4));

  Vector* sum = v_sum_vectors(a1, a2);

  v_print(sum);
  
  DestroyVector(a1);
  DestroyVector(a2);
  DestroyVector(sum);
  return 0;
}
