#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#include "queue.h"

void queue_init(struct queue *q)
{
	INIT_LIST_HEAD(&q->entry);
	pthread_mutex_init(&q->mutex, NULL);
	q->len = 0;
}

void queue_add(struct queue *q, void *entity)
{
	struct queue_header *header = NULL;

	header = malloc(sizeof(struct queue_header));
	if (!header) {
		fprintf(stderr, "malloc header fail.\n");
		exit(-1);
	}

	header->ptr = entity;

	pthread_mutex_lock(&q->mutex);
	list_add_tail(&header->head, &q->entry);
	q->len ++;
	pthread_mutex_unlock(&q->mutex);
}

void *queue_get(struct queue *q)
{
	void *tmp = NULL;
	struct queue_header *header = NULL;

	pthread_mutex_lock(&q->mutex);
	if (!q->len)
		goto out;

	header = list_entry(q->entry.next, struct queue_header, head);
	list_del(q->entry.next);
	q->len --;
	tmp = header->ptr;

	free(header);
	pthread_mutex_unlock(&q->mutex);

	return tmp;

out:
	pthread_mutex_unlock(&q->mutex);
	return NULL;
}

int queue_check_first(struct queue *q, check_first_element check_func, void *arg)
{
	void *tmp = NULL;
	struct queue_header *header = NULL;

	pthread_mutex_lock(&q->mutex);
	if (!q->len)
		goto out;

	header = list_entry(q->entry.next, struct queue_header, head);
	check_func(header, arg);
	pthread_mutex_unlock(&q->mutex);

	return 0;

out:
	pthread_mutex_unlock(&q->mutex);
	return -1;
}

void release_queue(struct queue *q, destroy_action func)
{
	struct queue_header *pos = NULL;
	struct queue_header *tmp = NULL;

	pthread_mutex_lock(&q->mutex);

	if (!q->len)
		goto out;

	list_for_each_entry_safe(pos, tmp, &q->entry, head) {
		destroy_action(pos);
		free(pos);
		pos = NULL;
		q->len --;
	}

	pthread_mutex_unlock(&q->mutex);

	return;

out:
	pthread_mutex_unlock(&q->mutex);
}
