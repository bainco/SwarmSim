/************************
**** Assignment 3 - setup_100_1.cpp
**** EECS 496
**** Date: 11/20/2017
**** Name: Connor Bain
************************/

#define ROBOT_COUNT 100
#define ROBOT_SPACING 40

#define ARENA_WIDTH 32*32 + 33*ROBOT_SPACING
#define ARENA_HEIGHT 32*32 + 33*ROBOT_SPACING

#define LIGHT_CENTER_X 1000
#define LIGHT_CENTER_Y 1000

#define SIMULATION_TIME 180 //in seconds

void setup_positions(float robot_pos[ROBOT_COUNT][4])
{
	//assign each robot a random position, centered around light source
	int robot_num;
	int x=100;
	int y=100;
	for(robot_num=0;robot_num<=ROBOT_COUNT;robot_num++) //for 100 robots
	{


		robot_pos[robot_num][0]=x;
		robot_pos[robot_num][1]=y;
		robot_pos[robot_num][3]=0;
		x+=200;
		if ( x>2430)
		{
			x=100;
			y+=200;
		}

		// Everyone is the same group
		robot_pos[robot_num][3] = 0;
	}
}
