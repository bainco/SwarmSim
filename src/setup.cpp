#define ROBOT_COUNT 50

#define ARENA_WIDTH 1000
#define ARENA_HEIGHT 1000

#define SIMULATION_TIME 180 //in seconds

#include <iostream>

void setup_positions(float robot_pos[ROBOT_COUNT][3])
{
	double distance;
  bool collision;
	// Set robot positions

	cout << (ARENA_HEIGHT - 100) << endl;
	for (int i = 0;i < ROBOT_COUNT;i++)
	{
		int myX;
		int myY;

		collision = true;
		while (collision) {
			collision = false;
			myX = rand() % (ARENA_WIDTH - 200) + 100;
			myY = rand() % (ARENA_HEIGHT - 200) + 100;
			
			for (int j = 0; j < i; j++) {
				distance = sqrt(pow((robot_pos[j][0] - myX), 2) + pow((robot_pos[j][1] - myY), 2));
				if (distance < 50) { collision = true; break; }
			}
		}

		robot_pos[i][0] = myX; //x
		robot_pos[i][1] = myY;   //y
		robot_pos[i][2]= rand() * 2 * PI / RAND_MAX; //thetas
	}


}
