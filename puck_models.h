#ifndef PUCK_MODELS_H
#define PUCK_MODELS_H 

#include "SDL.h"
#include "SDL_opengl.h"
#define NO_SDL_GLEXT
#include "SDL_image.h"
#include "SDL_thread.h"
#include "GL/gl.h"
#include "GL/glu.h"


void loadPuck( float r=0.0, float g=0.0, float b=0.0 );
void drawPuck( ); 

void load_puck( float r=0.0, float g=0.0, float b=0.0 ); //object: puck

#endif // PUCK_MODELS_H
