#include "cbat.h"

void CBat::init( float x, float y, float z ){ 
	this->x = x; 
	this->y = y; 
	this->z = z; 

	// call something from models.cpp here that will make display lists

	return; 
}

void CBat::draw( ){ 
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix( );

	glLoadIdentity( );
	glTranslatef( x, y, z );
	
	// call something from models.cpp here that will draw display lists

	glMatrixMode( GL_MODELVIEW );
	glPopMatrix( );

	return;
}

