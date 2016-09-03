#ifndef QUEUE_H
#define QUEUE_H

void* new_queue();

void queue_enqueue(void* queue, void* data);

void* queue_dequeue(void* queue);

void* queue_peek(void* queue);

int queue_size(void* queue);

#endif
