#ifndef GAME_RUNNING_H
#define GAME_RUNNING_H

#include "cgame.h"
#include "main.h"
#include "ccamera.h"
#include "ctexture.h"

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

#define MOUSE_SENSITIVITY 1/20.0
float x_mouse, y_mouse;
float x_mouse_prev=9999, y_mouse_prev=9999;

/**********************************************************************************************************************************************/ 



SDL_Thread *work_thread = NULL;

void initCamera( ){ 
	game_Running.camera1 = new CCamera( );
	game_Running.camera1->init( CAMERA_TYPE_PERSP, 0.0, -board.length * CAMERA_HOW_MUCH_BEHIND_BOARD * 1.2, board.length * CAMERA_HOW_MUCH_ABOVE_BOARD / 1.6, 1.0, 80.0 );
	game_Running.camera1->setEye( 0.0, 0.0, -board.length * CAMERA_HOW_MUCH_ABOVE_BOARD );

	return;
}


void renderScene( ){ 

	glClearColor( 1.0, 1.0, 1.0, 1.0 );
	glClearAccum( 0.0, 0.0, 0.0, 0.0 );
	glClear(GL_ACCUM_BUFFER_BIT);

	for( int i=0; i<numTimes_for_accum_buffer; i++ ){ 
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 

		game_Running.camera1->writeLookAt( );

		glMatrixMode( GL_MODELVIEW ); 
		glLoadIdentity( );

		board.draw( );
		player1.bat.draw( );
		player2.bat.draw( );
		puck.draw( );

		glAccum(GL_ACCUM, 1.0/numTimes_for_accum_buffer);
	}
	glAccum(GL_RETURN, 1.0);

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
		if( x_mouse_prev != 9999 )
			player1.bat.translate_X( (x_mouse-x_mouse_prev)*MOUSE_SENSITIVITY );
		if( y_mouse_prev != 9999 )
			player1.bat.translate_Y( (y_mouse-y_mouse_prev)*MOUSE_SENSITIVITY );
		x_mouse_prev = x_mouse;
		y_mouse_prev = y_mouse;
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
	x_mouse_prev=player1.bat.x;	y_mouse_prev=player1.bat.y; 
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

