#ifndef CGAME_H
#define CGAME_H

#include "SDL.h"
#include "SDL_opengl.h"
#define NO_SDL_GLEXT
#include "SDL_image.h"
#include "SDL_thread.h"
#include "GL/gl.h"
#include "GL/glu.h"

#include "Transformation.h"
#include "ccamera.h"

#include <vector> 
using namespace std;

enum GameState{ 
	MAIN_MENU,
	RUNNING,
	PAUSED,
	OVER
};


class CGame
{
    private: 
    SDL_sem *semaphore; 
    static vector <CGame*> list_of_games;

    public:
    GameState gameState;

    void (* renderScene) (void) ; 
    CCamera *camera1, *camera2; 
    void (* eventHandler) (SDL_Event &event) ;

    CGame( );
    void init( );
    void setGameState( GameState gameState ); 
    void setRenderScene( void (* renderScene) (void) ); 
    void callRenderScene( );
    void setEventHandler( void (* eventHandler) (SDL_Event &event) ); 
    void callEventHandler(SDL_Event &event);
    void setCamera1( CCamera *camera1 );
    void setCamera2( CCamera *camera2 );

    static CGame* getGameByGameState( GameState gameState ); 

};

#endif // CGAME_H

