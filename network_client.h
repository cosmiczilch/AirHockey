#ifndef NETWORK_CLIENT_H
#define NETWORK_CLIENT_H

#include "SDL_net.h"
#include "globalDefs.h"
#include "cpacketdata.h"
#include "cplayer.h"
#include "cbat.h"
#include "Utilities.h"

#include <string>
using namespace std;

/* Globals            : **********************************************************************************************************************/ 


UDPsocket client_socketDesc;       /* Socket descriptor           */
UDPpacket *client_udpPacket;       /* Pointer to packet memory    */

int network_client_thread_delay = 20;

IPaddress srvadd;		   /* IPaddress of remote server, set in case we are not the server */
string remote_server_ip_address;
extern IPaddress clntaddr; 	   /* IPaddress of remote client, set in case we are the server */

bool client_initd_and_running = false;

extern CPlayer player1, player2;
extern bool must_start_new_game;
extern bool running_on_port_2;

/*********************************************************************************************************************************************/ 

void initNetworkClient( ) {
	if (client_initd_and_running) {
		return;
	}

	printf( "\nInitiating Network Client for MultiPlayer Game ..." );

	/* Open a socket on random port */
	printf( "\nOpening CLient udp Socket ..." );
	if (!(client_socketDesc = SDLNet_UDP_Open(5000)))
	{
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
 
	if (!are_we_the_server) {
		/* Resolve server name  */
		printf( "\nResolving Remote Host %s ...", remote_server_ip_address.c_str() );
		if (SDLNet_ResolveHost(&srvadd, remote_server_ip_address.c_str(), running_on_port_2?NETWORK_SERVER_SOCKET:NETWORK_SERVER_SOCKET_2 ) == -1)
		{
			fprintf(stderr, "SDLNet_ResolveHost\n" );
			exit(EXIT_FAILURE);
		}
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

	while (true) {
		if ( client_initd_and_running ) {
			marshall_and_send_packet( );
		}
		SDL_Delay( network_client_thread_delay );
	}

	return 1;
}


#endif

