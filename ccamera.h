#ifndef CCAMERA_H
#define CCAMERA_H

#include "SDL.h"
#include "SDL_opengl.h"
#define NO_SDL_GLEXT
#include "SDL_image.h"
#include "SDL_thread.h"
#include "GL/gl.h"
#include "GL/glu.h"

#include "Transformation.h"

enum CameraType{ 
	CAMERA_TYPE_PERSP,
	CAMERA_TYPE_ORTHO
};

#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

#define NEG_X_AXIS 3
#define NEG_Y_AXIS 4
#define NEG_Z_AXIS 5

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3

#define ROT_ANGLE 5.0		// degrees
#define ZOOM_ANGLE_INC 2.0 	// degrees


class CCamera
{
private:
    SDL_sem *semaphore; 

    bool redraw;
    
    CameraType cameraType;
    vec3 camera_position;
    float x_eye, y_eye, z_eye;
    vec3 up_vector;
    int vp_x1, vp_x2, vp_y1, vp_y2; 	// viewport 

    // for a perspective gimbal camera
    float radius;
    float angle_of_vision;
    float aspecRatio;

    // for an orthographic camera
    float width; 
    float height; 
    float depth;

    vec3 axle_vector;
    float axle_x, axle_y, axle_z;

public:
    CCamera();

    // for a perspective gimbal camera
    void init( CameraType cameraType, float X, float Y, float Z, float AspecRatio, float Angle_of_vision );
    void setAspecRatio( float AspecRatio );
    void setAngle_of_vision( float Angle_of_vision );
    void setEye( float x, float y, float z );
    void setUpVector( float i, float j, float k );
    void setViewPort( int Vp_x1, int Vp_x2, int Vp_y1, int Vp_y2 );

    // for an orthographic camera
    void init( CameraType cameraType, float X, float Y, float Z, float Width, float Height, float Depth );
    void setWidth( float Width );
    void setHeight( float Height );
    void setDepth( float Depth );

    void setRedrawTrue( );
    void writeLookAt( bool force=false );
    void rotateAboutAxis( int axis, float rot_angle=ROT_ANGLE );
    void sidewind( int direction, float amount );
    void zoomin();
    void zoomout();
};

#endif // CCAMERA_H
