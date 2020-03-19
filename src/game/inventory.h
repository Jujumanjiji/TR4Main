#pragma once

enum INVENTORY_ITEM_ID
{
    INV_UZI,
    INV_PISTOLS,
    INV_SHOTGUN,
    INV_REVOLVER,
    INV_REVOLVER_LASER,
    INV_CROSSBOW,
    INV_CROSSBOW_LASER,
    INV_GRENADEGUN,
    INV_SHOTGUN_AMMO1,
    INV_SHOTGUN_AMMO2,
    INV_GRENADEGUN_AMMO1,
    INV_GRENADEGUN_AMMO2,
    INV_GRENADEGUN_AMMO3,
    INV_CROSSBOW_AMMO1,
    INV_CROSSBOW_AMMO2,
    INV_CROSSBOW_AMMO3,
    INV_REVOLVER_AMMO,
    INV_UZI_AMMO,
    INV_PISTOLS_AMMO,
    INV_LASERSIGHT,
    INV_BIGMEDI,
    INV_SMALLMEDI,
    INV_BINOCULARS,
    INV_FLARE,
    INV_COMPASS,
    INV_MEMCARD_LOAD,
    INV_MEMCARD_SAVE,
    INV_WATERSKIN_EMPTY,
    INV_WATERSKIN1_1,
    INV_WATERSKIN1_2,
    INV_WATERSKIN1_3,
    INV_WATERSKIN2_EMPTY,
    INV_WATERSKIN2_1,
    INV_WATERSKIN2_2,
    INV_WATERSKIN2_3,
    INV_WATERSKIN2_4,
    INV_WATERSKIN2_5,
    INV_PUZZLE1,
    INV_PUZZLE2,
    INV_PUZZLE3,
    INV_PUZZLE4,
    INV_PUZZLE5,
    INV_PUZZLE6,
    INV_PUZZLE7,
    INV_PUZZLE8,
    INV_PUZZLE9,
    INV_PUZZLE10,
    INV_PUZZLE11,
    INV_PUZZLE12,
    INV_PUZZLE1_COMBO1,
    INV_PUZZLE1_COMBO2,
    INV_PUZZLE2_COMBO1,
    INV_PUZZLE2_COMBO2,
    INV_PUZZLE3_COMBO1,
    INV_PUZZLE3_COMBO2,
    INV_PUZZLE4_COMBO1,
    INV_PUZZLE4_COMBO2,
    INV_PUZZLE5_COMBO1,
    INV_PUZZLE5_COMBO2,
    INV_PUZZLE6_COMBO1,
    INV_PUZZLE6_COMBO2,
    INV_PUZZLE7_COMBO1,
    INV_PUZZLE7_COMBO2,
    INV_PUZZLE8_COMBO1,
    INV_PUZZLE8_COMBO2,
    INV_KEY1,
    INV_KEY2,
    INV_KEY3,
    INV_KEY4,
    INV_KEY5,
    INV_KEY6,
    INV_KEY7,
    INV_KEY8,
    INV_KEY9,
    INV_KEY10,
    INV_KEY11,
    INV_KEY12,
    INV_KEY1_COMBO1,
    INV_KEY1_COMBO2,
    INV_KEY2_COMBO1,
    INV_KEY2_COMBO2,
    INV_KEY3_COMBO1,
    INV_KEY3_COMBO2,
    INV_KEY4_COMBO1,
    INV_KEY4_COMBO2,
    INV_KEY5_COMBO1,
    INV_KEY5_COMBO2,
    INV_KEY6_COMBO1,
    INV_KEY6_COMBO2,
    INV_KEY7_COMBO1,
    INV_KEY7_COMBO2,
    INV_KEY8_COMBO1,
    INV_KEY8_COMBO2,
    INV_PICKUP1,
    INV_PICKUP2,
    INV_PICKUP3,
    INV_PICKUP4,
    INV_PICKUP1_COMBO1,
    INV_PICKUP1_COMBO2,
    INV_PICKUP2_COMBO1,
    INV_PICKUP2_COMBO2,
    INV_PICKUP3_COMBO1,
    INV_PICKUP3_COMBO2,
    INV_PICKUP4_COMBO1,
    INV_PICKUP4_COMBO2,
    INV_QUEST1,
    INV_QUEST2,
    INV_QUEST3,
    INV_QUEST4,
    INV_QUEST5,
    INV_QUEST6,
    INV_BURNING_TORCH,
    INV_CROWBAR,
    INV_CLOCKWORK_BEETLE,
    INV_CLOCKWORK_BEETLE_COMBO1,
    INV_CLOCKWORK_BEETLE_COMBO2,
    INV_EXAMINE1,
    INV_EXAMINE2,
    INV_EXAMINE3,
    MAX_INVOBJ,
};

enum COMBINE_ITEM
{
    COMBINE_REVOLVER,
    COMBINE_CROSSBOW,
    COMBINE_PUZZLE1,
    COMBINE_PUZZLE2,
    COMBINE_PUZZLE3,
    COMBINE_PUZZLE4,
    COMBINE_PUZZLE5,
    COMBINE_PUZZLE6,
    COMBINE_PUZZLE7,
    COMBINE_PUZZLE8,
    COMBINE_KEY1,
    COMBINE_KEY2,
    COMBINE_KEY3,
    COMBINE_KEY4,
    COMBINE_KEY5,
    COMBINE_KEY6,
    COMBINE_KEY7,
    COMBINE_KEY8,
    COMBINE_PICKUP1,
    COMBINE_PICKUP2,
    COMBINE_PICKUP3,
    COMBINE_PICKUP4,
    COMBINE_CLOCKWORK_BEETLE,
    MAX_COMBINE,
};

enum INVENTORY_RING_TYPE
{
    RING_INVENTORY = 0,
    RING_COMBINE = 1,
    RING_AMMO = 1,
    MAX_RING = 2,
};

enum CLOCKWORK_BEETLE_FLAGS
{
    CLOCK_PRESENT = 0x1,
    CLOCK_COMBO1_PRESENT = 0x2,
    CLOCK_COMBO2_PRESENT = 0x4,
};

enum INV_AMMO_TYPE
{
    AINV_AMMO1,
    AINV_AMMO2,
    AINV_AMMO3
};

enum TABLE_OPTION_TYPE
{
    T_NONE,
    T_USE,
    T_CHOOSE_AMMO,
    T_COMBINE,
    T_SEPARATE,
    T_EQUIP,
    T_INVAMMO1,
    T_INVAMMO2,
    T_INVAMMO3,
    T_LOAD,
    T_SAVE,
    T_EXAMINE,
    MAX_TABLE_TYPE
};

enum TABLE_OPTION
{
    OPTION_COMBINE_ITEM = 0x1,
    OPTION_EQUIP = 0x2,
    OPTION_USE = 0x4,
    OPTION_COMBINE = 0x8,
    OPTION_SEPARATE = 0x10,
    OPTION_EXAMINE = 0x20,
    OPTION_SHOTGUN = 0x40,
    OPTION_CROSSBOW = 0x80,
    OPTION_GRENADEGUN = 0x100,
    OPTION_UZIS = 0x200,
    OPTION_PISTOLS = 0x400,
    OPTION_REVOLVER = 0x800,
    OPTION_LOAD = 0x1000,
    OPTION_SAVE = 0x2000
};

enum MENU_INVENTORY
{
    MENU_1, // use, examine, load, save
    MENU_2, // choose_ammo, combine, separate
    MENU_3, // combine, separate
    MAX_MENU
};

#pragma pack(push, 1)
struct COMBINELIST
{
    void(*combine_routine)(int flag);
    short item_a;
    short item_b;
    short result;
};

struct INVOBJ
{
    short object_number;
    short yoff;
    short scale;
    short yrot;
    short xrot;
    short zrot;
    short flags;
    short name;
    DWORD meshbits;
};

struct INVLIST
{
    short inv_item;
    WORD yrot;
    WORD bright;
};

struct RING2D
{
    INVLIST current_obj_list[MAX_INVOBJ];
    BOOL active;
    int obj_list_movement;
    int cur_obj_in_list;
    int num_obj_in_list;
};

struct AMMOLIST
{
    short inv_item;
    short amount;
    WORD yrot;
};

struct MENU
{
    int type;
    char* text;
};
#pragma pack(pop)

extern RING2D* ring_2d[MAX_RING];
extern MENU current_options[MAX_MENU];
extern short options_table[MAX_INVOBJ];
extern COMBINELIST combine_table[MAX_COMBINE];
extern DWORD inventory_light;
extern float inventory_drawX;
extern float inventory_drawY;
extern int inventory_xpos;
extern int inventory_ypos;
extern int compass_needle_angle;
extern int objlist_spacing;
extern int combine_obj1;
extern int combine_obj2;
extern short normal_ring_fade_val;
extern short normal_ring_fade_dir;
extern short ammo_selector_fade_val;
extern short ammo_selector_fade_dir;
extern short combine_ring_fade_val;
extern short combine_ring_fade_dir;
extern short examine_mode;
extern short stats_mode;
extern short inv_pistols_ammo_count;
extern short inv_shotgun_ammo1_count;
extern short inv_shotgun_ammo2_count;
extern short inv_uzi_ammo_count;
extern short inv_grenadegun_ammo1_count;
extern short inv_grenadegun_ammo2_count;
extern short inv_grenadegun_ammo3_count;
extern short inv_crossbow_ammo1_count;
extern short inv_crossbow_ammo2_count;
extern short inv_crossbow_ammo3_count;
extern short inv_revolver_ammo_count;
extern char current_selected_option;
extern char num_ammo_slots;
extern char current_ammo_type;
extern char combine_type_flag;
extern char ammo_selector_flag;
extern char seperate_type_flag;
extern char old_lara_busy;
extern char left_repeat;
extern char right_repeat;
extern char loading_or_saving;
extern char current_pistols_ammo_type;
extern char current_shotgun_ammo_type;
extern char current_uzi_ammo_type;
extern char current_grenadegun_ammo_type;
extern char current_crossbow_ammo_type;
extern char current_revolver_ammo_type;
extern char save_current_selection;
extern char save_pistols_ammo_type;
extern char save_shotgun_ammo_type;
extern char save_uzi_ammo_type;
extern char save_grenadegun_ammo_type;
extern char save_crossbow_ammo_type;
extern char save_revolver_ammo_type;
extern bool menu_active;
extern bool ammo_active;
extern bool left_debounce;
extern bool right_debounce;
extern bool up_debounce;
extern bool down_debounce;
extern bool go_left;
extern bool go_right;
extern bool go_up;
extern bool go_down;
extern bool select_debounce;
extern bool deselect_debounce;
extern bool go_select;
extern bool go_deselect;
extern bool use_items;
extern bool friggrimmer1;
extern bool friggrimmer2;

#define OPTION_ALL_WEAPONS (OPTION_PISTOLS|OPTION_SHOTGUN|OPTION_GRENADEGUN|OPTION_CROSSBOW|OPTION_REVOLVER|OPTION_UZIS)

#ifdef DLL_INJECT
///#define old_lara_busy                       VAR_U_(0x007FE9C0, char)
#define in_inventory                        VAR_I_(0x004BF3C8, int, 0)
///#define current_selected_option             VAR_U_(0x007FEA1B, char)
#define compass_needle                      VAR_U_(0x007E71DC, int)
#define GLOBAL_invitemlastchosen            VAR_I_(0x004AE064, int, NO_ITEM)
#define GLOBAL_invEnter                     VAR_I_(0x004AE068, int, NO_ITEM)
#define GLOBAL_invitemchosen                VAR_I_(0x004AE06C, int, NO_ITEM)
#define inventry_objects_list               ARRAY_(0x004AE070, INVOBJ, [MAX_INVOBJ])
///#define ring_2d                              ARRAY_(0x007FEA20, RING2D*, [MAX_RING])
///#define combine_table                       ARRAY_(0x004AEAC0, COMBINELIST, [22])
#define ammo_object_list                    ARRAY_(0x007FEA00, AMMOLIST, [3])
///#define current_options                     ARRAY_(0x007FE9E0, MENU, [3])
///#define options_table                       ARRAY_(0x004AE9BC, short, [129])
#define option_name                            ARRAY_(0x004AEAAA, short, [9])
///#define objlist_spacing                     VAR_U_(0x007FE9BC, int)
///#define inventory_xpos                      VAR_I_(0x004BF3D0, int, 0)
///#define inventory_ypos                      VAR_I_(0x004BF3D2, int, 0)
///#define inventory_drawX                     VAR_U_(0x007FE9CC, float)
///#define inventory_drawY                     VAR_U_(0x007FEA2C, float)
///#define inventory_light                     VAR_I_(0x004AE060, DWORD, RGBA_VECTORGET(127, 127, 127)) // r: 127, g: 127, b: 127
///#define compass_needle_angle                VAR_U_(0x007FEA50, int)
///#define examine_mode                        VAR_I_(0x004BF3CC, short, 0)
///#define stats_mode                          VAR_U_(0x007FEA18, short)
///#define left_debounce                       VAR_U_(0x007FE9C9, bool)
///#define right_debounce                      VAR_U_(0x007FEA38, bool)
///#define up_debounce                         VAR_U_(0x007FEA4C, bool)
///#define down_debounce                       VAR_U_(0x007FE9F8, bool)
///#define go_left                             VAR_U_(0x007FE9C1, bool)
///#define go_right                            VAR_U_(0x007FE9CA, bool)
///#define go_up                               VAR_U_(0x007FEA45, bool)
///#define go_down                             VAR_U_(0x007FEA4B, bool)
///#define select_debounce                     VAR_U_(0x007FEA43, bool)
///#define deselect_debounce                   VAR_U_(0x007FEA12, bool)
///#define go_select                           VAR_U_(0x007FF022, bool)
///#define go_deselect                         VAR_U_(0x007FEA54, bool)
///#define left_repeat                         VAR_U_(0x007FF021, char)
///#define right_repeat                        VAR_U_(0x007FEA39, char)
///#define loading_or_saving                   VAR_U_(0x007FE9CB, char)
///#define use_items                           VAR_U_(0x007FEA44, bool)
///#define num_ammo_slots                      VAR_U_(0x007FE9D0, char)
///#define inv_pistols_ammo_count              VAR_I_(0x004BF3C0, short, 0)
///#define inv_shotgun_ammo1_count             VAR_I_(0x004BF3AE, short, 0)
///#define inv_shotgun_ammo2_count             VAR_I_(0x004BF3AC, short, 0)
///#define inv_uzi_ammo_count                  VAR_I_(0x004BF3C4, short, 0)
///#define inv_grenadegun_ammo1_count          VAR_I_(0x004BF3AA, short, 0)
///#define inv_grenadegun_ammo2_count          VAR_I_(0x004BF3A6, short, 0)
///#define inv_grenadegun_ammo3_count          VAR_I_(0x004BF3A8, short, 0)
///#define inv_crossbow_ammo1_count            VAR_I_(0x004BF3B6, short, 0)
///#define inv_crossbow_ammo2_count            VAR_I_(0x004BF3B4, short, 0)
///#define inv_crossbow_ammo3_count            VAR_I_(0x004BF3B8, short, 0)
///#define inv_revolver_ammo_count             VAR_I_(0x004BF3BE, short, 0)
///#define current_ammo_type                   VAR_U_(0x007FEA3C, char*)
///#define current_pistols_ammo_type           VAR_I_(0x004BF3B0, char, 0)
///#define current_shotgun_ammo_type           VAR_I_(0x004BF3B2, char, 0)
///#define current_uzi_ammo_type               VAR_I_(0x004BF3BB, char, 0)
///#define current_grenadegun_ammo_type        VAR_I_(0x004BF3A4, char, 0)
///#define current_crossbow_ammo_type          VAR_I_(0x004BF3C6, char, 0)
///#define current_revolver_ammo_type          VAR_I_(0x004BF3B1, char, 0)
///#define ammo_selector_flag                  VAR_U_(0x007FEA4A, char)
///#define ammo_selector_fade_val              VAR_U_(0x007FEA2A, short)
///#define ammo_selector_fade_dir              VAR_U_(0x007FEA48, short)
///#define combine_ring_fade_val               VAR_U_(0x007FEA16, short)
///#define combine_ring_fade_dir               VAR_U_(0x007FEA40, short)
///#define combine_type_flag                   VAR_U_(0x007FF020, char)
///#define seperate_type_flag                  VAR_U_(0x007FEA1A, char)
///#define combine_obj1                        VAR_U_(0x007FEA34, int)
///#define combine_obj2                        VAR_U_(0x007FEA30, int)
///#define normal_ring_fade_val                VAR_U_(0x007FEA28, short)
///#define normal_ring_fade_dir                VAR_U_(0x007FEA46, short)
///#define menu_active                         VAR_U_(0x007FE9C8, bool)
///#define ammo_active                         VAR_U_(0x007FEA42, bool)
///#define friggrimmer1                        VAR_U_(0x007FEA14, bool)
///#define friggrimmer2                        VAR_U_(0x007FEA13, bool)
///#define save_pistol_ammo_type               VAR_I_(0x004BF3BC, char, 0)
///#define save_uzi_ammo_type                  VAR_I_(0x004BF3C4, char, 0)
///#define save_revolver_ammo_type             VAR_I_(0x004BF3BE, char, 0)
///#define save_shotgun_ammo_type              VAR_I_(0x004BF3C2, char, 0)
///#define save_grenadegun_ammo_type           VAR_I_(0x004BF3BA, char, 0)
///#define save_crossbow_ammo_type             VAR_I_(0x004BF3BD, char, 0)
///#define save_current_selection              VAR_I_(0x007FEA4D, char, 0)

#define GadwPolygonBuffers_RingNormal       VAR_U_(0x007FED40, RING2D)
#define GadwPolygonBuffers_RingCombine      VAR_U_(0x007FEA60, RING2D)
#endif

#define CONSTRUCT_ITEM 0
#define DECONSTRUCT_ITEM 1
#define INVENTORY_INPUT_REPEAT 8
#define INV_COMBO1 3
#define INV_COMBO2 12
#define INV_COMBO3 48
#define INV_COMBO4 192
#define INV_COMBO5 768
#define INV_COMBO6 3072
#define INV_COMBO7 12288
#define INV_COMBO8 49152

extern int show_inventory_2d(void);
extern void construct_inventory_2d(void);
extern void do_debounced_input(void);
extern void DrawThreeDeeObject2D(int x, int y, int num, int shade, int xrot, int yrot, int zrot, int bright, int overlay);
extern void DrawInventoryItem(ITEM_INFO *item, int shade, int overlay, BOOL shade_flags);
extern int go_and_load_game(void);
extern int go_and_save_game(void);
extern void construct_combine_object_list(void);
extern void insert_object_into_list_combine(short num);
extern void construct_object_list(void);
extern void insert_object_into_list_inventory(short num);
extern void draw_current_object_list(int ring_type); // TODO: need to change the item counter (inventory) !!!!
extern void handle_object_changeover(void);
extern void handle_inventry_menu(void);
extern void fade_ammo_selector(void);
extern void draw_ammo_selector(void);
extern void spinback(WORD* yangle);
extern void update_laras_weapons_status(void);
extern BOOL is_item_currently_combinable(short inv_item);
extern BOOL have_i_got_item(short inv_item);
extern BOOL do_these_objects_combine(short item_a, short item_b);
extern void combine_these_two_objects(short item_a, short item_b);
extern void seperate_object(short inv_item);
extern void combine_revolver(int flag);
extern void combine_crossbow(int flag);
extern void combine_puzzle_item1(int flag);
extern void combine_puzzle_item2(int flag);
extern void combine_puzzle_item3(int flag);
extern void combine_puzzle_item4(int flag);
extern void combine_puzzle_item5(int flag);
extern void combine_puzzle_item6(int flag);
extern void combine_puzzle_item7(int flag);
extern void combine_puzzle_item8(int flag);
extern void combine_key_item1(int flag);
extern void combine_key_item2(int flag);
extern void combine_key_item3(int flag);
extern void combine_key_item4(int flag);
extern void combine_key_item5(int flag);
extern void combine_key_item6(int flag);
extern void combine_key_item7(int flag);
extern void combine_key_item8(int flag);
extern void combine_pickup_item1(int flag);
extern void combine_pickup_item2(int flag);
extern void combine_pickup_item3(int flag);
extern void combine_pickup_item4(int flag);
extern void combine_clockwork(int flag);
extern void combine_waterskin(int flag);
extern void setup_objectlist_startposition_invitem(short inv_item);
extern void setup_objectlist_startposition_objnumber(short object_number);
extern void use_current_item(void);
extern void picked_up_object(short object_number);
extern BOOL have_i_got_object(short object_number);
extern int convert_obj_to_invobj(short object_number);
extern void do_compass_mode(void);
extern void do_examine_mode(void);