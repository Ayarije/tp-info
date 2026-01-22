#ifndef IMMUTABLE_INT_LIST_H
#define IMMUTABLE_INT_LIST_H

#include <stdlib.h>
#include <stdio.h>

struct cell_s {
  int head;
  struct cell_s* tail;
};

typedef struct cell_s cell_t;
typedef cell_t* int_list_t;

// Basic interface
int_list_t empty_list();
int list_head(int_list_t list);
int_list_t list_tail(int_list_t list);
int_list_t cons(int_list_t list, int head);
int is_empty(int_list_t list);

// Advanced methods
int list_length(int_list_t list);
int is_in_list(int_list_t list, int e);
void print_list(int_list_t list);
int_list_t list_rev(int_list_t list);
int_list_t list_concatenate(int_list_t lst1, int_list_t lst2);

#endif
