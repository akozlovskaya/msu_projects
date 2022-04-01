#include "calc.h"

#define SAFE_CALC(call) do { \
					flag = call; \
					if (flag != 0) { \
						return flag; \
					} \
				}\
				while (0)
                    
extern Var_table variables;

int
calc_const(const void *elem, Size_elem size, Stack *stack)
{   
    int flag;
    SAFE_CALC(push_stack(stack, elem, sizeof(Expression_item)));
    return 0;
}

int
calc_var(const void *elem, Size_elem size, Stack *stack)
{
    Expression_item item;
    void *p;
    p = &item;
    memcpy(&item, elem, sizeof(Expression_item));
    int flag;
    for (int i = 0; i < variables.size - 1; ++i) {
        if (strcmp(variables.data[i].str, item.data.var) == 0)
        {
            item.type = variables.data[i].type;
            if (item.type == eit_int) item.data.i = variables.data[i].data.i;
            if (item.type == eit_dbl) item.data.d = variables.data[i].data.d;
            SAFE_CALC(push_stack(stack, p, sizeof(Expression_item)));
            return 0;
        }
    }
    return NO_CONST_ERR;
}

int
calc_plus(const void *elem, Size_elem size, Stack *stack)
{
    Expression_item a, b, c;
    int flag;
    SAFE_CALC(pop_stack(stack, &a, sizeof(Expression_item)));
    SAFE_CALC(pop_stack(stack, &b, sizeof(Expression_item)));
    if (a.type == eit_int && b.type == eit_int) {
        c.type = eit_int;
        c.data.i = a.data.i + b.data.i;
    } else {
        c.type = eit_dbl;
        if (a.type == eit_dbl && b.type == eit_dbl) c.data.d = a.data.d + b.data.d;
        if (a.type == eit_int && b.type == eit_dbl) c.data.d = a.data.d + b.data.d;
        if (a.type == eit_dbl && b.type == eit_int) c.data.d = a.data.d + b.data.i;
    }
    
    SAFE_CALC(push_stack(stack, &c, sizeof(Expression_item)));
    return 0;
}

int
calc_minus(const void *elem, Size_elem size, Stack *stack)
{
    Expression_item a, b, c;
    int flag;
    SAFE_CALC(pop_stack(stack, &a, sizeof(Expression_item)));
    SAFE_CALC(pop_stack(stack, &b, sizeof(Expression_item)));
    
    if (a.type == eit_int && b.type == eit_int) {
        c.type = eit_int;
        c.data.i = b.data.i - a.data.i;
    } else {
        c.type = eit_dbl;
        if (a.type == eit_dbl && b.type == eit_dbl) c.data.d = b.data.d - a.data.d;
        if (a.type == eit_int && b.type == eit_dbl) c.data.d = b.data.d - a.data.i;
        if (a.type == eit_dbl && b.type == eit_int) c.data.d = b.data.i - a.data.d;
    }
    SAFE_CALC(push_stack(stack, &c, sizeof(Expression_item)));
    return 0;
}

int
calc_mul(const void *elem, Size_elem size, Stack *stack)
{
    Expression_item a, b, c;
    int flag;
    SAFE_CALC(pop_stack(stack, &a, sizeof(Expression_item)));
    SAFE_CALC(pop_stack(stack, &b, sizeof(Expression_item)));
    
    if (a.type == eit_int && b.type == eit_int) {
        c.type = eit_int;
        c.data.i = a.data.i * b.data.i;
    } else {
        c.type = eit_dbl;
        if (a.type == eit_dbl && b.type == eit_dbl) c.data.d = a.data.d * b.data.d;
        if (a.type == eit_int && b.type == eit_dbl) c.data.d = a.data.i * b.data.d;
        if (a.type == eit_dbl && b.type == eit_int) c.data.d = a.data.d * b.data.i;
    }
    SAFE_CALC(push_stack(stack, &c, sizeof(Expression_item)));
    return 0;
}

int
calc_div(const void *elem, Size_elem size, Stack *stack)
{
    Expression_item a, b, c;
    int flag;
    SAFE_CALC(pop_stack(stack, &a, sizeof(Expression_item)));
    SAFE_CALC(pop_stack(stack, &b, sizeof(Expression_item)));
    
    if (a.data.i == 0) {
        return DIV_ZERO_ERR;
    }
    c.type = eit_dbl;
    if (a.type == eit_int && b.type == eit_int) c.data.d = (double)b.data.i / a.data.i;
    if (a.type == eit_dbl && b.type == eit_dbl) c.data.d = b.data.d / a.data.d;
    if (a.type == eit_int && b.type == eit_dbl) c.data.d = b.data.d / a.data.i;
    if (a.type == eit_dbl && b.type == eit_int) c.data.d = b.data.i / a.data.d;

    SAFE_CALC(push_stack(stack, &c, sizeof(Expression_item)));
    return 0;
}