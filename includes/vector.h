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

Vector* InitVectorWithValue(int size, int value) {
    Vector* v = malloc(sizeof(Vector));
    if (!v) { return NULL; }
    v->lenght = size;
    v->array = malloc(sizeof(int)*v->lenght);

    for (int i = 0; i < v->lenght; i++) { v->array[i] = value; }

    return v;
}

Vector* CopyVector(Vector* v) {
    Vector* new_v = malloc(sizeof(Vector));
    if (!v) { return NULL; }
    new_v->lenght = v->lenght;
    new_v->array = malloc(sizeof(int)*new_v->lenght);

    for (int i = 0; i < v->lenght; i++) { new_v->array[i] = v->array[i]; }

    return new_v;
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

int* v_get(Vector* v, int i) {
    if (i < 0 || i >= v->lenght) { return NULL; }
    return &v->array[i];
}

void v_set(Vector* v, int i, int value) {
    if (i < 0 || i >= v->lenght) { return; }
    v->array[i] = value;
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
    for (int i = 0; i < v->lenght - 1; i++) {
        printf("%d ", v->array[i]);
    }
    printf("%d]\n", v->array[v->lenght - 1]);

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

int v_count(Vector* v, int value) {
    int count = 0;

    for (int i = 0; i < v->lenght; i++) {
        if (v->array[i] == value) { count++; }
    }

    return count;
}


struct Vector2d;

typedef struct Vector2d {
    int* shape;
    int lenght;
    int* array;
} Vector2d;

Vector2d* Init2dVector(int w, int h, int value) {
    Vector2d* v = malloc(sizeof(Vector2d));
    if (!v) { return NULL; }

    v->shape = malloc(sizeof(int)*2);
    v->shape[0] = w;
    v->shape[1] = h;

    v->lenght = w*h;
    v->array = malloc(sizeof(int)*v->lenght);

    for (int i = 0; i < v->lenght; i++) { v->array[i] = value; }

    return v;
}

Vector2d* Copy2dVector(Vector2d* v) {
    Vector2d* new_v = malloc(sizeof(Vector));
    if (!v) { return NULL; }
    new_v->lenght = v->lenght;
    new_v->shape = v->shape;
    new_v->array = malloc(sizeof(int)*new_v->lenght);

    for (int i = 0; i < v->lenght; i++) { new_v->array[i] = v->array[i]; }

    return new_v;
}

void Destroy2dVector(Vector2d* v) {
    free(v->shape);
    free(v->array);
    free(v);
}

int* v2d_get(Vector2d* v, int x, int y) {
    if (x < 0 || x >= v->shape[0]) { return NULL; }
    if (y < 0 || y >= v->shape[1]) { return NULL; }

    return &v->array[y*v->shape[0] + x];
}

void v2d_set(Vector2d* v, int x, int y, int value) {
    if (x < 0 || x >= v->shape[0]) { return; }
    if (y < 0 || y >= v->shape[1]) { return; }

    v->array[y*v->shape[0] + x] = value;
}

void v2d_set_line(Vector2d* v, int y, int* line, int line_size) {
    if (y < 0 || y >= v->shape[1]) { return; }
    if (line_size > v->shape[0]) { return; }

    int i = y * v->shape[0];

    for (int i0 = 0; i0 < line_size; i0++) {
        v->array[i] = line[i0];
        i++;
    }
}

Vector* v2d_get_line(Vector2d* v, int y) {
    Vector* line = InitVector();

    // if (y < 0 || y >= v->shape[1]) { return line; }

    for (int i = y * v->shape[0]; i < v->shape[0]; i++) {
        printf("adding %d at index %d to the new vector\n", v->array[i], i);
        v_append(line, v->array[i]);
    }

    v_print(line);
    return line;
}

void v2d_print(Vector2d* v) {
    printf("[");
    for (int y = 0; y < v->shape[1]; y++) {
        if (y != 0) { printf(" "); }
        printf("[ ");

        for (int x = 0; x < v->shape[0]; x++) {
            printf("%d ", v->array[y*v->shape[0] + x]);
        }

        printf("]");
        if (y == v->shape[1] - 1) { break; }
        printf("\n");
    }
    printf("]\n");
}


int v2d_min(Vector2d* v) {
    if (v->lenght == 0) { return -32768; }
    int min = v->array[0];

    for (int i = 1; i < v->lenght; i++) {
        if (v->array[i] < min) {
            min = v->array[i];
        }
    }

    return min;
}

int v2d_max(Vector2d* v) {
    if (v->lenght == 0) { return 32768; }
    int max = v->array[0];

    for (int i = 1; i < v->lenght; i++) {
        if (v->array[i] > max) {
            max = v->array[i];
        }
    }

    return max;
}

void v2d_sort(Vector2d* v) {
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

int* v2d_coords(Vector2d* v, int value) {
    int* output = malloc(sizeof(int) * 2);
    output[0] = -1;
    output[1] = -1;
    
    if (v->lenght == 0) { return output; }

    for (int x = 0; x < v->shape[0]; x++) {
        for (int y = 0; y < v->shape[1]; y++) {

            if (v->array[y*v->shape[0] + x] == value) {
                output[0] = x;
                output[1] = y;
                return output;
            }
            
        }
    }
    
    return output;
}

int v2d_count(Vector2d* v, int value) {
    int count = 0;

    for (int i = 0; i < v->lenght; i++) {
        if (v->array[i] == value) { count++; }
    }

    return count;
}

#endif