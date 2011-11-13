void execute_plan( ) {
	CBat *me = &player2.bat;

	me->motion.velocity[VY] = 0.0;

	if (puck.motion.velocity[VY] > 0) {	/* puck is moving toward 'my' goalPost 	*/

		/**
		 * find the time the puck would take to arrive (in ticks)
		 * at my 'y' if it were to continue along at its current velocity.
		 * is this overcounting? yes (due to friction)
		 */
		float time_left = abs( (me->y - puck.y) /\
			   	  puck.motion.velocity[VY]);

		/**
		 * compute a wanna_be_x for us to be at at the end of time_left
		 */
		float wanna_be_x;
		if (time_left > 3 && 	/* lots of time left	 */
		    puck.y < me->y) {	/* puck is not behind me */
			/**
			 * if there is too much time_left, better orient myself to
			 * score a goal along w/ just defending my goalPost
			 * this should be a function of AMBITION
			 * i want to make sure that me, the puck and the other goalPost
			 * are collinear
			 */
			float theta = atan2( puck.y - (-BOARD_LENGTH/2.0), puck.x - 0.0 );
			wanna_be_x = puck.x + cos(theta) * (BAT_RADIUS+PUCK_RADIUS);
		} else {		/* not too much time left */
		 	/**
			 * we should try to at least intercept the puck at the
			 * x it would be at at the end of time_left
			 * or, simpler still, we could just try to mimick the puck's x
			 * at every point till then
			 */
			wanna_be_x = puck.x;
			/**
			 * if there is not too much time_left, i might want to consider
			 * (depending on my AMBITION) moving back a little to better 
			 * align myself to score a goal
			 */
		}
		/**
		 * now calculate the velocity[VX] at which we'll have to sidle across
		 * so that we can be at wanna_be_x at the end of time_left
		 */
		float delta_x = wanna_be_x - me->x;
		me->motion.velocity[VX] = MAX_PUCK_SPEED_SUSP * delta_x / BOARD_WIDTH;

	} else {				/* puck is moving away from 'my' goalPost */
		/**
		 * if i am too far back, i might want to come forward a little
		 * too, towards the middle to better tackle the next attack on
		 * my goalPost, probably do this in "idle" time (when puck is in
		 * player1's court and moving away)
		 */
		if (puck.y <= 0) { 		/* puck is in player1's court */
			/**
			 * try and move my y to the middle of my court.
			 * note that this is in "idle time"
			 */
			float delta_y = BOARD_LENGTH/4.0 - me->y;
			me->motion.velocity[VY] = delta_y * BOARD_LENGTH/4.0 / MAX_PUCK_SPEED_SUSP;
		}

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

