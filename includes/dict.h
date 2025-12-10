#ifndef DICT_H
#define DICT_H

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include "array.h"

struct dict_s;

typedef struct dict_s {
    array_t* keys;
    size_t key_size;
    array_t* values;
    size_t value_size;
    int size;

    char* (*print_key)(void*);
    char* (*print_val)(void*);
} dict_t;

dict_t* InitDict(
    size_t key_size,
    size_t value_size
);
void DestroyDict(dict_t* d);
dict_t* CopyDict(dict_t* d);

int d_put(dict_t* d, const void* key, const void* value);
int d_delete(dict_t* d, const void* key);

int d_get(dict_t* d, const void* key, void* out_value);
int d_set(dict_t* d, const void* key, const void* value);
int d_contains(dict_t* d, const void* key);

void d_print(dict_t* d);

#endif