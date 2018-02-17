/*
This file is intended to test the Maze structure;
not to be compiled with any other file containing int main()
*/

#include "Matrix.h"
#include "Maze.h"
#include "CMF_adapter.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int tilesX = 16;
const int tilesY = 16;

const int stringBufferSize = 100;

int main(int argc, char* args[])
{
	time_t t;
	srand(time(&t));
	
	Matrix grid;
	int sx = 0,sy = 0;//starting point
	Matrix_Matrix(&grid, tilesX, tilesY);
	generateMaze(&grid, &sx, &sy);
	//int a1 = , b1, c1;
	//Matrix_itop(&grid, &a1, &b1, &c1);
	//printf("itop(%d) = (%d,%d)\n", a1, b1, c1);
	drawMaze(&grid, &sx, &sy);

	printf("starting cell ( %d , %d )\n", sx, sy);
	
	printf("%s", "Would you like to export this maze? [y/n] ");
	char a;
	scanf(" %c", &a);
	
	if(a == 'y' || a == 'Y')
	{
		char* filename = malloc(sizeof(char) * stringBufferSize);
		printf ("%s", "Please give a file name and extension: ");
		scanf("%s", filename);
		export(&grid, filename, &sx, &sy);
		free(filename);
	}
	
	printf("%s", "would you like to import a maze? [y/n] ");
	char b;
	scanf(" %c", &b);
	
	if(b == 'y' || b == 'Y')
	{
		Matrix tempmaze;
		char* filename = malloc(sizeof(char) * stringBufferSize);
		printf ("%s", "Please give a file name and extension: ");
		scanf("%s", filename);
		import(&tempmaze, filename, &sx, &sy);
		drawMaze(&tempmaze, &sx, &sy);
		free(filename);
		Matrix_clear(&tempmaze);
	}
	
	
	
	Matrix_clear(&grid);
	return 0;
}