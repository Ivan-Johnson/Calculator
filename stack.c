#include "linked_list.h"
#include "stack.h"

void* new_stack(){
  return new_linked_list();
}

void push (void* stack, void* data){
  insertFront(stack, data);
}

void* stack_peek (void* stack){
  return get(stack, 0);
}

void* pop (void* quenue){
  return removeFront(quenue);
}
