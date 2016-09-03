#include "linked_list.h"
#include "stack.h"

void* new_stack(){
  return new_linked_list();
}

void stack_push (void* stack, void* data){
  linked_list_insert_Front(stack, data);
}

void* stack_peek (void* stack){
  return linked_list_get(stack, 0);
}

void* stack_pop (void* quenue){
  return linked_list_remove_Front(quenue);
}

int stack_size (void* stack){
  return linked_list_size(stack);
}

