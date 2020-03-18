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
    INV_CLOCKWORK_BEETLE_COMBO2,
    INV_CLOCKWORK_BEETLE_COMBO1,
    INV_EXAMINE1,
    INV_EXAMINE2,
    INV_EXAMINE3,
    MAX_INVOBJ,
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

enum OPTION_TABLE_VALUE
{
    OPTION_SHOTGUN = 0x40,
    OPTION_CROSSBOW = 0x80,
    OPTION_GRENADEGUN = 0x100,
    OPTION_UZIS = 0x200,
    OPTION_PISTOLS = 0x400,
    OPTION_REVOLVER = 0x800,
    OPTION_ALL = (OPTION_PISTOLS|OPTION_SHOTGUN|OPTION_GRENADEGUN|OPTION_CROSSBOW|OPTION_REVOLVER|OPTION_UZIS)
};

#define current_selected_option             VAR_U_(0x007FEA1B, BYTE)
#define compass_needle                      VAR_U_(0x007E71DC, int)
#define GLOBAL_invitemlastchosen            VAR_I_(0x004AE064, int, NO_ITEM)
#define GLOBAL_invEnter                     VAR_I_(0x004AE068, int, NO_ITEM)
#define GLOBAL_invitemchosen                VAR_I_(0x004AE06C, int, NO_ITEM)
#define inventry_objects_list               ARRAY_(0x004AE070, INVOBJ, [MAX_INVOBJ])
#define ring2D                              ARRAY_(0x007FEA20, RING2D*, [MAX_RING])
#define combine_table                       ARRAY_(0x004AEAC0, COMBINELIST, [19])
#define ammo_object_list                    ARRAY_(0x007FEA00, AMMOLIST, [3])
#define current_options                     ARRAY_(0x007FE9E0, MENU, [3])
#define options_table                       ARRAY_(0x004AE9BC, short, [129])
#define objlist_spacing                     VAR_U_(0x007FE9BC, int)
#define inventory_xpos                      VAR_I_(0x004BF3D0, int, 0)
#define inventory_ypos                      VAR_I_(0x004BF3D2, int, 0)
#define inventory_drawX                     VAR_U_(0x007FE9CC, float)
#define inventory_drawY                     VAR_U_(0x007FEA2C, float)
#define inventory_light                     VAR_I_(0x004AE060, DWORD, RGBA_VECTORGET(127, 127, 127)) // r: 127, g: 127, b: 127
#define inventory_camera_angle              VAR_U_(0x007FEA50, int)
#define examine_mode                        VAR_I_(0x004BF3CC, BOOL, FALSE)
#define stats_mode                          VAR_U_(0x007FEA18, short)
#define left_debounce                       VAR_U_(0x007FE9C9, bool)
#define right_debounce                      VAR_U_(0x007FEA38, bool)
#define up_debounce                         VAR_U_(0x007FEA4C, bool)
#define down_debounce                       VAR_U_(0x007FE9F8, bool)
#define go_left                             VAR_U_(0x007FE9C1, bool)
#define go_right                            VAR_U_(0x007FE9CA, bool)
#define go_up                               VAR_U_(0x007FEA45, bool)
#define go_down                             VAR_U_(0x007FEA4B, bool)
#define select_debounce                     VAR_U_(0x007FEA43, bool)
#define deselect_debounce                   VAR_U_(0x007FEA12, bool)
#define go_select                           VAR_U_(0x007FF022, bool)
#define go_deselect                         VAR_U_(0x007FEA54, bool)
#define left_repeat                         VAR_U_(0x007FF021, char)
#define right_repeat                        VAR_U_(0x007FEA39, char)
#define loading_or_saving                   VAR_U_(0x007FE9CB, bool)
#define use_items                           VAR_U_(0x007FEA44, bool)
#define num_ammo_slots                      VAR_U_(0x007FE9D0, char)
#define inv_pistols_ammo_count              VAR_I_(0x004BF3C0, short, 0)
#define inv_shotgun_ammo1_count             VAR_I_(0x004BF3AE, short, 0)
#define inv_shotgun_ammo2_count             VAR_I_(0x004BF3AC, short, 0)
#define inv_uzi_ammo_count                  VAR_I_(0x004BF3C4, short, 0)
#define inv_grenadegun_ammo1_count          VAR_I_(0x004BF3AA, short, 0)
#define inv_grenadegun_ammo2_count          VAR_I_(0x004BF3A6, short, 0)
#define inv_grenadegun_ammo3_count          VAR_I_(0x004BF3A8, short, 0)
#define inv_crossbow_ammo1_count            VAR_I_(0x004BF3B6, short, 0)
#define inv_crossbow_ammo2_count            VAR_I_(0x004BF3B4, short, 0)
#define inv_crossbow_ammo3_count            VAR_I_(0x004BF3B8, short, 0)
#define inv_revolver_ammo_count             VAR_I_(0x004BF3BE, short, 0)
#define current_ammo_type                   VAR_U_(0x007FEA3C, char*)
#define current_pistols_ammo_type           VAR_I_(0x004BF3B0, char, 0)
#define current_shotgun_ammo_type           VAR_I_(0x004BF3B2, char, 0)
#define current_uzi_ammo_type               VAR_I_(0x004BF3BB, char, 0)
#define current_grenadegun_ammo_type        VAR_I_(0x004BF3A4, char, 0)
#define current_crossbow_ammo_type          VAR_I_(0x004BF3C6, char, 0)
#define current_revolver_ammo_type          VAR_I_(0x004BF3B1, char, 0)
#define ammo_selector_flag                  VAR_U_(0x007FEA4A, char)
#define ammo_selector_fade_val              VAR_U_(0x007FEA2A, short)
#define ammo_selector_fade_dir              VAR_U_(0x007FEA48, short)
#define combine_ring_fade_val               VAR_U_(0x007FEA16, short)
#define combine_ring_fade_dir               VAR_U_(0x007FEA40, short)
#define combine_type_flag                   VAR_U_(0x007FF020, char)
#define seperate_type_flag                  VAR_U_(0x007FEA1A, char)
#define combine_obj1                        VAR_U_(0x007FEA34, int)
#define combine_obj2                        VAR_U_(0x007FEA30, int)
#define normal_ring_fade_val                VAR_U_(0x007FEA28, short)
#define normal_ring_fade_dir                VAR_U_(0x007FEA46, short)
#define menu_active                         VAR_U_(0x007FE9C8, bool)
#define ammo_active                         VAR_U_(0x007FEA42, bool)
#define friggrimmer1                        VAR_U_(0x007FEA14, bool)
#define friggrimmer2                        VAR_U_(0x007FEA13, bool)
#define save_pistol_ammo_type               VAR_I_(0x004BF3BC, char, 0)
#define save_uzi_ammo_type                  VAR_I_(0x004BF3C4, char, 0)
#define save_revolver_ammo_type             VAR_I_(0x004BF3BE, char, 0)
#define save_shotgun_ammo_type              VAR_I_(0x004BF3C2, char, 0)
#define save_grenadegun_ammo_type           VAR_I_(0x004BF3BA, char, 0)
#define save_crossbow_ammo_type             VAR_I_(0x004BF3BD, char, 0)
#define save_current_selection              VAR_I_(0x007FEA4D, char, 0)


#define INVENTORY_INPUT_REPEAT 8

extern void construct_inventory_2D(void);
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

#ifdef DLL_INJECT
#define show_inventory ((int(__cdecl*)(void)) 0x0043B760) // in the end !!
///#define construct_inventory_2D ((void(__cdecl*)(void)) 0x0043B9B0)
///#define do_debounced_input ((void(__cdecl*)(void)) 0x0043BC30)
///#define DrawThreeDeeObject2D ((void(__cdecl*)(int x, int y, int num, int shade, int xrot, int yrot, int zrot, int bright, int overlay)) 0x0043BD80)
///#define DrawInventoryItem ((void(__cdecl*)(ITEM_INFO *item, int shade, int overlay, BOOL shade_flags)) 0x0043BF10)
///#define go_and_load_game ((int(__cdecl*)(void)) 0x0043C400)
///#define go_and_save_game ((int(__cdecl*)(void)) 0x0043C410)
///#define construct_combine_object_list ((void(__cdecl*)(void)) 0x0043C420)
///#define insert_object_into_list_combine ((void(__cdecl*)(short num)) 0x0043C5A0)
///#define construct_object_list ((void(__cdecl*)(void)) 0x0043C610)
///#define insert_object_into_list_inventory ((void(__cdecl*)(short num)) 0x0043CA90)
///#define draw_current_object_list ((void(__cdecl*)(int ringnum)) 0x0043CAE0)
///#define handle_object_changeover ((void(__cdecl*)(void)) 0x0043D450)
///#define setup_ammo_selector ((void(__cdecl*)(void)) 0x0043D470)
#define fade_ammo_selector ((void(__cdecl*)(void)) 0x0043DE30)
#define draw_ammo_selector ((void(__cdecl*)(void)) 0x0043DED0)
#define spinback ((void(__cdecl*)(WORD* angle)) 0x0043E140)
#define update_laras_weapons_status ((void(__cdecl*)(void)) 0x0043E1B0)
#define is_item_currently_combinable ((BOOL(__cdecl*)(short objNumber)) 0x0043E250)
#define have_i_got_item ((BOOL(__cdecl*)(short objNumber)) 0x0043E2F0)
#define do_these_objects_combine ((BOOL(__cdecl*)(short item_a, short item_b)) 0x0043E320)
#define combine_these_two_objects ((void(__cdecl*)(short item_a, short item_b)) 0x0043E360)
#define seperate_object ((void(__cdecl*)(short objNumber)) 0x0043E3D0)
#define combine_revolver ((void(__cdecl*)(int flag)) 0x0043E420)
#define combine_crossbow ((void(__cdecl*)(int flag)) 0x0043E480)
#define combine_puzzle_item1 ((void(__cdecl*)(int flag)) 0x0043E4E0)
#define combine_puzzle_item2 ((void(__cdecl*)(int flag)) 0x0043E500)
#define combine_puzzle_item3 ((void(__cdecl*)(int flag)) 0x0043E520)
#define combine_puzzle_item4 ((void(__cdecl*)(int flag)) 0x0043E540)
#define combine_puzzle_item5 ((void(__cdecl*)(int flag)) 0x0043E560)
#define combine_puzzle_item6 ((void(__cdecl*)(int flag)) 0x0043E580)
#define combine_puzzle_item7 ((void(__cdecl*)(int flag)) 0x0043E5A0)
#define combine_puzzle_item8 ((void(__cdecl*)(int flag)) 0x0043E5C0)
#define combine_key_item1 ((void(__cdecl*)(int flag)) 0x0043E5E0)
#define combine_key_item2 ((void(__cdecl*)(int flag)) 0x0043E600)
#define combine_key_item3 ((void(__cdecl*)(int flag)) 0x0043E620)
#define combine_key_item4 ((void(__cdecl*)(int flag)) 0x0043E640)
#define combine_key_item5 ((void(__cdecl*)(int flag)) 0x0043E660)
#define combine_key_item6 ((void(__cdecl*)(int flag)) 0x0043E680)
#define combine_key_item7 ((void(__cdecl*)(int flag)) 0x0043E6A0)
#define combine_key_item8 ((void(__cdecl*)(int flag)) 0x0043E6C0)
#define combine_pickup_item1 ((void(__cdecl*)(int flag)) 0x0043E6E0)
#define combine_pickup_item2 ((void(__cdecl*)(int flag)) 0x0043E700)
#define combine_pickup_item3 ((void(__cdecl*)(int flag)) 0x0043E720)
#define combine_pickup_item4 ((void(__cdecl*)(int flag)) 0x0043E740)
#define combine_clockwork ((void(__cdecl*)(int flag)) 0x0043E760)
#define combine_waterskin ((void(__cdecl*)(int flag)) 0x0043E770)
#define setup_objectlist_startposition2 ((void(__cdecl*)(short inv_item)) 0x0043E830)
#define setup_objectlist_startposition ((void(__cdecl*)(short inv_item)) 0x0043E860)
#define use_current_item ((void(__cdecl*)(void)) 0x0043E8A0)
#define picked_up_object ((void(__cdecl*)(short objNumber)) 0x0043EB80)
#define have_i_got_object ((BOOL(__cdecl*)(short objNumber)) 0x0043EF60)
#define search_detector ((void(__cdecl*)(short objNumber)) 0x0043F050)
#define convert_obj_to_invobj ((void(__cdecl*)(short objNumber)) 0x0043F150)
#define do_compass_mode ((void(__cdecl*)(void)) 0x0043F180)
#define do_examine_mode ((void(__cdecl*)(void)) 0x0043F1E0)
#define give_lara_items_cheat ((void(__cdecl*)(void)) 0x0043F4C0)
#endif
