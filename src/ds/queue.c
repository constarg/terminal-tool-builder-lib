#include <queue.h>

#include <malloc.h>


void tb_queue_enqueue(struct tb_queue_node_d *src, struct tb_queue *queue)
{
    if (queue->front == NULL) {
        queue->front = (struct tb_queue_node *) malloc(sizeof(struct tb_queue_node));
        memcpy(&queue->front->data, src, sizeof(struct tb_queue_node_d));
        queue->front->next = NULL;
        queue->rear = queue->front;
        return;
    }

    struct tb_queue_node *new_node = (struct tb_queue_node *)
            malloc(sizeof(struct tb_queue_node));
    memcpy(&new_node->data, src, sizeof(struct tb_queue_node_d));
    new_node->next = NULL;

    queue->rear->next = new_node;
    queue->rear = new_node;
}
