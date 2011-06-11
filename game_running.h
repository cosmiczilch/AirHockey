#ifndef GAME_RUNNING_H
#define GAME_RUNNING_H

#include "cgame.h"
#include "main.h"
#include "ccamera.h"
#include "ctexture.h"
#include <FTGL/ftgl.h>

#include "cbutton.h" 


namespace NSGame_Running{ 

/* File Variables     : **********************************************************************************************************************/ 

int work_thread_anim_delay_msecs = 100.0; 

GLuint filter;						// Which Filter To Use
GLuint fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR };	// Storage For Three Types Of Fog
GLuint fogfilter= 1;					// Which Fog To Use
GLfloat fogColor[4]= {0.7f, 0.7f, 0.7f, 1.0f};		// Fog Color

int numTimes_for_accum_buffer = 1; 

#define SOMEWHERE_FAR_AWAY 20000
#define SMALL_EPSILON 10.0

float x_mouse, y_mouse;

/**********************************************************************************************************************************************/ 



SDL_Thread *work_thread = NULL;

void initCamera( ){ 
	game_Running.camera1 = new CCamera( );
	game_Running.camera1->init( CAMERA_TYPE_PERSP, 0.0, -board.length * CAMERA_HOW_MUCH_BEHIND_BOARD, board.length * CAMERA_HOW_MUCH_ABOVE_BOARD, 1.0, 80.0 );

	return;
}


void renderScene( ){ 

	glClearColor( 1.0, 1.0, 1.0, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 

	game_Running.camera1->writeLookAt( );

	glMatrixMode( GL_MODELVIEW ); 
	glLoadIdentity( );

	board.draw( );
	player1.bat.draw( );
	player2.bat.draw( );
	puck.draw( );

	SDL_GL_SwapBuffers( );

	return;
};

void eventHandler( SDL_Event &event ){ 
	if( event.type == SDL_QUIT ){ 
		exit(0); 
	}
	else if( event.type == SDL_KEYDOWN ){ 
		if( event.key.keysym.sym == SDLK_ESCAPE ){ 
			exit(0);
			return;
		}
		if( event.key.keysym.sym == SDLK_UP ){ 
			// do something

		}
		if( event.key.keysym.sym == SDLK_DOWN ){ 
			// do something
			
		}
	} 
	else if( event.type == SDL_MOUSEMOTION ){ 
		float w = get_GW( ); 
		float h = get_GH( ); 
		x_mouse = -w/2.0 + event.motion.x; 
		y_mouse = h/2.0 - event.motion.y; 
		// do something
	}
		

	return; 
}

int work( void * ){ 
	while( 1 ){
		SDL_Delay( work_thread_anim_delay_msecs ); 

		// do something
	}

	return 0; 
}

void init( ){ 
	game_Running.init( );
	game_Running.setGameState( RUNNING );
	NSGame_Running::initCamera( );
	game_Running.setRenderScene( NSGame_Running::renderScene ); 
	game_Running.setEventHandler( NSGame_Running::eventHandler );

	SDL_ShowCursor( 0 );

	work_thread = SDL_CreateThread( work, NULL );

	return; 
}

void cleanup( ){ 
	SDL_KillThread( work_thread ); 

	return; 
}

};

#endif // GAME_RUNNING_H

