#ifndef CTEXTINPUT_H
#define CTEXTINPUT_H

#include "SDL.h"
#include "SDL_opengl.h"
#define NO_SDL_GLEXT
#include "SDL_image.h"
#include "SDL_thread.h"
#include "GL/gl.h"
#include "GL/glu.h"

#include "ctexture.h"
#include "cpanelobjek.h"
#include "Utilities.h"
#include "cfontprinter.h"

#include <string> 

using namespace std;

class CTextInput : public CPanelObjek {
	public:

	SFont font;
	string inputText;

	bool visible;
	bool enabled;

	void init( int Id, float Width, float Height,  float X, float Y, float Z );
	void setFont( SFont Font );
	void draw( );

	void (*onInputDone)( );
	void keyPressHandler( SDLKey key );
	void inputDoneHandler( );
};

extern CFontPrinter fontPrinter;

#endif

