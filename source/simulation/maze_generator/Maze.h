#ifndef MAZE_H
#define MAZE_H

#include "Matrix.h"
#include <stdlib.h>//srand and rand

/*
Note that the origin of this maze coordinate system is such
that the origin is on the top-left
*/

//enumeration for maze tile data
typedef enum {
	kNone = 0,
	kNorth = 0x01,
	kEast = 0x02,
	kSouth = 0x04,
	kWest = 0x08,
	kExplored = 0x10,
	/*
	3 more numbers were mentioned, allowing faster solving of the maze,
	but I did not pay enough attention at the meetings
	*/
	//kMagic1 = 0x20,
	//kMagic2 = 0x40,
	//kMagic3 = 0x80,
}Tile;

//randomly generate a maze which follows the rule CAMM2017 2.3
//m is the matrix of the Maze, 
//x and y are the indices of the starting corner of the maze
void generateMaze(Matrix* m, int* x, int* y);

//set the entire maze to 15, an empty canvas
//instead of setting every value to 0, 15 allows the generator
//to tunnel through the squares without causing accidental 2x2 squares
void zeroMaze(Matrix* m);

//randomly chooses one of 4 corners and returns index where corner will be
void chooseCorner(Matrix* m, int* x, int* y);

//sets every value in the maze into a random value 
void scrambleMaze(Matrix* m);

//Using the |= operator, add a wall to the given index
//also modifies the adjacent index
void addWall(Matrix* m, int* x, int* y, Tile t);

//Using the &= operator, remove a wall from the given index
//also modifies the adjacent index
void removeWall(Matrix* m, int* x, int* y, Tile t);

//using addWall and and removeWall, apply the tile to the given index
//also modifies the adjacent index
void setWall(Matrix* m, int* x, int* y, Tile t);

//returns true if the current indices are within a 2x2 square
bool checkFinish(Matrix* m, int* x, int* y);

//after the first tunnel, the finish needs to be set to explores,
//to allow only one entry hole to the finish from the outside
//if tunneling were to continue
void setFinishExplored(Matrix* m);

//set all squares to unexplored
void clearExplore(Matrix* m);

//Make the outer walls and corners
void createOuterWalls(Matrix* m);

//ensure the corner square has 3 walls
void createCorner(Matrix* m, int* x, int* y);

//create a 2x2 square somewhere in the maze
//arg center, if false, will create the finish at a random location
void createFinish(Matrix* m, bool center);

//guarantee there is at least one possible path by creating a possible path
//for the MicroMouse from the starting corner to the finish
void tunnelMaze(Matrix* m, int* x, int* y);

//draw the given maze using ASCII characters
//x and y are the starting position, will be marked on the maze
void drawMaze(Matrix* m, int* x, int* y);

#endif