#include "mutable_int_queue.h"


queue_t queue_create_empty(int max_size) {
  queue_t queue = malloc(sizeof(struct mutable_queue_s));
  queue->array = malloc(sizeof(int * max_size));
  queue->array_size = max_size;

  queue->start = 0;
  queue->size = 0;
}

int queue_is_empty(queue_t queue) {
  return queue->size == 0;
}

void queue_push(queue_t queue, int e) {
  if (queue->size < queue->array_size) return;
  int index = (queue->start + queue->size)%queue->array_size;
  queue->array[index] = e;
  queue->size++;
}

int queue_pop(queue_t queue) {
  if (queue_is_empty(queue)) {
    printf("Empty queue returning -1");
    return -1;
  }
  int out_e = queue->array[queue->start];
  queue->start = (queue->start + 1)%queue->array_size;
  queue->size--;
  return out_e;
}

void free_queue(queue_t queue) {
  free(queue->array);
  free(queue);
}

