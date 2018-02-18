#ifndef CMF_ADAPTER_H
#define CMF_ADAPTER_H

#include "Matrix.h"
#include "Maze.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//export: maze to CMF
void export(Matrix *m, char *fname, int* sx, int* sy);

//import: CMF to maze
void import(Matrix *m, char *fname, int* sx, int* sy);

#endif
