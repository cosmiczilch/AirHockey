#ifndef GAME_OVER_H
#define GAME_OVER_H

#include "cgame.h"
#include "main.h"
#include "ccamera.h"
#include "ctexture.h"
#include "cpanel.h"
#include "cpanelobjek.h"
#include "clabel.h"
#include "cbutton.h" 
#include "cpacketdata.h"
#include "cnetwork_queue.h"

#include <cstdlib>
#include <cstdio>


namespace NSGame_Over{ 

/* File Variables     : **********************************************************************************************************************/ 

bool entered = false;

int work_thread_anim_delay_msecs = 100.0; 

GLuint filter;						// Which Filter To Use
GLuint fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR };	// Storage For Three Types Of Fog
GLuint fogfilter= 1;					// Which Fog To Use
GLfloat fogColor[4]= {0.7f, 0.7f, 0.7f, 1.0f};		// Fog Color

int numTimes_for_accum_buffer = 1; 

#define SOMEWHERE_FAR_AWAY 20000
#define SMALL_EPSILON 10.0

float x_mouse, y_mouse;
CTexture my_cursor_texture; 
float side_length__my_cursor;

/* UI items */
#define GAME_OVER_PANEL 0
#define NUM_PANELS_OVER 1
CPanel panels[NUM_PANELS_OVER];

#define GAME_OVER_LABEL 0
#define YOU_LOST_LABEL 1
#define YOU_WON_LABEL 2
#define NUM_LABELS_OVER 3
CLabel labels[NUM_LABELS_OVER];
/* Done UI items */

/* countdown timer */
#define COUNTDOWNTIME_OVER 3000 //msecs
long int ticks;
/* countdown timer */

/**********************************************************************************************************************************************/ 



SDL_Thread *work_thread = NULL;

void entryFunction( ) {
	gameState = OVER;

	entered = true;

	panels[GAME_OVER_PANEL].enabled = true;
	labels[GAME_OVER_LABEL].enabled = true;
	labels[YOU_WON_LABEL].enabled = false;
	labels[YOU_LOST_LABEL].enabled = false;
	if (player1.numGoals > player2.numGoals) {
		labels[YOU_WON_LABEL].enabled = true;
	} else if (player1.numGoals < player2.numGoals) {
		labels[YOU_LOST_LABEL].enabled = true;
	}

	ticks = COUNTDOWNTIME_OVER / work_thread_anim_delay_msecs;

	SDL_ShowCursor( 0 );


	return;
}

void exitFunction( ) {
	entered = false;

	gameState = MAIN_MENU;

	SDL_WarpMouse( get_GW()/2.0, get_GH()/2.0 );

	SDL_ShowCursor( 1 );

	return;
}

void initCamera( ){ 
	float w = get_GW( ); 
	float h = get_GH( ); 

	game_Over.camera1 = new CCamera( );
	game_Over.camera1->init( CAMERA_TYPE_PERSP, 0.0, -board.length * CAMERA_HOW_MUCH_BEHIND_BOARD * 1.2, board.length * CAMERA_HOW_MUCH_ABOVE_BOARD / 1.6, 1.0, 80.0 );
	game_Over.camera1->setEye( 0.0, 0.0, -board.length * CAMERA_HOW_MUCH_ABOVE_BOARD );

	game_Over.camera2 = new CCamera( );
	game_Over.camera2->init( CAMERA_TYPE_ORTHO, 0.0, 0.0, SOMEWHERE_FAR_AWAY+SMALL_EPSILON,  w, h, 100.0 );  
	game_Over.camera2->setUpVector( 0.0, 1.0, 0.0 );

	return;
}

void blitMyCursorAtCurrentMouse( ){
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix( );
	glLoadIdentity( );

	game_MainMenu.camera2->writeLookAt( true );

	my_cursor_texture.bindTexture( );
	glBegin( GL_QUADS );
	glTexCoord2f( 0.0, 1.0 );	glVertex3f( x_mouse-side_length__my_cursor/2.0, y_mouse-side_length__my_cursor/2.0, SMALL_EPSILON * 3 );
	glTexCoord2f( 1.0, 1.0 );	glVertex3f( x_mouse+side_length__my_cursor/2.0, y_mouse-side_length__my_cursor/2.0, SMALL_EPSILON * 3 );
	glTexCoord2f( 1.0, 0.0 );	glVertex3f( x_mouse+side_length__my_cursor/2.0, y_mouse+side_length__my_cursor/2.0, SMALL_EPSILON * 3 );
	glTexCoord2f( 0.0, 0.0 );	glVertex3f( x_mouse-side_length__my_cursor/2.0, y_mouse+side_length__my_cursor/2.0, SMALL_EPSILON * 3 );
	glEnd( );
	my_cursor_texture.unbindTexture( );

	glMatrixMode( GL_MODELVIEW );
	glPopMatrix( );

	return; 
}

void draw_UI_items( ) {
	/* Draw UI items */

	glColor3f( 1.0, 1.0, 0.0 );

	GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 1.0, 1.0, 1.0, 1.0 };

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	game_Over.camera2->writeLookAt( true );
	for (int i=0; i<NUM_PANELS_OVER; i++ ) {
		panels[i].draw( );
	}

	/* Done drawing UI items */

	return;
}

void renderScene( ){ 
	if (!entered) {
		entryFunction( );
	}

	glClearColor( currentTheme->backgroundColor[0], currentTheme->backgroundColor[1], currentTheme->backgroundColor[2], 1.0 );
	glClearAccum( 0.0, 0.0, 0.0, 0.0 );
	glClear(GL_ACCUM_BUFFER_BIT);

	for( int i=0; i<numTimes_for_accum_buffer; i++ ){ 
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 

		game_Over.camera1->writeLookAt( true );

		glMatrixMode( GL_MODELVIEW ); 
		glLoadIdentity( );

		board.draw( );
		player1.bat.draw( );
		player2.bat.draw( );
		puck.draw( );

		/* Draw UI items */
		draw_UI_items( );

		/* Draw Mouse at Cursor */
		blitMyCursorAtCurrentMouse( );

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
		/* do something */
		float w = get_GW( ); 
		float h = get_GH( ); 
		x_mouse = -w/2.0 + event.motion.x; 
		y_mouse = h/2.0 - event.motion.y; 

	}
		

	return; 
}

int work( void * ){ 
	while( 1 ){
		SDL_Delay( work_thread_anim_delay_msecs ); 
		if (gameState != OVER) {
			continue;
		}

		ticks--;

		/* 
		 * if the timer_ticks times out call exitFunction()
		 * and go to main_menu
		 */
		if (entered && !ticks) {
			exitFunction( );
		}

		// do something
	}

	return 0; 
}

void init_UI_items( ) {
	float w = get_GW( ); 
	float h = get_GH( ); 

	panels[GAME_OVER_PANEL].init( (int)GAME_OVER_PANEL, w*60/100.0, h*60/100.0,  0.0, 0.0, 2*SMALL_EPSILON, \
	"./resources/images/panel.png" );
	panels[GAME_OVER_PANEL].visible = true;
	panels[GAME_OVER_PANEL].enabled = true;

	labels[GAME_OVER_LABEL].init( (int)GAME_OVER_LABEL, w*20/100.0, h*10/100.0,  -w*7/100.0, h*10/100.0, 2.2*SMALL_EPSILON );
	labels[GAME_OVER_LABEL].setLabelText( "GAME OVER" );
	labels[GAME_OVER_LABEL].onClick = NULL;

	labels[YOU_LOST_LABEL].init( (int)YOU_LOST_LABEL, w*20/100.0, h*10/100.0,  -w*6/100.0, -h*10/100.0, 2.2*SMALL_EPSILON );
	labels[YOU_LOST_LABEL].setLabelText( "You Lost" );
	labels[YOU_LOST_LABEL].onClick = NULL;

	labels[YOU_WON_LABEL].init( (int)YOU_WON_LABEL, w*20/100.0, h*10/100.0,  -w*6.0/100.0, -h*5/100.0, 2.2*SMALL_EPSILON );
	labels[YOU_WON_LABEL].setLabelText( "You Won" );
	labels[YOU_WON_LABEL].onClick = NULL;

	panels[GAME_OVER_PANEL].addPanelObjek( &labels[GAME_OVER_LABEL] );
	panels[GAME_OVER_PANEL].addPanelObjek( &labels[YOU_LOST_LABEL] );
	panels[GAME_OVER_PANEL].addPanelObjek( &labels[YOU_WON_LABEL] );

	return;
}

void init( ){ 
	float w = get_GW( ); 

	init_UI_items( );
	game_Over.init( );
	game_Over.setGameState( OVER );
	NSGame_Over::initCamera( );
	game_Over.setRenderScene( NSGame_Over::renderScene ); 
	game_Over.setEventHandler( NSGame_Over::eventHandler );

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

#endif // GAME_OVER_H

