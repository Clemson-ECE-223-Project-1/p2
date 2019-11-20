/* priority.c */

#include "priority.h"
#include "sim.h"
#include "event.h"
#include "queue.h"

#include <stdio.h>
#include <stdlib.h>

struct priority_s { //typedef removed
    int current_size; //current size of the heap-array
    int max_size;   //max size of the heap-array
    event_t **event_tree;
}; //priority_t removed

/* create and initialize a new priority queue 
   must be able to hold at least size items
   return pointer to the new priority queue, NULL if error */
priority_t *priority_init(int size) {
    if(size < 0) {
        printf("Size < 0\n");
        return NULL;
    }
    
    priority_t *q = (priority_t *)malloc(sizeof(priority_t));
    q->max_size = size; // maximum size of tree
    q->current_size = 0;   // number of nodes in tree
    q->event_tree = (event_t **)malloc(sizeof(event_t)*(size+1)); // tree
    
    for(int i = 0; i < size; i++) {
        q->event_tree[i] = NULL;
    }

    if(q == NULL) {
        printf("Cannot Intialize Priority Queue\n");
        return NULL;
    }

    return q;
}

/* return non-zero if the priority queue us empty */
int priority_empty(priority_t *q) {
    int isEmpty = -1;

    if(q->current_size == 0) {
        isEmpty = 1;
        // printf("Priority Queue currently empty\n");
    } else {
        isEmpty = 0;
    }

    return isEmpty;
}

/* return nono-zero if the priority queue is full 
   This may be trivial using a linked implementation */
int priority_full(priority_t *q) {
    int isFull = 0;

    if(q->current_size == q->max_size) {
        isFull = 1;
        isPQFull = 1;
        printf("Priority Queue Full\n");
    }

    return isFull;
}

//Function to make the array into a heap
static void heapify(priority_t *q, event_t *ev)
{//increasing the size of the array by 1 since the first element in the array needs to be empty
    q->current_size++;
    int child_pos, parent_pos;
    child_pos = q->current_size;
    parent_pos = child_pos/2;

    //if the parent position is the zero'th element, then skip this loop since it should be empty
    while(parent_pos != 0)
    {//if the event time is greater than the parent's event time, make this the chid's value since this is an
     //array that should have small values higher in the heap as opposed to large values
        if(ev->event_time >= q->event_tree[parent_pos]->event_time)
        {//set the child's priority value
            q->event_tree[child_pos] = ev;
            return;
        }
        else
        {//if the function makes it to this part, that means the parent is larger than the child (which shouldn't be the case)
         //sets the child as the old's parents position and moves the parent's position up a level
            q->event_tree[child_pos] = q->event_tree[parent_pos];
            child_pos = parent_pos;
            parent_pos = parent_pos/2;
        }
    }//set the child's priority value
    q->event_tree[child_pos] = ev;
}

/* insert an item into the priority queue 
   return 0 if successful, -1 otherwise */
int priority_insert(priority_t *q, event_t *ev) {
    int full, empty, success;

    full = priority_full(q);
    empty = priority_empty(q);
    success = 1;

    if(full == 1) {
        isPQFull = 1;
        success = -1;
    } else if(empty == 1) {
        q->current_size++;
        q->event_tree[1] = ev;
        success = 0;
    } else {
        heapify(q, ev);
        success = 0;
    }

    return success;
}

// function to reheapify the pq after a removal
static void reheapify(priority_t *q) {
    int current_pos = 1;
    int child_pos = 2*current_pos;
    event_t *ev;
    ev = q->event_tree[q->current_size];
    q->event_tree[q->current_size] = NULL;
    q->current_size--;

    if(priority_empty(q) != 1) {
        while(child_pos <= q->current_size) {
            if(child_pos < q->current_size) {
                double right_child = q->event_tree[child_pos+1]->event_time;
                double left_child = q->event_tree[child_pos]->event_time;

                if(right_child < left_child)
                    child_pos++;
            }

            if(q->event_tree[child_pos]->event_time >= ev->event_time)
                break;
            else {
                q->event_tree[current_pos] = q->event_tree[child_pos];
                current_pos = child_pos;
                child_pos = 2*child_pos;
            }
            
        }
        q->event_tree[current_pos] = ev;
    }
}

//remove the root (highest priority) of the heap
event_t *priority_remove(priority_t *q) {
    int empty = priority_empty(q);
    event_t *removal = event_create();
    
    if(empty == 1) {
        removal = NULL;
    } else {
        removal = q->event_tree[1];
        q->event_tree[1] = q->event_tree[q->current_size];
        reheapify(q);
    }

    return removal;
}

// function to print the current heap
void print_heap(priority_t *q) {
    int isEmpty = priority_empty(q);
    
    if(isEmpty) {
        return;
    } else {
        for(int i = 1; i <= q->current_size; i++) {
            printf("%f\n", q->event_tree[i]->event_time);
        }
        printf("\n");
    }
}

/* free all resourced used by the priority queue then free
   the queue itself */
void priority_finalize(priority_t *q) {
    for(int i = 0; i < q->current_size; i++) {
        free(q->event_tree[i]);
        q->event_tree[i] = NULL;
    }
    
    free(q->event_tree);
    q->event_tree = NULL;

    free(q);
    q = NULL;
}
