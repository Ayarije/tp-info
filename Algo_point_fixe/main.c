#include <stdio.h>
#include <stdlib.h>

#include "vector.h"


void mul_line(Vector* line, int n) {
    for (int i = 0; i < line->length; i++) {
        v_set(line, i, *v_get(line, i) * n);
    }
}

Vector* add_lines(Vector* l1, Vector* l2) {
    Vector* new_line = InitVector();

    if (l1->length != l2->length) { return new_line; }
    
    for (int i = 0; i < l1->length; i++) {
        v_append(new_line, *v_get(l1, i) + *v_get(l2, i));
    }

    return new_line;
}


void point_fixe(Vector2d* input) {
    Vector* pivot;
    Vector* current_line;
    Vector* result_line;
    int x_pivot;
    int x_used;

    for (int p = 0; p < input->shape[1] - 1; p++) {
        pivot = v2d_get_line(input, p);

        for (int y = p + 1; y < input->shape[1]; y++) {
            current_line = v2d_get_line(input, y);

            x_pivot = *v_get(pivot, p);
            x_used = *v_get(current_line, p);

            mul_line(pivot, x_used);
            mul_line(current_line, -x_pivot);

            result_line = add_lines(current_line, pivot);

            v2d_set_line(input, y, result_line->array, result_line->length);
            
            DestroyVector(current_line);
            DestroyVector(result_line);
        }

        DestroyVector(pivot);
    }
}


int main() {
    Vector2d* input = Init2dVector(4, 3, 0);

    v2d_set_line(input, 0, (int[]) {5, 9, 2, 3}, 4);
    v2d_set_line(input, 1, (int[]) {8, 4, 6, 1}, 4);
    v2d_set_line(input, 2, (int[]) {5, 8, 6, 2}, 4);

    v2d_print(input);

    point_fixe(input);
    printf("\n");

    v2d_print(input);

    Destroy2dVector(input);
    return 0;
}


