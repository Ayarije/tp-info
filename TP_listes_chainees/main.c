#include <stdio.h>

#include "immutable_int_list.h"


int main() {
  int_list_t lst1 = empty_list();
  int_list_t lst2 = cons(lst1, 0);
  int_list_t lst3 = cons(lst2, 1);
  int_list_t lst4 = list_tail(lst3);
  int_list_t lst5 = cons(lst4, 2);
  
  printf("%d\n", lst4 == lst2);

  printf("lst1 : "); print_list(lst1);
  printf("lst2 : "); print_list(lst2);
  printf("lst3 : "); print_list(lst3);
  printf("lst4 : "); print_list(lst4);
  printf("lst5 : "); print_list(lst5);
  
  int lst3_length = list_length(lst3);
  int is_2_in_lst3 = is_in_list(lst3, 2);
  int is_1_in_lst3 = is_in_list(lst3, 1);
  int_list_t lst3_and_lst5 = list_concatenate(lst3, lst5);

  printf("lst3 length = %d\nis 2 in lst3 = %d\nis 1 in lst3 = %d\n", lst3_length, is_2_in_lst3, is_1_in_lst3);
  printf("lst3_and_lst5 : "); print_list(lst3_and_lst5);
}
