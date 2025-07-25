#include "include/stack.h"

int push(Stack *stack, const float n) {
    if (size(stack) > STACK_MAX_SIZE)
        return -1;
    
    stack->data[stack->sp] = n;
    stack->sp += 1;

    return 0;
}

float pop(Stack *stack) {
    stack->sp -= 1;
    return stack->data[stack->sp];
}

float peek(Stack *stack) {
    return (is_empty(stack) || size(stack) > STACK_MAX_SIZE) ? -1 : stack->data[stack->sp - 1];
}

bool is_empty(Stack *stack) {
    return (stack > 0) ? false : true;
}

int size(Stack *stack) {
    return stack->sp;
}
