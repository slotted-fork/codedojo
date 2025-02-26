#include <assert.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct queue_t {
     uint32_t *items;
     size_t capacity;
     size_t head;
     size_t tail;
};

int queue_init(struct queue_t *queue, size_t size);
int queue_deinit(struct queue_t *queue);
int queue_enqueue(struct queue_t *queue, uint32_t value);
int queue_dequeue(struct queue_t *queue, uint32_t *value);

int main()
{
     const size_t queue_capacity = 1024;

     struct queue_t queue;
     queue_init(&queue, queue_capacity);

     size_t count = 0;
     while(!queue_enqueue(&queue, (uint32_t)count)) ++count;
     assert(count == 1024 && "queue is completely filled");
     assert(queue_enqueue(&queue, 55) && "queue does not allow extra enqueue");

     uint32_t value;
     while(!queue_dequeue(&queue, &value)) ++count;
     assert(count == 2048 && "queue is completely empty");
     assert(value == 1023 && "last queue element is 1022");
     assert(queue_dequeue(&queue, &value) && "quese does not allow extra dequeue");

     while(!queue_enqueue(&queue, (uint32_t)count)) ++count;
     assert(count == 3072 && "queue is completely filled");

     while(!queue_dequeue(&queue, &value)) ++count;
     assert(count == 4096 && "queue is completely empty");

     queue_deinit(&queue);

     return 0;
}

int queue_init(struct queue_t *queue, size_t size)
{
     assert(queue != NULL && "queue: must be allocated");

     queue->head = 0;
     queue->tail = 0;
     queue->capacity = size + 1; /* differenciate between full and empty using an extra slot */
     queue->items = malloc(queue->capacity * sizeof(*queue->items));
     if (queue->items == NULL) return 1;

     assert(queue->capacity == size + 1 && "queue: capacity must be set to size+1");
     assert(queue->items != NULL && "queue: items must be allocated");
     assert(queue->head == 0 && "queue: head must be set to 0");
     assert(queue->tail == 0 && "queue: tail must be set to 0");

     return 0;
}

int queue_deinit(struct queue_t *queue) {
     assert(queue != NULL && "queue: must be allocated");
     assert(queue->items != NULL && "queue: items must be allocated");

     free(queue->items);
     queue->items = NULL;
     queue->head = 0;
     queue->tail = 0;
     queue->capacity = 0;

     assert(queue->items == NULL && "queue: items must not be allocated");
     assert(queue->capacity == 0 && "queue: capacity must be set to 0");
     assert(queue->head == 0 && "queue: head must be set to 0");
     assert(queue->tail == 0 && "queue: tail must be set to 0");

     return 0;
}

int queue_enqueue(struct queue_t *queue, uint32_t value)
{
     assert(queue != NULL && "queue: must be allocated");
     assert(queue->items != NULL && "queue: items must be allocated");
     assert(queue->head <= queue->capacity && "queue: head must be within capacity");
     assert(queue->tail <= queue->capacity && "queue: tail must be within capacity");

     size_t queue_size = (queue->head - queue->tail + queue->capacity) % queue->capacity;

     /* queue is full */
     if (((queue->head + 1) % queue->capacity) == queue->tail) return 1;

     queue->head = (queue->head + 1) % queue->capacity;
     queue->items[queue->head] = value;

     assert(queue->head <= queue->capacity && "queue: head must be within capacity");
     assert(queue->tail <= queue->capacity && "queue: tail must be within capacity");
     assert((queue->head - queue->tail + queue->capacity) % queue->capacity == queue_size + 1
	    && "queue: queue size must have grown by one");

     return 0;
}

int queue_dequeue(struct queue_t *queue, uint32_t *value)
{
     assert(queue != NULL && "queue: must be allocated");
     assert(queue->items != NULL && "queue: items must be allocated");
     assert(queue->head <= queue->capacity && "queue: head must be within capacity");
     assert(queue->tail <= queue->capacity && "queue: tail must be within capacity");

     size_t queue_size = (queue->head - queue->tail + queue->capacity) % queue->capacity;

     /* queue is empty */
     if (queue->tail == queue->head) return 1;

     queue->tail = (queue->tail + 1) % queue->capacity;
     *value = queue->items[queue->tail];

     assert(queue->head <= queue->capacity && "queue: head must be within capacity");
     assert(queue->tail <= queue->capacity && "queue: tail must be within capacity");
     assert((queue->head - queue->tail + queue->capacity) % queue->capacity == queue_size - 1
	    && "queue: queue size must have shrunk by one");

     return 0;
}
