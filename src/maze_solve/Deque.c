#include "Deque.h"

bool comparePosIndex(PosIndex a, PosIndex b)
{
	return ((a.x == b.x) && (a.y == b.y));
}

void printPosIndex(PosIndex * a)
{
	printf("( %i , %i )", a->x, a->y);
}

void Deque_Deque(Deque * q)
{
	q->mFront = NULL;
	q->mBack = NULL;
	q->mSize = 0;
}

void Deque_push_back(Deque * q, PosIndex a)
{
	//create a new node
	PosNode* newnode = malloc(sizeof(PosNode));
	newnode->data = a;
	
	//if the deque is empty, make the new node both the front and back
	if (q->mFront == NULL)
	{
		newnode->next = NULL;
		newnode->prev = NULL;
		q->mFront = newnode;
		q->mBack = newnode;
	}
	
	//else put the deque in the back
	else 
	{
		q->mBack->next = newnode;
		newnode->prev = q->mBack;
		newnode->next = NULL;
		q->mBack = newnode;
	}
	
	q->mSize++;
}

void Deque_pop_front(Deque * q)
{
	//if the deque is already empty, the work is already done
	if (q->mFront == NULL)
		return;
	
	//if the deque only contains one element
	else if ((q->mFront == q->mBack) && q->mFront != NULL)
	{
		free(q->mFront);//free the data and nullify pointers
		q->mFront = NULL;
		q->mBack = NULL;
		q->mSize--;
	}
	
	//else remove the last element in the deque
	else
	{
		q->mFront = q->mFront->next; //move the front pointer to the next
		free(q->mFront->prev);//delete the front
		q->mFront->prev = NULL;//nullify the pointer
		q->mSize--;
	}
}

void Deque_push_front(Deque * q, PosIndex a)
{
	//create a new node
	PosNode* newnode = malloc(sizeof(PosNode));
	newnode->data = a;
	
	//if the deque is empty, make the new node both the front and back
	if (q->mFront == NULL)
	{
		newnode->next = NULL;
		newnode->prev = NULL;
		q->mFront = newnode;
		q->mBack = newnode;
	}
	
	//else put the deque in the front
	else 
	{
		q->mFront->prev = newnode;
		newnode->next = q->mFront;
		newnode->prev = NULL;
		q->mFront = newnode;
	}
	
	q->mSize++;
}

void Deque_pop_back(Deque * q)
{
	//if the deque is already empty, the work is already done
	if (q->mFront == NULL)//note: if the front is NULL, the back should be NULL
		return;
	
	//if the deque only contains one element
	else if ((q->mFront == q->mBack) && q->mFront != NULL)
	{
		free(q->mFront);//free the data and nullify pointers
		q->mFront = NULL;
		q->mBack = NULL;
		q->mSize--;
	}
	
	//else remove the last element in the deque
	else
	{
		q->mBack = q->mBack->prev; //move the back pointer to the prev
		free(q->mBack->next);//delete the back (next)
		q->mBack->next = NULL;//nullify the pointer
		q->mSize--;
	}
}

int Deque_getSize(Deque * q)
{
	return q->mSize;
}

PosIndex Deque_getFront(Deque * q)
{
	if (q->mFront == NULL)//check if empty
	{
		PosIndex temp = {-1,-1};
		return temp;//return some silly value if the deque is empty
	}
	else return q->mFront->data;
}

PosIndex Deque_getBack(Deque * q)
{
	if (q->mFront == NULL)//if empty, (do not need to check mBack)
	{
		PosIndex temp = {-1,-1};
		return temp;//return some silly value if the deque is empty
	}
	else return q->mBack->data;
}

void Deque_clear(Deque * q)
{
	while (q->mFront != NULL)
	{
		Deque_pop_front(q);
	}
}

bool Deque_empty(Deque* q)
{
	return (q->mFront == NULL);
}

