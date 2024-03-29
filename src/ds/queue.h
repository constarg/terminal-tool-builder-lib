#ifndef TOOL_BUILDER_QUEUE_H
#define TOOL_BUILDER_QUEUE_H

#include <tool_builder.h>
#include <memory.h>

// node data.
struct tb_queue_node_d
{
    struct tool_builder_args c_args;

    void (*c_callback)(const struct tool_builder_args *info);
};

// queue node.
struct tb_queue_node
{
    // data
    struct tb_queue_node_d data;
    // link to next node.
    struct tb_queue_node *next;
};

// queue.
struct tb_queue
{
    struct tb_queue_node *front; // the first element of queue.
    struct tb_queue_node *rear;  // the last element of queue.
};


static inline void tb_queue_init(struct tb_queue *queue)
{
    memset(queue, 0x0, sizeof(struct tb_queue));
}

static inline void tb_queue_destroy(struct tb_queue *queue)
{
    if (queue->front == NULL) return;
    struct tb_queue_node *tmp = queue->front->next;
    while (tmp) {
        free(queue->front->data.c_args.c_values);
        free(queue->front);
        queue->front = tmp;
        tmp = queue->front->next;
    }
    memset(queue, 0x0, sizeof(struct tb_queue));
}

static inline int tb_queue_is_empty(struct tb_queue *queue)
{
    return (queue->front == NULL) ? 0x1 : 0x0;
}

/**
 *	Put's an element in queue.
 *	@param src The data to put.
 *	@param queue The queue to put the element.
*/
extern void tb_queue_enqueue(struct tb_queue_node_d *src, struct tb_queue *queue);


/**
 *	Removes an element from queue.
 *	@param dst The destination where the last element of queue must be stored.
 *	@param queue The queue to remove the element.
*/
static inline void tb_queue_dequeue(struct tb_queue_node_d *dst, struct tb_queue *queue)
{
    if (queue->front == NULL) return;

    memcpy(dst, &queue->front->data, sizeof(struct tb_queue_node_d));
    struct tb_queue_node *removed = queue->front;
    queue->front = queue->front->next;

    free(removed);
}


#endif
