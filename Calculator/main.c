#include "poliz.h"
#include "constants.h"
#include "stack.h"
#include "calc.h"
#include "recurs_expl.h"
#include "init_var.h"
#include <stdio.h>

#define SAFE_BEGIN(call) do { \
					f = call; \
					if (f != 0) { \
                        print_err (f);\
                        free(a);\
                        free(variables.data);\
                        deinit_poliz(&my_poliz);\
						return f; \
					} \
				}\
				while (0)

Var_table variables;

void print_err (int err)
{
    if (err == STACK_INIT_ERR) fputs("Stack initialization error!\n", stderr);
    if (err == MEM_ERR) fputs("Memory allocation or resizing error!\n", stderr);
    if (err == EMPTY_STACK_ERR) fputs("Empty stack!\n", stderr);
    if (err == DATA_SIZE_ERR) fputs("The data size is larger than the stack size!\n", stderr);
    if (err == DIV_ZERO_ERR) fputs("Division by zero!\n", stderr);
    if (err == LP_ERR) fputs("The number of '(' more than number ')'!\n", stderr);
    if (err == PP_ERR) fputs("The number of ')' more than number '('!\n", stderr);
    if (err == LITER_ERR) fputs("Invalid operand!\n", stderr);
    if (err == NO_CONST_ERR) fputs("The entered variable is not initialized!\n", stderr);
    if (err == MIS_OPRTN) fputs("The operation is missing!\n", stderr);
    if (err == MIS_OPRND) fputs("The operand is missing!\n", stderr);
    
}

int
start_calc(Expression_item *resp)
{
    Help_struct control;
    control.buf = '\0';
    control.operand = 0;
    control.num_of_lp = 0;
    control.num_of_pp = 0;
    
    Poliz my_poliz;
    init_poliz(&my_poliz);
    
    Expression_item *a = malloc(sizeof(*a));
    
    int f;
    printf("Enter a math expression:\n\n");
    SAFE_BEGIN(rec_expl_next_char(a, &control));
    SAFE_BEGIN(rec_expl_add_sub(&my_poliz, a, &control));
    if (!f && a->type != eit_end) {
        f = LITER_ERR; 
        print_err (f);\
        free(a);
        deinit_poliz(&my_poliz);
		return f;
    }
    
    printf("\n\n\nEnter the variables and their values as follows:\n");
    printf("<variable name> = <variable value>\n\n");
    get_var_tabl(&variables);
    
    SAFE_BEGIN(calculate_poliz(&my_poliz, resp, sizeof(Expression_item)));
    free(variables.data);
    free(a);
    deinit_poliz(&my_poliz);
    return 0;
}

int
main(void)
/*int LLVMFuzzerTestOneInput
(const unsigned char *Data, size_t Size)*/
{
    Expression_item resp;
    int flag = start_calc(&resp);
    if (flag == 0) {
        print_elem(resp);
    }

    return 0;
}