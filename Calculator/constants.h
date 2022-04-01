#ifndef CONST_H_INCLUDED
#define CONST_H_INCLUDED

#include <stdlib.h>

/* Из книги А.В.Столярова "Программирование. Введение в профессию(2)" */

enum expr_item_types 
{
    eit_int,  eit_dbl,   eit_var, 
    eit_plus, eit_minus, eit_mul, 
    eit_div,  eit_lp,    eit_pp, 
    eit_end
};


typedef struct Expression_item
{
    int type;
    union {
        long long i;
        double d;
        char var[sizeof(double)];
    } data;
} Expression_item;


typedef struct Variable_value
{
    char str[sizeof(double)];
    int type;
    union {
        long long i;
        double d;
    } data;
} Variable_value;


typedef struct Var_table
{
    size_t size;
    Variable_value *data;
    
} Var_table;

enum Errors
{
    STACK_INIT_ERR = 1,   MEM_ERR,    NO_CONST_ERR,
    EMPTY_STACK_ERR,      LP_ERR,     DIV_ZERO_ERR, 
    DATA_SIZE_ERR,        PP_ERR,     LITER_ERR, 
    VAR_NOT_FOUND_ERR,    MIS_OPRND,  MIS_OPRTN
};

enum {VAR_NUM = 5};


#endif