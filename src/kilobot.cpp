/************************
**** Assignment 1 - kilobot.cpp
**** EECS 496
**** Name: Connor Bain
************************/

#pragma once
#include "kilolib.h"
#include <iostream>

#define TYPE_GRADIENT_BROADCAST 1
#define SEED_A_ID 0
#define SEED_B_ID 1
#define MAX_SEEDS 2

// #define SMOOTHING 0 // for no smoothing
#define SMOOTHING 0  // for smoothing
using namespace std;

class mykilobot : public kilobot
{
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
	char updatedRecently = 100;

	// Variables to store location
	float myX, myY;

	//
	struct seed {
		unsigned char id;
		unsigned char x;
		unsigned char y;
		unsigned char hopcount;
		unsigned float smooth_hopcount;
	};

	// Handle up to 10 seeds
	seed inSeeds[MAX_SEEDS];

	int msrx=0;

	float distToSeed(unsigned char seedID) {
		return sqrt(pow(inSeeds[seedID].x - myX, 2) + pow(inSeeds[seedID].y - myY, 2));
	}

	void displayMyColor() {

		int lookupX = (myX / 40);
		int lookupY = (myY / 40);

		cout << "lookup "<< lookupX << " " << lookupY << endl;;

		if (theImage[lookupX][lookupY] == 0)
		set_color(RGB(2, 0, 1));
		else
		set_color(RGB(0, 0, 0));
	}

	//main loop
	void loop()
	{
		// If we're a seed, set location and setup gradient message
		if (id == SEED_A_ID || id == SEED_B_ID) {
			// I'm a seed. I know where I am.
			myX = pos[0];
			myY = pos[1];

			out_message.data[0] = TYPE_GRADIENT_BROADCAST;
			out_message.data[1] = id; // send id
			out_message.data[2] = myX; // send x value
			out_message.data[3] = myY;  // send y value
			out_message.data[4] = 1;  // send hop-count
			set_color(RGB(1, 0, 0));
		}

		else if (state == 1) {
			// LOCALIZE
			float deltaX = 0.0;
			float deltaY = 0.0;

			float partDiffX = 0.0;
			float partDiffY = 0.0;

			float r = 40;
			float alpha = 0.01;

			for (int i = 0; i < MAX_SEEDS; i++) {
				if (inSeeds[i].hopcount > 0) {
					partDiffX += ((myX - inSeeds[i].x)*(1 - (distToSeed(i)/(r*inSeeds[i].hopcount))));
					partDiffY += ((myY - inSeeds[i].y)*(1 - (distToSeed(i)/(r*inSeeds[i].hopcount))));
				}
			}

			deltaX = (-1 * alpha) * partDiffX;
			deltaY = (-1 * alpha) * partDiffY;

			myX += deltaX;
			myY += deltaY;

			cout << "actual: " << pos[0] << "," << pos[1] << " predicted: " << myX << "," << myY << endl;
			displayMyColor();
		}

		else if (state == 0) {
			updatedRecently--; // tracker for localization readiness

			if (updatedRecently <= 0) {
				state = 1;
				// find closest seed and just assume we're where they are
				unsigned char distance = 255;
				for (int i = 0; i < MAX_SEEDS; i++) {
					if (inSeeds[i].hopcount < distance && inSeeds[i].hopcount > 0) {
						distance = inSeeds[i].hopcount;
						myX = inSeeds[i].x;
						myY = inSeeds[i].y;
					}
				}
			}
		}
	}

	//executed once at start
	void setup()
	{
		// Max out all seed hopcounts
		for (char i = 0; i < MAX_SEEDS; i++) {
			inSeeds[i].hopcount = 255;
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
		// Send every 10 cycles
		if (!(count % 10))
		{
			return &out_message;
		}
		return NULL;
	}

	//receives message
	void message_rx(message_t *message, distance_measurement_t *distance_measurement)
	{
		// [type, id, x, y, hopcount]
		unsigned char inType = message->data[0];
		unsigned char inID   = message->data[1];
		unsigned char inX    = message->data[2];
		unsigned char inY    = message->data[3];
		unsigned char inHop  = message->data[4];

		cout << "message rx: " << (int) inType << " " << (int) inID << " " << (int) inX << " " << (int) inY << " " << (int) inHop << endl;

		// Check if it's a valid gradient broadcast
		if (inType == TYPE_GRADIENT_BROADCAST && inHop > 0) {

			inSeeds[inID].id = inID;
			inSeeds[inID].x = inX;
			inSeeds[inID].y = inY;

			//cout << "recorded x: " << inSeeds[inID].x << " " << inSeeds[inID].y << endl;
			if (state == 0 && inHop < inSeeds[inID].hopcount) {
				updatedRecently = 100;
				inSeeds[inID].hopcount = inHop;
				inSeeds[inID].smooth_hopcount = inHop;
			}

			// If smoothing is on, using exponential moving average for seed hopcounts
			if (state == 1 && SMOOTHING == 1) {
				inSeeds[inID].smooth_hopcount = (0.6*inHop) + (0.4*inSeeds[inID].smooth_hopcount);
			}

			// Propogate the regular message
			out_message.type = NORMAL;
			out_message.data[0] = TYPE_GRADIENT_BROADCAST;
			out_message.data[1] = inID;
			out_message.data[2] = inX;
			out_message.data[3] = inY;
			out_message.data[4] = inseeds[inID].hopcount + 1;
			out_message.crc = message_crc(&out_message);
		}
		rxed=1;
	}
};
