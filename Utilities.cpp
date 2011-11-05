#include "Utilities.h"
#include <cmath>

float distance_between_2_points( float x1, float y1,  float x2, float y2 ) {
	return ( sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) ) );
}

