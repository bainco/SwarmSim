/************************
**** Assignment 1 (late) - kilobot.cpp
**** EECS 496
**** Date: 10/28/2017
**** Name: Connor Bain
****
Changes since last submission:
	+ Implemented brute-force multilateration
	+ Tweaked 'r' (comm dist)
************************/

#pragma once
#include "kilolib.h"
#include <iostream>

// Message type codes
#define TYPE_GRADIENT_BROADCAST 1

// SPECIAL SEEDS
#define SEED_A_ID 0
#define SEED_B_ID 1
#define MAX_SEEDS 2

//#define SMOOTHING 0 // for no smoothing
#define SMOOTHING 1  // for smoothing
using namespace std;

class mykilobot : public kilobot
{
	// The image we need to display
	unsigned char theImage[32][32] = {
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,0,0,1,1,1,0,0,0,0,0,0,0,1,1,1,1,0,0,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,1,0,0,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};

	unsigned char distance;
	message_t out_message;
	int rxed=0;

	int motion=0;
	long int motion_timer=0;

	// 0 -> initial state
	// 1 -> received both gradients and stable
	// 2 -> caluclated local positions
	char state;
	int updatedRecently = 200; // just a counter to say whether or not we've updated recently

	// Variables to store location
	float myX, myY;

	// data structure for seeds
	struct seed {
		unsigned char id;
		unsigned char x;
		unsigned char y;
		unsigned char hopcount;
		float smooth_hopcount;
	};
	seed inSeeds[MAX_SEEDS]; // to store seed info in

	int msrx=0;

	// Method to calculate the distance from any point to any seed
	float distToSeed(unsigned char seedID, int x, int y) {
		float dist = sqrt(pow(inSeeds[seedID].x - x, 2) + pow(inSeeds[seedID].y - y, 2));
		return dist;
	}

	// Method to display the color based on our shape and location
	void displayMyColor() {

		int lookupX = myX;
		int lookupY = myY;

		lookupY = 31 - lookupY;

		if (theImage[lookupY][lookupX] == 0)
			set_color(RGB(2, 0, 1));
		else
			set_color(RGB(1, 1, 1));
	}

	//main loop
	void loop()
	{
		// If we're a seed, set location and setup gradient message
		if (id == SEED_A_ID || id == SEED_B_ID) {
			// I'm a seed. I know where I am.
			if (id == SEED_A_ID)
				myX = 0;
			else myX = 31;
				myY = 0;

			out_message.data[0] = TYPE_GRADIENT_BROADCAST;
			out_message.data[1] = id; // send id
			out_message.data[2] = myX; // send x value
			out_message.data[3] = myY;  // send y value
			out_message.data[4] = 1;  // send hop-count
			set_color(RGB(1, 0, 0)); // seeds are red
		}

		// If we received both gradients go ahead and perform multilateration
		else if (state == 1) {
			float r = 3.75;

			float error;
			float theHopCount;
			float max_error = 1000000;
			for (int x = 0; x < 32; x++) {
				for (int y = 0; y < 32; y++) {
					error = 0.0;
					for (int i = 0; i < MAX_SEEDS; i++) {
						if (SMOOTHING == 1)
							theHopCount = inSeeds[i].smooth_hopcount;
						else
							theHopCount = inSeeds[i].hopcount;
						error += fabs(distToSeed(i, x, y) - (r*theHopCount));
					}
					if (error < max_error) {
						myX = x;
						myY = y;
						max_error = error;
					}
				}
			}
			displayMyColor();
		}

		// If you haven't received both gradients (state 0), check to see if you have
		// if not, go ahead and tick, otherwise, guess at your location
		else if (state == 0) {
			updatedRecently--; // tracker for localization readiness

			if (updatedRecently <= 0 && inSeeds[0].hopcount != 255 && inSeeds[1].hopcount != 255) {
				state = 1;
				inSeeds[0].smooth_hopcount = inSeeds[0].hopcount;
				inSeeds[1].smooth_hopcount = inSeeds[1].hopcount;
			}
			// If you haven't received one of the gradients, reset your counter
			if (inSeeds[0].hopcount == 255 || inSeeds[1].hopcount == 255) updatedRecently = 200;
		}
	}

	//executed once at start
	void setup()
	{
		// Max out all seed hopcounts
		for (char i = 0; i < MAX_SEEDS; i++) {
			inSeeds[i].hopcount = 255;
			inSeeds[i].smooth_hopcount = 0.0;
		}

		// We all start in state 0
		state = 0;

		// Prep out message but leave it blank
		out_message.type = NORMAL;
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
		int cycleRate = 20;
		if (id == SEED_A_ID || id == SEED_B_ID)
			cycleRate = 5;
		if (!(count % cycleRate))
		{
			return &out_message;
		}
		return NULL;
	}

	//receives message
	void message_rx(message_t *message, distance_measurement_t *distance_measurement)
	{
		if (id != SEED_A_ID && id != SEED_B_ID) {
			// read in message: [type, id, x, y, hopcount]
			unsigned char inType = message->data[0];
			unsigned char inID   = message->data[1];
			unsigned char inX    = message->data[2];
			unsigned char inY    = message->data[3];
			unsigned char inHop  = message->data[4];

			// Check if it's a valid gradient broadcast
			if (inType == TYPE_GRADIENT_BROADCAST && inHop > 0) {

				inSeeds[inID].id = inID;
				inSeeds[inID].x = inX;
				inSeeds[inID].y = inY;

				if (state == 0 && inHop < inSeeds[inID].hopcount) {
					updatedRecently = 200;
					inSeeds[inID].hopcount = inHop;
				}

				// If smoothing is on, using exponential moving average for smooth hopcounts
				if (state == 1 && SMOOTHING == 1) {
					inSeeds[inID].smooth_hopcount = (0.05* ((float) (inHop - 1))) + (0.95*inSeeds[inID].smooth_hopcount);
				}

				// Propogate the regular message
				out_message.type = NORMAL;
				out_message.data[0] = TYPE_GRADIENT_BROADCAST;
				out_message.data[1] = inID;
				out_message.data[2] = inX;
				out_message.data[3] = inY;
				out_message.data[4] = inSeeds[inID].hopcount + 1;
				out_message.crc = message_crc(&out_message);
			}
			rxed=1;
		}
	}
};
