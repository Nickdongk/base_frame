#ifndef _STACK_H_
#define _STACK_H_

#include <list.h>

struct stack_n {
	struct list_head head;
	void* ptr;
};

struct stack {
	struct list_head entry;
	size_t len;
};



typedef void (*release_stack_node)(struct stack_n *p_stack_n);

extern struct stack *stack_create(void);
extern size_t stack_len(struct stack *stack_in);
extern int stack_push(struct stack *stack_in, void *val);
extern void *stack_pop(struct stack *stack_in);
extern void stack_release(struct stack *stack_in, release_stack_node releas_callback);


#endif

