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
#define MAX_SEEDS 10
using namespace std;

class mykilobot : public kilobot
{
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
	int myX, myY;

	// Variables to store our color (RGB) values
	unsigned char myR, myG, myB;

	//
	struct seed {
		unsigned char id;
		unsigned char x;
		unsigned char y;
		unsigned char hopcount;
	};

  // Handle up to 10 seeds
	seed inSeeds[MAX_SEEDS];

	int msrx=0;

	void calculateLocalPosition() {

	}

	//main loop
	void loop()
	{
		// If we're a seed, set location and setup gradient message
		if (id == SEED_A_ID || id == SEED_B_ID) {
			// I'm a seed. I know where I am.
			myX = id;
			myY = 0;

			out_message.data[0] = TYPE_GRADIENT_BROADCAST;
			out_message.data[1] = id; // send id
			out_message.data[2] = myX; // send x value
			out_message.data[3] = myY;  // send y value
			out_message.data[4] = 1;  // send hop-count
			set_color(RGB(1, 0, 2));
		}

		else if (state == 0) {
			updatedRecently--;
			//cout << seed1.hopcount << endl;
			if (inSeeds[0].hopcount % 3 == 0)
				set_color(RGB(1, 1, 1));
			else if (inSeeds[0].hopcount % 3 == 1)
				set_color(RGB(2, 2, 2));
			else
				set_color(RGB(0, 0, 0));

			if (updatedRecently <= 0) {
				state = 1;
				set_color(RGB(1,0,2));
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
		//if (state == 0) {
			// [type, id, x, y, hopcount]
			unsigned char inType = message->data[0];
			// type: 1 -> GRADIENT BROADCAST, 2 -> SMOOTHING BROADCAST,
			unsigned char inID = message->data[1]; // We'll say id
			unsigned char inX = message->data[2];
			unsigned char inY = message->data[3];
			unsigned char inHop = message->data[4];

			if (inType == TYPE_GRADIENT_BROADCAST && inHop > 0) {
				if (inHop < inSeeds[inID].hopcount) {
					// Update our memory of the seed
					updatedRecently = 100;
					inSeeds[inID].hopcount = inHop;
					inSeeds[inID].id = inID;
					inSeeds[inID].x = inX;
					inSeeds[inID].y = inY;

					// Propogate the message
					out_message.type = NORMAL;
					out_message.data[0] = TYPE_GRADIENT_BROADCAST;
					out_message.data[1] = inID;
					out_message.data[2] = inX;
					out_message.data[3] = inY;
					out_message.data[4] = inHop + 1;
					out_message.crc = message_crc(&out_message);
				}
			}

		rxed=1;
	}
};
