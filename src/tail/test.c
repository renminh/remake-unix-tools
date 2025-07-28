#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int main(int argc, char **argv)
{
    Queue queue;
    size_t size_of_queue = 5;
    int rc;

    if (init_queue(&queue, size_of_queue) < 0) {
        perror("malloc");
        exit(1);
    }

    char *string = "hello my name is";
    char *string_two = " john doe";

    rc = enqueue(&queue, string);
    if (rc < 0) {
        fprintf(stderr, "error: overflow in the queue\n");
        exit(1);
    }

    rc = enqueue(&queue, string_two);
    if (rc < 0) {
        fprintf(stderr, "error: overflow in the queue\n");
        exit(1);
    }

    printf("%s\n", dequeue(&queue));
    printf("%s\n", dequeue(&queue));

    return 0;
}
