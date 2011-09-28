#include "clabel.h"


void CLabel::init( int Id, float Width, float Height,  float X, float Y, float Z ) {
	id = Id;

	width = Width; 	height = Height;
	x = X;	y = Y; 	z = Z;

	font.fontWidth = 20;
	font.fontHeight = 30;
	font.foreColor.r = 0.0;
	font.foreColor.g = 0.0;
	font.foreColor.b = 0.0;
	font.foreColor.a = 1.0;

	hasFocus = false;
	enabled = true;
	visible = true;

	popUpAnim = true;

	return;
}

void CLabel::setLabelText( string LabelText ) {
	labelText = LabelText;

	width = labelText.size( ) * font.fontWidth;
	height = font.fontHeight;

	return;
}
	

void CLabel::draw( ) {
	if ( !enabled || !visible ) {
		return;
	}

	SFont font_temp = font;
	float x_temp=x, y_temp=y;
	if ( hasFocus && popUpAnim ) {
		font_temp.fontWidth+=font_temp.fontWidth*20/100.0;
		font_temp.fontHeight+=font_temp.fontHeight*20/100.0;
		x_temp+=x_temp*20/100.0;
		y_temp+=y_temp*20/100.0;

	}
		
	fontPrinter.setFont( font_temp );
	fontPrinter.print( labelText, x_temp, y_temp, z );

	return;
}

bool CLabel::pointLiesWithin( float px, float py, float tolerance ){ 
	if( px>=(x-tolerance) && px<=(x+width+tolerance)  &&  py>=(y-tolerance) && py<=(y+height+tolerance) ){ 
		return true; 
	}
	return false; 
}

void CLabel::clickHandler( float x, float y ) {
	if ( onClick ) {
		onClick( x, y );
	}

	return;
}

