#ifndef QUEUE_H
#define QUEUE_H

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
typedef struct _Queue
{
	PosNode *mFront;
	PosNode *mBack;
	int mSize;//size will be decremented and incremented when dequeue and enqueue are called, respectively
} Queue;


void Queue_Queue(Queue * q);//constructor initializing everything to zero and NULL
void Queue_enqueue(Queue * q, PosIndex a);//add some value to the back of the queue
void Queue_dequeue(Queue * q);//remove some value from the front of the queue
int Queue_getSize(Queue * q);//return mSize;
PosIndex Queue_getFront(Queue * q);//return the position index in the front of the queue
void Queue_clear(Queue * q);//empties the queue
bool Queue_empty(Queue* q);//returns true if the queue is empty

#endif