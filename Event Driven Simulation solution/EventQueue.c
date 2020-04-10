#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "EventQueue.h"
#include "tellerline.h"
#include "allstructs.h"

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
