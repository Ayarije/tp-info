#ifndef MUTABLE_INT_QUEUE_H
#define MUTABLE_INT_QUEUE_H

#include <stdlib.h>
#include <stdio.h>

#include "mutable_int_list.h"

struct mutable_queue_s {
  int* array;
  int array_size;

  int start;
  int size;
};

typedef struct mutable_queue_s* queue_t;

// Basic interface
queue_t queue_create_empty(int max_size);
int queue_is_empty(queue_t queue);
void queue_push(queue_t queue, int e);
int queue_pop(queue_t queue, int* out_e);
void free_queue(queue_t queue);

#endif
