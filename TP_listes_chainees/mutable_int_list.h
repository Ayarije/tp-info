#ifndef MUTABLE_INT_LIST_H
#define MUTABLE_INT_LIST_H

#include <stdbool.h>
struct cell_s {
    int head;
    struct cell_s* tail;
};
typedef struct cell_s cell_t;
typedef cell_t* int_list_t;

int_list_t create_empty_list();
bool is_empty(int_list_t list);
void cons(int head, int_list_t* list_ptr);
int list_head(int_list_t list);
int_list_t list_tail(int_list_t list);
void free_int_list(int_list_t list);
void print_int_list(int_list_t list);

#endif
