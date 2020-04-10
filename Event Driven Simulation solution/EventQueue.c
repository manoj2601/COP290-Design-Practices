#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "EventQueue.h"
#include "tellerline.h"
#include "allstructs.h"

void insertNode(Node *tobeinsert)
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

