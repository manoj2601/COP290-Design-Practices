#ifndef ALLSTRUCTS_H
#define ALLSTRUCTS_H

struct costumer
{
	float arrTime; //random
};

typedef struct tellerline {
    struct costumer *items;
    // int capacity;
    int front;
    int rear;
    int total;
} tellerline;

struct teller
{
	tellerline tline;	//queue of the teller
	float idletime;//random idle time in the range of 1 - 600 seconds.
};

typedef struct Event
{
	int typeofevent;
	float eventtime;
	void* object;

	void (*fun_ptr)(struct teller **, int, float, float);
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



#endif