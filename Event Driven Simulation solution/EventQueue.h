#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

typedef struct Event
{
	int typeofevent;
	int eventtime;
	void* object;
	void (*fun_ptr)(int);
	/*
	TYPE 1 : Arrival of an costumer, 
				call the funtion add_costumer() add that costumer at the end of a teller line
	
	TYPE 2 : customer whose serivce has been completed
				call the function remove_costumer() callect following statics - total time in bank delete the Event object
	
	TYPE 3 : teller who has completed serving a costumer //after the completion of each costumer
				work on next costumer if any 
					remove the first costumer from its line, generate  random service time time according to the input parameters
					the program float serviceTime = 2*averageServiceTime*rand()/float(RAND_MAX);
					add two events 
						1. TYPE 2 
						2. TYPE 3
				OR if there is no customer in any line, put TYPE 4 event in the queue with a random idle time of 0-150 seconds.
	
	TYPE 4 : teller who has completed idle time task 

	*/


	// function pointer; //it will be used to invoke a function.

} Event;

typedef struct Node
{
	Event CurrEvent;
	struct Node *NextNode;
} Node;

typedef struct EventQueue {
    Node *head;
    int size;
} EventQueue;


void EventQueue_init (EventQueue *);
bool is_empty(EventQueue *);
void enQueueEvent(EventQueue *, Event);
Event deQueueEvent(EventQueue *);
int EventQueue_total(EventQueue *);
void printEventQueue(EventQueue *);



// void enQueue(tellerline *, struct costumer);
// struct costumer deQueue(tellerline *);

// static void queue_resize(tellerline *, int);
// void queue_free(tellerline *);

#endif