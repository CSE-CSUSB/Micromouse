#include <stdio.h>
#include "Queue.h"

void printPosIndex(PosIndex * a)
{
	printf("( %i , %i )\n", a->x, a->y);
}

int main(int argc, char* argv[])
{
	PosIndex a;
	a.x = 12; a.y = 13;
	
	Queue q;
	Queue_Queue(&q);
	
	a.x = 10; a.y = 33;
	Queue_enqueue(&q, a);
	a = Queue_getFront(&q);
	printPosIndex(&a);
	printf("queue size %i\n", Queue_getSize(&q));
	
	a.x = 19; a.y = 11;
	Queue_enqueue(&q, a);
	a = Queue_getFront(&q);
	printPosIndex(&a);
	printf("queue size %i\n", Queue_getSize(&q));
	
	Queue_dequeue(&q);
	a = Queue_getFront(&q);
	printPosIndex(&a);
	printf("queue size %i\n", Queue_getSize(&q));
	
	
	Queue_dequeue(&q);
	a = Queue_getFront(&q);
	printPosIndex(&a);
	printf("queue size %i\n", Queue_getSize(&q));

	if (Queue_empty(&q))
		printf("%s\n", "Queue empty");
	
	return 0;
}