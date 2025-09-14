#include <stdlib.h>
#include <stdio.h>

#include "vector.h"


Vector* random_array(int array_size, int n, int init_value) {
    Vector* array = InitVector();
    v_append(array, init_value%n);

    for (int i = 1; i < array_size; i++) {
        v_append(array, (2 * v_get(array, i-1) + 1)%n);
    }

    return array;
}


int main() {

    Vector* rng_array = random_array(8, 10, 5103);

    v_print(rng_array);

    DestroyVector(rng_array);
    return 0;
}