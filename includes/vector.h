#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <stdlib.h>

struct Vector;

typedef struct Vector {
    int lenght;
    int* array;
} Vector;

Vector* InitVector() {
    Vector* v = malloc(sizeof(Vector));
    if (!v) { return NULL; }
    v->lenght = 0;
    v->array = malloc(sizeof(int)*v->lenght);
    return v;
}

void DestroyVector(Vector* v) {
    free(v->array);
    free(v);
}

void swap(int* xp, int* yp){
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

int v_get(Vector* v, int i) {
    if (i < 0 || i >= v->lenght) { return 0; }
    return v->array[i];
}

void v_append(Vector* v, int value) {
    int* new_array = malloc(sizeof(int)*(v->lenght + 1));
    
    for (int i = 0; i < v->lenght; i++) {
        new_array[i] = v->array[i];
    }
    new_array[v->lenght] = value;

    free(v->array);
    v->array = new_array;
    v->lenght++;
}

void v_pop(Vector* v) {
    if (v->lenght != 0) { return; }

    int* new_array = malloc(sizeof(int)*(v->lenght - 1));
    
    for (int i = 0; i < v->lenght-1; i++) {
        new_array[i] = v->array[i];
    }

    free(v->array);
    v->array = new_array;
    v->lenght--;
}

void v_insert(Vector* v, int value, int index) {
    if (index < 0 || index > v->lenght) { return; }

    int* new_array = malloc(sizeof(int)*(v->lenght + 1));
    
    for (int i = 0; i < v->lenght - index; i++) {
        new_array[i] = v->array[i];
    }
    new_array[index] = value;
    for (int i = index+1; i < v->lenght+1; i++) {
        new_array[i] = v->array[i];
    }

    free(v->array);
    v->array = new_array;
    v->lenght++;
}

void v_delete(Vector* v, int index) {
    if (index < 0 || index >= v->lenght) { return; }

    int* new_array = malloc(sizeof(int)*(v->lenght - 1));

    for (int i = 0; i < v->lenght - index; i++) {
        new_array[i] = v->array[i];
    }
    for (int i = index+1; i < v->lenght - 1; i++) {
        new_array[i] = v->array[i];
    }

    free(v->array);
    v->array = new_array;
    v->lenght--;
}

void v_print(Vector* v) {

    printf("[");
    for (int i = 0; i < v->lenght; i++) {
        printf("%d ", v->array[i]);
    }
    printf("]\n");

}


int v_min(Vector* v) {
    if (v->lenght == 0) { return -32768; }
    int min = v->array[0];

    for (int i = 1; i < v->lenght; i++) {
        if (v->array[i] < min) {
            min = v->array[i];
        }
    }

    return min;
}

int v_max(Vector* v) {
    if (v->lenght == 0) { return 32768; }
    int max = v->array[0];

    for (int i = 1; i < v->lenght; i++) {
        if (v->array[i] > max) {
            max = v->array[i];
        }
    }

    return max;
}

void v_sort(Vector* v) {
    int i, j;
    int swapped;

    for (i = 0; i < v->lenght - 1; i++) {
        swapped = 0;
        for (j = 0; j < v->lenght - i - 1; j++) {
            if (v->array[j] > v->array[j + 1]) {
                swap(&(v->array[j]), &(v->array[j + 1]));
                swapped = 1;
            }
        }

        if (swapped == 0) {
            break;
        }
    }
}

int v_index(Vector* v, int value) {
    if (v->lenght == 0) { return -1; }

    for (int i = 0; i < v->lenght; i++) {
        if (v->array[i] == value) {
            return i;
        }
    }
    return -1;
}

#endif