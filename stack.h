#ifndef STACK_H
#define STACK_H

extern void* new_stack();

extern void stack_push(void* stack, void* data);

extern void* stack_pop(void* stack);

extern void* stack_peek(void* stack);

extern int stack_size(void* stack);

#endif
