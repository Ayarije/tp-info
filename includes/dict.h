#ifndef DICT_H
#define DICT_H

#include <stdlib.h>
#include <stdio.h>

#include "vector.h"

typedef struct Dict {
    Vector** keys;
    int* values;
    int size;
} Dict;

Dict* InitDict() {
    Dict* dict = (Dict*) malloc(sizeof(Dict));

    dict->keys = (Vector**) malloc(0);
    dict->values = (int*) malloc(0);
    dict->size = 0;

    return dict;
}

void DestroyDict(Dict* d) {
    for (int i = 0; i < d->size; i++) {
        DestroyVector(d->keys[i]);
    }
    free(d->keys);
    free(d->values);
    free(d);
}

void d_put(Dict* d, Vector* key, int value) {
    for (int i = 0; i < d->size; i++) {
        if (v_cmp(key, d->keys[i])) { return; }
    }

    Vector** r_keys = (Vector**) malloc(sizeof(Vector) * d->size+1);
    int* r_values = (int*) malloc(sizeof(int) * d->size + 1);

    for (int i = 0; i < d->size; i++) {
        r_keys[i] = d->keys[i];
        r_values[i] = d->values[i];
    }
    r_keys[d->size] = key;
    r_values[d->size] = value;

    free(d->keys);
    free(d->values);

    d->keys = r_keys;
    d->values = r_values;
    d->size++;
}

void d_delete(Dict* d, Vector* key) {
    Vector** r_keys = (Vector**) malloc(sizeof(Vector) * d->size - 1);
    int* r_values = (int*) malloc(sizeof(int) * d->size - 1);

    int c = 0;
    for (int i = 0; i < d->size - 1; i++) {
        if (v_cmp(key, d->keys[i])) {
            c = -1;
            continue;
        }
        r_keys[i + c] = d->keys[i];
        r_values[i + c] = d->values[i];
    }

    free(d->keys);
    free(d->values);

    d->keys = r_keys;
    d->values = r_values;
    d->size--;
}

int d_get(Dict* d, Vector* key) {
    for (int i = 0; i < d->size; i++) {
        if (!v_cmp(key, d->keys[i])) { continue; }

        return d->values[i];
    }
}

void d_set(Dict* d, Vector* key, int value) {
    for (int i = 0; i < d->size; i++) {
        if (!v_cmp(key, d->keys[i])) { continue; }

        d->values[i] = value;
    }
}

int d_contains(Dict* d, Vector* key) {
    for (int i = 0; i < d->size; i++) {
        if (v_cmp(key, d->keys[i])) { return 1; }
    }
    return 0;
}

#endif