#include <stdio.h>
#include <time.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#define NUM_SYSCALLS 1000
#define NUM_CONTEXT_SWITCHES 500

/*
 * oslat means os latency to 
 * benchmark syscalls & context switches
 */

/*
 * The purpose of this program is to
 * measure how long it takes for the OS
 * to do a system call as well as a
 * context switch
 */

static inline uint64_t get_delta(const uint64_t start_s, const uint64_t end_s,
                                 const uint64_t start_ns, const uint64_t end_ns) {
    return (end_s - start_s) * 1e9 + (end_ns - start_ns);
}

int main(int argc, char **argv) {
    struct timespec ts;
    uint64_t sum_ns = 0;

    printf(" - RUNNING %d SYSCALLS\n", NUM_SYSCALLS);
    printf(" - RUNNING %d CONTEXT_SWITCHES\n",
           NUM_CONTEXT_SWITCHES);


    for (int i = 0; i < NUM_SYSCALLS / 2; i++) {
        uint64_t start_s, end_s;
        uint64_t start_ns, end_ns;

        clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
        start_s = (uint64_t) ts.tv_sec;
        start_ns = (uint64_t) ts.tv_nsec;
        getpid();
        clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
        end_s = (uint64_t) ts.tv_sec;
        end_ns = (uint64_t) ts.tv_nsec;

        sum_ns += get_delta(start_s, end_s, start_ns, end_ns);
    }

    float avg_ns_syscalls = (float) sum_ns / NUM_SYSCALLS;

    printf("Average system call time:\t%0.2fns\n", avg_ns_syscalls);

    /*
     *
     * for context switching, we can use pipes
     * via ping pong some nth amount
     *
     */

    char *buffer;       // buffer to inherit by parent and child
    size_t bufsize = 1;

    buffer = (char *) malloc(bufsize * sizeof(char));

    if (buffer == NULL) {
        perror("malloc");
        exit(1);
    }

    int ptcfd[2];   // parent to child pipe (ptc)
    int ctpfd[2];   // child to parent pipe (ctp)

    if (pipe(ptcfd) < 0 || pipe(ctpfd)) {
        perror("pipe");
        exit(1);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {         // child
        close(ctpfd[0]);    // child to parent read is unused
        close(ptcfd[1]);    // parent to child write is unused

        for (int i = 0; i < NUM_CONTEXT_SWITCHES; i++) {
            write(ctpfd[1], "p", bufsize);
            read(ptcfd[0], buffer, bufsize);   // do nothing with it
        }

        free(buffer);
        close(ctpfd[1]);
        close(ptcfd[0]);

    } else {                // parent
        close(ptcfd[0]);    // parent to child read is unused
        close(ctpfd[1]);    // child to parent write is unused

        uint64_t start_s, end_s;
        uint64_t start_ns, end_ns;
        sum_ns = 0;
        
        for (int i = 0; i < NUM_CONTEXT_SWITCHES / 2; i++) {
            clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
            start_s = (uint64_t) ts.tv_sec;
            start_ns = (uint64_t) ts.tv_nsec;

            write(ptcfd[1], "p", bufsize);
            read(ctpfd[0], buffer, bufsize);   // do nothing with it

            clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
            end_s = (uint) ts.tv_sec;
            end_ns = (uint) ts.tv_nsec;

            sum_ns += get_delta(start_s, end_s, start_ns, end_ns);
        }

        free(buffer);
        close(ptcfd[1]);
        close(ctpfd[0]);

        float avg_ns_ctxsw = (sum_ns / NUM_CONTEXT_SWITCHES);

        printf("Average context switch time:\t%0.2fns\n",
                avg_ns_ctxsw);
       
        float perc_diff = ((avg_ns_ctxsw - avg_ns_syscalls) / avg_ns_syscalls) * 100;
        printf("\nSummary\n");
        printf(" - Context switching is %0.2f%% slower than syscalls\n",
               perc_diff);
    }

    return 0;
}
