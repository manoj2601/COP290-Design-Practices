#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "EventQueue.h"
// #include "tellerline.h"
#include "allstructs.h"
#include "helperfunctions.h"
void insertNode(Node *head3, Node *tobeinsert)
{
		if(head3->NextNode == NULL) //zero elements in the EventQueue
		{
			head3->NextNode = tobeinsert;
			return;
		}

		bool inend=false;
		Node *curr = head3->NextNode;
		Node *prev = head3;
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
		printf("Event %d, %f\n", temp.CurrEvent.typeofevent, temp.CurrEvent.eventtime);
		if(temp.CurrEvent.typeofevent == 2)
		{
			printf("delete_costumer with arrTime : %f\n", ((struct costumer *)temp.CurrEvent.object)->arrTime);
		}
		else if(temp.CurrEvent.fun_ptr == (void*)&add_costumer1)
		{
			printf("add_costumer1 with arrTime : %f\n", ((struct costumer *)temp.CurrEvent.object)->arrTime);
		}
		else if(temp.CurrEvent.fun_ptr == (void*)&add_costumer2)
		{
			printf("add_costumer2 with arrTime : %f\n", ((struct costumer *)temp.CurrEvent.object)->arrTime);
		}
		else if (temp.CurrEvent.typeofevent == 3)
		{
			printf("nextjob_teller\n");
		}
		temp = (temp.NextNode)[0];
		//sleep(0.1);
	}
	printf("last Event : %d, %f\n\n", temp.CurrEvent.typeofevent, temp.CurrEvent.eventtime);
		if(temp.CurrEvent.typeofevent == 2)
		{
			printf("delete_costumer with arrTime : %f\n", ((struct costumer *)temp.CurrEvent.object)->arrTime);
		}
		else if(temp.CurrEvent.fun_ptr == (void*)&add_costumer1)
		{
			printf("add_costumer1 with arrTime : %f\n", ((struct costumer *)temp.CurrEvent.object)->arrTime);
		}
		else if(temp.CurrEvent.fun_ptr == (void*)&add_costumer2)
		{
			printf("add_costumer2 with arrTime : %f\n", ((struct costumer *)temp.CurrEvent.object)->arrTime);
		}
		else if (temp.CurrEvent.typeofevent == 3)
		{
			printf("nextjob_teller\n");
		}
}