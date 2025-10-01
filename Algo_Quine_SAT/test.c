#include "formula.h"


int main() {

    Formula* formula = InitFormule("((a)&(b))&((a)|(!(c)))");

    f_print(formula);
    f_sf_print(formula);

    DestroyFormule(formula);
    return 0;
}