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

/* !!!!!!!!! See also entryFunction() */
int work_thread_anim_delay_msecs = 10; 
long int ticks = 0;
/* !!!!!!!!! See also entryFunction() */

GLuint filter;
GLuint fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR };	// Storage For Three Types Of Fog
GLuint fogfilter= 1;					// Which Fog To Use
GLfloat fogColor[4]= {0.7f, 0.7f, 0.7f, 1.0f};		// Fog Color

#if ANTI_ALIASING
int numTimes_for_accum_buffer = 5; 			// Should be 1, or a multiple of 5
#else
int numTimes_for_accum_buffer = 1; 			// Should be 1, or a multiple of 5
#endif
float jitter_amount = 0.05;

#define SOMEWHERE_FAR_AWAY 20000
#define SMALL_EPSILON 10.0

/* UI items */
#define SCORE_PANEL 0
#define NUM_PANELS_RUNNING 1
CPanel panels[NUM_PANELS_RUNNING];

#define SCORE_LABEL_P1 0
#define SCORE_LABEL_P2 1
#define TIMER_LABEL 2
#define NUM_LABELS_RUNNING 3
CLabel labels[NUM_LABELS_RUNNING];
/* Done UI items */

/*
 * the lesser the difference between MAX_BAT_SPEED and
 * MAX_BAT_SPEED_SUSP, the more pronounced the effects
 * of friction shall be. See compute_velocity()
 */
#define MAX_BAT_SPEED (float)(work_thread_anim_delay_msecs/6.0)			// empirical: 5.0
#define MAX_BAT_SPEED_SUSP (float)(work_thread_anim_delay_msecs/8.0)			// empirical: 4.0
#define MAX_PUCK_SPEED_SUSP (work_thread_anim_delay_msecs/4.0)		// empirical: 10.0
#define MAX_PUCK_DECELERATION_FACTOR 1.0500 	// empirical
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
bool collission_handled = false;	/* used for multiplayer games */
bool collission_handled2 = false;	/* used for soundEffects */
bool detect_collission__puck_with_bat( );
#define EPSILON_FOR_GOAL_DETECTION 1.003

/**
 * scripting moves for player2
 */
enum Player2_Plans {
	DO_NOTHING,		/* nothing */
	SIMPLE_INTERCEPT,	/* just get in the way of the puck */
	AMBITIOUS_INTERCEPT,	/* rush forward and get in the way of the puck */
	SWING_BACK, 		/* take a step back and make room (if the puck is too close) */
	GET_BEHIND_PUCK,
	COME_TO_MIDDLE_OF_BOARD,/* use idle time to better position yourself */
	PASS_THE_PUCK, 		/* if puck is in my side, just send it across */
	AMBITIOUS_PASS_THE_PUCK,/* if puck is in my side, score a goal */
	NUM_PLANS
};
struct Planning {
	Player2_Plans plan_type;
} player2_planning;
#define AGGRESSION 100
#define AGGRESSION_MAX 100

int timer_secs, timer_minutes;

void copy_bat1_to_packet( CPacketData* pdata );
void copy_puck_to_packet( CPacketData* pdata );

/**********************************************************************************************************************************************/ 



SDL_Thread *work_thread = NULL;

void entryFunction ( ) {
	entered = true;

	PLAYER2_THINK_AHEAD_TICKS = (PLAYER2_THINK_AHEAD_TIME/work_thread_anim_delay_msecs);

	soundPlayer.playSoundEffect( SOUND_GOAL_SCORED );

	SDL_ShowCursor( 0 );
#if !DEBUG
	SDL_WM_GrabInput( SDL_GRAB_ON );
#endif

	work_thread_anim_delay_msecs = 10.0;
	ticks = 0;

	timer_secs = 0;
	timer_minutes = 0;
	player1.numGoals = 0;
	player2.numGoals = 0;

	return;
}

void exitFunction ( ) {
	entered = false;

	SDL_ShowCursor( 1 );
	SDL_WM_GrabInput( SDL_GRAB_OFF );

	gameState = OVER;

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

void jitter_the_camera( int i ) {
	switch (i) {
		case 0 :
			// do nothing;
			break;

		case 1 :
			game_Running.camera1->sidewind( UP, jitter_amount );
			break;
		case 2 :
			game_Running.camera1->sidewind( DOWN, jitter_amount );
			break;
		case 3 :
			game_Running.camera1->sidewind( LEFT, jitter_amount );
			break;
		case 4 :
			game_Running.camera1->sidewind( RIGHT, jitter_amount );
			break;
	}

	return;
}

void unjitter_the_camera( int i ) {
	switch (i) {
		case 0 :
			// do nothing;
			break;

		case 1 :
			game_Running.camera1->sidewind( UP, -1 * jitter_amount );
			break;
		case 2 :
			game_Running.camera1->sidewind( DOWN, -1 * jitter_amount );
			break;
		case 3 :
			game_Running.camera1->sidewind( LEFT, -1 * jitter_amount );
			break;
		case 4 :
			game_Running.camera1->sidewind( RIGHT, -1 * jitter_amount );
			break;
	}
	return;
}

void renderScene( ){ 
	if (!entered) {
		entryFunction( );
	}

	glClearColor( currentTheme->backgroundColor[0], currentTheme->backgroundColor[1], currentTheme->backgroundColor[2], 1.0 );
	// glClearColor( 1.0, 1.0, 1.0, 1.0 );
	glClearAccum( 0.0, 0.0, 0.0, 0.0 );
	glClear(GL_ACCUM_BUFFER_BIT);

	for( int i=0; i<numTimes_for_accum_buffer; i++ ){ 
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 

		jitter_the_camera( i );

		game_Running.camera1->writeLookAt( true );

		/*
		switch (i) {
			case 0 : glTranslatef( jitter_amount, 0, 0 ); break;
			case 1 : glTranslatef( -jitter_amount, 0, 0 ); break;
			case 2 : glTranslatef( 0, jitter_amount, 0 ); break;
			case 3 : glTranslatef( 0, -jitter_amount, 0 ); break;
		}
		*/

		// game_Running.camera1->rotateAboutAxis( ((i%4)>1)?X_AXIS:Z_AXIS, 4.8*(i%2?-1.0:1.0) ); 

		glMatrixMode( GL_MODELVIEW ); 
		glLoadIdentity( );

		board.draw( );
		player1.bat.draw( );
		player2.bat.draw( );
		puck.draw( );

		glAccum(GL_ACCUM, 1.0/numTimes_for_accum_buffer);

		unjitter_the_camera( i );
	}
	glAccum(GL_RETURN, 1.0);

	/* Draw UI items */
	game_Running.camera2->writeLookAt( true );

	static char temp_string[10];
	sprintf( temp_string, "%d", player1.numGoals );
	labels[SCORE_LABEL_P1].setLabelText( temp_string );
	sprintf( temp_string, "%d", player2.numGoals );
	labels[SCORE_LABEL_P2].setLabelText( temp_string );
	sprintf( temp_string, "%d:%d", timer_minutes, timer_secs );
	labels[TIMER_LABEL].setLabelText( temp_string );

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

void mouse_sensitivity_change_handler( bool increase ) {
	if (increase) {
		MOUSE_SENSITIVITY += MOUSE_SENSITIVITY_DELTA;
	} else {
		MOUSE_SENSITIVITY -= MOUSE_SENSITIVITY_DELTA;
	}

	if (MOUSE_SENSITIVITY > MOUSE_SENSITIVITY_MAX) {
		MOUSE_SENSITIVITY = MOUSE_SENSITIVITY_MAX;
	}
	if (MOUSE_SENSITIVITY < MOUSE_SENSITIVITY_MIN) {
		MOUSE_SENSITIVITY = MOUSE_SENSITIVITY_MIN;
	}

	return;
}


void eventHandler( SDL_Event &event ){
	static Uint8 *keys;

	if( event.type == SDL_QUIT ){ 
		exit(0); 
	}
	else if( event.type == SDL_KEYDOWN ){ 
		if( event.key.keysym.sym == SDLK_ESCAPE ){ 
			exitFunction( );
			return;
		} else if( event.key.keysym.sym == SDLK_UP ){ 
			// do something

		} else if( event.key.keysym.sym == SDLK_DOWN ){ 
			// do something
			
		} else if (event.key.keysym.sym == SDLK_m) {
			keys = SDL_GetKeyState(NULL);
			if (keys[SDLK_LSHIFT] || keys[SDLK_RSHIFT]) {
				// increase mouse sensitivity
				mouse_sensitivity_change_handler( true );
			} else {
				// decrease mouse sensitivity
				mouse_sensitivity_change_handler( false );
			}
		} else if (event.key.keysym.sym == SDLK_v) {
			keys = SDL_GetKeyState(NULL);
			if (keys[SDLK_LSHIFT] || keys[SDLK_RSHIFT]) {
				soundPlayer.increase_volume();
			} else {
				soundPlayer.decrease_volume();
			}
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
		motion->velocity *= max_speed;
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
		collission_handled2 = true;

	}

	if (distance_between_2_points( puck.x, puck.y, \
				       player2.bat.x, player2.bat.y ) <= \
	    (BAT_RADIUS + PUCK_RADIUS)*BUFFER_FACTOR_FOR_COLLISSION_DETECTION ) {

		handle_collission__puck_with_bat( &puck, &player2.bat );
		collission_handled = true;
		collission_handled2 = true;

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
	soundPlayer.playSoundEffect( SOUND_GOAL_SCORED );

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
	     collission_handled2 = true;
	}
	if ( (puck.y+PUCK_RADIUS) >= BOARD_LENGTH/2.0 ||
	     (puck.y-PUCK_RADIUS) <= -BOARD_LENGTH/2.0 ) {
	     // flip_puck_motion_horizontally
	     puck.motion.velocity[VY] *= -1.0;
	     collission_handled = true;
	     collission_handled2 = true;
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

float ticks_to_arrive_at( float curr_point, float dest_point, float velocity ) {
	if (curr_point == dest_point) {
		return 0.10011;
	}
	if (velocity == 0.0) {
		return INFINITY;
	}
	float time_left = (abs( (dest_point-curr_point) / velocity)) ;
	/**
	 * cap time_left to a max of 3seconds,
	 * this is for the pathological "hung" case where the puck is moving
	 * very slowly along y, and probably briskly along x
	 */
	if (time_left > 3000/work_thread_anim_delay_msecs) {
		time_left = 3000/work_thread_anim_delay_msecs;
	}

	return time_left;
}

void make_plan( ) {
	CBat *me = &player2.bat;

	if (puck.y > me->y){ 			/* puck is not "behind" me */
		if (AGGRESSION > AGGRESSION_MAX/1.5 && \
		    rand()%100 < AGGRESSION) {
			player2_planning.plan_type = GET_BEHIND_PUCK;
			return;
		} else {
			player2_planning.plan_type = DO_NOTHING;
			return;
		}
	}

	/* reached only if puck is not behind me : */

	if (puck.motion.velocity[VY] > 0) { 	/* puck is moving toward my goalPost */
		/**
		 * find the time the puck would take to arrive (in ticks)
		 * at my 'y' if it were to continue along at its current velocity.
		 * is this overcounting? yes (due to friction)
		 *
		 * time_left is in ticks
		 */
		float time_left = ticks_to_arrive_at( me->y, puck.y, puck.motion.velocity[VY] );

		if (time_left < (PLAYER2_THINK_AHEAD_TICKS/2.0) && \
		    me->y <= (50.0/100.0*BOARD_LENGTH/2.0) ) {		/* i am standing too close to the middle line,
		    							 * so, i'll go back a little and manoeuvre */
			if (AGGRESSION > AGGRESSION_MAX/2 && \
			    rand()%100 < AGGRESSION) {
				player2_planning.plan_type = SWING_BACK;
				return;
			} else {
				player2_planning.plan_type = SIMPLE_INTERCEPT;
				return;
			}

		} else {

			if ( me->y >= (50.0/100.0*BOARD_LENGTH/2.0) && 	/* i am standing far enough away from the middle line
									 * to come swooping down at the puck */
			    AGGRESSION > AGGRESSION_MAX/2 && \
			    rand()%100 < AGGRESSION) {
				player2_planning.plan_type = AMBITIOUS_INTERCEPT;
				return;
			} else {
				player2_planning.plan_type = SIMPLE_INTERCEPT;
				return;
			}

		}

		player2_planning.plan_type = SIMPLE_INTERCEPT;
		return;
	
	} else {				/* puck is moving away from my goalPost */

		if (puck.y < 0.0) {		/* puck is not on my side */

			player2_planning.plan_type = COME_TO_MIDDLE_OF_BOARD;
			return;

		} else {			/* puck is on my side, and is moving away */

			if (AGGRESSION > AGGRESSION_MAX/1.5 && \
			    rand()%100 < AGGRESSION) {
				player2_planning.plan_type = AMBITIOUS_PASS_THE_PUCK;
			} else {
				player2_planning.plan_type = PASS_THE_PUCK;
			}
				
			return;

		}

		player2_planning.plan_type = DO_NOTHING;
		return;

	}


	return;
}

float predict_x_of_puck( float time_left ) {
	float predicted_x_of_puck = puck.x + puck.motion.velocity[VX] * time_left;
	if (predicted_x_of_puck < -BOARD_WIDTH/2.0) {
		predicted_x_of_puck += abs( predicted_x_of_puck - (-BOARD_WIDTH/2.0) );
	}
	if (predicted_x_of_puck >  BOARD_WIDTH/2.0) {
		predicted_x_of_puck -= abs( predicted_x_of_puck - BOARD_WIDTH/2.0 );
	}

	return predicted_x_of_puck;
}

float predict_y_of_puck( float time_left ) {
	/**
	 * predicted y of the leading edge of the puck : hence the PUCK_RADIUS
	 */
	float predicted_y_of_puck = puck.y + puck.motion.velocity[VY] * time_left +\
					PUCK_RADIUS;

	return predicted_y_of_puck;
}


void execute_plan( ) {
	CBat *me = &player2.bat;

	/**
	 * find the time the puck would take to arrive (in ticks)
	 * at my 'y' if it were to continue along at its current velocity.
	 * is this overcounting? yes (due to friction)
	 *
	 * time_left is in ticks
	 */
	float time_left = 0.0;
	/**
	 * what will be the x of the puck when it arrives after
	 * time_left ticks
	 * the prediction can predict against 1 bounce off the walls
	 */
	float predicted_x_of_puck = 0.0;

	/**
	 * used for AMBITIOUS_INTERCEPT
	 */
	float wanna_meet_at_y = 0.0;


	switch (player2_planning.plan_type) {

		case DO_NOTHING:
			me->motion.velocity[VX] = 0.0;
			me->motion.velocity[VY] = 0.0;
			printf( "\n DO_NOTHING" );
			break;

		case SIMPLE_INTERCEPT:
			/* puck will be here by ... */
			time_left = ticks_to_arrive_at( me->y, puck.y, puck.motion.velocity[VY] );
			/* ... at x ... */
			predicted_x_of_puck = predict_x_of_puck( time_left );
			/* just get in the way of the puck */
			me->motion.velocity[VX] = (predicted_x_of_puck - me->x) / time_left;
			
			printf( "\n SIMPLE_INTERCEPT" );
			if (time_left < 5000/work_thread_anim_delay_msecs)
				break;
			// else, fall through

		case AMBITIOUS_INTERCEPT:
			/**
			 * we are quite far enough away from the puck
			 * try to meet the puck at a y that is nearly halfway between our
			 * current y and the middle line
			 */
			wanna_meet_at_y = (me->y - 0.0) / (2.0 + AGGRESSION/AGGRESSION_MAX);
			/* puck will be at wanna_meet_at_y by ... */
			time_left = ticks_to_arrive_at( wanna_meet_at_y, puck.y, puck.motion.velocity[VY] );
			/* ... at x ... */
			predicted_x_of_puck = predict_x_of_puck( time_left );
			me->motion.velocity[VX] = (predicted_x_of_puck - me->x) / time_left;
			me->motion.velocity[VY] = (wanna_meet_at_y-me->y) / time_left;

			printf( "\n AMBITIOUS_INTERCEPT" );
			break;

		case SWING_BACK:
			/**
			 * i am standing too close to the board; i'll swing back to
			 * a point nearly halfway between current y and the edge of the board
			 * at a speed that's enough to take me there by PLAYER2_THINK_AHEAD_TICKS
			 */
			wanna_meet_at_y = me->y + (BOARD_LENGTH/2.0 - me->y) / (2.0 + AGGRESSION/AGGRESSION_MAX);
			time_left = PLAYER2_THINK_AHEAD_TICKS;
			predicted_x_of_puck = predict_x_of_puck( time_left );
			me->motion.velocity[VX] = (predicted_x_of_puck - me->x) / time_left;
			me->motion.velocity[VY] = (wanna_meet_at_y-me->y) / time_left;

			printf( "\n SWING_BACK" );
			break;

		case GET_BEHIND_PUCK:
			/**
			 * puck is behind me, try and get behind it
			 */
			wanna_meet_at_y = me->y + (BOARD_LENGTH/2.0 - me->y) / (2.0 + AGGRESSION/AGGRESSION_MAX);
			time_left = PLAYER2_THINK_AHEAD_TICKS;
			me->motion.velocity[VY] = (wanna_meet_at_y-me->y) / time_left;

			printf( "\n GET_BEHIND_PUCK" );
			break;

		case COME_TO_MIDDLE_OF_BOARD:
			wanna_meet_at_y = BOARD_LENGTH/4.0;
			predicted_x_of_puck = 0.0;
			time_left = PLAYER2_THINK_AHEAD_TICKS;
			me->motion.velocity[VX] = (predicted_x_of_puck - me->x) / time_left;
			me->motion.velocity[VY] = (wanna_meet_at_y-me->y) / time_left;

			printf( "\n COME_TO_MIDDLE_OF_BOARD" );
			break;

		case PASS_THE_PUCK:
			/**
			 * puck is on my side and is moving away. i'll have to nudge it across
			 * find the time_left before it crosses over to the other side
			 */
			time_left = ticks_to_arrive_at( 0.0, puck.y, puck.motion.velocity[VY] );
			/* we'll intercept it at 70% of time_left */
			time_left = time_left * (70/100.0);
			predicted_x_of_puck = predict_x_of_puck( time_left );
			wanna_meet_at_y = predict_y_of_puck( time_left );
			me->motion.velocity[VX] = (predicted_x_of_puck - me->x) / time_left;
			me->motion.velocity[VY] = (wanna_meet_at_y-me->y) / time_left;

			printf( "\n PASS_THE_PUCK" );
			break;

		case AMBITIOUS_PASS_THE_PUCK:
			/**
			 * puck is on my side and is moving away. i'll have to nudge it across.
			 * find the time_left before it crosses over to the other side
			 */
			time_left = ticks_to_arrive_at( 0.0, puck.y, puck.motion.velocity[VY] );
			/* we'll intercept it at 50% of time_left */
			time_left = time_left * (50/100.0);

			predicted_x_of_puck = predict_x_of_puck( time_left );
			wanna_meet_at_y = predict_y_of_puck( time_left );
			me->motion.velocity[VX] = (predicted_x_of_puck - me->x) / time_left;
			me->motion.velocity[VY] = (wanna_meet_at_y - me->y) / time_left;

			printf( "\n AMBITIOUS_PASS_THE_PUCK" );
			break;

		default:
			break;
	}

	if (player2_planning.plan_type == AMBITIOUS_PASS_THE_PUCK && puck.motion.velocity[VY] == 0.0) {
		switch (difficulty_level) {
			case DIFFICULTY_LEVEL_HARD : clamp_velocity( &me->motion, MAX_BAT_SPEED_SUSP * AGGRESSION/AGGRESSION_MAX/8.0 );
						     break;
			default : clamp_velocity( &me->motion, MAX_BAT_SPEED_SUSP * AGGRESSION/AGGRESSION_MAX/12.0 );
						     break;
		}
	} else if (player2_planning.plan_type == AMBITIOUS_PASS_THE_PUCK && puck.motion.velocity[VY] == 0.0) {
		switch (difficulty_level) {
			case DIFFICULTY_LEVEL_HARD : clamp_velocity( &me->motion, MAX_BAT_SPEED_SUSP * AGGRESSION/AGGRESSION_MAX/2.0 );
						     break;
			default : clamp_velocity( &me->motion, MAX_BAT_SPEED_SUSP * AGGRESSION/AGGRESSION_MAX/3.0 );
						     break;
		}
	} else {
		switch (difficulty_level) {
			case DIFFICULTY_LEVEL_HARD : clamp_velocity( &me->motion, MAX_BAT_SPEED_SUSP * AGGRESSION/AGGRESSION_MAX );
						     break;
			default : clamp_velocity( &me->motion, MAX_BAT_SPEED_SUSP * AGGRESSION/AGGRESSION_MAX/2.0 );
						     break;
		}
	}

	return;
}

void scripting_for_player2( ) {

	make_plan( );

	execute_plan( );

	return;
}

int work( void * ){

	while( 1 ){
		SDL_Delay( work_thread_anim_delay_msecs ); 
		if (gameState != RUNNING) {
			continue;
		}

		/* update clock */
		if ((int)(ticks*work_thread_anim_delay_msecs)%1000 == 0) { // every second
			timer_secs++;
			if (timer_secs > 60) {
				timer_minutes++;
				timer_secs = 0;
			}
		}

		/* check if game_over */
		if (player1.numGoals >= 5 ||
		    player2.numGoals >= 5 ||
		    timer_minutes    >= 3) {
			exitFunction();
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

		if (gameType == SINGLE_PLAYER) {
			if (ticks%(long int)PLAYER2_THINK_AHEAD_TICKS == 0) {
				/**
				 * Perform some predictive calculations on player2's
				 * plan once every PLAYER2_THINK_AHEAD_TICKS ticks
				 */
				scripting_for_player2( );
			}
			/**
			 * Now, apply the computed velocity to player1's bat
			 */
			player2.bat.translate_X( player2.bat.motion.velocity[VX] );
			player2.bat.translate_Y( player2.bat.motion.velocity[VY], player2.player_ID );

		}

		puck_was_on_my_side = PUCK_IS_ON_MY_SIDE;
		/*
		 * IMPORTANT :
		 * collission detection and handling(done inside)
		 */
		collission_handled2 = false;
		collission_detection( );
		if (collission_handled2) {
			soundPlayer.playSoundEffect( SOUND_TIKTIK );
		}


		// if in MULTI_PLAYER mode, send coordinates data to remote client
		if (gameType == MULTI_PLAYER) {
			/*
			 * we are in a MULTI_PLAYER game, 
			 * constantly send coordinates data
			 */
			if (ticks%2 == 0) {
				/*
				 * Don't want to inundate the network w/ too many packets.
				 * Send 1 packet every 2*work_thread_anim_delay_msecs
				 */
				CPacketData pdata;
				pdata.header = PLAYER1_COORD_PACKET_HEADER;
				pdata.ack_packet = false;
				copy_bat1_to_packet( &pdata );

				if (puck_was_on_my_side) { // && collission_handled) {
					/*
					 * we handled the collission and
					 * we are responsible for sending the position of the puck as well
					 */
					printf( "have done collission handling" );
					pdata.header = PUCK_AND_PLAYER1_COORD_PACKET_HEADER;
					pdata.ack_packet = false;
					copy_puck_to_packet( &pdata );
					/* IMPORTANT */
					collission_handled = false;
				}


				if (!(puck_was_on_my_side && !PUCK_IS_ON_MY_SIDE)) {
					network_queue.insert( pdata );
				}
			}
		}

		// do something
		ticks++;
	}

	return 0; 
}

void copy_bat1_to_packet( CPacketData *pdata ) {
	pdata->batx = player1.bat.x;
	pdata->baty = player1.bat.y;
	pdata->batsx = player1.bat.sx;
	pdata->batsy = player1.bat.sy;
	
	pdata->batvelocityx = player1.bat.motion.velocity[VX];
	pdata->batvelocityy = player1.bat.motion.velocity[VY];

	return;
}

void copy_puck_to_packet( CPacketData *pdata ) {
	pdata->puckx = puck.x;
	pdata->pucky = puck.y;

	pdata->puckvelocityx = puck.motion.velocity[VX];
	pdata->puckvelocityy = puck.motion.velocity[VY];

	return;
}

void init_UI_items( ) {
	float w = get_GW( ); 
	float h = get_GH( ); 

	panels[SCORE_PANEL].init( (int)SCORE_PANEL, w*100/100.0, h*15/100,  0.0, -h*42.5/100.0, 2*SMALL_EPSILON, \
	currentTheme->scorePanelImage );
	panels[SCORE_PANEL].visible = true;
	panels[SCORE_PANEL].enabled = true;

	labels[SCORE_LABEL_P1].init( (int)SCORE_LABEL_P1, w*90/100.0, h*10/100,  -w*20/100.0, -h*42.5/100.0, 2.2*SMALL_EPSILON );
	labels[SCORE_LABEL_P1].visible = true;
	labels[SCORE_LABEL_P1].enabled = true;
	labels[SCORE_LABEL_P1].setLabelText( "0" );
	
	labels[SCORE_LABEL_P2].init( (int)SCORE_LABEL_P1, w*90/100.0, h*10/100,  w*20/100.0, -h*42.5/100.0, 2.2*SMALL_EPSILON );
	labels[SCORE_LABEL_P2].visible = true;
	labels[SCORE_LABEL_P2].enabled = true;
	labels[SCORE_LABEL_P2].setLabelText( "0" );

	labels[TIMER_LABEL].init( (int)TIMER_LABEL, w*90/100.0, h*10/100,  w*0/100.0, -h*42.5/100.0, 2.2*SMALL_EPSILON );
	labels[TIMER_LABEL].visible = true;
	labels[TIMER_LABEL].enabled = true;
	labels[TIMER_LABEL].setLabelText( "0:0" );

	panels[SCORE_PANEL].addPanelObjek( &labels[SCORE_LABEL_P1] );
	panels[SCORE_PANEL].addPanelObjek( &labels[SCORE_LABEL_P2] );
	panels[SCORE_PANEL].addPanelObjek( &labels[TIMER_LABEL] );

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

