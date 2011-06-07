#include "cplayer.h"

void CPlayer::init( PlayerID player_ID, float r, float g, float b,  float x, float y, float z ){ 
	this->player_ID = player_ID; 
	this->r = r;
	this->g = g;
	this->b = b;

	bat.init( x, y, z );

	return;
}

