/************************
**** Assignment 1 - setup.cpp
**** EECS 496
**** Name: Connor Bain
************************/

#define ARENA_WIDTH 1300
#define ARENA_HEIGHT 1300

#define ROBOT_COUNT 1024

#define SAFE_DIST 50

#define SIMULATION_TIME 180 //in seconds


#define TOPOLOGY 1 // for hex
//#define TOPOLOGY 0 // for rectangle

#include <iostream>

void setup_positions(float robot_pos[ROBOT_COUNT][4])
{
	double distance; // Variable to store the distance calc between bots
	bool collision;  // Flag to store if there is a collision

	// Set robot positions
	for (int i = 0; i < 32;i++) {
		for (int j = 0; j < 32; j++) {

			int which = (32*i) + j;

			// For every robot generate an (X,Y,H) 3-tuple
			robot_pos[which][0] = (j * 40) + 25; // Save the new X value

			if (TOPOLOGY == 1 && which % 2 == 1)
				robot_pos[which][1] = (i * 40) + 20 + 25; // Save the new Y value
			else
				robot_pos[which][1] = (i * 40) + 25;


			robot_pos[which][2] = 0; //thetas

			// Special seed IDs
			if (which == 0) robot_pos[which][3] = 0; //id
			else if (which == 31) robot_pos[which][3] = 1;

			// This is only to correct for a bug in the behavior of main where
			// the id is assigned after it generates a random id so this is just
			// generating a random id (excepting my specials)
			else robot_pos[which][3] = rand() % RAND_MAX;
			//else robot_pos[which][3] = 999.0;
		}
	}
}
