#include <stdio.h>
#include <time.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#define NUM_SYSCALLS 1000

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

static inline uint64_t get_delta(const uint64_t start_s, const uint64_t end_s,
                   const uint64_t start_ns, const uint64_t end_ns) {
    return (end_s - start_s) * 1e9 + (end_ns - start_ns);
}

int main(int argc, char **argv) {
    struct timespec ts;
    uint64_t sum_ns = 0;

    printf("RUNNING %d SYSCALLS FOR BENCHMARKING:\n",
           NUM_SYSCALLS);
    
    for (int i = 0; i < NUM_SYSCALLS; i++) {
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
     * via ping pong back and forth some nth amount
     *
     */

    // todo, add ping pong for pipes
    // search how

    int fd[2];

    if (pipe(fd) < 0) {
        perror("pipe");
        exit(1);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        fprintf(stdin, "nothing\n");
    } else {
        uint64_t start_s, end_s;
        uint64_t start_ns, end_ns;

        clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
        start_s = (uint64_t) ts.tv_sec;
        start_ns = (uint64_t) ts.tv_nsec;

        pid_t rc_wait = wait(NULL);
        clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
        end_s = (uint) ts.tv_sec;
        end_ns = (uint) ts.tv_nsec;

        sum_ns = get_delta(start_s, end_s, start_ns, end_ns);
        
        printf("Average context switch time:\t%0.2fμs\n",
               ((float) sum_ns - avg_ns_syscalls) / 1000);
    }
    
    return 0;
}
