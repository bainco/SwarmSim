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

	int msrx=0;
	struct mydata {
		unsigned int data1;
		unsigned int data2;
	};

	//main loop
	void loop()
	{
			// 90% chance of taking a step forward
			if(rand_hard()%100<90)
			{
				spinup_motors();//first start motors
				set_motors(kilo_straight_left, kilo_straight_right);//then command motion
			}

			// 75% chance of turning
			if(rand_hard()%100<75)
			{
				// Equal chance left right
				if(rand_hard()%100<50){
					spinup_motors();
					set_motors(0, kilo_turn_right);
				}
				else {
					spinup_motors();
					set_motors(kilo_turn_left, 0);
				}
			}

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
		// Wasn't sure if we were supposed to leave this stuff alone
		// or not.
		id=id&0xff;
		out_message.type = NORMAL;
		out_message.data[0] = id;
		out_message.data[1] = 0;
		out_message.data[2] = 0;

		// Generate 3 random 2 digit binary values
	  myR = rand_hard() >> 6;
		myG = rand_hard() >> 6;
		myB = rand_hard() >> 6;

		// Go ahead and save these in our out_message buffer
		out_message.data[3] = myR;
		out_message.data[4] = myG;
		out_message.data[5] = myB;

		// Genereate the crc for the out_message
		out_message.crc = message_crc(&out_message);

		// Set our color the randomly generated values
		set_color(RGB(myR, myG, myB));
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
		distance = estimate_distance(distance_measurement);

		// Again, not sure if we need to keep this code
		out_message.data[0] = message->data[0];
		out_message.data[1] = message->data[1];
		out_message.data[2] = message->data[2];

		// Go ahead and read off the RGB data from the incoming message
		myR = message->data[3];
		myG = message->data[4];
		myB = message->data[5];

		// Reset our color appropriately
		set_color(RGB(myR, myG, myB));

		// Update the out_message buffer with our new color
		out_message.data[3] = myR;
		out_message.data[4] = myG;
		out_message.data[5] = myB;

		rxed=1;
	}
};
