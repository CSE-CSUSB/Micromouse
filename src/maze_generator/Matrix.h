#ifndef MATRIX_H
#define MATRIX_H

#include <stdbool.h>//type definition for bool
#include <stdlib.h>//#define NULL and malloc
#include <assert.h>//assertions
#include <stdint.h>

typedef uint8_t Uint8;//a character will be an unsigned 8 bit integer

/*
Matrix is a 2-D, dynamically allocated character(Uint8) array
As opposed to a static array Uint8[][];
*/

typedef struct _Matrix
{
	int mX;//X, the width of the matrix
	int mY;//Y, the height of the matrix
	long mSize;//the size of the matrix, (X * Y)
	Uint8* mData;//the memory in the matrix
	
	bool mPow2; //is true if mX is a power of 2
	Uint8 mPowX;//number of bit shifts to the left needed such that (n * mY) == (n << mPowY)
				//will only work if Y is a power of 2
} Matrix;

void Matrix_Matrix(Matrix* m, int x, int y);//constructor for Matrix
void Matrix_setDimensions(Matrix* m, int x, int y);//reset the dimensions
void Matrix_clear(Matrix* m);//empties the array
Uint8* Matrix_get(Matrix* m, int x, int y);//Returns a pointer to the dimensions given, must be dereferenced
long int Matrix_size(Matrix* m);//returns the size of the memory allocation
int Matrix_width(Matrix* m);//returns the width of the matrix
int Matrix_height(Matrix* m);//returns the height of the matrix
bool Matrix_valid(Matrix* m, int x, int y);//returns true if the x,y coordinates are below the mX and mY
void Matrix_itop(Matrix* m, int* i, int* x, int* y);//convert single dimensional index to two dimensional position
void Matrix_ptoi(Matrix* m, int* i, int* x, int* y);//convert two dimensional position to single dimensional index

//helper functions
void Matrix_mCheckPower(Matrix* m);

#endif

/*
Sample usage of Matrix struct:
	Matrix grid;
	Matrix_Matrix(&grid,16,16);
	*Matrix_get(&grid, 0, 1) = 1 || 2;
	*Matrix_get(&grid, 1, 1) = 2;
	*Matrix_get(&grid, 0, 2) = 2 || 4;
	*Matrix_get(&grid, 0, 3) = 0;
	Matrix_clear(&grid);	

if you do not include assert.h, you will need 
to use Matrix_valid() before Matrix_get() 
to prevent segfaults
*/