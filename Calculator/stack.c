#include "stack.h"

int
init_stack(Stack *st)
{
    st->size = 0;
    st->data = NULL;
    return 0;   
}

int
push_stack(Stack *stack, const void *data, size_t size_data)
{
    size_t prev_size = stack->size;
    stack->size += size_data;
    if ((stack->data = realloc(stack->data, stack->size)) == NULL) {
        return MEM_ERR;
    }
    for (int i = 0; i < size_data; ++i) {
        ((char *)stack->data)[i + prev_size] = ((char *)data)[i];
    }
    return 0;
}
int
pop_stack(Stack *stack, const void *resp, size_t size_res)
{
    if (stack->size < 1) return EMPTY_STACK_ERR;
    if (stack->size < size_res) return DATA_SIZE_ERR;
    stack->size -= size_res;
    for (int i = 0; i < size_res; ++i) {
        ((char *)resp)[i] = ((char *)stack->data)[i + stack->size];
    }    
    if ((stack->data = realloc(stack->data, stack->size)) == NULL) {
        if (stack->size) return MEM_ERR;
    }
    return 0;
}

void
final_stack(Stack *stack)
{
    free(stack->data);
    stack->size = 0;
    
}