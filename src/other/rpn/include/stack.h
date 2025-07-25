#ifndef STACK_H
#define STACK_H

#define STACK_MAX_SIZE 100

/*
 * Stack pointer points to the NEXT available space
 * not the current top-most item
 */

typedef struct {
    float data[STACK_MAX_SIZE];
    int sp;
} Stack;

int push(Stack *stack, const float e);
float pop(Stack *stack);
float peek(Stack *stack);
bool is_empty(Stack *stack);
int size(Stack *stack);

#endif

