#include "init_var.h"

extern Var_table variables;

int
get_var(Variable_value *cur_var)
{
    int c;
    while ((c = getc(stdin)) == ' ');
    int i, f = 0;
    strncpy(cur_var->str, "", sizeof(double));
    for (i = 0; i < 7; i++) {
        if (isdigit(c) || isalpha(c) || c == '_') {
            cur_var->str[i] = c;
            c = getc(stdin);
            if (c == EOF) return EOF;
        } else if (c == ' ' || c == '=') {
            break;
        } else {
            f = 1;
            break;
        }
    }
    
    if (isdigit(cur_var->str[0])) f = 1;
    if (c != ' ' && c != '=') f = 1;
    
    while (c == ' ') c = getc(stdin);
    if (c != '=') {
        f = 1;
    } else c = getc(stdin);
    while (c == ' ') c = getc(stdin);
    if (c == EOF) f = 1;
    int j = 0, size = 4, dot = 0;
    char *buf = malloc(size*sizeof(char));
    while (c != '\n' && c != '\0' && c != EOF && !f) {
        if (j > size - 2) {
            size *= 2;
            buf = realloc(buf, size*sizeof(char));
        }
        if (isdigit(c) || c == '.' || c == '+' || c == '-') {
            if ((c == '+' || c == '-') && j != 0) f = 1;
            if (c == '.') {
                if (dot != 0) {
                    f = 1;
                } else dot = 1;
            }
            if (!f) {
                buf[j] = c;
                if ((c = getc(stdin)) == EOF) break;
                j++;
            } else break;
        } else break;
    }
    if (!f) {
        buf[j] = '\0';
        if (dot) {
            cur_var->type = eit_dbl;
            cur_var->data.d = strtod(buf, NULL);
        } else { 
            cur_var->type = eit_int;
            cur_var->data.i = strtol(buf, NULL, 0);
        }
        free(buf);
    } else {
        free(buf);
        if (c == EOF) return EOF;
        return 1;
    }
    if (c == EOF) return EOF;
    return 0;
}


int
get_var_tabl(Var_table *my_table)
{
    int f = 0;
    Variable_value *cur_var = malloc(sizeof(*cur_var));
    my_table->size = 0;
    my_table->data = NULL;
    do {
        f = get_var(cur_var);
        if (f != 1) {
            int new_mean = 0;
            for(int k = 0; k < my_table->size; k++) {
                if ((strcmp(variables.data[k].str, cur_var->str) == 0)) {
                    my_table->data[k] = *cur_var;
                    new_mean = 1;
                }
            }
            if (!new_mean) {
                my_table->size++;
                my_table->data = realloc(my_table->data, my_table->size*sizeof(*cur_var));
                my_table->data[my_table->size - 1] = *cur_var;
            }
        }
    } while (f != EOF);
    free(cur_var);
    return 0;
}
















