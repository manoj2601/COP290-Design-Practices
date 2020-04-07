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
	EventQueue eventq;
	EventQueue_init (&eventq);
	//creating an array of tellers
	struct teller arraytellers[totaltellers];
	
	for(int i=0; i<totaltellers; i++)
	{
		
		tellerline l1;
		tellerline_init(&l1);
		int idletime = rand()%600;
		printf("idletime is : %d\n", idletime);
		struct teller *t = {&l1, idletime};
		arraytellers[i] = *t;
		Event e = {4, idletime, (void* ) t,/*pointer of TYPE4 function*/ &idletellercomp};
		enQueueEvent(&eventq, e);
		printf("going to exit\n");
		// printEventQueue(&eventq);

	}
exit(1);
	//all tellers created and stored in an array of tellers "arraytellers"

	printf("size of arraytellers is : &d\n", totaltellers);
	printf("printing arraytellers\n");
	for(int i=0; i<totaltellers; i++)
	{
		printf("%dth teller's idle time : %f\n", i, arraytellers[i].idletime);
	}



	// for(int i=0; i<totalcostumers; i++)
	// {
	// 	float arrTime = simulationTime * (rand()%10)/9.0;
	// 	struct costumer c = {arrTime};
	// 	//now create an event of type 1
	// 	// Event* e;
	// 	// Event e = {1, v}
	// }


	// struct teller t1;
	// vector v;
	// vector_init(&v);
	// printf("Manoj Kumar ne start kr diya\n");
	// vector_add(&v, &t1);
	// printf("added element is : %f", ((struct teller *) vector_get(&v, 0))->idletime);

	// struct costumer c1 = {3.00};
	// tellerline l1;
	// tellerline_init (&l1);
	// enQueue(&l1, c1);

	// vector v;
	// vector_init(&v);

	// struct costumer costumerarray [totalcostumers];
	// struct teller tellerarray [totaltellers];
	// eventQueue = L


	// float arrTime = simulationTime * rand()/float(RAND_MAX);
	// float serviceTime = 2*averageServiceTime*rand()/float(RAND_MAX);
}