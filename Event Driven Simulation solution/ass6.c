#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <unistd.h>

#include "vector.h"
#include "tellerline.h"
#include "EventQueue.h"
#include "allstructs.h"
// struct costumer
// {
// 	float arrTime; //random
// };
Node *head;

void insertNode(Node *head, Node *tobeinsert)
{
		if(head->NextNode == NULL) //zero elements in the EventQueue
		{
			head->NextNode = tobeinsert;
			return;
		}

		bool inend=false;
		Node *curr = head->NextNode;
		Node *prev = head;
		while(curr->CurrEvent.eventtime < tobeinsert->CurrEvent.eventtime)
		{
			if(curr->NextNode == NULL)
			{
				inend = true;
				break;
			}
			prev = curr;
			curr = curr->NextNode;
		}
		if(inend)
		{
			curr->NextNode = tobeinsert;
			return;
		}
		else
		{
			tobeinsert->NextNode = curr;
			prev->NextNode = tobeinsert;
			return;
		}
}

int* minindex(struct teller* arraytellers[], int totaltellers)
{
	int length = totaltellers;//length of arraytellers i.e. total tellers.
	int sizes[length];
	for(int i=0; i<length; i++)
	{
		 struct teller *t = arraytellers[i];
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
void add_costumer(struct teller* arraytellers[], int totaltellers, float clk, float averageServiceTime)//struct teller* arraytellers[0], Node* head, Event e)
{
	Event curr = head->NextNode->CurrEvent; //Event on which we are working
	head->NextNode = head->NextNode->NextNode;	//removing the head of EventQueue
	struct costumer c = *((struct costumer *) curr.object);
	int* mins = minindex(arraytellers, totaltellers);	//all indices of tellers with minimum costumers 
	int totalmins=0;	//size of mins
	while(mins[totalmins] != -1)
	{
		totalmins++;
	}
	int rnd = rand()%totalmins;
	//now we need to add costumer c in the mins[rnd]-th element of the vector v.
	int targetindex = mins[rnd];
	enQueue(&(arraytellers[targetindex]->tline), c);
}

//TYPE 2
void delete_costumer(struct teller* arraytellers[], int totaltellers, float clk, float averageServiceTime)
{
	Event curr = head->NextNode->CurrEvent; //Event on which we are working
	head->NextNode = head->NextNode->NextNode;	//removing the head of EventQueue
	struct costumer c = *((struct costumer *) curr.object);
	//gather information about the costumer
	float totaltime = clk - c.arrTime;
}

//TYPE 3
void nextjob_teller(struct teller* arraytellers[], int totaltellers, float clk, float averageServiceTime)
{
	Event curr = head->NextNode->CurrEvent; //Event on which we are working
	head->NextNode = head->NextNode->NextNode;	//removing the head of EventQueue
	struct teller *t = ((struct teller *) curr.object);
	//gather statics about the teller

	//assigning new work to the teller
	if(queue_total(&(t->tline)) != 0)
	{
		struct costumer c = deQueue(&(t->tline));
		float serviceTime = 2*averageServiceTime*rand()/(RAND_MAX + 0.0);
		//insert delete_costumer Event in EventQueue
		struct Event e1;
		e1.typeofevent = 2;
		e1.eventtime = clk + serviceTime;
		e1.object = &c;
		e1.fun_ptr = &delete_costumer;
		Node *tobeinsert = (Node*) malloc(sizeof(Node));
		tobeinsert->CurrEvent = e1;
		tobeinsert->NextNode = NULL;
		insertNode(head, tobeinsert);
		//Insert nextjob_teller Event in EventQueue
		struct Event e2;
		e2.typeofevent = 3;
		e2.eventtime = clk + serviceTime;
		e2.object = t;
		e2.fun_ptr = &nextjob_teller;
		Node *tobeinsert2 = (Node*) malloc(sizeof(Node));
		tobeinsert2->CurrEvent = e2;
		tobeinsert2->NextNode = NULL;
		insertNode(head, tobeinsert2);	
	}
	else
	{
		int nonzerotellers[totaltellers];
		int sizenzerots=0;
		for(int i=0; i<totaltellers; i++)
		{
			if(queue_total(&(arraytellers[i]->tline)) != 0)
			{
				nonzerotellers[sizenzerots] = i;
				sizenzerots++;
			}
		}
		if(sizenzerots != 0)
		{
			int r = rand()%sizenzerots;
			struct costumer c = deQueue(&(arraytellers[r]->tline));
			float serviceTime = 2*averageServiceTime*rand()/(RAND_MAX + 0.0);
			//insert delete_costumer Event in EventQueue
			struct Event e1;
			e1.typeofevent = 2;
			e1.eventtime = clk + serviceTime;
			e1.object = &c;
			e1.fun_ptr = &delete_costumer;
			Node *tobeinsert = (Node*) malloc(sizeof(Node));
			tobeinsert->CurrEvent = e1;
			tobeinsert->NextNode = NULL;
			insertNode(head, tobeinsert);
			//Insert nextjob_teller Event in EventQueue
			struct Event e2;
			e2.typeofevent = 3;
			e2.eventtime = clk + serviceTime;
			e2.object = t;
			e2.fun_ptr = &nextjob_teller;
			Node *tobeinsert2 = (Node*) malloc(sizeof(Node));
			tobeinsert2->CurrEvent = e2;
			tobeinsert2->NextNode = NULL;
			insertNode(head, tobeinsert2);
		}
		else
		{
			// struct teller *t1 = (struct teller *) malloc(sizeof(struct teller));
			t->idletime = rand()%150;
			// tellerline_init(&(t->tline));
			// arraytellers[k] = t;

			struct Event e1;
			e1.typeofevent = 3;
			e1.eventtime = clk + t->idletime;
			e1.object = t;
			e1.fun_ptr = &nextjob_teller;
			Node *tobeinsert = (Node*) malloc(sizeof(Node));
			tobeinsert->CurrEvent = e1;
			tobeinsert->NextNode = NULL;
			insertNode(head, tobeinsert);
		}

	}

}

void printLinkedList(Node *head)
{
	Node temp = *head;
	if(temp.NextNode != NULL)
	{
		temp = *(temp.NextNode);
	}
	else return;
	while(temp.NextNode != NULL)
	{
		printf("original %d, %f\n", temp.CurrEvent.typeofevent, temp.CurrEvent.eventtime);
		temp = *(temp.NextNode);
		sleep(0.1);
	}
	printf("last %d, %f\n\n", temp.CurrEvent.typeofevent, temp.CurrEvent.eventtime);
}




int main(int argc, char** args)
{
	int totalcostumers = atoi(args[1]);
	int totaltellers = atoi(args[2]);
	float simulationTime = atof(args[3])*60;
	float averageServiceTime = atof(args[4])*60;
	
	vector equeue;
	vector_init (&equeue);
	//creating an array of tellers
	struct teller* arraytellers[totaltellers]; //array of all tellers
	head = (Node*) malloc(sizeof(Node)); //Head of  EventQueue Head is nothing but a node with CurrEvent NULL, first Node of LinkedList starts with head->NextNode.
	head->NextNode = NULL;
	int k=0;
	for(k=0; k<totaltellers; k++)
	{	
		// struct teller t2;
		struct teller *t1 = (struct teller *) malloc(sizeof(struct teller));
		t1->idletime = rand()%600;
		tellerline_init(&(t1->tline));
		arraytellers[k] = t1;

		struct Event e1;
		e1.typeofevent = 3;
		e1.eventtime = t1->idletime;
		e1.object = t1;
		e1.fun_ptr = &nextjob_teller;

		Node *tobeinsert = (Node*) malloc(sizeof(Node));
		tobeinsert->CurrEvent = e1;
		tobeinsert->NextNode = NULL;
		insertNode(head, tobeinsert);
	}
	printLinkedList(head);
	printf("initially all tellers added\n");
	sleep(3);
	//All tellers event inserted

	for(k=0; k<totalcostumers; k++)
	{
		float arrTime = simulationTime * rand()/(RAND_MAX+0.0);
		printf("arrTime is : %f\n", arrTime);

		struct costumer c = {arrTime};
		
		struct Event e1;
		e1.typeofevent = 1;
		e1.object = &c;
		e1.eventtime = c.arrTime;
		e1.fun_ptr = &add_costumer;

		Node *tobeinsert = (Node*) malloc(sizeof(Node));
		tobeinsert->CurrEvent = e1;
		tobeinsert->NextNode = NULL;
		insertNode(head, tobeinsert);
	}
	printLinkedList(head);
	printf("initially all costumers added\n");
	sleep(3);
	// Execution start
	float clk=0;
	while(clk <= simulationTime)
	{
		if(head->NextNode == NULL)
			break;
		clk = head->NextNode->CurrEvent.eventtime;
		//Invoking function
		head->NextNode->CurrEvent.fun_ptr(arraytellers, totaltellers, clk, averageServiceTime);
		printf("Event completed %f\n", clk);
	}
	exit(1);


	// float arrTime = simulationTime * rand()/float(RAND_MAX);
	// float serviceTime = 2*averageServiceTime*rand()/float(RAND_MAX);
}