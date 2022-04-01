#include "recurs_expl.h"

#define SAFE_RE(call) do { \
					flag = call; \
					if (flag != 0) { \
						return flag; \
					} \
				}\
				while (0)


void
print_elem(Expression_item a)
{
    if (a.type == 0) {
        printf("Result is %lld\n\n", a.data.i);
    } else if (a.type == 1) {
        printf("Result is %f\n\n", a.data.d);
    } else fputs("Error with result type!\n", stderr);
}

int ctrl_op(int t, Help_struct *ctrl)
{
    if (t == eit_plus || t == eit_minus || t == eit_mul || 
        t == eit_plus || t == eit_div)
    {
        if (ctrl->operand == 1) {
            ctrl->operand = 0;
            return 0;
        } else return MIS_OPRND;
    }
    
    if (t == eit_lp) {
        ctrl->num_of_lp++;
        return 0;
    }  
    
    if (t == eit_pp) {
        ctrl->num_of_pp++;
        if (ctrl->num_of_lp < ctrl->num_of_pp) return PP_ERR;
        return 0;
    }   
    
    if (t == eit_int || t == eit_dbl || t == eit_var) {
        if (ctrl->operand == 0) {
            ctrl->operand = 1;
            return 0;
        } else return MIS_OPRTN;
         
    }
    if (t == eit_end) {
        if (ctrl->num_of_lp > ctrl->num_of_pp) return LP_ERR;
        if (ctrl->num_of_lp < ctrl->num_of_pp) return PP_ERR;
        return 0;
    }
    return LITER_ERR;
}

int
rec_expl_next_char(Expression_item *cur_expr, Help_struct *control)
{
    if (control->buf != '\0') {
        if (control->buf == '+') {
            cur_expr->type = eit_plus;
        } else if (control->buf == '-') {
            cur_expr->type = eit_minus;
        } else if (control->buf == '*') {
            cur_expr->type = eit_mul;
        } else if (control->buf == '/') {
            cur_expr->type = eit_div;
        } else if (control->buf == ')') {
            cur_expr->type = eit_pp;
        } else if (control->buf == '\0' || 
                   control->buf == '\n' ||
                   control->buf == EOF) {
            cur_expr->type = eit_end;
            return 0;
        }
        control->buf = '\0';
        return (ctrl_op(cur_expr->type, control));
    }
    
    int c;
    if ((c = getc(stdin)) != '\0' && c != '\n' && c != EOF) {
       
        while (c == ' ') c = getc(stdin);
       
        if (c == '+') {
            cur_expr->type = eit_plus;
            return (ctrl_op(cur_expr->type, control));
        } else if (c == '-') {
            cur_expr->type = eit_minus;
            return (ctrl_op(cur_expr->type, control));
        } else if (c == '*') {
            cur_expr->type = eit_mul;
            return (ctrl_op(cur_expr->type, control));
        } else if (c == '/') {
            cur_expr->type = eit_div;
            return (ctrl_op(cur_expr->type, control));
        } else if (c == '(') {
            cur_expr->type = eit_lp;
            return (ctrl_op(cur_expr->type, control));
        } else if (c == ')') {
            cur_expr->type = eit_pp;
            return (ctrl_op(cur_expr->type, control));
        } else {
            char *str = NULL;
            int len = 0;
            int dbl_flag = 0, var_flag = 0;
            do {
                ++len;
                str = realloc(str, len*sizeof(char));
                str[len - 1] = c;
                if (isalpha(c) || c == '_') {
                    if (isdigit(str[0])) {
                        free(str);
                        return LITER_ERR;
                    }
                    if (!dbl_flag && len < 7) {
                        var_flag = 1;
                    } else {
                        free(str);
                        return LITER_ERR;
                    }
                } else if (c == '.') {
                    if (!dbl_flag && !var_flag) {
                        dbl_flag = 1;
                    } else {
                        free(str);
                        return LITER_ERR;
                    }
                } else if (!isdigit(c)) {
                    free(str);
                    return LITER_ERR;
                }
                c = getc(stdin);
                if (c == '+' || c == '-' || c == '*' || c == '/' || 
                    c == ')' || c == '\0' || c == '\n' || c == EOF)
                {
                    control->buf = c;
                    break;
                }
            } while (c != '\0' && c != '\n' && c != EOF && c != ' ');
            ++len;
            str = realloc(str, len*sizeof(char));
            str[len - 1] = '\0';
            if (dbl_flag) {
                cur_expr->type = eit_dbl;
                cur_expr->data.d = strtod(str, NULL);
            } else if (var_flag) {
                cur_expr->type = eit_var;
                memcpy(cur_expr->data.var, str, len);
            } else {
                cur_expr->type = eit_int;
                cur_expr->data.i = strtol(str, NULL, 0);
            }
            free(str);
            return (ctrl_op(cur_expr->type, control));
        }
    } else {
        cur_expr->type = eit_end;
        return 0;
    }
}

int
rec_expl_add_sub(Poliz *poliz, Expression_item *curc, Help_struct *control)
{
    int flag;
    SAFE_RE(rec_expl_mul_div(poliz, curc, control));
    while (curc->type == eit_plus || curc->type == eit_minus) {
        char temp = curc->type;
        SAFE_RE(rec_expl_next_char(curc, control));
        SAFE_RE(rec_expl_mul_div(poliz, curc, control));
        Calculate_elem funct;
        if (temp == eit_plus) funct = calc_plus;
        if (temp == eit_minus) funct = calc_minus;
        new_elem_poliz(poliz, NULL, 0, &funct);
    }
    return 0;
}

int rec_expl_mul_div(Poliz *poliz, Expression_item *curc, Help_struct *control) {
    int flag;
    SAFE_RE(rec_expl_symbol(poliz, curc, control));
    while (curc->type == eit_mul || curc->type == eit_div) {
        char temp = curc->type;
        SAFE_RE(rec_expl_next_char(curc, control));
        SAFE_RE(rec_expl_symbol(poliz, curc, control));
        Calculate_elem funct;
        if (temp == eit_mul) funct = calc_mul;
        if (temp == eit_div) funct = calc_div;
        new_elem_poliz(poliz, NULL, 0, &funct);
    }
    return 0;
}

int rec_expl_symbol(Poliz *poliz, Expression_item *curc, Help_struct *control) {
    int flag;
    if (curc->type == eit_end) return 0;
    if (curc->type == eit_lp) {
        SAFE_RE(rec_expl_next_char(curc, control));
        SAFE_RE(rec_expl_add_sub(poliz, curc, control));
        if(curc->type != eit_pp) {
            return LP_ERR;
        } else {
            SAFE_RE(rec_expl_next_char(curc, control));
        }
    } else if (curc->type == eit_int || curc->type == eit_dbl) {
        Calculate_elem funct = calc_const;
        new_elem_poliz(poliz, curc, sizeof(*curc), &funct);
        SAFE_RE(rec_expl_next_char(curc, control));
    } else if (curc->type == eit_var) {
        Calculate_elem funct = calc_var;
        new_elem_poliz(poliz, curc, sizeof(*curc), &funct);
        SAFE_RE(rec_expl_next_char(curc, control));
    } else if (curc->type == eit_pp) {
        return PP_ERR;
    } else {
        if (curc->type >= eit_plus && curc->type <= eit_div && control->operand == 0) {
            return MIS_OPRND;
        }
        return LITER_ERR;
    }
    return 0;
}