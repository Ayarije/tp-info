#include "matrices.h"

matrix* matrix_create(int width, int height, size_t element_size) {
    if (width <= 0 || height <= 0 || element_size == 0)
        return NULL;

    matrix* mat = malloc(sizeof(matrix));
    if (!mat)
        return NULL;

    mat->w = width;
    mat->h = height;
    mat->elem_size = element_size;
    mat->w_mem_space = width;
    mat->h_mem_space = height;

    mat->matrix = malloc(height * sizeof(void*));
    if (!mat->matrix) {
        free(mat);
        return NULL;
    }

    for (int i = 0; i < height; i++) {
        mat->matrix[i] = malloc(width * element_size);
        if (!mat->matrix[i]) {
            for (int j = 0; j < i; j++)
                free(mat->matrix[j]);
            free(mat->matrix);
            free(mat);
            return NULL;
        }
    }

    return mat;
}

void matrix_destroy(matrix* mat) {
    if (!mat)
        return;

    if (mat->matrix) {
        for (int i = 0; i < mat->h_mem_space; i++)
            free(mat->matrix[i]);
        free(mat->matrix);
    }
    free(mat);
}

matrix* matrix_copy(const matrix* src) {
    if (!matrix_is_valid(src))
        return NULL;

    matrix* copy = matrix_create(src->w, src->h, src->elem_size);
    if (!copy)
        return NULL;

    for (int i = 0; i < src->h; i++)
        memcpy(copy->matrix[i], src->matrix[i], src->w * src->elem_size);

    return copy;
}

int matrix_resize(matrix* mat, int new_width, int new_height) {
    if (!matrix_is_valid(mat) || new_width <= 0 || new_height <= 0)
        return 0;

    if (new_width <= mat->w_mem_space && new_height <= mat->h_mem_space) {
        mat->w = new_width;
        mat->h = new_height;
        return 1;
    }

    return matrix_reserve(mat, new_width, new_height);
}

int matrix_reserve(matrix* mat, int reserve_width, int reserve_height) {
    if (!matrix_is_valid(mat) || reserve_width < mat->w || reserve_height < mat->h)
        return 0;

    void** new_matrix = malloc(reserve_height * sizeof(void*));
    if (!new_matrix)
        return 0;

    for (int i = 0; i < reserve_height; i++) {
        new_matrix[i] = malloc(reserve_width * mat->elem_size);
        if (!new_matrix[i]) {
            for (int j = 0; j < i; j++)
                free(new_matrix[j]);
            free(new_matrix);
            return 0;
        }

        if (i < mat->h)
            memcpy(new_matrix[i], mat->matrix[i], mat->w * mat->elem_size);
    }

    for (int i = 0; i < mat->h_mem_space; i++)
        free(mat->matrix[i]);
    free(mat->matrix);

    mat->matrix = new_matrix;
    mat->w_mem_space = reserve_width;
    mat->h_mem_space = reserve_height;

    return 1;
}

void* matrix_get(const matrix* mat, int x, int y) {
    if (!matrix_is_valid(mat) || x < 0 || x >= mat->w || y < 0 || y >= mat->h)
        return NULL;

    return (char*)mat->matrix[y] + (x * mat->elem_size);
}

int matrix_set(matrix* mat, int x, int y, const void* element) {
    if (!matrix_is_valid(mat) || !element || x < 0 || x >= mat->w || y < 0 || y >= mat->h)
        return 0;

    memcpy((char*)mat->matrix[y] + (x * mat->elem_size), element, mat->elem_size);
    return 1;
}

int matrix_clear(matrix* mat) {
    if (!matrix_is_valid(mat))
        return 0;

    for (int i = 0; i < mat->h; i++)
        memset(mat->matrix[i], 0, mat->w * mat->elem_size);

    return 1;
}

int matrix_fill(matrix* mat, const void* element) {
    if (!matrix_is_valid(mat) || !element)
        return 0;

    for (int i = 0; i < mat->h; i++)
        for (int j = 0; j < mat->w; j++)
            matrix_set(mat, j, i, element);

    return 1;
}

matrix* matrix_transpose(const matrix* mat) {
    if (!matrix_is_valid(mat))
        return NULL;

    matrix* trans = matrix_create(mat->h, mat->w, mat->elem_size);
    if (!trans)
        return NULL;

    for (int i = 0; i < mat->h; i++)
        for (int j = 0; j < mat->w; j++)
            matrix_set(trans, i, j, matrix_get(mat, j, i));

    return trans;
}

int matrix_swap_rows(matrix* mat, int row1, int row2) {
    if (!matrix_is_valid(mat) || row1 < 0 || row1 >= mat->h || row2 < 0 || row2 >= mat->h)
        return 0;

    void* temp = mat->matrix[row1];
    mat->matrix[row1] = mat->matrix[row2];
    mat->matrix[row2] = temp;

    return 1;
}

int matrix_swap_columns(matrix* mat, int col1, int col2) {
    if (!matrix_is_valid(mat) || col1 < 0 || col1 >= mat->w || col2 < 0 || col2 >= mat->w)
        return 0;

    char* temp = malloc(mat->elem_size);
    if (!temp)
        return 0;

    for (int i = 0; i < mat->h; i++) {
        memcpy(temp, matrix_get(mat, col1, i), mat->elem_size);
        memcpy((char*)mat->matrix[i] + (col1 * mat->elem_size),
               (char*)mat->matrix[i] + (col2 * mat->elem_size),
               mat->elem_size);
        memcpy((char*)mat->matrix[i] + (col2 * mat->elem_size),
               temp,
               mat->elem_size);
    }

    free(temp);
    return 1;
}

int matrix_is_valid(const matrix* mat) {
    return mat && mat->matrix && mat->w > 0 && mat->h > 0 && 
           mat->elem_size > 0 && mat->w_mem_space >= mat->w && 
           mat->h_mem_space >= mat->h;
}

int matrix_get_width(const matrix* mat) {
    return matrix_is_valid(mat) ? mat->w : -1;
}

int matrix_get_height(const matrix* mat) {
    return matrix_is_valid(mat) ? mat->h : -1;
}

size_t matrix_get_element_size(const matrix* mat) {
    return matrix_is_valid(mat) ? mat->elem_size : 0;
}

int count_digits(int n) {
    if (n == 0) return 1;
    int count = 0;
    if (n < 0) {
        n *= -1;
        count++;
    }
    while (n > 0) {
        n /= 10;
        count++;
    }
    return count;
}

void matrix_print(const matrix* mat) {
    if (!matrix_is_valid(mat)) {
        printf("Error: Invalid matrix\n");
        return;
    }
    if (mat->elem_size != sizeof(int)) {
        printf("Error: You can only print int matrices\n");
        return;
    }

    // Get max digits of the matrix
    int max_n = 0;
    matrix_maximum(mat, &max_n, NULL, NULL);
    int max_digits = count_digits(max_n);

    // Print column indices
    printf("  ");
    for (int i = 0; i < mat->w; i++) {
        printf("%d", i);
        int curr_digits = count_digits(i);
        for (int j = 0; j < max_digits - curr_digits + 1; j++) { printf(" "); }
    }
    printf("\n");

    // Print each row with row indices
    for (int i = 0; i < mat->h; i++) {
        printf("%d ", i);
        for (int j = 0; j < mat->w; j++) {
            int element = *(int*)matrix_get(mat, j, i); // Assuming the matrix holds integers

            int curr_digits;
            if (element < 0) {
                printf("-");
                curr_digits = 1;
            }
            else {
                printf("%d", element);
                curr_digits = count_digits(element);
            }

            for (int j = 0; j < max_digits - curr_digits + 1; j++) { printf(" "); }
        }
        printf("\n");
    }
}

int matrix_minimum(const matrix* mat, int* out, int* out_x, int* out_y) {
    if (!matrix_is_valid(mat) || !out || mat->elem_size != sizeof(int))
        return 0;

    int min_x = 0, min_y = 0;
    int min = *(int*)matrix_get(mat, 0, 0);

    for (int y = 0; y < mat->h; y++) {
        for (int x = 0; x < mat->w; x++) {
            int* p = (int*)matrix_get(mat, x, y);
            if (!p) continue;
            if (*p < min) {
                min = *p;
                min_x = x;
                min_y = y;
            }
        }
    }

    *out = min;
    if (out_x) *out_x = min_x;
    if (out_y) *out_y = min_y;
    return 1;
}

int matrix_maximum(const matrix* mat, int* out, int* out_x, int* out_y) {
    if (!matrix_is_valid(mat) || !out || mat->elem_size != sizeof(int))
        return 0;

    int max_x = 0, max_y = 0;
    int max = *(int*)matrix_get(mat, 0, 0);

    for (int y = 0; y < mat->h; y++) {
        for (int x = 0; x < mat->w; x++) {
            int* p = (int*)matrix_get(mat, x, y);
            if (!p) continue;
            if (*p > max) {
                max = *p;
                max_x = x;
                max_y = y;
            }
        }
    }

    if (out) *out = max;
    if (out_x) *out_x = max_x;
    if (out_y) *out_y = max_y;
    return 1;
}

matrix* matrix_rotate_90_right(const matrix* mat) {
    if (!matrix_is_valid(mat))
        return NULL;

    matrix* rotated = matrix_create(mat->h, mat->w, mat->elem_size);
    if (!rotated)
        return NULL;

    for (int i = 0; i < mat->h; i++) {
        for (int j = 0; j < mat->w; j++) {
            void* element = matrix_get(mat, j, i);
            matrix_set(rotated, mat->h - 1 - i, j, element);
        }
    }

    return rotated;
}

matrix* matrix_rotate_90_left(const matrix* mat) {
    if (!matrix_is_valid(mat))
        return NULL;

    matrix* rotated = matrix_create(mat->h, mat->w, mat->elem_size);
    if (!rotated)
        return NULL;

    for (int i = 0; i < mat->h; i++) {
        for (int j = 0; j < mat->w; j++) {
            void* element = matrix_get(mat, j, i);
            matrix_set(rotated, i, mat->w - 1 - j, element);
        }
    }

    return rotated;
}