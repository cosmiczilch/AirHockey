#ifndef CBAT_H
#define CBAT_H

#include "GL/gl.h"
#include "GL/glu.h"
#include "SDL.h"
#define NO_SDL_GLEXT
#include "SDL_opengl.h"
#include "SDL_image.h"


class CBat{ 
	public :
	float x, y, z; 

	void init( float x, float y, float z ); 
	void draw( ); 
};

#endif  //CBAT_H
