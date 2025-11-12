#ifndef MATRICES_H
#define MATRICES_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct matrix_s
{
    int w;              // Current width
    int h;              // Current height
    size_t elem_size;   // Size of each element
    void** matrix;      // 2D array of elements
    int w_mem_space;    // Allocated width
    int h_mem_space;    // Allocated height
} matrix;

// Creation and destruction
matrix* matrix_create(int width, int height, size_t element_size);
void matrix_destroy(matrix* mat);
matrix* matrix_copy(const matrix* src);

// Memory management
int matrix_resize(matrix* mat, int new_width, int new_height);
int matrix_reserve(matrix* mat, int reserve_width, int reserve_height);

// Element access and modification
void* matrix_get(const matrix* mat, int x, int y);
int matrix_set(matrix* mat, int x, int y, const void* element);

// Matrix operations
int matrix_clear(matrix* mat);
int matrix_fill(matrix* mat, const void* element);
matrix* matrix_transpose(const matrix* mat);
matrix* matrix_rotate_90_right(const matrix* mat);
matrix* matrix_rotate_90_left(const matrix* mat);

// Row and column operations
int matrix_swap_rows(matrix* mat, int row1, int row2);
int matrix_swap_columns(matrix* mat, int col1, int col2);

// Utility functions
int matrix_is_valid(const matrix* mat);
int matrix_get_width(const matrix* mat);
int matrix_get_height(const matrix* mat);
size_t matrix_get_element_size(const matrix* mat);
void matrix_print(const matrix* mat);
int matrix_minimum(const matrix* mat, int* out, int* out_x, int* out_y);
int matrix_maximum(const matrix* mat, int* out, int* out_x, int* out_y);

#endif