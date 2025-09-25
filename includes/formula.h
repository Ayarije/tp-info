#ifndef FORMULA_H
#define FORMULA_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "vector.h"

typedef struct formula
{
    Vector* f;   // Formule principale
    int** sf; // Ensemble des sous formules
    int size;    // Taille de la formule (sf->lenght)
    int height;  // Hauteur de la formule
} Formula;


char f_table_i_c(int id) {
    switch (id)
    {
    case 0: return '-';
    case 1: return '|';
    case 2: return '&';
    case 3: return '(';
    case 4: return ')';
    case 5: return 'O';
    case 6: return 'N';
    default:
        char* alphabet = "abcdefghijklmnopkrstuvwxyz";
        if (id + 6 >= 32) { return ' '; }
        return alphabet[id + 6];
    }
}

int f_table_c_i(char c) {
    switch (c)
    {
    case '-': return 0;
    case '|': return 1;
    case '&': return 2;
    case '(': return 3;
    case ')': return 4;
    case 'O': return 5;
    case 'N': return 6;
    default:
        char* alphabet = "abcdefghijklmnopkrstuvwxyz";
        for (int i = 6; i < 32; i++) {
            if (alphabet[i-6] == c) {return i; }
        }
        return -1;
    }
}

Formula* InitFormule(char* f) {
    Formula* formula = (Formula*) malloc(sizeof(Formula));
    
    formula->f = InitVector();
    for (int i = 0; i < strlen(f); i++) { v_append(formula->f, f_table_c_i(f[i])); }

    
}

#endif