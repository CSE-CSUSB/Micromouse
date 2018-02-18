#ifndef DEQUE_H
#define DEQUE_H

#include <stdlib.h>//#define NULL and malloc
#include <stdbool.h>//type definition for bool

//the position index will be the index representing a square on the maze
//for example, {0,0} will be the top-leftmost square in the maze
typedef struct _PosIndex
{
	int x;
	int y;
} PosIndex;

bool comparePosIndex(PosIndex a, PosIndex b);
void printPosIndex(PosIndex * a);

//the definition for the node, which will contain
//Since the Queue structure will be doubly linked list, the node will have a previous and a next pointer
typedef struct _PosNode
{
	struct _PosNode* next;
	struct _PosNode* prev;
	PosIndex data;
} PosNode;

//The queue is a doubly linked list
//the front of the queue will never have a previous node
//the back of the queue will never have a next node
typedef struct _Deque
{
	PosNode *mFront;
	PosNode *mBack;
	int mSize;//size will be decremented and incremented when dequeue and enqueue are called, respectively
} Deque;


void Deque_Deque(Deque * q);//constructor initializing everything to zero and NULL
void Deque_push_back(Deque * q, PosIndex a);//add some value to the back of the queue
void Deque_pop_front(Deque * q);//remove some value from the front of the queue
void Deque_push_front(Deque * q, PosIndex a);//add some value to the front of the queue
void Deque_pop_back(Deque * q);//remove some value from the back of the queue
int Deque_getSize(Deque * q);//return mSize;
PosIndex Deque_getFront(Deque * q);//return the position index in the front of the queue
PosIndex Deque_getBack(Deque * q);//return the position index in the back of the queue
void Deque_clear(Deque * q);//empties the queue
bool Deque_empty(Deque * q);//returns true if the queue is empty

#endif