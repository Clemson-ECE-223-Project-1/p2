/* priority.h */

#ifndef PRIORITY_H
#define PRIORITY_H

/* the priority queue type */
typedef struct priority_s priority_t;

/* this is they type of items put in the queue, opaque to the queue */
typedef struct priority_data p_data_t;

/* type lt_t is a pointer to a function that returns in and takes 
   two pointers for arguments. computes less than of the two
   pointer's value */
typedef int (* cmp_t)(p_data_t *left, p_data_t *right);

/* create and initialize a new priority queue 
   must be able to hold at least size items
   return pointer to the new priority queue, NULL if error */
priority_t *priority_init(int size, cmp_t cmp);

/* insert an item into the priority queue 
   return 0 if successful, -1 otherwise */
void priority_insert(priority_t *pri, p_data_t *pd);

/* remove the highest priority item from the queue 
   and return it, return NULL if there is an error */
p_data_t *priority_remove(priority_t *pri);

/* return non-zero if the priority queue us empty */
int priority_empty(priority_t *pri);

/* return nono-zero if the priority queue is full 
   This may be trivial using a linked implementation */
int priority_full(priority_t *pri);

/* free all resourced used by the priority queue then free
   the queue itself */
void priority_finalize(priority_t *pri);

#endif
