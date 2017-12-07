/************************
**** Assignment 3 - kilobot.cpp
**** EECS 496
**** Date: 11/20/2017
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
#define REUPLSE_CAP 6.4
#define C_RAND 0.6

using namespace std;

class mykilobot : public kilobot
{
	unsigned char distance;
	float theta;
	message_t out_message;
	int rxed=0;

	char neighborRxCount = -1;

	int turn_timer = 0;
	int turn_direction = -1;

	double repulse_X = 0;
	double repulse_Y = 0;

	double rand_X = 0;
	double rand_Y = 0;

	double taxis_X = 0;
	double taxis_Y = 0;

	double final_X = 0;
	double final_Y = 0;

	int my_radius;

	int msrx=0;

	unsigned char myID;

	//main loop
	void loop()
	{

		// Always move forward (we're fixed wing aircraft!)
		spinup_motors();
		set_motors(kilo_straight_left, kilo_straight_right);

		// Handle turn count downs
		if (turn_timer > 0) {
			spinup_motors();
			if (turn_direction == 0) {
				set_motors(kilo_turn_left, 0);
			}
			else {
				set_motors(0, kilo_turn_right);
			}
			turn_timer--;
		}

			// Force calculations...
			// Otherwise if we're still moving forward
			// If we get here, it means we need to calculate a new force vector
			else if (false) {

				// Generate a (coarse) random vector (dependent on turn-rate)
				double randTheta = ((rand_hard() % 126) * TURNING_RATE);
				rand_X = cos(randTheta) * C_RAND;
				rand_Y = sin(randTheta) * C_RAND;

				// Scale the repulse vector if necessary
				double repulseMag = sqrt(pow(repulse_X, 2) + pow(repulse_Y, 2));
				if (repulseMag > REUPLSE_CAP) {
					repulse_X = (REUPLSE_CAP / repulseMag) * repulse_X;
					repulse_Y = (REUPLSE_CAP / repulseMag) * repulse_Y;
				}

				// Calculate the unit vector (in terms of BOT_SPEED) to the light
				taxis_X = cos(angle_to_light);
				taxis_Y = sin(angle_to_light);

				// Add them together
				final_X = taxis_X + rand_X + repulse_X;
				final_Y = taxis_Y + rand_Y + repulse_Y;

				// Solve for the components
				double temp = atan2(final_Y, final_X);

				// Set the timers
				turn_timer =  abs(temp) / TURNING_RATE;
				if (temp > 0)
				turn_direction = 1;
				else
				turn_direction = 0;

				// Forget your previous repulsion values
				repulse_X = 0;
				repulse_Y = 0;
			}

	}

	void setup() {
		myID = rand_hard();
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

		cout << "HELLO" << endl;

		count--;
		// Send every 10 cycles
		int cycleRate = 10;
		if (!(count % cycleRate))
		{
			return &out_message;
		}
		return NULL;
	}

	//receives message
	void message_rx(message_t *message, distance_measurement_t *distance_measurement, float t)
	{
		distance = estimate_distance(distance_measurement);
		theta=t;

		float beta;
		float turn;

		neighborRxCount = (neighborRxCount + 1) % 4;

		out_message.data[1 + (neighborRxCount*2)] = message->data[0]; // THE ID THAT IS INCOMING
		out_message.data[2 + (neighborRxCount*2)] = (theta + PI) / TURNING_RATE; // THE # right turns it takes to this ID THAT IS INCOMING

		for (char i = 1; i < 9; i+2) {
			if (message->data[i] == myID) {
				beta = (message->data[i+1] * TURNING_RATE) - PI;

				if (theta <= 0) {
					turn = -1*beta - 180 + fabs(theta);
				}
				else
					turn = -1*beta + 180 - fabs(theta);
			}
		}


		rxed = 1;
	}
};
