#ifndef NETWORK_SERVER_H
#define NETWORK_SERVER_H

#include "SDL_net.h"
#include "globalDefs.h"
#include "cpacketdata.h"
#include "cplayer.h"
#include "cbat.h"
#include "Utilities.h"

/* Globals            : **********************************************************************************************************************/ 



int network_server_thread_delay = 20;

IPaddress clntaddr; 		   /* IPaddress of remote client, set in case we are the server */
extern IPaddress srvadd;	   /* IPaddress of remote server, set in case we are not the server */

extern CPlayer player1, player2;
bool running_on_port_2 = false;

/*********************************************************************************************************************************************/ 


void initNetworkServer ( ) {
	/* Open a socket */
	running_on_port_2 = false;

	

	return;
}




void handlePacket( ) {
	CPacketData *packetData = (CPacketData *)server_udpPacket->data;

	switch (packetData->header) {
		case INITIATE_MP_GAME_HEADER :
			if (are_we_the_server) {
				// we are the server, we received a request to start new mp game from remote client, ack back
				printf( "\nWe are the server, received request packet for new game..." );
				must_initiate_new_mp_game = true;
				clntaddr = server_udpPacket->address;
				initNetworkClient( );
			} else {
				// we are not the server, we had sent a request to start a new mp game, and have received an ack from remote server
				// ASSERT( packetData->ack_packet, "Received unexpected INITIATE_MP_GAME_HEADER packet from remote client" );
				printf( "\nNot the server, received ack packet for new game..." );
				gameState = RUNNING;
				new_mp_game_initiated = true;
			}
			break;

		case PLAYER1_COORD_PACKET_HEADER :
			printf( "\nNServer : (data:coords) %ld %f %f %f ", packetData->seqNum, packetData->cordinates.x, \
			packetData->cordinates.y, packetData->cordinates.z );

			// Set Player2's bat from cordinates in packet received
			player2.bat.x = -1 * packetData->cordinates.x;	// the -1 is to correct for courts of player1 and player2
			player2.bat.y = -1 * packetData->cordinates.y;	// the -1 is to correct for courts of player1 and player2
			player2.bat.z = packetData->cordinates.z;
			// Done Setting Player2's bat from cordinates in packet received

			break;

		default :
			break;
	}

	return;
}


int network_server_loop ( void *data ) {
	printf( "\nStarting local network server ..." );

	while ( true ) {
		if ( check_for_packets( ) ) {
			// do something; handle packet
			handlePacket( );
		}
		SDL_Delay( network_server_thread_delay );
	}

	return 1;
}

#endif

