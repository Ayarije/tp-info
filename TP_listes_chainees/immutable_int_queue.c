#include "immutable_queue.h"

void _transfer_in_out(queue_t queue) {
  
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

queue_t queue_push(queue_t queue) {
  if (is_empty(queue->output)) {
    
  }
}
