#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include "vector.h"
#include "tellerline.h"
#include "EventQueue.h"
#include "allstructs.h"
#include "helperfunctions.h"


void Common_Queue(int totalcostumers, int totaltellers, float simulationTime, float averageServiceTime, struct teller ** arrayTellers2, struct costumer ** arraycostumers2, FILE* output)
{
	float clk=0;
	FILE *ft = fopen("costumersStaticesCommonLine.txt", "w");
	FILE *ft2 = fopen("tellersStaticesCommonLine.txt", "w");
	fprintf(ft, "arrTime\t\tWaitingTime\tserviceTime\tTotalTimeinBank\n");
	totalServedCostumers2 = 0;
	simulation(arrayTellers2, totaltellers, clk, averageServiceTime, ft, ft2, simulationTime, head2);
	fprintf(ft2, "totalIdleTime\ttotalserviceTime\n");
	for(int i=0; i<totaltellers; i++)
	{
		fprintf(ft2, "%f\t%f\n", arrayTellers2[i]->totalIdleTime, arrayTellers2[i]->totalServiceTime);
	}
	fclose(ft);
	fclose(ft2);
	FILE *ft3 = fopen("costumersStaticesCommonLine.txt", "r");
	if(ft3 == NULL)
	{
		printf("experiment failed2\n");
		exit(1);
	}
	//printing output.txt
	char str[20];
	for(int i=0; i<4; i++)
		fscanf(ft3,"%s", str);
	float SpentTimeInBank[totalServedCostumers2];
	float MaxWaitingTime = 0;
	float totalSpentTimeInBank=0;
	for(int i=0; i<totalServedCostumers2; i++)
	{
			char arrTime[20];
			fscanf(ft3,"%s", arrTime);//arrival time
			char waitingTime[20];
			fscanf(ft3, "%s", waitingTime); //waiting time
			if(MaxWaitingTime < atof(waitingTime))
				MaxWaitingTime = atof(waitingTime);
			char serviceTime[20];
			SpentTimeInBank[i] = atof(waitingTime)+atof(serviceTime);
			totalSpentTimeInBank += SpentTimeInBank[i];
			fscanf(ft3, "%s", serviceTime); //servicetime
			fscanf(ft3, "%s", str);//clk on that time
			printf("clk is %s\n", str);
	}
	float avgSpentTime = totalSpentTimeInBank/totalServedCostumers2;
	float stdD = 0;
	for(int i=0; i<totalServedCostumers2; i++)
	{
		stdD += (SpentTimeInBank[i] - avgSpentTime)*(SpentTimeInBank[i] - avgSpentTime);
	}
	stdD = stdD/totalServedCostumers2;
	stdD = sqrt(stdD);
	FILE* ft4 = fopen("tellersStaticesSepLine.txt", "r");
	float totalServiceTime = 0;
	float totalIdleTime = 0;
	for(int i=0; i<2; i++)
		fscanf(ft4, "%s", str);
	for(int i=0; i<totaltellers; i++)
	{
		fscanf(ft4, "%s", str);
		totalIdleTime += atof(str);
		fscanf(ft4, "%s", str);
		totalServiceTime += atof(str);		
	}
	fprintf(output, "type of queuing : one per teller\n");
	fprintf(output, "total number of tellers : %d\n", totaltellers);
	fprintf(output, "total number of costumers served : %d\n", totalServedCostumers2);
	fprintf(output, "total time required to serve these costumers : %f\n", totalServiceTime);
	fprintf(output, "average Spent Time by a costumer : %f\t", avgSpentTime);
	fprintf(output, "standard deviation : %f\n", stdD);
	fprintf(output, "Maximum Waiting Time of a Costumer : %f\n", MaxWaitingTime);
	fprintf(output, "Total ServiceTime by all tellers : %f\n", totalServiceTime);
	fprintf(output, "Total IdleTime by all tellers : %f\n", totalIdleTime);
	printf("clk is : %f\n", clk);
}

int main(int argc, char** args)
{
	int totalcostumers = atoi(args[1]);
	int totaltellers = atoi(args[2]);
	float simulationTime = atof(args[3])*60;
	float averageServiceTime = atof(args[4])*60;
	//creating an array of tellers
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
		tellerline_init(&(t1->tline));
		arrayTellers1[k] = t1;

		struct Event e1; //teller event
		e1.typeofevent = 3; //teller event
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
		tellerline_init(&(t2->tline));
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

	for(k=0; k<totalcostumers; k++)
	{
		float arrTime = simulationTime * rand()/(RAND_MAX+0.0);
		
		struct costumer *c1 = (struct costumer *) malloc(sizeof(struct costumer));
		c1->arrTime = arrTime;
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
	FILE* output = fopen("output.txt", "w");
	//all costumer arrival events inserted.
	// Execution for separate line for each teller
	// One_Each_Teller(totalcostumers, totaltellers, simulationTime, averageServiceTime, arrayTellers1, arraycostumers1, output);
	Common_Queue(totalcostumers, totaltellers, simulationTime, averageServiceTime, arrayTellers2, arraycostumers2, output);
	exit(1);
}


// Test case 
// total tellers = 1
// total costumers = 1
// arrTime of costumer = 4sec
// initial idle time of teller = 5sec
// service Time = 6sec //costant
// idle time later for teller = 20sec //constant 
