#pragma once

using namespace std;

#define DEBUG_CHEAT
#define LOG_DEBUG
#define LOG_FOLDER "log"
#define LOG_PATH "log/debug.log"
#define SCRIPT_PATH "scripts"
#define DEBUG_MODE

#define PISTOLS_GUNPOS_X 10
#define PISTOLS_GUNPOS_Y 140
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

constexpr auto ALL_MESH = -1;
constexpr auto NO_TRACK = -1;
constexpr auto NO_FLYING = 0;
constexpr auto NO_HORIZON = -1;
constexpr auto NO_CLIP = -1;
constexpr auto NOT_EXIST = -1;
constexpr auto NO_ROPE = -1;
constexpr auto NO_TILT = 0;
constexpr auto BADDIE_POISON_DAMAGE = 1;

#define GUARD 0x0001
#define AMBUSH 0x0002
#define PATROL1 0x0004
#define MODIFY 0x0008
#define FOLLOW 0x0010
#define PATROL2 0x0020
#define ALL_AIOBJ (GUARD|AMBUSH|PATROL1|MODIFY|FOLLOW|PATROL2)

#define TIMID FALSE
#define VIOLENT TRUE

constexpr auto MAX_PICKUPS = 8;
constexpr auto MAX_SLOTS = 32;
constexpr auto MAX_EXPANSION = 5;
constexpr auto MAX_SHOCKWAVE = 16;
constexpr auto MAX_BUBBLES = 30;
constexpr auto MAX_DRIPS = 32;
constexpr auto MAX_RIPPLES = 16;
constexpr auto MAX_SPLASHES = 4;
constexpr auto MAX_BLOOD = 32;
constexpr auto MAX_DEBRIS = 256;
constexpr auto MAX_GUNSHELLS = 24;
constexpr auto MAX_SMOKESPARKS = 32;
constexpr auto MAX_SPARKS = 256;
constexpr auto MAX_GUNFLASH = 4;
constexpr auto MAX_FIRE_SPARKS = 20;
constexpr auto MAX_FIRE_LIST = 32;
constexpr auto MAX_LIGHTNING = 3;
constexpr auto MAX_DYNAMICS = 32;
constexpr auto MAX_ROPES = 4;
constexpr auto MAX_SPAWNER_ENTITY = 50000; // ~50 (1000 = 1)

constexpr auto SFX_ALWAYS = 2;
constexpr auto DONT_TARGET = -16384;
constexpr auto NO_ROOM = 0xFF;
constexpr auto NO_HEIGHT = 32512;
constexpr auto STEPUP_HEIGHT = ((3 * STEP_L) / 2);

constexpr auto NUM_SLOTS = 5;
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