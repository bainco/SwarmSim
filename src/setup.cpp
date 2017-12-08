/************************
**** Assignment 3 - setup.cpp
**** EECS 496
**** Date: 11/20/2017
**** Name: Connor Bain
************************/
#define ROBOT_COUNT 10
#define ROBOT_SPACING 40

#define ARENA_WIDTH 4000
#define ARENA_HEIGHT 4000

#define LIGHT_CENTER_X 2000
#define LIGHT_CENTER_Y 2000

#define PI 3.14159265358979324

#define SIMULATION_TIME 180 //in seconds

void setup_positions(float robot_pos[ROBOT_COUNT][4])
{

	//assign each robot a random position, centered around light source
	int robot_num;

	for(robot_num=0;robot_num<=ROBOT_COUNT;robot_num++) //for 100 robots
	{
		robot_pos[robot_num][0]= rand() % ARENA_WIDTH;
		robot_pos[robot_num][1]= rand() % ARENA_HEIGHT;

		robot_pos[robot_num][2]=(((double)(rand() % 360)) - 180)/360*2*PI;

	}
}
