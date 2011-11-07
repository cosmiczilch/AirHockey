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

GLuint filter;
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

#define SCORE_LABEL_P1 0
#define SCORE_LABEL_P2 1
#define NUM_LABELS_RUNNING 2
CLabel labels[NUM_LABELS_RUNNING];
/* Done UI items */

#define MOUSE_SENSITIVITY 1/1.0

/*
 * the lesser the difference between MAX_BAT_SPEED and
 * MAX_BAT_SPEED_SUSP, the more pronounced the effects
 * of friction shall be. See compute_velocity()
 */
#define MAX_BAT_SPEED 5.0			// empirical
#define MAX_BAT_SPEED_SUSP 4.0			// empirical
#define MAX_PUCK_SPEED_SUSP 11.0		// empirical
#define MAX_PUCK_DECELERATION_FACTOR 1.0900 	// empirical
/**
 * used for mimicking friction; however, its more important
 * role is in limiting the puck's speed from becoming too
 * high and causing errors in collission_detection (esp w/
 * bat moving "over" puck)
 * but, we cannot clamp the puck's speed to some fixed number
 * as this will worsen the problem by causing errors in the
 * collission handling calculation
 * so, the idea is to increase the friction experienced by
 * the puck when its velocity is high, thus bringing its
 * velocity to a more managable value
 * its a division factor ranging from 1.0 (no friction)
 * to MAX_PUCK_DECELERATION_FACTOR (max friction)
 * see : adjust_puck_decelaration_and_velocity()
 */
float puck_decelaration_factor;

/**
 * needed for handle_collission__puck_with_bat(), 
 * but declaring here to save time there
 */
vec2 unit_normal;
vec2 unit_tangent;
float puck_normal, puck_tangent;
float bat_normal, bat_tangent;
float puck_normal_dash;	// post collission
float bat_normal_dash;	// post collission
vec2 puck_final_normal , bat_final_normal ;
vec2 puck_final_tangent, bat_final_tangent;

#define PUCK_IS_ON_MY_SIDE (puck.y <= 0)
bool puck_was_on_my_side = false;
bool collission_handled = false;
bool detect_collission__puck_with_bat( );
#define EPSILON_FOR_GOAL_DETECTION 1.003

/**********************************************************************************************************************************************/ 



SDL_Thread *work_thread = NULL;

void entryFunction ( ) {
	entered = true;

	SDL_ShowCursor( 0 );
#if !DEBUG
	SDL_WM_GrabInput( SDL_GRAB_ON );
#endif


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

	static char temp_string[10];
	sprintf( temp_string, "%d", player1.numGoals );
	labels[SCORE_LABEL_P1].setLabelText( temp_string );
	sprintf( temp_string, "%d", player2.numGoals );
	labels[SCORE_LABEL_P2].setLabelText( temp_string );

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
	unit_normal[VX] = puck->x - bat->x;
	unit_normal[VY] = puck->y - bat->y;
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
bool detect_collission__puck_with_bats( ) {
	if (distance_between_2_points( puck.x, puck.y, \
				       player1.bat.x, player1.bat.y ) <= \
	    (BAT_RADIUS + PUCK_RADIUS)*BUFFER_FACTOR_FOR_COLLISSION_DETECTION ) {

		handle_collission__puck_with_bat( &puck, &player1.bat );
		collission_handled = true;

	}

	if (distance_between_2_points( puck.x, puck.y, \
				       player2.bat.x, player2.bat.y ) <= \
	    (BAT_RADIUS + PUCK_RADIUS)*BUFFER_FACTOR_FOR_COLLISSION_DETECTION ) {

		handle_collission__puck_with_bat( &puck, &player2.bat );
		collission_handled = true;

	}

	return false;
}

void pauli() {
	/**
	 * this runs post collission_detection()
	 * check if player1's bat is moving into the puck, and if so,
	 * move the puck a little aside
	 * call this condition(1)
	 * if we are in SINGLE_PLAYER mode, this should be done for
	 * player2's bat as well
	 */
	static vec2 axis;
	static float epsilon;
	static bool cond1_corrected = false, cond2_corrected = false;

	axis[VX] = puck.x - player1.bat.x;
	axis[VY] = puck.y - player1.bat.y;
	if (axis.Length() < (BAT_RADIUS+PUCK_RADIUS)) {
		epsilon = (BAT_RADIUS+PUCK_RADIUS) - axis.Length();
		axis.Normalize();
		puck.x += axis[VX] * epsilon;
		puck.y += axis[VY] * epsilon;
		collission_handled = true;
		// printf( "\npauli1" );
	}

	if (gameType == SINGLE_PLAYER) {
		axis[VX] = puck.x - player2.bat.x;
		axis[VY] = puck.y - player2.bat.y;
		if (axis.Length() < (BAT_RADIUS+PUCK_RADIUS)) {
			epsilon = (BAT_RADIUS+PUCK_RADIUS) - axis.Length();
			axis.Normalize();
			puck.x += axis[VX] * epsilon;
			puck.y += axis[VY] * epsilon;
			collission_handled = true;
			// printf( "\npauli1" );
		}
	}

	/**
	 * now, check for puck moving into the walls and if so,
	 * move the puck a little aside
	 * call this condition(2)
	 */
	if ((puck.x+PUCK_RADIUS) >  BOARD_WIDTH/2.0) {
		puck.x -= abs( (puck.x+PUCK_RADIUS) - BOARD_WIDTH/2.0);
		// printf( "\npauli2" );
		collission_handled = true;
		cond2_corrected = true;
	}
	if ((puck.x-PUCK_RADIUS) < -BOARD_WIDTH/2.0) {
		puck.x += abs( (puck.x-PUCK_RADIUS) - (-BOARD_WIDTH/2.0));
		// printf( "\npauli2" );
		collission_handled = true;
		cond2_corrected = true;
	}
	if ((puck.y+PUCK_RADIUS) > BOARD_LENGTH/2.0) {
		puck.y -= abs( (puck.y+PUCK_RADIUS) - BOARD_LENGTH/2.0);
		// printf( "\npauli2" );
		collission_handled = true;
		cond2_corrected = true;
	}
	if ((puck.y - PUCK_RADIUS) < -BOARD_LENGTH/2.0) {
		puck.y += abs( (puck.y-PUCK_RADIUS) - (-BOARD_LENGTH/2.0));
		// printf( "\npauli2" );
		collission_handled = true;
		cond2_corrected = true;
	}

	/**
	 * now, having corrected condition(2)
	 * we might have - in the process - broken condition(1),
	 * correct it, but this time by moving the bat instead
	 * If we are in SINGLE_PLAYER mode, this should be done for
	 * player2's bat as well
	 */
	if (cond2_corrected) {
		axis[VX] = puck.x - player1.bat.x;
		axis[VY] = puck.y - player1.bat.y;
		if (axis.Length() < (BAT_RADIUS+PUCK_RADIUS)) {
			epsilon = (BAT_RADIUS+PUCK_RADIUS) - axis.Length();
			axis.Normalize();
			player1.bat.x -= axis[VX] * epsilon;
			player1.bat.y -= axis[VY] * epsilon;
			// printf( "\npauli3" );
		}
		if (gameType == SINGLE_PLAYER) {
			axis[VX] = puck.x - player2.bat.x;
			axis[VY] = puck.y - player2.bat.y;
			if (axis.Length() < (BAT_RADIUS+PUCK_RADIUS)) {
				epsilon = (BAT_RADIUS+PUCK_RADIUS) - axis.Length();
				axis.Normalize();
				player2.bat.x -= axis[VX] * epsilon;
				player2.bat.y -= axis[VY] * epsilon;
				// printf( "\npauli3" );
			}
		}
	}

	cond1_corrected = cond2_corrected = false;

	return;
}

void reset_puck_after_goal( ) {
	puck.x = 0.0; puck.y = 0.0;
	puck.motion.velocity[VX] = 0.0;
	puck.motion.velocity[VY] = 0.0;
	return;
}

void collission_detection( ) {
	// check for goals
	if ( (puck.y+PUCK_RADIUS) >=  BOARD_LENGTH/2.0   &&
	     (puck.x-PUCK_RADIUS) >= -GOALPOST_WIDTH/2.0 &&
	     (puck.x+PUCK_RADIUS) <=  GOALPOST_WIDTH/2.0 ) {
	     player1.numGoals++;
	     reset_puck_after_goal( );
	}
	if ( (puck.y-PUCK_RADIUS) <=  -BOARD_LENGTH/2.0   &&
	     (puck.x-PUCK_RADIUS) >= -GOALPOST_WIDTH/2.0 &&
	     (puck.x+PUCK_RADIUS) <=  GOALPOST_WIDTH/2.0 ) {
	     player2.numGoals++;
	     reset_puck_after_goal( );
	}
	     
	// puck w/ walls
	if ( (puck.x+PUCK_RADIUS) >= BOARD_WIDTH/2.0 ||
	     (puck.x-PUCK_RADIUS) <= -BOARD_WIDTH/2.0 ) {
	     // flip_puck_motion_vertically
	     puck.motion.velocity[VX] *= -1.0;
	     collission_handled = true;
	}
	if ( (puck.y+PUCK_RADIUS) >= BOARD_LENGTH/2.0 ||
	     (puck.y-PUCK_RADIUS) <= -BOARD_LENGTH/2.0 ) {
	     // flip_puck_motion_horizontally
	     puck.motion.velocity[VY] *= -1.0;
	     collission_handled = true;
	}

	// puck w/ bats
	detect_collission__puck_with_bats( );	// and handle appropriately too

	/**
	 * in favor of pauli's exclusion principle
	 * make sure that puck cannot move into walls, and that
	 * bats cannot move into puck
	 */
	pauli( );

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

void adjust_puck_decelaration_and_velocity( ) {
	/**
	 * puck_decelaration_factor will lie between 1.0 and
	 * MAX_PUCK_DECELERATION_FACTOR
	 * it is 1.0 when puck's velocity is 0.0, and MAX when
	 * puck's velocity is MAX_PUCK_SPEED_SUSP
	 */
	puck_decelaration_factor  = 1.0;
	puck_decelaration_factor += (MAX_PUCK_DECELERATION_FACTOR - 1.0) * puck.motion.velocity.Length() /\
							MAX_PUCK_SPEED_SUSP;
	
	puck.motion.velocity[VX] /= puck_decelaration_factor;
	puck.motion.velocity[VY] /= puck_decelaration_factor;

	// printf( "\nv, d : %f, %f", puck.motion.velocity.Length(), puck_decelaration_factor);

	return;
}

void scripting_for_player2( ) {
	CBat *me = &player2.bat;

	me->motion.velocity[VY] = 0.0;

	if (puck.motion.velocity[VY] > 0) {	/* puck is moving toward 'my' goalPost 		*/

		/**
		 * find the time the puck would take to arrive (in ticks)
		 * at my 'y' if it were to continue along at its current velocity.
		 * is this overcounting? yes (due to friction)
		 */
		float time_left = 	(me->y - puck.y) /\
					puck.motion.velocity[VY];
		/**
		 * we should try and intercept the puck at the x it would be at
		 * time_left ticks from now; so, calculate the velocity[VX] at
		 * which we'll have to sidle across
		 */
		float delta_x = puck.x - me->x;
		me->motion.velocity[VX] = MAX_PUCK_SPEED_SUSP * delta_x / BOARD_WIDTH;

		/**
		 * if there is too much time_left, better orient myself to
		 * score a goal along w/ just defending my goalPost
		 * this should be a function of AMBITION
		 */
		/**
		 * if there is not too much time_left, i might want to consider
		 * (depending on my AMBITION) moving back a little to better 
		 * align myself to score a goal
		 */
		/**
		 * if i am too far back, i might want to come forward a little
		 * too, towards the middle to better tackle the next attack on
		 * my goalPost, probably do this in "idle" time (when puck is in
		 * player1's court and moving away)
		 */

	} else {
		/**
		 * if the puck is moving away from my goalPost AND its in my court,
		 * calculate the time_left before it leaves my court, and align
		 * myself to try and score a goal
		 * this too, along w/ the force(velocity) w/ which i strike the puck
		 * should be a function of AMBITION
		 */

		me->motion.velocity[VX] = 0.0;
	}


	return;
}

int work( void * ){
	static long int ticks = 0;

	while( 1 ){
		SDL_Delay( work_thread_anim_delay_msecs ); 
		if (gameState != RUNNING) {
			continue;
		}

		/* animation */
		
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
		// printf( "\nvelocity : %f %f", player1.bat.motion.velocity[VX], 
		//				 player1.bat.motion.velocity[VY] );

		/*
		 * Now, apply the computed velocity to player1's bat
		 */
		player1.bat.translate_X( player1.bat.motion.velocity[VX] );
		player1.bat.translate_Y( player1.bat.motion.velocity[VY], player1.player_ID );

		if ( 1 || gameType == SINGLE_PLAYER ||
	      	   (gameType == MULTI_PLAYER && are_we_the_server)) {
		       /*
		        * if in SINGLE_PLAYER mode or are_we_the_server, then we
			* are reponsible for the motion of the puck as well
			*/

			puck.translate( );
			adjust_puck_decelaration_and_velocity( );
		}

		puck_was_on_my_side = PUCK_IS_ON_MY_SIDE;
		/*
		 * IMPORTANT :
		 * collission detection and handling(done inside)
		 */
		collission_detection( );

		if (gameType == SINGLE_PLAYER) {
			/**
			 * Perform some predictive calculations to
			 * move player2's bat
			 */
			scripting_for_player2( );
			/*
			 * Now, apply the computed velocity to player1's bat
			 */
			player2.bat.translate_X( player2.bat.motion.velocity[VX] );
			player2.bat.translate_Y( player2.bat.motion.velocity[VY], player2.player_ID );

		}

		// if in MULTI_PLAYER mode, send coordinates data to remote client
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
				pdata.bat_data = player1.bat;

				if (puck_was_on_my_side && collission_handled) {
					/*
					 * we handled the collission and
					 * we are responsible for sending the position of the puck as well
					 */
					printf( "have done collission handling" );
					pdata.header = PUCK_AND_PLAYER1_COORD_PACKET_HEADER;
					pdata.ack_packet = false;
					pdata.puck_data = puck;
					/* IMPORTANT */
					collission_handled = false;
				}


				network_queue.insert( pdata );
			}
		}

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

	labels[SCORE_LABEL_P1].init( (int)SCORE_LABEL_P1, w*90/100.0, h*10/100,  -w*20/100.0, -h*40/100.0, 2.2*SMALL_EPSILON );
	labels[SCORE_LABEL_P1].visible = true;
	labels[SCORE_LABEL_P1].enabled = true;
	labels[SCORE_LABEL_P1].setLabelText( "0" );
	
	labels[SCORE_LABEL_P2].init( (int)SCORE_LABEL_P1, w*90/100.0, h*10/100,  w*20/100.0, -h*40/100.0, 2.2*SMALL_EPSILON );
	labels[SCORE_LABEL_P2].visible = true;
	labels[SCORE_LABEL_P2].enabled = true;
	labels[SCORE_LABEL_P2].setLabelText( "0" );

	panels[SCORE_PANEL].addPanelObjek( &labels[SCORE_LABEL_P1] );
	panels[SCORE_PANEL].addPanelObjek( &labels[SCORE_LABEL_P2] );

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

