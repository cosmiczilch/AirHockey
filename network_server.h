#ifndef NETWORK_SERVER_H
#define NETWORK_SERVER_H

#include "SDL_net.h"
#include "globalDefs.h"
#include "cpacketdata.h"
#include "cplayer.h"
#include "cbat.h"

/* Globals            : **********************************************************************************************************************/ 

SDL_Thread *networkServer_thread = NULL;

UDPsocket server_socketDesc;       /* Socket descriptor         */
UDPpacket *server_udpPacket;       /* Pointer to packet memory  */

int network_server_thread_delay = 20;

extern CPlayer player1, player2;

/*********************************************************************************************************************************************/ 


void initNetworkServer ( ) {
	/* Open a socket */
	printf( "\nOpening Server udp Socket... " );
	if (!(server_socketDesc = SDLNet_UDP_Open(NETWORK_SERVER_SOCKET))) {
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		printf( "\nCould not bind to port:2000, probably another instance of AirHockey running" );
		printf( "\non this system. Trying port:6000..." );
		if (!(server_socketDesc = SDLNet_UDP_Open(NETWORK_SERVER_SOCKET_2))) {
			fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
			exit(EXIT_FAILURE);
		}
	}


	printf( "\nAllocating packet... " );
	/* Make space for the packet */
	if (!(server_udpPacket = SDLNet_AllocPacket(UDP_PACKET_SIZE))) {
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	return;
}



static 
bool check_for_packets ( ) {
	if (SDLNet_UDP_Recv(server_socketDesc, server_udpPacket)) {
		printf("\nSomething has come ...\n");
		/*
		printf("\tChan:    %d\n", server_udpPacket->channel);
		printf("\tData:    %s\n", (char *)server_udpPacket->data);
		printf("\tLen:     %d\n", server_udpPacket->len);
		printf("\tMaxlen:  %d\n", server_udpPacket->maxlen);
		printf("\tStatus:  %d\n", server_udpPacket->status);
		printf("\tAddress: %x %x\n", server_udpPacket->address.host, server_udpPacket->address.port);
		*/

		return true;
	}		
	else { 
		return false;
		printf("\nFalse Alarm. \n");
	}
}


int network_server_loop ( void *data ) {
	printf( "\nStarting local network server ..." );

	while ( true ) {
		if ( check_for_packets( ) ) {
			// do something; handle packet
			CPacketData *packetData = (CPacketData *)server_udpPacket->data;
			printf( "\nNServer : (data:coords) %ld %f %f %f ", packetData->seqNum, packetData->cordinates.x, packetData->cordinates.y, packetData->cordinates.z );

			// Set Player2's bat from cordinates in packet received
			player2.bat.x = -1 * packetData->cordinates.x;	// the -1 is to correct for courts of player1 and player2
			player2.bat.y = -1 * packetData->cordinates.y;	// the -1 is to correct for courts of player1 and player2
			player2.bat.z = packetData->cordinates.z;
			// Done Setting Player2's bat from cordinates in packet received
		}
		SDL_Delay( network_server_thread_delay );
	}

	return 1;
}

#endif

