#ifndef _QUEUE_H_
#define _QUEUE_H_
#include <pthread.h>
#include <list.h>

struct queue {
	struct list_head entry;
	pthread_mutex_t mutex; //protect this queue
	unsigned int len;
};

struct queue_header {
	struct list_head head;
	void *ptr;
};

typedef void (*check_first_element)(struct queue_header *, void *arg);

typedef void (* destroy_action)(struct queue_header *);

extern void queue_init(struct queue *q);
extern void queue_add(struct queue *q, void *entity);
extern void *queue_get(struct queue *q);
extern int queue_check_first(struct queue *q, check_first_element check_func, void *arg);
extern void release_queue(struct queue *q, destroy_action func);
#endif

