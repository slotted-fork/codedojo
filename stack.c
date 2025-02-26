#include <assert.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct stack_t {
     uint32_t *items;
     size_t capacity;
     size_t count;
};

int stack_init(struct stack_t *stack, size_t size);
int stack_deinit(struct stack_t *stack);
int stack_push(struct stack_t *stack, uint32_t value);
int stack_pop(struct stack_t *stack, uint32_t *value);

int main()
{
     const size_t stack_capacity = 1024;

     struct stack_t stack;
     stack_init(&stack, stack_capacity);

     size_t count = 0;
     while(!stack_push(&stack, (uint32_t)count)) ++count;
     assert(count == 1024 && "stack is full");
     assert(stack_push(&stack, 55) && "stack must not allow extra push");

     uint32_t value;
     while(!stack_pop(&stack, &value)) ++count;
     assert(count == 2048 && "stack is empty");
     assert(value == 0 && "last stack element is 0");
     assert(stack_pop(&stack, &value) && "stack must not allow extra pop");

     while(!stack_push(&stack, (uint32_t)count)) ++count;
     assert(count == 3072 && "stack is full");

     while(!stack_pop(&stack, &value)) ++count;
     assert(count == 4096 && "stack is empty");

     stack_deinit(&stack);

     return 0;
}

int stack_init(struct stack_t *stack, size_t size)
{
     assert(stack != NULL && "stack: must be allocated");

     stack->count = 0;
     stack->capacity = size;
     stack->items = malloc(stack->capacity * sizeof(*stack->items));
     if (stack->items == NULL) return 1;

     assert(stack->items != NULL && "stack: items must be allocated");
     assert(stack->capacity == size && "stack: capacity must be set to size");
     assert(stack->count == 0 && "stack: count must be set to 0");

     return 0;
}

int stack_deinit(struct stack_t *stack) {
     assert(stack != NULL && "stack: must be allocated");
     assert(stack->items != NULL && "stack: items must be allocated");

     free(stack->items);
     stack->items = NULL;
     stack->count = 0;
     stack->capacity = 0;

     assert(stack->items == NULL && "stack: items must not be allocated");
     assert(stack->capacity == 0 && "stack: capacity must be set to 0");
     assert(stack->count == 0 && "stack: count must be set to 0");

     return 0;
}

int stack_push(struct stack_t *stack, uint32_t value)
{
     assert(stack != NULL && "stack: must be allocated");
     assert(stack->items != NULL && "stack: items must be allocated");
     assert(stack->count <= stack->capacity && "stack: count must be within capacity");

     size_t stack_size = stack->count;

     /* stack is full */
     if (stack->count == stack->capacity) return 1;

     stack->items[stack->count] = value;
     stack->count++;

     assert(stack->count <= stack->capacity && "stack: count must be within capacity");
     assert(stack->count == stack_size + 1 && "stack: count must have grown by one");

     return 0;
}

int stack_pop(struct stack_t *stack, uint32_t *value)
{
     assert(stack != NULL && "stack: must be allocated");
     assert(stack->items != NULL && "stack: items must be allocated");
     assert(stack->count <= stack->capacity && "stack: count must be within capacity");

     size_t stack_size = stack->count;

     /* stack is empty */
     if (stack->count == 0) return 1;

     stack->count--;
     *value = stack->items[stack->count];

     assert(stack->count <= stack->capacity && "stack: count must be within capacity");
     assert(stack->count == stack_size - 1 && "stack: count must have shrunk by one");

     return 0;
}
