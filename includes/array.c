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
    array->free_elem = NULL;
    return array;
}

array_t* InitZerosArray(size_t elem_size, int length) {
    array_t* array = malloc(sizeof(array_t));
    if (!array) return NULL;
    array->mem_space = length * 2;
    array->length = length;
    array->elem_size = elem_size;
    array->array = calloc(array->mem_space, elem_size);
    array->free_elem = NULL;
    return array;
}

void DestroyArray(array_t* a) {
    if (a == NULL) return;

    if (a->free_elem != NULL) {
        for (int i = 0; i < a->length; i++) {
            void* e_ptr = (char*)a->array + (i * a->elem_size);
            a->free_elem(e_ptr);
        }
    }

    free(a->array);
    free(a);
}

array_t* CopyArray(array_t* old_a) {
    array_t* new_a = malloc(sizeof(array_t));
    if (!new_a) return NULL;
    new_a->mem_space = old_a->mem_space;
    new_a->length = old_a->length;
    new_a->elem_size = old_a->elem_size;
    new_a->free_elem = old_a->free_elem;
    new_a->array = malloc(new_a->elem_size * new_a->mem_space);
    memcpy((char*) new_a->array, (char*) old_a->array, new_a->elem_size * new_a->length);
}

void a_set(array_t* a, int index, const void* value) {
    if (!a || index < 0 || index >= a->length) return;

    memcpy((char*)a->array + index * a->elem_size, value, a->elem_size);
}

int a_get(array_t* a, int index, void* out_value) {
    if (!out_value) return 0;
    if (!a || index < 0 || index >= a->length) {
        /* on error, zero the destination so callers don't keep previous/garbage value */
        size_t n = a ? a->elem_size : sizeof(void*);
        memset(out_value, 0, n);
        return 0;
    }
    memcpy(out_value, (char*)a->array + index * a->elem_size, a->elem_size);
    return 1;
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

void a_remove(array_t* a, int index) {
    if (a->length - 1 == index) {
        a->length--;
        return;
    }

    memmove(
        (char*) a->array + index * a->elem_size,
        (char*) a->array + (index+1) * a->elem_size,
        a->elem_size * (a->length - index - 1)
    );
    a->length--;
}

int a_search(array_t* a, const void* value) {
    for (int i = 0; i < a->length; i++) {
        void* e;
        a_get(a, i, e);
        if (memcmp(e, value, a->elem_size) == 0) {
            return i;
        }
    }
    return -1;
}