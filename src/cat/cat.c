#include <stdio.h>
#include <stdlib.h>

#define PROGRAM_NAME "cat"

int main(int argc, char *argv[]) {
    int error = 0;
    int ch;

    if (argc > 1) {
        
        for (int i = 1; i < argc; i++) {
            FILE *fptr = fopen(argv[i], "r");

            if (fptr == NULL) {
                error = 1;
                fprintf(stderr,
                       "%s: %s: No such file or directory\n",
                       PROGRAM_NAME, argv[i]);
            } else {
                while ((ch = fgetc(fptr)) != EOF)
                    fputc(ch, stdout);
                fclose(fptr);
            }

        }
    } else { // user enters text through standard input
        while ((ch = getchar()) != EOF)
            putchar(ch);
    }

    if (!error)
        return EXIT_SUCCESS;
    else
        return EXIT_FAILURE;
}
