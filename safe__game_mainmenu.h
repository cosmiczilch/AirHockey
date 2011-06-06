#ifndef GAME_MAINMENU_H
#define GAME_MAINMENU_H

#include "cgame.h"
#include "main.h"
#include "ccamera.h"
#include "ctexture.h"
#include <FTGL/ftgl.h>

#include "cbutton.h" 


namespace NSGame_MainMenu{ 

/* File Variables     : **********************************************************************************************************************/ 

int work_thread_anim_delay_msecs = 100.0; 

CButton singlePlayer_button; 
CButton multiPlayer_button; 
CButton controls_button; 
CButton exit_button; 

GLuint filter;						// Which Filter To Use
GLuint fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR };	// Storage For Three Types Of Fog
GLuint fogfilter= 1;					// Which Fog To Use
GLfloat fogColor[4]= {0.7f, 0.7f, 0.7f, 1.0f};		// Fog Color

int numTimes_for_accum_buffer = 1; 

CTexture mainMenu_backgroundImage; 
float mainMenu_width, mainMenu_height; 

/**********************************************************************************************************************************************/ 



SDL_Thread *work_thread = NULL;

void initCamera( ){ 
	game_MainMenu.camera1 = new CCamera( );
	game_MainMenu.camera1->init( CAMERA_TYPE_PERSP, 60.0, 60.0, 30.0, 1.0, 30.0 );

	game_MainMenu.camera2 = new CCamera( );
	game_MainMenu.camera2->init( CAMERA_TYPE_ORTHO, 0.0, 20000.0, 0.0,  mainMenu_width, mainMenu_height, 100.0 );  

	return;
}

void renderScene( ){ 

	glClearColor( 1.0, 1.0, 1.0, 1.0 );
	glClearAccum( 0.0, 0.0, 0.0, 0.0 );

	glClear(GL_ACCUM_BUFFER_BIT);
	for( int i=0; i<numTimes_for_accum_buffer; i++ ){ 

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 

		game_MainMenu.camera1->writeLookAt( );

		glMatrixMode( GL_MODELVIEW ); 
		glLoadIdentity( );

		/* 
		glFogi(GL_FOG_MODE, fogMode[fogfilter]);		// Fog Mode
		glFogfv(GL_FOG_COLOR, fogColor);			// Set Fog Color
		glFogf(GL_FOG_DENSITY, 0.00f);				// How Dense Will The Fog Be
		glHint(GL_FOG_HINT, GL_NICEST);			// Fog Hint Value
		glFogf(GL_FOG_START, 1.0f);				// Fog Start Depth
		glFogf(GL_FOG_END, 100.0f);				// Fog End Depth
		glEnable(GL_FOG);					// Enables GL_FOG
		*/ 

		game_MainMenu.camera1->rotateAboutAxis( ((i%4)>1)?X_AXIS:Z_AXIS, 0.008*(i%2?-1.0:1.0) ); 

		board.draw( );
		glAccum(GL_ACCUM, 1.0/numTimes_for_accum_buffer);

		// singlePlayer_button.draw( );

	}
	glAccum(GL_RETURN, 1.0);

	SDL_GL_SwapBuffers( );

	return;
};

void eventHandler( SDL_Event &event ){ 
	if( event.type == SDL_QUIT ){ 
		exit(0); 
	}
	if( event.type == SDL_KEYDOWN ){ 
		if( event.key.keysym.sym == SDLK_ESCAPE ){ 
			exit(0);
			return;
		}
		if( event.key.keysym.sym == SDLK_UP ){ 
			game_MainMenu.camera1->rotateAboutAxis( Z_AXIS, 5.0 );
			game_MainMenu.camera1->setRedrawTrue( );
		}
		if( event.key.keysym.sym == SDLK_DOWN ){ 
			game_MainMenu.camera1->rotateAboutAxis( Z_AXIS, -5.0 );
			game_MainMenu.camera1->setRedrawTrue( );
		}
	} 

	return; 
}

int work( void * ){ 
	while( 1 ){
		SDL_Delay( work_thread_anim_delay_msecs ); 

		game_MainMenu.camera1->rotateAboutAxis( Z_AXIS, 1.0 );
	}

	return 0; 
}

void init( ){ 
	game_MainMenu.init( );
	game_MainMenu.setGameState( MAIN_MENU );
	NSGame_MainMenu::initCamera( );
	game_MainMenu.setRenderScene( NSGame_MainMenu::renderScene ); 
	game_MainMenu.setEventHandler( NSGame_MainMenu::eventHandler );

	float w = get_GW( ); 
	float h = get_GH( ); 
	singlePlayer_button.init( "SinglePlayer", w*10/100.0, h*85/100.0, 50 );

	mainMenu_width = get_GW( ); 
	mainMenu_height = get_GH( ); 
	mainMenu_backgroundImage.makeTexture( "./resources/images/mainMenu_background.png", PNG );

	work_thread = SDL_CreateThread( work, NULL );

	return; 
}

void cleanup( ){ 
	SDL_KillThread( work_thread ); 

	return; 
}

};

#endif // GAME_MAINMENU_H

