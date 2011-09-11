#if 0
#!/bin/sh
g++ -g -Wall -lGLU -lGLEW -lGL `sdl-config --cflags` models.cpp bat_models.cpp puck_models.cpp Transformation.cpp ctexture.cpp cgame.cpp cboard.cpp cbat.cpp cpuck.cpp cplayer.cpp ccamera.cpp cbutton.cpp main.cpp -o main `sdl-config --libs` -lSDL_net -lSDL_image

exit
#endif


#include "SDL.h"
#include "SDL_opengl.h"
#define NO_SDL_GLEXT
#include "SDL_image.h"
#include "SDL_thread.h"
#include "GL/gl.h"
#include "GL/glu.h"

#include "models.h"
#include "bat_models.h"
#include "Transformation.h"
#include "ccamera.h"
#include "ctexture.h"
#include "cboard.h"
#include "cplayer.h"
#include "cpuck.h"
#include "cgame.h"
#include "game_mainmenu.h"
#include "game_running.h"
#include "poller.h"

#include "main.h"

using namespace std; 

int mainLoop( ){
	while( 1 ){ 

		// must pump the eventLoop
		// printf( "\nmainLoop has the semaphore\n" );
		SDL_mutexP( availableEvents );
		if( queueNonEmpty ){ 
			SDL_CondWait( drainedPump, availableEvents );
		}
		SDL_PumpEvents( );
		queueNonEmpty = true; 

		SDL_mutexV( availableEvents );
		SDL_CondSignal( donePumping );
		static int numTimes =0;
		numTimes++;
		// done pumping the eventLoop

		game_Generic = CGame::getGameByGameState( gameState ); 
		game_Generic->callRenderScene( );


		// change this a bit to use SDL_GetTicks( )
		SDL_Delay( TIME_SLICE );

		continue;

	}

	return 1; 
}

void cleanUp( ){ 
	SDL_Quit( ); 
	printf( "\nCleaned up. \n" ); 

	return; 
}

void initThreads( ){ 
	poller_thread = SDL_CreateThread( poller_0, NULL );
	availableEvents = SDL_CreateMutex( );
	donePumping = SDL_CreateCond( );
	drainedPump = SDL_CreateCond( );

	return;
}

void initLighting( ){ 
	glEnable( GL_LIGHTING );

	glEnable( GL_NORMALIZE );

	// glEnable( GL_COLOR_MATERIAL );
	// glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );

	float cmbr[4] = { 0.4, 0.4, 0.4, 1.0 };
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, cmbr );

	float l1Amb[4] = { 0.4, 0.4, 0.4, 1.0 };
	float l1Diff[4] = { 0.4, 0.4, 0.4, 1.0 };
	float l1Spec[4] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv( GL_LIGHT0, GL_AMBIENT, l1Amb );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, l1Diff );
	glLightfv( GL_LIGHT0, GL_SPECULAR, l1Spec );
	float l1Pos[4] = { 0.0, 0.0, 50.0, 1.0 };
	glLightfv( GL_LIGHT0, GL_POSITION, l1Pos );
	glEnable( GL_LIGHT0 );

	return; 
}

void setup_rc( ){ 
	glEnable( GL_TEXTURE_2D );

	glEnable( GL_DEPTH_TEST ); 
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	glShadeModel( GL_SMOOTH ); 

	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST );

	glEnable( GL_POLYGON_SMOOTH ); 
	glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );

	// glEnable(GL_MULTISAMPLE);

	initLighting( );
	
	return; 
}

void initObjeks( ){ 
	board.init( BOARD_WIDTH, BOARD_LENGTH, BOARD_THICKNESS ); 
	player1.init( PLAYER_1,  0.0, 0.4, 0.0,  BOARD_WIDTH/4.0, -BOARD_LENGTH/4.0, BOARD_THICKNESS/2.0 );
	player2.init( PLAYER_2,  0.0, 0.0, 0.4,  -BOARD_WIDTH/4.0, BOARD_LENGTH/4.0, BOARD_THICKNESS/2.0 );
	puck.init( 0.0, 0.0, 0.0,  0.0, 0.0, 0.0 );

	return; 
}

void initGameStates( ){ 
	NSGame_MainMenu::init( );
	NSGame_Running::init( );

	return;
}

float get_GW( ){ 
	const SDL_VideoInfo* vidInfo = SDL_GetVideoInfo( ); 
	return vidInfo->current_w;
}

float get_GH( ){ 
	const SDL_VideoInfo* vidInfo = SDL_GetVideoInfo( ); 
	return vidInfo->current_h;
}

int main( int argc, char *argv[] ){ 

        if ( SDL_Init(SDL_INIT_EVERYTHING) != 0 ) {
                printf("Unable to initialize SDL: %s\n", SDL_GetError());
                return 1;
        }

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	// SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
         
	/*
	 * Initialize fullscreen and get the width and height of the entire screen
	 * and then switch back to !fullscreen and set the video size accordingly.
	 * Works for extended desktops also. 
	 */
        G_screen = SDL_SetVideoMode( 0, 0, 0, SDL_OPENGL ); // *changed*
	float temp_w, temp_h; 
	temp_w = get_GW( );
	temp_h = get_GH( );
	printf ("\n%f : %f\n", (float)temp_w, (float)temp_h );
	if( temp_w/temp_h >= (float)GW/(float)GH ){ 
		temp_w = (float)GW/(float)GH*temp_h;
	} else{ 
		temp_h = (float)GH/(float)GW*temp_w;
	}
	GW=temp_w * 95.0/100.0;  GH=temp_h * 95.0/100.0;
        G_screen = SDL_SetVideoMode( GW, GH, 0, SDL_OPENGL ); // *changed*

	SDL_WM_SetCaption( "first_sdl_ogl", NULL );

	// int Buffers, Samples;
	// glGetIntegerv( GL_SAMPLE_BUFFERS_ARB, &Buffers );
	// glGetIntegerv( GL_SAMPLES_ARB, &Samples );
	/* if( !Buffers || !Samples ) {
		printf( "\n Oh, crap. \n" );
		exit( 0 );
		// you didn't get a FSAA context, probably older hardware.
		// or you asked for more than one buffer, or you asked for
		// some insane number of samples (2, 4, or 8 is about it)
	} else {
		// FSAA was enabled.
	}
	*/

        setup_rc( ); 

	initObjeks( );
	initGameStates( );
	initThreads( );

	mainLoop( );

	cleanUp( ); 

        return 0; 
}

