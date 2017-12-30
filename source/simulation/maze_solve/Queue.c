#include "Queue.h"

bool comparePosIndex(PosIndex a, PosIndex b)
{
	return ((a.x == b.x) && (a.y == b.y));
}

void Queue_Queue(Queue * q)
{
	q->mFront = NULL;
	q->mBack = NULL;
	q->mSize = 0;
}

void Queue_enqueue(Queue * q, PosIndex a)
{
	//create a new node
	PosNode* newnode = malloc(sizeof(PosNode));
	newnode->data = a;
	
	//if the queue is empty, make the new node both the front and back
	if (q->mFront == NULL)
	{
		newnode->next = NULL;
		newnode->prev = NULL;
		q->mFront = newnode;
		q->mBack = newnode;
	}
	
	//else put the queue in the back
	else 
	{
		q->mBack->next = newnode;
		newnode->prev = q->mBack;
		newnode->next = NULL;
		q->mBack = newnode;
	}
	
	q->mSize++;
}

void Queue_dequeue(Queue * q)
{
	//if the queue is already empty, the work is already done
	if (q->mFront == NULL)
		return;
	
	//if the queue only contains one element
	else if ((q->mFront == q->mBack) && q->mFront != NULL)
	{
		free(q->mFront);//free the data and nullify pointers
		q->mFront = NULL;
		q->mBack = NULL;
		q->mSize--;
	}
	
	//else remove the last element in the queue
	else
	{
		q->mFront = q->mFront->next; //move the front pointer to the next
		free(q->mFront->prev);//delete the front
		q->mFront->prev = NULL;//nullify the pointer
		q->mSize--;
	}
}

int Queue_getSize(Queue * q)
{
	return q->mSize;
}

PosIndex Queue_getFront(Queue * q)
{
	if (q->mFront == NULL)
	{
		PosIndex temp = {0,0};
		return temp;//return some silly value if the queue is empty
	}
	else return q->mFront->data;
}

void Queue_clear(Queue * q)
{
	while (q->mFront != NULL)
	{
		Queue_dequeue(q);
	}
}

bool Queue_empty(Queue* q)
{
	return (q->mFront == NULL);
}