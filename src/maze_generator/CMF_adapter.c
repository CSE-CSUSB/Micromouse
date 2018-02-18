#include "CMF_adapter.h"

void export(Matrix *m, char *fname, int* sx, int* sy)
{
	FILE *cmf;
	cmf = fopen(fname,"w+");
	int filesize = Matrix_size(m);
	
	//CMF has 4 parts, size, start, goal, and tuples
	//size
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
	printf ("%s", "Export complete\n");
}


void import(Matrix *m, char *fname, int* sx, int* sy)
{
	FILE *cmf;
	cmf = fopen(fname,"r+");
	int filesize = Matrix_size(m);
	int width = -1, height = -1;//filesize = w * h;
	int start = 0;
	int ignore = 0; //outputs which will be ignored
	
	//CMF has 4 parts, size, start, goal, and tuples
	//size
	fscanf(cmf,"%d ", &filesize);
	
	
	//CMF 1.0: size = width * height, width = height
	
	//to find width and height, calculate square root using binary search
	int i = 0, j = filesize, mid;
	while (i < j)
	{
		mid = i + ((j - i)/ 2); //mid is the average between i and j
		int square = mid * mid;
		if (square == filesize)//if mid^2 == size
		{
			width = height = mid;
			break;
		}
		else if (square < filesize)
			i = mid;
		else //square > filesize
			j = mid;
	}
	if (width > 0 && height > 0)//if size was a square number
		Matrix_Matrix(m, width, height);
	
	
	//start
	fscanf(cmf, "%d ", &start);
	Matrix_itop(m, &start, sx, sy);//convert start index to position (sx, sy)
	
	//goal, ignored
	fscanf(cmf, "%d ", &ignore);
	
	//tuples
	for (int i = 0, j = Matrix_height(m); i < j; ++i)
		for (int k = 0, l = Matrix_width(m); k < l; ++k)
			fscanf(cmf, "%d", (int*)Matrix_get(m,k,i));//casting Uint8* to int*
		
	//done	
	fclose(cmf);
	printf ("%s", "Import complete\n");
}