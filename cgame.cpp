#include "cgame.h" 

/* Static Variables   : **********************************************************************************************************************/ 

#ifndef STATIC_VARIABLES_CGAME
#define STATIC_VARIABLES_CGAME

vector <CGame*> CGame::list_of_games;

#endif // STATIC_VARIABLES_CGAME


/**********************************************************************************************************************************************/ 

CGame::CGame( ){ 

	return; 
}

void CGame::init( ){ 
	semaphore = SDL_CreateSemaphore( 1 );
	list_of_games.push_back( this );

	return; 
}

void CGame::setGameState( GameState gameState ){ 
	SDL_SemWait( semaphore ); 

	this->gameState = gameState; 

	SDL_SemPost( semaphore ); 

	return; 
}

void CGame::setRenderScene( void (* renderScene) (void) ){ 
	SDL_SemWait( semaphore ); 

	this->renderScene = renderScene; 

	SDL_SemPost( semaphore ); 

	return; 
}

void CGame::callRenderScene( ){ 
	SDL_SemWait( semaphore ); 

	this->renderScene( );

	SDL_SemPost( semaphore ); 

	return; 
}

void CGame::callEventHandler( SDL_Event &event ){ 
	SDL_SemWait( semaphore ); 

	this->eventHandler( event );

	SDL_SemPost( semaphore ); 

	return; 
}

void CGame::setEventHandler( void (* eventHandler) (SDL_Event &event) ){ 
	SDL_SemWait( semaphore ); 

	this->eventHandler = eventHandler;

	SDL_SemPost( semaphore ); 

	return; 
}

void CGame::setCamera1( CCamera *camera1 ){
	SDL_SemWait( semaphore ); 

	this->camera1 = camera1; 

	SDL_SemPost( semaphore ); 

	return; 
}

void CGame::setCamera2( CCamera *camera2 ){
	SDL_SemWait( semaphore ); 

	this->camera2 = camera2; 

	SDL_SemPost( semaphore ); 

	return; 
}
	

CGame* CGame::getGameByGameState( GameState gameState ){ 
	for( unsigned int i=0; i<list_of_games.size( ); i++ ){ 
		if( list_of_games[i]->gameState == gameState ){ 
			return list_of_games[i]; 
		}
	}

	return NULL;
}

