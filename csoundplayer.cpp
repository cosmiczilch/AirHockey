#include "csoundplayer.h"

void CSoundPlayer::init( ) {
	int audio_rate = 22050;
	Uint16 audio_format = AUDIO_S16; 
	int audio_channels = 2;
	int audio_buffers = 4096;

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

	if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers)) {
		printf("Unable to open audio!\n");
		exit(1);
	}

	soundEffect[SOUND_TIKTIK] = NULL;
	soundEffect[SOUND_GOAL_SCORED] = NULL;
	music = NULL;

	volume = 64;

	return;
}


void CSoundPlayer::loadSoundEffect( int which_soundEffect, string path ) {
	soundEffect[which_soundEffect] = Mix_LoadWAV( path.c_str() );

	return;
}
	
void CSoundPlayer::loadMusic( string path ) {
	music = Mix_LoadMUS( path.c_str() );

	return;
}
	
void CSoundPlayer::haltAllSounds( ) {
	Mix_HaltChannel( -1 );
	Mix_HaltMusic( );
	Mix_FreeMusic( music );

	return;
}

void CSoundPlayer::playMusic( ) {
	Mix_PlayMusic( music, -1 );

	return;
}

void CSoundPlayer::playSoundEffect( int which_soundEffect ) {
	Mix_HaltChannel( -1 );
	Mix_PlayChannel( -1, soundEffect[which_soundEffect], 0 );

	return;
}

void CSoundPlayer::quit( ) {
	Mix_CloseAudio( );

	return;
}

void CSoundPlayer::decrease_volume( ) {
	volume -= 10;
	if (volume < 0) {
		volume = 0;
	}
	Mix_VolumeChunk( soundEffect[SOUND_TIKTIK], volume );
	Mix_VolumeChunk( soundEffect[SOUND_GOAL_SCORED], volume );
	Mix_VolumeMusic( volume );

	return;
}

void CSoundPlayer::increase_volume( ) {
	volume += 10;
	if (volume > 128) {
		volume = 128;
	}

	Mix_VolumeChunk( soundEffect[SOUND_TIKTIK], volume );
	Mix_VolumeChunk( soundEffect[SOUND_GOAL_SCORED], volume );
	Mix_VolumeMusic( volume );

	return;
}

