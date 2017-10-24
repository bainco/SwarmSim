/************************
**** Assignment 1 - kilobot.cpp
**** EECS 496
**** Name: Connor Bain
************************/

#pragma once
#include "kilolib.h"

using namespace std;

class mykilobot : public kilobot
{
	unsigned char distance;
	message_t out_message;
	int rxed=0;

	int motion=0;
	long int motion_timer=0;

	// Variables to store our color (RGB) values
	unsigned char myR, myG, myB;

	//
	struct seed {
		unsigned int id;
		unsigned int x;
		unsigned int y;
	}

	struct seed seedInfo[2];

	int msrx=0;
	struct mydata {
		unsigned int data1;
		unsigned int data2;
	};

	//main loop
	void loop()
	{
		//update message (again, not sure if we're supposed to keep this)
		out_message.type = NORMAL;
		out_message.data[0] = id;
		out_message.data[1] = 0;
		out_message.data[2] = 0;

		out_message.crc = message_crc(&out_message);
	}

	//executed once at start
	void setup()
	{
		// Mask the id (limit to 256) (already there...not sure if removing?)
		id=id&0xff;

		out_message.type = NORMAL;

		// If we're a seed, send gradient
		if (id == 31 || id == 0) {
			out_message.data[0] = id; // send id
			out_message.data[1] = id; // send x value
			out_message.data[2] = 0;  // send y value
			out_message.data[3] = 1;  // send hop-count
		}

		// Genereate the crc for the out_message
		out_message.crc = message_crc(&out_message);

		// Set our color the randomly generated values
		set_color(RGB(0, 0, 0));
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
		// Send every 100 cycles
		if (!(count % 100))
		{
			return &out_message;
		}
		return NULL;
	}

	//receives message
	void message_rx(message_t *message, distance_measurement_t *distance_measurement)
	{
		out_message.data[0] = message->data[0]; // We'll say id
		out_message.data[1] = message->data[1]; // hop count

		// Update the out_message buffer with the propogated packet
		out_message.data[0] = myR;
		out_message.data[1] = myG;

		rxed=1;
	}
};
