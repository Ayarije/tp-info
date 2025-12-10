#include <stdio.h>
#include <stdlib.h>

#include "array.h"
#include "dict.h"

int MEM_SIZE = 1024;
int MAX_ARGS = 4;
int MAX_ARGS_SIZE = 50;

array_t* memory;
int cursor;
int running;

int read_next(int* out) {
    cursor += 1;

    if (!a_get(memory, cursor, out)) {
        printf("\nError: memory overflow from read next at %d\n", cursor);
        running = 0;
        return 0;
    }
    return 1;
}

int read_index(int index, int* out) {
    if (!a_get(memory, index, out)) {
        printf("\nError: memory overflow from read index at %d\n", index);
        running = 0;
        return 0;
    }
    return 1;
}

int read_value_at_next(int* out) {
    int addr = 0;
    if (!read_next(&addr)) return 0;
    if (!read_index(addr, out)) return 0;
    return 1;
}

void operation(int (*op)(int, int)) {
    int a = 0;
    int b = 0;
    int c_addr = 0;

    if (!read_value_at_next(&a) || !read_value_at_next(&b) || !read_next(&c_addr)) return;

    int result = op(a, b);
    a_set(memory, c_addr, &result);
}

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }

void copy() {
    int a = 0;
    int b_addr = 0;

    if (!read_value_at_next(&a) || !read_next(&b_addr)) return;

    a_set(memory, b_addr, &a);
}

void conditional_jump() {
    int condition = 0;
    int target = 0;

    if (!read_value_at_next(&condition) || !read_next(&target) || condition < 0) return;

    cursor = target;
}

void read_int() {
    int addr = 0;
    int val = 0;
    scanf("%d", &val);

    if (!read_next(&addr)) return;

    a_set(memory, addr, &val);
}

void print_int() {
    int val = 0;
    if (!read_value_at_next(&val)) {
        return;
    }

    printf("%d\n", val);
}

int convert_int_to_ascii(int i, char* out) {
    if (i < 0 || i > 127) {
        printf("\nError: Unknown char %d", i);
        running = 0;
        return 0;
    }

    *out = (char) i;
    return 1;
}

void print_char() {
    int char_id = 0;
    if (!read_value_at_next(&char_id)) return;

    char c = '\0';
    if (!convert_int_to_ascii(char_id, &c)) return;

    printf("%c", c);
}

void free_str(void* e_ptr) {
    free(*((char**) e_ptr));
}

int load_instruction(array_t* t_mem, dict_t* vars, char** instructions) {
    char* code = instructions[0];
    if (strcmp(code, "END_FLAG") == 0) return;

    if (strcmp(code, "SET") == 0) {
        char* id = instructions[1];
        int value;
        sscanf(instructions[2], "%d", &value);

        if (!d_put(vars, &id, &value)) d_set(vars, &id, &value);
    } else if (strcmp(code, "ADD")) {
        char* code = "1";
        a_push(t_mem, &code);
        a_push(t_mem, &instructions[1]);
        a_push(t_mem, &instructions[2]);
    } else if (strcmp(code, "MUL") == 0) {
        char* code = "2";
        a_push(t_mem, &code);
        a_push(t_mem, &instructions[1]);
        a_push(t_mem, &instructions[2]);
    } else if (strcmp(code, "SUB") == 0) {
        char* code = "3";
        a_push(t_mem, &code);
        a_push(t_mem, &instructions[1]);
        a_push(t_mem, &instructions[2]);
    } else if (strcmp(code, "CPY") == 0) {
        char* code = "4";
        a_push(t_mem, &code);
        a_push(t_mem, &instructions[1]);
        a_push(t_mem, &instructions[2]);
    } else if (strcmp(code, "JMP") == 0) {
        char* code = "5";
        a_push(t_mem, &code);
        a_push(t_mem, &instructions[1]);
        a_push(t_mem, &instructions[2]); // instruction n°x
    } else if (strcmp(code, "R_INT") == 0) {
        char* code = "6";
        a_push(t_mem, &code);
        a_push(t_mem, &instructions[1]);
    } else if (strcmp(code, "P_INT") == 0) {
        char* code = "7";
        a_push(t_mem, &code);
        a_push(t_mem, &instructions[1]);
    } else if (strcmp(code, "P_STR") == 0) {
        char* code = "8";
        for (int i = 0; i < strlen(instructions[1]); i++) {
            a_push(t_mem, &code);
            int c = (int) instructions[1][i];
            char* str = malloc(sizeof(char) * 10);
            sprintf(str, "%d", c);
            a_push(t_mem, &str);
        }
    }
}

int load_temp_memory(array_t* t_mem, dict_t* vars) {
    int mem_size = 2*t_mem->length + 2;

    for (int i = 0; i < t_mem->length; i++) {
        char* str_code;
        a_get(t_mem, i, &str_code);
        int code;
        
    }

    array_t* mem = InitZerosArray(sizeof(int), );
}

array_t* load_program(const char* filepath) {
    FILE* fptr = fopen(filepath, "r");
    if (fptr == NULL) {
        printf("\nError: the file doesn't exist at \"%s\". please check your filepath\n", filepath);
        return NULL;
    }
    array_t* temp_mem = InitEmptyArray(sizeof(char*));
    temp_mem->free_elem = &free_str;
    dict_t* var_registry = InitDict(sizeof(char*), sizeof(int));
    var_registry->keys->free_elem = &free_str;

    // Le maximum d'arguments + 1 pour l'instruction + 1 pour détecter la fin via END_FLAG
    char** instructions = malloc(sizeof(char*) * MAX_ARGS + 2);
    instructions[0] = malloc(sizeof(char) * (MAX_ARGS_SIZE + 1)); // +1 pour \0
    
    int cursor = 0;
    int args_cursor = 0;
    char l = fgetc(fptr);

    while (l != EOF) {
        if (l == ';') {
            instructions[args_cursor + 1] = "END_FLAG";
            load_instruction(temp_mem, var_registry, instructions);

            for (int i = 0; i <= MAX_ARGS; i++) {
                instructions[i] = malloc(sizeof(char) * (MAX_ARGS_SIZE + 1));
            }
            cursor = 0;
            args_cursor = 0;
            continue;
        } else if (l == ' ') {
            instructions[args_cursor][cursor] = '\0';

            args_cursor++;
            cursor = 0;
            continue;
        } else if ((int) l >= 32) {
            instructions[args_cursor][cursor] = l;
        }
        cursor++;
    }
}

array_t* load_memory(const char* filepath) {
    FILE* fptr = fopen(filepath, "r");
    if (fptr == NULL) {
        printf("\nError: the file doesn't exist at \"%s\". please check your filepath\n", filepath);
        return NULL;
    }
    array_t* mem = InitEmptyArray(sizeof(int));

    char* current_id = malloc(sizeof(char) * 10);
    int cursor = 0;
    char l = fgetc(fptr);

    while (l != EOF) {
        if (l == '0' || l == '1' || l == '2' || l == '3' || l == '4' || l == '5' || l == '6' || l == '7' || l == '8' || l == '9') {
            current_id[cursor] = l;
            cursor++;
        } else if (l == ';') {
            current_id[cursor] = '\0';
            int val;
            sscanf(current_id, "%d", &val);
            a_push(mem, &val);

            free(current_id);
            current_id = malloc(sizeof(char) * 10);
            cursor = 0;
        }
        l = fgetc(fptr);
    }
    free(current_id);
    fclose(fptr);
    return mem;
}

int main(int argc, char** argv) {
    memory = load_memory(argv[1]);
    running = 1;
    cursor = 0;

    for (int i = 0; i < memory->length; i++) {
        int e;
        a_get(memory, i, &e);
        printf("%d ", e);
    }
    printf("\n");

    while (running) {
        int code;
        if (!a_get(memory, cursor, &code)) {
            printf("\nError: memory overflow from main at %d\n", cursor);
            running = 0;
            break;
        }

        switch (code)
        {
        case 0:
            running = 0;
            break;
        case 1:
            operation(&add);
            break;
        case 2:
            operation(&mul);
            break;
        case 3:
            operation(&sub);
            break;
        case 4:
            copy();
            break;
        case 5:
            conditional_jump();
            break;
        case 6:
            read_int();
            break;
        case 7:
            print_int();
            break;
        case 8:
            print_char();
            break;
        default:
            printf("Error: unknown operation %d. exiting...\n", code);
            running = 0;
        }
        cursor++;
    }
}