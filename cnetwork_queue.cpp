#include "cnetwork_queue.h"

CNetwork_Queue::CNetwork_Queue( ) {
	semaphore = SDL_CreateSemaphore( 1 );

	return;
}

void CNetwork_Queue::insert( CPacketData pdata ) {
	SDL_SemWait( semaphore ); 

	queue_to_send.push_back( pdata );

	SDL_SemPost( semaphore ); 

	return;
}

CPacketData CNetwork_Queue::remove( ) {
	SDL_SemWait( semaphore ); 

	CPacketData packetData = (CPacketData)queue_to_send.front();
	queue_to_send.erase( queue_to_send.begin() );

	SDL_SemPost( semaphore ); 

	return packetData;
}

bool CNetwork_Queue::empty( ) {
	return queue_to_send.empty( );
}

