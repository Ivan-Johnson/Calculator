#ifndef QUEUE_H
#define QUEUE_H

extern void* new_queue();

extern void queue_enqueue(void* queue, void* data);

extern void* queue_dequeue(void* queue);

extern void* queue_peek(void* queue);

extern int queue_size(void* queue);

#endif
