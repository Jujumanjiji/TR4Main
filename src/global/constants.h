#pragma once

using namespace std;

//#define DEBUG_MODE

#define LT_Default             (char*)0
#define LT_Enter               (char*)1
#define LT_Exit                (char*)2
#define LT_Release             (char*)3
#define LT_Info                (char*)4
#define LT_WindowMessage       (char*)5
#define LT_DXTry               (char*)6
#define LT_Error               (char*)7
#define LT_Warning             (char*)8
#define LT_Version             (char*)9
#define LT_Texture             (char*)10    // crappy temporary texture loading messages
#define LT_Sound               (char*)11    // sample info

#define NO_TRACK -1
#define NO_FLY 0
#define NO_HORIZON -1
#define NO_CLIP -1
#define NOT_EXIST -1
#define NO_TILT 0

#define ONE_DEGREE 182
#define ANGLE(x) ((x) * ONE_DEGREE)
#define GAME_FOV 80
#define DEFAULT_FOV (GAME_FOV * ONE_DEGREE)
#define BADDIE_POISON_DAMAGE 1

#define GUARD 0x0001
#define AMBUSH 0x0002
#define PATROL1 0x0004
#define MODIFY 0x0008
#define FOLLOW 0x0010
#define PATROL2 0x0020

#define TIMID FALSE
#define VIOLENT TRUE

#define MAX_SLOTS 32
#define MAX_EXPANSION 5
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

#define SFX_ALWAYS 2
#define DONT_TARGET (-16384)
#define NO_ROOM 255
#define WALL_L 1024
#define STEP_L 256
#define FLY_ZONE 0x2000
#define STEPUP_HEIGHT ((STEP_L*3)/2)

#define NUM_SLOTS 5
#define NO_BOX 0x7FF
#define BOX_NUMBER 0x7FF
#define BOX_END_BIT 0x8000
#define BOX_JUMP 0x800
#define BOX_MONKEY 0x2000

#define CLIP_LEFT   1
#define CLIP_RIGHT  2
#define CLIP_TOP    4
#define CLIP_BOTTOM 8
#define ALL_CLIP (CLIP_LEFT|CLIP_RIGHT|CLIP_TOP|CLIP_BOTTOM)
#define SECONDARY_CLIP 16

#define BIFF 0x200
#define BIFF_AVOID_TURN 0x800

#define BLOCKABLE 0x8000
#define BLOCKED 0x4000
#define OVERLAP_INDEX 0x3FFF
#define SEARCH_NUMBER 0x7FFF
#define BLOCKED_SEARCH 0x8000
#define WALL_FLOOR -32512

#define FEELER_DISTANCE 512
#define FEELER_ANGLE ANGLE(45)
#define REACHED_GOAL_RADIUS (STEP_L*3)
#define PREDICTIVE_SCALE_FACTOR 14

#define MAX_TILT ANGLE(3)
#define MAX_JOINT_CHANGE ANGLE(3)
#define JOINT_ARC 0x3000

#define CREATURE_FLOAT_SPEED (WALL_L/32)