#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "tellerline.h"
#include "EventQueue.h"
#include "allstructs.h"
#include "helperfunctions.h"

int main(int argc, char** args)
{
	int totalcostumers = atoi(args[1]);
	int totaltellers = atoi(args[2]);
	float simulationTime = atof(args[3])*60;
	float averageServiceTime = atof(args[4])*60;
	struct teller* arrayTellers1[totaltellers]; //array of all tellers
	struct teller* arrayTellers2[totaltellers]; //array of all tellers
	struct costumer* arraycostumers1[totalcostumers];
	struct costumer* arraycostumers2[totalcostumers];
	head2 = (Node*) malloc(sizeof(Node)); //head2 of  EventQueue head2 is nothing but a node with CurrEvent NULL, first Node of LinkedList starts with head2->NextNode.
	head2->NextNode = NULL;
	head = (Node*) malloc(sizeof(Node)); //head of  EventQueue head is nothing but a node with CurrEvent NULL, first Node of LinkedList starts with head2->NextNode.
	head->NextNode = NULL;
	int k=0;
	for(k=0; k<totaltellers; k++)
	{	
		// struct teller t1;
		struct teller *t1 = (struct teller *) malloc(sizeof(struct teller));
		float rndmtime = rand()%600;
		t1->idletime = rndmtime;
		t1->totalIdleTime = rndmtime;
		t1->totalServiceTime = 0;
		TellerNode *tNode = (TellerNode *) malloc(sizeof(TellerNode));
		// tNode->CurrCostumer = NULL;
		tNode->NextNode = NULL;
		t1->Head = tNode;
		arrayTellers1[k] = t1;

		struct Event e1; //teller event
		e1.typeofevent = 5; //teller event
		e1.eventtime = t1->idletime; //teller idle time
		e1.object = t1; //teller ka object
		e1.fun_ptr = (void *) &nextjob_teller; //nextjob_teller ko call krega

		Node *tobeinsert = (Node*) malloc(sizeof(Node));
		tobeinsert->CurrEvent = e1;
		tobeinsert->NextNode = NULL;
		insertNode(head, tobeinsert);

		//struct teller t2
		struct teller *t2 = (struct teller *) malloc(sizeof(struct teller));
		t2->idletime = rndmtime;
		t2->totalIdleTime = rndmtime;
		t2->totalServiceTime = 0;
		// tellerline_init(&(t2->tline));
		TellerNode *tNode2 = (TellerNode *) malloc(sizeof(TellerNode));
		// tNode->CurrCostumer = NULL;
		tNode2->NextNode = NULL;
		t2->Head = tNode2;
		arrayTellers2[k] = t2;

		struct Event e2; //teller event
		e2.typeofevent = 3; //teller event
		e2.eventtime = t2->idletime; //teller idle time
		e2.object = t2; //teller ka object
		e2.fun_ptr = (void *) &nextjob_teller; //nextjob_teller ko call krega

		Node *tobeinsert2 = (Node*) malloc(sizeof(Node));
		tobeinsert2->CurrEvent = e2;
		tobeinsert2->NextNode = NULL;
		insertNode(head2, tobeinsert2);
	}
	//All tellers event inserted
	FILE*ft6 = fopen("output//arrTimes.txt", "w");
	for(k=0; k<totalcostumers; k++)
	{
		float arrTime = (simulationTime * rand())/(RAND_MAX+0.0);
		
		struct costumer *c1 = (struct costumer *) malloc(sizeof(struct costumer));
		c1->arrTime = arrTime;
		fprintf(ft6, "%f\n", arrTime);
		c1->WaitingTime = -1;
		c1->serviceTime = -1;
		arraycostumers1[k] = c1;
		struct Event e1;
		e1.typeofevent = 1;
		e1.object = arraycostumers1[k];
		e1.eventtime = c1->arrTime;
		e1.fun_ptr = (void *) &add_costumer1;

		Node *tobeinsert = (Node*) malloc(sizeof(Node));
		tobeinsert->CurrEvent = e1;
		tobeinsert->NextNode = NULL;
		insertNode(head, tobeinsert);


		struct costumer *c2 = (struct costumer *) malloc(sizeof(struct costumer));
		c2->arrTime = arrTime;
		c2->WaitingTime = -1;
		c2->serviceTime = -1;
		arraycostumers2[k] = c2;
		struct Event e2;
		e2.typeofevent = 1;
		e2.object = arraycostumers2[k];
		e2.eventtime = c2->arrTime;
		e2.fun_ptr = (void *) &add_costumer2;

		Node *tobeinsert2 = (Node*) malloc(sizeof(Node));
		tobeinsert2->CurrEvent = e2;
		tobeinsert2->NextNode = NULL;
		insertNode(head2, tobeinsert2);
	}
	
	FILE* output = fopen("output//output.txt", "a");
	
	//all costumer arrival events inserted.
	// Execution for separate line for each teller
	
	//printLinkedList(head);
	//sleep(3);
	//char lol[4];
	//scanf("%s", lol);
	// free(arraycostumers1);
	// free(arrayTellers1);
	// sleep(2);
	// printf("half completed\n");
	One_Each_Teller(totalcostumers, totaltellers, simulationTime, averageServiceTime, arrayTellers1, arraycostumers1, output);
	//sleep(3);	
	Common_Queue(totalcostumers, totaltellers, simulationTime, averageServiceTime, arrayTellers2, arraycostumers2, output);
	fprintf(output, "\n\n");		
	fclose(output);
	// printf("Manoj\n");
	return 0;
}


// Test case 
// total tellers = 1
// total costumers = 1
// arrTime of costumer = 4sec
// initial idle time of teller = 5sec
// service Time = 6sec //costant
// idle time later for teller = 20sec //constant 
