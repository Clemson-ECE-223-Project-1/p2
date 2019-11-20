/* queue.h */

typedef struct passenger_s passenger_t;
typedef struct queue_s queue_t;
typedef struct queue_node node;

/*create a linked list*/
struct queue_node { //typedef
	passenger_t *passenger;
	struct queue_node *next;
}; //node

/*	create queue which pointers to the first node in the queue, the last
	and the amount of items in the queue*/
struct queue_s { //typedef
	int count;
	node *front;
	node *rear;
}; //queue_t

/* create and initialize a new queue 
   must be able to hold at least size items
   return pointer to the new queue, NULL if error */
queue_t *queue_init(int size);

/* insert an item into the queue 
   return 0 if successful, -1 otherwise */
int queue_insert(queue_t *q, passenger_t *c);

/* return the next item in the queue but do not remove it
   return NULL if the queue is empty or on error */
passenger_t *queue_peek(queue_t *q);

/* remove the next item from the queue 
   and return it, return NULL if there is an error */
passenger_t *queue_remove(queue_t *q);

/* return the number of items in the queue 
   You can see if queue is empty with this */
int queue_size(queue_t *q);

/* return non-zero if the queue is full
   This may be trivial using a linked implementation */
int queue_full(queue_t *q);

/* free all resourced used by the queue then free
   the queue itself */
void queue_finalize(queue_t *q);
