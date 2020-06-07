#pragma once

using namespace std;

constexpr auto RADIAN = 0.01745329252f;

#define DEBUG_CHEAT
#define LOG_DEBUG
#define LOG_FOLDER "log"
#define LOG_PATH "log/debug.log"
#define SCRIPT_PATH "scripts"
#define DEBUG_MODE

#define PISTOLS_GUNPOS_X 10
#define PISTOLS_GUNPOS_Y 190
#define PISTOLS_GUNPOS_Z 50
#define UZIS_GUNPOS_X 10
#define UZIS_GUNPOS_Y 190
#define UZIS_GUNPOS_Z 50
#define SHOTGUN_GUNPOS_X 0
#define SHOTGUN_GUNPOS_Y 180
#define SHOTGUN_GUNPOS_Z 80
#define REVOLVER_GUNPOS_X 0
#define REVOLVER_GUNPOS_Y 215
#define REVOLVER_GUNPOS_Z 65
#define GRENADEGUN_GUNPOS_X -16
#define GRENADEGUN_GUNPOS_Y 228
#define GRENADEGUN_GUNPOS_Z 32

#define LT_Function            __FUNCTION__
#define LT_Default             "Default"
#define LT_Enter               "Enter"
#define LT_Exit                "Exit"
#define LT_Release             "Release"
#define LT_Info                "Info"
#define LT_DXTry               "DirectX"
#define LT_Error               "Error"
#define LT_Warning             "Warning"
#define LT_Version             "Version"

#define ALL_MESH -1
#define NO_TRACK -1
#define NO_FLYING 0
#define NO_HORIZON -1
#define NO_CLIP -1
#define NOT_EXIST -1
#define NO_ROPE -1
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
#define ALL_AIOBJ (GUARD|AMBUSH|PATROL1|MODIFY|FOLLOW|PATROL2)

#define TIMID FALSE
#define VIOLENT TRUE

#define MAX_PICKUPS 8
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
#define MAX_DYNAMICS 32
#define MAX_ROPES 4
#define MAX_SPAWNER_ENTITY 50000 // ~50

#define SFX_ALWAYS 2
#define DONT_TARGET (-16384)
#define NO_ROOM 0xFF
#define NO_HEIGHT (32512)
#define WALL_L 1024
#define STEP_L 256
#define FLY_ZONE 0x2000
#define STEPUP_HEIGHT ((STEP_L*3)/2) // (STEPUP_HEIGHT = 384)

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