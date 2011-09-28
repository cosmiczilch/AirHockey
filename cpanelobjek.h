#ifndef CPANELOBJEK_H
#define CPANELOBJEK_H

#include "ctexture.h"
#include <vector>
#include <string>

using namespace std;


class CPanelObjek {
	public :
	int id;

	float width, height;
	float x, y, z;

	bool enabled, visible;

	virtual void init( int Id, float Width, float Height,  float X, float Y, float Z ) = 0;
	virtual void draw( ) = 0;
};

#endif

