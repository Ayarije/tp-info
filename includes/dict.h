#ifndef DICT_H
#define DICT_H

#include <stdlib.h>
#include <stdio.h>

#include "vector.h"

struct dict_s;

typedef struct dict_s {
    Vector** keys;
    int* values;
    int size;
} dict_t;

dict_t* InitDict();
void DestroyDict(dict_t* d);

void d_put(dict_t* d, Vector* key, int value);
void d_delete(dict_t* d, Vector* key);

int d_get(dict_t* d, Vector* key);
void d_set(dict_t* d, Vector* key, int value);
int d_contains(dict_t* d, Vector* key);

#endif