#include <stdio.h>
#include <time.h>
#include <unistd.h>

/*
 * oslat means os latency for 
 * benchmark syscalls & context switches
 */

/*
 * The purpose of this program is to
 * measure how long it takes for the OS
 * to do a system call as well as a
 * context switch
 */

int main(int argc, char **argv) {
    struct timespec ts;
    char buff[0];
    size_t nbyte = 0;
    
    size_t sum_nsec = 0;
    int num_syscalls;

    printf("How many syscalls do you wish to run: ");
    scanf("%d", &num_syscalls);
    
    for (int i = 0; i < num_syscalls; i++) {
        size_t start_nsec, end_nsec;

        clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
        start_nsec = (size_t) ts.tv_nsec;
        // read(STDIN_FILENO, buff, nbyte);
        getpid();
        clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
        end_nsec = (size_t) ts.tv_nsec;

        sum_nsec += end_nsec - start_nsec;
    }

    printf("average of %0.2f nanoseconds per %d syscalls\n",
           (float) sum_nsec / num_syscalls, num_syscalls);
    
    return 0;
}
