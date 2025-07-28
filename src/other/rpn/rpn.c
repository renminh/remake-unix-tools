#include <stdio.h>
#include <stdlib.h>
#include "include/stack.h"

#define NUMBER '0'

/*
 *
 * Program is currently doing a very basic job on parsing
 * i.e, it cannot handle any number that is not single digit
 *
 * DOESN'T WORK X   -> 23 5 +
 *
 * WHAT WORKS       -> 2 3 +
 *
 */

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

int main(int argc, char *argv[]) {
    Stack stack;
    stack.sp = 0;
    int ch; 
    float first_op, second_op;

    printf("Enter a reverse polish notation expression: ");
    while ((ch = getchar()) != '\n') {
        int type = getop(ch);

        switch (type) {
            case NUMBER:
                if (push(&stack, (float) (ch - '0')) < 0) {
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
        fprintf(stderr, "error: incomplete or broken expression\n");
        exit(1);
    }

    printf("final value: %0.2f\n", pop(&stack));

    return 0;
}
