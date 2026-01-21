#include <stdio.h>
#include "mutable_int_list.h"


int main() {
    int_list_t l_1 = create_empty_list();
    for(int i=0; i<10; i=i+1){
        cons(i, &l_1);
    }
    int h = list_head(l_1);
    printf("h = %d\n", h);
    free_int_list(l_1);
    return 0;
}