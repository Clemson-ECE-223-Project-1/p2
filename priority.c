/* priority.h */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

// #include "event.h"
#include "priority.h"

#define PRISIZE 100

/* the priority queue type */
typedef struct priority_s priority_t;
struct priority_s
{
    int size;
    int count;
    cmp_t cmp;
    p_data_t *A[PRISIZE];
};

/* type cmp_t is a pointer to a function that returns in and takes 
   two pointers for arguments. computes less than of the two
   pointer's value */
typedef int (* cmp_t)(p_data_t *left, p_data_t *right);

/* create and initialize a new priority queue 
   must be able to hold at least size items
   return pointer to the new priority queue, NULL if error */
priority_t *priority_init(int size, cmp_t cmp)
{
    priority_t *pri = (priority_t *)malloc(sizeof(priority_t));
    memset(pri, 0, sizeof(priority_t));
    pri->size = PRISIZE;
    pri->count = 0;
    pri->cmp = cmp; 
    return pri;
}

void priority_insert(priority_t *pri, p_data_t *pd)
{
    int child, parent;

    if (priority_full(pri))
    {
        return;
    }

    //printf("PRI INSERT %f\n", pd->event_time);
    pri->count++;
    child = pri->count;
    parent = child/2;

    while(parent != 0)
    {
        if(pri->cmp(pd, pri->A[parent]))
        {
            pri->A[child] = pd;
            return;
        }
        else
        {
            pri->A[child] = pri->A[parent];
            child = parent;
            parent = parent/2;
        }
    }
    pri->A[child] = pd;
}

/* remove the highest priority item from the queue 
   and return it, return NULL if there is an error */
p_data_t *priority_remove(priority_t *pri)
{
    int current = 1;
    int child = 2;    /* 2 times current */
    
    if (priority_empty(pri))
    {
        /* error - empty */
        return NULL;
    }

    p_data_t *val = pri->A[1];
    p_data_t *place = pri->A[pri->count];
    pri->count--;

    while(child <= pri->count)
    {
        if (child < pri->count)
        {
            if (pri->cmp(pri->A[child+1], pri->A[child]))
            {
                child++;
            }
        }
        if (pri->cmp(pri->A[child], place))
        {
            pri->A[current] = place;
            return val;
        }
        else
        {
            pri->A[current] = pri->A[child];
            current = child;
            child = 2 * current;
        }
    }

    pri->A[current] = place;
    // pri->A[pri->count + 1] = NULL;
    // printf("PRI RETURNS %f\n", val->event_time);
    return val;
}

/* return non-zero if the priority queue us empty */
int priority_empty(priority_t *pri)
{
    return (pri->count == 0);
}

/* return nono-zero if the priority queue is full 
   This may be trivial using a linked implementation */
int priority_full(priority_t *pri)
{
    return (pri->count == pri->size);
}

/* free all resourced used by the priority queue then free
   the queue itself */
void priority_finalize(priority_t *pri)
{
    free(pri);
}
