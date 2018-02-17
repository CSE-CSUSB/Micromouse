/*
This file is intended to test the Deque structure;
not to be compiled with any other file containing  int main()
*/

#include <stdio.h>
#include "Deque.h"

void printPosIndex(PosIndex * a)
{
	printf("( %i , %i )\n", a->x, a->y);
}

int main(int argc, char* argv[])
{
	PosIndex a;
	a.x = 12; a.y = 13;
	
	Deque q;
	Deque_Deque(&q);
	
	a.x = 10; a.y = 33;
	Deque_push_back(&q, a);
	a = Deque_getFront(&q);
	printPosIndex(&a);
	printf("Deque size %i\n", Deque_getSize(&q));
	
	a.x = 19; a.y = 11;
	Deque_push_front(&q, a);
	a = Deque_getFront(&q);
	printPosIndex(&a);
	printf("Deque size %i\n", Deque_getSize(&q));
	
	Deque_pop_front(&q);
	a = Deque_getFront(&q);
	printPosIndex(&a);
	printf("Deque size %i\n", Deque_getSize(&q));
	
	
	Deque_pop_back(&q);
	a = Deque_getFront(&q);
	printPosIndex(&a);
	printf("Deque size %i\n", Deque_getSize(&q));

	if (Deque_empty(&q))
		printf("%s\n", "Deque empty");
	
	return 0;
}