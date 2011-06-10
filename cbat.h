#ifndef CBAT_H
#define CBAT_H

#include "GL/gl.h"
#include "GL/glu.h"
#include "SDL.h"
#define NO_SDL_GLEXT
#include "SDL_opengl.h"
#include "SDL_image.h"

#include "bat_models.h"

class CBat{ 
	public :
	float x, y, z; 
	float r, g, b;		// bat color

	void init( float x, float y, float z,  float r, float g, float b ); 
	void draw( ); 
};

#endif  //CBAT_H
