#ifndef GAME_MAINMENU_H
#define GAME_MAINMENU_H

#include "SDL_net.h"

#include "cgame.h"
#include "main.h"
#include "ccamera.h"
#include "ctexture.h"

#include "cbutton.h" 
#include "cpanel.h"
#include "cpanelobjek.h"
#include "clabel.h"
#include "ctextinput.h"
#include "cnetwork_queue.h"
#include "globalDefs.h"

/* Extern Variables   : **********************************************************************************************************************/ 

extern IPaddress remote_machine_ip;

/**********************************************************************************************************************************************/ 

namespace NSGame_MainMenu{ 

/* File Variables     : **********************************************************************************************************************/ 

bool entered = false;

int work_thread_anim_delay_msecs = 25.0; 

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

#define CREATE_SERVER_LABEL 0
#define JOIN_GAME_LABEL 1
#define ENTER_IP_LABEL 2
#define ENTER_IP_LABEL2 3
#define NUM_MULTIPLAYER_LABELS 4
CLabel multiplayer_panel_labels[NUM_MULTIPLAYER_LABELS];

#define MULTIPLAYEROPTIONS_PANEL 6
CPanel multiplayerOptions_panel;


#define MOUSE_SENSITIVITY_LABEL 0
#define MOUSE_SENSITIVITY_LABEL2 1
#define DIFFICULTY_LABEL 2
#define DIFFICULTY_EASY_LABEL 3
#define DIFFICULTY_NORMAL_LABEL 4
#define DIFFICULTY_HARD_LABEL 5
#define SOUND_VOLUME_LABEL 6
#define SOUND_VOLUME_LABEL2 7
#define NUM_SETTINGS_LABELS 8
CLabel settings_panel_labels[NUM_SETTINGS_LABELS];

#define SETTINGS_PANEL 7
CPanel settings_panel;

#define CREDITS_PANEL 8
CPanel credits_panel;

#define IPADDR_TEXTINPUT 0
CTextInput ipaddr_textinput;

// End - Main Menu Items 

float x_mouse, y_mouse;
CTexture my_cursor_texture; 
float side_length__my_cursor;

void startSounds( );

/**********************************************************************************************************************************************/ 


SDL_Thread *work_thread = NULL;

void entryFunction( ) {
	entered = true;
	startSounds( );

	/**
	 * reset board's rim's color to white in mainmenu,
	 * lest it jar too much against the mainmenu colors
	 */
	board.r = 0.8; board.g = 0.8; board.b = 0.8; 

	return;
}


void exitFunction( ) {
	gameState = PAUSED;

	entered = false;

	return;
}

void startSounds( ) {
	soundPlayer.playMusic( );

	return;
}


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

	// draw the mainmenu background
	mainMenu_backgroundImageTexture.bindTexture( );
	glBegin( GL_QUADS ); 
	glTexCoord2f( 0.0, 1.0 );	glVertex3f( -mainMenu_width/2.0, -mainMenu_height/2.0, 0.0 );
	glTexCoord2f( 1.0, 1.0 );	glVertex3f( mainMenu_width/2.0, -mainMenu_height/2.0, 0.0 );
	glTexCoord2f( 1.0, 0.0 );	glVertex3f( mainMenu_width/2.0, mainMenu_height/2.0, 0.0 );
	glTexCoord2f( 0.0, 0.0 );	glVertex3f( -mainMenu_width/2.0, mainMenu_height/2.0, 0.0 );
	glEnd( ); 
	mainMenu_backgroundImageTexture.unbindTexture( );

	// draw the buttons
	for ( int i=0; i<NUM_BUTTONS; i++ ) {
		buttons[i].draw( );
	}

	// draw the multiplayerOptions_panel : it'll get drawn if visible
	multiplayerOptions_panel.draw( );

	// draw the settings_panel : it'll get drawn if visible
	settings_panel.draw( );

	// draw the credits_panel : it'll get drawn if visible
	credits_panel.draw( );

	// fontPrinter.print( "Hello World. ", 30.0, 40.0, 25.0 ); // !!!!!!!!!! REMOVE THIS LINE LATER. 

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

void renderScene( ){ 
	if (!entered) {
		entryFunction( );
	}

	glClearColor( 1.0, 1.0, 1.0, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 

	game_MainMenu.camera1->writeLookAt( );

	glMatrixMode( GL_MODELVIEW ); 
	glLoadIdentity( );

	board.draw( );
	player1.bat.draw( );
	player2.bat.draw( );
	puck.draw( );

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
		if ( ipaddr_textinput.enabled ) {
			ipaddr_textinput.keyPressHandler( event.key.keysym.sym );
		}
	} 
	else if( event.type == SDL_MOUSEMOTION ){ 
		float w = get_GW( ); 
		float h = get_GH( ); 
		x_mouse = -w/2.0 + event.motion.x; 
		y_mouse = h/2.0 - event.motion.y; 

		// multiplayer panel labels animation : when mouse hovers on it (pop up) 
		if ( multiplayerOptions_panel.enabled ) {
			for ( int i=0; i<NUM_MULTIPLAYER_LABELS; i++ ) {
				multiplayer_panel_labels[i].hasFocus = false;
			}
			for ( int i=0; i<NUM_MULTIPLAYER_LABELS; i++ ) {
				if ( multiplayer_panel_labels[i].pointLiesWithin( x_mouse, y_mouse, side_length__my_cursor/4.0 ) ) {
					multiplayer_panel_labels[i].hasFocus = true;
				}
			}
		// settings panel labels animation : when mouse hovers on it (pop up) 
		} else if (settings_panel.enabled) {
			for (int i=0; i<NUM_SETTINGS_LABELS; i++) {
				settings_panel_labels[i].hasFocus = false;
			}
			for (int i=0; i<NUM_SETTINGS_LABELS; i++) {
				if ( settings_panel_labels[i].pointLiesWithin( x_mouse, y_mouse, side_length__my_cursor/4.0 ) ) {
					settings_panel_labels[i].hasFocus = true;
				}
			}
		} else {	// so, button anim happens only when multiplayerOptions_panel and settings_panel are not enabled
			// button animation : when mouse hovers on it (pop up) 
			for( int i=0; i<NUM_BUTTONS; i++ ){ 
				buttons[i].hasFocus = false; 
			}
			for( int i=0; i<NUM_BUTTONS; i++ ){ 
				if( buttons[i].pointLiesWithin( x_mouse, y_mouse, side_length__my_cursor/4.0 ) ){ 
					buttons[i].hasFocus = true;
				}
			}
		}

		

	}
	else if( event.type == SDL_MOUSEBUTTONDOWN ){ 
		for ( int i=0; i<NUM_BUTTONS; i++ ) { 
			if ( buttons[i].hasFocus && \
			buttons[i].pointLiesWithin( x_mouse, y_mouse, side_length__my_cursor/4.0 ) ) { 
				buttons[i].clickHandler( x_mouse, y_mouse );
				return;
			}
		}
		for ( int i=0; i<NUM_MULTIPLAYER_LABELS; i++ ) {
			if ( multiplayer_panel_labels[i].hasFocus && \
			multiplayer_panel_labels[i].pointLiesWithin( x_mouse, y_mouse, side_length__my_cursor/4.0 ) ) { 
				multiplayer_panel_labels[i].clickHandler( x_mouse, y_mouse );
				return;
			}
		}
		for ( int i=0; i<NUM_SETTINGS_LABELS; i++ ) {
			if ( settings_panel_labels[i].hasFocus && \
			settings_panel_labels[i].pointLiesWithin( x_mouse, y_mouse, side_length__my_cursor/4.0 ) ) { 
				settings_panel_labels[i].clickHandler( x_mouse, y_mouse );
				return;
			}
		}
		if (multiplayerOptions_panel.pointLiesWithin( x_mouse, y_mouse, side_length__my_cursor/4.0 )) {
			multiplayerOptions_panel.clickHandler( x_mouse, y_mouse );
			return;
		}
		if (settings_panel.pointLiesWithin( x_mouse, y_mouse, side_length__my_cursor/4.0 )) {
			settings_panel.clickHandler( x_mouse, y_mouse );
			return;
		}
		if (credits_panel.pointLiesWithin( x_mouse, y_mouse, side_length__my_cursor/4.0 )) {
			credits_panel.clickHandler( x_mouse, y_mouse );
			return;
		}

		multiplayerOptions_panel.visible = false;
		multiplayerOptions_panel.enabled = false;
		ipaddr_textinput.enabled = false;
		multiplayerOptions_panel.disableChildren( );

		settings_panel.visible = false;
		settings_panel.enabled = false;
		settings_panel.disableChildren( );

		credits_panel.visible = false;
		credits_panel.enabled = false;
	}


	return; 
}

int work( void * ){ 
	while( 1 ){
		SDL_Delay( work_thread_anim_delay_msecs ); 
		if (gameState != MAIN_MENU) {
			continue;
		}

		game_MainMenu.camera1->rotateAboutAxis( Z_AXIS, 1.0 );
	}

	return 0; 
}

bool checkIpAddr( string input ) {

	return true;
}

void resolve_ip( string ip ) {
	if (SDLNet_ResolveHost(&remote_machine_ip, ip.c_str(), NETWORK_SERVER_SOCKET) == -1)
	{
		fprintf(stderr, "SDLNet_ResolveHost\n" );
		exit(EXIT_FAILURE);
	}

	return;
}


void singlePlayerButton_clickHandler( float x, float y ){ 
	gameType = SINGLE_PLAYER;

	exitFunction( );

	// cleanUp( );

	return; 
}

void multiPlayerButton_clickHandler( float x, float y ){ 
	multiplayerOptions_panel.enableChildren( );
	multiplayer_panel_labels[CREATE_SERVER_LABEL].enabled = true;
	multiplayer_panel_labels[CREATE_SERVER_LABEL].visible = true;
	multiplayer_panel_labels[JOIN_GAME_LABEL].enabled = true;
	multiplayer_panel_labels[JOIN_GAME_LABEL].visible = true;

	multiplayer_panel_labels[ENTER_IP_LABEL].visible = false;
	multiplayer_panel_labels[ENTER_IP_LABEL].enabled = false;
	multiplayer_panel_labels[ENTER_IP_LABEL2].visible = false;
	multiplayer_panel_labels[ENTER_IP_LABEL2].enabled = false;
	ipaddr_textinput.enabled = false;
	ipaddr_textinput.visible = false;

	multiplayerOptions_panel.visible = true;
	multiplayerOptions_panel.enabled = true;


	return; 
}

void settingsButton_clickHandler( float x, float y ){ 
	settings_panel.enableChildren( );

	settings_panel_labels[MOUSE_SENSITIVITY_LABEL].enabled = true;
	settings_panel_labels[MOUSE_SENSITIVITY_LABEL].visible = true;
	settings_panel_labels[MOUSE_SENSITIVITY_LABEL2].enabled = true;
	settings_panel_labels[MOUSE_SENSITIVITY_LABEL2].visible = true;

	settings_panel.visible = true;
	settings_panel.enabled = true;


	return; 
}

void creditsButton_clickHandler( float x, float y ){ 
	credits_panel.visible = true;
	credits_panel.enabled = true;

	return; 
}

void exitButton_clickHandler( float x, float y ){ 
	exit(0);

	return; 
}

void createServer_clickHandler( float x, float y ){
	are_we_the_server = true;

	gameType = MULTI_PLAYER;

	exitFunction( );


	return; 
}

void joinGame_clickHandler( float x, float y ){ 
	multiplayerOptions_panel.disableChildren( );
	multiplayer_panel_labels[CREATE_SERVER_LABEL].enabled = false;
	multiplayer_panel_labels[CREATE_SERVER_LABEL].visible= false;
	multiplayer_panel_labels[JOIN_GAME_LABEL].enabled = false;
	multiplayer_panel_labels[JOIN_GAME_LABEL].visible = false;

	multiplayer_panel_labels[ENTER_IP_LABEL].enabled = true;
	multiplayer_panel_labels[ENTER_IP_LABEL].visible = true;
	multiplayer_panel_labels[ENTER_IP_LABEL2].enabled = true;
	multiplayer_panel_labels[ENTER_IP_LABEL2].visible = true;
	ipaddr_textinput.enabled = true;
	ipaddr_textinput.visible = true;

	return; 
}

void ipaddr_inputDoneHandler( ) {
	if ( checkIpAddr( ipaddr_textinput.inputText ) ) {	// this is always true for now.

		resolve_ip( ipaddr_textinput.inputText );

		gameType = MULTI_PLAYER;
		are_we_the_server = false;

		exitFunction( );

		// cleanUp( );
	}

	return;
}

void set_difficulty_level_easy( float, float ) {
	difficulty_level = DIFFICULTY_LEVEL_EASY;
	PLAYER2_THINK_AHEAD_TIME = PLAYER2_THINK_AHEAD_TIME_EASY;
}

void set_difficulty_level_medium( float, float ) {
	difficulty_level = DIFFICULTY_LEVEL_MEDIUM;
	PLAYER2_THINK_AHEAD_TIME = PLAYER2_THINK_AHEAD_TIME_MEDIUM;
}

void set_difficulty_level_hard( float, float ) {
	difficulty_level = DIFFICULTY_LEVEL_HARD;
	PLAYER2_THINK_AHEAD_TIME = PLAYER2_THINK_AHEAD_TIME_HARD;
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

	// Init buttons
	float w = get_GW( ); 
	float h = get_GH( ); 
	buttons[SINGLE_PLAYER_BUTTON].init( (int)SINGLE_PLAYER_BUTTON,  -w*8/100.0, h*20/100.0, SMALL_EPSILON,  w*21/100, h*8.03/100,  \
	"./resources/images/singleplayer_button.png" );
	buttons[MULTI_PLAYER_BUTTON].init( (int)MULTI_PLAYER_BUTTON,  w*22/100.0, h*20/100.0, SMALL_EPSILON,  w*21/100, h*8.03/100,  \
	"./resources/images/multiplayer_button.png" );
	buttons[SETTINGS_BUTTON].init( (int)SETTINGS_BUTTON,  -w*42.5/100.0, -h*6.66/100.0, SMALL_EPSILON,  w*15/100, h*5.83/100,  \
	"./resources/images/settings_button.png" );
	buttons[CREDITS_BUTTON].init( (int)CREDITS_BUTTON,  -w*42.5/100.0, -h*23/100.0, SMALL_EPSILON,  w*13/100, h*4.33/100,  \
	"./resources/images/credits_button.png" );
	buttons[EXIT_BUTTON].init( (int)EXIT_BUTTON,  -w*42.5/100.0, -h*41.66/100.0, SMALL_EPSILON,  w*24.3/100, h*5.0/100,  \
	"./resources/images/exit_button.png" );

	buttons[SINGLE_PLAYER_BUTTON].onClick = singlePlayerButton_clickHandler;
	buttons[MULTI_PLAYER_BUTTON].onClick = multiPlayerButton_clickHandler;
	buttons[SETTINGS_BUTTON].onClick = settingsButton_clickHandler;
	buttons[CREDITS_BUTTON].onClick = creditsButton_clickHandler;
	buttons[EXIT_BUTTON].onClick = exitButton_clickHandler;
	// Done Init'ing buttons

	// Init multiplayerOptions_panel
	multiplayerOptions_panel.init( (int)MULTIPLAYEROPTIONS_PANEL, w*60/100.0, h*60/100,  0.0, 0.0, 2*SMALL_EPSILON, \
	"./resources/images/panel.png" );
	multiplayerOptions_panel.visible = false;
	multiplayerOptions_panel.enabled = false;
	multiplayerOptions_panel.onClick = NULL;

	multiplayer_panel_labels[CREATE_SERVER_LABEL].init( (int)CREATE_SERVER_LABEL, w*20/100.0, h*10/100.0,  -w*14/100.0, h*10/100.0, 2.2*SMALL_EPSILON );
	multiplayer_panel_labels[CREATE_SERVER_LABEL].setLabelText( "Create Local Server" );
	multiplayer_panel_labels[CREATE_SERVER_LABEL].onClick = createServer_clickHandler;

	multiplayer_panel_labels[JOIN_GAME_LABEL].init( (int)JOIN_GAME_LABEL, w*20/100.0, h*10/100.0,  -w*11/100.0, -h*10/100.0, 2.2*SMALL_EPSILON );
	multiplayer_panel_labels[JOIN_GAME_LABEL].setLabelText( "Join Remote Game" );
	multiplayer_panel_labels[JOIN_GAME_LABEL].onClick = joinGame_clickHandler;

	multiplayer_panel_labels[ENTER_IP_LABEL].init( (int)ENTER_IP_LABEL, w*20/100.0, h*10/100.0,  -w*14/100.0, h*10/100.0, 2.2*SMALL_EPSILON );
	multiplayer_panel_labels[ENTER_IP_LABEL].setLabelText( "Enter IP Address:" );
	multiplayer_panel_labels[ENTER_IP_LABEL].popUpAnim = false;

	multiplayer_panel_labels[ENTER_IP_LABEL2].init( (int)ENTER_IP_LABEL2, w*20/100.0, h*10/100.0,  -w*13/100.0, h*0.0/100.0, 2.2*SMALL_EPSILON );
	multiplayer_panel_labels[ENTER_IP_LABEL2].setLabelText( "___.___.___.___" );
	multiplayer_panel_labels[ENTER_IP_LABEL2].popUpAnim = false;

	ipaddr_textinput.init( (int)IPADDR_TEXTINPUT, w*20/100.0, h*10/100.0,  -w*13/100.0, -h*10/100.0, 2.2*SMALL_EPSILON );
	ipaddr_textinput.onInputDone = ipaddr_inputDoneHandler;

	multiplayerOptions_panel.addPanelObjek( &multiplayer_panel_labels[CREATE_SERVER_LABEL] );
	multiplayerOptions_panel.addPanelObjek( &multiplayer_panel_labels[JOIN_GAME_LABEL] );
	multiplayerOptions_panel.addPanelObjek( &multiplayer_panel_labels[ENTER_IP_LABEL] );
	multiplayerOptions_panel.addPanelObjek( &multiplayer_panel_labels[ENTER_IP_LABEL2] );
	multiplayerOptions_panel.addPanelObjek( &ipaddr_textinput );
	// Done Init'ing multiplayerOptions_panel



	// init the settings_panel
	settings_panel.init( (int)SETTINGS_PANEL, w*60/100.0, h*60/100,  0.0, 0.0, 2*SMALL_EPSILON, \
	"./resources/images/panel.png" );
	settings_panel.visible = false;
	settings_panel.enabled = false;
	settings_panel.onClick = NULL;

	settings_panel_labels[MOUSE_SENSITIVITY_LABEL].init( (int)MOUSE_SENSITIVITY_LABEL, w*20/100.0, h*10/100.0,  -w*20/100.0, h*20/100.0, 2.2*SMALL_EPSILON );
	settings_panel_labels[MOUSE_SENSITIVITY_LABEL].setLabelText( "Mouse Sensitivity: during the game" );
	settings_panel_labels[MOUSE_SENSITIVITY_LABEL].popUpAnim = false;
	settings_panel_labels[MOUSE_SENSITIVITY_LABEL].onClick = NULL;
	settings_panel_labels[MOUSE_SENSITIVITY_LABEL].font.fontWidth  = 15;
	settings_panel_labels[MOUSE_SENSITIVITY_LABEL].font.fontHeight = 25;

	settings_panel_labels[MOUSE_SENSITIVITY_LABEL2].init( (int)MOUSE_SENSITIVITY_LABEL2, w*20/100.0, h*10/100.0,  -w*25/100.0, h*15/100.0, 2.2*SMALL_EPSILON );
	settings_panel_labels[MOUSE_SENSITIVITY_LABEL2].setLabelText( "press 'm' to decrease / press 'M' to increase" );
	settings_panel_labels[MOUSE_SENSITIVITY_LABEL2].popUpAnim = false;
	settings_panel_labels[MOUSE_SENSITIVITY_LABEL2].onClick = NULL;
	settings_panel_labels[MOUSE_SENSITIVITY_LABEL2].font.fontWidth  = 15;
	settings_panel_labels[MOUSE_SENSITIVITY_LABEL2].font.fontHeight = 25;

	settings_panel_labels[DIFFICULTY_LABEL].init( (int)DIFFICULTY_LABEL, w*20/100.0, h*10/100.0,  -w*10/100.0, h*5/100.0, 2.2*SMALL_EPSILON );
	settings_panel_labels[DIFFICULTY_LABEL].setLabelText( "Difficulty Level:" );
	settings_panel_labels[DIFFICULTY_LABEL].popUpAnim = false;
	settings_panel_labels[DIFFICULTY_LABEL].onClick = NULL;
	settings_panel_labels[DIFFICULTY_LABEL].font.fontWidth  = 15;
	settings_panel_labels[DIFFICULTY_LABEL].font.fontHeight = 25;

	settings_panel_labels[DIFFICULTY_EASY_LABEL].init( (int)DIFFICULTY_EASY_LABEL, w*4/100.0, h*10/100.0,  -w*20/100.0, h*0/100.0, \
	2.2*SMALL_EPSILON );
	settings_panel_labels[DIFFICULTY_EASY_LABEL].setLabelText( "easy" );
	settings_panel_labels[DIFFICULTY_EASY_LABEL].popUpAnim = true;
	settings_panel_labels[DIFFICULTY_EASY_LABEL].onClick = set_difficulty_level_easy;

	settings_panel_labels[DIFFICULTY_NORMAL_LABEL].init( (int)DIFFICULTY_NORMAL_LABEL, w*4/100.0, h*10/100.0,  -w*5/100.0, h*0/100.0, \
	2.2*SMALL_EPSILON );
	settings_panel_labels[DIFFICULTY_NORMAL_LABEL].setLabelText( "medium" );
	settings_panel_labels[DIFFICULTY_NORMAL_LABEL].popUpAnim = true;
	settings_panel_labels[DIFFICULTY_NORMAL_LABEL].onClick = set_difficulty_level_medium;

	settings_panel_labels[DIFFICULTY_HARD_LABEL].init( (int)DIFFICULTY_HARD_LABEL, w*4/100.0, h*10/100.0,  w*15/100.0, h*0/100.0, \
	2.2*SMALL_EPSILON );
	settings_panel_labels[DIFFICULTY_HARD_LABEL].setLabelText( "hard" );
	settings_panel_labels[DIFFICULTY_HARD_LABEL].popUpAnim = true;
	settings_panel_labels[DIFFICULTY_HARD_LABEL].onClick = set_difficulty_level_hard;

	settings_panel_labels[SOUND_VOLUME_LABEL].init( (int)SOUND_VOLUME_LABEL, w*20/100.0, h*10/100.0,  -w*20/100.0, -h*12/100.0, 2.2*SMALL_EPSILON );
	settings_panel_labels[SOUND_VOLUME_LABEL].setLabelText( "To Adjust Volume: during the game" );
	settings_panel_labels[SOUND_VOLUME_LABEL].popUpAnim = false;
	settings_panel_labels[SOUND_VOLUME_LABEL].onClick = NULL;
	settings_panel_labels[SOUND_VOLUME_LABEL].font.fontWidth  = 15;
	settings_panel_labels[SOUND_VOLUME_LABEL].font.fontHeight = 25;

	settings_panel_labels[SOUND_VOLUME_LABEL2].init( (int)SOUND_VOLUME_LABEL2, w*20/100.0, h*10/100.0,  -w*25/100.0, -h*17/100.0, 2.2*SMALL_EPSILON );
	settings_panel_labels[SOUND_VOLUME_LABEL2].setLabelText( "press 'v' to decrease / press 'V' to increase" );
	settings_panel_labels[SOUND_VOLUME_LABEL2].popUpAnim = false;
	settings_panel_labels[SOUND_VOLUME_LABEL2].onClick = NULL;
	settings_panel_labels[SOUND_VOLUME_LABEL2].font.fontWidth  = 15;
	settings_panel_labels[SOUND_VOLUME_LABEL2].font.fontHeight = 25;

	settings_panel.addPanelObjek( &settings_panel_labels[MOUSE_SENSITIVITY_LABEL] );
	settings_panel.addPanelObjek( &settings_panel_labels[MOUSE_SENSITIVITY_LABEL2] );
	settings_panel.addPanelObjek( &settings_panel_labels[DIFFICULTY_LABEL] );
	settings_panel.addPanelObjek( &settings_panel_labels[DIFFICULTY_EASY_LABEL] );
	settings_panel.addPanelObjek( &settings_panel_labels[DIFFICULTY_NORMAL_LABEL] );
	settings_panel.addPanelObjek( &settings_panel_labels[DIFFICULTY_HARD_LABEL] );
	settings_panel.addPanelObjek( &settings_panel_labels[SOUND_VOLUME_LABEL] );
	settings_panel.addPanelObjek( &settings_panel_labels[SOUND_VOLUME_LABEL2] );
	// Done Init'ing settings_panel

	credits_panel.init( (int)CREDITS_PANEL, w*60/100.0, h*60/100,  0.0, 0.0, 2*SMALL_EPSILON, \
	"./resources/images/credits_panel.png" );
	credits_panel.visible = false;
	credits_panel.enabled = false;
	credits_panel.onClick = NULL;


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

