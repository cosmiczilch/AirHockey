#ifndef THEMES_H
#define THEMES_H

#include "ctheme.h"
#include "csoundplayer.h"

extern CTheme *theme_list_head;
extern CTheme *currentTheme;
extern CSoundPlayer soundPlayer;

void set_current_theme ( int index ) {
	CTheme *iter = theme_list_head;
	int i=0;
	while (i<index) {
		iter = iter->next;
		i++;
	}

	currentTheme = iter;

	soundPlayer.haltAllSounds( );
	soundPlayer.loadMusic( currentTheme->backgroundMusic );
	soundPlayer.loadSoundEffect( SOUND_TIKTIK, currentTheme->tiktikSound );
	soundPlayer.loadSoundEffect( SOUND_GOAL_SCORED, "./resources/sounds/goal.wav" );


	return;
}

#endif

