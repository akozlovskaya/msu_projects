#include "poliz.h"

#define SAFE(call) do { \
					flag = call; \
					if (flag != 0) { \
						goto FINALLY; \
					} \
				}\
				while (0)

int
calculate_poliz(const Poliz *pol, void *resp, size_t size_res)
{
	Stack stack;
	int flag = init_stack(&stack);
	if (flag != 0) {
		return flag;
	}

	for (size_t curr_size = 0; curr_size < pol->size; curr_size += sizeof (Size_elem) + ((char *)pol->data)[curr_size])
    {
		void *elem = &((char *)pol->data)[curr_size + sizeof (Size_elem)];
        
        Calculate_elem func;
        memcpy(&func, elem, sizeof(Calculate_elem));
    
        elem = &((char *)pol->data)[curr_size + sizeof (Size_elem) + sizeof(Calculate_elem)];
        SAFE((*func)(elem, ((char *)pol->data)[curr_size] - sizeof(Calculate_elem), &stack));
	}
    
	SAFE(pop_stack(&stack, resp, size_res));
	
	final_stack(&stack);
	return 0;

FINALLY:
	final_stack(&stack);
	return flag;

}


int
init_poliz(Poliz *poliz)
{
    poliz->data = NULL;
    poliz->cap = 0;
    poliz->size = 0;
    return 0;
}


int                            /*size - element only size (no function)*/
new_elem_poliz(Poliz *poliz, void *item, Size_elem size, Calculate_elem *func)
{
    size_t prev_size = poliz->size;
    poliz->size += sizeof(Calculate_elem *) + sizeof(Size_elem) + size;
    if (poliz->cap == 0) {
        poliz->cap = sizeof(Calculate_elem *) + sizeof(Size_elem) + size;
        poliz->data = realloc(poliz->data, poliz->cap);
    }
    while (poliz->size > poliz->cap) {
        poliz->cap *= 2;
        poliz->data = realloc(poliz->data, poliz->cap);
    }
    Size_elem size_cur_elem = sizeof(func) + size;
    for (int i = 0; i < sizeof(Size_elem); ++i) {
        ((char *)poliz->data)[i + prev_size] = ((char *)(&size_cur_elem))[i];
    }
    prev_size += sizeof(Size_elem);
    
    for (int i = 0; i < sizeof(Calculate_elem *); ++i) {
        ((char *)poliz->data)[i + prev_size] = ((char *)func)[i];
    }
    if (item == NULL) return 0;
    prev_size += sizeof(Calculate_elem *);
    for (int i = 0; i < size; ++i) {
        ((char *)poliz->data)[i + prev_size] = ((char *)(item))[i];
    }
    return 0;
}


int
deinit_poliz(Poliz *poliz)
{
    free(poliz->data);
    poliz->cap = 0;
    poliz->size = 0;
    return 0;
}