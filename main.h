#ifndef MAIN_H
#define MAIN_H 

/* Globals            : **********************************************************************************************************************/ 

#define FRAME_RATE 30			// 20fps
#define TIME_SLICE 1000.0/FRAME_RATE  	// in milliseconds

SDL_Surface* G_screen; 
int GW=800, GH=600; 

GameState gameState;		// Global gameState, , change this to change the game state
CGame game_MainMenu; 
CGame game_Running; 
CGame game_Paused; 
CGame game_Over; 
CGame *game_Generic;

enum GameType{ 
	SINGLE_PLAYER, 
	MULTI_PLAYER
}gameType;

CBoard board;
CPlayer player1, player2;
CPuck puck;


CFontPrinter fontPrinter;

void (*renderScene) ( void );

#define CAMERA_HOW_MUCH_BEHIND_BOARD 0.60 	// times the board length (use as a multiplicative factor for board.length)
#define CAMERA_HOW_MUCH_ABOVE_BOARD 0.40	// times the board length (use as a multiplicative factor for board.length)

/**********************************************************************************************************************************************/ 



/* Function Prototypes : **********************************************************************************************************************/ 

void setup_rc( void );
void resizeHandler( );
void cleanUp( void ); 
float get_GW( ); 
float get_GH( );
void initObjeks( );

/**********************************************************************************************************************************************/ 


#endif // MAIN_H
