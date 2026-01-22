#include <stdlib.h>
#include <stdio.h>
#include "mutable_int_list.h"

int_list_t create_empty_list(){
  return NULL;
}

bool is_empty(int_list_t list){
  return list == NULL;
}

void cons(int head, int_list_t* list_ptr){
  //Attention ici, cons prend un pointeur vers une liste,
  //c'est à dire un pointeur de pointeur de cellule
  //ce choix est fait afin de gérer le cas
  //où la liste en tête de laquelle on veut ajouter est vide,
  //dans ce cas, on ne peut pas modifier le contenu de la case en question,
  //il faut en allouer une nouvelle, et donc il faut un pointeur vers la liste pour la "modifier"
  //Et on peut se permettre de ne rien renvoyer puisqu'on agit par effet de bord
  //(modification du contenu de la case pointée par list_ptr)

  cell_t* new_cell_ptr = (cell_t*)malloc(sizeof(cell_t));
  if (*list_ptr == NULL){
    //Dans ce cas, la liste obtenue est la liste donc la tête est head, et la queue NULL
    new_cell_ptr->head = head;
    new_cell_ptr->tail = NULL;
    //On modifie le contenu de la case vers laquelle pointe le pointeur de liste.
    //pour en faire une liste, c'est à dire un pointeur vers une cellule.
    *list_ptr = new_cell_ptr;
  }
  else{
    //Dans le cas contaire, on copie la première cellule de la liste à compléter
    //dans le nouveau maillon
    new_cell_ptr->head = (*list_ptr)->head;
    new_cell_ptr->tail = (*list_ptr)->tail;
    //Puis on peut remplacer la tête par la nouvelle valeur en tête,
    //et la queue par un pointeur vers la copie
    (*list_ptr)->head = head;
    (*list_ptr)->tail= new_cell_ptr;
  }
}

int list_head(int_list_t list){
  if (list == NULL){
    printf("Empty list, returning 0 for the head\n");
    return 0;
  }
  else{
    return list->head;
  }
}

int_list_t list_tail(int_list_t list){
  if (list == NULL){
    printf("Empty list, returning NULL for the tail\n");
    return NULL;
  }
  else{
    return list->tail;
  }
}
void print_int_list(int_list_t list){
  printf("[");
  if(list !=NULL){
    printf("%d", list->head);
    list = list->tail;
  }
  while(list != NULL){
    printf(";%d", list->head);
    list = list->tail;
  }
  printf("]\n");
}

void free_int_list(int_list_t list) {
  if (is_empty(list)) {
    return;
  }
  free_int_list(list->tail);
  free(list);
  return;
}

int sum_list(int_list_t list) {
  
}
