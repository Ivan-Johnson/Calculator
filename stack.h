#ifndef STACK_H
#define STACK_H

void* new_stack();

void stack_push(void* stack, void* data);

void* stack_pop(void* stack);

void* stack_peek(void* stack);

int stack_size(void* stack);

#endif
