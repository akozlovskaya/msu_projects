#ifndef POLIZ_H_INCLUDED
#define POLIZ_H_INCLUDED

#include <stdio.h>
#include "stack.h"

typedef struct Poliz
{
	void *data;
    size_t cap;   // what size of memory is allocated
	size_t size;  // how much memory is actually occupied
} Poliz;

typedef char Size_elem;

typedef int (*Calculate_elem)(const void *elem, Size_elem size, Stack *stack);

int calculate_poliz(const Poliz *, void *, size_t);
int init_poliz(Poliz *poliz);
int new_elem_poliz(Poliz *poliz, void *item, Size_elem size, Calculate_elem *func);
int deinit_poliz(Poliz *poliz);


#endif