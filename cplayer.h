#ifndef CPLAYER_H
#define CPLAYER_H

#include "cbat.h" 

enum PlayerID{ 
	PLAYER_1=1, 
	PLAYER_2=2
};

class CPlayer{ 
	public :
	PlayerID player_ID; 	// can be one of 1 or 2
	int numGoals; 

	CBat bat;

	void init( PlayerID player_ID,  float r, float g, float b, float x, float y, float z );			// here x, y, and z are the initial co-ordinates of the player's bat

};

#endif  //CPLAYER_H
