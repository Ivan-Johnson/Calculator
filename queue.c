#include "linked_list.h"
#include "queue.h"

void* new_queue() {
    return new_linked_list();
}

void queue_enqueue(void* quenue, void* data) {
    linked_list_insert_Front(quenue, data);
}

void* queue_dequeue(void* quenue) {
    return linked_list_remove_back(quenue);
}

void* queue_peek(void* queue) {
    return linked_list_get_back(queue);
}

int queue_size(void* queue) {
    return linked_list_size(queue);
}
