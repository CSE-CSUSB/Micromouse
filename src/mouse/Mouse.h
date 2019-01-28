#ifndef MOUSE_H
#define MOUSE_H

#include "../maze_generator/Matrix.h"

typedef struct _Mouse
{
	double x;//position, in cm
	double y;//position, in cm
	double velocity;//velocity
	double theta;//angle which the mouse is pointing measured in degrees
	double velocity_x;//x component of the velocity
	double velocity_y;//y component of the velocity
} Mouse;

void Mouse_init(Mouse* m);//set the mouse in a neutral position
void Mouse_rotate(Mouse* m, double deg);//rotate by angle deg, DO NOT SET
void Mouse_setspeed(Mouse* m, double vel);//set velocity to vel
void Mouse_accelerate(Mouse* m, double vel);//add vel to velocity
void Mouse_move(Mouse* m, double vel);//move the mouse forward one unit of time
void Mouse_setcomponent(Mouse* m);//set the components of velocity based on theta and velocity
void Mouse_discoverwall(Mouse* m, Matrix* maze, double dist);//discover one or more walls given a distance (camera I/O)




#endif