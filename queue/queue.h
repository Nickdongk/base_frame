#ifndef _QUEUE_H_
#define _QUEUE_H_
#include <pthread.h>
#include <list.h>

/**
 *queue struct
 */
struct queue {
	struct list_head entry;
	pthread_mutex_t mutex; //protect this queue
	unsigned int len;
};

/**
 *queue header struct
 */
struct queue_header {
	struct list_head head;
	void *ptr;
};
/**
 *check_first_element
 *@param q_header The queue node checked
 *@param arg The argument
 *@brief check first element callback
 */
typedef void (*check_first_element)(struct queue_header *q_header, void *arg);

/**
 *destroy_action
 *@param q_header The queue node that will be deleted
 *@brief destroy queue node callback
 */
typedef void (*destroy_action)(struct queue_header *q_header);

/**
 *
 */
extern void queue_init(struct queue *q);
extern void queue_add(struct queue *q, void *entity);
extern void *queue_get(struct queue *q);
extern int queue_check_first(struct queue *q, check_first_element check_func, void *arg);
extern void release_queue(struct queue *q, destroy_action func);
#endif

