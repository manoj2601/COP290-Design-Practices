#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

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

int totalServedCostumers1 = 0;
int totalServedCostumers2 = 0;

void printLinkedList(Node *head3)
{
	Node temp = head3[0];
	if(temp.NextNode != NULL)
	{
		temp = (temp.NextNode)[0];
	}
	else return;
	while(temp.NextNode != NULL)
	{
		printf("original %d, %f\n", temp.CurrEvent.typeofevent, temp.CurrEvent.eventtime);
		if(temp.CurrEvent.typeofevent == 2)
		{
			printf("since delete_costumer so arrTime : %f\n", ((struct costumer *)temp.CurrEvent.object)->arrTime);
		}
		else if(temp.CurrEvent.typeofevent == 1)
		{
			printf("since add_costumer1 so arrTime : %f\n", ((struct costumer *)temp.CurrEvent.object)->arrTime);
		}
		temp = (temp.NextNode)[0];
		//sleep(0.1);
	}
	printf("last %d, %f\n\n", temp.CurrEvent.typeofevent, temp.CurrEvent.eventtime);
		if(temp.CurrEvent.typeofevent == 2)
		{
			printf("since delete_costumer so arrTime : %f\n", ((struct costumer *)temp.CurrEvent.object)->arrTime);
		}
		else if(temp.CurrEvent.typeofevent == 1)
		{
			printf("since add_costumer1 so arrTime : %f\n", ((struct costumer *)temp.CurrEvent.object)->arrTime);
		}
}

int* minindex(struct teller* arrayTellers1[], int totaltellers)
{
	int length = totaltellers;//length of arrayTellers1 i.e. total tellers.
	int sizes[length];
	for(int i=0; i<length; i++)
	{
		 struct teller *t = arrayTellers1[i];
		 sizes[i] = queue_total( &(t->tline) );
	}

	int minvalue = sizes[0];
	for(int i=1; i<length; i++)
	{
		if(sizes[i] < minvalue)
			minvalue = sizes[i];
	}
 	int ret[length];
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
void add_costumer1(struct teller* arrayTellers1[], int totaltellers, float clk, float averageServiceTime, FILE* ft, FILE *ft2, float simulationTime, Node *head3)//struct teller* arrayTellers1[0], Node* head, Event e)
{
	Event curr = head3->NextNode->CurrEvent; //Event on which we are working
	head3->NextNode = head3->NextNode->NextNode;	//removing the head of EventQueue
	struct costumer *c = ((struct costumer *) curr.object);
	int* mins = minindex(arrayTellers1, totaltellers);	//all indices of tellers with minimum costumers 
	int totalmins=0;	//size of mins
	while(mins[totalmins] != -1)
		totalmins++;
	int rnd = rand()%totalmins;
	//now we need to add costumer c in the mins[rnd]-th element of the vector v.
	int targetindex = mins[rnd];
	enQueue(&(arrayTellers1[targetindex]->tline), c);
}

void add_costumer2(struct teller* arrayTellers2[], int totaltellers, float clk, float averageServiceTime, FILE* ft, FILE *ft2, float simulationTime, Node *head3)//struct teller* arrayTellers1[0], Node* head, Event e)
{
	Event curr = head3->NextNode->CurrEvent; //Event on which we are working
	head3->NextNode = head3->NextNode->NextNode;	//removing the head of EventQueue
	struct costumer *c = ((struct costumer *) curr.object);
	int targetindex = 0; //add all the costumers in the same tellerline (0th)
	enQueue(&(arrayTellers2[targetindex]->tline), c);
}

//TYPE 2
void delete_costumer(struct teller* arrayTellers1[], int totaltellers, float clk, float averageServiceTime, FILE* ft, FILE *ft2, float simulationTime, Node* head3)
{
	Event curr = (head3->NextNode->CurrEvent); //Event on which we are working
	struct costumer *c = ((struct costumer *) curr.object);
	head3->NextNode = head3->NextNode->NextNode;	//removing the head of EventQueue
	fprintf(ft, "%f\t%f\t%f\t%f\n", c->arrTime, c->WaitingTime, c->serviceTime, clk);
	totalServedCostumers1++;
	/*gather information about the costumer
		print total time in bank = clk - c.arrTime;
		print total waiting time = c.totalWaitingTime;
		print total service time = c.totalserviceTime;
	*/
	
}

//TYPE 3
void nextjob_teller(struct teller* arrayTellers1[], int totaltellers, float clk, float averageServiceTime, FILE* ft, FILE *ft2, float simulationTime, Node* head3)
{
	
	Event curr = head3->NextNode->CurrEvent; //Event on which we are working
	head3->NextNode = head3->NextNode->NextNode;	//removing the head of EventQueue
	if(clk == simulationTime)
	{
		return;
	}
	struct teller *t = ((struct teller *) curr.object);
	//gather statics about the teller

	//assigning new work to the teller
	if(queue_total(&(t->tline)) > 0)
	{
		// printf("picking 1st costumer from his own line\n");
		struct costumer *c = deQueue(&(t->tline));
		float serviceTime = 2*averageServiceTime*rand()/(RAND_MAX + 0.0);
		//insert delete_costumer Event in EventQueue
		//If serviceTime is greater then simulationTime than we will take serviceTime is upto simulationTime.
		if(clk + serviceTime > simulationTime)
			serviceTime = simulationTime-clk;

		t->totalServiceTime += serviceTime;
		c->WaitingTime = clk - c->arrTime;
		c->serviceTime = serviceTime;

		struct Event e1; //costumer completion event
		e1.typeofevent = 2;
		e1.eventtime = clk + serviceTime;
		e1.object = (void *) malloc(sizeof(struct costumer));
		e1.object = c;
		e1.fun_ptr = (void *)&delete_costumer;

		Node *tobeinsert = (Node*) malloc(sizeof(Node));
		tobeinsert->CurrEvent = e1;
		tobeinsert->NextNode = NULL;

		//Insert nextjob_teller Event in EventQueue
		struct Event e2; //teller event
		e2.typeofevent = 3;
		e2.eventtime = clk + serviceTime;
		e2.object = t;
		e2.fun_ptr = (void *)&nextjob_teller;
		Node *tobeinsert2 = (Node*) malloc(sizeof(Node));
		tobeinsert2->CurrEvent = e2;
		tobeinsert2->NextNode = NULL;
		insertNode(head3,  tobeinsert2);
		insertNode(head3,  tobeinsert);
		return;
	}
	else
	{
		int nonzerotellers[totaltellers];
		int sizenzerots=0;
		for(int i=0; i<totaltellers; i++)
		{
			if(queue_total(&(arrayTellers1[i]->tline)) > 0)
			{
				nonzerotellers[sizenzerots] = i;
				sizenzerots++;
			}
		}
		if(sizenzerots != 0)
		{
			// printf("picking costumer from other teller's queue\n");
			int r = rand()%sizenzerots;
			struct costumer *c = deQueue(&(arrayTellers1[nonzerotellers[r]]->tline));
			float serviceTime = 2*averageServiceTime*rand()/(RAND_MAX + 0.0);
			if(clk + serviceTime > simulationTime)
				serviceTime = simulationTime-clk;

			t->totalServiceTime += serviceTime;
			c->WaitingTime = clk - c->arrTime;
			c->serviceTime = serviceTime;

			//insert delete_costumer Event in EventQueue
			struct Event e1;
			e1.typeofevent = 2;
			e1.eventtime = clk + serviceTime;
			e1.object = c;
			e1.fun_ptr = (void *)&delete_costumer;
			Node *tobeinsert = (Node*) malloc(sizeof(Node));
			tobeinsert->CurrEvent = e1;
			tobeinsert->NextNode = NULL;

			//Insert nextjob_teller Event in EventQueue
			struct Event e2;
			e2.typeofevent = 3;
			e2.eventtime = clk + serviceTime;
			e2.object = t;
			e2.fun_ptr = (void *)&nextjob_teller;
			Node *tobeinsert2 = (Node*) malloc(sizeof(Node));
			tobeinsert2->CurrEvent = e2;
			tobeinsert2->NextNode = NULL;
			insertNode(head3,  tobeinsert2);
			insertNode(head3,  tobeinsert);
		}
		else
		{
			t->idletime = rand()%150;
			// printf("teller going for idleTime %f\n", t->idletime);
			if(t->idletime + clk > simulationTime)
			{
				t->idletime = simulationTime - clk;
			}
			t->totalIdleTime += t->idletime;	
			struct Event e1;
			e1.typeofevent = 3;
			e1.eventtime = clk + t->idletime;
			e1.object = t;
			e1.fun_ptr = (void *)&nextjob_teller;
			Node *tobeinsert = (Node*) malloc(sizeof(Node));
			tobeinsert->CurrEvent = e1;
			tobeinsert->NextNode = NULL;
			insertNode(head3, tobeinsert);
		}
	}
}

void simulation(struct teller* arrayTellers1[], int totaltellers, float clk, float averageServiceTime, FILE* ft, FILE *ft2, float simulationTime, Node *head3)
{
	clk = 0;
	while(clk <= simulationTime)
	{
		if(head3->NextNode == NULL)
			break;
		clk = head3->NextNode->CurrEvent.eventtime;
		if(clk >simulationTime) {
			clk = simulationTime;
			break;
		}
		//Invoking function
		if(head3->NextNode->CurrEvent.fun_ptr == (void *) &delete_costumer)
			{
				printf("delete_costumer starting\n");
			}
		else if(head3->NextNode->CurrEvent.fun_ptr == (void *) &add_costumer1)
			{
				printf("add_costumer1 starting\n");
			}
		else if (head3->NextNode->CurrEvent.fun_ptr == (void *) &nextjob_teller)
			{
				printf("nextjob_teller starting\n");
			}
		else if(head3->NextNode->CurrEvent.fun_ptr == (void *) &add_costumer2)
			{
				printf("add_costumer2 starting\n");
			}		
		Event eve = head3->NextNode->CurrEvent;
		eve.fun_ptr(arrayTellers1, totaltellers, clk, averageServiceTime, ft, ft2, simulationTime, (void *)head3);
		printf("Event completed %f\n", clk);
	}
}

void One_Each_Teller(int totalcostumers, int totaltellers, float simulationTime, float averageServiceTime, struct teller ** arrayTellers1, struct costumer ** arraycostumers, FILE* output)
{
	float clk=0;
	FILE *ft = fopen("costumersStaticesSepLine.txt", "w");
	FILE *ft2 = fopen("tellersStaticesSepLine.txt", "w");
	fprintf(ft, "arrTime\t\tWaitingTime\tserviceTime\tTotalTimeinBank\n");
	totalServedCostumers1 = 0;
	simulation(arrayTellers1, totaltellers, clk, averageServiceTime, ft, ft2, simulationTime, head);
	fprintf(ft2, "totalIdleTime\ttotalserviceTime\n");
	for(int i=0; i<totaltellers; i++)
	{
		fprintf(ft2, "%f\t%f\n", arrayTellers1[i]->totalIdleTime, arrayTellers1[i]->totalServiceTime);
	}
	fclose(ft);
	fclose(ft2);
	FILE *ft3 = fopen("costumersStaticesSepLine.txt", "r");
	if(ft3 == NULL)
	{
		printf("experiment failed\n");
		exit(1);
	}
	//printing output.txt
	char str[20];
	for(int i=0; i<4; i++)
		fscanf(ft3,"%s", str);
	float SpentTimeInBank[totalServedCostumers1];
	float MaxWaitingTime = 0;
	float totalSpentTimeInBank=0;
	for(int i=0; i<totalServedCostumers1; i++)
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
	float avgSpentTime = totalSpentTimeInBank/totalServedCostumers1;
	float stdD = 0;
	for(int i=0; i<totalServedCostumers1; i++)
	{
		stdD += (SpentTimeInBank[i] - avgSpentTime)*(SpentTimeInBank[i] - avgSpentTime);
	}
	stdD = stdD/totalServedCostumers1;
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
	fprintf(output, "total number of costumers served : %d\n", totalServedCostumers1);
	fprintf(output, "total time required to serve these costumers : %f\n", totalServiceTime);
	fprintf(output, "average Spent Time by a costumer : %f\t", avgSpentTime);
	fprintf(output, "standard deviation : %f\n", stdD);
	fprintf(output, "Maximum Waiting Time of a Costumer : %f\n", MaxWaitingTime);
	fprintf(output, "Total ServiceTime by all tellers : %f\n", totalServiceTime);
	fprintf(output, "Total IdleTime by all tellers : %f\n", totalIdleTime);
	printf("clk is : %f\n", clk);
}

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

#endif