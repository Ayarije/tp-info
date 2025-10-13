#include "formula.h"


// fonctions utilitaires
Vector* get_first_member(Vector* f) {
    if (f->length <= 2) { return NULL; }

    int open_p = 0; // nb de parenthèses ouvertes
    int closed_p = 0; // nb de parenthèses fermée

    for (int i = 0; i < f->length; i++) {
        switch (*v_get(f, i))
        {
            case 3: open_p++; break;
            case 4: closed_p++; break;
            default: break;
        } // compte le nombre de parenthèses ouvertes et fermées

        if (open_p == closed_p) {
            return v_get_btw(f, 1, i - 1);
        }
    }

    return NULL;
}

Vector* get_second_member(Vector* f) {
    if (f->length <= 6) { return NULL; }
    int open_p = 0; // nb de parenthèses ouvertes
    int closed_p = 0; // nb de parenthèses fermée

    int f_index = 0; // index de la fin de la première parenthèse

    for (int i = 0; i < f->length; i++) {
        switch (*v_get(f, i))
        {
            case 3: open_p++; break;
            case 4: closed_p++; break;
            default: break;
        } // compte le nombre de parenthèses ouvertes et fermées

        if (open_p == closed_p) {
            f_index = i + 2; // Enregistre la fin de la 1ere plus grande suite de parenthèse
            break;
        }
    }

    open_p = 0;
    closed_p = 0;
    for (int i = f_index; i < f->length; i++) {
        switch (*v_get(f, i))
        {
            case 3: open_p++; break;
            case 4: closed_p++; break;
            default: break;
        } // compte le nombre de parenthèses ouvertes et fermées

        if (open_p == closed_p) {
            return v_get_btw(f, f_index + 1, i - 1);
        }
    }

    return NULL;
}

int get_largest_connector(Vector* f) {
    if (*v_get(f, 0) == 0) { return 0; } // connecteur '!'
    if (f->length <= 6) { return -1; }

    int open_p = 0; // nb de parenthèses ouvertes
    int closed_p = 0; // nb de parenthèses fermée

    for (int i = 0; i < f->length; i++) {
        switch (*v_get(f, i))
        {
            case 3: open_p++; break;
            case 4: closed_p++; break;
            default: break;
        } // compte le nombre de parenthèses ouvertes et fermées

        if (open_p == closed_p) {
            return *v_get(f, i+1);
        }
    }

    return -1;
}

// Encodage de f
char f_table_i_c(int id) {
    switch (id)
    {
    case 0: return '!';
    case 1: return '|';
    case 2: return '&';
    case 3: return '(';
    case 4: return ')';
    case 5: return '1';
    case 6: return '0';
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
    case '1': return 5;
    case '0': return 6;
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

Vector* f_str_to_vec(char* str_f) {
    int f_length = strlen(str_f);
    Vector* vec_f = InitVectorWithValue(f_length, 0);

    for (int i = 0; i < f_length; i++) {
        v_set(vec_f, i, f_table_c_i(str_f[i]));
    }

    return vec_f;
}

char* f_vec_to_str(Vector* vec_f) {
    int f_length = vec_f->length;
    char* str_f = malloc(sizeof(char) * (f_length + 1));

    for (int i = 0; i < f_length; i++) {
        str_f[i] = f_table_i_c(*v_get(vec_f, i));
    }
    str_f[f_length] = '\0';

    return str_f;
}

/*
    Init Formule Subfunctions
*/

int calculate_size_and_encode_f(formula_t* f, char* str_f) {
    f->f = InitVectorWithValue(strlen(str_f), 0);
    f->size = 0;

    for (int i = 0; i < (int) strlen(str_f); i++) {
        // Calculate size (& or | : +2 ; ! : +1)
        if (str_f[i] == '&' || str_f[i] == '|') {
            f->size = f->size + 2;
        } else if (str_f[i] == '!') {
            f->size++;
        }

        int code = f_table_c_i(str_f[i]);
        if (code == -1) {
            fprintf(stderr, "Invalid symbol in formula: %c\n", str_f[i]);
            free(f);
            return 0;
        }
        v_set(f->f, i, code);
    }
    return 1;
}

void build_sf(formula_t* f) {
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
                DestroyVector(sf);
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

void sort_sf_by_length(formula_t* f) {
    // Create a vector storing sf lengths
    Vector* length = InitVectorWithValue(f->sf_length, 0);
    for (int i = 0; i < f->sf_length; i++) {
        v_set(length, i, f->sf[i]->length);
    }

    // Bubble sort length vector and sf in parallel
    int swapped;

    for (int i = 0; i < f->sf_length - 1; i++) {
        swapped = 0;
        for (int j = 0; j < f->sf_length - i - 1; j++) {
            if (*v_get(length, j) > *v_get(length, j+1)) {
                // Swap length in vector
                swap(v_get(length, j), v_get(length, j+1));
                
                // Swap vector in array
                Vector* temp = f->sf[j];
                f->sf[j] = f->sf[j+1];
                f->sf[j+1] = temp;

                swapped = 1;
            }
        }
        if (swapped == 0) {
            break;
        }
    }
    DestroyVector(length);
}

formula_t* InitFormule(char* f) {
    formula_t* formula = (formula_t*) malloc(sizeof(formula_t));

    if (!calculate_size_and_encode_f(formula, f)) { return NULL; };
    build_sf(formula);
    sort_sf_by_length(formula);

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

void f_print_val(dict_t* val) {
    for (int i = 0; i < val->size; i++) {
        for (int j = 0; j < val->keys[i]->length; j++) {
            printf("%c", f_table_i_c(*v_get(val->keys[i], j)));
        }
        printf(" : %d\n", val->values[i]);
    }
}

dict_t* f_get_global_valuation(formula_t* f, dict_t* valuation) {
    dict_t* val_storage = CopyDict(valuation);

    int val; // used to stored valuation data of a formule

    Vector* sf; // sous formule étudiée

    Vector* sf_member1; // First member of the operator
    Vector* sf_member2; // Second member of the operator

    int val1; // store valuation data of member 1
    int val2; // store valuation data of member 2

    for (int i = 0; i <= f->sf_length; i++) {
        // Parcours les sous formules en incluant la formule globale
        if (i == f->sf_length) { sf = f->f; }
        else { sf = f->sf[i]; }

        // si sf est déjà dans la valuation passe au suivant
        if (d_contains(val_storage, sf)) { continue; }

        switch (get_largest_connector(sf))
        {
        case 0: // connecteur 'non' ('!')
            sf_member1 = v_get_btw(sf, 2, sf->length - 2); // get formula
            val = (d_get(val_storage, sf_member1)+1)%2; // turn 0 to 1 and 1 to 0

            d_put(val_storage, CopyVector(sf), val); // register new formula in dict

            // Free memory
            DestroyVector(sf_member1);
            break;
        
        case 1: // connecteur 'ou' ('|')
            sf_member1 = get_first_member(sf);
            sf_member2 = get_second_member(sf);

            val1 = d_get(val_storage, sf_member1);
            val2 = d_get(val_storage, sf_member2);

            val = val1 || val2;

            d_put(val_storage, CopyVector(sf), val);

            // Free memory
            DestroyVector(sf_member1);
            DestroyVector(sf_member2);
            break;
        case 2: // connecteur 'et' ('&')
            sf_member1 = get_first_member(sf);
            sf_member2 = get_second_member(sf);

            val1 = d_get(val_storage, sf_member1);
            val2 = d_get(val_storage, sf_member2);

            val = val1 && val2;

            d_put(val_storage, CopyVector(sf), val);

            // Free memory
            DestroyVector(sf_member1);
            DestroyVector(sf_member2);
            break;
        default:
            break;
        }
    }

    return val_storage;
}

formula_t* f_substitute(formula_t* f, Vector* old_f, Vector* new_f) {
    Vector* return_f = InitVector();
    int i = 0;
    while (i < f->f->length) {
        int match = 1;
        if (i + old_f->length <= f->f->length) {
            for (int j = 0; j < old_f->length; j++) {
                if (*v_get(f->f, i + j) != *v_get(old_f, j)) {
                    match = 0;
                    break;
                }
            }
        } else {
            match = 0;
        }

        if (match) {
            // Insert new_f instead of old_f
            for (int j = 0; j < new_f->length; j++) {
                v_append(return_f, *v_get(new_f, j));
            }
            i += old_f->length;
        } else {
            v_append(return_f, *v_get(f->f, i));
            i++;
        }
    }

    char* str_f = f_vec_to_str(return_f);
    DestroyVector(return_f);
    formula_t* final_return_f = InitFormule(str_f);
    free(str_f);

    return final_return_f;
}

void quine_minimization(formula_t* f) {
    /*
     - !(0) = 1
     - !(1) = 0
     - (1)|(f) = (f)|(1) = 1
     - (0)|(f) = (f)|(0) = f 
     - (1)&(f) = (f)&(1) = f
     - (0)&(f) = (f)&(0) = 0
    */
    
    Vector* temp_f; // Treated f

    int* conn_value;

    int start_i;
    int end_i;

    int open_p;
    int closed_p;

    while ((v_contain(f->f, 5) || v_contain(f->f, 6)) && f->f->length != 1) {
        temp_f = CopyVector(f->f);

        for (int i = 0; i < temp_f->length; i++) {
            switch (*v_get(temp_f, i))
            {
            case 5: // top
                conn_value = v_get(f->f, i-2);
                
                if (conn_value != NULL && *conn_value == 0) { // !(1) = 0
                    
                    v_set(temp_f, i-2, 6); // Replace not by bot
                    // shift everything to the left
                    for (int j = i+2; j < temp_f->length; j++) {
                        v_set(temp_f, j - 3, *v_get(temp_f, j));
                    }
                    temp_f->length = temp_f->length - 3;

                    break;
                }

                if (conn_value != NULL && *conn_value == 1) { // (f)|(1) = 1
                    end_i = i - 3;
                    open_p = 0;
                    closed_p = 0;
                    
                    for (int j = end_i; j >= 0; j--) {
                        switch (*v_get(temp_f, j))
                        {
                            case 3: open_p++; break;
                            case 4: closed_p++; break;
                            default: break;
                        } // compte le nombre de parenthèses ouvertes et fermées

                        if (open_p != closed_p) { continue; }
                        start_i = j;
                        break;
                    }

                    v_set(temp_f, start_i, 5);
                    for (int j = i + 2; j < temp_f->length; j++) {
                        v_set(temp_f, j - end_i + start_i - 4, *v_get(temp_f, j));
                    }
                    temp_f->length = temp_f->length - 4 - end_i + start_i;

                    break;
                }

                if (conn_value != NULL && *conn_value == 2) { // (f)&(1) = f
                    end_i = i - 3;
                    open_p = 0;
                    closed_p = 0;
                    
                    for (int j = end_i; j >= 0; j--) {
                        switch (*v_get(temp_f, j))
                        {
                            case 3: open_p++; break;
                            case 4: closed_p++; break;
                            default: break;
                        } // compte le nombre de parenthèses ouvertes et fermées

                        if (open_p != closed_p) { continue; }
                        start_i = j;
                        break;
                    }

                    for (int j = start_i + 1; j < temp_f->length; j++) {
                        if (j < end_i) {
                            v_set(temp_f, j - 1, *v_get(temp_f, j));
                        }
                        if (j > end_i + 5) {
                            v_set(temp_f, j - 5, *v_get(temp_f, j));
                        }
                    }
                    temp_f->length = temp_f->length - 6;

                    break;
                }
                
                conn_value = v_get(f->f, i+2);

                if (conn_value != NULL && *conn_value == 2) { // (1)&(f) = f
                    start_i = i + 3;
                    open_p = 0;
                    closed_p = 0;
                    
                    for (int j = start_i; j < temp_f->length; j++) {
                        switch (*v_get(temp_f, j))
                        {
                            case 3: open_p++; break;
                            case 4: closed_p++; break;
                            default: break;
                        } // compte le nombre de parenthèses ouvertes et fermées

                        if (open_p != closed_p) { continue; }
                        end_i = j;
                        break;
                    }

                    for (int j = start_i + 1; j < temp_f->length; j++) {
                        if (j < end_i) {
                            v_set(temp_f, j - 5, *v_get(temp_f, j));
                        }
                        if (j > end_i) {
                            v_set(temp_f, j - 6, *v_get(temp_f, j));
                        }
                    }
                    temp_f->length = temp_f->length - 6;

                    break;
                }
                
                if (conn_value != NULL && *conn_value == 1) { // (1)|(f) = 1
                    start_i = i + 3;
                    open_p = 0;
                    closed_p = 0;
                    
                    for (int j = start_i; j < temp_f->length; j++) {
                        switch (*v_get(temp_f, j))
                        {
                            case 3: open_p++; break;
                            case 4: closed_p++; break;
                            default: break;
                        } // compte le nombre de parenthèses ouvertes et fermées

                        if (open_p != closed_p) { continue; }
                        end_i = j;
                        break;
                    }

                    v_set(temp_f, i - 1, 5);
                    for (int j = end_i + 1; j < temp_f->length; j++) {
                        v_set(temp_f, j - end_i + i, *v_get(temp_f, j));
                    }
                    temp_f->length = temp_f->length - end_i + start_i - 4;

                    break;
                }

                break;
            case 6: // bot
                conn_value = v_get(f->f, i-2);

                if (conn_value != NULL && *conn_value == 0) { // !(0) = 1
                    
                    v_set(temp_f, i-2, 5); // Replace not by top
                    // shift everything to the left
                    for (int j = i+2; j < temp_f->length; j++) {
                        v_set(temp_f, j - 3, *v_get(temp_f, j));
                    }
                    temp_f->length = temp_f->length - 3;
                    
                    break;
                }

                if (conn_value != NULL && *conn_value == 2) { // (f)&(0) = 0
                    end_i = i - 3;
                    open_p = 0;
                    closed_p = 0;
                    
                    for (int j = end_i; j >= 0; j--) {
                        switch (*v_get(temp_f, j))
                        {
                            case 3: open_p++; break;
                            case 4: closed_p++; break;
                            default: break;
                        } // compte le nombre de parenthèses ouvertes et fermées

                        if (open_p != closed_p) { continue; }
                        start_i = j;
                        break;
                    }

                    v_set(temp_f, start_i, 6);
                    for (int j = i + 2; j < temp_f->length; j++) {
                        v_set(temp_f, j - end_i + start_i + 5, *v_get(temp_f, j));
                    }
                    temp_f->length = temp_f->length - 4 - end_i + start_i;

                    break;
                }

                if (conn_value != NULL && *conn_value == 1) { // (f)|(0) = f
                    end_i = i - 3;
                    open_p = 0;
                    closed_p = 0;
                    
                    for (int j = end_i; j >= 0; j--) {
                        switch (*v_get(temp_f, j))
                        {
                            case 3: open_p++; break;
                            case 4: closed_p++; break;
                            default: break;
                        } // compte le nombre de parenthèses ouvertes et fermées

                        if (open_p != closed_p) { continue; }
                        start_i = j;
                        break;
                    }

                    for (int j = start_i + 1; j < temp_f->length; j++) {
                        if (j < end_i) {
                            v_set(temp_f, j - 1, *v_get(temp_f, j));
                        }
                        if (j > end_i + 5) {
                            v_set(temp_f, j - 5, *v_get(temp_f, j));
                        }
                    }
                    temp_f->length = temp_f->length - 6;

                    break;
                }
                
                conn_value = v_get(f->f, i+2);

                if (conn_value != NULL && *conn_value == 1) { // (0)|(f) = f
                    start_i = i + 3;
                    open_p = 0;
                    closed_p = 0;
                    
                    for (int j = start_i; j < temp_f->length; j++) {
                        switch (*v_get(temp_f, j))
                        {
                            case 3: open_p++; break;
                            case 4: closed_p++; break;
                            default: break;
                        } // compte le nombre de parenthèses ouvertes et fermées

                        if (open_p != closed_p) { continue; }
                        end_i = j;
                        break;
                    }

                    for (int j = start_i + 1; j < temp_f->length; j++) {
                        if (j < end_i) {
                            v_set(temp_f, j - 5, *v_get(temp_f, j));
                        }
                        if (j > end_i) {
                            v_set(temp_f, j - 6, *v_get(temp_f, j));
                        }
                    }

                    temp_f->length = temp_f->length - 6;

                    break;
                }
                
                if (conn_value != NULL && *conn_value == 2) { // (0)&(f) = 0
                    start_i = i + 3;
                    open_p = 0;
                    closed_p = 0;
                    
                    for (int j = start_i; j < temp_f->length; j++) {
                        switch (*v_get(temp_f, j))
                        {
                            case 3: open_p++; break;
                            case 4: closed_p++; break;
                            default: break;
                        } // compte le nombre de parenthèses ouvertes et fermées

                        if (open_p != closed_p) { continue; }
                        end_i = j;
                        break;
                    }

                    v_set(temp_f, i - 1, 6);
                    for (int j = end_i + 1; j < temp_f->length; j++) {
                        v_set(temp_f, j - end_i + i, *v_get(temp_f, j));
                    }
                    temp_f->length = temp_f->length - end_i + start_i - 4;

                    break;
                }
                
                break;
            default: break;
            }
        }

        DestroyVector(f->f);
        f->f = CopyVector(temp_f);
        DestroyVector(temp_f);
    }
}
