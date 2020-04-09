#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <unistd.h>

#include "vector.h"
#include "tellerline.h"
#include "EventQueue.h"
#include "allstructs.h"

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

void printLinkedList()
{
	Node temp = head;
	if(temp.NextNode != NULL)
	{
		temp = *(temp.NextNode);
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
			printf("since add_costumer so arrTime : %f\n", ((struct costumer *)temp.CurrEvent.object)->arrTime);
		}
		temp = *(temp.NextNode);
		sleep(0.1);
	}
	printf("last %d, %f\n\n", temp.CurrEvent.typeofevent, temp.CurrEvent.eventtime);
		if(temp.CurrEvent.typeofevent == 2)
		{
			printf("since delete_costumer so arrTime : %f\n", ((struct costumer *)temp.CurrEvent.object)->arrTime);
		}
		else if(temp.CurrEvent.typeofevent == 1)
		{
			printf("since add_costumer so arrTime : %f\n", ((struct costumer *)temp.CurrEvent.object)->arrTime);
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
void add_costumer(struct teller* arraytellers[], int totaltellers, float clk, float averageServiceTime, FILE* ft)//struct teller* arraytellers[0], Node* head, Event e)
{
	printf("adding costumer\n");
	Event curr = head->NextNode->CurrEvent; //Event on which we are working
	head->NextNode = head->NextNode->NextNode;	//removing the head of EventQueue
	struct costumer c = *((struct costumer *) curr.object);
	int* mins = minindex(arraytellers, totaltellers);	//all indices of tellers with minimum costumers 
	int totalmins=0;	//size of mins
	while(mins[totalmins] != -1)
		totalmins++;
	int rnd = rand()%totalmins;
	//now we need to add costumer c in the mins[rnd]-th element of the vector v.
	int targetindex = mins[rnd];
	enQueue(&(arraytellers[targetindex]->tline), c);
}

//TYPE 2
void delete_costumer(struct teller* arraytellers[], int totaltellers, float clk, float averageServiceTime, FILE* ft)
{
	printf("deleting costumer\n");
	
	Event curr = (head->NextNode->CurrEvent); //Event on which we are working
	struct costumer c = *((struct costumer *) curr.object);
	// fprintf(ft, "deleting costumer %f, %f, %f\n", ((struct costumer *) curr->object)->arrTime, c.WaitingTime, c.serviceTime);
	head->NextNode = head->NextNode->NextNode;	//removing the head of EventQueue
	
	/*gather information about the costumer
		print total time in bank = clk - c.arrTime;
		print total waiting time = c.totalWaitingTime;
		print total service time = c.totalserviceTime;
	*/
	
}

//TYPE 3
void nextjob_teller(struct teller* arraytellers[], int totaltellers, float clk, float averageServiceTime, FILE* ft)
{
	printf("teller looking for next job\n");	
	Event curr = head->NextNode->CurrEvent; //Event on which we are working
	head->NextNode = head->NextNode->NextNode;	//removing the head of EventQueue
	struct teller *t = ((struct teller *) curr.object);
	//gather statics about the teller
	//assigning new work to the teller
	if(queue_total(&(t->tline)) > 0)
	{
		printf("picking 1st costumer from his own line\n");
		struct costumer c = deQueue(&(t->tline));
		float serviceTime = 6;//2*averageServiceTime*rand()/(RAND_MAX + 0.0);
		//insert delete_costumer Event in EventQueue
		t->totalServiceTime += serviceTime;
		c.WaitingTime = clk - c.arrTime;
		c.serviceTime = serviceTime;
		fprintf(ft,"%f, %f, %f\n", c.arrTime, c.WaitingTime, c.serviceTime);
		struct costumer *c1 = (struct costumer *) malloc(sizeof(struct costumer));
		c1 = &c;
		struct Event e1;
		e1.typeofevent = 2;
		e1.eventtime = clk + serviceTime;
		e1.object = (void *) malloc(sizeof(struct costumer));
		e1.object = c1;
		
		e1.fun_ptr = &delete_costumer;

		Node *tobeinsert = (Node*) malloc(sizeof(Node));
		tobeinsert->CurrEvent = e1;
		tobeinsert->NextNode = NULL;

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
		printf("tellerline se dequeue k baad ka arrTime : %f\n", ((struct costumer *)(tobeinsert->CurrEvent).object)->arrTime);
		insertNode(head, tobeinsert);
		printLinkedList();
		return;
	}
	else
	{
		int nonzerotellers[totaltellers];
		int sizenzerots=0;
		for(int i=0; i<totaltellers; i++)
		{
			if(queue_total(&(arraytellers[i]->tline)) > 0)
			{
				nonzerotellers[sizenzerots] = i;
				sizenzerots++;
			}
		}
		if(sizenzerots != 0)
		{
			printf("side wali queue me se uthayenge\n");
			int r = rand()%sizenzerots;
			struct costumer c = deQueue(&(arraytellers[nonzerotellers[r]]->tline));
			float serviceTime = 2*averageServiceTime*rand()/(RAND_MAX + 0.0);
			t->totalServiceTime += serviceTime;
			c.WaitingTime = clk - c.arrTime;
			c.serviceTime = serviceTime;
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
			printf("sone ka time ho gyaa\n");
			t->idletime = 20;//rand()%150;
			struct Event e1;
			e1.typeofevent = 3;
			e1.eventtime = clk + t->idletime;
			if(e1.eventtime > 60)
				t->totalIdleTime += 60 - clk;
			else t->totalIdleTime += t->idletime;
			e1.object = t;
			e1.fun_ptr = &nextjob_teller;
			Node *tobeinsert = (Node*) malloc(sizeof(Node));
			tobeinsert->CurrEvent = e1;
			tobeinsert->NextNode = NULL;
			insertNode(head, tobeinsert);
		}
	}
}






int main(int argc, char** args)
{
	int totalcostumers = atoi(args[1]);
	int totaltellers = atoi(args[2]);
	float simulationTime = atof(args[3])*60;
	float averageServiceTime = atof(args[4])*60;
	
	//creating an array of tellers
	struct teller* arraytellers[totaltellers]; //array of all tellers
	head = (Node*) malloc(sizeof(Node)); //Head of  EventQueue Head is nothing but a node with CurrEvent NULL, first Node of LinkedList starts with head->NextNode.
	head->NextNode = NULL;
	int k=0;
	for(k=0; k<totaltellers; k++)
	{	
		// struct teller t2;
		struct teller *t1 = (struct teller *) malloc(sizeof(struct teller));
		float rndmtime = 5;//rand()%600;
		t1->idletime = rndmtime;
		t1->totalIdleTime = rndmtime;
		t1->totalServiceTime = 0;
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
	//All tellers event inserted

	for(k=0; k<totalcostumers; k++)
	{
		float arrTime = 4;//simulationTime * rand()/(RAND_MAX+0.0);
		struct costumer c = {arrTime, -1, -1};
		
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
	sleep(3);
	// Execution start
	float clk=0;
	FILE *ft = fopen("output.txt", "w");
	printf("EventQueue initially\n");
	printLinkedList();
	while(clk <= simulationTime)
	{

		if(head->NextNode == NULL)
			break;
		
		clk = head->NextNode->CurrEvent.eventtime;
		//Invoking function
		if(head->NextNode->CurrEvent.fun_ptr == &delete_costumer)
			{
				printf("delete_costumer starting\n");
			}
		else if(head->NextNode->CurrEvent.fun_ptr == &add_costumer)
			printf("add_costumer starting\n");
		else if (head->NextNode->CurrEvent.fun_ptr == &nextjob_teller)
			printf("nextjob_teller starting\n");
		Event eve = head->NextNode->CurrEvent;
		eve.fun_ptr(arraytellers, totaltellers, clk, averageServiceTime, ft);
		printf("Event completed %f\n", clk);
		printLinkedList();
	}
	FILE *ft2 = fopen("output2.txt", "w");
	fprintf(ft2, "totalIdleTime           totalserviceTime\n");
	for(int i=0; i<totaltellers; i++)
	{
		fprintf(ft2, "%f, %f\n", arraytellers[i]->totalIdleTime, arraytellers[i]->totalServiceTime);
	}
	fclose(ft);
	fclose(ft2);
	printf("clk is : %f\n", clk);
	exit(1);

	// float arrTime = simulationTime * rand()/float(RAND_MAX);
	// float serviceTime = 2*averageServiceTime*rand()/float(RAND_MAX);
}


// Test case 
// total tellers = 1
// total costumers = 1
// arrTime of costumer = 4sec
// initial idle time of teller = 5sec
// service Time = 6sec //costant
// idle time later for teller = 20sec //constant 
