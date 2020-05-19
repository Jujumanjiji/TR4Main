#pragma once
#pragma pack(push, 1)

typedef FILE* LPFILE;

typedef char            int8;
typedef unsigned char   uint8;
typedef signed char     sint8;
typedef short           int16;
typedef unsigned short  uint16;
typedef signed short    sint16;
typedef int             int32;
typedef unsigned int    uint32;
typedef signed int      sint32;
/// Cant be 32 since int is set, so i renommed it, but it's not 64, it's 32 !
typedef long            int64;
typedef unsigned long   uint64;
typedef signed long     sint64;

struct JMP
{
    BYTE opCode;     // must be 0xE9;
    DWORD offset;    // jump offset
};

struct STATS
{
    DWORD timer;
    DWORD distance;
    DWORD ammo_used;
    DWORD ammo_hits;
    WORD kills;
    BYTE secrets;
    BYTE health_used;
};

struct BONE_STRUCT
{
    DWORD flags;
    int x;
    int y;
    int z;
};

struct PHD_MATRIX // size: 48
{
    int m00; // 0
    int m01; // 1
    int m02; // 2
    int m03; // 3
    int m10; // 4
    int m11; // 5
    int m12; // 6
    int m13; // 7
    int m20; // 8
    int m21; // 9
    int m22; // 10
    int m23; // 11
};

struct LARA_MATRIX
{
    PHD_MATRIX hips;
    PHD_MATRIX thigh_l;
    PHD_MATRIX calf_l;
    PHD_MATRIX foot_l;
    PHD_MATRIX thigh_r;
    PHD_MATRIX calf_r;
    PHD_MATRIX foot_r;
    PHD_MATRIX torso;
    PHD_MATRIX head;
    PHD_MATRIX uarm_r;
    PHD_MATRIX larm_r;
    PHD_MATRIX hand_r;
    PHD_MATRIX uarm_l;
    PHD_MATRIX larm_l;
    PHD_MATRIX hand_l;
};

struct GAMEFLOW
{
    uint32 cheat_enabled : 1;        // LOBYTE: 0x1
    uint32 load_save_enabled : 1;    // LOBYTE: 0x2
    uint32 title_enabled : 1;        // LOBYTE: 0x4
    uint32 play_any_level : 1;       // LOBYTE: 0x8
    uint32 languages : 3;            // LOBYTE: 0x10
    uint32 demo_disc : 1;            // LOBYTE: 0x20
    uint32 unused : 24;              // LOBYTE: 0x40 HIBYTE: 0x1
    uint32 input_timeout;
    uint8 security_tag;
    uint8 number_level;
    uint8 number_filename;
    uint8 pad;
    uint16 filename_length;
    uint16 script_length;
};

struct HANG_STRUCT
{
    int type;
    int dest_x;
    int dest_z;
    short dest_angle;
};

struct BOUNDARY
{
    short x_min;
    short x_max;
    short y_min;
    short y_max;
    short z_min;
    short z_max;
    short xrot_min;
    short xrot_max;
    short yrot_min;
    short yrot_max;
    short zrot_min;
    short zrot_max;
};

struct WEAPON_INFO
{
    short lock_angle[4];
    short left_angle[4];
    short right_angle[4];
    short aim_speed;
    short shot_accuracy;
    short gun_height;
    short target_dist;
    char damage;
    char recoil_frame;
    char flash_time;
    char draw_frame;
    short sample_id;
};

struct PISTOL_DEF
{
    short object_number;
    char draw1_anim2;
    char draw1_anim;
    char draw2_anim;
    char recoil_anim;
};


struct BITE_INFO
{
    int x;
    int y;
    int z;
    int mesh;
};

struct AIOBJECT
{
    short object_number;
    short room_number;
    int x;
    int y;
    int z;
    short trigger_flags;
    short flags;
    short y_rot;
    short box_number;
};

struct OBJ_ROTATION
{
    short torso_y;
    short torso_x;
    short head_y;
    short head_x;
};

struct GAME_VECTOR
{
    int x;
    int y;
    int z;
    short room_number;
    short box_number;
};

struct OBJECT_VECTOR
{
    int x;
    int y;
    int z;
    short data;
    short flags;
};

struct PHD_3DPOS
{
    int xPos;
    int yPos;
    int zPos;
    short xRot;
    short yRot;
    short zRot;
};

struct PHD_VECTOR
{
    int x;
    int y;
    int z;
};

struct FLOOR_INFO
{
    unsigned short index;
    unsigned short fx : 4;
    unsigned short box : 11;
    unsigned short stopper : 1;
    unsigned char pit_room;
    char floor;
    unsigned char sky_room;
    char ceiling;
};

struct CVECTOR
{
    BYTE r;
    BYTE g;
    BYTE b;
    BYTE cd;

    CVECTOR()
    {
        this->r = 0;
        this->g = 0;
        this->b = 0;
        this->cd = 0;
    }

    CVECTOR(BYTE r, BYTE g, BYTE b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->cd = 0;
    }

    CVECTOR(BYTE r, BYTE g, BYTE b, BYTE a)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->cd = a;
    }
};

struct LIGHT_INFO
{
    int x;
    int y;
    int z;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char type;        // LIGHT_FLAGS::
    unsigned char falloff;
    unsigned char intensity;
    float in;
    float out;
    float length;
    float cutoff;
    float x_target;
    float y_target;
    float z_target;
};

struct MESH_INFO
{
    int x;
    int y;
    int z;
    short y_rot;
    short shade;
    short ocb; // MESH_OCB flags
    short static_number;
};

struct COLOR_VECTOR
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char cd;
};

struct FLOAT_VECTOR
{
    float x;
    float y;
    float z;
};

struct DOOR_INFO
{
    WORD room_number;
    short point[3];
    short corner[12];
};

struct DOORS
{
    WORD number_doors;
    DOOR_INFO door_info;
};

struct PCLIGHT
{
    float x, y, z;       // Position of light, in world coordinates
    float r, g, b;       // Colour of the light
    unsigned int shadow_intensity;    // only if LightType == LIGHT_SHADOW
    float In;            // Cosine of the IN value for light / size of IN value
    float Out;           // Cosine of the OUT value for light / size of OUT value
    float RadIn;         // (IN radians) * 2
    float RadOut;        // (OUT radians) * 2
    float Range;         // Range of light
    float dx, dy, dz;    // Direction - used only by sun and spot lights
    int x2, y2, z2;      // Same as position, only in integer.
    int dx2, dy2, dz2;   // Same as direction, only in integer.
    float r2, g2, b2;
    float r3, g3, b3;
    int unknown;
    BYTE light_type;
    BYTE unknown_2;
    int dx3, dy3, dz3;
    int magnsq;
};

struct ITEM_LIGHT
{
    int r;
    int g;
    int b;
    CVECTOR room_ambient;
    int d_r;
    int d_g;
    int d_b;
    int room_number;
};

struct ROOM_INFO
{
    short* data;
    short* door;
    FLOOR_INFO* floor;
    LIGHT_INFO* light;
    MESH_INFO* mesh;
    int x;
    int y;
    int z;
    int minfloor;
    int maxceiling;
    short x_size;
    short y_size;
    unsigned long ambiant;
    short num_light;
    short num_meshes;
    unsigned char reverb_type;
    unsigned char flip_number;
    char mesh_effect;
    char bound_active;
    short left;
    short right;
    short top;
    short bottom;
    short test_left;
    short test_right;
    short test_top;
    short test_bottom;
    short item_number;
    short fx_number;
    short flipped_room;
    unsigned short flags; // ROOM_INFO_Flags
    int vertice_amount;
    int vertice_water_amount;
    int vertice_dry_amount;
    void* pDirect3DVertexBuffer;
    void* pRectangles;
    float room_x;
    float room_y;
    float room_z;
    int vertice_offset;
    void* pTriAndQuad;
    void* pQuadVertice;
    void* pTriVertice;
    int unknown;
    FLOAT_VECTOR* vertice_float;
    int triangle_amount;
    int rectangle_amount;
    void* pLightObject;
};


struct ITEM_INFO
{
    long floor;
    DWORD touchBits;
    DWORD meshBits;
    short objectNumber;
    short currentAnimState;
    short goalAnimState;
    short requiredAnimState;
    short animNumber;
    short frameNumber;
    short roomNumber;
    short nextItem;
    short nextActive;
    short speed;
    short fallspeed;
    short hitPoints;
    WORD boxNumber;
    short timer;
    short flags;                       // For oneshot and code switches (i.e. NOT flags)
    short shade;
    short triggerFlags;
    short carriedItem;
    short afterDeath;
    WORD firedWeapon;
    short itemFlags[4];
    LPVOID data;
    PHD_3DPOS pos;
    ITEM_LIGHT il;
    BYTE legacyLightData[5496];
    unsigned int active : 1;            // LOBYTE: 0x1
    unsigned int status : 2;            // LOBYTE: 0x4
    unsigned int gravityStatus : 1;     // LOBYTE: 0x8
    unsigned int hitStatus : 1;         // LOBYTE: 0x10
    unsigned int collidable : 1;        // LOBYTE: 0x20
    unsigned int lookedAt : 1;         // LOBYTE: 0x40
    unsigned int dynamicLight : 1;     
    unsigned int poisoned : 1;
    unsigned int aiBits : 5;
    unsigned int reallyActive : 1;
    unsigned int InDrawRoom : 1;
    DWORD meshswapMeshbits;
    short drawRoom;
    short TOSSPAD;
};

struct COLL_INFO
{
    int mid_floor;              // relative floor and ceiling heights at midpoint
    int mid_ceiling;
    int mid_type;
    int front_floor;            // relative floor and ceiling heights at front
    int front_ceiling;
    int front_type;
    int left_floor;             // relative floor and ceiling heights at front/left side
    int left_ceiling;
    int left_type;
    int right_floor;            // relative floor and ceiling heights at front/right side
    int right_ceiling;
    int right_type;
    int left_floor2;            // relative floor and ceiling heights at back/left side
    int left_ceiling2;
    int left_type2;
    int right_floor2;           // relative floor and ceiling heights at back/right side
    int right_ceiling2;
    int right_type2;
    long radius;                // INPUT Collision Radius...
    long bad_pos, bad_neg;      // INPUT Relative Heights We want to Collide against...
    long bad_ceiling;           // INPUT Relative Bad Ceiling Height...
    PHD_VECTOR shift;           // Shift/Push Values..
    PHD_VECTOR old;             // INPUT Old Positions
    short old_anim_state;       // INPUT old animation
    short old_anim_number;
    short old_frame_number;
    short facing;                // INPUT Angle we are Moving...
    short quadrant;              // Quadrant we are moving ( Not necessarily facing! )
    short coll_type;             // Type of Collision (COLL_TYPE enum)
    short* trigger;              // Trigger info index
    char tilt_x, tilt_z;         // Type of Tilt of Floor
    char hit_by_baddie;          // Flag to indicate Lara has been hit by a Baddie
    char hit_static;             // Flag to indicate Lara has collided with a Static object
    WORD slopes_are_walls : 2;   // Treat big slopesUp as walls
    WORD slopes_are_pits : 1;    // Treat big slopesDown as pits
    WORD lava_is_pit : 1;        // Treat Lava as Bad place to go onto
    WORD enable_baddie_push : 1; // Baddies can push Lara
    WORD enable_spaz : 1;        // Spaz animations are enabled
    WORD hit_ceiling : 1;        // Has Lara hit ceiling? (For up jump only).
};

typedef void(*LPINITIALISE)(short item_number);
typedef void(*LPCONTROL)(short item_number);
typedef void(*LPFLOOR)(ITEM_INFO* item, int x, int y, int z, int* height);
typedef void(*LPCEILING)(ITEM_INFO* item, int x, int y, int z, int* height);
typedef void(*LPDRAW)(ITEM_INFO* item);
typedef void(*LPCOLLISION)(short item_number, ITEM_INFO* laraitem, COLL_INFO* coll);
typedef void(*LPDRAWEXTRA)(ITEM_INFO* item);

struct OBJECT_INFO
{
    short nMeshes;
    short meshIndex;
    int boneIndex;
    short* frameBase;
    LPINITIALISE initialise;
    LPCONTROL control;
    LPFLOOR floor;
    LPCEILING ceiling;
    LPDRAW drawRoutine;
    LPCOLLISION collision;
    short mipDistance;         // if mip distance is set (more than 0) then if lara is far away, the object will be changed to the _MIP obj after the current one (objects.h).
    short animIndex;
    short hitPoints;
    short pivotLength;
    short radius;
    short shadowSize;
    WORD bitOffset;
    WORD loaded : 1;
    WORD intelligent : 1;
    WORD nonLot : 1;
    WORD savePosition : 1;
    WORD saveHitpoints : 1;
    WORD saveFlags : 1;
    WORD saveAnim : 1;
    WORD semiTransparent : 1;
    WORD waterCreature : 1;
    WORD usingDrawAnimatingItem : 1;
    WORD hitEffect : 2;
    WORD undead : 1;
    WORD saveMesh : 1;
    WORD unknown : 2;
    LPDRAWEXTRA drawRoutineExtra;
    int explodableMeshBits;
    int pad;
};

struct RANGE_STRUCT
{
    short startFrame;
    short endFrame;
    short linkAnimNum;
    short linkFrameNum;
};

struct CHANGE_STRUCT
{
    short anim_goal_state;
    short number_ranges;
    short range_index;
};

struct ANIM_STRUCT
{
    short* framePtr;
    short interpolation;
    short currentStateAnim;
    int ySpeed;     // velocity
    int yFallspeed; // acceleration
    int xSpeed;
    int xFallspeed;
    short frameBase;
    short frameEnd;
    short jumpAnimNum;
    short jumpFrameNum;
    short numberChanges;
    short changeIndex;
    short numberCommands;
    short commandIndex;
};

struct STATIC_INFO
{
    short mesh_number;
    short flags;
    short x_minp;
    short x_maxp;
    short y_minp;
    short y_maxp;
    short z_minp;
    short z_maxp;
    short x_minc;
    short x_maxc;
    short y_minc;
    short y_maxc;
    short z_minc;
    short z_maxc;
};

struct CAMERA_INFO
{
    GAME_VECTOR pos;
    GAME_VECTOR target;
    int type;
    int old_type;
    int shift;
    int flags;
    int fixed_camera;
    int number_frames;
    int bounce;
    int underwater;
    int target_distance;
    short target_angle;
    short target_elevation;
    short actual_elevation;
    short actual_angle;
    short number;
    short last;
    short timer;
    short speed;
    ITEM_INFO *item;
    ITEM_INFO *last_item;
    OBJECT_VECTOR *fixed;
    int mike_at_lara;
    PHD_VECTOR unknown; // last_target/last_pos ?
    PHD_VECTOR mike_pos;
    short mike_pos_room;
};

struct LARA_ARM
{
    short *frame_base;
    short frame_number;
    short anim_number;
    short lock;
    short y_rot;
    short x_rot;
    short z_rot;
    short flash_gun;
};

struct LOCATION
{
    char current;
    char highest;
    char pad;
};

struct LARA_MESH
{
    sint16 *hips;
    sint16 *thigh_l;
    sint16 *calf_l;
    sint16 *foot_l;
    sint16 *thigh_r;
    sint16 *calf_r;
    sint16 *foot_r;
    sint16 *torso;
    sint16 *uarm_r;
    sint16 *larm_r;
    sint16 *hand_r;
    sint16 *uarm_l;
    sint16 *larm_l;
    sint16 *hand_l;
    sint16 *head;
};

struct FX_INFO
{
    PHD_3DPOS pos;
    short room_number;
    short object_number;
    short next_fx;
    short next_active;
    short speed;
    short fallspeed;
    short frame_number;
    short counter;
    short shade;
    short flag1;
    short flag2;
};

struct DOOR_VBUF
{
    int xv;
    int yv;
    int zv;
};

struct ROPE_STRUCT
{
    PHD_VECTOR seg[24];
    PHD_VECTOR vel[24];
    PHD_VECTOR normal[24];
    PHD_VECTOR mesh[24];
    PHD_VECTOR pos;
    PHD_VECTOR unknown[24];
    int segLength;
    short active;
    short coiled;
};

struct PENDULUM
{
    PHD_VECTOR pos;
    PHD_VECTOR vel;
    int node;
    ROPE_STRUCT *rope;
};

struct GUNFLASH_STRUCT
{
    PHD_MATRIX matrix;
    short on;
};

struct HAIR_STRUCT
{
    PHD_3DPOS pos;
    PHD_VECTOR vel;
};

struct DYNAMIC
{
    int x;
    int y;
    int z;
    unsigned char on;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned short falloff;
    unsigned char used;
    unsigned char pad1[1];
    int falloff_scale;
};

struct FIRE_SPARKS
{
    short x;
    short y;
    short z;
    short xvel;
    short yvel;
    short zvel;
    short gravity;
    short rotang;
    short flags;
    unsigned char sSize;
    unsigned char dSize;
    unsigned char Size;
    unsigned char friction;
    unsigned char scalar;
    unsigned char def;
    char rotadd;
    char maxyvel;
    unsigned char on;
    unsigned char sR;
    unsigned char sG;
    unsigned char sB;
    unsigned char dR;
    unsigned char dG;
    unsigned char dB;
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char colfadespeed;
    unsigned char fadetoblack;
    unsigned char slife;
    unsigned char life;
};

struct GUNSHELL_STRUCT
{
    PHD_3DPOS pos;
    short fallspeed;
    short room_number;
    short speed;
    short counter;
    short direction_xrot;
    short object_number;
};

struct SMOKE_SPARKS
{
    int x;
    int y;
    int z;
    short xvel;
    short yvel;
    short zvel;
    short gravity;
    short rotang;
    short flags;
    unsigned char sSize;
    unsigned char dSize;
    unsigned char size;
    unsigned char friction;
    unsigned char scalar;
    unsigned char def;
    char rotadd;
    char maxyvel;
    unsigned char on;
    unsigned char sShade;
    unsigned char dShade;
    unsigned char shade;
    unsigned char colfadespeed;
    unsigned char fadetoblack;
    char sLife;
    char life;
    unsigned char transtype;
    unsigned char fx_obj;
    unsigned char node_number;
    unsigned char mirror;
};

struct SPARKS
{
    int x;
    int y;
    int z;
    short xvel;
    short yvel;
    short zvel;
    short gravity;
    short rotang;
    short flags; // SPF_ flags
    unsigned char sSize;
    unsigned char dSize;
    unsigned char size;
    unsigned char friction;
    unsigned char scalar;
    unsigned char def; // sprite id (need SPF_DEF flags !)
    char rotadd;
    char maxyvel;
    unsigned char on; // can be bool later
    BYTE sR;
    BYTE sG;
    BYTE sB;
    BYTE dR;
    BYTE dG;
    BYTE dB;
    BYTE r;
    BYTE g;
    BYTE b;
    unsigned char colfadespeed;
    unsigned char fadetoblack;
    unsigned char sLife;
    unsigned char life;
    unsigned char transtype; // TRANS_ flags
    unsigned char extras;
    char dynamic;
    unsigned char fx_obj;
    unsigned char room_number;
    unsigned char node_number;
};

struct BLOOD_STRUCT
{
    int x;
    int y;
    int z;
    short xvel;
    short yvel;
    short zvel;
    short gravity;
    short rotang;
    unsigned char sSize;
    unsigned char dSize;
    unsigned char size;
    unsigned char friction;
    char rotadd;
    unsigned char on;
    unsigned char sShade;
    unsigned char dShade;
    unsigned char shade;
    unsigned char colfadespeed;
    unsigned char fadetoblack;
    char sLife;
    char life;
    char pad;
};

struct SPLASH_STRUCT
{
    int x;
    int y;
    int z;
    short inner_rad;
    short inner_size;
    short inner_radvel;
    short inner_yvel;
    short inner_y;
    short middle_rad;
    short middle_size;
    short middle_radvel;
    short middle_yvel;
    short middle_y;
    short outer_rad;
    short outer_size;
    short outer_radvel;
    char flags;
    unsigned char life;
};

struct BUBBLE_STRUCT
{
    PHD_VECTOR pos;
    short room_number;
    short speed;
    short size;
    short dSize;
    unsigned char shade;
    unsigned char vel;
    unsigned char y_rot;
    char flags;
    short xvel;
    short yvel;
    short zvel;
    short pad;
};

struct DRIP_STRUCT
{
    int x;
    int y;
    int z;
    unsigned char on;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    short yvel;
    unsigned char gravity;
    unsigned char life;
    short room_number;
    unsigned char outside;
    unsigned char pad;
};

struct SHOCKWAVE_STRUCT
{
    int x;
    int y;
    int z;
    short inner_rad;
    short outer_rad;
    short x_rot;
    short flags;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char life;
    short speed;
    short temp;
};


struct RIPPLE_STRUCT
{
    int x;
    int y;
    int z;
    char flags;
    unsigned char life;
    unsigned char size;
    unsigned char init;
};

struct DEBRIS_STRUCT
{
    void* textinfo;
    int x;
    int y;
    int z;
    short xyz_offset1[3];
    short direction;
    short xyz_offset2[3];
    short speed;
    short xyz_offset3[3];
    short yvel;
    short gravity;
    short room_number;
    unsigned char on;
    unsigned char x_rot;
    unsigned char y_rot;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char pad[22];
};

struct FIRE_LIST
{
    int x;
    int y;
    int z;
    unsigned char on;
    unsigned char size;
    short room_number;
};

struct BOX_NODE
{
    short exit_box;
    unsigned short search_number;
    short next_expansion;
    short box_number;
};

struct BOX_INFO
{
    unsigned char left;
    unsigned char right;
    unsigned char top;
    unsigned char bottom;
    short height;
    short overlap_index;
};

struct AI_INFO
{
    short zoneNumber;
    short enemyZone;
    int distance;
    int ahead;
    int bite;
    short angle;
    short xAngle;
    short enemyFacing;
};

struct LOT_INFO
{
    BOX_NODE *node;
    short head;
    short tail;
    unsigned short search_number;
    unsigned short block_mask;
    short step;
    short drop;
    short zone_count;
    short target_box;
    short required_box;
    short fly;
    unsigned short can_jump : 1;
    unsigned short can_monkey : 1;
    unsigned short is_amphibious : 1;
    unsigned short is_jumping : 1;
    unsigned short is_monkeying : 1;
    PHD_VECTOR target;
    int zone;
};

struct CREATURE_INFO
{
    short jointRotation[4];
    short maximumTurn;
    short flags;
    unsigned short alerted : 1;
    unsigned short headLeft : 1;
    unsigned short headRight : 1;
    unsigned short reachedGoal : 1;
    unsigned short hurtByLara : 1;
    unsigned short patrol2 : 1;
    unsigned short jumpAhead : 1;
    unsigned short monkeyAhead : 1;
    MOOD_TYPE mood;
    ITEM_INFO *enemy;
    ITEM_INFO aiTarget;
    short pad;
    short itemNumber;
    PHD_VECTOR target;
    LOT_INFO LOT;
};

typedef short(*lpEffect)(int x, int y, int z, short speed, short y_rot, short roomNumber);

struct ENTITY_JUMP
{
    bool can_jump_1click;
    bool can_jump_2click;
};

struct OBJECT_FOUND
{
    short item_number;
    ITEM_INFO* target;
};

struct LARA_INFO
{
    short item_number;
    short gun_status;         // LHS_ enum
    short gun_type;           // LG_ enum
    short gun_request_type;   // LG_ enum
    short gun_last_type;      // LG_ enum
    short calc_fallspeed;
    short water_status;       // LWS_ enum
    short climb_status;
    short pose_count;
    short hit_frame;
    short hit_direction;
    short air;
    short dive_count;
    short death_count;
    short current_active;
    short current_xvel;
    short current_yvel;
    short current_zvel;
    short spaz_effect_count;
    short flare_age;
    short skidoo;
    short weapon_item;
    short back_gun;
    short flare_frame;
    short poisoned;
    short poisoned_2;
    unsigned char anxiety;
    unsigned char wet[15];
    WORD flare_control_left : 1; // LOBYTE: 0x1
    WORD flare_control_right : 1;// LOBYTE: 0x2
    WORD look : 1;               // LOBYTE: 0x4
    WORD burn : 1;               // LOBYTE: 0x8
    WORD keep_ducked : 1;        // LOBYTE: 0x10
    WORD is_moving : 1;          // LOBYTE: 0x20
    WORD can_monkey_swing : 1;   // LOBYTE: 0x40
    WORD burn_blue : 1;          // LOBYTE: 0x80
    WORD not_used : 1;           // LOBYTE: 0x100,  HIBYTE: 0x1
    WORD burn_smoke : 1;         // LOBYTE: 0x200,  HIBYTE: 0x2
    WORD is_ducked : 1;          // LOBYTE: 0x400,  HIBYTE: 0x4
    WORD has_fired : 1;          // LOBYTE: 0x800,  HIBYTE: 0x8
    WORD busy : 1;               // LOBYTE: 0x1000, HIBYTE: 0x10
    WORD lit_torch : 1;          // LOBYTE: 0x2000, HIBYTE: 0x20
    WORD is_climbing : 1;        // LOBYTE: 0x4000, HIBYTE: 0x40
    WORD fired : 1;              // LOBYTE: 0x8000, HIBYTE: 0x80
    long water_surface_dist;
    PHD_VECTOR last_pos;
    FX_INFO *spaz_effect;
    int mesh_effects;
    LARA_MESH mesh;
    ITEM_INFO *target;
    short target_angles[2];
    short turn_rate;
    short move_angle;
    short head_y_rot;
    short head_x_rot;
    short head_z_rot;
    short torso_y_rot;
    short torso_x_rot;
    short torso_z_rot;
    LARA_ARM l_arm;
    LARA_ARM r_arm;
    WORD holsters;
    CREATURE_INFO *creature;
    long corner_x;
    long corner_z;
    char rope_segment;
    char rope_direction;
    short rope_arc_front;
    short rope_arc_back;
    short rope_last_x;
    short rope_maxX_forward;
    short rope_maxX_backward;
    long rope_dframe;
    long rope_frame;
    WORD rope_framerate;
    WORD rope_y;
    long rope_ptr;
    LPVOID general_ptr;
    int rope_offset;
    DWORD rope_downvel;
    char rope_flag;
    char move_count;
    int rope_count;
    char pistols_type_carried;    // CR_ flags
    char uzi_type_carried;        // CR_ flags
    char shotgun_type_carried;    // CR_ flags
    char crossbow_type_carried;   // CR_ flags
    char grenadegun_type_carried; // CR_ flags
    char revolver_type_carried;   // CR_ flags
    char lasersight;
    char binoculars;
    char crowbar;
    char clockwork_beetle;
    char water_skin_empty1;
    char water_skin_empty2;
    char examine1;
    char examine2;
    char examine3;
    char puzzleitems[12];
    short puzzleitemscombo;
    short keyitems;
    short keyitemscombo;
    short pickupitems;
    short pickupitemscombo;
    short questitems;
    short small_medipack_count;
    short large_medipack_count;
    short flare_count;
    short pistol_ammo_count;
    short uzi_ammo_count;
    short revolver_ammo_count;
    short shotgun_ammo1_count;
    short shotgun_ammo2_count;
    short grenade_ammo1_count;
    short grenade_ammo2_count;
    short grenade_ammo3_count;
    short crossbow_ammo1_count;
    short crossbow_ammo2_count;
    short crossbow_ammo3_count;
    char unknown;
    char blind_timer;
    LOCATION locate;
    short dash_timer;
};

struct BONE
{
    int bone1;
    int bone2;
    int bone3;

    BONE(int* bone, int boneID)
    {
        bone1 = *(bone + 1 + (boneID * 4));
        bone2 = *(bone + 2 + (boneID * 4));
        bone3 = *(bone + 3 + (boneID * 4));
    }

    BONE(int boneX, int boneY, int boneZ)
    {
        bone1 = boneX;
        bone2 = boneY;
        bone3 = boneZ;
    }
};

typedef short**(*lpAddons)(ITEM_INFO* item, OBJECT_INFO* obj, OBJECT_INFO* obj_sec, int meshID);

struct RENDER
{
    int meshID;
    int boneID;
    bool rotHead, rotTorso;
    bool push, pop;
    bool start;
    lpAddons function;

    // start for frame[]
    RENDER(int meshID, bool push, bool pop)
    {
        this->start = true;
        this->boneID = 0;
        this->meshID = meshID;
        this->rotHead = false;
        this->rotTorso = false;
        this->push = push;
        this->pop = pop;
        this->function = NULL;
    }

    RENDER(int boneID, int meshID, bool push, bool pop)
    {
        this->start = false;
        this->boneID = boneID;
        this->meshID = meshID;
        this->rotHead = false;
        this->rotTorso = false;
        this->push = push;
        this->pop = pop;
        this->function = NULL;
    }

    RENDER(int boneID, int meshID, lpAddons function, bool push, bool pop)
    {
        this->start = false;
        this->boneID = boneID;
        this->meshID = meshID;
        this->rotHead = false;
        this->rotTorso = false;
        this->push = push;
        this->pop = pop;
        this->function = function;
    }

    RENDER(int boneID, int meshID, bool rotHead, bool rotTorso, bool push, bool pop)
    {
        this->start = false;
        this->boneID = boneID;
        this->meshID = meshID;
        this->rotHead = rotHead;
        this->rotTorso = rotTorso;
        this->push = push;
        this->pop = pop;
        this->function = NULL;
    }

    RENDER(int boneID, int meshID, lpAddons function, bool rotHead, bool rotTorso, bool push, bool pop)
    {
        this->start = false;
        this->boneID = boneID;
        this->meshID = meshID;
        this->rotHead = rotHead;
        this->rotTorso = rotTorso;
        this->push = push;
        this->pop = pop;
        this->function = function;
    }
};

struct D3DTEXTUREINFO
{
    DDPIXELFORMAT ddpf;
    unsigned long bpp;
    bool bPalette;
    bool bAlpha;
    unsigned char rbpp;
    unsigned char gbpp;
    unsigned char bbpp;
    unsigned char abpp;
    unsigned char rshift;
    unsigned char gshift;
    unsigned char bshift;
    unsigned char ashift;
};

struct DISPLAYMODE
{
    int w;
    int h;
    unsigned long bpp;
    unsigned int mipMapCount;
    bool bPalette;
    DDSURFACEDESC2 ddsd;
    unsigned char rbpp;
    unsigned char gbpp;
    unsigned char bbpp;
    unsigned char rshift;
    unsigned char gshift;
    unsigned char bshift;
};

struct DIRECTSOUNDINFO
{
    char Name[30];
    char About[80];
    LPGUID lpGuid;
    GUID Guid;
};

struct DIRECTDRAWINFO
{
    char Name[30];
    char About[80];
    LPGUID lpGuid;
    GUID Guid;
    DDCAPS DDCaps;
    DDDEVICEIDENTIFIER DDDevIdent;
    int nDisplayMode;
    DISPLAYMODE *DisplayMode;
    int nD3DInfo;
    D3DTEXTUREINFO *D3DInfo;
};

struct DEVICEINFO
{
    int nDDInfo;
    int nDSInfo;
    DIRECTDRAWINFO* DDInfo;
    DIRECTSOUNDINFO* DSInfo;
};

struct DXCONFIG
{
    int nDD;
    int nD3D;
    int nVMode;
    int nTexture;
    int bZbuffer;
    int nSoundCard;
};

struct WINAPP
{
    HINSTANCE hInstance;
    HWND WindowHandle;
    WNDCLASS WindowClass;
    DEVICEINFO DeviceInfo;
    DXCONFIG DXConfig;
    char Unknown;
    LPDIRECTDRAW4 lpDD;
    LPDIRECT3D3 lpD3D;
    LPDIRECT3DDEVICE3 lpD3DDevice;
    LPDIRECT3DDEVICE3 lpD3DDevice_BIS;
    LPDIRECTDRAWSURFACE4 lpFrontBuffer;
    LPDIRECTDRAWSURFACE4 lpBackBuffer;
    LPDIRECTDRAWSURFACE4 lpZBuffer;
    LPDIRECT3DVIEWPORT3 lpViewPort;
    LPDIRECTSOUND lpDS;
    int WinWidth;
    int WinHeight;
    RECT GnWindowRect;
    RECT GnClientRect;
    unsigned int Flags;
    unsigned int DXWndStyle;
    unsigned int DXCoopLevel;
    LPDIRECTINPUT7 lpDI;
    LPDIRECTINPUTDEVICE2A lpDIKeyboard;
    LPDIRECTINPUTDEVICE2A lpDIJoystick;
    BOOL sceneEnabled;
    BOOL bNoFocus;
    BOOL sceneDone;
    HANDLE hObject;
    float fps;
    DWORD dword753A51;
    DWORD dword753A55;
    HACCEL hAccTable;
    BYTE gameInitialized;
    BYTE settingBumpMap;
    DWORD settingTextRes;
    DWORD settingDumpRes;
    BYTE mmxSupport;
    BYTE settingFilter;
    BYTE settingVolumetric;
    BYTE disableSound;
    DWORD settingHardware;
    BYTE playFmv;
    DWORD originalBPP;
    BOOL autoTarget;
};

struct SAVEGAME_COUNT // size: 88
{
    short days;
    short seconds;
    short minutes;
    short hours;
    int save_count;
    char levelName[75];
    bool present;
};

struct PICKUP_STRUCT
{
    short timer;
    short object_number;
};

struct SPHERE
{
    sint32 x;
    sint32 y;
    sint32 z;
    sint32 r;
};

struct SHATTER_ITEM
{
    SPHERE sptr;
    ITEM_LIGHT *il;
    short *meshp;
    int bit;
    short yrot;
    short flags;
};

struct D3DTLBUMPVERTEX
{
    D3DVALUE sx;
    D3DVALUE sy;
    D3DVALUE sz;
    D3DVALUE rhw;
    D3DCOLOR color;
    D3DCOLOR specular;
    D3DVALUE tu;
    D3DVALUE tv;
    D3DVALUE tu2;
    D3DVALUE tv2;
};

struct DINFO_HEAD
{
    float z;
    short texattr;
    short tile_and_flag;
    short num_verts;
    short unknown;
};

struct DINFO_LINE
{
    DINFO_HEAD head;
    D3DTLBUMPVERTEX v1;
    D3DTLBUMPVERTEX v2;
};

struct DINFO_TRI
{
    DINFO_HEAD head;
    D3DTLBUMPVERTEX v1;
    D3DTLBUMPVERTEX v2;
    D3DTLBUMPVERTEX v3;
};

struct DINFO_QUAD
{
    DINFO_HEAD head;
    D3DTLBUMPVERTEX v1;
    D3DTLBUMPVERTEX v2;
    D3DTLBUMPVERTEX v3;
    D3DTLBUMPVERTEX v1bis;
    D3DTLBUMPVERTEX v3bis;
    D3DTLBUMPVERTEX v4;
};

struct OBJECT_TEXTURE_VERT
{
    float x;
    float y;
};

struct OBJECT_TEXTURE
{
    unsigned short attribute;
    unsigned short tile_and_flag;
    unsigned short new_flags;
    OBJECT_TEXTURE_VERT vertices[4];
};

struct TEXTUREBUCKET
{
    int tpage;
    int cnt;
    D3DTLBUMPVERTEX Vertex[2080];
};

typedef void(*EFFECT_ROUTINES_STRUCT)(ITEM_INFO* item);

#pragma pack(pop)