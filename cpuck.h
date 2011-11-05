#ifndef CPUCK_H
#define CPUCK_H

#include "GL/gl.h"
#include "GL/glu.h"
#include "SDL.h"
#define NO_SDL_GLEXT
#include "SDL_opengl.h"
#include "SDL_image.h"
#include "Utilities.h"
#include "globalDefs.h"

#include <cmath>

#include "puck_models.h"

class CPuck{ 
	public :
	float x, y, z; 
	SMotion motion;
	float r, g, b;		// puck color

	void init( float x, float y, float z,  float r, float g, float b ); 
	void draw( ); 
	void translate( );
};

#endif  //CPUCK_H
