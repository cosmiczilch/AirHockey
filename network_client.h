#ifndef NETWORK_CLIENT_H
#define NETWORK_CLIENT_H

#include "SDL_net.h"
#include "globalDefs.h"
#include "cpacketdata.h"
#include "cplayer.h"
#include "cbat.h"

#include <string>
using namespace std;

/* Globals            : **********************************************************************************************************************/ 

SDL_Thread *networkClient_thread = NULL;

UDPsocket client_socketDesc;       /* Socket descriptor         */
UDPpacket *client_udpPacket;       /* Pointer to packet memory  */

int network_client_thread_delay = 20;

IPaddress srvadd;		   /* IP address of remote host */
string remote_server_ip_address;

bool client_initd_and_running = false;

extern CPlayer player1, player2;

/*********************************************************************************************************************************************/ 

void initNetworkClient( ) {
	printf( "\nInitiating Network Client for MultiPlayer Game ..." );

	/* Open a socket on random port */
	printf( "\nOpening CLient udp Socket ..." );
	if (!(client_socketDesc = SDLNet_UDP_Open(0)))
	{
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
 
	/* Resolve server name  */
	printf( "\nResolving Remote Host %s ...", remote_server_ip_address.c_str() );
	if (SDLNet_ResolveHost(&srvadd, remote_server_ip_address.c_str(), NETWORK_SERVER_SOCKET ) == -1)
	{
		fprintf(stderr, "SDLNet_ResolveHost\n" );
		exit(EXIT_FAILURE);
	}

	/* Allocate memory for the packet */
	printf( "\nAllocating packet... " );
	if (!(client_udpPacket = SDLNet_AllocPacket(UDP_PACKET_SIZE)))
	{
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	client_initd_and_running = true;

	return;
}

int network_client_loop( void *data ) {
	printf( "\nStarting local network client ..." );

	long int seqNum = 0;
	CPacketData packetData;
	while (true) {
		if ( client_initd_and_running ) {
			if ( gameState == RUNNING ) {
				// Setting Player1's bat's coordinates in packetData
				packetData.cordinates.x = player1.bat.x;
				packetData.cordinates.y = player1.bat.y;
				packetData.cordinates.z = player1.bat.z;
				// Done Setting Player1's bat's coordinates in packetData

				packetData.seqNum = seqNum++;
				printf( "\nNClient : (data:coords)" );

				client_udpPacket->data = (Uint8 *)&packetData;
				client_udpPacket->address.host = srvadd.host;	/* Set the destination host */
				client_udpPacket->address.port = srvadd.port;	/* And destination port */

				client_udpPacket->len = sizeof( packetData );

				SDLNet_UDP_Send(client_socketDesc, -1, client_udpPacket); /* This sets the p->channel */
			}
		}
		SDL_Delay( network_client_thread_delay );
	}

	return 1;
}


#endif

