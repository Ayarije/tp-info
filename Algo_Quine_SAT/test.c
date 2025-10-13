#include "formula.h"


int main() {
    dict_t* d = InitDict();
    d_put(d, InitVectorWithValue(1, f_table_c_i('a')), 1);
    d_put(d, InitVectorWithValue(1, f_table_c_i('b')), 1);
    d_put(d, InitVectorWithValue(1, f_table_c_i('c')), 1);

    printf("Dict :\n");
    d_print(d);

    formula_t* formula = InitFormule("!(((a)&(b))&((a)|(!(c))))");

    printf("\nFormule :\n");
    f_print(formula);
    printf("sous formules :\n");
    f_sf_print(formula);
    
    dict_t* val = f_get_global_valuation(formula, d);

    printf("\nformula valuation :\n");
    f_print_val(val);

    Vector* old_f = f_str_to_vec("(a)&(b)");
    Vector* new_f = f_str_to_vec("d");
    formula_t* sub_f = f_substitute(formula, old_f, new_f);

    printf("\nSubstitution ('(a)&(b)' -> 'd'):\n");
    f_print(formula);
    f_print(sub_f);

    formula_t* quine_f = InitFormule("(d)|((c)&(!(((a)&(b))|(1))))");

    printf("\nQuine minimization :\n");
    f_print(quine_f);
    quine_minimization(quine_f);
    f_print(quine_f);

    DestroyDict(d);
    DestroyDict(val);
    
    DestroyFormule(sub_f);
    DestroyFormule(formula);
    DestroyFormule(quine_f);

    DestroyVector(old_f);
    DestroyVector(new_f);
    return 0;
}