#define ROBOT_COUNT 210
#define ROBOT_SPACING 40

#define ARENA_WIDTH 50*50 + 50*ROBOT_SPACING
#define ARENA_HEIGHT 50*50 + 50*ROBOT_SPACING

#define LIGHT_CENTER_X 2500
#define LIGHT_CENTER_Y 2500

#define SIMULATION_TIME 180 //in seconds

void setup_positions(float robot_pos[ROBOT_COUNT][4])
{

		/*robot_pos[0][0] = 100;
		robot_pos[0][1] = 100;
		robot_pos[0][2] = 0; //theta
		robot_pos[0][3] = 0;



		robot_pos[1][0] = 150;
		robot_pos[1][1] = 150;
		robot_pos[1][2] = 0; //theta
		robot_pos[1][3] = 1;
*/


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

		//robot_pos[robot_num][3] = 0;

		 if(robot_num<=70)
	 		robot_pos[robot_num][3]=0;
		 else if (robot_num <= 140)
		 	robot_pos[robot_num][3] = 1;
			else
				robot_pos[robot_num][3] = 2;


	}


}
