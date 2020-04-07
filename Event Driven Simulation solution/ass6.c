#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "vector.h"
#include "tellerline.h"
#include "EventQueue.h"
// struct costumer
// {
// 	float arrTime; //random
// };

struct teller
{
	tellerline tline;	//queue of the teller
	float idletime;//random idle time in the range of 1 - 600 seconds.
};

int* minindex(vector *v/*vector of teller structs*/)
{
	int length = vector_total(v);
	int sizes[length];
	for(int i=0; i<length; i++)
	{
		 struct teller *t = (struct teller *) vector_get(v, i);
		 sizes[i] = queue_total( &(t->tline) );
	}
	int minvalue = sizes[0];
	for(int i=1; i<length; i++)
	{
		if(sizes[i] < minvalue)
			minvalue = sizes[i];
	}


 	int ret[length];
 	// int retsize=0;
 	int k=0;
 	for(int i=0; i<length; i++)
 	{
 		if(sizes[i] == minvalue)
 		{
 			ret[k] = i;
 			k++;
 		}
 	}
	int* ret1 = malloc(k+1);
	for(int i=1; i<k; i++)
	{
		ret1[i] = ret[i];
	}
	ret1[k] = -1;
	return ret1;
}

//TYPE 1
void add_costumer(vector *v/*vector of teller structs*/, struct costumer c)
{
	
	int* mins = minindex(v);
	int totalmins = 0;
	while(mins[totalmins] != -1)
	{
		totalmins++;
	}
	int rnd = rand()%totalmins;
	//now we need to add costumer c in the mins[rnd]-th element of the vector v.
	int targetindex = mins[rnd];
	struct teller *t = vector_get(v, targetindex);
	//t->tline mein add krna hai c ko
	// vector_add(v->tline, &c);
}

//TYPE 2
int delete_costumer(int currentTime, struct costumer *c)
{
	return (currentTime - c->arrTime);
}

//TYPE 3
void nextjob_teller(struct teller *t, vector *v/*vector of teller structs*/)
{
	
}

//TYPE 4
void idletellercomp()
{

}

struct NodeEventQueue
{
	int data;
	struct Node* next;
};


int main(int argc, char** args)
{
	int totalcostumers = atoi(args[1]);
	int totaltellers = atoi(args[2]);
	float simulationTime = atof(args[3]);
	float averageServiceTime = atof(args[4]);
	
	vector equeue;
	vector_init (&equeue);
	
	//creating an array of tellers
	struct teller arraytellers[totaltellers];
	// printEventQueue(&eventq);
	// exit(1);
	//all tellers created and stored in an array of tellers "arraytellers"


	// for(int i=0; i<totalcostumers; i++)
	// {
	// 	float arrTime = simulationTime * (rand()%10)/9.0;
	// 	struct costumer c = {arrTime};
	// 	//now create an event of type 1
	// 	// Event* e;
	// 	// Event e = {1, v}
	// }

	

	// EventQueue eventq;
	// EventQueue_init (&eventq);
	struct teller t1;
	t1.idletime = rand()%600;

	struct Event e1;// = {4, t1.idletime, (void* ) t1, &add_costumer};
	e1.typeofevent = 4;
	e1.eventtime = t1.idletime;
	e1.object = &t1;
	e1.fun_ptr = &idletellercomp;
	// enQueueEvent(&eventq, e1);
	Node head1 = {e1, NULL};
	Node *head= &head1;
	
	(*head).CurrEvent = e1;
	
	(*head).NextNode = NULL;

	vector v;
	vector_init(&v);
	vector_add(&v, &t1);

	struct teller t2;
	t2.idletime = rand()%600;
	vector_add(&v, &t2);

	struct Event e2;// = {4, t1.idletime, (void* ) t1, &add_costumer};
	e2.typeofevent = 4;
	e2.eventtime = t2.idletime;
	e2.object = &t2;
	e2.fun_ptr = &idletellercomp;
	// enQueueEvent(&eventq, e2);

	Node *temp = head;

	while((*temp).NextNode != NULL)
	{
		temp = &((*temp).NextNode);
		continue;
	}
	printf("Manoj Kumar ne start kr diya\n");
	Node n;// = {e2, NULL};
	n.CurrEvent = e2;
	n.NextNode = NULL;
	(*temp).NextNode = &n;
	printf("Manoj Kumar ne start kr diya\n");

	struct teller t3;
	t3.idletime = rand()%600;
	vector_add(&v, &t3);

	struct Event e3;// = {4, t1.idletime, (void* ) t1, &add_costumer};
	e3.typeofevent = 4;
	e3.eventtime = t3.idletime;
	e3.object = &t3;
	e3.fun_ptr = &idletellercomp;
	// enQueueEvent(&eventq, e3);
	Node *temp2 = head;
	while(temp2->NextNode != NULL)
	{
		temp2 = &(temp2->NextNode);
		continue;
	}
	Node n2;// = {e2, NULL};
	n2.CurrEvent = e3;
	n2.NextNode = NULL;
	temp2->NextNode = &n2;
	// temp2->NextNode->CurrEvent = e3;

	int i=0;
	for(i=0; i<3; i++)
		printf("added element is : %f\n", ((struct teller *) vector_get(&v, i))->idletime);

	Node *temp3 = head;
	if(temp3->NextNode != NULL)
	{
		printf("laul%d, %d\n", temp3->CurrEvent.typeofevent, temp3->CurrEvent.eventtime);
		temp3 = &(head->NextNode);
	}
	printf("lol%d, %d\n", temp3->CurrEvent.typeofevent, temp3->CurrEvent.eventtime);

	// struct costumer c1 = {3.00};
	// tellerline l1;
	// tellerline_init (&l1);
	// enQueue(&l1, c1);

	// vector v;
	// vector_init(&v);

	// struct costumer costumerarray [totalcostumers];
	// struct teller tellerarray [totaltellers];
	// // eventQueue = L


	// float arrTime = simulationTime * rand()/float(RAND_MAX);
	// float serviceTime = 2*averageServiceTime*rand()/float(RAND_MAX);
}