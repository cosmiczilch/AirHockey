#ifndef POLLER_H
#define POLLER_H 

#include "main.h"
#include "cgame.h"


/* Globals            : **********************************************************************************************************************/ 

SDL_Thread *poller_thread = NULL;
SDL_mutex *availableEvents = NULL;
SDL_cond *donePumping = NULL;
SDL_cond *drainedPump = NULL;
bool queueNonEmpty = false; 

/**********************************************************************************************************************************************/ 

int poller_0( void *data ){
	while( 1 ){ 

		SDL_mutexP( availableEvents );
		if( !queueNonEmpty ){ 
			SDL_CondWait( donePumping, availableEvents );
		}
		printf( "\npoller has the semaphore\n" );

		// NOTE TO SELF : this can handle only 20events at once. see what can be done. 
		SDL_Event events[20];
		int result = SDL_PeepEvents( events, 20, SDL_GETEVENT, SDL_ALLEVENTS );
		if( result == -1 ){ 
			printf("\n PeepEvents failed \n"); 
			exit(0);
		}
		queueNonEmpty = false;

		SDL_mutexV( availableEvents );
		SDL_CondSignal( drainedPump );

		for( int i=0; i<result; i++ ){
			game_Generic = CGame::getGameByGameState( gameState );
			game_Generic->callEventHandler( events[i] );

		} 

		
	}

	return 1;
}

#endif // POLLER_H
