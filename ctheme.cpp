#include "ctheme.h"
#include <cstdlib>

#include <iostream>
#include <fstream>

extern CTheme *theme_list_head;

CTheme::CTheme( ) {
	next = NULL;
}

void CTheme::readThemesFromConf( ) {
	ifstream themeFile( "./resources/themes/themes.conf" );
	string line;
	CTheme *iter = theme_list_head;

	while (themeFile.good( )) {
		getline( themeFile, line );
		if (line[0] == '#') { 	/* coment line, ignore */
			continue;
		}

		/* found one entry for a theme-description */
		printf( "\nReading themes.conf file ..." );
		if (theme_list_head == NULL) {
			theme_list_head = new CTheme( );
			iter = theme_list_head;
		} else {
			iter->next = new CTheme( );
			iter = iter->next;
		}
		iter->themeName = line;

		getline( themeFile, line );
		iter->boardImage = line;

		getline( themeFile, line );
		iter->scorePanelImage = line;

		getline( themeFile, line );
		iter->backgroundMusic = line;

		getline( themeFile, line );
		iter->tiktikSound = line;

		for (int z=0; z<3; z++) {
			getline( themeFile, line );
			iter->boardColor[z] = atof( line.c_str() );
		}
			
		for (int z=0; z<3; z++) {
			getline( themeFile, line );
			iter->bat1Color[z] = atof( line.c_str() );
		}

		for (int z=0; z<3; z++) {
			getline( themeFile, line );
			iter->bat2Color[z] = atof( line.c_str() );
		}

		for (int z=0; z<3; z++) {
			getline( themeFile, line );
			iter->puckColor[z] = atof( line.c_str() );
		}

		iter->next = NULL;

	}

	return;
}

