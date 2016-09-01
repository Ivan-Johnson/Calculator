#ifndef STACK_H
#define STACK_H


void* new_stack();

void push (void* stack, void* data);

void* pop (void* stack);

void* stack_peek (void* stack);

#endif
