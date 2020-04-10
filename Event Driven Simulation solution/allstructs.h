#ifndef ALLSTRUCTS_H
#define ALLSTRUCTS_H

struct costumer
{
	float arrTime; //random
	float WaitingTime;
	float serviceTime;
};

typedef struct tellerline {
    struct costumer **items;
    // int capacity;
    int front;
    int rear;
    int total;
} tellerline;

struct teller
{
	tellerline tline;	//queue of the teller
	float idletime;//random idle time in the range of 1 - 600 seconds.
	float totalIdleTime;
	float totalServiceTime;
};

typedef struct Event
{
	int typeofevent;
	float eventtime;
	void* object;

	void (*fun_ptr)(struct teller **, int, float, float, FILE*, FILE*, float, void** );
	// function pointer; //it will be used to invoke a function.
} Event;

typedef struct Node
{
	Event CurrEvent;
	struct Node *NextNode;
} Node;



#endif