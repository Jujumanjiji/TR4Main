#pragma once

//#define DEBUG_MODE

#define SCRIPT_FUNCTION "scripts/function.json"

#define LT_Default			(char*)0
#define LT_Enter			(char*)1
#define LT_Exit				(char*)2
#define LT_Release			(char*)3
#define LT_Info				(char*)4
#define LT_WindowMessage	(char*)5
#define LT_DXTry			(char*)6
#define LT_Error			(char*)7
#define LT_Warning			(char*)8
#define LT_Version			(char*)9
#define LT_Texture			(char*)10	// crappy temporary texture loading messages
#define LT_Sound			(char*)11	// sample info

#define NO_FLY 0
#define NO_HORIZON -1
#define NO_CLIP -1
#define NOT_EXIST -1
#define NO_TILT 0

#define ONE_DEGREE 182
#define GAME_FOV 80
#define DEFAULT_FOV (GAME_FOV * ONE_DEGREE)
#define BADDIE_POISON_DAMAGE 1

#define AI_GUARD 0x0001
#define AI_AMBRUSH 0x0002
#define AI_PATROL1 0x0004
#define AI_MODIFY 0x0008
#define AI_FOLLOW 0x0010

#define TIMID FALSE
#define VIOLENT TRUE

#define MAX_SHOCKWAVE 16
#define MAX_BUBBLES 30
#define MAX_DRIPS 32
#define MAX_RIPPLES 16
#define MAX_SPLASHES 4
#define MAX_BLOOD 32
#define MAX_DEBRIS 256
#define MAX_GUNSHELLS 24
#define MAX_SMOKESPARKS 32
#define MAX_SPARKS 256
#define MAX_GUNFLASH 4
#define MAX_FIRE_SPARKS 20
#define MAX_FIRE_LIST 32
#define MAX_LIGHTNING 3
#define MAX_DYNAMICS 64

#define DONT_TARGET (-16384)
#define NO_ROOM 255
#define WALL_L 1024
#define STEP_L 256
#define FLY_ZONE 0x2000
#define STEPUP_HEIGHT ((STEP_L*3)/2)

#define NUM_SLOTS 5
#define NO_BOX 0x7FF
#define BOX_NUMBER 0x7FF
#define BOX_END_BIT	0x8000

#define BLOCKABLE 0x8000
#define BLOCKED 0x4000
#define OVERLAP_INDEX 0x3FFF
#define SEARCH_NUMBER 0x7FFF
#define BLOCKED_SEARCH 0x8000
#define WALL_FLOOR -32512

#define PREDICTIVE_SCALE_FACTOR 14