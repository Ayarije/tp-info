#include <stdio.h>
#include <stdlib.h>

#include "array.h"

int MEM_SIZE = 1024;

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