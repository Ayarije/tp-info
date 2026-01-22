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

int_list_t list_concatenate(int_list_t left_lst, int_list_t right_lst) {
  if (!is_empty(left_lst)) {
    return cons(list_concatenate(left_lst->tail, right_lst), left_lst->head);
  } else if (!is_empty(right_lst)) {
    return cons(list_concatenate(left_lst, right_lst->tail), right_lst->head);
  } else {
    return empty_list();
  }
}

int_list_t _list_rev(int_list_t list, int_list_t reversed_list) {
  if (is_empty(list)) return reversed_list;
  return _list_rev(list_tail(list), cons(reversed_list, list_head(list)));
}

int_list_t list_rev(int_list_t list) {
  return _list_rev(list, empty_list());
}
