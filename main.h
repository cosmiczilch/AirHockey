#ifndef MAIN_H
#define MAIN_H 

#include <FTGL/ftgl.h>

/* Globals            : **********************************************************************************************************************/ 

#define FRAME_RATE 10			// 20fps
#define TIME_SLICE 1000.0/FRAME_RATE  	// in milliseconds

SDL_Surface* G_screen; 
int GW=800, GH=600; 

GameState gameState;
CGame game_MainMenu; 
CGame game_Running; 
CGame game_Paused; 
CGame game_Over; 
CGame *game_Generic;

CBoard board;

void (*renderScene) ( void );

FTGLPixmapFont font("./resources/fonts/ARIAL.TTF");

/**********************************************************************************************************************************************/ 



/* Function Prototypes : **********************************************************************************************************************/ 

void setup_rc( void );
void resizeHandler( );
void cleanUp( void ); 
float get_GW( ); 
float get_GH( );

/**********************************************************************************************************************************************/ 


#endif // MAIN_H
