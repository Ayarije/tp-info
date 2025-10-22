#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>

typedef struct {
    void *array;       // raw buffer
    size_t elem_size;  // bytes per element
    int mem_space;     // allocated slots
    int length;        // used slots
} array_t;

array_t* InitEmptyArray(size_t elem_size);
array_t* InitZerosArray(size_t elem_size, int length);
void DestroyArray(array_t* array);
void a_set(array_t* a, int index, const void* value);
void a_get(array_t* a, int index, void* out_value);
void a_push(array_t* a, const void* value);
void a_free(array_t* a);

#endif