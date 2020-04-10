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
#include "helperSepQueue.h"


void Common_Queue(int totalcostumers, int totaltellers, float simulationTime, float averageServiceTime, struct teller ** arraytellers, struct costumer ** arraycostumers, FILE* output)
{
	float clk=0;
	FILE *ft = fopen("costumersStaticesCommonLine.txt", "w");
	FILE *ft2 = fopen("tellersStaticesCommonLine.txt", "w");
	fprintf(ft, "arrTime\t\tWaitingTime\tserviceTime\tTotalTimeinBank\n");
	totalServedCostumers2 = 0;
	while(clk <= simulationTime)
	{
		if(head2->NextNode == NULL)
			break;
		clk = head2->NextNode->CurrEvent.eventtime;
		if(clk >simulationTime) {
			clk = simulationTime;
			break;
		}
		//Invoking function
		if(head2->NextNode->CurrEvent.fun_ptr == &delete_costumer)
			{
				printf("delete_costumer starting\n");
			}
		else if(head2->NextNode->CurrEvent.fun_ptr == &add_costumer1)
			{
				printf("add_costumer1 starting\n");
			}
		else if (head2->NextNode->CurrEvent.fun_ptr == &nextjob_teller1)
			{
				printf("nextjob_teller1 starting\n");
			}		
		Event eve = head2->NextNode->CurrEvent;
		eve.fun_ptr(arraytellers, totaltellers, clk, averageServiceTime, ft, ft2, simulationTime);
		printf("Event completed %f\n", clk);
		
	}
	
	fprintf(ft2, "totalIdleTime\ttotalserviceTime\n");
	for(int i=0; i<totaltellers; i++)
	{
		fprintf(ft2, "%f\t%f\n", arraytellers[i]->totalIdleTime, arraytellers[i]->totalServiceTime);
	}
	fclose(ft);
	fclose(ft2);
	FILE *ft3 = fopen("costumersStaticesSepLine.txt", "r");
	if(ft3 == NULL)
	{
		printf("experiment failed\n");
		exit(1);
	}
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
	exit(1);
	printf("clk is : %f\n", clk);
}

int main(int argc, char** args)
{
	int totalcostumers = atoi(args[1]);
	int totaltellers = atoi(args[2]);
	float simulationTime = atof(args[3])*60;
	float averageServiceTime = atof(args[4])*60;
	//creating an array of tellers
	struct teller* arraytellers[totaltellers]; //array of all tellers
	struct costumer* arraycostumers[totalcostumers];
	head2 = (Node*) malloc(sizeof(Node)); //head2 of  EventQueue head2 is nothing but a node with CurrEvent NULL, first Node of LinkedList starts with head2->NextNode.
	head2->NextNode = NULL;
	head = (Node*) malloc(sizeof(Node)); //head of  EventQueue head is nothing but a node with CurrEvent NULL, first Node of LinkedList starts with head2->NextNode.
	head->NextNode = NULL;
	int k=0;
	for(k=0; k<totaltellers; k++)
	{	
		// struct teller t2;
		struct teller *t1 = (struct teller *) malloc(sizeof(struct teller));
		float rndmtime = rand()%600;
		t1->idletime = rndmtime;
		t1->totalIdleTime = rndmtime;
		t1->totalServiceTime = 0;
		tellerline_init(&(t1->tline));
		arraytellers[k] = t1;

		struct Event e1; //teller event
		e1.typeofevent = 3; //teller event
		e1.eventtime = t1->idletime; //teller idle time
		e1.object = t1; //teller ka object
		e1.fun_ptr = &nextjob_teller1; //nextjob_teller1 ko call krega

		Node *tobeinsert = (Node*) malloc(sizeof(Node));
		tobeinsert->CurrEvent = e1;
		tobeinsert->NextNode = NULL;
		insertNode1(tobeinsert);
	}
	//All tellers event inserted
	for(k=0; k<totalcostumers; k++)
	{
		float arrTime = simulationTime * rand()/(RAND_MAX+0.0);
		struct costumer *c = (struct costumer *) malloc(sizeof(struct costumer));
		c->arrTime = arrTime;
		c->WaitingTime = -1;
		c->serviceTime = -1;
		arraycostumers[k] = c;
		struct Event e1;
		e1.typeofevent = 1;
		e1.object = arraycostumers[k];
		e1.eventtime = c->arrTime;
		e1.fun_ptr = &add_costumer1;

		Node *tobeinsert = (Node*) malloc(sizeof(Node));
		tobeinsert->CurrEvent = e1;
		tobeinsert->NextNode = NULL;
		insertNode1( tobeinsert);
	}
	FILE* output = fopen("output.txt", "w");
	//all costumer arrival events inserted.
	// Execution for separate line for each teller
	One_Each_Teller(totalcostumers, totaltellers, simulationTime, averageServiceTime, arraytellers, arraycostumers, output);
	// Common_Queue(totalcostumers, totaltellers, simulationTime, averageServiceTime, arraytellers, arraycostumers, output);
	exit(1);
}


// Test case 
// total tellers = 1
// total costumers = 1
// arrTime of costumer = 4sec
// initial idle time of teller = 5sec
// service Time = 6sec //costant
// idle time later for teller = 20sec //constant 
