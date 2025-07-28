#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

#define DEFAULT_LINE_NUM 10
#define BUFFER_LINE_SIZE 255

int main(int argc, char **argv)
{
    char **queue;
    char *line_buffer;
    size_t bufsize = BUFFER_LINE_SIZE;
    size_t num_of_lines = DEFAULT_LINE_NUM;
    size_t characters;

    line_buffer = (char *) malloc(bufsize * sizeof(char));

    if (line_buffer == NULL) {
        perror("malloc");
        exit(1);
    }

    /*
     *  queue is therefore holding 
     *  N pointers to the lines
     */
    queue = (char **) malloc(num_of_lines * sizeof(char));

    if (queue == NULL) {
        perror("malloc");
        exit(1);
    }

    if (argc > 1)
        goto parse_files_in_args;
    /*
     * below up to the parse_files_in_args is
     * for dealing with stdin
     */
    printf("no args provided, goto exit\n");

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

        while((characters = getline(&line_buffer, &bufsize, file)) != EOF) {
            printf(line_buffer);
        }

        /* new line for file headers if more than one file */
        printf("%s", (argc > 2 && p < argv + argc - 1) ? "\n" : "");

        fclose(file);
    }

exit:
    return 0;
}
