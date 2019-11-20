/* queue.c */

#include "queue.h"
#include "event.h"
#include "sim.h"

#include <stdio.h>
#include <stdlib.h>

/* check to see if queue is empty */
int queue_empty(queue_t *q) {
	if (q->front == NULL) {
       printf("Queue is empty.\n");
	   return 1;
   } else {
	   printf("Queue is not empty.\n");
	   return 0;
   }
}

/*	initialize queue by setting count to 0
	and pointing the head and tail to NULL */
queue_t *queue_init(int size) {
	queue_t  *q = (queue_t *)malloc(size*sizeof(queue_t));
	q->count = 0;
	q->front = NULL;
	q->rear = NULL;
	return (q);
}

/* function to add elements to the queue from the rear */
int queue_insert(queue_t *q, passenger_t *c) {
	node *temp; 							// create a new node to add to the queue
	temp = (node *)malloc(sizeof(node));	// request memory for the node
	temp->passenger = c;
	temp->next = NULL;

	// if queue not empty, add to end of queue
	if (queue_empty(q) == 0) {
		q->rear->next = temp;
		q->rear = temp;
	// if queue is full, can't add to queue
	} else if(queue_full(q) != 0) {
		return 1;
	// if empty, insert first node into queue
	} else {
		q->front = q->rear = temp;
	}
	q->count++;							// update size of queue
	return 0;
}

/* return the next item in the queue but do not remove it
   return NULL if the queue is empty or on error */
passenger_t *queue_peek(queue_t *q) {
	return q->front->passenger;
}

/* function to delete the front node and return its value */
passenger_t *queue_remove(queue_t *q) {
	passenger_t *removed_passenger;
	if (queue_empty(q) == 1) {
		printf("Error: Cannot remove from empty queue!\n");
		return NULL;
   	} else {
		node *newFront = q->front;
		q->front = q->front->next;
		q->count--;
		removed_passenger = newFront->passenger;
		free(newFront);
		return (removed_passenger);
	}
}

/* return the number of items in the queue 
   You can see if queue is empty with this */
int queue_size(queue_t *q) {
    printf("Size of Queue = %d\n", q->count);
	return q->count;
}

/* check to see if queue is full */
int queue_full(queue_t *q) {
	if (q->count == MAX_PASS) {
       printf("Queue is full.\n");
	   return 1;
   } else {
	   printf("Queue is not full.\n");
	   return 0;
   }
}

/* free all resourced used by the queue then free
   the queue itself */
void queue_finalize(queue_t *q) {
	free(q->front);
	free(q->rear);
	free(q);
}
