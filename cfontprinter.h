#ifndef CFONTPRINTER_H
#define CFONTPRINTER_H

#include "Utilities.h"
#include "ctexture.h"

#include <string>

using namespace std;

class CFontPrinter {
	public:
	SFont font;
	string fontImageFile;
	CTexture texture;

	public:
	void init( float FontWidth, float FontHeight, SColor ForeColor,  string pathToFontImage );
	void setFont( float FontWidth, float FontHeight, SColor ForeColor );
	void setFont( SFont Font );
	void print( string text, float x, float y, float z );
};
	

#endif


