/************************
**** Assignment 2 - kilobot.cpp
**** EECS 496
**** Date: 11/22/2017
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

	int motion_timer = 0;
	int turn_timer = 0;
	int wait_timer = 100;
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

	//main loop
	void loop()
	{

		my_radius = 10*pow(2,id+1);

		if (id == 0)
			set_color(RGB(1, 0, 0));
		if (id == 1)
			set_color(RGB(0, 1, 0));
		if (id == 2)
			set_color(RGB(0, 0, 1));

		if (wait_timer == 0) {

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

		else if (motion_timer > 0){
			spinup_motors();
			set_motors(kilo_straight_left, kilo_straight_right);
			motion_timer--;
			if (motion_timer == 0) wait_timer = 100;
		}


		// // TURNING RATE = 0.05 radians per tick
		// //cout << angle_to_light << endl;//sensor gives heading (in rad) to light source
		//
		// else if (fabs(angle_to_light) >= 0.05) {
		//
		// 	angle_to_light
		// 	// turn_timer = (int) (fabs(angle_to_light) / TURNING_RATE);
		// 	// if (angle_to_light > 0){
		// 	// 	turn_direction = 1;
		// 	// }
		// 	// else
		// 	// 	turn_direction = 0;
		// }
		//
		// // else {
		// // 	spinup_motors();
		// // 	set_motors(kilo_straight_left, kilo_straight_right);
		// // }
		//
		// // GENERATE RANDOM VECTOR
		//
		// // Q: How do i generate a random direction?
		// // rand_hard()
		//
		// angle_to_light

		// Really a random turn interval
		else {
		double randTheta = ((rand_hard() % 126) * TURNING_RATE);
		rand_X = cos(randTheta) * C_RAND;
		rand_Y = sin(randTheta) * C_RAND;

		double repulseMag = sqrt(pow(repulse_X, 2) + pow(repulse_Y, 2));
		if (repulseMag > REUPLSE_CAP) {
				repulse_X = (REUPLSE_CAP / repulseMag) * repulse_X;
				repulse_Y = (REUPLSE_CAP / repulseMag) * repulse_Y;
		}

		taxis_X = cos(angle_to_light);
		taxis_Y = sin(angle_to_light);

		final_X = taxis_X + rand_X + repulse_X;
		final_Y = taxis_Y + rand_Y + repulse_Y;

		double temp = atan2(final_Y, final_X);
		turn_timer =  abs(temp) / TURNING_RATE;
		if (temp > 0)
			turn_direction = 1;
		else
			turn_direction = 0;

		motion_timer = (int) sqrt(pow(final_X, 2) + pow(final_Y, 2));

		if (motion_timer > 8) motion_timer = 8;

		repulse_X = 0;
		repulse_Y = 0;
	}
}

	else wait_timer--;

	}


	void setup() {
		out_message.type = NORMAL;
		out_message.data[0] = 0;
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
		// Send every c cycles
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
		double k = 0.2;
		distance = estimate_distance(distance_measurement);
		theta=t;

		double magnitude = 0;
		if (wait_timer > 0) {
				if (distance < 2*my_radius){
					magnitude = abs(k*((2*my_radius/BOT_SPEED) - (distance/BOT_SPEED)));
							repulse_X += magnitude*cos((PI + theta));
							repulse_Y += magnitude*sin((PI + theta));
				}
		}
		rxed = 1;
	}
};
