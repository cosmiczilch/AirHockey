#include "cbutton.h" 

CButton::CButton( ){ 

	return; 
}

void CButton::init( int Id, float X, float Y, float Z, float Width, float Height, string Path_to_image ){ 
	id = Id; 
	x = X;  y = Y;  z = Z;
	width = Width; 
	height = Height; 
	path_to_image = Path_to_image; 

	texture.makeTexture( path_to_image, PNG );

	hasFocus = false;

	return; 
}

void CButton::draw( ){ 
	float width_temp, height_temp;
	width_temp = width;  height_temp = height;
	if( hasFocus ){
		width_temp+=width_temp*20.0/100.0;
		height_temp+=height_temp*20.0/100.0;
	}
	texture.bindTexture( );
	glBegin( GL_QUADS );
	glTexCoord2f( 0.0, 1.0 );	glVertex3f( x, y, z );
	glTexCoord2f( 1.0, 1.0 );	glVertex3f( x+width_temp, y, z );
	glTexCoord2f( 1.0, 0.0 );	glVertex3f( x+width_temp, y+height_temp, z );
	glTexCoord2f( 0.0, 0.0 );	glVertex3f( x, y+height_temp, z );
	glEnd( );
	texture.unbindTexture( );

	return; 
}

	
bool CButton::pointLiesWithin( float px, float py, float tolerance ){ 
	if( px>=(x-tolerance) && px<=(x+width+tolerance)  &&  py>=(y-tolerance) && py<=(y+height+tolerance) ){ 
		return true; 
	}
	return false; 
}

