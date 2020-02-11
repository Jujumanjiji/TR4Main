#pragma once
#pragma pack(push, 1)

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
    char floor;
    unsigned char sky_room;
    char ceiling;
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
    unsigned short room_number;
    short point[3];
    short corner[12];
};

struct DOORS
{
    unsigned short number_door;
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
    int floor;
    int touch_bits;
    int mesh_bits;
    short object_number;
    short state_current;
    short state_next;
    short state_required;
    short current_anim;
    short current_frame;
    short room_number;
    short next_item;
    short next_free;
    short speed;
    short fallspeed;
    short hit_points;
    unsigned short box_number;
    short timer;
    short flags;                       // For oneshot and code switches (i.e. NOT flags)
    short shade;
    short ocb_bits;
    short carried_item;
    short after_death;
    unsigned short fired_weapon;
    short reserved_1;
    short reserved_2;
    short reserved_3;
    short reserved_4;
    void* data;
    PHD_3DPOS pos;
    unsigned char light_data[5528];
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

typedef void(*LPINITIALISE)(short item_number);
typedef void(*LPCONTROL)(short item_number);
typedef void(*LPFLOOR)(ITEM_INFO* item, int x, int y, int z, int* height);
typedef void(*LPCEILING)(ITEM_INFO* item, int x, int y, int z, int* height);
typedef void(*LPDRAW)(ITEM_INFO* item);
typedef void(*LPCOLLISION)(short item_number, ITEM_INFO* laraitem, COLL_INFO* coll);
typedef void(*LPDRAWEXTRA)(ITEM_INFO* item);

struct OBJECT_INFO
{
    short nmeshes;
    short mesh_index;
    int bone_index;
    short* frame_base;
    LPINITIALISE initialise;
    LPCONTROL control;
    LPFLOOR floor;
    LPCEILING ceiling;
    LPDRAW draw_routine;
    LPCOLLISION collision;
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
    unsigned int pad;
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
    unsigned char interpolation;
    unsigned char frame_size;
    short current_anim_state;
    int yspeed;
    int yfallspeed;
    int xspeed;
    int xfallspeed;
    short frame_base;
    short frame_end;
    short jump_anim_num;
    short jump_frame_num;
    short number_changes;
    short change_index;
    short number_commands;
    short command_index;
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
    OBJECT_VECTOR* fixed;
    int dword_7FE880;
    int dword_7FE884;
    int dword_7FE888;
    int mike_at_lara;
    PHD_VECTOR mike_pos;
    short mike_pos_room;
};

struct LARA_ARM
{
    short *frame_base;
    short frame_curr;
    short anim_curr;
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
    short current_frame;
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
    unsigned char sR;
    unsigned char sG;
    unsigned char sB;
    unsigned char dR;
    unsigned char dG;
    unsigned char dB;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char colfadespeed;
    unsigned char fadetoblack;
    unsigned char sLife;
    unsigned char life;
    unsigned char transtype;
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
    short zone_number; // NUMBER
    short enemy_zone; // NUMBER
    int distance;
    int ahead;
    int bite;
    short angle;
    short x_angle;
    short enemy_facing;
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
    short joint_rotation[4];
    short maximum_turn;
    short flags;
    unsigned short alerted : 1;
    unsigned short head_left : 1;
    unsigned short head_right : 1;
    unsigned short reached_goal : 1;
    unsigned short hurt_by_lara : 1;
    unsigned short patrol2 : 1;
    unsigned short jump_ahead : 1;
    unsigned short monkey_ahead : 1;
    MOOD_TYPE mood;
    ITEM_INFO *enemy;
    ITEM_INFO ai_target;
    short pad;
    short item_number;
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

#pragma pack(pop)