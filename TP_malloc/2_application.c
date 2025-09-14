#include <stdlib.h>
#include <stdio.h>

#include "vector.h"


Vector* random_array(int array_size, int n, int init_value) {
    Vector* array = InitVector();
    v_append(array, init_value%n);

    for (int i = 1; i < array_size; i++) {
        v_append(array, (2 * *v_get(array, i-1) + 1)%n);
    }

    return array;
}


int biggest_sum(Vector* array, int k) {
    int sum = 0;
    int max_sum = -32768;
    int max_i = 0;

    for (int i = 0; i < array->lenght - k; i++) {

        sum += *v_get(array, i);

        if (i < k - 1) { continue; }

        if (i != k - 1)    { sum -= *v_get(array, i - k); }
        if (sum > max_sum) {
            max_sum = sum;
            max_i = i - k + 1;
        }

    }

    return max_i;
}


Vector* count_occurence(Vector* array) {
    Vector* count_arr = InitVectorWithValue(array->lenght, 0);

    for (int i = 0; i < array->lenght; i++) {
        v_set(count_arr, i, v_count(array, *v_get(array, i)));
    }

    return count_arr;
}


int main() {
    Vector* rng_array = random_array(16, 100, 5414);

    Vector* simple_array = InitVector();
    v_append(simple_array, 5);
    v_append(simple_array, 4);
    v_append(simple_array, 7);
    v_append(simple_array, 2);
    v_append(simple_array, 3);

    int max_sum = biggest_sum(simple_array, 3);
    Vector* count = count_occurence(rng_array);

    v_print(rng_array);
    v_print(count);

    v_print(simple_array);
    printf("first index of max consecutive sum of 3 nb : %d\n", max_sum);

    DestroyVector(rng_array);
    DestroyVector(simple_array);
    DestroyVector(count);
    return 0;
}