#include "BFS.h"

/*
The search algorithm uses two integers (x,y) to represent an index
as opposed to using a single integer per index as shown in CMF documentation
*/

void BFS_BFS(BFS* bfs, Matrix* m, int x1, int y1, int x2, int y2)
{
	bfs->mMaze = m;
	bfs->mX1 = bfs->mXc = x1;
	bfs->mY1 = bfs->mYc = y1;
	bfs->mX2 = x2;
	bfs->mY2 = y2;
	
	Matrix_Matrix(&bfs->mBin, Matrix_width(m), Matrix_height(m));//init mBin
	
	for (int k = 0, l = Matrix_height(&bfs->mBin); k < l; ++k)//set mBin
		for (int i = 0, j = Matrix_width(&bfs->mBin); i < j; ++i)
			*Matrix_get(&bfs->mBin, i, k) = false;//set the entire matrix to false
	
	bfs->mDict = malloc(Matrix_size(m) * sizeof(int));//dictionary keeping track of the index of the parent
	for (int i = 0, j = (int)Matrix_size(m); i < j; ++i)//set dictionary
		bfs->mDict[i] = -1; //set every index to -1
		
	Deque_Deque(&bfs->mQ);
}

//return path through the maze
//from point 1 to point 2 (x1,y1) -> (x1,y2)
Deque BFS_solve(BFS* bfs)
{
	PosIndex temppos;
	temppos.x = bfs->mX1; temppos.y = bfs->mY1;
	Deque_push_back(&bfs->mQ,temppos);//enqueue the initial position onto the queue
	*Matrix_get(&bfs->mBin, bfs->mX1, bfs->mY1) = true;//mark the initial position as true
	
	//traverse the maze using BFS until you reach desired destination 
	while (!BFS_checkSolve(bfs) && !Deque_empty(&bfs->mQ)) //goal not reached, and all possible routes not exhausted
	{
		BFS_step(bfs);
	}
	
	//find the solution
	Deque solution = BFS_getPath(bfs);
	
	//free memory
	Matrix_clear(&bfs->mBin);
	Deque_clear(&bfs->mQ);
	free(bfs->mDict);
	
	return solution;
	
}

void BFS_step(BFS * bfs)
{
	PosIndex temppos;
	temppos.x = bfs->mXc = Deque_getFront(&bfs->mQ).x;
	temppos.y = bfs->mYc = Deque_getFront(&bfs->mQ).y;//set current position
	int child = -1, parent;
	Matrix_ptoi(bfs->mMaze, &parent, &temppos.x, &temppos.y);//set parent index
	
	Uint8 direction;
	int tempx, tempy;
	for (int i = 0, j = 4; i < j; ++i)
	{
		switch(i)
		{
		case 0: //North
			direction = kNorth;
			tempx = bfs->mXc;
			tempy = bfs->mYc - 1;
			break;
		case 1: //East
			direction = kEast;
			tempx = bfs->mXc + 1;
			tempy = bfs->mYc;
			break;
		case 2: //South
			direction = kSouth;
			tempx = bfs->mXc;
			tempy = bfs->mYc + 1;
			break;
		case 3: //West
			direction = kWest;
			tempx = bfs->mXc - 1;
			tempy = bfs->mYc;
			break;
		}
		
		if (!(*Matrix_get(bfs->mMaze, bfs->mXc, bfs->mYc) & direction)//current parent index wall doesnt exist
			&& Matrix_valid(bfs->mMaze, tempx, tempy)//if tile exists
			//&& check to see if explored, exploration is incomplete
			&& !(*Matrix_get(&bfs->mBin, tempx, tempy)))//if it has not been traversed
		{
			//put data to dictionary
			Matrix_ptoi(bfs->mMaze, &child, &tempx, &tempy);
			bfs->mDict[child] = parent;
			
			//mark position as traversed
			*Matrix_get(&bfs->mBin, tempx, tempy) = true;
			
			//push PosIndex onto Deque
			temppos.x = tempx;
			temppos.y = tempy;
			Deque_push_back(&bfs->mQ, temppos);
		}
	}
	Deque_pop_front(&bfs->mQ);
}

bool BFS_checkSolve(BFS* bfs)
{
	int i = -1;
	Matrix_ptoi(bfs->mMaze, &i, &bfs->mX2, &bfs->mY2);
	//true if the target's parent is not -1
	return (bfs->mDict[i] != -1);
}


Deque BFS_getPath(BFS* bfs)
{
	Deque solution;
	Deque_Deque(&solution);
	PosIndex temppos;
	int tempx = bfs->mX2;
	int tempy = bfs->mY2;
	
	int i;
	Matrix_ptoi(bfs->mMaze, &i, &tempx, &tempy);//set i to the goal index
	
	
	while(bfs->mDict[i] != -1)
	{
		Matrix_itop(bfs->mMaze, &i, &temppos.x, &temppos.y);
		Deque_push_front(&solution, temppos);//add the posindex of i to the solution deque
		i = bfs->mDict[i];//point the index to it's parent index
	}
	
	//one more time to include the starting position
	Matrix_itop(bfs->mMaze, &i, &temppos.x, &temppos.y);
	Deque_push_front(&solution, temppos);//add the posindex of i to the solution dequeMatrix_itop(bfs->mMaze, &i, &temppos.x, &temppos.y);

	return solution;
}

void BFS_printPath(Deque q)
{
	while(!Deque_empty(&q))
	{
		printPosIndex(&(q.mFront->data));
		Deque_pop_front(&q);
	}
}