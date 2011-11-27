#ifndef CBOARD_H
#define CBOARD_H 

#include "GL/gl.h"
#include "GL/glu.h"
#include "SDL.h"
#define NO_SDL_GLEXT
#include "SDL_opengl.h"
#include "SDL_image.h"

#include "models.h"
#include "ctexture.h"

#include <string>
using namespace std;

#define BOARD_LENGTH 60
#define BOARD_WIDTH 40
#define BOARD_THICKNESS 5
#define GOALPOST_WIDTH 10

class CBoard{ 
	public: 
	float width, length, thickness; 
	float r, g, b;
	float goalPost_width; 
	CTexture board_texture;

	public: 
	void init( float Width, float Length, float Thickness, float R, float G, float B, string board_texture_image );
	void draw( );

};


#endif // CBOARD_H
