#include <stdio.h>
#include <stdlib.h>
#include "include/stack.h"

#define NUMBER '0'

int getop(const char ch) {
    if (ch >= '0' && ch <= '9')
        return NUMBER;

    return ch;
}

float do_op(const float a, const float b, const char op) {
    float res;
    switch (op) {
        case '+':   res = a + b;
                    break;
        case '-':   res = a - b;
                    break;
        case '*':   res = a * b;
                    break;
        case '/':   if (b == 0) {
                        fprintf(stderr, "error: divide by zero\n");
                        exit(1);
                    }
                    res = a / b;
                    break;
        default:    fprintf(stderr, "Error in do_op()\n");
                    exit(1);
    }

    return res;
}

void log_stack_info(Stack stack) {
    printf("stack size:\t%d\n", size(&stack));
    printf("stack pointer:\t%d\n", stack.sp);
    printf("stack is empty:\t%s\n", (is_empty(&stack)) ? "true" : "false");
}

int main(int argc, char *argv[]) {
    Stack stack;
    stack.sp = 0;
    int ch, first_op, second_op;

    printf("Enter a reverse polish notation expression: ");
    while ((ch = getchar()) != '\n') {
        int type = getop(ch);

        switch (type) {
            case NUMBER:
                push(&stack, (float) (ch - '0'));
                if (push < 0) {
                    fprintf(stderr, "error: stack overflow\n");
                    exit(1);
                }
                break;
            case '+': case '-': case '*': case '/':
                if (is_empty(&stack) || (size(&stack) - 1 < 0)) {
                    fprintf(stderr, "error: stack underflow\n");
                    exit(1);
                }
                second_op = pop(&stack);
                first_op = pop(&stack);
                push(&stack, do_op(first_op, second_op, ch));
                break;
            case ' ': break;
            default:
                fprintf(stderr, "Invalid symbol detected: %c cannot be used", ch);
                exit(1);
        }
    }

    if (stack.sp != 1) {
        // log_stack_info(stack);
        fprintf(stderr, "error: incomplete or broken expression\n");
        exit(1);
    }

    printf("final value: %0.2f\n", pop(&stack));

    return 0;
}
