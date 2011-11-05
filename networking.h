#ifndef NETWORKING_H
#define NETWORKING_H

#include "SDL_net.h"
#include "globalDefs.h"
#include "cpacketdata.h"
#include "cplayer.h"
#include "cbat.h"
#include "Utilities.h"
#include "globalDefs.h"
#include "cnetwork_queue.h"

#include <string>
using namespace std;

/* File Variables     : **********************************************************************************************************************/ 

SDL_Thread *networkSending_thread = NULL;
SDL_Thread *networkReceiving_thread = NULL;

UDPsocket socketDesc;
UDPpacket *sending_udpPacket;       /* Pointer to packet memory  */
UDPpacket *receiving_udpPacket;       /* Pointer to packet memory  */

IPaddress remote_machine_ip;

int network_sending_thread_delay_msecs = 100;
int network_receiving_thread_delay_msecs = 100;

extern CNetwork_Queue network_queue;

/*********************************************************************************************************************************************/ 

void initNetworking ( ) {
	printf( "\nInitializing Networking Components... " );

	/* Initialize SDL_net */
	printf( "\nInitializing SDL_net... " );
	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	printf( "\nOpening UDP Socket... " );
	if (!(socketDesc = SDLNet_UDP_Open(NETWORK_SERVER_SOCKET))) {
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		printf( "\nCould not bind to port:2000, probably another instance of AirHockey running" );
		printf( "\non this system. Trying port:6000..." );
		if (!(socketDesc = SDLNet_UDP_Open(NETWORK_SERVER_SOCKET_2))) {
			fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
			exit(EXIT_FAILURE);
		}
	}

	printf( "\nAllocating sending_packet... " );
	/* Make space for the packet */
	if (!(sending_udpPacket = SDLNet_AllocPacket(UDP_PACKET_SIZE))) {
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	printf( "\nAllocating receiving_packet... " );
	/* Make space for the packet */
	if (!(receiving_udpPacket = SDLNet_AllocPacket(UDP_PACKET_SIZE))) {
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	printf( "\nNetworking Components: Done. " );
}

static 
bool check_for_packets ( ) {
	static long int seqNum = -1;

	if (SDLNet_UDP_Recv(socketDesc, receiving_udpPacket)) {
		printf("\nSomething has come ...");
		/*
		   printf("\tChan:    %d\n", server_udpPacket->channel);
		   printf("\tData:    %s\n", (char *)server_udpPacket->data);
		   printf("\tLen:     %d\n", server_udpPacket->len);
		   printf("\tMaxlen:  %d\n", server_udpPacket->maxlen);
		   printf("\tStatus:  %d\n", server_udpPacket->status);
		   printf("\tAddress: %x %x\n", server_udpPacket->address.host, server_udpPacket->address.port);
		 */

		CPacketData *packetData = (CPacketData *)receiving_udpPacket->data;
		if (packetData->seqNum <= seqNum) {
			// udp network issues; out of order packet delivery
			printf( "\nIgnoring out of order packet" );
			return false;
		}
		printf( "\t%ld", seqNum );

		seqNum = packetData->seqNum;

		return true;
	}		
	return false;
}


void marshall_and_send_packet ( ) {
	CPacketData packetData;
	static long int seqNum = 0;

	if ( network_queue.empty() ) {
		return;
	}

	/* Start marshalling */
	packetData = (CPacketData)network_queue.remove( );
	packetData.seqNum = seqNum++;

	sending_udpPacket->data = (Uint8 *)&packetData;
	sending_udpPacket->address = remote_machine_ip;

	sending_udpPacket->len = sizeof( packetData );

	printf( "\nSending some Packet to remote_machine ...\t%ld", seqNum );
	SDLNet_UDP_Send(socketDesc, -1, sending_udpPacket); /* This sets the p->channel */
	/* Send the packet */

	printf( "\tNum packets backed up for sending : %d", network_queue.queue_to_send.size() );

	return;
}

void handlePacket ( UDPpacket *udpPacket ) {
	CPacketData *packetData = (CPacketData *)udpPacket->data;
	switch (gameState) {
		case PAUSED :
			if (gameType == MULTI_PLAYER && \
			    are_we_the_server) {
				/*
				 * we are the server, and having created a local server we are
				 * waiting for remote_machine to join. 
				 * remote_machine has now requested ot start a new mp game, ack back,
				 * also, save remote_machine's ip for further communication first
				 * also advance gameState to RUNNING
				 */
				remote_machine_ip = udpPacket->address;

				CPacketData pdata;
				pdata.header = INITIATE_MP_GAME_HEADER;
				pdata.ack_packet = true;
				network_queue.insert( pdata );


				gameState = RUNNING;

			} else if ( gameType == MULTI_PLAYER && \
				    !are_we_the_server && \
				    packetData->ack_packet) {
				/*
				 * we are Not the server, but we had previously pinged a remote server
				 * asking to initiate a new mp game. That machine has now ack'ed back
				 * Set gameState to RUNNING, and begin the game. 
				 */
				gameState = RUNNING;

			}
			break;

		case RUNNING :
			if (gameType == MULTI_PLAYER) {
				/* 
				 * we are in a MULTI_PLAYER game, and we received a packet
				 * containing the remote_machine's player1's bat's coordinates,
				 * update our player2's bat's coordinates accordingly
				 */
				if (packetData->header == PLAYER1_COORD_PACKET_HEADER ||
				    packetData->header == PUCK_AND_PLAYER1_COORD_PACKET_HEADER) {
					player2.bat.x = -1 * packetData->cordinates.x;
					player2.bat.y = -1 * packetData->cordinates.y;
					player2.bat.z = packetData->cordinates.z;
					if (!are_we_the_server) {
						/*
						 * we are not the server; 
						 * must accept the puck's coordinates from remote server
						 */
						ASSERT( packetData->header == PUCK_AND_PLAYER1_COORD_PACKET_HEADER, 
								"PLAYER1_COORD_PACKET_HEADER received from server");
						puck.x = -1 * packetData->puck_cordinates.x;
						puck.y = -1 * packetData->puck_cordinates.y;
					}
				}
				
			}
			break;

		default :
			break;
	}


	return;
}

int network_sending_loop( void *data ) {
	while (true) {
		if (!network_queue.empty()) {
			marshall_and_send_packet( );
		}
		SDL_Delay(network_sending_thread_delay_msecs);
	}

	return 0;
}

int network_receiving_loop( void *data ) {
	while (true) {
		if ( check_for_packets() ) {
			handlePacket( receiving_udpPacket );
		}

		SDL_Delay(network_receiving_thread_delay_msecs);
	}


	return 0;
}

#endif  // NETWORKING_H

