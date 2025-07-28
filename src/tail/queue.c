#include <stdlib.h>
#include "queue.h"

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

        // this is fine for the first time we dequeue
        // but on the second time, because we aren't actually moving everything down
        // it'll continue to remain on the old element
        case FRONT_OFFSET:
            offset = (queue->back - queue->front) \
                - (queue->back-queue->front);
            break;
        case BACK_OFFSET: 
            offset = queue->back + queue->front + 1;
            break;
        default:
            offset = (queue->back - queue->front) \
                - (queue->back-queue->front);
            break;
    }

    return offset;
}

bool is_empty(Queue *queue)
{
    return (queue->front == queue->back - 1) ? true : false;
}

bool is_full(Queue *queue)
{
    // issue with the math here
    // for example if there is 1 element,
    // running size will be 2 instead of 1
    // b = 1 - f = -1 == 2
    int running_size = queue->back - queue->front;
    return (running_size > queue->max_size) ? true : false;
}

int enqueue(Queue *queue, char *s)
{
    if (is_full(queue))
        return -1;

    *(queue->data + get_offset(queue, BACK_OFFSET)) = s;
    queue->back += 1;

    return 0;
}

char *dequeue(Queue *queue)
{
    if (is_empty(queue))
        return NULL;
    
    /*
     * we aren't actually dequeing anything in the real
     * queue unless we wrap around but even so
     * the queue contains the old code(this isn't a stack. we're using the heap)
     */

    queue->front += 1;
    return *(queue->data + get_offset(queue, FRONT_OFFSET));
}

char *peek(Queue *queue)
{
    if (is_empty(queue))
        return NULL;

    return *(queue->data + get_offset(queue, FRONT_OFFSET));
}
