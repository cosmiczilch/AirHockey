#ifndef CSOUNDPLAYER_H
#define CSOUNDPLAYER_H

#include "SDL.h"
#include "SDL_mixer.h"
#include <string>

using namespace std;

#define SOUND_TIKTIK 0
#define SOUND_GOAL_SCORED 1

class CSoundPlayer {
	Mix_Chunk *soundEffect[2];
	Mix_Music *music;

	public: 
	void init( );

	void loadSoundEffect( int which_soundEffect, string path );
	void loadMusic( string path );

	void haltAllSounds( );

	void playMusic( );
	void playSoundEffect( int which_soundEffect );

	void quit( );
};

#endif

