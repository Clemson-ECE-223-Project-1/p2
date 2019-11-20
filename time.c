/* time.h */

#include "time.h"
#include "randsim.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

double sim_time;

/* creates a double for using as the global sim time */
void time_init() {
    sim_time = 0;
}

/* cleans up the time system as needed */
void time_fini() {

}

/* returns the current sim time */
double time_get() {
    return sim_time;
}

/* sets the current sim time */
void time_set(double t) {
    sim_time = t;
}

/* these return a random time which is used to set the
   time each passenger takes to make each of these steps
   these times are set into the corresponding event 
   before it is scheduled. */
double time_arrive() {
    return interarrival_time();
}

double time_airline() {
    return enter_airline_queue_time();
}

double time_id() {
    srand(time(NULL));
    double randnum = rand();
    return randnum;
}

double time_scan() { 
    srand(time(NULL));
    double randnum = rand();
    return randnum;
}

double time_train() {
    srand(time(NULL));
    double randnum = rand();
    return randnum;
}

double time_gate() {
    srand(time(NULL));
    double randnum = rand();
    return randnum;
}
