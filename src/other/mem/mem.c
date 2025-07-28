#include <stdlib.h>

#define ALLOCBUFF_SIZE 20

/*
 * damn this is way above my caliber
 *
 * i'm starting to realize that i can't simply use 0s to initialize the allocbuff
 * as strings need those, therefore i won't even know if i have a valid space or not
 *
 * then i am thinking about whether i could use a seperate parallel array that
 * keeps track of where exactly in the buffer i have allocated memory to and is
 * in use, but that sucks, i'm wasting so much space just for something that uses
 * so little
 *
 * therefore, i could also use a header before the actual allocation of memory
 * where the header is n - 1 the actual pointer and the header allows me to know
 * how much space is being taken up, therefore skip ahead that amount and check if
 * there's another header before the required size amount for allocating memory
 *
 * i think that'll be a better choice as i know vectors do the same thing
 * where they keep a struct of all the details of the array, and if it gets too
 * big, it'll just allocate more space to itself somewhere available and copy over
 * its contents and free the previous location
 * */


static char allocbuff[ALLOCBUFF_SIZE];
static char *allocptr = allocbuff;

char *alloc();
void *afree();

char *printallocbuff() {
    printf("|")
    for (int *p = allocbuff; p < allocbuff + ALLOCBUFF_SIZE; p++) { 
        if (*p == '\0') {
            printf("\\0|");
        } else {
            printf("%c|", *p);
        }
    }
    printf("|");
}

/*
 * alloc doesn't account for the null termintor,
 * whenever running alloc, add 1 plus the string
 */

char *alloc(size_t size) {
    char *alloc = NULL;
    int *p = allocbuff;
    while ((p + size - allocbuff) < ALLOCBUFF_SIZE) {
        
        // search for available space
        while (*p++)
            ;

        // checking if empty patch has room
        // large enough for size
        bool is_valid_patch = true;
        int i = 0;
        for (; i < size; i++) {
            if (!*(p + i)) {
                is_valid_patch = false;
                break;
            }
        }

        if (!valid_batch) {
            p = p + i;
            continue;
        }

        *(p + i) = '\0';
        alloc = p;
        break;
    }

    return alloc;
}

void afree(char *p) {
    if (p < allocbuff || p > ALLOCBUFF_SIZE) {
        fprintf(stderr,
            "error: attempted to free a pointer not allocated by alloc\n");
        exit(1);
    }
    
    while (*p++)
        *p = 0;
    
    // null terminator
    *p = 0;
    
    return 0;
}
