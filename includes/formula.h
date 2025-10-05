#ifndef FORMULA_H
#define FORMULA_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "vector.h"
#include "dict.h"

struct formula_s;

typedef struct formula_s
{
    Vector* f;     // Formule principale
    Vector** sf;   // Ensemble des sous formules
    int sf_length; // Taille de sf
    int size;      // Taille de la formule (taille maximale de sf)
} formula_t;


char f_table_i_c(int id);
int f_table_c_i(char c);
Vector* f_str_to_vec(char* str_f);
char* f_vec_to_str(Vector* vec_f);

// Init Formule SubFunctions
int calculate_size_and_encode_f(formula_t* f, char* str_f);
void build_sf(formula_t* f);
void sort_sf_by_length(formula_t* f);

formula_t* InitFormule(char* f);
void DestroyFormule(formula_t* f);

void f_print(formula_t* f);
void f_sf_print(formula_t* f);
void f_print_val(dict_t* val);

dict_t* f_get_global_valuation(formula_t* f, dict_t* valuation);

formula_t* f_substitute(formula_t* f, Vector* old_f, Vector* new_f);

void quine_minimization(formula_t* f);

#endif