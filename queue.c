#include "linked_list.h"
#include "queue.h"

void* new_queue(){
  return new_linked_list();
}

void enqueue (void* quenue, void* data){
  insertFront(quenue, data);
}

void* dequeue (void* quenue){
  return removeBack(quenue);
}

void* queue_peek (void* queue){
  return getBack(queue);
}
