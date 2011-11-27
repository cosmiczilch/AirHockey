#ifndef MODELS_H
#define MODELS_H 

#include "SDL.h"
#include "SDL_opengl.h"
#define NO_SDL_GLEXT
#include "SDL_image.h"
#include "SDL_thread.h"
#include "GL/gl.h"
#include "GL/glu.h"


void loadModel();
void drawModel( float r, float g, float b ); 

void load_0();	//object: goalPostF
void load_1();	//object: goalPostN
void load_2();	//object: guardRailR
void load_3();	//object: guardRailL
void load_4();	//object: board


#endif // MODELS_H
