#include "cfontprinter.h"

void CFontPrinter::init( float FontWidth, float FontHeight, SColor ForeColor,  string pathToFontImage ) {
	font.fontWidth = FontWidth; font.fontHeight = FontHeight;

	font.foreColor.r = ForeColor.r;
	font.foreColor.g = ForeColor.g;
	font.foreColor.b = ForeColor.b;
	font.foreColor.a = ForeColor.a;

	fontImageFile = pathToFontImage;
	texture.makeTexture( fontImageFile, PNG );

	return;
}

void CFontPrinter::setFont( float FontWidth, float FontHeight, SColor ForeColor ) {
	font.fontWidth = FontWidth; font.fontHeight = FontHeight;

	font.foreColor.r = ForeColor.r;
	font.foreColor.g = ForeColor.g;
	font.foreColor.b = ForeColor.b;
	font.foreColor.a = ForeColor.a;

	return;
}

void CFontPrinter::setFont( SFont Font ) {
	font.fontWidth = Font.fontWidth;
	font.fontHeight = Font.fontHeight;
	font.foreColor.r = Font.foreColor.r;
	font.foreColor.g = Font.foreColor.g;
	font.foreColor.b = Font.foreColor.b;
	font.foreColor.a = Font.foreColor.a;

	return;
}

void CFontPrinter::print( string text, float x, float y, float z ) {
	glMatrixMode( GL_MODELVIEW );
        glPushMatrix( );
        glPushAttrib( GL_CURRENT_BIT );

	glLoadIdentity( );
        glTranslatef( x, y, z );

        glScalef( font.fontWidth, font.fontHeight, 1.0 );

        glColor4f( font.foreColor.r, font.foreColor.g, font.foreColor.b, font.foreColor.a );

        glEnable( GL_BLEND );
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

        glEnable( GL_TEXTURE_2D );
        for( unsigned int i=0; i<text.size( ); i++ ){
            char c = text[i];
            if( c<32 || c>127 ){ continue; }
            texture.bindTexture( );
            glBegin( GL_QUADS );
                glTexCoord2f( ( (c-32)%16) * 0.0625   , 1.0 - ( (c-32+16)/16 * 0.0625 ) );   glVertex3f( -0.5, -0.5, 0.0 );
                glTexCoord2f( ( ( (c-32)%16) + 1 ) * 0.0625 , 1.0 - ( (c-32+16)/16 * 0.0625 ) );   glVertex3f( 0.5, -0.5, 0.0 );
                glTexCoord2f( ( ( (c-32)%16) + 1 )* 0.0625 , 1.0 - ( (c-32)/16 * 0.0625 )   );   glVertex3f( 0.5, 0.5, 0.0 );
                glTexCoord2f( ( (c-32)%16) * 0.0625   , 1.0 - ( (c-32)/16 * 0.0625 )   );   glVertex3f( -0.5, 0.5, 0.0 );
            glEnd( );
	    texture.unbindTexture( );
            glTranslatef( 1.0, 0.0, 0.0 );
        }
        glDisable( GL_TEXTURE_2D );

        glEnable( GL_BLEND );
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);


        glPopAttrib( );
        glPopMatrix( );

        return;
}


