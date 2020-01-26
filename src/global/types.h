#pragma once
#pragma pack(push, 1)

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

struct PHD_MATRIX
{
    int m00;
    int m01;
    int m02;
    int m03;
    int m10;
    int m11;
    int m12;
    int m13;
    int m20;
    int m21;
    int m22;
    int m23;
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
    int x;
    int y;
    int z;
    short x_rot;
    short y_rot;
    short z_rot;
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
	signed char floor;
    unsigned char sky_room;
	signed char ceiling;
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
    short ocb;
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
	WORD number_door;
	DOOR_INFO doors[256];
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
	DWORD ambiant;
    short num_light;
    short num_meshes;
    BYTE reverb_type;
    BYTE flip_number;
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
    WORD flags; // ROOM_INFO_Flags
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
    int floor;
    unsigned int touch_bits;
	unsigned int mesh_bits;
    short object_number;
    short anim_state_current;
    short anim_state_goal;
    short anim_state_required;
    short anim_number;
    short frame_number;
    short room_number;
    short next_item;
    short next_free;
    short speed;
    short fallspeed;
    short hit_points;
    WORD box_number;
    short timer;
    short flags;                       // For oneshot and code switches (i.e. NOT flags)
    short shade;
    short trigger_bits;
    short carried_item;
    short after_death;
	WORD fired_weapon;
    short item_flags[4];
    LPVOID data;
    PHD_3DPOS pos;
    BYTE light_data[5528];
	unsigned int active : 1;
	unsigned int status : 2;
	unsigned int gravity_status : 1;
	unsigned int hit_status : 1;
	unsigned int collidable : 1;
	unsigned int looked_at : 1;
	unsigned int dynamic_light : 1;
	unsigned int poisoned : 1;
	unsigned int ai_bits : 5;
	unsigned int really_active : 1;
	unsigned int in_draw_room : 1;
    int meshswap_meshbits;
    short draw_room;
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
    short facing;               // INPUT Angle we are Moving...
    short quadrant;             // Quadrant we are moving ( Not necessarily facing! )
    short coll_type;            // Type of Collision
    short* trigger;             // Trigger info index
    char tilt_x, tilt_z;        // Type of Tilt of Floor
    char hit_by_baddie;         // Flag to indicate Lara has been hit by a Baddie
    char hit_static;            // Flag to indicate Lara has collided with a Static object
    unsigned short slopes_are_walls : 2;   // Treat big slopesUp as walls
    unsigned short slopes_are_pits : 1;    // Treat big slopesDown as pits
    unsigned short lava_is_pit : 1;        // Treat Lava as Bad place to go onto
    unsigned short enable_baddie_push : 1; // Baddies can push Lara
    unsigned short enable_spaz : 1;        // Spaz animations are enabled
    unsigned short hit_ceiling : 1;        // Has Lara hit ceiling? (For up jump only).
};

typedef void(*LPDRAW)(ITEM_INFO* item);
typedef void(*LPDRAWEXTRA)(ITEM_INFO* item);

struct OBJECT_INFO
{
    short nmeshes;
    short mesh_index;
    int bone_index;
    short* frame_base;
    LPVOID initialise;
    LPVOID control;
    LPVOID floor;
    LPVOID ceiling;
	LPDRAW draw_routine;
    LPVOID collision;
    short object_mip;
    short anim_index;
    short hit_points;
    short pivot_length;
    short radius;
    short shadow_size;
    unsigned short bit_offset;
    unsigned short loaded : 1;
    unsigned short intelligent : 1;
    unsigned short non_lot : 1;                // can fly correctly
    unsigned short save_position : 1;
    unsigned short save_hitpoints : 1;
    unsigned short save_flags : 1;
    unsigned short save_anim : 1;
    unsigned short semi_transparent : 1;
    unsigned short water_creature : 1;
    unsigned short using_drawanimating_item : 1;
    unsigned short hit_effect : 2;
    unsigned short undead : 1;
    unsigned short save_mesh : 1;
	unsigned short unknown : 2;
	LPDRAWEXTRA draw_routine_extra;
	unsigned int explodable_meshbits;
    int pad;
};

struct RANGE_STRUCT
{
    short start_frame;
    short end_frame;
    short link_anim_num;
    short link_frame_num;
};

struct CHANGE_STRUCT
{
    short anim_goal_state;
    short number_ranges;
    short range_index;
};

struct ANIM_STRUCT
{
    short* frame_ptr;
    short  interpolation;
    short  frame_size;
    short  current_anim_state;
    int   yspeed;
    int   yfallspeed;
    int   xspeed;
    int   xfallspeed;
    short  frame_base;
    short  frame_end;
    short  jump_anim_num;
    short  jump_frame_num;
    short  number_changes;
    short  change_index;
    short  number_commands;
    short  command_index;
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
	GAME_VECTOR src;
	GAME_VECTOR target;
	int type;
	int old_type;
	int shift;
	int flags;
	int fixed_camera;
	int number_frames;
	int bounce;
	int underwater;
	int distance;
	short target_angle;
	short target_elevation;
	short actual_angle;
	short actual_elevation;
	short number;
	short timer;
	short lara_node;
	short speed;
	ITEM_INFO* item;
	ITEM_INFO* last_item;
};

struct LARA_ARM
{
	short *frame_base;
	short frame_number;
	short anim_number;
	short y_rot;
	short x_rot;
	short z_rot;
	short lock;
	short flash_gun;
};

struct LARA_INFO
{
	short item_number;
	short gun_status;
	short gun_type;
	short gun_request_type;
	short gun_last_type;
	short calc_fallspeed;
	short water_status;
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
	short weapon_back_item;
	short flare_frame;
	short poison1;
	short poison2;
	unsigned char anxiety;
	unsigned char wet[15];
	unsigned short flags;
	int water_surface_dist;
	PHD_VECTOR last_pos;
	int* spaz_effect;
	int mesh_effect;
	short *meshes[15];
	ITEM_INFO* target;
	short target_angles[2];
	short turn_rate;
	short move_angle;
	short head_y_rot;
	short head_x_rot;
	short head_z_rot;
	short torso_y_rot;
	short torso_x_rot;
	short torso_z_rot;
	LARA_ARM left_arm;
	LARA_ARM right_arm;
	short holster;
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

struct GUNFLASH_STRUCT
{
	PHD_MATRIX matrix;
	short on;
};

struct DYNAMIC
{
	int x;
	int y;
	int z;
	unsigned char on;
	BYTE r;
	BYTE g;
	BYTE b;
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
	BYTE sSize;
	BYTE dSize;
	BYTE Size;
	BYTE friction;
	BYTE scalar;
	BYTE def;
	char rotadd;
	char maxyvel;
	BYTE on;
	BYTE sR;
	BYTE sG;
	BYTE sB;
	BYTE dR;
	BYTE dG;
	BYTE dB;
	BYTE R;
	BYTE G;
	BYTE B;
	BYTE colfadespeed;
	BYTE fadetoblack;
	BYTE slife;
	BYTE life;
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
	BYTE sSize;
	BYTE dSize;
	BYTE size;
	BYTE friction;
	BYTE scalar;
	BYTE def;
	char rotadd;
	char maxyvel;
	BYTE on;
	BYTE sShade;
	BYTE dShade;
	BYTE shade;
	BYTE colfadespeed;
	BYTE fadetoblack;
	char sLife;
	char life;
	BYTE transtype;
	BYTE fx_obj;
	BYTE node_number;
	BYTE mirror;
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
	short flags;
	BYTE sSize;
	BYTE dSize;
	BYTE size;
	BYTE friction;
	BYTE scalar;
	BYTE def;
	char rotadd;
	char maxyvel;
	BYTE on;
	BYTE sR;
	BYTE sG;
	BYTE sB;
	BYTE dR;
	BYTE dG;
	BYTE dB;
	BYTE r;
	BYTE g;
	BYTE b;
	BYTE colfadespeed;
	BYTE fadetoblack;
	BYTE sLife;
	BYTE life;
	BYTE transtype;
	BYTE extras;
	char dynamic;
	BYTE fx_obj;
	BYTE room_number;
	BYTE node_number;
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
	BYTE sSize;
	BYTE dSize;
	BYTE size;
	BYTE friction;
	char rotadd;
	BYTE on;
	BYTE sShade;
	BYTE dShade;
	BYTE shade;
	BYTE colfadespeed;
	BYTE fadetoblack;
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
	BYTE life;
};

struct BUBBLE_STRUCT
{
	PHD_VECTOR pos;
	short room_number;
	short speed;
	short size;
	short dSize;
	BYTE shade;
	BYTE vel;
	BYTE y_rot;
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
	BYTE on;
	BYTE r;
	BYTE g;
	BYTE b;
	short yvel;
	BYTE gravity;
	BYTE life;
	short room_number;
	BYTE outside;
	BYTE pad;
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
	BYTE r;
	BYTE g;
	BYTE b;
	BYTE life;
	short speed;
	short temp;
};


struct RIPPLE_STRUCT
{
	int x;
	int y;
	int z;
	char flags;
	BYTE life;
	BYTE size;
	BYTE init;
};

struct DEBRIS_STRUCT
{
	LPVOID textinfo;
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
	BYTE on;
	BYTE x_rot;
	BYTE y_rot;
	BYTE r;
	BYTE g;
	BYTE b;
	BYTE pad[22];
};

struct FIRE_LIST
{
	int x;
	int y;
	int z;
	BYTE on;
	BYTE size;
	short room_number;
};

struct BOX_NODE
{
	short exit_box;
	WORD search_number;
	short next_expansion;
	short box_number;
};

struct BOX_INFO
{
	BYTE left;
	BYTE right;
	BYTE top;
	BYTE bottom;
	short height;
	short overlap_index;
};

struct AI_INFO
{
	short zone_number; // NUMBER
	short enemy_zone; // NUMBER
	int distance;
	int ahead;
	BOOL bite;
	short angle;
	short x_angle;
	short enemy_facing;
};

struct LOT_INFO
{
	BOX_NODE *node;
	short head;
	short tail;
	WORD search_number;
	WORD block_mask;
	short step;
	short drop;
	short zone_count;
	short target_box;
	short required_box;
	short fly;
	WORD can_jump : 1;
	WORD can_monkey : 1;
	WORD is_amphibious : 1;
	WORD is_jumping : 1;
	WORD is_monkeying : 1;
	PHD_VECTOR target;
	int zone;
};

struct CREATURE_INFO
{
	short joint_rotation[4];
	short maximum_turn;
	short flags;
	WORD alerted : 1;
	WORD head_left : 1;
	WORD head_right : 1;
	WORD reached_goal : 1;
	WORD hurt_by_lara : 1;
	WORD patrol2 : 1;
	WORD jump_ahead : 1;
	WORD monkey_ahead : 1;
	MOOD_TYPE mood;
	ITEM_INFO *enemy;
	ITEM_INFO ai_target;
	short pad;
	short item_number;
	PHD_VECTOR target;
	LOT_INFO LOT;
};

#pragma pack(pop)