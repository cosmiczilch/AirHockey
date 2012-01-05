#include "cbat.h"

void CBat::init( float x, float y, float z, float r, float g, float b ){ 
	this->sx = this->x = x; 
	this->sy = this->y = y; 
	this->sz = this->z = z; 

	this->sx *= 3.0;
	this->sy *= 3.0;
	this->sz *= 3.0;

	this->r = r; 
	this->g = g; 
	this->b = b; 

	this->motion.velocity[VX] = this->motion.velocity[VY] = 0.0;

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

// translate the bat along x-axis by amount "deltaX"
void CBat::translate_X( float deltaX, int player_id, bool shadow_cordinates_uh){
	float *px = shadow_cordinates_uh? &x : &sx;

	*px += deltaX;

	/*
	 * The following Hack is to help improve the UI
	 */
	if (shadow_cordinates_uh && player_id == PLAYER_1) {
		return;
	}

	if( (*px+BAT_RADIUS)>BOARD_WIDTH/2.0 )
		*px = BOARD_WIDTH/2.0 - BAT_RADIUS;
	if( (*px-BAT_RADIUS)<-BOARD_WIDTH/2.0 )
		*px = -BOARD_WIDTH/2.0 + BAT_RADIUS;
	
	return;
}
		
// translate the bat along y-axis by amount "deltaY"
void CBat::translate_Y( float deltaY, int player_id, bool shadow_cordinates_uh){
	float *py = shadow_cordinates_uh? &y : &sy;

	*py += deltaY;

	/*
	 * The following Hack is to help improve the UI
	 */
	if (shadow_cordinates_uh && player_id == PLAYER_1) {
		return;
	}

	switch ( player_id ) {
		case PLAYER_1 :
			if ( (*py+BAT_RADIUS)>0 )
				*py = 0.0 - BAT_RADIUS;
			if ( (*py-BAT_RADIUS)<-BOARD_LENGTH/2.0 )
				*py = -BOARD_LENGTH/2.0 + BAT_RADIUS;
			break;

		case PLAYER_2 :
			if ( (*py-BAT_RADIUS)<0 )
				*py = 0.0 + BAT_RADIUS;
			if ( (*py+BAT_RADIUS)>BOARD_LENGTH/2.0 )
				*py = BOARD_LENGTH/2.0 - BAT_RADIUS;
			break;
	}

	return;
}

CBat& CBat::operator = (const CBat& bat2) {
	x = bat2.x; y = bat2.y; z = bat2.z; 
	sx = bat2.sx; sy = bat2.sy; sz = bat2.sz; 

	motion.velocity[VX] = bat2.motion.velocity[VX];
	motion.velocity[VY] = bat2.motion.velocity[VY];

	r = bat2.r; g = bat2.g; b = bat2.b; 

	return *this;
}


