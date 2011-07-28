#include "ccamera.h"

CCamera::CCamera()
{
}


void CCamera::init( CameraType cameraType, float X, float Y, float Z, float AspecRatio, float Angle_of_vision ){
    semaphore = SDL_CreateSemaphore( 1 );

    SDL_SemWait( semaphore ); 

    this->cameraType = cameraType; 

    camera_position[0] = X; camera_position[1] = Y; camera_position[2] = Z;
    radius = sqrt( camera_position[0] * camera_position[0] + camera_position[1] * camera_position[1] + camera_position[2] * camera_position[2] );

    x_eye=0.0;  y_eye=0.0;  z_eye=0.0;
    up_vector[0] = 0.0; up_vector[1] = 0.0; up_vector[2] = 1.0;
    axle_vector[0] = 1.0;  axle_vector[1] = 0.0; axle_vector[2] = 0.0;

    const SDL_VideoInfo* vidInfo = SDL_GetVideoInfo( ); 
    float GW = vidInfo->current_w; 
    float GH = vidInfo->current_h; 
    vp_x1 = 0.0; 	vp_x2 = GW;
    vp_y1 = 0.0; 	vp_y2 = GH;

    angle_of_vision = Angle_of_vision;
    this->aspecRatio=AspecRatio;

    redraw = true;

    SDL_SemPost( semaphore ); 

    return;
}

void CCamera::init( CameraType cameraType, float X, float Y, float Z, float Width, float Height, float Depth ){ 
    semaphore = SDL_CreateSemaphore( 1 );

    SDL_SemWait( semaphore ); 

    this->cameraType = cameraType; 

    camera_position[0] = X; camera_position[1] = Y; camera_position[2] = Z;

    x_eye=0.0;  y_eye=0.0;  z_eye=0.0;
    up_vector[0] = 0.0; up_vector[1] = 0.0; up_vector[2] = 1.0;
    axle_vector[0] = 1.0;  axle_vector[1] = 0.0; axle_vector[2] = 0.0;

    const SDL_VideoInfo* vidInfo = SDL_GetVideoInfo( ); 
    float GW = vidInfo->current_w; 
    float GH = vidInfo->current_h; 
    vp_x1 = 0.0; 	vp_x2 = GW;
    vp_y1 = 0.0; 	vp_y2 = GH;

    width = Width; 
    height = Height; 
    depth = Depth; 

    redraw = true; 

    SDL_SemPost( semaphore ); 

    return;
}

void CCamera::setAspecRatio( float AspecRatio ){ 
	SDL_SemWait( semaphore ); 

	aspecRatio = AspecRatio; 

	SDL_SemPost( semaphore ); 
	
	return; 
}

void CCamera::setEye( float x, float y, float z ){ 
	// Nah!!!!!!!! Don't use yet. Doesn't work yet. Need to fix up_vector also. 

	SDL_SemWait( semaphore ); 

	x_eye = x;
	y_eye = y;
	z_eye = z;

	SDL_SemPost( semaphore ); 

	return;
}

void CCamera::setAngle_of_vision( float Angle_of_vision ){
	SDL_SemWait( semaphore ); 

	angle_of_vision = Angle_of_vision; 
	redraw = true;

	SDL_SemPost( semaphore ); 

	return; 
}

void CCamera::setUpVector( float i, float j, float k ){ 
	SDL_SemWait( semaphore ); 

	up_vector[0]=i; 
	up_vector[1]=j; 
	up_vector[2]=k; 
	redraw = true;

	SDL_SemPost( semaphore ); 

	return; 
}

void CCamera::setViewPort( int Vp_x1, int Vp_y1, int Vp_x2, int Vp_y2 ){
	SDL_SemWait( semaphore ); 

	vp_x1 = Vp_x1;
	vp_y1 = Vp_y1;
	vp_x2 = Vp_x2;
	vp_y2 = Vp_y2;
	redraw = true;

	SDL_SemPost( semaphore ); 

	return; 
}
	

void CCamera::setWidth( float Width ){ 
	SDL_SemWait( semaphore ); 

	width = Width; 
	redraw = true;

	SDL_SemPost( semaphore ); 

	return; 
}

void CCamera::setHeight( float Height ){ 
	SDL_SemWait( semaphore ); 

	height = Height; 
	redraw = true;

	SDL_SemPost( semaphore ); 

	return; 
}

void CCamera::setDepth( float Depth ){ 
	SDL_SemWait( semaphore ); 

	depth = Depth; 
	redraw = true;

	SDL_SemPost( semaphore ); 

	return; 
}

void CCamera::setRedrawTrue( ){
	SDL_SemWait( semaphore ); 

	redraw=true;

	SDL_SemPost( semaphore ); 

	return; 
}

void CCamera::writeLookAt( bool force ){
    SDL_SemWait( semaphore ); 

    if( !redraw && !force ){ 
        SDL_SemPost( semaphore ); 
        return; 
    }

    const SDL_VideoInfo* vidInfo = SDL_GetVideoInfo( ); 
    float GW = vidInfo->current_w; 
    float GH = vidInfo->current_h; 
    if( GH==0 ){ GH=1; }
    aspecRatio = (float)GW/(float)GH;

    glViewport( vp_x1, vp_y1, vp_x2, vp_y2 );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    if( cameraType == CAMERA_TYPE_PERSP ){ 

	    gluPerspective( angle_of_vision, aspecRatio, 1.0, 8888.88 );
	    gluLookAt( camera_position[0], camera_position[1], camera_position[2], x_eye, y_eye, z_eye,  up_vector[0], up_vector[1], up_vector[2] );

    }
    else if( cameraType == CAMERA_TYPE_ORTHO ){ 
    	    glOrtho( camera_position[0]-width/2.0, camera_position[0]+width/2.0,  camera_position[1]-height/2.0, camera_position[1]+height/2.0,  camera_position[2]-depth/2.0, camera_position[2]+depth/2.0 ); 
	    gluLookAt( camera_position[0], camera_position[1], camera_position[2], x_eye, y_eye, z_eye,  up_vector[0], up_vector[1], up_vector[2] );

    }

    glMatrixMode( GL_MODELVIEW );

    redraw = false;

    SDL_SemPost( semaphore ); 

    return;
}

void CCamera::rotateAboutAxis( int axis, float rot_angle ){
    SDL_SemWait( semaphore ); 

    vec3 axis_of_rotation;
    mat3 rotation_matrix;

    switch( axis ){
    case Z_AXIS :
        axis_of_rotation = vec3::vec3(up_vector);
        rotation_matrix = mat3::Rotation3DDeg( axis_of_rotation, rot_angle );
        camera_position = rotation_matrix * camera_position;
        up_vector = rotation_matrix * up_vector;
        axle_vector = rotation_matrix * axle_vector;

        break;

    case NEG_Z_AXIS :
        axis_of_rotation = vec3::vec3(up_vector);
        rotation_matrix = mat3::Rotation3DDeg( axis_of_rotation, -rot_angle );
        camera_position = rotation_matrix * camera_position;
        up_vector = rotation_matrix * up_vector;
        axle_vector = rotation_matrix * axle_vector;

        break;

    case X_AXIS :
        axis_of_rotation = vec3::vec3(axle_vector);
        rotation_matrix = mat3::Rotation3DDeg( axis_of_rotation, -rot_angle );
        camera_position = rotation_matrix * camera_position;
        up_vector = rotation_matrix * up_vector;
        axle_vector = rotation_matrix * axle_vector;

        break;

    case NEG_X_AXIS :
        axis_of_rotation = vec3::vec3(axle_vector);
        rotation_matrix = mat3::Rotation3DDeg( axis_of_rotation, rot_angle );
        camera_position = rotation_matrix * camera_position;
        up_vector = rotation_matrix * up_vector;
        axle_vector = rotation_matrix * axle_vector;

        break;

    }

    redraw = true;

    SDL_SemPost( semaphore ); 

    return;
}

void CCamera::sidewind( int direction, float amount ) {
    SDL_SemWait( semaphore ); 

    switch( direction ) {
    	case UP :
    	case DOWN :
    	case LEFT :
    	case RIGHT :
		break;
    }
	
    redraw = true;
    SDL_SemPost( semaphore ); 

    return;
}


void CCamera::zoomin(){
    SDL_SemWait( semaphore ); 

    angle_of_vision-=ZOOM_ANGLE_INC;
    redraw = true;

    SDL_SemPost( semaphore ); 

    return;
}

void CCamera::zoomout(){
    SDL_SemWait( semaphore ); 

    angle_of_vision+=ZOOM_ANGLE_INC;
    redraw = true;

    SDL_SemPost( semaphore ); 

    return;
}

