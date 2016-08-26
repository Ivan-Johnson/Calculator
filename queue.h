#ifndef QUEUE_H
#define QUEUE_H

void* new_queue();

void enqueue (void* queue, void* data);

void* dequeue (void* queue);

#endif
