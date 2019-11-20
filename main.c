/* main.c */
/* initialize queues */
/* malloc new EV_ARRIVE event and passenger */

#include "priority.h"
#include "randsim.h"
#include "time.h"
#include "sim.h"
#include "event.h"
#include "queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// #define MAX_PASS 1000000
int MAX_PASS = 1000000;
#define MAX_SCAN 4
#define QSZ 100
int num_passengers = 0; /* counts the number of passengers */
int isPQFull = 0;

int main(int argc, char **argv) {
    // gets size of priority queue from command line argument
    int c;
    while ((c = getopt(argc, argv, "e:")) != -1) {
        if (c == 'e') {
            MAX_PASS = atoi(optarg);
        }
    }

    /* initialize modules */
    event_init(QSZ);
    time_init();

    /* intialize queues */
    queue_t *airlineQ = queue_init(QSZ);
    queue_t *idQ = queue_init(QSZ);
    queue_t *scanQ[MAX_SCAN];
    for(int i = 0; i < MAX_SCAN; i++) {
        scanQ[i] = queue_init(QSZ);
    }
    queue_t *trainQ = queue_init(QSZ);

    event_t *start_ev;
    start_ev = event_create();
    start_ev->passenger = (passenger_t *)malloc(sizeof(passenger_t));
    
    /* schedule EV_ARRIVE event at t=0 and put in event queue */
    start_ev->event_type = EV_ARRIVE;
    start_ev->event_time = 0.0;
    event_schedule(start_ev);

    /* run main loop */
    while(event_empty(start_ev) != 0) {
        
        if(isPQFull) {
            return 0;
        }

        event_t *new_ev;
        new_ev = event_cause();
        // time_set(new_ev->event_time); - P2

        switch (new_ev->event_type) {
            case (EV_ARRIVE) : {
                event_t *airline_ev;
                airline_ev = event_create();
                airline_ev->passenger = new_ev->passenger;
                airline_ev->passenger->arrival_time = time_get();
                airline_ev->event_time = time_airline();
                airline_ev->event_type = EV_ARRIVE;
                event_schedule(airline_ev);

                // new_ev->passenger->arrival_time = time_get();
                // /* create new EV_AIRLINEQ event and passenger */
                // new_ev->event_type = EV_AIRLINEQ;
                // new_ev->event_time = time_airline();
                // /* schedule EV_AIRLINEQ event */
                // event_schedule(new_ev);
                
                printf("Passenger #%d arrived at %f\n", num_passengers+1, new_ev->passenger->arrival_time);

                if (MAX_PASS > num_passengers++) {
                    /* create new EV_ARRIVE event and passsenger */
                    event_t *arrive_ev = event_create();
                    arrive_ev->passenger = (passenger_t *)malloc(sizeof(passenger_t));
                    arrive_ev->event_time = time_arrive();
                    arrive_ev->event_type = EV_ARRIVE;
                    /* schedule EV_ARRIVE event */ 
                    event_schedule(arrive_ev);
                }
            }
                break;
            case (EV_AIRLINEQ) : {
                
            }
                break;
            case (EV_AIRLINE) : {

            }
                break;
            case (EV_IDQ) : {

            }
                break;
            case (EV_ID) : {

            }
                break;
            case (EV_SCANQ) : {

            }
                break;
            case (EV_SCAN) : {

            }
                break;
            case (EV_TRAINQ) : {

            }
                break;
            case (EV_TRAIN) : {

            }
                break;
            case (EV_GATE) : {

            }
                break;
            default :{
                /* error */
                printf("ERROR\n");
            }
                break;
            }
        
        /* free event */
        event_destroy(new_ev);
    }
    time_fini();
    event_fini();

    return 0;
}
