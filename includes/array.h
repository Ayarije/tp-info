#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>

typedef struct {
    void *array;             // raw buffer
    size_t elem_size;        // bytes per element
    int mem_space;           // allocated slots
    int length;              // used slots
    void (*free_elem)(void*); // free function
} array_t;

array_t* InitEmptyArray(size_t elem_size);
array_t* InitZerosArray(size_t elem_size, int length);
array_t* CopyArray(array_t* array);
void DestroyArray(array_t* array);
// set value at index in array
void a_set(array_t* a, int index, const void* value);
// get at index and export to ptr value. return 1 if success else 0
int a_get(array_t* a, int index, void* out_value);
void a_push(array_t* a, const void* value); // push value to the end of array
void a_free(array_t* a); // Free array
// Remove the element at index in the array
void a_remove(array_t* a, int index);
// return the index of the first occurence of value in array if it exists else -1
int a_search(array_t* a, const void* value);

#endif