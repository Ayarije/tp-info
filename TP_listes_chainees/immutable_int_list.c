#include "immutable_int_list.h"


int_list_t empty_list() {
  return NULL;
}

int_list_t cons(int_list_t list, int head) {
  int_list_t new_list = malloc(sizeof(cell_t));
  new_list->head = head;
  new_list->tail = list;
  return new_list;
}

int is_empty(int_list_t list) {
  return list == NULL;
}

int list_head(int_list_t list) {
  return list->head;
}

int_list_t list_tail(int_list_t list) {
  return list->tail;
}

int list_length(int_list_t list) {
  if (list == NULL) {
    return 0;
  }
  return 1 + list_length(list->tail);
}

int is_in_list(int_list_t list, int e) {
  if (list == NULL) {
    return 0;
  } else if (list->head == e) {
    return 1;
  } else {
    return is_in_list(list->tail, e);
  }
}

void print_list(int_list_t list) {
  if (list == NULL) {
    printf("\n");
    return;
  }
  printf("%d ", list->head);
  print_list(list->tail);
}

int_list_t list_reverse(int_list_t reversed, int_list_t lst) {
  if (lst == NULL) {
    return empty_list();
  }
  return list_reverse(cons(reversed, list_head(lst)), list_tail(lst));
}

int_list_t list_concatenate(int_list_t left_lst, int_list_t right_lst) {
  int_list_t out_lst = empty_lst();
  
  while (!is_empty(left_lst)) {
    cons()
  }
}
