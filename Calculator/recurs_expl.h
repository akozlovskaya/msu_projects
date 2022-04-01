#ifndef RECURS_EXPL_H_INCLUDED
#define RECURS_EXPL_H_INCLUDED

#include "poliz.h"
#include "stack.h"
#include "constants.h"
#include "calc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct Help_struct
{
    int buf;
    int operand;
    int num_of_lp;
    int num_of_pp;
    
} Help_struct;
  
void print_elem(Expression_item a);
int rec_expl_next_char(Expression_item *cur_expr, Help_struct *ctrl);
int rec_expl_add_sub(Poliz *poliz, Expression_item *curc, Help_struct *ctrl);
int rec_expl_mul_div(Poliz *poliz, Expression_item *curc, Help_struct *ctrl);
int rec_expl_symbol(Poliz *poliz, Expression_item *curc, Help_struct *ctrl);

#endif