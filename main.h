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

/* network stuff */
CNetwork_Queue network_queue;
bool are_we_the_server = false;
bool must_initiate_new_mp_game = false;
bool new_mp_game_initiated = false;
/* network stuff */

CBoard board;
CPlayer player1, player2;
CPuck puck;

float MOUSE_SENSITIVITY=1/4.0;

#define DIFFICULTY_LEVEL_EASY 0
#define DIFFICULTY_LEVEL_MEDIUM 1
#define DIFFICULTY_LEVEL_HARD 2
int difficulty_level=DIFFICULTY_LEVEL_EASY;

#define PLAYER2_THINK_AHEAD_TIME_EASY 900
#define PLAYER2_THINK_AHEAD_TIME_MEDIUM 600
#define PLAYER2_THINK_AHEAD_TIME_HARD 400
float PLAYER2_THINK_AHEAD_TIME=PLAYER2_THINK_AHEAD_TIME_MEDIUM;	/* milliseconds */
float PLAYER2_THINK_AHEAD_TICKS=1;

CFontPrinter fontPrinter;

CSoundPlayer soundPlayer;

CTheme *theme_list_head;
CTheme *currentTheme;

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
void ASSERT( bool condition, string panic_string );

/**********************************************************************************************************************************************/ 


#endif // MAIN_H
