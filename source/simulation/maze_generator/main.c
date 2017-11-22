#include "Matrix.h"
#include "Maze.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int tilesX = 16;
const int tilesY = 16;
const int stringBufferSize = 100;

//export to cmf
void export(Matrix *m, char *fname, int* sx, int* sy)
{
	FILE *cmf;
	cmf = fopen(fname,"w+");
	int filesize = Matrix_size(m);
	
	//cmf has 4 parts, filesize, start, goal, and tuples
	//filesize
	fprintf(cmf,"%d", filesize);
	fprintf(cmf,"%c",' ');
	
	//start
	fprintf(cmf, "%d", *sx + *sy * Matrix_width(m));
	fprintf(cmf,"%c",' ');
	
	//goal, top left corner of the goal
	bool goalfound = false;
	for (int i = 0, j = Matrix_height(m); (i < j) && !goalfound; ++i)
	{
		for (int k = 0, l = Matrix_width(m); (k < l) && !goalfound; ++k)
			if (checkFinish(m, &k, &i))//search for the goal
			{
				int goal = k + i * Matrix_width(m);
				fprintf(cmf, "%d", goal);
				fprintf(cmf,"%c",' ');
				goalfound = true;
				break;
			}
	}
	
	//tuples
	for (int i = 0, j = Matrix_height(m); i < j; ++i)
	{
		for (int k = 0, l = Matrix_width(m); k < l; ++k)
		{
			fprintf(cmf, "%d", (int)*Matrix_get(m,k,i));
			fprintf(cmf,"%c",' ');			
		}
	}
		
	//done	
	fclose(cmf);
	printf ("%s", "Export complete");
}

int main(int argc, char* args[])
{
	time_t t;
	srand(time(&t));
	
	Matrix grid;
	int sx = 0,sy = 0;//starting point
	Matrix_Matrix(&grid, tilesX, tilesY);
	generateMaze(&grid, &sx, &sy);

	drawMaze(&grid, &sx, &sy);

	printf("starting cell ( %d , %d )\n", sx, sy);
	
	printf("%s", "Would you like to export this maze? [y/n] ");
	char a;
	scanf("%c", &a);
	
	if(a == 'y' || a == 'Y')
	{
		char* filename = malloc(sizeof(char) * stringBufferSize);
		printf ("%s", "Please give a file name and extension: ");
		scanf("%s", filename);
		export(&grid, filename, &sx, &sy);
		free(filename);
	}
	
	
	Matrix_clear(&grid);
	return 0;
}