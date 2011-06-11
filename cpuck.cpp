#include "cpuck.h"

void CPuck::init( float x, float y, float z, float r, float g, float b ){ 
	this->x = x; 
	this->y = y; 
	this->z = z; 

	this->r = r; 
	this->g = g; 
	this->b = b; 

	// call something from puck_models.cpp here that will make display lists
	loadPuck( );

	return; 
}

void CPuck::draw( ){ 
	GLfloat mat_ambient[] = { r, g, b, 1.0 };
	GLfloat mat_diffuse[] = { r, g, b, 1.0 };
	GLfloat mat_specular[] = { r, g, b, 1.0 };
	GLfloat mat_shininess[] = { 120.0 };

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix( );

	glLoadIdentity( );
	glTranslatef( x, y, z );
	
	// call something from puck_models.cpp here that will draw display lists
	drawPuck( );

	glMatrixMode( GL_MODELVIEW );
	glPopMatrix( );

	return;
}

