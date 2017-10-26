#include "Matrix.h"

void Matrix_Matrix(Matrix* m, int x, int y)
{
	m->mData = NULL;
	m->mX = 0;
	m->mY = 0;//just in case other memories may conflict
	m->mPow2 = false;
	m->mPowX = 0;
	Matrix_setDimensions(m, x, y);
}

void Matrix_setDimensions(Matrix* m, int x, int y)
{
	//if the current dimensions are already what you want, do nothing
	if (x == m->mX && y == m->mY)
		return;

	//if there is data to delete, delete it
	if (Matrix_size(m) > 0)
	{
		free(m->mData);
		m->mData = NULL;
	}

	//use the given arguments to replace the current size 
	m->mX = x;
	m->mY = y;
	m->mSize = x * y;
	Matrix_mCheckPower(m);
	
	if (m->mSize > 0)//data will only be allocated if it is of non-zero size
		m->mData = malloc(m->mSize * sizeof(char));	//In C++ this would be an opportunity to joke about "Neo" and "Matrix"
	
	else 
	{//if the size() is 0, it could be like 1024*0 or whatever, I prefer it just be 0*0
		m->mX = 0;
		m->mY = 0;
	}
}


void Matrix_clear(Matrix* m)
{
	if (Matrix_size(m) > 0)
	{
		//if there is data to delete, delete it,
		free(m->mData);
		m->mData = NULL;
	}

	//then set the dimensions to 0
	m->mX = 0;
	m->mY = 0;
	m->mSize = 0;
}


Uint8* Matrix_get(Matrix* m, int x, int y)
{
	long i;
	if (m->mPow2)
		i = x + (y << m->mPowX);//bit shift operation instead of multiplication
	else i = x + (m->mX * y);
	
	//exception handling, if assert.h is included
#ifdef assert
	assert(x < m->mX && "Matrix X position out of bounds");
	assert(y < m->mY && "Matrix Y position out of bounds");
	//if mY is out of bounds, this implies (i > m->mSize), so no assert needed for that
	assert(x >= 0 && y >=0 && "Matrix negative value passed (may lead to segfault)");
	assert(m->mData != NULL && "Matrix has not been properly allocated");
#endif

	return &(m->mData[i]);
}


long int Matrix_size(Matrix* m)
{
	if (m->mData == NULL)
		return 0;
	else return m->mSize;
}

int Matrix_width(Matrix* m)
{
	return m->mX;
}

int Matrix_height(Matrix* m)
{
	return m->mY;
}

bool Matrix_valid(Matrix* m, int x, int y)
{
	return (x < m->mX
		&& x >= 0
		&& y < m->mY
		&& y >= 0
		&& m->mData);
}

void Matrix_mCheckPower(Matrix* m)
{
	//if mX is a power of 2, then mPowX will be set to log base 2 of mX
	//in other words mX = 2^i, mPowX = i
	
	for(int i = 0, j = 1; i < 31; ++i) //signed integer is only 2^31 - 1, so we only check up to 2^30
	{
		if (m->mX == j) 
		{
			m->mPow2 = true;
			m->mPowX = i;
			return;
		}
		else if (m->mX  < j)//since j will increase
			break;
			
		j = j << 1; //j *= 2 , j = 2^i
	}
	m->mPow2 = false;
	m->mPowX = 0;
}