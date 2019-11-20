/* event.c */

#include "priority.h"
#include "time.h"
#include "randsim.h"
#include "sim.h"
#include "event.h"
#include "queue.h"

#include <stdio.h>
#include <stdlib.h>

static priority_t *q = NULL;

/* initializes events, creates a priority queue */
void event_init(int MAX_PASS) {
    if(q == NULL) {
        q = priority_init(MAX_PASS);
        if(q == NULL) {
            printf("Cannot Intialize Event\n");
        }
    }
}

/* frees up all event space, including space in the priority queue */
void event_fini() {
    priority_finalize(q);
}

/* allocate a fresh event with empty fields */
event_t *event_create() {
    event_t *ev = (event_t *)malloc(sizeof(event_t));
    ev->passenger = (passenger_t *)malloc(sizeof(passenger_t));

    if(ev == NULL || ev->passenger == NULL)
    {
        printf("Issue with your event/event passenger data.");
        return NULL;
    }
    return ev;
}

/* free an event */
void event_destroy(event_t *e) {
    free(e->passenger);
    free(e->queue);
    free(e);
}

/* insert the event into the priority queue.  The key
   value is the current sim time plus the event_time in
   event.  Update the event time to the key value. */
void event_schedule(event_t *e) {
    double key_time = time_get();
    e->event_time += key_time; 
    priority_insert(q, e);
}

/* remove the next event from the priority and return it to the program for execution */
event_t *event_cause() {
    event_t *event = priority_remove(q);
    if(event == NULL) {
        printf("The queue is empty.");
        return NULL;
    }
    time_set(event->event_time);
    return event;
}

// function to check if event is empty
// returns 1 if empty, 0 if not
int event_empty(event_t *e){
    int isEmpty = -1;

    if(e->queue == NULL){
        isEmpty = 1;
        // printf("Event Queue currently empty\n");
    } else {
        isEmpty = 0;
    }
    
    return isEmpty;
}

// function to get the event's time
double event_time(event_t *e) {
   return e->event_time;
}
