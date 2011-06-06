#include "cboard.h"

void CBoard::init( float Width, float Length, float Thickness ){ 
	width=Width; 
	length=Length; 
	thickness=Thickness; 

	loadModel( ); 		// make display display lists out of the board's 3D model

	board_texture.makeTexture( "./resources/images/board.png", PNG );

	return; 
}

void CBoard::draw( ){ 
	glColor3f( 0.3, 0.3, 0.3 );
	drawModel( );

	board_texture.bindTexture( );
	glColor3f( 1.0, 1.0, 1.0 );
	glBegin( GL_QUADS ); 
	glTexCoord2f( 0.0, 0.0 );	 glVertex3f( -width/2.0, -length/2.0, 0.5 ); 
	glTexCoord2f( 1.0, 0.0 );	 glVertex3f( width/2.0, -length/2.0, 0.5 ); 
	glTexCoord2f( 1.0, 1.0 );	 glVertex3f( width/2.0, length/2.0, 0.5 ); 
	glTexCoord2f( 0.0, 1.0 );	 glVertex3f( -width/2.0, length/2.0, 0.5 ); 
	glEnd( );
	board_texture.unbindTexture( );

	return; 
}

