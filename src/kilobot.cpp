/************************
**** Assignment 1 - kilobot.cpp
**** EECS 496
**** Name: Connor Bain
************************/

#pragma once
#include "kilolib.h"
#include <iostream>

#define SEED_A_ID 0
#define SEED_B_ID 1

using namespace std;

class mykilobot : public kilobot
{
	unsigned char distance;
	message_t out_message;
	int rxed=0;

	int motion=0;
	long int motion_timer=0;

	// 0 -> initial state
	// 1 -> received both gradients
	// 2 -> caluclated local positions
  char state;
	char updating = 100;


	// Variables to store location
	int myX, myY;

	// Variables to store our color (RGB) values
	unsigned char myR, myG, myB;

	//
	struct seed {
		unsigned int id;
		unsigned int x;
		unsigned int y;
		unsigned int hopcount;
	} seed1, seed2;

	int msrx=0;
	struct mydata {
		unsigned int data1;
		unsigned int data2;
	};

	//main loop
	void loop()
	{
		// If we're a seed, set location and setup gradient message
		if (id == SEED_A_ID || id == SEED_B_ID) {
			myX = id;
			myY = 0;

			out_message.data[0] = id; // send id
			out_message.data[1] = myX; // send x value
			out_message.data[2] = myY;  // send y value
			out_message.data[3] = 1;  // send hop-count
			set_color(RGB(1, 0, 2));
		}

		else if (state == 0) {
			updating--;
			//cout << seed1.hopcount << endl;
			if (seed1.hopcount % 3 == 0)
				set_color(RGB(1, 1, 1));
			else if (seed1.hopcount % 3 == 1)
				set_color(RGB(2, 2, 2));
			else
				set_color(RGB(0, 0, 0));

			if (updating <= 0) {
				state = 1;
				set_color(RGB(1,0,2));
			}
		}
	}

	//executed once at start
	void setup()
	{
		seed1.hopcount = 999;
		seed2.hopcount = 999;

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
			// [id, x, y, hopcount]
			int inID = message->data[0]; // We'll say id
			int inX = message->data[1];
			int inY = message->data[2];
			int inHop = message->data[3];

			if (inID == SEED_B_ID && inHop > 0 && seed2.hopcount > inHop) {
				updating = 100;
				cout << id << " update to: " << inHop << endl;
				seed2.hopcount = inHop;
				seed2.id = inID;
				seed2.x = inX;
				seed2.y = inY;

				out_message.type = NORMAL;

				out_message.data[0] = inID;
				out_message.data[1] = inX;
				out_message.data[2] = inY;
				out_message.data[3] = inHop + 1;

				// Genereate the crc for the out_message
				out_message.crc = message_crc(&out_message);
				//set_color(RGB(2, 2, 2));
			}
			if (inID == SEED_A_ID && inHop > 0 && seed1.hopcount > inHop) {
				updating = 100;
				cout << id << " update to: " << inHop << endl;
				seed1.hopcount = inHop;
				seed1.id = inID;
				seed1.x = inX;
				seed1.y = inY;

				out_message.type = NORMAL;

				out_message.data[0] = inID;
				out_message.data[1] = inX;
				out_message.data[2] = inY;
				out_message.data[3] = inHop + 1;

				// Genereate the crc for the out_message
				out_message.crc = message_crc(&out_message);
				//set_color(RGB(2, 2, 2));
			}
	//}

		rxed=1;
	}
};
