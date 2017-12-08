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


unsigned char turn_timer = 0;
unsigned char turn_direction = 0;

	char neighborRxCount = -1;

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

	unsigned int ticks = 0;

	int my_radius;

	int msrx=0;

	struct neighbor {
		unsigned char distanceTo;
		float angleTo;
		unsigned char countdown;
		float heading;
	};
	neighbor nullNeighbor = {0, 0, 0, 0};

	neighbor myNeighbors[256] = {nullNeighbor};

	unsigned char myID;
	unsigned char neighborCount;

	void updateNeighbors(unsigned char theID, unsigned char theDist, float theTheta, float theirHeading) {
		myNeighbors[theID].distanceTo = theDist;
		myNeighbors[theID].angleTo = theTheta;
		myNeighbors[theID].heading = theirHeading;
		//cout << "theirID " << (int) theID << " their heading" << theirHeading;
		myNeighbors[theID].countdown = 100;
	}

	double magnitude(double x, double y) {
		return sqrt(pow(x,2) + pow(y, 2));
	}


	//main loop
	void loop() {

		ticks++;


			spinup_motors();
			set_motors(kilo_straight_left, kilo_straight_right);

		if (ticks % 5 == 0){
		neighborCount = 0;
		for (int i = 0; i < 256; i++) {
			if (myNeighbors[i].countdown > 0) {
				myNeighbors[i].countdown--;
				neighborCount++;

				align_X += myNeighbors[i].distanceTo*cos(myNeighbors[i].heading);
				align_Y += myNeighbors[i].distanceTo*sin(myNeighbors[i].heading);

				cohesion_X += myNeighbors[i].distanceTo*cos(myNeighbors[i].angleTo);
				cohesion_Y += myNeighbors[i].distanceTo*sin(myNeighbors[i].angleTo);

				//cout << (int) myNeighbors[i].distanceTo << endl;
				repulse_X += (255 - myNeighbors[i].distanceTo)*cos(myNeighbors[i].angleTo + PI);
				repulse_Y += (255 - myNeighbors[i].distanceTo)*sin(myNeighbors[i].angleTo + PI);

			//	cout << (int) myID << ": " << i << " is my neighbor" << endl;
			}
		}

		if (neighborCount == 0) set_color(RGB(1, 0, 0));
		else  set_color(RGB(0, 1, 0));

		cohesion_X = cohesion_X / neighborCount;
		cohesion_Y = cohesion_Y / neighborCount;

		double mag = magnitude(cohesion_X, cohesion_Y);

		cohesion_X = cohesion_X / mag;//
		cohesion_Y = cohesion_Y / mag;//

		mag = magnitude(repulse_X, repulse_Y);
		repulse_X = repulse_X / mag;
		repulse_Y = repulse_Y / mag;

		// Calculate the unit vector (in terms of BOT_SPEED) to the light
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

		//if (neighborCount > 0) {
		//
	//}


		final_X = 2*cohesion_X + (2*repulse_X) + (0.45*taxis_X) + (1.1*align_X);
		final_Y = 2*cohesion_Y + (2*repulse_Y) + (0.45*taxis_Y) + (1.1*align_Y);

		double temp = atan2(final_Y, final_X);

		if (neighborCount == 0) {
				temp = atan2(taxis_Y, taxis_X);
		}

		repulse_X = 0;
		repulse_Y = 0;
		align_X = 0;
		align_Y = 0;
		taxis_X = 0;
		taxis_Y = 0;
		cohesion_X = 0;
		cohesion_Y = 0;

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

		//cout << (int) myID << " theta " << theta << endl;

		float alpha;
		float beta;
		float turn;

		for (char i = 1; i < 9; i = i+2){
			if (out_message.data[i] == message->data[0])
				out_message.data[i] = 0;
		}

		neighborRxCount = (neighborRxCount + 1) % 4;
		out_message.data[1 + (neighborRxCount*2)] = message->data[0]; // THE ID THAT IS INCOMING
		out_message.data[2 + (neighborRxCount*2)] = (unsigned char) (theta + PI) / TURNING_RATE; // THE # right turns it takes to this ID THAT IS INCOMING

		for (char i = 1; i < 9; i = i+2) {
			if (message->data[i] == myID) {

				set_color(RGB(0, 1, 0));

				alpha = (message->data[i+1] * TURNING_RATE) - PI;
				beta = theta;

				alpha = alpha - PI;
				turn = beta + alpha;

				turn = modPI(turn) / 2;

//				cout << (int) myID <<" turn" << " " << turn << endl;



			}
		}

		updateNeighbors(message->data[0], distance, theta, turn);

		rxed = 1;
	}
};
