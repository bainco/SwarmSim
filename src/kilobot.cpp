/************************
**** Assignment 4 - kilobot.cpp
**** EECS 496
**** Date: 12/07/2017
**** Name: Connor Bain
************************/

#pragma once
#include "kilolib.h"
#include <iostream>
#include <cmath>

#define PI 3.14159265358979324

#define TURNING_RATE 0.05
#define BOT_SPEED 4.068
#define ROBOT_SPACING 40

using namespace std;

class mykilobot : public kilobot
{
	unsigned int ticks = 0;
	unsigned char myID;

	unsigned char distance;
	float theta;
	message_t out_message;
	int rxed=0;
	int msrx=0;

	// Force vector storage
	double repulse_X = 0;
	double repulse_Y = 0;

	double align_X = 0;
	double align_Y = 0;

	double cohesion_X = 0;
	double cohesion_Y = 0;

	double taxis_X = 0;
	double taxis_Y = 0;

	double final_X = 0;
	double final_Y = 0;

	// Neighbor Memory
	char neighborRxCount = -1;
	struct neighbor {
		unsigned char distanceTo;
		float angleTo;
		unsigned char countdown;
		float heading;
	};
	neighbor nullNeighbor = {0, 0, 0, 0};
	neighbor myNeighbors[256] = {nullNeighbor};
	unsigned char neighborCount;


	// Procedure to update our neighbor memory
	void updateNeighbors(unsigned char theID, unsigned char theDist, float theTheta, float theirHeading) {
		// Save our neighbor information
		myNeighbors[theID].distanceTo = theDist;
		myNeighbors[theID].angleTo = theTheta;
		myNeighbors[theID].heading = theirHeading;
		// After 40 ticks, we forget our neighbors
		myNeighbors[theID].countdown = 40;
	}

	// Helper to find the magnitude of a vector
	double magnitude(double x, double y) {
		return sqrt(pow(x,2) + pow(y, 2));
	}

	//main loop
	void loop() {

		ticks++;

		// We're always moving forward since we're fixed wing!
		spinup_motors();
		set_motors(kilo_straight_left, kilo_straight_right);

		// Every 5 ticks, recalculate path (artificially lowers turning rate)
		if (ticks % 5 == 0){

			// Go through our neighbor memory and see who's nearby
			neighborCount = 0;
			for (int i = 0; i < 256; i++) {
				if (myNeighbors[i].countdown > 0) {
					// Any recent neigbhors slowly fade
					myNeighbors[i].countdown--;
					neighborCount++;

					// Calculate the alignment vector
					align_X += myNeighbors[i].distanceTo*cos(myNeighbors[i].heading);
					align_Y += myNeighbors[i].distanceTo*sin(myNeighbors[i].heading);

					// Calculate the cohesion vector
					cohesion_X += myNeighbors[i].distanceTo*cos(myNeighbors[i].angleTo);
					cohesion_Y += myNeighbors[i].distanceTo*sin(myNeighbors[i].angleTo);

					// Calculate the repulsion vector
					repulse_X += (255 - myNeighbors[i].distanceTo)*cos(myNeighbors[i].angleTo + PI);
					repulse_Y += (255 - myNeighbors[i].distanceTo)*sin(myNeighbors[i].angleTo + PI);
				}
			}

			// Set color based on "flocking" or "wandering"
			if (neighborCount == 0) set_color(RGB(1, 0, 0));
			else  set_color(RGB(0, 1, 0));

			// Weight and normalize all vectors
			cohesion_X = cohesion_X / neighborCount;
			cohesion_Y = cohesion_Y / neighborCount;
			double mag = magnitude(cohesion_X, cohesion_Y);
			cohesion_X = cohesion_X / mag;//
			cohesion_Y = cohesion_Y / mag;//

			mag = magnitude(repulse_X, repulse_Y);
			repulse_X = repulse_X / mag;
			repulse_Y = repulse_Y / mag;

			taxis_X = cos(angle_to_light);
			taxis_Y = sin(angle_to_light);
			mag = magnitude(taxis_X, taxis_Y);
			taxis_X = taxis_X / mag;
			taxis_Y = taxis_Y / mag;

			align_X = align_X / neighborCount;
			align_Y = align_Y / neighborCount;
			mag = magnitude(align_X, align_Y);
			align_X = align_X / mag ;
			align_Y = align_Y / mag;

			// Calculate the final vector with some weights
			final_X = 2*cohesion_X + (2*repulse_X) + (0.45*taxis_X) + (1.1*align_X);
			final_Y = 2*cohesion_Y + (2*repulse_Y) + (0.45*taxis_Y) + (1.1*align_Y);

			// Calculate a final turn angle
			double temp = atan2(final_Y, final_X);

			// If we don't have any neighbors, just taxis
			if (neighborCount == 0) {
				temp = atan2(taxis_Y, taxis_X);
			}

			// Zero out our forces
			repulse_X = 0;
			repulse_Y = 0;
			align_X = 0;
			align_Y = 0;
			taxis_X = 0;
			taxis_Y = 0;
			cohesion_X = 0;
			cohesion_Y = 0;

			// Handle Turning
			if (temp < 0){
				spinup_motors();
				set_motors(kilo_turn_left, 0);
			}
			else if (temp > 0) {
				spinup_motors();
				set_motors(0, kilo_turn_right);
			}
		}

	}

	void setup() {
		// Generate a random ID
		myID = rand_hard();
		set_color(RGB(1, 0, 0));
		// Establish a blank message
		out_message.type = NORMAL;
		out_message.data[0] = myID;
		out_message.crc = message_crc(&out_message);
	}

	//executed on successfull message send
	void message_tx_success()
	{
		msrx=1;
	}

	//sends message at fixed rate
	message_t *message_tx()
	{
		static int count = rand();

		count--;
		// Send every 10 cycles
		int cycleRate = 5;
		if (!(count % cycleRate))
		{
			return &out_message;
		}
		return NULL;
	}

	// Helper to modPI radian angles
	float modPI(float t) {
		while (t < -1*PI) {
			t += 2*PI;
		}
		while (t > 1*PI) {
			t -= 2*PI;
		}
		return t;
	}

	//receives message
	void message_rx(message_t *message, distance_measurement_t *distance_measurement, float t)
	{
		distance = estimate_distance(distance_measurement);
		theta=t;

		// various angles to calculate heading
		float alpha;
		float beta;
		float turn;

		for (char i = 1; i < 9; i = i+2){
			if (out_message.data[i] == message->data[0])
				out_message.data[i] = 0;
		}

		// Update your beacon with the info about your recent neigbhors
		neighborRxCount = (neighborRxCount + 1) % 4;
		out_message.data[1 + (neighborRxCount*2)] = message->data[0]; // THE ID THAT IS INCOMING
		out_message.data[2 + (neighborRxCount*2)] = (unsigned char) (theta + PI) / TURNING_RATE; // THE # right turns it takes to this ID THAT IS INCOMING

		// Search incoming beacons for you!
		for (char i = 1; i < 9; i = i+2) {
			if (message->data[i] == myID) {

				set_color(RGB(0, 1, 0));

				alpha = (message->data[i+1] * TURNING_RATE) - PI;
				beta = theta;

				alpha = alpha - PI;
				turn = beta + alpha;

				// You only need to turn half-way since the other bot will be doing the same
				turn = modPI(turn) / 2;
			}
		}
		// Save this stuff in our memory
		updateNeighbors(message->data[0], distance, theta, turn);

		rxed = 1;
	}
};
