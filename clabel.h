#ifndef CLABEL_H
#define CLABEL_H

#include "cpanelobjek.h"
#include "cfontprinter.h"
#include "Utilities.h"
#include <string>

using namespace std;

extern CFontPrinter fontPrinter;

class CLabel : public CPanelObjek {
	public :
	string labelText;
	SFont font;
	void (*onClick)( float x, float y );
	bool visible, enabled;
	bool hasFocus;
	bool popUpAnim;

	void init( int Id, float Width, float Height,  float X, float Y, float Z );
	void setLabelText( string LabelText );
	bool pointLiesWithin( float px, float py, float tolerance=0 ); 

	void draw( );
	void clickHandler( float x, float y );

};

#endif

