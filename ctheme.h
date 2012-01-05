#ifndef CTHEME_H
#define CTHEME_H

#include <string>

using namespace std;

class CTheme {
	public:
	CTheme *next; 		/* linked list */

	string themeName;
	string boardImage;
	string scorePanelImage;
	string fontImage;
	string backgroundMusic;
	string tiktikSound;

	float boardColor[3];
	float bat1Color[3];
	float bat2Color[3];
	float puckColor[3];
	float backgroundColor[3];

	CTheme( );
	static int readThemesFromConf( );

};



#endif

