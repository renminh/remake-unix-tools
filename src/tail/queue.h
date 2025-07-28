#ifndef QUEUE_H
#define QUEUE_H

/*
 * circular queue holds an array of pointers
 * the struct contains the index of the front
 * and back of the queue. they do not wrap around
 * however as they continously increase
 *
 * max_size indicates the number of elements
 * that can fit in the queue
 *
 * front points to one element before the one
 * that will be dequeued while back points to the
 * next empty available element
 */

typedef struct {
    char **data;
    int max_size;
    int front;
    int back;
} Queue;

#define FRONT_OFFSET 1
#define BACK_OFFSET 2

int get_offset(Queue *queue, int offset_flag);
int init_queue(Queue *queue, size_t size_of_queue);

bool is_empty(Queue *queue);
bool is_full(Queue *queue);

int enqueue(Queue *queue, char *s);
char *dequeue(Queue *queue);
char *peek(Queue *queue);

void clear_queue(Queue *queue);

#endif /* QUEUE_H */
