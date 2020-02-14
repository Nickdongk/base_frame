#include <stdlib.h>
#include <stdio.h>

#include "stack.h"


struct stack *stack_create(void)
{
	struct stack *r_stack = NULL;

	r_stack = malloc(sizeof(struct stack));
	if (!r_stack) {
		fprintf(stderr, "malloc struct stack fail.\n");
		return NULL;
	}

	INIT_LIST_HEAD(&r_stack->entry);
	r_stack->len = 0;

	return r_stack;
}

size_t stack_len(struct stack *stack_in)
{
	return stack_in->len;
}

int stack_push(struct stack* stack_in, void* val)
{
	struct stack_n* p_stack_n = NULL;

	p_stack_n = malloc(sizeof(struct stack_n));
	if (!p_stack_n) {
		fprintf(stderr, "malloc struct stack_n fail.\n");
		return -1;
	}

	p_stack_n->ptr = val;
	list_add(&p_stack_n->head, &stack_in->entry);
	stack_in->len++;

	return 1;
}

void* stack_pop(struct stack* stack_in)
{
	struct stack_n* p_stack_n = NULL;
	void* val = NULL;

	if (!stack_in->len)
		return NULL;

	list_del(stack_in->entry.prev);
	stack_in->len--;

	p_stack_n = container_of(stack_in->entry.prev, struct stack_n, head);
	val = p_stack_n->ptr;
	free(p_stack_n);
	return val;
}

void stack_release(struct stack* stack_in, release_stack_node release_callback)
{
	struct stack_n *p_stack_n = NULL;
	struct stack_n *tmp_stack_n = NULL;

	if (!stack_in->len)
		goto empty;

	list_for_each_entry_safe(p_stack_n, tmp_stack_n,
		&stack_in->entry, head) {
		list_del(&p_stack_n->head);
		release_callback(p_stack_n);
		free(p_stack_n);
	}

empty:
	free(stack_in);
}
