/* event.h */

typedef struct event_s event_t;
typedef struct queue_s queue_t;
typedef struct priority_s priority_t;
typedef struct passenger_s passenger_t;

struct event_s { //typedef removed
   int event_type;        /* type of event â€“ see below */
   queue_t *queue;        /* queue passenger is waiting in */
   double event_time;     /* sim time when event occurs */
   passenger_t *passenger;/* passenger related to this event */
}; //event_t;

double event_time(event_t *e);

/* initializes events, creates a priority queue */
void event_init();

/* frees up all event space, including space in the priority queue */
void event_fini();

/* allocate a fresh event with empty fields */
event_t *event_create();

/* free an event */
void event_destroy(event_t *e);

/* insert the event into the priority queue.  The key
   value is the current sim time plus the event_time in
   event.  Update the event time to the key value. */
void event_schedule(event_t *e);

/* remove the next event from the priority and return it to the program for execution */
event_t *event_cause();

// function to check if event is empty
// returns 1 if empty, 0 if not
int event_empty(event_t *e);
