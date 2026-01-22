#include "immutable_queue.h"

void _transfer_in_out(queue_t queue) {
  queue->output = list_rev(queue->input);
  queue->input = empty_list();
}

queue_t queue_create_empty() {
  queue_t queue = malloc(sizeof(struct cell_s));
  queue->input = empty_list();
  queue->output = empty_list();
  return queue;
}

int queue_is_empty(queue_t queue) {
  return is_empty(queue->input) && is_empty(queue->output);
}

queue_t queue_pop(queue_t queue, int* out_e) {
  if (is_empty(queue->output)) {
    _transfer_in_out(queue);
  }
  
  if (is_empty(queue->output)) {
    printf("[ERROR] Empty queue");
    return queue;
  }

  *out_e = list_head(queue->output);
  queue_t new_queue = queue_create_empty();
  new_queue->input = queue->input;
  new_queue->output = list_tail(queue->output);

  return new_queue;
}

queue_t queue_push(queue_t queue, int e) {
  queue_t new_queue = queue_create_empty();
  new_queue->input = cons(queue->input, e);
  new_queue->output = queue->output;
  return new_queue;
}
