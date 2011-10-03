#ifndef CPACKETDATA_H
#define CPACKETDATA_H

#include "Utilities.h"

/* packet headers */
#define INITIATE_MP_GAME_HEADER 0
#define START_MP_GAME_HEADER 1
#define PLAYER1_COORD_PACKET_HEADER 2
#define PUCK_COORD_PACKET_HEADER 3
#define SCORE_PACKET_HEADER 4
#define END_MP_GAME_HEADER 5
#define NUM_PACKET_HEADERS 6
/* packet headers */

class CPacketData {
	public:
	int header;
	bool ack_packet;
	long int seqNum;

	SCordinates cordinates;
};


#endif

