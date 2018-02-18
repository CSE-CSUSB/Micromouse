#include "Deque.h"
#include "BFS.h"
#include "../maze_generator/Matrix.h"
#include "../maze_generator/Maze.h"
#include "../maze_generator/CMF_adapter.h"

const int tilewidth = 16;
const int tileheight = 16;
const int stringBufferSize = 100;



int main()
{
	int goalx = tilewidth / 2;
	int goaly = tileheight / 2;
	
	int sx, sy;//starting position
	Matrix grid;
	Matrix_Matrix(&grid, tilewidth, tileheight);
	
	printf("%s", "would you like to import a maze? [y/n] ");
	char b;
	scanf(" %c", &b);
	
	
	if(b == 'y' || b == 'Y')
	{
		//load and print
		Matrix tempmaze;
		char* filename = malloc(sizeof(char) * stringBufferSize);
		printf ("%s", "Please give a file name and extension: ");
		scanf("%s", filename);
		import(&tempmaze, filename, &sx, &sy);
		drawMaze(&tempmaze, &sx, &sy);
		
		//solve and output
		BFS bfs;
		BFS_BFS(&bfs, &tempmaze, sx, sy, goalx, goaly);
		Deque tempdeque = BFS_solve(&bfs);
		BFS_printPath(tempdeque);
		
		//free memory
		free(filename);
		Deque_clear(&tempdeque);
		Matrix_clear(&tempmaze);
	}
	
	
	
	Matrix_clear(&grid);
	return 0;
	return 0;
}