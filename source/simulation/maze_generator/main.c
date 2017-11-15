#include "Matrix.h"
#include "Maze.h"
#include "stdio.h"
#include "time.h"

const int tilesX = 16;
const int tilesY = 16;

int main(int argc, char* args[])
{
	time_t t;
	srand(time(&t));
	
	Matrix grid;
	int sx = 0,sy = 0;//starting point
	Matrix_Matrix(&grid, tilesX, tilesY);
	generateMaze(&grid, &sx, &sy);

	drawMaze(&grid, &sx, &sy);
	/*
	for (int i = 0; i < tilesX; ++i)
	{
		for(int j = 0; j < tilesY; ++j)
		{
			printf("%s", checkFinish(&grid, &i, &j) ? "f" : "n");
		}
		printf("\n");
	}
	*/
	printf("starting cell ( %d , %d )\n", sx, sy);
	
	
	Matrix_clear(&grid);
	return 0;
}