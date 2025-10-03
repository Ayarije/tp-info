#include "formula.h"


int main() {

    int a[8] = { 2, 5, 9, 1, 3, 5, 2, 6 };
    Vector* v = InitVectorArray(8, a);
    v_print(v);
    v_bubble_sort(v);
    v_print(v);

    formula_t* formula = InitFormule("((a)&(b))&((a)|(!(c)))");

    f_print(formula);
    f_sf_print(formula);

    DestroyFormule(formula);
    DestroyVector(v);
    return 0;
}