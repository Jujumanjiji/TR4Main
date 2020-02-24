#pragma once

#define LARA_CLAMPP(var, incValue, maxValue)\
    var += incValue;\
    if (var > maxValue) var = maxValue

#define LARA_CLAMPN(var, incValue, maxValue)\
    var -= incValue;\
    if (var < -maxValue) var = -maxValue

#define LARA_LEAN_UNDO  	(ONE_DEGREE)        				// Leaning around Corners ..
#define LARA_LEAN_RATE		((ONE_DEGREE/2) + LARA_LEAN_UNDO)
#define LARA_LEAN_MAX		((10*ONE_DEGREE) + LARA_LEAN_UNDO)

#define LARA_TURN_UNDO  	(2*ONE_DEGREE) 						// Large Turn UNDO limits LAG
#define LARA_TURN_RATE		((ONE_DEGREE/4) + LARA_TURN_UNDO)
#define LARA_JUMP_TURN		((ONE_DEGREE*1) + LARA_TURN_UNDO)
#define LARA_SLOW_TURN		((ONE_DEGREE*2) + LARA_TURN_UNDO)
#define LARA_MED_TURN		((ONE_DEGREE*4) + LARA_TURN_UNDO)
#define LARA_FAST_TURN		((ONE_DEGREE*6) + LARA_TURN_UNDO)



#define LARA_SPRINT_TO_ROLL_FRAME 2