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
void add_costumer(struct teller* arraytellers[], float clk)//struct teller* arraytellers[0], Node* head, Event e)
{
	// printf("add_costumer is calling %d\n", 10);
	// Event curr = head->NextNode->CurrEvent; //Event on which we are working
	// head->NextNode = head->NextNode->NextNode;	//removing the head of EventQueue
	
	// int* mins = minindex(arraytellers);
	// int totalmins=0;
	// while(mins[totalmins] != -1)
	// {
	// 	totalmins++;
	// }
	// int rnd = rand()%totalmins;
	// //now we need to add costumer c in the mins[rnd]-th element of the vector v.
	// int targetindex = mins[rnd];
	// enQueue(arraytellers[targetindex]->tline, (struct costumer) *curr.object);
	// struct teller *t = vector_get(v, targetindex);
}
void add_costumer1(vector *v/*vector of teller structs*/, struct costumer c)
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
void idletellercomp(struct teller* arraytellers[], float clk)
{
	printf("idle teller completed %d\n", 10);
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

void insertNode(Node *head, Node *tobeinsert)
{
		//without sorting
		// Node *temp = head;
		// while(temp->NextNode != NULL)
		// {
		// 	temp = (temp->NextNode);
		// }
		// temp->NextNode = tobeinsert;


		//with sorting
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


int main(int argc, char** args)
{
	int totalcostumers = atoi(args[1]);
	int totaltellers = atoi(args[2]);
	float simulationTime = atof(args[3]);
	float averageServiceTime = atof(args[4]);
	
	vector equeue;
	vector_init (&equeue);
	//creating an array of tellers
	struct teller* arraytellers[totaltellers]; //array of all tellers
	head = (Node*) malloc(sizeof(Node)); //Head of  EventQueue Head is nothing but a node with CurrEvent NULL, first Node of LinkedList starts with head->NextNode.
	head->NextNode = NULL;
	int k=0;
	for(k=0; k<totaltellers; k++)
	{	
		struct teller t1;
		t1.idletime = rand()%600;
		tellerline tl;
		tellerline_init(&tl);
		t1.tline = tl; 
		arraytellers[k] = &t1;

		struct Event e1;
		e1.typeofevent = 4;
		e1.eventtime = t1.idletime;
		e1.object = &t1;
		e1.fun_ptr = &idletellercomp;

		Node *tobeinsert = (Node*) malloc(sizeof(Node));
		tobeinsert->CurrEvent = e1;
		tobeinsert->NextNode = NULL;
		insertNode(head, tobeinsert);
	}
	printLinkedList(head);
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
	// exit(1);
	float clk=0;
	while(true)
	{
		if(head->NextNode == NULL)
			break;
		clk = head->NextNode->CurrEvent.eventtime;
		//Invoking function
		head->NextNode->CurrEvent.fun_ptr(arraytellers, clk);
		
		// curr->CurrEvent.fun_ptr(5);	//all tellers, NodeHead, costumer, 
		/*
		for type 1 : add_costumer()
			arraytellers, 
		for type 2 : delete_costumer()
			FILE* (to print total time)
		for type 3 : teller_completed_a_costumer()
			tellersarray, head
		fun_ptr(struct teller **, Node *head)
		*/
		printf("Event completed %f\n", clk);
		printLinkedList(head);
	}
	exit(1);


	// vector v;
	// vector_init(&v);
	// Node *head = (Node*) malloc(sizeof(Node));
	// struct teller t1;
	// t1.idletime = rand()%600;
	// vector_add(&v, &t1);

	// struct Event e1;
	// e1.typeofevent = 4;
	// e1.eventtime = t1.idletime;
	// e1.object = &t1;
	// e1.fun_ptr = &idletellercomp;
	// // enQueueEvent(&eventq, e1);
	// // Node head1 = {e1, NULL};
	// head->CurrEvent = e1;
	// head->NextNode = NULL;
	
	// struct teller t2;
	// t2.idletime = rand()%600;
	// vector_add(&v, &t2);

	// struct Event e2;// = {4, t1.idletime, (void* ) t1, &add_costumer};
	// e2.typeofevent = 4;
	// e2.eventtime = t2.idletime;
	// e2.object = &t2;
	// e2.fun_ptr = &idletellercomp;
	// Node *temp = (Node*) malloc(sizeof(Node));
	// temp->CurrEvent = e2;
	// temp->NextNode = NULL;
	// head->NextNode = temp;

	// printf("Manoj Kumar ne start kr diya\n");
	
	// printf("Manoj Kumar ne start kr diya\n");

	// struct teller t3;
	// t3.idletime = rand()%600;
	// vector_add(&v, &t3);

	// struct Event e3;// = {4, t1.idletime, (void* ) t1, &add_costumer};
	// e3.typeofevent = 4;
	// e3.eventtime = t3.idletime;
	// e3.object = &t3;
	// e3.fun_ptr = &idletellercomp;

	// Node *temp2 = (Node*) malloc(sizeof(Node));
	// temp2->CurrEvent = e3;
	// temp2->NextNode = NULL;
	// temp->NextNode = temp2;
	// int i=0;
	// for(i=0; i<3; i++)
	// 	printf("added element is : %f\n", ((struct teller *) vector_get(&v, i))->idletime);

	// printf("lol11 %d, %d\n", head->CurrEvent.typeofevent, head->CurrEvent.eventtime);
	// printf("lol11 %d, %d\n", head->NextNode->CurrEvent.typeofevent, head->NextNode->CurrEvent.eventtime);
	// printf("lol11 %d, %d\n", head->NextNode->NextNode->CurrEvent.typeofevent, head->NextNode->NextNode->CurrEvent.eventtime);
	// printLinkedList(head);
	// exit(1);

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