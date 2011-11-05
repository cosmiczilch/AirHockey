#ifndef GAME_RUNNING_H
#define GAME_RUNNING_H

#include "cgame.h"
#include "main.h"
#include "ccamera.h"
#include "ctexture.h"
#include "cpanel.h"
#include "cpanelobjek.h"
#include "clabel.h"
#include "cbutton.h" 
#include "cpacketdata.h"
#include "cnetwork_queue.h"


namespace NSGame_Running{ 

/* File Variables     : **********************************************************************************************************************/ 

bool entered = false;

int work_thread_anim_delay_msecs = 40.0; 

GLuint filter;						// Which Filter To Use
GLuint fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR };	// Storage For Three Types Of Fog
GLuint fogfilter= 1;					// Which Fog To Use
GLfloat fogColor[4]= {0.7f, 0.7f, 0.7f, 1.0f};		// Fog Color

int numTimes_for_accum_buffer = 1; 

#define SOMEWHERE_FAR_AWAY 20000
#define SMALL_EPSILON 10.0

/* UI items */
#define SCORE_PANEL 0
#define NUM_PANELS_RUNNING 1
CPanel panels[NUM_PANELS_RUNNING];

#define SCORE_LABEL 0
#define NUM_LABELS_RUNNING 1
CLabel labels[NUM_LABELS_RUNNING];
/* Done UI items */

#define MOUSE_SENSITIVITY 1/7.0

/*
 * the lesser the difference between MAX_BAT_SPEED and
 * MAX_BAT_SPEED_SUSP, the more pronounced the effects
 * of friction shall be. See compute_velocity()
 */
#define MAX_BAT_SPEED 7.5	// empirical
#define MAX_BAT_SPEED_SUSP 1.0	// empirical
#define MAX_PUCK_SPEED_SUSP 3.0	// empirical

/**
 * needed for handle_collission__puck_with_bat(), but declaring here
 * to save time there
 */
vec2 unit_normal;
vec2 unit_tangent;
float puck_normal, puck_tangent;
float bat_normal, bat_tangent;
float puck_normal_dash;	// post collission
float bat_normal_dash;	// post collission
vec2 puck_final_normal , bat_final_normal ;
vec2 puck_final_tangent, bat_final_tangent;

bool player1_collission_handled = false;
bool detect_collission__puck_with_bat( );

/**********************************************************************************************************************************************/ 



SDL_Thread *work_thread = NULL;

void entryFunction ( ) {
	entered = true;

	SDL_ShowCursor( 0 );
	// SDL_WM_GrabInput( SDL_GRAB_ON );


	return;
}

void exitFunction ( ) {
	entered = false;

	SDL_ShowCursor( 1 );
	SDL_WM_GrabInput( SDL_GRAB_OFF );

	return;
}

void initCamera( ){ 
	float w = get_GW( ); 
	float h = get_GH( ); 

	game_Running.camera1 = new CCamera( );
	game_Running.camera1->init( CAMERA_TYPE_PERSP, 0.0, -board.length * CAMERA_HOW_MUCH_BEHIND_BOARD * 1.2, board.length * CAMERA_HOW_MUCH_ABOVE_BOARD / 1.6, 1.0, 80.0 );
	game_Running.camera1->setEye( 0.0, 0.0, -board.length * CAMERA_HOW_MUCH_ABOVE_BOARD );

	game_Running.camera2 = new CCamera( );
	game_Running.camera2->init( CAMERA_TYPE_ORTHO, 0.0, 0.0, SOMEWHERE_FAR_AWAY+SMALL_EPSILON,  w, h, 100.0 );  
	game_Running.camera2->setUpVector( 0.0, 1.0, 0.0 );

	return;
}


void renderScene( ){ 
	if (!entered) {
		entryFunction( );
	}

	glClearColor( 1.0, 1.0, 1.0, 1.0 );
	glClearAccum( 0.0, 0.0, 0.0, 0.0 );
	glClear(GL_ACCUM_BUFFER_BIT);

	for( int i=0; i<numTimes_for_accum_buffer; i++ ){ 
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 

		game_Running.camera1->writeLookAt( true );

		glMatrixMode( GL_MODELVIEW ); 
		glLoadIdentity( );

		board.draw( );
		player1.bat.draw( );
		player2.bat.draw( );
		puck.draw( );

		glAccum(GL_ACCUM, 1.0/numTimes_for_accum_buffer);
	}
	glAccum(GL_RETURN, 1.0);

	/* Draw UI items */
	game_Running.camera2->writeLookAt( true );

	for (int i=0; i<NUM_PANELS_RUNNING; i++) {
		panels[i].draw( );
	}

	for (int i=0; i<NUM_LABELS_RUNNING; i++) {
		labels[i].draw( );
	}
	/* Done drawing UI items */

	SDL_GL_SwapBuffers( );

	return;
};

void eventHandler( SDL_Event &event ){
	if( event.type == SDL_QUIT ){ 
		exit(0); 
	}
	else if( event.type == SDL_KEYDOWN ){ 
		if( event.key.keysym.sym == SDLK_ESCAPE ){ 
			exit(0);
			return;
		}
		if( event.key.keysym.sym == SDLK_UP ){ 
			// do something

		}
		if( event.key.keysym.sym == SDLK_DOWN ){ 
			// do something
			
		}
	} 
	else if( event.type == SDL_MOUSEMOTION ){ 
		/*
		 * increment the shadow cordinates of player1's bat
		 * to the new x_mouse, y_mouse.
		 * setting the velocity correctly for the bat's actual
		 * cordinates to catch up to the shadow cordinates is
		 * done in work()
		 * passing false to translate_X/Y( ) below updates the
		 * shadow cordinates instead of actual cordinates
		 */
		float delta_x = event.motion.xrel;
		float delta_y = -1.0 * event.motion.yrel;
		player1.bat.translate_X( delta_x*MOUSE_SENSITIVITY, false );
		player1.bat.translate_Y( delta_y*MOUSE_SENSITIVITY, player1.player_ID, false );

		// make sure bat cannot move "over" puck

		// detect collission of puck w/ bats and handle appropriately

	}

	return; 
}

void clamp_angle( float *angle ) {
	if (*angle >= 360.0) {
		*angle -= 360.0;
	}

	return;
}

void clamp_velocity( SMotion *motion, float max_speed ) {
	if (motion->velocity.Length() > max_speed) {
		motion->velocity.Normalize();
		motion->velocity /= max_speed;
	}

	return;
}

void handle_collission__puck_with_bat( CPuck *puck, CBat *bat ) {
	/**
	 * Find the unit normal and unit tangent vectors, wr.to
	 * the axis of collission
	 */
	unit_normal = puck->motion.velocity - bat->motion.velocity;
	unit_normal.Normalize();

	unit_tangent[VX] = -1.0 * unit_normal[VY];
	unit_tangent[VY] = unit_normal[VX];

	/**
	 * Resolve the velocities of puck and bat into normal and
	 * tangential components (wr.to the axis of collission)
	 * by projecting the vectors onto the unit_normal and the
	 * unit_tangent vectors calculated above
	 */
	puck_normal  = unit_normal  * puck->motion.velocity;	// dot product
	puck_tangent = unit_tangent * puck->motion.velocity;	// dot product
	bat_normal   = unit_normal  * bat->motion.velocity;	// dot product
	bat_tangent  = unit_tangent * bat->motion.velocity;	// dot product

	/* The tangential components of the velocities do not change
	 * in 2d collission. 
	 * Now compute the normal components after
	 * collission by treating it as simple 1d collission
	 */
	puck_normal_dash = (puck_normal * (PUCK_MASS - BAT_MASS) + 2*BAT_MASS*bat_normal) /\
			   (PUCK_MASS + BAT_MASS);
	bat_normal_dash  = (bat_normal * (BAT_MASS - PUCK_MASS) + 2*PUCK_MASS*puck_normal) /\
			   (PUCK_MASS + BAT_MASS);

	/**
	 * Now, we have the normal and tangential components of the
	 * final (reqd) velocity vectors after collission.
	 * Put them together into 2 vectors, normal and tangential
	 */
	puck_final_tangent = unit_tangent * puck_tangent;
	bat_final_tangent  = unit_tangent * bat_tangent ;
	puck_final_normal  = unit_normal  * puck_normal_dash;
	bat_final_normal   = unit_normal  * bat_normal_dash ;

	/**
	 * Done. Now find the final vectors by adding the normal and
	 * tangential vectors
	 */
	puck->motion.velocity = puck_final_normal + puck_final_tangent;
	bat->motion.velocity  = bat_final_normal  + bat_final_tangent ;

	clamp_velocity( &puck->motion, MAX_PUCK_SPEED_SUSP );

	return;
}

// returns 1 if collission was detected and handled
bool detect_collission__puck_with_bat( ) {
	if (distance_between_2_points( puck.x, puck.y, \
				       player1.bat.x, player1.bat.y ) <= \
	    (BAT_RADIUS + PUCK_RADIUS)*BUFFER_FACTOR_FOR_COLLISSION_DETECTION ) {

		handle_collission__puck_with_bat( &puck, &player1.bat );

	}

	if (distance_between_2_points( puck.x, puck.y, \
				       player2.bat.x, player2.bat.y ) <= \
	    (BAT_RADIUS + PUCK_RADIUS)*BUFFER_FACTOR_FOR_COLLISSION_DETECTION ) {

		handle_collission__puck_with_bat( &puck, &player2.bat );

	}

	return false;
}

void collission_detection( ) {
	// puck w/ wall
	if ( (puck.x+PUCK_RADIUS) >= BOARD_WIDTH/2.0 ||
	     (puck.x-PUCK_RADIUS) <= -BOARD_WIDTH/2.0 ) {
	     // flip_puck_motion_vertically
	     puck.motion.velocity[VX] *= -1.0;
	}
	if ( (puck.y+PUCK_RADIUS) >= BOARD_LENGTH/2.0 ||
	     (puck.y-PUCK_RADIUS) <= -BOARD_LENGTH/2.0 ) {
	     // flip_puck_motion_horizontally
	     puck.motion.velocity[VY] *= -1.0;
	}

	// puck w/ bats
	detect_collission__puck_with_bat( );	// and handle appropriately too

	return;
}

void compute_velocity( ) {
	float vx, vy;
	float delta_x, delta_y;
	/*
	 * delta_x/y is the seperation between the actual and shadow
	 * cordinates of player1's bat, i.e., it is the distance the
	 * actual cordinates have to catch up by
	 */
	delta_x = (player1.bat.sx - player1.bat.x);
	delta_y = (player1.bat.sy - player1.bat.y);
	float delta_max = sqrt( BOARD_WIDTH*BOARD_WIDTH + BOARD_LENGTH/2.0*BOARD_LENGTH/2.0 );
	/*
	 * the idea is that bat's velocity should be proportional to
	 * the amount by which the actual cordinates have to catch up,
	 * i.e., delta_x/y . the velocity should be maximum when delta
	 * is delta_max, and proportional to delta otherwise
	 */
	vx = delta_x * MAX_BAT_SPEED / delta_max;
	vy = delta_y * MAX_BAT_SPEED / delta_max;

	player1.bat.motion.velocity[VX] = vx;
	player1.bat.motion.velocity[VY] = vy;

	clamp_velocity( &player1.bat.motion, MAX_BAT_SPEED_SUSP );

	return;
}

int work( void * ){
	static long int ticks = 0;

	while( 1 ){
		SDL_Delay( work_thread_anim_delay_msecs ); 
		if (gameState != RUNNING) {
			continue;
		}

		if (gameType == MULTI_PLAYER) {
			/*
			 * we are in a MULTI_PLAYER game, 
			 * constantly send coordinates data
			 */
			if (ticks%3 == 0) {
				/*
				 * Don't want to inundate the network w/ too many packets.
				 * Send 1 packet every 3*work_thread_anim_delay_msecs
				 */
				CPacketData pdata;
				pdata.header = PLAYER1_COORD_PACKET_HEADER;
				pdata.ack_packet = false;
				pdata.cordinates.x = player1.bat.x;
				pdata.cordinates.y = player1.bat.y;
				pdata.cordinates.z = player1.bat.z;

				if (are_we_the_server) {
					/*
					 * we are the server; we are responsible for doing collission detection
					 * and sending the position of the puck as well
					 */
					pdata.header = PUCK_AND_PLAYER1_COORD_PACKET_HEADER;
					pdata.ack_packet = false;
					pdata.puck_cordinates.x = puck.x;
					pdata.puck_cordinates.y = puck.y;
					pdata.puck_cordinates.z = puck.z;
				}


				network_queue.insert( pdata );
			}
		}

		// animation
		if (gameType == SINGLE_PLAYER ||
		   (gameType == MULTI_PLAYER && are_we_the_server)) {
			/*
			 * the velocity of player1's bat should be proportional to the
			 * distance between itself and the shadow cordinates (elastic
			 * band). That way, it'll move faster when it has to cover more
			 * ground to catch up, but will slow down before it comes to
			 * rest.
			 * The current (x, y) of the leading edge of the elastic was set
			 * as shadow cordinates in the bat in eventHandler()
			 */
			compute_velocity( );
			printf( "\nvelocity : %f %f", player1.bat.motion.velocity[VX], \
						      player1.bat.motion.velocity[VY] );

			/*
			 * Now, apply the computed velocity to player1's bat
			 */
			player1.bat.translate_X( player1.bat.motion.velocity[VX] );
			player1.bat.translate_Y( player1.bat.motion.velocity[VY], player1.player_ID );

			puck.translate( );

			/* compute bat1's motion */
			/* compute bat2's motion */


			/* save bat1's old_x, old_y and old_z */
			/* save bat2's old_x, old_y and old_z */

		}

		// collission detection : PLACEHOLDER
		collission_detection( );


		// do something
		ticks++;
	}

	return 0; 
}

void init_UI_items( ) {
	float w = get_GW( ); 
	float h = get_GH( ); 

	panels[SCORE_PANEL].init( (int)SCORE_PANEL, w*90/100.0, h*10/100,  0.0, -h*40/100.0, 2*SMALL_EPSILON, \
	"./resources/images/panel.png" );
	panels[SCORE_PANEL].visible = true;
	panels[SCORE_PANEL].enabled = true;

	return;
}


void init( ){ 
	init_UI_items( );
	game_Running.init( );
	game_Running.setGameState( RUNNING );
	NSGame_Running::initCamera( );
	game_Running.setRenderScene( NSGame_Running::renderScene ); 
	game_Running.setEventHandler( NSGame_Running::eventHandler );

	work_thread = SDL_CreateThread( work, NULL );

	return; 
}

void cleanup( ){ 
	SDL_KillThread( work_thread ); 

	return; 
}

};

#endif // GAME_RUNNING_H

