#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "queue.h"

#define DEFAULT_LINE_NUM 10
#define BUFFER_LINE_SIZE 255

void get_last_nth_lines_to_queue(char **buffer, size_t *bufsize,
                                 FILE *file_stream, Queue *queue)
{   
    int rc;

    while(getline(buffer, bufsize, file_stream) != EOF) {
        if (is_full(queue))
            dequeue(queue);

        rc = enqueue(queue, strdup(*buffer));

        if (rc < 0) {
            fprintf(stderr, "error: overflow occured in queue\n");
            exit(1);
        }
    }
}

void print_circular_queue(Queue *queue)
{
    // for (int i = 0; i < (queue.back - queue.front - 1); i++)
    //     printf("%s", queue.data[i]);
    
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
    char *line_buffer;
    size_t bufsize = BUFFER_LINE_SIZE;
    int rc;

    line_buffer = (char *) malloc(bufsize * sizeof(char));
    
    if (line_buffer == NULL) {
        perror("malloc");
        exit(1);
    }

    /*
     *  queue is therefore holding 
     *  N pointers to the lines
     */
    Queue queue;
    rc = init_queue(&queue, DEFAULT_LINE_NUM);

    if (rc < 0) {
        perror("malloc");
        exit(1);
    }

    if (argc > 1)
        goto parse_files_in_args;
    /*
     * below up to the parse_files_in_args is
     * for dealing with stdin
     */

    goto exit;

    /*
     * this deals with when files were
     * given as arguments
     */
parse_files_in_args:
    for (char **p = argv + 1; p < argv + argc; p++) {
        FILE *file = fopen(*p, "r");

        if (file == NULL) {
            printf("tail: cannot open '%s' for reading\n", *p);
            continue;
        }

        /* print header for file arguments */
        if (argc > 2) printf("==> %s <==\n", *p);

        get_last_nth_lines_to_queue(&line_buffer, &bufsize,
                                    file, &queue);
        
        print_circular_queue(&queue);

        /* new line for file headers if more than one file */
        printf("%s", (argc > 2 && p < argv + argc - 1) ? "\n" : "");
        
        clear_queue(&queue);
        fclose(file);
    }

exit:
    free(line_buffer);
    return 0;
}
