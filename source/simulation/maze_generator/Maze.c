#include "Maze.h"

void generateMaze(Matrix* m, int* x, int* y)
{
	
	//make sure there are no NULL pointers
#ifdef assert
	assert(m && x && y && "m and x and y are non-NULL");
#endif
	if (m == NULL || x == NULL || y == NULL) return;
	
	//choose one of the 4 corners randomly
	chooseCorner(m, x, y);
	
	zeroMaze(m);//zero the entire matrix
	
	scrambleMaze(m);//scramble the maze up, randomly
	
	createOuterWalls(m);
	
	createCorner(m, x, y);
	
	createFinish(m, true);
	
	tunnelMaze(m, x, y);
	
}

void zeroMaze(Matrix* m)
{
	//instead of a nested loop using Matrix_get()
	
	for (int i = 0, j = (int)Matrix_size(m); i < j; ++i)
		m->mData[i] = 0;
}

void chooseCorner(Matrix* m, int* x, int* y)
{
	switch(rand() % 4)
	{
		case 0:
			*x = 0; *y = 0;
			break;
		case 1:
			*x = 0; 
			*y = Matrix_height(m) - 1;
			break;
		case 2:
			*x = Matrix_width(m) - 1; 
			*y = 0;
			break;
		default://3 and beyond
			*x = Matrix_width(m) - 1; 
			*y = Matrix_height(m) - 1;
			break;
	}
}

void scrambleMaze(Matrix* m)
{
	//generate a random number 0-15, with weight toward larger numbers
	int randno = 0;
	for (int i = 0, j = 16; i < j; ++i)
		randno = ((rand() % 55) != 0) ? (randno + 1) : (randno);
	
	for(int i = 0, j = Matrix_width(m); i < j; ++i)
		for(int k = 0, l = Matrix_height(m); k < l; ++k)
			setWall(m, &i, &k, rand() % 8 + 8);
}

void addWall(Matrix* m, int* x, int* y, Tile t)
{
	if (!Matrix_valid(m,*x,*y)) return;
	if (t & kNorth)//set the current index to North, set the adjacent index to South
	{
		*Matrix_get(m,*x,*y) |= kNorth;
		if (Matrix_valid(m,*x,*y-1))
			*Matrix_get(m,*x,*y-1) |= kSouth;//the unit square to the north will have a wall to the south
	}
	if (t & kEast)//east
	{
		*Matrix_get(m,*x,*y) |= kEast;
		if (Matrix_valid(m,*x+1,*y))
			*Matrix_get(m,*x+1,*y) |= kWest;
	}
	if (t & kSouth)//south
	{
		*Matrix_get(m,*x,*y) |= kSouth;
		if (Matrix_valid(m,*x,*y+1))
			*Matrix_get(m,*x,*y+1) |= kNorth;
	}
	if (t & kWest)//west
	{
		*Matrix_get(m,*x,*y) |= kWest;
		if (Matrix_valid(m,*x-1,*y))
			*Matrix_get(m,*x-1,*y) |= kEast;
	}
}

void removeWall(Matrix* m, int* x, int* y, Tile t)
{
	if (!Matrix_valid(m,*x,*y)) return;
	if (t & kNorth)//set the current index to North, set northern square to have a south wall
	{
		*Matrix_get(m,*x,*y) &= ~kNorth;
		if (Matrix_valid(m, *x, *y - 1))
			*Matrix_get(m, *x, *y - 1) &= ~kSouth;//the unit square to the north will have a wall to the south
	}
	if (t & kEast)//east
	{
		*Matrix_get(m, *x, *y) &= ~kEast;
		if (Matrix_valid(m, *x + 1, *y))
			*Matrix_get(m, *x + 1, *y) &= ~kWest;
	}
	if (t & kSouth)//south
	{
		*Matrix_get(m,*x,*y) &= ~kSouth;
		if (Matrix_valid(m, *x, *y + 1))
			*Matrix_get(m, *x, *y + 1) &= ~kNorth;
	}
	if (t & kWest)//west
	{
		*Matrix_get(m,*x,*y) &= ~kWest;
		if (Matrix_valid(m, *x - 1, *y))
			*Matrix_get(m, *x - 1, *y) &= ~kEast;
	}
}

void setWall(Matrix* m, int* x, int* y, Tile t)
{
	addWall(m,x,y,t);
	t = ~t;//invert bits
	removeWall(m,x,y,t);
	t = ~t;
}

bool checkFinish(Matrix* m, int* x, int* y)
{
	//there are 4 squares in the finish, need to perform 4 checks
	//Note that per check, only two squares need their walls tested
	//because edge-adjacent squares share a wall
	
	if( !Matrix_valid(m, *x, *y))//check for out of bounds
		return false;
	
	//top-left square of a 2x2 block
	if ( !(*Matrix_get(m,*x,*y) & (kEast | kSouth)) //if the top-left square does not have a south or east wall
	&& Matrix_valid(m, *x + 1, *y + 1)
	&& !(*Matrix_get(m, *x + 1, *y + 1) & (kWest | kNorth))//if the bottom-right square does not have a north or west wall
	) return true;
	
	//top-right
	if ( !(*Matrix_get(m,*x,*y) & (kWest | kSouth)) //top-right, south or west wall
	&& Matrix_valid(m, *x - 1, *y + 1)
	&& !(*Matrix_get(m, *x - 1, *y + 1) & (kEast | kNorth))//bottom left, north or east wall
	) return true;
	
	//bottom-left
	if ( !(*Matrix_get(m,*x,*y) & (kEast | kNorth)) //bottom-left, north or east wall
	&& Matrix_valid(m, *x + 1, *y - 1)
	&& !(*Matrix_get(m, *x + 1, *y - 1) & (kWest | kSouth))//top-right, north or east wall
	) return true;
	
	//bottom-right
	if ( !(*Matrix_get(m,*x,*y) & (kWest | kNorth)) //bottom-right, north or west wall
	&& Matrix_valid(m, *x - 1, *y - 1)
	&& !(*Matrix_get(m, *x - 1, *y - 1) & (kEast | kSouth))//top-left, south or east wall
	) return true;
	
	return false;
}

void createOuterWalls(Matrix* m)
{
	int j = Matrix_width(m);
	int k = Matrix_height(m);
	int temp = 0;
	
	//2 loops, one for vertical walls and another for horizontal
	//horizontal walls
	for(int i = 0; i < j; ++i)
	{
		temp = 0;
		addWall(m, &i, &temp, kNorth);//north wall
		temp = k - 1;
		addWall(m, &i, &temp, kSouth);//south wall
	}
	
	//vertical walls
	for(int i = 0; i < k; ++i)
	{
		temp = 0;
		addWall(m, &temp, &i, kWest);//west wall
		temp = j - 1;
		addWall(m, &temp, &i, kEast);//east wall
	}
}

void createCorner(Matrix* m, int* x, int* y)
{
	addWall(m, x, y, kNorth | kEast | kSouth | kWest);//create a wall in all directions, an opening will be made in tunnelMaze()
}

void createFinish(Matrix* m, bool center)
{
	int tlcx;//top-left corner of the 2x2 square
	int tlcy;
	
	//choose the top-left corner of the 2x2 square
	if (center)
	{
		tlcx = (Matrix_width(m) >> 1) - 1; //(Matrix_width(m) / 2) - 1
		tlcy = (Matrix_height(m) >> 1) - 1;
	}
	else 
	{
		tlcx = rand() % (Matrix_width(m) - 3) + 1;
		tlcy = rand() % (Matrix_height(m) - 3) + 1;
	}
	
	setWall(m, &tlcx, &tlcy, kNorth | kWest);//top-left
	tlcx++;
	setWall(m, &tlcx, &tlcy, kNorth | kEast);//top-right
	tlcy++;
	setWall(m, &tlcx, &tlcy, kSouth | kEast);//bottom-right
	tlcx--;
	setWall(m, &tlcx, &tlcy, kSouth | kWest);//bottom-left
}

void tunnelMaze(Matrix* m, int* x, int* y)
{
	for (int i = 0, j = (int)Matrix_size(m); i < j; ++i)
		m->mData[i] &= ~kExplored; //set all squares to unexplored
	
	//temporary x and y values
	int tX = *x;
	int tY = *y;
	int movements = 0;
	
	while(!checkFinish(m, &tX, &tY))//the tunneling 
	{
		*Matrix_get(m, tX, tY) |= kExplored;//mark every tunneled square as explored
		
		//return;
		//this tunneling algorithm may potentially create a 2x2 square.
		switch(rand() % 4)
		{
		case 0://north
			if (Matrix_valid(m, tX, tY - 1))
			{
				if (!(*Matrix_get(m, tX, tY - 1) & kExplored))//remove the wall only if the square hasn't been explored
					removeWall(m, &tX, &tY, kNorth);
				tY--;
			}
			break;
		case 1://east
			if (Matrix_valid(m, tX + 1, tY))
			{
				if (!(*Matrix_get(m, tX + 1, tY) & kExplored))
					removeWall(m, &tX, &tY, kEast);
				tX++;
			}
			break;
		case 2://south
			if (Matrix_valid(m, tX, tY + 1))
			{
				if (!(*Matrix_get(m, tX, tY + 1) & kExplored))
					removeWall(m, &tX, &tY, kSouth);
				tY++;
			}
			break;
		default://west
			if (Matrix_valid(m, tX - 1, tY))
			{
				if (!(*Matrix_get(m, tX - 1, tY) & kExplored))
					removeWall(m, &tX, &tY, kWest);
				tX--;;
			}
			break;
		}
	}
	
	for (int i = 0, j = (int)Matrix_size(m); i < j; ++i)
		m->mData[i] &= ~kExplored; //set all squares to unexplored
}

void drawMaze(Matrix* m, int* x, int* y)
{
	char vWall = '\xBA';//decimal 186 ║
	char hWall = '\xCD';//decimal 205 ═
	char cross = '\xCE';//decimal 206 ╬
	char space = ' ';
	char start = 'x';
	
	/*
	since the function is drawing in text, it must be read horizontally first, line by line, left to right.
	a nested loop will do the trick, where x is the inner loop, y is the outer loop.
	*/
	for(int i = 0, j = Matrix_height(m); i < j; ++i)
	{
		//this loop deals with the horizontal walls, drawn from left to right
		//it also draws cross sections of walls if they exist
		for (int k = 0, l = Matrix_width(m); k < l; ++k)
		{
			//condition for a cross section, if there is a wall either north or west
			if (*Matrix_get(m, k, i) & (kNorth | kWest))
				printf("%c", cross);
			//if the square above has a wall, you need a cross
			else if(Matrix_valid(m, k, i)
				&& *Matrix_get(m, k, i - 1) & (kSouth | kWest))
				printf("%c", cross);
			else printf("%c", space);
			
			//if there is a north wall, draw a horizontal wall
			if (*Matrix_get(m,k,i) & kNorth)
				printf("%c", hWall);
			else printf("%c", space);
			
		}
		
		//assuming the maze is closed on all edges
		printf("%c", cross);
		printf("\n");
		
		for (int k = 0, l = Matrix_width(m); k < l; ++k)//this loop deals with the vertical walls, drawn from left to right
		{
			//if there is a north wall, draw a horizontal wall
			if (*Matrix_get(m, k, i) & kWest)
				printf("%c", vWall);
			else printf("%c", space);
			
			//if the current value is the starting position
			//x && yy && is to check these arent null pointers
			if (x && y && *x == k && *y == i)
				printf("%c", start);
			else printf("%c", space);
		}
		
		//assuming the maze is closed on all edges
		printf("%c", vWall);
		printf("\n");
	}
	
	//need one more loop just for the bottom row of the maze
	
	int k = Matrix_height(m) - 1;
	for (int i = 0, j = Matrix_width(m); i < j; ++i)
	{
				//condition for a cross section, if there is a wall either south or east
			if (*Matrix_get(m, i, k) & (kSouth | kEast))
				printf("%c", cross);
			//if the square above has a wall, you need a cross
			else if(Matrix_valid(m, i, k)
				&& *Matrix_get(m, i, k - 1) & (kSouth | kWest))
				printf("%c", cross);
			else printf("%c", space);
			
			//if there is a south wall, draw a horizontal wall
			if (*Matrix_get(m,i,k) & kSouth)
				printf("%c", hWall);
			else printf("%c", space);
	}
	printf("%c", cross);
	
	printf("\n");
}
