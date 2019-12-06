/* main.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "sim.h"
#include "event.h"
#include "queue.h"
#include "time.h"
 
// #define MAX_PASS 100
int MAX_PASS = 100;
#define MAX_SCAN 2
#define QSZ 100 /* not used by queue.c */

// static void parse_args(argc, argv);

int num_passengers = 0; /* counts the number of passengers */

queue_t *airlineQ;
queue_t *idQ;
queue_t *scanQ[MAX_SCAN];
queue_t *trainQ;

int main(int argc, char **argv)
{
    /* process command line arguments */
    // parse_args(argc, argv);
    
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

    /* initialize queues */
    airlineQ = queue_init(QSZ);
    idQ = queue_init(QSZ);
    for (int i = 0; i < MAX_SCAN; i++)
        scanQ[i] = queue_init(QSZ);
    trainQ = queue_init(100);
    
    event_t *start_ev;
    start_ev = event_create();
    start_ev->passenger = passenger_create();
    start_ev->passenger->pass_id = ++num_passengers;
    start_ev->event_time = 0.0;
    start_ev->event_type = EV_ARRIVE;
    event_schedule(start_ev);
    /* run main loop */
    while(!event_empty())
    {
        event_t *new_ev;
        new_ev = event_cause();
        //time_set(new_ev->event_time);
        switch (new_ev->event_type)
        {
        
            /* this is when passenger arrives at the airport */
            case (EV_ARRIVE) :
            {
                event_t *airline_ev;

                printf("new passenger %d arrives: %f\n", new_ev->passenger->pass_id, new_ev->event_time);

                airline_ev = event_create();
                airline_ev->passenger = new_ev->passenger;
                airline_ev->passenger->arrival_time = time_get();
                airline_ev->event_time = time_airlineQ();
                airline_ev->event_type = EV_AIRLINEQ;
                event_schedule(airline_ev);

                if (MAX_PASS > num_passengers)
                {
                    event_t *arrive_ev;
                    arrive_ev = event_create();
                    arrive_ev->passenger = passenger_create();
                    arrive_ev->passenger->pass_id = ++num_passengers;
                    arrive_ev->event_time = time_arrive();
                    arrive_ev->event_type = EV_ARRIVE;
                    event_schedule(arrive_ev);
                }
            }
                break;
            
            /* this is when she selects an airline and gets in line to check bags*/
            case (EV_AIRLINEQ) :
            {
                event_t *airlineq_ev;

                printf("passenger %d departs: %f\n", new_ev->passenger->pass_id, new_ev->event_time);

                airlineq_ev = event_create();
                airlineq_ev->passenger = new_ev->passenger;
                airlineq_ev->passenger->arrival_time = time_get();
                airlineq_ev->event_time = time_airline();
                airlineq_ev->event_type = EV_AIRLINE;
                event_schedule(airlineq_ev);

                break;
            }

            /* this is when she gets to the agent to check bags */
            case (EV_AIRLINE) :
            {
                event_t *airline_ev;

                printf("passenger %d checks bags: %f\n", new_ev->passenger->pass_id, new_ev->event_time);

                airline_ev = event_create();
                airline_ev->passenger = new_ev->passenger;
                airline_ev->passenger->arrival_time = time_get();
                airline_ev->event_time = time_idQ();
                airline_ev->event_type = EV_IDQ;
                event_schedule(airline_ev);

                break;
            }

            /* this is when she enters the line to check her ID */
            case (EV_IDQ) :
            {
                event_t *idQ_ev;

                printf("passenger %d enters line: %f\n", new_ev->passenger->pass_id, new_ev->event_time);

                idQ_ev = event_create();
                idQ_ev->passenger = new_ev->passenger;
                idQ_ev->passenger->arrival_time = time_get();
                idQ_ev->event_time = time_id();
                idQ_ev->event_type = EV_ID;
                event_schedule(idQ_ev);

                break;
            }

            /* this is when she gets her ID check */
            case (EV_ID) :
            {
                event_t *id_ev;

                printf("passenger %d gets ID checked: %f\n", new_ev->passenger->pass_id, new_ev->event_time);

                // scanQ
                // int compareInt = 2;
                // for(i = 0; i < MAX_SCAN; i++) {
                
                // }
                
                id_ev = event_create();
                id_ev->passenger = new_ev->passenger;
                id_ev->passenger->arrival_time = time_get();
                id_ev->event_time = time_scanQ();
                id_ev->event_type = EV_SCANQ;
                event_schedule(id_ev);

                break;
            }

            /* this is when she gets in line for the scan */
            case (EV_SCANQ) :
            {
                event_t *scanQ_ev;
                
                printf("passenger %d enters scan line: %f\n", new_ev->passenger->pass_id, new_ev->event_time);

                scanQ_ev = event_create();
                scanQ_ev->passenger = new_ev->passenger;
                scanQ_ev->passenger->arrival_time = time_get();
                scanQ_ev->event_time = time_scan();
                scanQ_ev->event_type = EV_SCAN;
                event_schedule(scanQ_ev);

                break;
            }

            /* this is when she gets to the scanner */
            case (EV_SCAN) :
            {
                event_t *scan_ev;

                printf("passenger %d gets to scanner: %f\n", new_ev->passenger->pass_id, new_ev->event_time);

                scan_ev = event_create();
                scan_ev->passenger = new_ev->passenger;
                scan_ev->passenger->arrival_time = time_get();
                scan_ev->event_time = time_trainQ();
                scan_ev->event_type = EV_TRAINQ;
                event_schedule(scan_ev);

                break;
            }

            /* this is when she is finished being scanned and heads for the train */
            case (EV_TRAINQ) :
            {
                event_t *trainQ_ev;

                printf("passenger %d heading to train: %f\n", new_ev->passenger->pass_id, new_ev->event_time);

                trainQ_ev = event_create();
                trainQ_ev->passenger = new_ev->passenger;
                trainQ_ev->passenger->arrival_time = time_get();
                trainQ_ev->event_time = time_train();
                trainQ_ev->event_type = EV_TRAIN;
                event_schedule(trainQ_ev);

                break;
            }

            /* this is when she gets off the train  and heads for her gate */
            case (EV_TRAIN) :
            {
                event_t *train_ev;

                printf("passenger %d heading to gate: %f\n", new_ev->passenger->pass_id, new_ev->event_time);

                train_ev = event_create();
                train_ev->passenger = new_ev->passenger;
                train_ev->passenger->arrival_time = time_get();
                train_ev->event_time = time_gate();
                train_ev->event_type = EV_GATE;
                event_schedule(train_ev);

                break;
            }

            /* this is when she arrives at the gate */
            case (EV_GATE) :
            {
                printf("passenger %d arrived to gate: %f\n", new_ev->passenger->pass_id, new_ev->event_time);
                passenger_destroy(new_ev->passenger);

                break;
            }
        
            default :
                printf("ERROR: Default reached in case statement.\n");
                /* error */
                break;
        }
        /* free event */
        event_destroy(new_ev);
    }
    /* Print overall stats */
    /* finalize modules */
    event_finalize();
    time_finalize();
    return 0;
}

// static void parse_args(argc, argv)
// {
//     /* optional command line args processed here */
// }
