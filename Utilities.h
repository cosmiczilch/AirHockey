#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>

using namespace std;

struct SColor{
    float r, g, b, a;

    SColor( ){
        r = g = b = a = 0.0;
    }

    SColor( float R, float G, float B, float A ){
        r = R;  g = G;  b = B; a = A;
    }

    void init( float R, float G, float B, float A ){
        r = R;  g = G;  b = B; a = A;
    }
};


struct SCordinates{
    float x, y, z;

    SCordinates( ){
        x = y = z = 0.0;
    }

    SCordinates( float X, float Y, float Z ){
        x = X;  y = Y; z = Z;
    }

    void init( float X, float Y, float Z ){
        x = X;  y = Y; z = Z;
    }
};


struct SFont{
    float fontWidth, fontHeight;
    SColor foreColor;
};



#endif

