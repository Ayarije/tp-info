#ifndef FORMULA_H
#define FORMULA_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "vector.h"
#include "dict.h"

typedef struct formula
{
    Vector* f;     // Formule principale
    Vector** sf;   // Ensemble des sous formules
    int sf_length; // Taille de sf
    int size;      // Taille de la formule (taille maximale de sf)
} Formula;


char f_table_i_c(int id) {
    switch (id)
    {
    case 0: return '!';
    case 1: return '|';
    case 2: return '&';
    case 3: return '(';
    case 4: return ')';
    case 5: return 'O';
    case 6: return 'N';
    default:
        char* alphabet = "abcdefghijklmnopqrstuvwxyz";
        int idx = id - 7; // alphabet commence à 7
        if (idx < 0 || idx >= 26) return '?';
        return alphabet[idx];
    }
}

int f_table_c_i(char c) {
    switch (c)
    {
    case '!': return 0;
    case '|': return 1;
    case '&': return 2;
    case '(': return 3;
    case ')': return 4;
    case 'O': return 5;
    case 'N': return 6;
    default:
        char* alphabet = "abcdefghijklmnopkrstuvwxyz";
        int len = strlen(alphabet);
        for (int i = 0; i < len; i++) {
            if (alphabet[i] == c) return i + 7;
        }
        fprintf(stderr, "Unknown character in formula: %c\n", c);
        return -1; // error code
    }
}

Formula* InitFormule(char* f) {
    Formula* formula = (Formula*) malloc(sizeof(Formula));
    
    formula->f = InitVector();

    formula->size = 0;
    
    for (int i = 0; i < (int) strlen(f); i++) {
        // Calculate size
        if (f[i] == '&' || f[i] == '|') {
            formula->size = formula->size + 2;
        } else if (f[i] == '!') {
            formula->size++;
        }

        int code = f_table_c_i(f[i]);
        if (code == -1) {
            fprintf(stderr, "Invalid symbol in formula: %c\n", f[i]);
            free(formula);
            return NULL;
        }
        v_append(formula->f, code);
    }

    formula->sf = malloc(sizeof(Vector*) * (formula->size));
    int cursor = 0;

    int open_p; // nb de parenthèse ouvertes
    int closed_p; // nb de parenthèse fermées

    int already_in;

    Vector* sf;
    for (int i = 0 ; i < formula->f->length; i++) { // Build f->sf

        if (*v_get(formula->f, i) != 3) { // Cherche la première parenthèse ouvrante
            continue;
        }

        open_p = 1;
        closed_p = 0;

        for (int j = i + 1; j < formula->f->length; j++) { // cherche un equilibrage

            switch (*v_get(formula->f, j))
            {
                case 3: open_p++; break;
                case 4: closed_p++; break;
                default: break;
            }

            if (open_p == closed_p) {
                sf = v_get_btw(formula->f, i + 1, j - 1);
                already_in = 0;
                // check if sf is already in f->sf
                for (int k = 0; k < cursor; k++) {
                    if (v_cmp(sf, formula->sf[k])) {
                        already_in = 1;
                        break;
                    }
                }

                if (already_in) {
                    free(sf);
                    break;
                }

                formula->sf[cursor] = sf;
                cursor++;
                break;
            }
        }
    }

    formula->sf_length = cursor;


    printf("cursor = %d\nf->size = %d\nsf length = %d\n", cursor, formula->size, formula->sf_length);

    // Sort f->sf by each sf length
    Vector* length = InitVectorWithValue(formula->sf_length, 0); // Create a vector storing sf lengths

    for (int i = 0; i < formula->sf_length; i++) { v_set(length, i, formula->sf[i]->length); }

    int swapped;

    for (int i = 0; i < formula->sf_length - 1; i++) {
        swapped = 0;
        for (int j = 0; j < formula->sf_length - i - 1; j++) {
            if (*v_get(length, j) > v_get(length, j+1)) {
                // Swap length in vector
                swap(v_get(length, j), v_get(length, j+1));
                
                // Swap vector in array
                Vector* temp = formula->sf[j];
                formula->sf[j] = formula->sf[j+1];
                formula->sf[j+1] = temp;

                swapped = 1;
            }
        }

        if (swapped == 0) {
            break;
        }
    }

    free(length);

    return formula;
}

void DestroyFormule(Formula* f) {
    for (int i = 0; i< f->sf_length; i++) {
        DestroyVector(f->sf[i]);
    }
    free(f->sf);
    DestroyVector(f->f);
    free(f);
}

void f_print(Formula* f) {
    for (int i = 0; i < f->f->length; i++) {
        printf("%c", f_table_i_c(*v_get(f->f, i)));
    }
    printf("\n");
}

void f_sf_print(Formula* f) {
    for (int i = 0; i < f->sf_length; i++) {

        for (int j = 0; j < f->sf[i]->length; j++) {

            printf("%c", f_table_i_c(*v_get(f->sf[i], j)));

        }
        printf("\n");
    }
}

int f_check_valuation_chars(Formula* f, char* vars, Vector* values) {
    if (strlen(vars) != values->length) { return 0; }
    
    

    return 0;

}

int f_check_valuation(Formula* f, Vector* vars, Vector* values) {
    if (vars->length != values->length) { return 0; }

    Dict* bool_values = InitDict();

    Vector* sf;
    int c;
    for (int i = f->size - 1; i >= 0; i--) {
        sf = f->sf[i];



        for (int j = 0; j < sf->length; j++) {
            c = *v_get(sf, j);
            
            switch (c)
            {
            case 0: // !p
                /* code */
                break;
            
            default:
                break;
            }
        }
    }

    return 0;
}

#endif