#ifndef CPLAYER_H
#define CPLAYER_H

#include "cbat.h" 
#include "globalDefs.h"

class CPlayer{ 
	public :
	int player_ID; 	// can be one of 1 or 2
	float r, g, b;
	int numGoals; 

	CBat bat;

	void init( int player_ID,  float r, float g, float b, float x, float y, float z );			// here x, y, and z are the initial co-ordinates of the player's bat

};

#endif  //CPLAYER_H
