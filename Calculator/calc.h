#ifndef CALC_H_INCLUDED
#define CALC_H_INCLUDED

#include "poliz.h"
#include "stack.h"
#include "constants.h"
#include <string.h>

int calc_const(const void *elem, Size_elem size, Stack *stack);
int calc_var(const void *elem, Size_elem size, Stack *stack);
int calc_plus(const void *elem, Size_elem size, Stack *stack);
int calc_minus(const void *elem, Size_elem size, Stack *stack);
int calc_mul(const void *elem, Size_elem size, Stack *stack);
int calc_div(const void *elem, Size_elem size, Stack *stack);


#endif