#ifndef GAME_MAINMENU_H
#define GAME_MAINMENU_H

#include "cgame.h"
#include "main.h"
#include "ccamera.h"
#include "ctexture.h"

#include "cbutton.h" 


namespace NSGame_MainMenu{ 

/* File Variables     : **********************************************************************************************************************/ 

int work_thread_anim_delay_msecs = 100.0; 

GLuint filter;						// Which Filter To Use
GLuint fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR };	// Storage For Three Types Of Fog
GLuint fogfilter= 1;					// Which Fog To Use
GLfloat fogColor[4]= {0.7f, 0.7f, 0.7f, 1.0f};		// Fog Color

int numTimes_for_accum_buffer = 1; 

CTexture mainMenu_backgroundImageTexture; 
float mainMenu_width, mainMenu_height; 
#define SOMEWHERE_FAR_AWAY 20000
#define SMALL_EPSILON 10.0

// Main Menu Items 

#define SINGLE_PLAYER_BUTTON 0
#define MULTI_PLAYER_BUTTON 1
#define SETTINGS_BUTTON 2
#define CREDITS_BUTTON 3
#define EXIT_BUTTON 4
#define NUM_BUTTONS 5

CButton buttons[NUM_BUTTONS];

// End - Main Menu Items 

float x_mouse, y_mouse;
CTexture my_cursor_texture; 
float side_length__my_cursor;

/**********************************************************************************************************************************************/ 



SDL_Thread *work_thread = NULL;

void initCamera( ){ 
	float w = get_GW( ); 
	float h = get_GH( ); 

	game_MainMenu.camera1 = new CCamera( );
	game_MainMenu.camera1->init( CAMERA_TYPE_PERSP, 50.0, 50.0, 25.0, 1.0, 60.0 );
	game_MainMenu.camera1->setViewPort( w*15/100, 0,  w*85/100, h*85/100 );		// the rotating board is inset in 85% of the main menu area

	game_MainMenu.camera2 = new CCamera( );
	game_MainMenu.camera2->init( CAMERA_TYPE_ORTHO, 0.0, 0.0, SOMEWHERE_FAR_AWAY+SMALL_EPSILON,  mainMenu_width, mainMenu_height, 100.0 );  
	game_MainMenu.camera2->setUpVector( 0.0, 1.0, 0.0 );

	return;
}

void drawMainMenu( ){ 

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix( );
	glLoadIdentity( );

	game_MainMenu.camera2->writeLookAt( true );

	glColor3f( 1.0, 1.0, 0.0 );

	GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 1.0, 1.0, 1.0, 1.0 };

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	mainMenu_backgroundImageTexture.bindTexture( );
	glBegin( GL_QUADS ); 
	glTexCoord2f( 0.0, 1.0 );	glVertex3f( -mainMenu_width/2.0, -mainMenu_height/2.0, 0.0 );
	glTexCoord2f( 1.0, 1.0 );	glVertex3f( mainMenu_width/2.0, -mainMenu_height/2.0, 0.0 );
	glTexCoord2f( 1.0, 0.0 );	glVertex3f( mainMenu_width/2.0, mainMenu_height/2.0, 0.0 );
	glTexCoord2f( 0.0, 0.0 );	glVertex3f( -mainMenu_width/2.0, mainMenu_height/2.0, 0.0 );
	glEnd( ); 
	mainMenu_backgroundImageTexture.unbindTexture( );

	for( int i=0; i<NUM_BUTTONS; i++ ){ 
		buttons[i].draw( );
	}

	glMatrixMode( GL_MODELVIEW );
	glPopMatrix( );

	return; 
}

void blitMyCursorAtCurrentMouse( ){
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix( );
	glLoadIdentity( );

	game_MainMenu.camera2->writeLookAt( true );

	my_cursor_texture.bindTexture( );
	glBegin( GL_QUADS );
	glTexCoord2f( 0.0, 1.0 );	glVertex3f( x_mouse-side_length__my_cursor/2.0, y_mouse-side_length__my_cursor/2.0, SMALL_EPSILON * 2 );
	glTexCoord2f( 1.0, 1.0 );	glVertex3f( x_mouse+side_length__my_cursor/2.0, y_mouse-side_length__my_cursor/2.0, SMALL_EPSILON * 2 );
	glTexCoord2f( 1.0, 0.0 );	glVertex3f( x_mouse+side_length__my_cursor/2.0, y_mouse+side_length__my_cursor/2.0, SMALL_EPSILON * 2 );
	glTexCoord2f( 0.0, 0.0 );	glVertex3f( x_mouse-side_length__my_cursor/2.0, y_mouse+side_length__my_cursor/2.0, SMALL_EPSILON * 2 );
	glEnd( );
	my_cursor_texture.unbindTexture( );

	glMatrixMode( GL_MODELVIEW );
	glPopMatrix( );

	return; 
}

void renderScene( ){ 

	glClearColor( 1.0, 1.0, 1.0, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 

	game_MainMenu.camera1->writeLookAt( );

	glMatrixMode( GL_MODELVIEW ); 
	glLoadIdentity( );

	board.draw( );

	drawMainMenu( );

	blitMyCursorAtCurrentMouse( );

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
			game_MainMenu.camera1->rotateAboutAxis( Z_AXIS, 5.0 );
			game_MainMenu.camera1->setRedrawTrue( );
		}
		if( event.key.keysym.sym == SDLK_DOWN ){ 
			game_MainMenu.camera1->rotateAboutAxis( Z_AXIS, -5.0 );
			game_MainMenu.camera1->setRedrawTrue( );
		}
	} 
	else if( event.type == SDL_MOUSEMOTION ){ 
		float w = get_GW( ); 
		float h = get_GH( ); 
		x_mouse = -w/2.0 + event.motion.x; 
		y_mouse = h/2.0 - event.motion.y; 

		// button animation when mouse hovers on it (pop up) 
		for( int i=0; i<NUM_BUTTONS; i++ ){ 
			buttons[i].hasFocus = false; 
		}
		for( int i=0; i<NUM_BUTTONS; i++ ){ 
			if( buttons[i].pointLiesWithin( x_mouse, y_mouse, side_length__my_cursor/4.0 ) ){ 
				buttons[i].hasFocus = true;
			}
		}
	}
	else if( event.type == SDL_MOUSEBUTTONDOWN ){ 
		for( int i=0; i<NUM_BUTTONS; i++ ){ 
			if( buttons[i].hasFocus ){ 
				buttons[i].clickHandler( x_mouse, y_mouse );
			}
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

void singlePlayerButton_clickHandler( float x, float y ){ 
	gameState = RUNNING;
	gameType = SINGLE_PLAYER;

	return; 
}

void multiPlayerButton_clickHandler( float x, float y ){ 
	gameState = RUNNING;
	gameType = MULTI_PLAYER;

	return; 
}

void settingsButton_clickHandler( float x, float y ){ 

	return; 
}

void creditsButton_clickHandler( float x, float y ){ 

	return; 
}

void exitButton_clickHandler( float x, float y ){ 
	exit(0);

	return; 
}


void init( ){ 
	mainMenu_width = get_GW( ); 
	mainMenu_height = get_GH( ); 
	mainMenu_backgroundImageTexture.makeTexture( "./resources/images/mainMenu_background.png", PNG );

	game_MainMenu.init( );
	game_MainMenu.setGameState( MAIN_MENU );
	NSGame_MainMenu::initCamera( );
	game_MainMenu.setRenderScene( NSGame_MainMenu::renderScene ); 
	game_MainMenu.setEventHandler( NSGame_MainMenu::eventHandler );

	float w = get_GW( ); 
	float h = get_GH( ); 
	buttons[SINGLE_PLAYER_BUTTON].init( (int)SINGLE_PLAYER_BUTTON,  -w*8/100.0, h*20/100.0, SMALL_EPSILON,  w*21/100, h*8.03/100,  "./resources/images/singleplayer_button.png" );
	buttons[MULTI_PLAYER_BUTTON].init( (int)MULTI_PLAYER_BUTTON,  w*22/100.0, h*20/100.0, SMALL_EPSILON,  w*21/100, h*8.03/100,  "./resources/images/multiplayer_button.png" );
	buttons[SETTINGS_BUTTON].init( (int)SETTINGS_BUTTON,  -w*42.5/100.0, -h*6.66/100.0, SMALL_EPSILON,  w*15/100, h*5.83/100,  "./resources/images/settings_button.png" );
	buttons[CREDITS_BUTTON].init( (int)CREDITS_BUTTON,  -w*42.5/100.0, -h*23/100.0, SMALL_EPSILON,  w*13/100, h*4.33/100,  "./resources/images/credits_button.png" );
	buttons[EXIT_BUTTON].init( (int)EXIT_BUTTON,  -w*42.5/100.0, -h*41.66/100.0, SMALL_EPSILON,  w*24.3/100, h*5.0/100,  "./resources/images/exit_button.png" );

	buttons[SINGLE_PLAYER_BUTTON].onClick = singlePlayerButton_clickHandler;
	buttons[MULTI_PLAYER_BUTTON].onClick = multiPlayerButton_clickHandler;
	buttons[SETTINGS_BUTTON].onClick = settingsButton_clickHandler;
	buttons[CREDITS_BUTTON].onClick = creditsButton_clickHandler;
	buttons[EXIT_BUTTON].onClick = exitButton_clickHandler;

	SDL_ShowCursor( 0 );
	side_length__my_cursor = w*5/100.0; 
	my_cursor_texture.makeTexture( "./resources/images/cursor.png", PNG );

	work_thread = SDL_CreateThread( work, NULL );

	return; 
}

void cleanup( ){ 
	SDL_KillThread( work_thread ); 

	return; 
}

};

#endif // GAME_MAINMENU_H

