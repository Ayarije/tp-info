#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <stdlib.h>

struct Vector;

typedef struct Vector {
    int length;
    int* array;
} Vector;

Vector* InitVector() { // Empty vector constructor
    Vector* v = malloc(sizeof(Vector));
    if (!v) { return NULL; }
    v->length = 0;
    v->array = malloc(sizeof(int)*v->length);
    return v;
}

Vector* InitVectorWithValue(int size, int value) { // Sized vector constructor
    /*
    int size  : vector length
    int value : vector's cells value
    */
    Vector* v = malloc(sizeof(Vector));
    if (!v) { return NULL; }
    v->length = size;
    v->array = malloc(sizeof(int)*v->length);

    for (int i = 0; i < v->length; i++) { v->array[i] = value; }

    return v;
}

Vector* InitVectorArray(int size, int* array) {
    Vector* v = malloc(sizeof(Vector));
    if (!v) { return NULL; }
    v->length = size;
    v->array = malloc(sizeof(int)*v->length);

    for (int i = 0; i < v->length; i++) { v->array[i] = array[i]; }

    return v;
}

Vector* CopyVector(Vector* v) { // Copy constructor
    Vector* new_v = malloc(sizeof(Vector));
    if (!v) { return NULL; }
    new_v->length = v->length;
    new_v->array = malloc(sizeof(int)*new_v->length);

    for (int i = 0; i < v->length; i++) { new_v->array[i] = v->array[i]; }

    return new_v;
}

void DestroyVector(Vector* v) { // Destructor
    free(v->array);
    free(v);
}

void swap(int* xp, int* yp){ // Useful for bubble sort
    // Take two pointer and swap their pointed variable's value
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

int* v_get(Vector* v, int i) { // Get a vector's value at i index
    if (i < 0 || i >= v->length) { return NULL; }
    return &v->array[i];
}

int v_cmp(Vector* v1, Vector* v2) {
    if (v1->length != v2->length) { return 0; }

    for (int i = 0; i < v1->length; i++) { 
        if (v1->array[i] != v2->array[i]) { return 0; }
    }
    
    return 1;
}

Vector* v_get_btw(Vector* v, int x, int y) {
    if (x < 0 || x > y || y < x || y >= v->length) { return InitVector(); }

    Vector* result = InitVectorWithValue(y - x + 1, 0);

    for (int i = x; i <= y; i++) { result->array[i - x] = v->array[i]; }

    return result;
}

void v_set(Vector* v, int i, int value) { // Set a vector's value at i index
    if (i < 0 || i >= v->length) { return; }
    v->array[i] = value;
}

int v_sum(Vector* v) { // sum each vector's cell value
    int sum = 0;

    for (int i = 0; i < v->length; i++) {
        sum += v->array[i];
    }
    
    return sum;
}

void v_append(Vector* v, int value) { // Append a cell at the end of the vector
    int* new_array = malloc(sizeof(int)*(v->length + 1)); // Create new array of length + 1
    
    for (int i = 0; i < v->length; i++) { // Fill the new array with prev array values
        new_array[i] = v->array[i];
    }
    new_array[v->length] = value; // Add the new value

    free(v->array);
    v->array = new_array;
    v->length++;
}

Vector* v_sum_vectors(Vector* l1, Vector* l2) { // Sum two vector (cell by cell)
    /*
    input: two vectors of equivalent length
    output: new vector with each cell equals the sum of
            the corresponding cell of the prev vectors
    */

    if (l1->length != l2->length) { return InitVector(); }

    Vector* result = InitVectorWithValue(l1->length, 0);
    
    for (int i = 0; i < l1->length; i++) {
        result->array[i] = l1->array[i] + l2->array[i];
    }

    return result;
}

void v_pop(Vector* v) { // Pop a cell at the end of the vector
    if (v->length != 0) { return; }

    int* new_array = malloc(sizeof(int)*(v->length - 1)); // Create a new array of length - 1
    
    // Fill the new array with the length - 1 prev values
    for (int i = 0; i < v->length-1; i++) {
        new_array[i] = v->array[i];
    }

    free(v->array);
    v->array = new_array;
    v->length--;
}

void v_insert(Vector* v, int value, int index) { // Insert a cell at i index in the vector
    if (index < 0 || index > v->length) { return; }

    int* new_array = malloc(sizeof(int)*(v->length + 1)); // Create new array of length + 1
    
    // Fill it until index i
    for (int i = 0; i < v->length - index; i++) {
        new_array[i] = v->array[i];
    }
    new_array[index] = value; // insert value
    for (int i = index+1; i < v->length+1; i++) { // finish filling it
        new_array[i] = v->array[i];
    }

    free(v->array);
    v->array = new_array;
    v->length++;
}

void v_delete(Vector* v, int index) { // Delete a cell ar index i
    if (index < 0 || index >= v->length) { return; }

    int* new_array = malloc(sizeof(int)*(v->length - 1)); // Create a new array of length - 1

    // Fill it until index i
    for (int i = 0; i < v->length - index; i++) {
        new_array[i] = v->array[i];
    }
    // Skip index i and continue filling it
    for (int i = index+1; i < v->length - 1; i++) {
        new_array[i] = v->array[i];
    }

    free(v->array);
    v->array = new_array;
    v->length--;
}

void v_print(Vector* v) { // print vector
    printf("[");
    for (int i = 0; i < v->length - 1; i++) {
        printf("%d ", v->array[i]);
    }
    printf("%d]\n", v->array[v->length - 1]);
}


int v_min(Vector* v) { // return the minimum value of the vector
    if (v->length == 0) { return -32768; }
    int min = v->array[0];

    for (int i = 1; i < v->length; i++) {
        if (v->array[i] < min) {
            min = v->array[i];
        }
    }

    return min;
}

int v_max(Vector* v) { // return the maximum value of the vector
    if (v->length == 0) { return 32768; }
    int max = v->array[0];

    for (int i = 1; i < v->length; i++) {
        if (v->array[i] > max) {
            max = v->array[i];
        }
    }

    return max;
}

void v_sort(Vector* v) { // bubble sorting the vector
    int i, j;
    int swapped;

    for (i = 0; i < v->length - 1; i++) {
        swapped = 0;
        for (j = 0; j < v->length - i - 1; j++) {
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

int v_index(Vector* v, int value) { // return the first index of vector's value == value
    if (v->length == 0) { return -1; }

    for (int i = 0; i < v->length; i++) {
        if (v->array[i] == value) {
            return i;
        }
    }
    return -1;
}

int v_count(Vector* v, int value) { // count how many values are in value are in the vector
    int count = 0;

    for (int i = 0; i < v->length; i++) {
        if (v->array[i] == value) { count++; }
    }

    return count;
}


struct Vector2d;

typedef struct Vector2d {
    int* shape;
    int length;
    int* array;
} Vector2d;

Vector2d* Init2dVector(int w, int h, int value) {
    Vector2d* v = malloc(sizeof(Vector2d));
    if (!v) { return NULL; }

    v->shape = malloc(sizeof(int)*2);
    v->shape[0] = w;
    v->shape[1] = h;

    v->length = w*h;
    v->array = malloc(sizeof(int)*v->length);

    for (int i = 0; i < v->length; i++) { v->array[i] = value; }

    return v;
}

Vector2d* Copy2dVector(Vector2d* v) {
    Vector2d* new_v = malloc(sizeof(Vector));
    if (!v) { return NULL; }
    new_v->length = v->length;
    new_v->shape = v->shape;
    new_v->array = malloc(sizeof(int)*new_v->length);

    for (int i = 0; i < v->length; i++) { new_v->array[i] = v->array[i]; }

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

    if (y < 0 || y >= v->shape[1]) { return line; }

    for (int i = y * v->shape[0]; i < y * v->shape[0] + v->shape[0]; i++) {
        v_append(line, v->array[i]);
    }

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
    if (v->length == 0) { return -32768; }
    int min = v->array[0];

    for (int i = 1; i < v->length; i++) {
        if (v->array[i] < min) {
            min = v->array[i];
        }
    }

    return min;
}

int v2d_max(Vector2d* v) {
    if (v->length == 0) { return 32768; }
    int max = v->array[0];

    for (int i = 1; i < v->length; i++) {
        if (v->array[i] > max) {
            max = v->array[i];
        }
    }

    return max;
}

void v2d_sort(Vector2d* v) {
    int i, j;
    int swapped;

    for (i = 0; i < v->length - 1; i++) {
        swapped = 0;
        for (j = 0; j < v->length - i - 1; j++) {
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
    
    if (v->length == 0) { return output; }

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

    for (int i = 0; i < v->length; i++) {
        if (v->array[i] == value) { count++; }
    }

    return count;
}

#endif
