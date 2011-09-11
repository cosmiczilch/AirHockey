#include "cbat.h"

void CBat::init( float x, float y, float z, float r, float g, float b ){ 
	this->x = x; 
	this->y = y; 
	this->z = z; 

	this->r = r; 
	this->g = g; 
	this->b = b; 

	// call something from bat_models.cpp here that will make display lists
	loadBat( );

	return; 
}

void CBat::draw( ){ 
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
	
	// call something from bat_models.cpp here that will draw display lists
	drawBat( );

	glMatrixMode( GL_MODELVIEW );
	glPopMatrix( );

	return;
}

void CBat::translate_X( float X ){
	if( X > 0 ){
		if( x<BOARD_WIDTH/2.0 ){
			x+=X;
		}
	}
	else{
		if( x>-BOARD_WIDTH/2.0){
			x+=X;
		}
	}
	if( x>BOARD_WIDTH/2.0 )
		x = BOARD_WIDTH/2.0;
	if( x<-BOARD_WIDTH/2.0 )
		x = -BOARD_WIDTH/2.0;
	
	return;
}
		
void CBat::translate_Y( float Y ){
	if( Y > 0 ){
		if( y<BOARD_LENGTH/2.0 ){
			y+=Y;
		}
	}
	else{
		if( y>-BOARD_LENGTH/2.0){
			y+=Y;
		}
	}
	if( y>BOARD_LENGTH/2.0 )
		y = BOARD_LENGTH/2.0;
	if( y<-BOARD_LENGTH/2.0 )
		y = -BOARD_LENGTH/2.0;
	
	return;
}

