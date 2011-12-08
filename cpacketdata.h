#ifndef CPACKETDATA_H
#define CPACKETDATA_H

#include "Utilities.h"
#include "cbat.h"
#include "cpuck.h"

/* packet headers */
#define INITIATE_MP_GAME_HEADER 0
#define START_MP_GAME_HEADER 1
#define PLAYER1_COORD_PACKET_HEADER 2
#define PUCK_AND_PLAYER1_COORD_PACKET_HEADER 3
#define PUCK_COORD_PACKET_HEADER 4
#define SCORE_PACKET_HEADER 5
#define END_MP_GAME_HEADER 6
#define NUM_PACKET_HEADERS 7
/* packet headers */

class CPacketData {
	public:
	int header;
	int ack_packet;
	int seqNum;

	/**
	 * player1's bat data
	 */
	float batx, baty;
	float batsx, batsy;
	float batvelocityx, batvelocityy;

	/**
	 * puck's data
	 */
	float puckx, pucky;
	float puckvelocityx, puckvelocityy;

};


#endif

