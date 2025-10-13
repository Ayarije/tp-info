#include "formula.h"


int heuristic(formula_t* f) {
    Vector* count_array = InitVectorWithValue(26, 0);

    for (int i = 0; i < f->f->length; i++) {
        if (*v_get(f->f, i) <= 6) { continue; }
        count_array->array[i - 7]++;
    }
    return v_max_i(count_array) + 7;
}

dict_t* quine_algorithm(int (*h) (formula_t*), formula_t* f) {
    
}

int main() {
    return 0;
}
