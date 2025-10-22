#include "array.h"
#include <stdlib.h>
#include <string.h>

array_t* InitEmptyArray(size_t elem_size) {
    array_t* array = malloc(sizeof(array_t));
    if (!array) return NULL;
    array->mem_space = 50;
    array->length = 0;
    array->elem_size = elem_size;
    array->array = malloc(elem_size * array->mem_space);
    return array;
}

array_t* InitZerosArray(size_t elem_size, int length) {
    array_t* array = malloc(sizeof(array_t));
    if (!array) return NULL;
    array->mem_space = length * 2;
    array->length = length;
    array->elem_size = elem_size;
    array->array = calloc(array->mem_space, elem_size);
    return array;
}

void DestroyArray(array_t* array) {
    free(array->array);
    free(array);
}

void a_set(array_t* a, int index, const void* value) {
    if (!a || index < 0 || index >= a->length) return;
    memcpy((char*)a->array + index * a->elem_size, value, a->elem_size);
}

void a_get(array_t* a, int index, void* out_value) {
    if (!out_value) return;
    if (!a || index < 0 || index >= a->length) {
        /* on error, zero the destination so callers don't keep previous/garbage value */
        size_t n = a ? a->elem_size : sizeof(void*);
        memset(out_value, 0, n);
        return;
    }
    memcpy(out_value, (char*)a->array + index * a->elem_size, a->elem_size);
}

void a_push(array_t* a, const void* value) {
    if (!a) return;
    if (a->length >= a->mem_space) {
        int new_space = a->mem_space * 2;
        void* new_buf = realloc(a->array, new_space * a->elem_size);
        if (!new_buf) return;
        a->array = new_buf;
        a->mem_space = new_space;
    }
    memcpy((char*)a->array + a->length * a->elem_size, value, a->elem_size);
    a->length++;
}

void a_free(array_t* a) {
    if (!a) return;
    free(a->array);
    free(a);
}