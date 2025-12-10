#include "dict.h"

dict_t* InitDict(size_t key_size, size_t value_size) {
    dict_t* dict = (dict_t*) malloc(sizeof(dict_t));

    dict->keys = InitEmptyArray(key_size);
    dict->key_size = key_size;
    dict->values = InitEmptyArray(value_size);
    dict->value_size = value_size;
    dict->size = 0;

    dict->print_key = NULL;
    dict->print_val = NULL;

    return dict;
}

dict_t* CopyDict(dict_t* d) {
    dict_t* new_d = (dict_t*) malloc(sizeof(dict_t));

    new_d->keys = CopyArray(d->keys);
    new_d->values = CopyArray(d->values);
    new_d->size = d->size;
    new_d->key_size = d->key_size;
    new_d->value_size = d->value_size;

    return new_d;
}

void DestroyDict(dict_t* d) {
    DestroyArray(d->keys);
    DestroyArray(d->values);
    free(d);
}

int d_put(dict_t* d, const void* key, const void* value) {
    if (a_search(d->keys, value) != -1) return 0;

    a_push(d->keys, key);
    a_push(d->values, value);

    d->size++;
    return 1;
}

int d_delete(dict_t* d, const void* key) {
    int index = a_search(d->keys, key);
    if (index == -1) return 0;

    a_remove(d->keys, index);

    d->size--;
    return 1;
}

int d_get(dict_t* d, const void* key, void* out_value) {
    int index = a_search(d->keys, key);
    if (index == -1) return 0;

    a_get(d->values, index, out_value);
    return 1;
}

int d_set(dict_t* d, const void* key, const void* value) {
    int index = a_search(d->keys, key);
    if (index == -1) return 0;

    a_set(d->values, index, value);
    return 1;
}

int d_contains(dict_t* d, const void* key) {
    return a_search(d->keys, key) != -1;
}

void d_print(dict_t* d) {
    printf("{\n");
    for (int i = 0; i < d->size; i++) {
        void* key;
        void* val;

        a_get(d->keys, i, key);
        a_get(d->values, i, val);

        if (d->print_key == NULL) {
            printf("  %p: ", key);
        } else {
            printf("  %s: ", d->print_key(key));
        }
        if (d->print_val == NULL) {
            printf("%p,\n", val);
        } else {
            printf("%s,\n", d->print_val(val));
        }
    }
    printf("}\n");
}