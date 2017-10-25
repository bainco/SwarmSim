/************************
**** Assignment 1 - setup.cpp
**** EECS 496
**** Name: Connor Bain
************************/

#define ARENA_WIDTH 2040
#define ARENA_HEIGHT 2040

#define ROBOT_COUNT 1024

#define SAFE_DIST 50

#define SIMULATION_TIME 180 //in seconds

#include <iostream>

void setup_positions(float robot_pos[ROBOT_COUNT][4])
{
	double distance; // Variable to store the distance calc between bots
	bool collision;  // Flag to store if there is a collision


	// RECTANGLE
	// Set robot positions
	for (int i = 0; i < 32;i++) {
		for (int j = 0; j < 32; j++) {

			int which = (32*i) + j;
			// For every robot generate an (X,Y,H) 3-tuple
			robot_pos[which][0] = (j * 40) + 25; // Save the new X value
			robot_pos[which][1] = (i * 40) + 25; // Save the new Y value
			robot_pos[which][2] = 0; //thetas

			// Special seed IDs
			if (which == 0)  robot_pos[which][3] = 0.0; //id
			else if (which == 31) robot_pos[which][3] = 31.0;
			else robot_pos[which][3] = 999.0;
		}
	}
}
