#ifndef BFS_H
#define BFS_H

#include "Deque.h"
#include "../maze_generator/Matrix.h"
#include "../maze_generator/Maze.h"

typedef struct BFS_
{
	Matrix* mMaze;//the matrix which will be solved
	int mX1;//starting position
	int mY1;
	int mX2;//target position
	int mY2;
	int mXc;//current position
	int mYc;
	Deque mQ;//deque which will perform BFS
	int* mDict;//dictionary keeping track of the parent index of a visited position
	Matrix mBin; //binary matrix keeping track of whether a position is visited
}BFS;


void BFS_BFS(BFS* bfs, Matrix* m, int x1, int y1, int x2, int y2);//construct BFS struct
Deque BFS_solve(BFS* bfs);//traverse the maze, return the shortest path
void BFS_step(BFS * bfs);//helper function, explore children of a parent position
bool BFS_checkSolve(BFS* bfs);//return whether the goal has been explored
Deque BFS_getPath(BFS* bfs);//helper function, return the path based on dictionary

void BFS_printPath(Deque q);

#endif
