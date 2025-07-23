#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

bool is_word_in_line(const char *line, const char *word) {
    size_t line_len = strlen(line);
    size_t word_len = strlen(word);
    int n = 0;
    bool found_word = false;

    if (line_len < word_len)
        return false;


    while (n < (line_len - word_len)) {
        
        // search if current window is the word we are on
        // starting from n to n + word_len
        // using j as the index to the word, and i as the index
        // to the line
        found_word = true;
        for (int i = n, j = 0; j < word_len; i++, j++) {
            if (line[i] != word[j]) {
                found_word = false;
                break;
            }
        }

        if (found_word)
            return true;
        
        // go to the next character and repeat
        n++;
    }

    return found_word;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "grep: searchterm [file ...]\n");
        exit(1);
    }
    
    char *word = strdup(argv[1]);
    char *buffer;
    size_t bufsize = 255;
    size_t characters;

    buffer = (char *) malloc(bufsize * sizeof(char));
    if (buffer == NULL) {
        perror("malloc");
        exit(1);
    }
    
    // if no file was given for the args
    if (argc == 2) {
        while ((characters = getline(&buffer, &bufsize, stdin)) != -1) {
            if (is_word_in_line(buffer, word))
                printf("%s", buffer);
        }
    }
    
    // open up all files from stdin if file was passed
    for (int i = 2; i < argc; i++) {
        FILE *file = fopen(argv[i], "r");
        
        if (file == NULL) {
            fprintf(stderr, "grep: %s: file does not exist\n", argv[i]);
            continue;
        }

        while ((characters = getline(&buffer, &bufsize, file)) != -1) {
            if (is_word_in_line(buffer, word))
                printf("%s", buffer);
        }
        
        fclose(file);
    }
    
    free(word);
    free(buffer);

    return 0;
}

