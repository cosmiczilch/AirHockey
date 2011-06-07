#ifndef CBUTTON_H
#define CBUTTON_H

#include "SDL.h"
#include "SDL_opengl.h"
#define NO_SDL_GLEXT
#include "SDL_image.h"
#include "SDL_thread.h"
#include "GL/gl.h"
#include "GL/glu.h"

#include "ctexture.h"

#include <string> 

using namespace std;


class CButton{ 
	public: 
	int id;
	float x, y, z; 
	float width, height;
	string path_to_image;
	bool enabled;		// don't draw if not enabled
	bool hasFocus;
	CTexture texture;

	public: 
	CButton( );
	void init( int Id, float X, float Y, float Z, float Width, float Height, string Path_to_image );
	bool pointLiesWithin( float px, float py, float tolerance=0 ); 

	void draw( ); 

};

#endif // CBUTTON_H

