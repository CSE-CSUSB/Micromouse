#include <math.h>
#include "Mouse.h"
#include "Maze_consts.h"

void Mouse_init(Mouse* m)
{
	m->x = cell_width / 2;//start the mouse in the direct center of the cell (0,0)
	m->y = cell_width / 2;
	m->velocity = 0;
	m->theta = 0; //this would be facing east
	m->velocity_x = 0;
	m->velocity_y = 0;
	
}
void Mouse_rotate(Mouse* m, double deg)
{
	//add deg to theta
	//use fmod() to make sure resulting theta is somewhere between -360 and 360
	//make sure to call setcomponent() at the end of this function
}

void Mouse_setspeed(Mouse* m, double vel)
{
	//make sure to call setcomponent() at the end of this function
}

void Mouse_accelerate(Mouse* m, double vel)
{
	//make sure to call setcomponent() at the end of this function
}

void Mouse_move(Mouse* m, double vel)
{
	//add the components to the current position
}

void Mouse_setcomponent(Mouse* m)
{
	m->velocity_x = m->velocity * sin(m->theta);
	m->velocity_y = m->velocity * cos(m->theta);
}

void Mouse_discoverwall(Mouse* m, Matrix* maze, double dist)
{
	//you're gonna need to use some hardcore math to figure this one out
}