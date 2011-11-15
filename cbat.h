#ifndef CBAT_H
#define CBAT_H

#include "GL/gl.h"
#include "GL/glu.h"
#include "SDL.h"
#define NO_SDL_GLEXT
#include "SDL_opengl.h"
#include "SDL_image.h"

#include "bat_models.h"
#include "cboard.h"
#include "globalDefs.h"
#include "Utilities.h"

class CBat{ 
	public :
	/*
	 * bat's cordinates (actual cordinates)
	 */
	float x, y, z; 
	/*
	 * shadow cordinates, for the leading edge of the elastic band.
	 * the bat's actual cordinates will always try to catch up to
	 * the shadow cordinates w/ a velocity proportional to the 
	 * distance between them
	 */
	float sx, sy, sz;

	SMotion motion;

	float r, g, b;		// bat color

	void init( float x, float y, float z,  float r, float g, float b ); 
	void draw( ); 
	void translate_X( float deltaX, bool shadow_cordinates_uh=true);
	void translate_Y( float deltaY, int player_ID, bool shadow_cordinates_uh=true);

	CBat& operator	= ( const CBat& bat2 );	// assignment of a vec2

};

#endif  //CBAT_H
