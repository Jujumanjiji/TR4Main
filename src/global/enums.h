#pragma once

enum SHIFT_FLAG
{
    WALL_SHIFT = 10,
    W2V_SHIFT = 14
};

enum ZONE_TYPE
{
	ZONE_SKELLY,
	ZONE_BASIC,
	ZONE_WATER,
	ZONE_HUMAN,
	ZONE_FLYER,
};

enum MOOD_TYPE
{
	BORED_MOOD,
	ATTACK_MOOD,
	ESCAPE_MOOD,
	STALK_MOOD
};

enum BONE_TYPE
{
	BT_FLAG,
	BT_X,
	BT_Y,
	BT_Z,
};

enum RENDER_TYPE
{
	RENDER_DYNAMICS = 0,
	RENDER_STATIC = 1,
	RENDER_OBJECTS = 2,
	RENDER_EFFECT = 3,
	RENDER_LARA = 4,
	RENDER_HORIZON_AND_LAYER = 6
};

enum TARGET_TYPE
{
	NO_TARGET = 0x0,
	PRIMARY_TARGET = 0x1,
	SECONDARY_TARGET = 0x2,
};

///==============================================///
///      USED BY IDAPRO TO FOUND THE FLAGS       ///
///==============================================///
/// PS: can be used in TRNG, becareful some flag may be missing/incomplete/not_correct_address.

// use lara_info flag instead (isDucked etc...)
enum LARA_INFO_FlagsComplete
{
    LCINFO_NONE = 0x0,
    LCINFO_FLARE_CONTROL_LEFT = 0x1,	// LOBYTE()
    LCINFO_FLARE_CONTROL_RIGHT = 0x2,
    LCINFO_LOOK = 0x4,
    LCINFO_BURN = 0x8,
    LCINFO_KEEP_DUCKED = 0x10,
    LCINFO_IS_MOVING = 0x20,
    LCINFO_CAN_MONKEY_SWING = 0x40,
    LCINFO_BURN_BLUE = 0x80,
    LCINFO_BURN_SMOKE = 0x100,			// HIBYTE()
    LCINFO_IS_DUCKED = 0x200,
    LCINFO_HAS_FIRED = 0x400,
    LCINFO_BUSY = 0x800,
    LCINFO_LIT_TORCH = 0x1000,
    LCINFO_USING_ITEM = 0x2000,
    LCINFO_IS_CLIMBING = 0x4000,
    LCINFO_FIRED = 0x8000
};

enum CREATURE_INFO_Flags
{
	CINFO_ALERTED = 0x0001,
	CINFO_HEAD_LEFT = 0x0002,
	CINFO_HEAD_RIGHT = 0x0004,
	CINFO_REACHED_GOAL = 0x0008,
	CINFO_HURT_BY_LARA = 0x0010,
	CINFO_PATROL2 = 0x0020,
	CINFO_JUMP_AHEAD = 0x0040,
	CINFO_MONKEY_AHEAD = 0x0080
};

enum LOT_INFO_Flags
{
	LOT_JUMP = 0x1,
	LOT_MONKEY = 0x2,
	LOT_IS_AMPHIBIOUS = 0x4,
	LOT_IS_JUMPING = 0x8,
	LOT_IS_MONKEYING = 0x10,
};

// complete version (start to 0 if you using 2 different flags at WATER_CREATURE !)
enum OBJECT_INFO_FlagsComplete
{
    OCINFO_LOADED = 0x1,               // LOWORD (flags1)
    OCINFO_INTELLIGENT = 0x2,
    OCINFO_NON_LOT = 0x4,
    OCINFO_SAVE_POSITION = 0x8,
    OCINFO_SAVE_HITPOINTS = 0x10,
    OCINFO_SAVE_FLAGS = 0x20,
    OCINFO_SAVE_ANIM = 0x40,
    OCINFO_SEMI_TRANSPARENT = 0x80,
    OCINFO_WATER_CREATURE = 0x100,     // HIWORD (flags2)
    OCINFO_USING_DRAWANIMATINGITEM = 0x200,
    OCINFO_HIT_EFFECT = 0x400,
    OCINFO_UNDEAD = 0x800,
    OCINFO_SAVE_MESH = 0x1000,
    OCINFO_UNKNOWN = 0x2000
};

enum OBJECT_INFO_LOWORDFlags
{
    OINFO_LOADED = 0x1,               // LOWORD (flags1)
    OINFO_INTELLIGENT = 0x2,
    OINFO_NON_LOT = 0x4,
    OINFO_SAVE_POSITION = 0x8,
    OINFO_SAVE_HITPOINTS = 0x10,
    OINFO_SAVE_FLAGS = 0x20,
    OINFO_SAVE_ANIM = 0x40,
    OINFO_SEMI_TRANSPARENT = 0x80
};

enum OBJECT_INFO_HIWORDFlags
{
    OINFO_WATER_CREATURE = 0,     // HIWORD (flags2)
    OINFO_USING_DRAWANIMATINGITEM = 0x2,
    OINFO_HIT_EFFECT = 0x4,
    OINFO_UNDEAD = 0x8,
    OINFO_SAVE_MESH = 0x10,
    OINFO_UNKNOWN = 0x20
};

enum LARA_INFO_Flags2
{
    LINFO2_NONE = 0x0,
    LINFO2_BURN_SMOKE = 0x1,
    LINFO2_IS_DUCKED = 0x2,
    LINFO2_HAS_FIRED = 0x4,
    LINFO2_BUSY = 0x8,
    LINFO2_LIT_TORCH = 0x10,
    LINFO2_USING_ITEM = 0x20,
    LINFO2_IS_CLIMBING = 0x40
};

enum ITEM_INFO_Flags
{
	IFLAG_SWITCH_ONESHOT = 0x0040,
	IFLAG_CLEAR_BODY = 0x0080,
	IFLAG_INVISIBLE = 0x0100,
	IFLAG_CODEBITS = 0x3E00,
	IFLAG_REVERSE = 0x4000,
	IFLAG_KILLED_ITEM = 0x8000,
};

enum ITEM_INFO_PrimaryFlagsAlternate
{
	FITEM_ACTIVE = 0x0000,
	FITEM_NOT_ACTIVE = 0x0001,
	FITEM_DEACTIVATED = 0x0002,
	FITEM_INVISIBLE = 0x0003
};

enum ITEM_INFO_PrimaryFlags
{
    NO_ITEM = -1,
    FITEM_GRAVITY = 0x0008,
    FITEM_HIT_STATUE = 0x0010,
    FITEM_COLLIDABLE = 0x0020,
    FITEM_LOOKED_AT = 0x0040,
    FITEM_DYNAMIC_LIGHT = 0x0080,
    FITEM_POISONED = 0x0100,
    FITEM_AI_GUARD = 0x0200,
    FITEM_AI_AMBRUSH = 0x0400,
    FITEM_AI_PATROL1 = 0x0800,
    FITEM_AI_MODIFY = 0x1000,
    FITEM_AI_FOLLOW = 0x2000,
    FITEM_REALLY_ACTIVE = 0x4000,
    FITEM_IN_DRAW_ROOM = 0x8000,
	FITEM_ALL_AI = (FITEM_AI_GUARD | FITEM_AI_AMBRUSH | FITEM_AI_PATROL1 | FITEM_AI_MODIFY | FITEM_AI_FOLLOW)
};

enum ROOM_INFO_Flags
{
    ROOM_WATER = 0x0001,
    ROOM_ISFLIPPED = 0x0002,
    ROOM_QUICKSAND = 0x0004,
    ROOM_HORIZON = 0x0008,
    ROOM_DAMAGE = 0x0010,
    ROOM_OUTSIDE = 0x0020,
    ROOM_INSIDE = 0x0040,
    ROOM_NON_LENS_FLARE = 0x0080,
    ROOM_MIST = 0x0100,
    ROOM_REFLECT = 0x0200,
    ROOM_SNOW = 0x0400,
    ROOM_RAIN = 0x0800,
    ROOM_COLD = 0x1000
};

enum gfLevel_Flags
{
    SLEV_YOUNG_LARA = 0x0001,
    SLEV_WEATHER = 0x0002,
    SLEV_HORIZON = 0x0004,
    SLEV_LAYER1 = 0x0008,
    SLEV_LAYER2 = 0x0010,
    SLEV_STAR_FIELD = 0x0020,
    SLEV_LIGHTNING = 0x0040,
    SLEV_TRAIN = 0x0080,
    SLEV_PULSE = 0x0100,
    SLEV_COL_ADD_HORIZON = 0x0200,
    SLEV_RESET_HUB = 0x0400,
    SLEV_LENS_FLARE = 0x0800,
    SLEV_TIMER = 0x1000,
    SLEV_MIRROR = 0x2000,
    SLEV_REMOVE_AMULET = 0x4000,
    SLEV_NO_LEVEL = 0x8000
};

///========================================///
///                OTHER                   ///
///========================================///

enum LIGHT_FLAGS
{
    D3D_NO_LIGHT = -1,
    D3D_SUN,
    D3D_LIGHT,
    D3D_SPOT_OR_EFFECT,
    D3D_SHADOW,
    D3D_FOG_BULB
};

enum Bones_Flags
{
    ROT_X = 0x4,
    ROT_Y = 0x8,
    ROT_Z = 0x10,
    ROT_ALL = ROT_X | ROT_Y | ROT_Z
};

enum CommandTypes
{
    COMMAND_NULL,
    COMMAND_MOVE_ORIGIN,
    COMMAND_JUMP_VELOCITY,
    COMMAND_ATTACK_READY,
    COMMAND_DEACTIVATE,
    COMMAND_SOUND_FX,
    COMMAND_EFFECT
};

enum MATRIX
{
    M00,
    M01,
    M02,
    M03,
    M10,
    M11,
    M12,
    M13,
    M20,
    M21,
    M22,
    M23
};

enum HIT_EFFECT_Flags
{
    HIT_NONE,
    HIT_BLOOD,
    HIT_SMOKE,
    HIT_FRAGMENT
};

enum LARA_HOLD
{
	LG_UNARMED = 0,
	LG_PISTOLS,      // WEAPON
	LG_REVOLVER,     // WEAPON
	LG_UZIS,         // WEAPON
	LG_SHOTGUN,      // WEAPON
	LG_GRENADEGUN,   // WEAPON
	LG_CROSSBOW,     // WEAPON
	LG_FLARE,        // WEAPON
	LG_TORCH,        // ITEM
	LG_FIRED_TORCH,  // ITEM
	LG_JEEP,         // VEHICLE
	LG_SIDECAR,      // VEHICLE
	LG_MAX_HOLD      // END_OF_LARA_HOLD (DONT DELETE IT !!)
};

enum FunctionMap
{
	F_MOTORBIKE_INIT,
	F_MOTORBIKE_CONTROL,
	F_MOTORBIKE_COLLISION,
	F_MOTORBIKE_EFFECT,
	F_MOTORBIKE_DRAWSMOKE,
	F_JEEP_INIT,
	F_JEEP_COLLISION,
	F_JEEP_DRAW_EFFECT,
	F_JEEP_CONTROL,
	F_GUIDE_INIT,
	F_GUIDE_CONTROL,
	F_SKELETON_INIT,
	F_SKELETON_CONTROL,
	F_VONCROY_INIT,
	F_VONCROY_CONTROL_TUTORIAL,
	F_VONCROY_CONTROL_NORMAL,
	F_BADDY_INIT,
	F_BADDY_CONTROL,
	F_SCORPION_INIT,
	F_SCORPION_CONTROL,
	F_SMALLSCORPION_INIT,
	F_SMALLSCORPION_CONTROL,
	F_MUMMY_INIT,
	F_MUMMY_CONTROL,
	F_KNIGHTTEMPLAR_INIT,
	F_KNIGHTTEMPLAR_CONTROL,
	F_SPHINX_INIT,
	F_SPHINX_CONTROL,
	F_SETHA_INIT,
	F_SETHA_CONTROL,
	F_LARADOUBLE_INIT,
	F_LARADOUBLE_CONTROL,
	F_HORSEMAN_INIT,
	F_HORSEMAN_CONTROL,
	F_HAMMERHEAD_INIT,
	F_HAMMERHEAD_CONTROL,
	F_CROCODILE_INIT,
	F_CROCODILE_CONTROL,
	F_DEMIGOD_INIT,
	F_DEMIGOD_CONTROL,
	F_MUTANT_INIT,
	F_MUTANT_CONTROL,
	F_TROOPS_INIT,
	F_TROOPS_CONTROL,
	F_SAS_INIT,
	F_SAS_CONTROL,
	F_HARPY_INIT,
	F_HARPY_CONTROL,
	F_WILDBOAR_INIT,
	F_WILDBOAR_CONTROL,
	F_DOG_INIT,
	F_DOG_CONTROL,
	F_AHMET_INIT,
	F_AHMET_CONTROL,
	F_BABOON_INIT,
	F_BABOON_CONTROL,
	F_ENEMYJEEP_TRAIN_INIT,
	F_ENEMYJEEP_INIT,
	F_ENEMYJEEP_TRAIN_CONTROL,
	F_ENEMYJEEP_CONTROL,
	F_BAT_INIT,
	F_BAT_CONTROL,
	F_BIGBEETLE_INIT,
	F_BIGBEETLE_CONTROL,
	F_SENTRYGUN_INIT,
	F_SENTRYGUN_CONTROL,
	F_HORSE_INIT,
	F_SASDYING_INIT,
	F_SASDYING_CONTROL,
	F_JEANYVES_INIT,
	F_JEANYVES_CONTROL,
	F_GAMEPIECE_INIT,
	F_GAMEPIECE_CONTROL,
	F_WHEELOFFORTUNE_INIT,
	F_WHEELOFFORTUNE_COLLISION,
	F_WHEELOFFORTUNE_CONTROL,
	F_WRAITH_INIT,
	F_WRAITH_CONTROL,
	F_WRAITH_DRAW,
	F_LITTLEBEETLE_INIT,
	F_LITTLEBEETLE_CONTROL,
	F_FISH_INIT,
	F_FISH_CONTROL,
};