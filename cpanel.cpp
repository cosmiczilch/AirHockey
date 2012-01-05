#include "cpanel.h"

void CPanel::init( int Id, float Width, float Height,  float X, float Y, float Z,  string textureFileName ) {
	id = Id;
	width = Width; 	height = Height;
	x = X; 	y = Y;	z = Z;

	texture.makeTexture( textureFileName, PNG );

	visible = false;	enabled = false;

	return;
}

void CPanel::addPanelObjek( CPanelObjek *panelObjek ) {
	panelObjeks.push_back( (CPanelObjek *) panelObjek );

	return;
}

void CPanel::removePanelObjek( CPanelObjek *panelObjek ) {
	// doesn't work yet

	return;
}

void CPanel::draw( ) {
	if ( !enabled || !visible ) {
		return;
	}

	glDisable( GL_LIGHTING );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix( );
	glLoadIdentity( );

	glTranslatef( x, y, z );

	// draw the background image of the panel
	texture.bindTexture( );
	glBegin( GL_QUADS );
	glTexCoord2f( 0.0, 1.0 );	glVertex3f( -width/2.0, -height/2.0, 0.0 );
	glTexCoord2f( 1.0, 1.0 );	glVertex3f( width/2.0, -height/2.0, 0.0 );
	glTexCoord2f( 1.0, 0.0 );	glVertex3f( width/2.0, height/2.0, 0.0 );
	glTexCoord2f( 0.0, 0.0 );	glVertex3f( -width/2.0, height/2.0, 0.0 );
	glEnd( );
	texture.unbindTexture( );

	// draw the panel objeks
	for ( unsigned int i=0; i<panelObjeks.size( ); i++ ) {
		panelObjeks[i]->draw( );
	}

	glMatrixMode( GL_MODELVIEW );
	glPopMatrix( );
	
	glEnable( GL_LIGHTING );

	return;
}

void CPanel::enableChildren( ) {	// !!!! Doesn't work yet
	for ( unsigned int i=0; i<panelObjeks.size( ); i++ ) {
		panelObjeks[i]->visible = true;
		panelObjeks[i]->enabled = true;
	}

	return;
}

void CPanel::disableChildren( ) {	// !!!! Doesn't work yet
	for ( unsigned int i=0; i<panelObjeks.size( ); i++ ) {
		panelObjeks[i]->visible = false;
		panelObjeks[i]->enabled = false;
	}

	return;
}

bool CPanel::pointLiesWithin( float px, float py, float tolerance ){ 
	if (px >= (x-width/2.0 -tolerance) && px <= (x+width/2.0+ tolerance) && \
	    py >= (y-height/2.0-tolerance) && py <= (y+height/2.0+tolerance)) {
		return true; 
	}
	return false; 
}

void CPanel::clickHandler( float x, float y ) {
	if ( onClick ) {
		onClick( x, y );
	}

	return;
}

