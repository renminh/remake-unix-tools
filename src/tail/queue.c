#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

/*
 * my life would've been way easier if i had just
 * wrapped the index instead of checking the range
 * between front and back...
 */

int init_queue(Queue *queue, size_t size_of_queue)
{
    char *temp;
    queue->data = (char **) malloc(size_of_queue * sizeof(temp));

    if (queue->data == NULL)
        return -1;

    queue->front = -1;
    queue->back = 0;
    queue->max_size = size_of_queue; 

    return 0;
}

/* 
 * indexing through the array will be done here to handle
 * front and back constantly changing or increasing,
 * which will end up surpassing the bounds of the
 * original max_size but getting the difference
 * will solve that issue
 *
 * the queue will continue to keep the old remnants in the heap,
 * but since we are converting this queue to a circular queue
 * we can ignore it via the front and back pointers
 */
int get_offset(Queue *queue, int offset_flag)
{
    int offset;
    switch (offset_flag) {
        case FRONT_OFFSET:
            offset = queue->front;
            break;
        case BACK_OFFSET: 
            offset = queue->back;
            break;
        default:
            offset = queue->front;
            break;
    }

    return offset % (queue->max_size);
}

bool is_empty(Queue *queue)
{
    return (queue->front == queue->back - 1) ? true : false;
}

bool is_full(Queue *queue)
{
    int running_size = queue->back - queue->front - 1;
    return (running_size == queue->max_size) ? true : false;
}

int enqueue(Queue *queue, char *s)
{
    if (is_full(queue))
        return -1;

    *(queue->data + get_offset(queue, BACK_OFFSET)) = s;
    queue->back += 1;

    return 0;
}

void clear_queue(Queue *queue)
{
    for (int i = 0; i < queue->max_size; i++)
        queue->data[i] = NULL;

    queue->front = -1;
    queue->back = 0;
}

char *dequeue(Queue *queue)
{
    if (is_empty(queue))
        return NULL;
    
    queue->front += 1;
    return *(queue->data + get_offset(queue, FRONT_OFFSET));
}

char *peek(Queue *queue)
{
    if (is_empty(queue))
        return NULL;

    return *(queue->data + get_offset(queue, FRONT_OFFSET));
}
