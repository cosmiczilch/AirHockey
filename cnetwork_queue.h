#ifndef CNETWORK_QUEUE_H
#define CNETWORK_QUEUE_H

#include "SDL.h"
#include "SDL_net.h"
#include "cpacketdata.h"
#include "globalDefs.h"

#include <vector>
#include <string>
using namespace std;

class CNetwork_Queue {
	public :
	SDL_sem *semaphore; 
	vector<CPacketData> queue_to_send;

	CNetwork_Queue( );
	void insert( CPacketData pdata );
	CPacketData remove( );
	bool empty( );
};

#endif  // CNETWORK_QUEUE_H

