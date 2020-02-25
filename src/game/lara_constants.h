#pragma once

#define LARA_AIR (2*30*30)
#define LARA_HITPOINTS 1000
#define LARA_DASH_TIME (30*4)
#define GRAVITY 6
#define SWAMP_GRAVITY 2
#define LARA_RAD 100
#define LARA_HITE 762
#define LARA_DUCK_HEIGHT 400
#define UW_RADIUS 300
#define UW_HITE 400
#define CLIMB_WIDTHR 120
#define CLIMB_WIDTHL 120
#define DEATH_SPEED 100
#define DEATH_ACC 5
#define LARA_EXPOSURE_TIME ((30*10)*2)  // max exposure time (* precision)
#define LARA_WADE_EXPOSURE 1            // wade exposure rate
#define LARA_SWIM_EXPOSURE 2            // swim exposure rate
#define LARA_HEAT_EXPOSURE 1            // replenish exposure rate
#define DIVE_COUNT 10
#define CAM_A_HANG 	0
#define CAM_E_HANG -ANGLE(60)
#define HANG_ANGLE ANGLE(35)
#define MAX_HEAD_ROTATION ANGLE(44)		// Look Around Limits
#define MAX_HEAD_TILT ANGLE(30)
#define MIN_HEAD_TILT -ANGLE(35)
#define HEAD_TURN ANGLE(2)
#define LARA_DEF_ADD_EDGE ANGLE(5)
#define LARA_DUCK_DEFLECT ANGLE(2)
#define LARA_CRAWLB_RAD 250
#define VAULT_ANGLE	ANGLE(30)
#define SLOPE_DIF 60
#define BAD_JUMP_CEILING ((STEP_L*3)/4)
#define UW_MAXSPEED (50*4)
#define WATER_FRICTION 6
#define UW_WALLDEFLECT ANGLE(2)

#define LARA_CLAMPP(var, incValue, maxValue)\
    var += incValue;\
    if (var > maxValue) var = maxValue

#define LARA_CLAMPN(var, incValue, maxValue)\
    var -= incValue;\
    if (var < -maxValue) var = -maxValue

#define LARA_LEAN_UNDO (ONE_DEGREE)                                 // Leaning around Corners ..
#define LARA_LEAN_RATE ((ONE_DEGREE/2) + LARA_LEAN_UNDO)
#define LARA_LEAN_MAX ((10*ONE_DEGREE) + LARA_LEAN_UNDO)
#define LARA_TURN_UNDO (2*ONE_DEGREE)                               // Large Turn UNDO limits LAG
#define LARA_TURN_RATE ((ONE_DEGREE/4) + LARA_TURN_UNDO)
#define LARA_JUMP_TURN ((ONE_DEGREE*1) + LARA_TURN_UNDO)
#define LARA_SLOW_TURN ((ONE_DEGREE*2) + LARA_TURN_UNDO)
#define LARA_MED_TURN ((ONE_DEGREE*4) + LARA_TURN_UNDO)
#define LARA_FAST_TURN ((ONE_DEGREE*6) + LARA_TURN_UNDO)
#define LARA_DASH_LEAN_RATE ((ONE_DEGREE/2) + LARA_LEAN_UNDO)
#define LARA_DASH_LEAN_MAX ((ONE_DEGREE*15) + LARA_LEAN_UNDO)
#define LARA_DASH_TURN_RATE ((ONE_DEGREE/4) + LARA_TURN_UNDO)
#define LARA_DASH_TURN_MAX ((ONE_DEGREE*2) + LARA_TURN_UNDO)
#define LARA_MONKEY_TURN_RATE ((ONE_DEGREE/4) + LARA_TURN_UNDO)
#define LARA_MONKEY_TURN_MAX ((ONE_DEGREE*2) + LARA_TURN_UNDO)
#define LARA_LEAN_MAX_UW (LARA_LEAN_MAX*2)
#define FASTFALL_SPEED 128
#define LARA_FASTFALL_SPEED	(FASTFALL_SPEED+3)
#define FASTFALL_FRICTION(item) (item->speed * 95) / 100
#define DAMAGE_START  140
#define DAMAGE_LENGTH 14

#define LARA_SPRINT_TO_ROLL_FRAME 2
#define LARA_STAND_TO_ROLL_FRAME 2