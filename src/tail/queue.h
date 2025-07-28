#ifndef QUEUE_H
#define QUEUE_H

/*
 * queue will hold a list of pointers
 * that point to a string of characters.
 *
 * max_size indicates the number of elements
 * that can fit(allocated) to the queue
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
