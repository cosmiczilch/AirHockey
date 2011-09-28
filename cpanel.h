#ifndef CPANEL_H
#define CPANEL_H

#include "ctexture.h"
#include "cpanelobjek.h"
#include <vector>
#include <string>

using namespace std;

class CPanel {
	public :
	int id;

	float width, height;
	float x, y, z;

	CTexture texture;

	bool visible, enabled;

	vector<CPanelObjek *> panelObjeks;

	void init( int Id, float Width, float Height,  float X, float Y, float Z,  string textureFileName );
	void addPanelObjek( CPanelObjek *panelObjek );
	void removePanelObjek( CPanelObjek *panelObjek );
	void draw( );

	void enableChildren( );
	void disableChildren( );
};

#endif

