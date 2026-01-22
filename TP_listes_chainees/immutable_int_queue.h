#ifndef IMMUTABLE_INT_QUEUE_H
#define IMMUTABLE_INT_QUEUE_H

#include <stdlib.h>
#include <stdio.h>

#include "immutable_int_list.h"

struct immutable_queue_s {
  int_list_t input;
  int_list_t output;
};

typedef struct immutable_queue_s* queue_t;

// Basic interface
queue_t queue_create_empty();
int queue_is_empty(queue_t queue);
queue_t queue_push(queue_t queue, int e);
queue_t queue_pop(queue_t queue, int* out_e);

#endif
