#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "EventQueue.h"

void EventQueue_init (EventQueue *q)
{
	// q->head = NULL;
	q->size = 0;
}

bool is_empty(EventQueue *q)
{
	if(q->size == 0)
		return true;
	else return false;
}

int EventQueue_total(EventQueue *q)
{
	return q->size;
}

void enQueueEvent(EventQueue *q, Event e)
{
	printEventQueue(q);
	Node newnode = {e, NULL};
	if(q->size == 0)
	{
		// Node n = {e, NULL};

		q->head = &newnode;
	}
	else
	{
		Node *curr = (q->head);
		Node *prev;
		bool isdone = false;
		//First go for head
		if(q->head->CurrEvent.eventtime > e.eventtime)
		{
			Node n = *(q->head);
			// Node *m = &n;
			printf("adding is going\n");
			
			// printf("3rd element is %d, %d\n", n.NextNode->CurrEvent.typeofevent, n.NextNode->CurrEvent.eventtime);
			printf("Done\n");
			newnode.NextNode = &n;
			q->head = &newnode;
			isdone = true;
		} //done for head
		else{
			prev = curr;
			curr = curr->NextNode;
		for(int i=1; i<q->size; i++)
		{

			if(curr->CurrEvent.eventtime < e.eventtime)
			{
				prev = curr;
				curr = curr->NextNode;
			}
			else
			{
				printf("I am here\n");
				newnode.NextNode = curr;
				prev->NextNode = &newnode;
				isdone = true;
				break;
			}
		}
		}

		if(!isdone)
		{
			curr = &newnode;
		}
	}
	q->size++;
}

Event deQueueEvent(EventQueue *q)
{
	if(q->size == 0)
	{
		printf("EventQueue is empty\n");
		Event n;
		return n;
	}
	q->size--;
	Node n = *(q->head);
	q->head = q->head->NextNode;
	return n.CurrEvent;
}

void printEventQueue(EventQueue *q)
{
	// printf("printing Event Queue\n");
	int length = q->size;
	Node *temp = (q->head->NextNode);
	if(length != 0)
		printf("head is : %d, %d\n", q->head->CurrEvent.typeofevent, q->head->CurrEvent.eventtime);
	for(int i=1; i<length; i++)
	{
		printf("%d, %d\n" ,temp->CurrEvent.typeofevent, temp->CurrEvent.eventtime);
		temp = temp->NextNode;
	}
}