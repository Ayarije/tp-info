#include "formula.h"

// Encodage de f

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

/*
    Init Formule Subfunctions
*/

int calculate_size_and_encode_f(formula_t* f, char* str_f) {
    f->f = InitVector();
    f->size = 0;

    for (int i = 0; i < (int) strlen(f); i++) {
        // Calculate size (& or | : +2 ; ! : +1)
        if (str_f[i] == '&' || str_f[i] == '|') {
            f->size = f->size + 2;
        } else if (str_f[i] == '!') {
            f->size++;
        }

        int code = f_table_c_i(str_f[i]);
        if (code == -1) {
            fprintf(stderr, "Invalid symbol in formula: %c\n", f[i]);
            free(f);
            return 0;
        }
        v_append(f->f, code);
    }
    return 1;
}

int build_sf(formula_t* f) {
    f->sf = malloc(sizeof(Vector*) * (f->size));
    int cursor = 0;

    int open_p; // nb de parenthèse ouvertes
    int closed_p; // nb de parenthèse fermées

    int already_in;

    Vector* sf;
    for (int i = 0 ; i < f->f->length; i++) { // Build f->sf

        // Cherche la première parenthèse ouvrante
        if (*v_get(f->f, i) != 3) { continue; }

        open_p = 1;
        closed_p = 0;

        for (int j = i + 1; j < f->f->length; j++) { // cherche un equilibrage

            switch (*v_get(f->f, j))
            {
                case 3: open_p++; break;
                case 4: closed_p++; break;
                default: break;
            } // compte le nombre de parenthèses ouvertes et fermées

            // à l'equilibre (mm nb de parenthèses ouvertes et fermées)
            if (open_p != closed_p) { continue; }

            // Récupère la formule entre les deux parenthèses reliées
            sf = v_get_btw(f->f, i + 1, j - 1);
            already_in = 0;

            // Vérifie si la sous formule est déjà dans l'ensemble sf sinon ne l'ajoute pas
            for (int k = 0; k < cursor; k++) {
                if (v_cmp(sf, f->sf[k])) {
                    already_in = 1;
                    break;
                }
            }
            if (already_in) {
                free(sf);
                break;
            }

            // Ajoute la sous formule à l'ensemble
            f->sf[cursor] = sf;
            cursor++;
            break;
        }
    }

    f->sf_length = cursor; // cursor est la taille de l'ensemble à la fin de la boucle for
}

int sort_sf_by_length(formula_t* f) {

    // Build vector with corresponding sf length
    Vector* length = InitVectorWithValue(f->sf_length, 0);
    for (int i = 0; i < f->sf_length; i++) {
        v_set(length, i, f->sf[i]->length);
    }

    

}

formula_t* InitFormule(char* f) {
    formula_t* formula = (formula_t*) malloc(sizeof(formula_t));

    if (!calculate_size_and_encode_f(formula, f)) { return NULL; };

    build_sf(formula);

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

    DestroyVector(length);

    return formula;
}

void DestroyFormule(formula_t* f) {
    for (int i = 0; i< f->sf_length; i++) {
        DestroyVector(f->sf[i]);
    }
    free(f->sf);
    DestroyVector(f->f);
    free(f);
}

void f_print(formula_t* f) {
    for (int i = 0; i < f->f->length; i++) {
        printf("%c", f_table_i_c(*v_get(f->f, i)));
    }
    printf("\n");
}

void f_sf_print(formula_t* f) {
    for (int i = 0; i < f->sf_length; i++) {

        for (int j = 0; j < f->sf[i]->length; j++) {

            printf("%c", f_table_i_c(*v_get(f->sf[i], j)));

        }
        printf("\n");
    }
}