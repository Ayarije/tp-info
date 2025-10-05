#include "dict.h"


dict_t* InitDict() {
    dict_t* dict = (dict_t*) malloc(sizeof(dict_t));

    dict->keys = (Vector**) malloc(0);
    dict->values = (int*) malloc(0);
    dict->size = 0;

    return dict;
}

dict_t* CopyDict(dict_t* d) {
    dict_t* new_d = (dict_t*) malloc(sizeof(dict_t));

    new_d->keys = (Vector**) malloc(sizeof(Vector) * d->size);
    new_d->values = (int*) malloc(sizeof(int) * d->size);
    new_d->size = d->size;

    for (int i = 0; i < new_d->size; i++) {
        new_d->keys[i] = CopyVector(d->keys[i]);
        new_d->values[i] = d->values[i];
    }

    return new_d;
}

void DestroyDict(dict_t* d) {
    for (int i = 0; i < d->size; i++) {
        DestroyVector(d->keys[i]);
    }
    free(d->keys);
    free(d->values);
    free(d);
}

void d_put(dict_t* d, Vector* key, int value) {
    for (int i = 0; i < d->size; i++) {
        if (v_cmp(key, d->keys[i])) { return; }
    }

    Vector** r_keys = (Vector**) malloc(sizeof(Vector) * (d->size + 1));
    int* r_values = (int*) malloc(sizeof(int) * (d->size + 1));

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

void d_delete(dict_t* d, Vector* key) {
    Vector** r_keys = (Vector**) malloc(sizeof(Vector) * (d->size - 1));
    int* r_values = (int*) malloc(sizeof(int) * (d->size - 1));

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

int d_get(dict_t* d, Vector* key) {
    for (int i = 0; i < d->size; i++) {
        if (!v_cmp(key, d->keys[i])) { continue; }

        return d->values[i];
    }
    return NULL;
}

void d_set(dict_t* d, Vector* key, int value) {
    for (int i = 0; i < d->size; i++) {
        if (!v_cmp(key, d->keys[i])) { continue; }

        d->values[i] = value;
    }
}

int d_contains(dict_t* d, Vector* key) {
    for (int i = 0; i < d->size; i++) {
        if (v_cmp(key, d->keys[i])) { return 1; }
    }
    return 0;
}

void d_print(dict_t* d) {
    for (int i = 0; i < d->size; i++) {
        printf("[");
        for (int j = 0; j < d->keys[i]->length - 1; j++) {
            printf("%d ", *v_get(d->keys[i], j));
        }
        printf("%d] : %d\n", *v_get(d->keys[i], d->keys[i]->length-1), d->values[i]);
    }
}