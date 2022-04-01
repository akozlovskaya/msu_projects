#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <string.h>
#include <stdlib.h>
#include "constants.h"

typedef struct Stack
{
    void *data;
    size_t size;
    
} Stack;

int init_stack(Stack *stack);
int push_stack (Stack *stack, const void *data, size_t size_data);
int pop_stack(Stack *stack, const void *resp, size_t size_res);
void final_stack(Stack *stack);

#endif