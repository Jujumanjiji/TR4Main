#pragma once

enum OPTION_TABLE_VALUE
{
    OPTION_CROSSBOW = 0x80,
    OPTION_GRENADEGUN = 0x100,
    OPTION_UZIS = 0x200,
    OPTION_PISTOLS = 0x400,
    OPTION_REVOLVER = 0x800,
};


extern void construct_inventory_2D(void);

#ifdef DLL_INJECT
#define show_inventory ((int(__cdecl*)(void)) 0x0043B760)
///#define construct_inventory_2D ((void(__cdecl*)(void)) 0x0043B9B0)
#define do_debounced_input ((void(__cdecl*)(void)) 0x0043BC30)
#define DrawThreeDeeObject2D ((void(__cdecl*)(int x, int y, int num, int shade, int xrot, int yrot, int zrot, int bright, int overlay)) 0x0043BD80)
#define DrawInventoryItem ((void(__cdecl*)(ITEM_INFO *item, int shade, int overlay, int shadflag)) 0x0043BF10)
#define go_and_load_game ((int(__cdecl*)(void)) 0x0043C400)
#define go_and_save_game ((int(__cdecl*)(void)) 0x0043C410)
#define construct_combine_object_list ((void(__cdecl*)(void)) 0x0043C420)
#define insert_object_into_list_v2 ((void(__cdecl*)(short inv_item)) 0x0043C5A0)
#define construct_object_list ((void(__cdecl*)(void)) 0x0043C610)
#define insert_object_into_list ((void(__cdecl*)(short inv_item)) 0x0043CA90)
#define draw_current_object_list ((void(__cdecl*)(int ringnum)) 0x0043CAE0)
#define handle_object_changeover ((void(__cdecl*)(void)) 0x0043D450)
#define setup_ammo_selector ((void(__cdecl*)(void)) 0x0043D470)
#define fade_ammo_selector ((void(__cdecl*)(void)) 0x0043DE30)
#define draw_ammo_selector ((void(__cdecl*)(void)) 0x0043DED0)
#define spinback ((void(__cdecl*)(WORD angle)) 0x0043E140)
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
#define have_i_got_object ((void(__cdecl*)(short objNumber)) 0x0043EF60)
#define search_detector ((void(__cdecl*)(short objNumber)) 0x0043F050)
#define convert_obj_to_invobj ((void(__cdecl*)(short objNumber)) 0x0043F150)
#define do_compass_mode ((void(__cdecl*)(void)) 0x0043F180)
#define do_examine_mode ((void(__cdecl*)(void)) 0x0043F1E0)
#define give_lara_items_cheat ((void(__cdecl*)(void)) 0x0043F4C0)
#endif
