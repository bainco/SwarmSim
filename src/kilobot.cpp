/************************
**** Assignment 1 - kilobot.cpp
**** EECS 496
**** Name: Connor Bain
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

#define SMOOTHING 1 // for no smoothing
//#define SMOOTHING 1  // for smoothing
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
	int updatedRecently = 500; // just a counter to say whether or not we've updated recently1000

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

	// Handle up to 10 seeds
	seed inSeeds[MAX_SEEDS];

	int msrx=0;

	float distToSeed(unsigned char seedID) {
		return sqrt(pow(inSeeds[seedID].x - myX, 2) + pow(inSeeds[seedID].y - myY, 2));
	}

	void displayMyColor() {

		int lookupX = myX;
		int lookupY = myY;

		if (lookupX < 0)
			lookupX = 0;
		else if (lookupX > 31)
			lookupX = 31;

			if (lookupY < 0)
				lookupY = 0;
			else if (lookupY > 31)
				lookupY = 31;

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

		// If we received both gradients (and haven't updated recently)
		// go ahead and attempt to localize
		else if (state == 1) {
			// LOCALIZE
			float r_mod = 3.75;
			float d = 31;
			float R;
			float r;
			if (SMOOTHING == 0) {
				R = r_mod*((int)inSeeds[0].hopcount);
				r = r_mod*((int)inSeeds[1].hopcount);
		}
		else {
		  R = r_mod*(inSeeds[0].smooth_hopcount);
			r = r_mod*(inSeeds[1].smooth_hopcount);
		}
			cout << "mypos " << pos[0] << " " << pos[1]<< "hop measure " << (int)inSeeds[0].hopcount << " " << R << " " << (int)inSeeds[1].hopcount << " "  << r << endl;

			myX = (pow(d,2)-pow(r, 2)+pow(R, 2)) / (2*d);
			myY = sqrt((-d + r - R)*(-d - r + R)*(-d + r + R)*(d + r + R))/(2*d);

			state = 2;

			//cout << "actual: " << pos[0] << "," << pos[1] << " predicted: " << myX << "," << myY << endl;
			displayMyColor();
		}

		// If you haven't received both gradients (state 0), check to see if you have
		// if not, go ahead and tick, otherwise, guess at your location
		else if (state == 0) {
			updatedRecently--; // tracker for localization readiness

			if (updatedRecently <= 0) {
				state = 1;
				if (inSeeds[0].hopcount == 2)
					set_color(RGB(1,1,1));
				else if (inSeeds[0].hopcount == 3)
						set_color(RGB(2,2,2));
					else
						set_color(RGB(0,0,0));
				// find losest seed and just assume we're where they are
				unsigned char distance = 255;
				for (int i = 0; i < MAX_SEEDS; i++) {
					if (inSeeds[i].hopcount < distance && inSeeds[i].hopcount > 0) {
						distance = inSeeds[i].hopcount;
						myX = inSeeds[i].x;
						myY = inSeeds[i].y;
					}
				}
				/*if (myX == 31)
					set_color(RGB(0,2,0));
				else
					set_color(RGB(0,0,2));*/
			}
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
		// read in message: [type, id, x, y, hopcount]
		unsigned char inType = message->data[0];
		unsigned char inID   = message->data[1];
		unsigned char inX    = message->data[2];
		unsigned char inY    = message->data[3];
		unsigned char inHop  = message->data[4];

		//cout << "message rx: " << (int) inType << " " << (int) inID << " " << (int) inX << " " << (int) inY << " " << (int) inHop << endl;

		// Check if it's a valid gradient broadcast
		if (inType == TYPE_GRADIENT_BROADCAST && inHop > 0) {

			inSeeds[inID].id = inID;
			inSeeds[inID].x = inX;
			inSeeds[inID].y = inY;

			//cout << "recorded x: " << inSeeds[inID].x << " " << inSeeds[inID].y << endl;
			if (state == 0 && inHop < inSeeds[inID].hopcount) {
				updatedRecently = 500;
				inSeeds[inID].hopcount = inHop;
				inSeeds[inID].smooth_hopcount = inHop;
			}

			// If smoothing is on, using exponential moving average for seed hopcounts
			if (state == 1 && SMOOTHING == 1) {
				inSeeds[inID].smooth_hopcount = ((float) (0.9*inHop)) + (0.1*inSeeds[inID].smooth_hopcount);
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
};
