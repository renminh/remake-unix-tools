#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "queue.h"

#define DEFAULT_LINE_NUM 10
#define BUFFER_LINE_SIZE 255

void tail_to_queue(char **buffer, size_t *bufsize,
                   FILE *file_stream, Queue *queue)
{   
    int rc;

    while(getline(buffer, bufsize, file_stream) != -1) {
        char *dup;

        if (is_full(queue)) {
            dup = dequeue(queue);
            free(dup);
        }
        
        dup = strdup(*buffer);
        
        if (dup == NULL) {
            perror("strdup");
            exit(1);
        }

        rc = enqueue(queue, dup);

        if (rc < 0) {
            fprintf(stderr, "error: overflow occured in queue\n");
            exit(1);
        }
    }
}

void print_tail(Queue *queue)
{
    int start = (queue->front + 1) % (queue->max_size);
    for (int i = 0; i < queue->max_size; i++) {
        int offset = (start + i) % queue->max_size;
        
        if (*(queue->data + offset) == NULL)
            break;

        printf("%s", *(queue->data + offset)); 
    }
}

int main(int argc, char **argv)
{
    Queue queue;
    char *line_buffer;
    size_t bufsize = BUFFER_LINE_SIZE;
    int rc;

    line_buffer = (char *) malloc(bufsize * sizeof(char));
    rc = init_queue(&queue, DEFAULT_LINE_NUM);

    if (line_buffer == NULL || rc < 0) {
        perror("malloc");
        exit(1);
    }

    char **p = argv + 1;
    do {
        FILE *file = (argc > 1) ? fopen(*p, "r") : stdin;

        if (file == NULL) {
            printf("tail: cannot open '%s' for reading\n", *p);
            continue;
        }

        if (argc > 2) printf("==> %s <==\n", *p);
        tail_to_queue(&line_buffer, &bufsize, file, &queue);
        print_tail(&queue);
        printf("%s", (argc > 2 && p < argv + argc - 1) ? "\n" : "");
        clear_queue(&queue);

        /* don't close stdin */
        if (argc == 1) continue;

        fclose(file);
        p++;
    } while (p < argv + argc);

    free(line_buffer);
    return 0;
}
