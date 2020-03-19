#include "framework.h"
#include "inventory.h"
#include "3d_gen.h"
#include "3d_gen_a.h"
#include "draw.h"
#include "gameflow.h"
#include "lara.h"
#include "input.h"
#include "sound.h"
#include "loadsave.h"
#include "text.h"
#include "output.h"
#include "libgpu.h"
#include "specific.h"
#include "utils.h"

RING2D* ring_2d[MAX_RING];
MENU current_options[MAX_MENU];

short options_table[MAX_INVOBJ] = {
    522, // INV_UZI
    (OPTION_PISTOLS|OPTION_EQUIP), // INV_PISTOLS
};

COMBINELIST combine_table[MAX_COMBINE] = {
    { combine_revolver, INV_REVOLVER, INV_LASERSIGHT, INV_REVOLVER_LASER },
    { combine_crossbow, INV_CROSSBOW, INV_LASERSIGHT, INV_CROSSBOW_LASER },
    { combine_puzzle_item1, INV_PUZZLE1_COMBO1, INV_PUZZLE1_COMBO2, INV_PUZZLE1 },
    { combine_puzzle_item2, INV_PUZZLE2_COMBO1, INV_PUZZLE2_COMBO2, INV_PUZZLE2 },
    { combine_puzzle_item3, INV_PUZZLE3_COMBO1, INV_PUZZLE3_COMBO2, INV_PUZZLE3 },
    { combine_puzzle_item4, INV_PUZZLE4_COMBO1, INV_PUZZLE4_COMBO2, INV_PUZZLE4 },
    { combine_puzzle_item5, INV_PUZZLE5_COMBO1, INV_PUZZLE5_COMBO2, INV_PUZZLE5 },
    { combine_puzzle_item6, INV_PUZZLE6_COMBO1, INV_PUZZLE6_COMBO2, INV_PUZZLE6 },
    { combine_puzzle_item7, INV_PUZZLE7_COMBO1, INV_PUZZLE7_COMBO2, INV_PUZZLE7 },
    { combine_puzzle_item8, INV_PUZZLE8_COMBO1, INV_PUZZLE8_COMBO2, INV_PUZZLE8 },
    { combine_key_item1, INV_KEY1_COMBO1, INV_KEY1_COMBO2, INV_KEY1 },
    { combine_key_item2, INV_KEY2_COMBO1, INV_KEY2_COMBO2, INV_KEY2 },
    { combine_key_item3, INV_KEY3_COMBO1, INV_KEY3_COMBO2, INV_KEY3 },
    { combine_key_item4, INV_KEY4_COMBO1, INV_KEY4_COMBO2, INV_KEY4 },
    { combine_key_item5, INV_KEY5_COMBO1, INV_KEY5_COMBO2, INV_KEY5 },
    { combine_key_item6, INV_KEY6_COMBO1, INV_KEY6_COMBO2, INV_KEY6 },
    { combine_key_item7, INV_KEY7_COMBO1, INV_KEY7_COMBO2, INV_KEY7 },
    { combine_key_item8, INV_KEY8_COMBO1, INV_KEY8_COMBO2, INV_KEY8 },
    { combine_pickup_item1, INV_PICKUP1_COMBO1, INV_PICKUP1_COMBO2, INV_PICKUP1 },
    { combine_pickup_item2, INV_PICKUP2_COMBO1, INV_PICKUP2_COMBO2, INV_PICKUP2 },
    { combine_pickup_item3, INV_PICKUP3_COMBO1, INV_PICKUP3_COMBO2, INV_PICKUP3 },
    { combine_pickup_item4, INV_PICKUP4_COMBO1, INV_PICKUP4_COMBO2, INV_PICKUP4 },
    { combine_clockwork, INV_CLOCKWORK_BEETLE_COMBO1, INV_CLOCKWORK_BEETLE_COMBO2, INV_CLOCKWORK_BEETLE }
};

DWORD inventory_light = RGBA_VECTORGET(127, 127, 127);
float inventory_drawX;
float inventory_drawY;
int inventory_xpos = 0;
int inventory_ypos = 0;

int compass_needle_angle;
int objlist_spacing;
int combine_obj1;
int combine_obj2;
short normal_ring_fade_val;
short normal_ring_fade_dir;
short ammo_selector_fade_val;
short ammo_selector_fade_dir;
short combine_ring_fade_val;
short combine_ring_fade_dir;
short examine_mode = 0;
short stats_mode;
short inv_pistols_ammo_count = 0;
short inv_shotgun_ammo1_count = 0;
short inv_shotgun_ammo2_count = 0;
short inv_uzi_ammo_count = 0;
short inv_grenadegun_ammo1_count = 0;
short inv_grenadegun_ammo2_count = 0;
short inv_grenadegun_ammo3_count = 0;
short inv_crossbow_ammo1_count = 0;
short inv_crossbow_ammo2_count = 0;
short inv_crossbow_ammo3_count = 0;
short inv_revolver_ammo_count = 0;
char current_selected_option;
char num_ammo_slots;
char current_ammo_type;
char combine_type_flag;
char ammo_selector_flag;
char seperate_type_flag;
char old_lara_busy;
char left_repeat;
char right_repeat;
char loading_or_saving;
char current_pistols_ammo_type = 0;
char current_shotgun_ammo_type = 0;
char current_uzi_ammo_type = 0;
char current_grenadegun_ammo_type = 0;
char current_crossbow_ammo_type = 0;
char current_revolver_ammo_type = 0;
char save_current_selection = 0;
char save_pistols_ammo_type = 0;
char save_shotgun_ammo_type = 0;
char save_uzi_ammo_type = 0;
char save_grenadegun_ammo_type = 0;
char save_crossbow_ammo_type = 0;
char save_revolver_ammo_type = 0;
bool menu_active;
bool ammo_active;
bool left_debounce;
bool right_debounce;
bool up_debounce;
bool down_debounce;
bool go_left;
bool go_right;
bool go_up;
bool go_down;
bool select_debounce;
bool deselect_debounce;
bool go_select;
bool go_deselect;
bool use_items;
bool friggrimmer1;
bool friggrimmer2;

int show_inventory_2d(void)
{
    short flag;
    int end, val;

    old_lara_busy = lara.busy;
    friggrimmer1 = false;
    if (CHK_ANY(TrInput, IN_SELECT))
        friggrimmer1 = true;

    ring_2d[RING_INVENTORY] = &GadwPolygonBuffers_RingNormal;
    ring_2d[RING_COMBINE] = &GadwPolygonBuffers_RingCombine;
    CreateMonoScreen();
    in_inventory = TRUE;
    construct_inventory_2d();
    camera.number_frames = 2;
    flag = 0;
    end = 0;

    do
    {
        if (reset_flag)
            break;
        objlist_spacing = phd_centerx >> 1;
        if (compass_needle_angle != WALL_L)
            compass_needle_angle -= 32;

        GPU_BeginScene();
        SetDebounce = 1;
        S_UpdateInput();
        TrInput = TrInputDB;
        UpdatePulseColour();
        GameTimer++;

        if (CHK_EXI(dbinput, IN_OPTION))
        {
            SoundEffect(SFX_MENU_SELECT, nullptr, SFX_ALWAYS);
            flag = 1;
        }

        if (thread_started)
            return thread_started;

        draw_outlines();
        do_debounced_input();

        if (examine_mode)
        {
            do_examine_mode();
        }
        else
        {
            draw_current_object_list(RING_INVENTORY);
            handle_inventry_menu();
            if (ring_2d[RING_COMBINE]->active)
                draw_current_object_list(RING_COMBINE);
            draw_ammo_selector();
            fade_ammo_selector();
            do_compass_mode();
        }

        if (use_items && TrInput == IN_NONE)
            flag = 1;

        S_OutputPolyList();
        camera.number_frames = S_DumpScreen();

        if (loading_or_saving)
        {
            do
            {
                GPU_BeginScene();
                val = 0;
                SetDebounce = 1;
                S_UpdateInput();
                TrInput = TrInputDB;
                UpdatePulseColour();

                if (loading_or_saving == 1)
                {
                    val = go_and_load_game();
                }
                else
                {
                    if (go_and_save_game())
                        val = 1;
                }
            } while (val == 0);

            if (val == 1 && loading_or_saving == val)
            {
                end = 1;
                flag = 1;
            }

            friggrimmer2 = true;
            friggrimmer1 = true;
            deselect_debounce = false;
            go_deselect = false;
            loading_or_saving = 0;
        }
    }
    while (!flag);

    ///InitialisePickUpDisplay(); // it reset the pickup display !
    GLOBAL_invitemlastchosen = ring_2d[RING_INVENTORY]->current_obj_list[ring_2d[RING_INVENTORY]->cur_obj_in_list].inv_item;
    update_laras_weapons_status();
    if (use_items)
        use_current_item();
    RemoveMonoScreen();
    in_inventory = false;
    lara.busy = old_lara_busy;

    return end;
}

void construct_inventory_2d(void)
{
    compass_needle_angle = SECTOR(4);
    examine_mode = 0;
    stats_mode = 0;

    AlterFOV(DEFAULT_FOV);
    lara.busy = FALSE;

    GLOBAL_invitemchosen = NO_ITEM;
    GLOBAL_invitemlastchosen = NO_ITEM;
    left_debounce = false;
    right_debounce = false;
    up_debounce = false;
    down_debounce = false;
    go_left = false;
    go_right = false;
    go_up = false;
    go_down = false;
    select_debounce = false;
    deselect_debounce = false;
    go_select = false;
    go_deselect = false;
    left_repeat = false;
    right_repeat = false;
    loading_or_saving = 0;
    use_items = false;

    inv_pistols_ammo_count = lara.pistol_ammo_count;
    inv_shotgun_ammo1_count = (lara.shotgun_ammo1_count == INFINITE_AMMO) ? (INFINITE_AMMO) : (lara.shotgun_ammo1_count / 6);
    inv_shotgun_ammo2_count = (lara.shotgun_ammo2_count == INFINITE_AMMO) ? (INFINITE_AMMO) : (lara.shotgun_ammo2_count / 6);
    inv_uzi_ammo_count = lara.uzi_ammo_count;
    inv_grenadegun_ammo1_count = lara.grenade_ammo1_count;
    inv_grenadegun_ammo2_count = lara.grenade_ammo2_count;
    inv_grenadegun_ammo3_count = lara.grenade_ammo3_count;
    inv_crossbow_ammo1_count = lara.crossbow_ammo1_count;
    inv_crossbow_ammo2_count = lara.crossbow_ammo2_count;
    inv_crossbow_ammo3_count = lara.crossbow_ammo3_count;
    inv_revolver_ammo_count = lara.revolver_ammo_count;
    
    construct_object_list();

    if (GLOBAL_invEnter == NO_ITEM)
    {
        if (GLOBAL_invitemlastchosen != NO_ITEM)
        {
            if (have_i_got_item(GLOBAL_invitemlastchosen))
            {
                setup_objectlist_startposition_invitem(GLOBAL_invitemlastchosen);
            }
            else
            {
                if (GLOBAL_invitemlastchosen >= INV_WATERSKIN_EMPTY && GLOBAL_invitemlastchosen <= INV_WATERSKIN1_3)
                {
                    for (int i = INV_WATERSKIN_EMPTY; i <= INV_WATERSKIN1_3; i++)
                    {
                        if (have_i_got_item(i))
                            setup_objectlist_startposition_invitem(i);
                    }
                }
                else if (GLOBAL_invitemlastchosen >= INV_WATERSKIN2_EMPTY && GLOBAL_invitemlastchosen <= INV_WATERSKIN2_5)
                {
                    for (int i = INV_WATERSKIN2_EMPTY; i <= INV_WATERSKIN2_5; i++)
                    {
                        if (have_i_got_item(i))
                            setup_objectlist_startposition_invitem(i);
                    }
                }
            }

            GLOBAL_invitemlastchosen = NO_ITEM;
        }
    }
    else
    {
        if (have_i_got_object(GLOBAL_invEnter))
            setup_objectlist_startposition_objnumber(GLOBAL_invEnter);
        GLOBAL_invEnter = NO_ITEM;
    }

    ammo_selector_fade_val = 0;
    ammo_selector_fade_dir = 0;
    combine_ring_fade_val = 0;
    combine_ring_fade_dir = 0;
    combine_type_flag = 0;
    seperate_type_flag = 0;
    combine_obj1 = 0;
    combine_obj2 = 0;
    normal_ring_fade_val = 128;
    normal_ring_fade_dir = 0;
    handle_object_changeover();
}

void do_debounced_input(void)
{
    go_left = false;
    go_right = false;
    go_up = false;
    go_down = false;
    go_select = false;
    go_deselect = false;

    /// CHECK FOR LEFT AND RIGHT
    if (CHK_ANY(TrInput, IN_LEFT))
    {
        if (left_repeat >= INVENTORY_INPUT_REPEAT)
            go_left = true;
        else
            ++left_repeat;
        if (!left_debounce)
            go_left = true;
        left_debounce = true;
    }
    else
    {
        left_debounce = false;
        left_repeat = 0;
    }

    if (CHK_ANY(TrInput, IN_RIGHT))
    {
        if (right_repeat >= INVENTORY_INPUT_REPEAT)
            go_right = true;
        else
            ++right_repeat;
        if (!right_debounce)
            go_right = true;
        right_debounce = true;
    }
    else
    {
        right_debounce = false;
        right_repeat = 0;
    }

    /// NOW CHECK FOR UPWARD AND BACKWARD
    if (CHK_ANY(TrInput, IN_FORWARD))
    {
        if (!up_debounce)
            go_up = true;
        up_debounce = true;
    }
    else
    {
        up_debounce = false;
    }

    if (CHK_ANY(TrInput, IN_BACK))
    {
        if (!down_debounce)
            go_down = true;
        down_debounce = true;
    }
    else
    {
        down_debounce = false;
    }

    /// NOW CHECK FOR ACTION/SELECT
    if (CHK_ANY(TrInput, IN_ACTION) || CHK_ANY(TrInput, IN_SELECT))
    {
        select_debounce = true;
    }
    else
    {
        if (select_debounce && !friggrimmer1)
            go_select = true;
        select_debounce = false;
        friggrimmer1 = false;
    }

    /// NOW CHECK ESCAPE
    if (CHK_ANY(TrInput, IN_DESELECT))
    {
        deselect_debounce = true;
    }
    else
    {
        if (deselect_debounce && !friggrimmer2)
            go_deselect = true;
        deselect_debounce = false;
        friggrimmer2 = false;
    }
}

void DrawThreeDeeObject2D(int x, int y, int num, int shade, int xrot, int yrot, int zrot, int bright, int overlay)
{
    ITEM_INFO item;
    INVOBJ* obj = &inventry_objects_list[num];

    item.object_number = obj->object_number;
    item.pos.x_rot = xrot + obj->xrot;
    item.pos.y_rot = yrot + obj->yrot;
    item.pos.z_rot = zrot + obj->zrot;

    phd_LookAt(NULL, WALL_L, NULL, NULL, NULL, NULL, NULL);

    if (bright)
    {
        if (bright == 1)
            inventory_light = RGBA_VECTORGET(CVECTOR(0x2F, 0x2F, 0x2F));
        else
            inventory_light = RGBA_VECTORGET(CVECTOR(bright, bright, bright));
    }
    else
    {
        inventory_light = RGBA_VECTORGET(CVECTOR(0x7F, 0x7F, 0x7F));
    }

    SetD3DViewMatrix();
    phd_PushUnitMatrix();
    phd_TranslateRel(0, 0, obj->scale);

    inventory_drawX = float(x);
    inventory_drawY = float(y + obj->yoff);
    
    item.current_anim = objects[item.object_number].anim_index;
    item.mesh_bits = obj->meshbits;
    item.shade = -1;
    item.pos.x = 0;
    item.pos.y = 0;
    item.pos.z = 0;
    item.room_number = 0;
    item.il.room_ambient.r = 0x7F;
    item.il.room_ambient.g = 0x7F;
    item.il.room_ambient.b = 0x7F;

    DrawInventoryItem(&item, shade, overlay, (obj->flags & 8) != 0);
    phd_PopMatrix();

    inventory_drawX = float(phd_centerx);
    inventory_drawY = float(phd_centery);
}

void DrawInventoryItem(ITEM_INFO* item, int shade, int overlay, BOOL shade_flags)
{
    ANIM_STRUCT* anim;
    OBJECT_INFO* obj;
    short* rotation;
    short** mesh;
    int* bone;
    int mesh_bits;

    phd_PushMatrix();
    phd_TranslateRel(item->pos.x, item->pos.y, item->pos.z);
    phd_RotYXZ(item->pos.y_rot, item->pos.x_rot, item->pos.z_rot);

    if (item->object_number >= EXAMINE1 && item->object_number <= EXAMINE3 && examine_mode)
    {
        PHD_VECTOR examine_shift;

        switch (item->object_number)
        {
            case EXAMINE1:
                examine_shift.x = 0x1C00;
                examine_shift.y = 0x1200;
                examine_shift.z = 0x1000;
                ShiftMatrixBasedPos(&examine_shift);
                break;
            case EXAMINE2:
                examine_shift.x = 0x1680;
                examine_shift.y = 0x1400;
                examine_shift.z = 0x1000;
                ShiftMatrixBasedPos(&examine_shift);
                break;
        }
    }

    obj = &objects[item->object_number];
    anim = &anims[item->current_anim];
    mesh = &meshes[obj->mesh_index];
    bone = &bones[obj->bone_index];
    mesh_bits = 1;

    if (!shade_flags)
        phd_TranslateRel((int)*(anim->frame_ptr + 6), (int)*(anim->frame_ptr + 7), (int)*(anim->frame_ptr + 8));
    rotation = anim->frame_ptr + 9;
    gar_RotYXZsuperpack(&rotation, 0);

    if (item->mesh_bits & 1) // BODY
    {
        if (overlay)
        {
            phd_PutPolygons_inv(*mesh, inventory_drawX, inventory_drawY, inventory_light);
        }
        else
        {
            DWORD item_backup = item_color;
            if (shade <= 127)
                shade *= 2;
            else
                shade = 255;
            item_color = shade << 24;
            phd_PutPolygons_inv(*mesh, inventory_drawX, inventory_drawY, inventory_light);
            item_color = item_backup;
        }
    }

    for (int i = obj->nmeshes - 1; i > 0; i--)
    {
        mesh += 2;
        mesh_bits *= 2;

        if (bone[BT_FLAG] & BT_POP)
            phd_PopMatrix();
        if (bone[BT_FLAG] & BT_PUSH)
            phd_PushMatrix();

        phd_TranslateRel(bone[BT_X], bone[BT_Y], bone[BT_Z]);
        gar_RotYXZsuperpack(&rotation, 0);

        /// DELETED COMPASS CHEAT !!!
        /// DONT NEED COMPASS CHEAT AGAIN !
        if (item->object_number == COMPASS_ITEM)
        {
            short compass_speed = (short)(compass_needle_angle * 4 * rcossin_tbl[(compass_needle & 63) << 10 >> 3] >> W2V_SHIFT);
            short compass_angle = (lara_item->pos.y_rot + compass_speed) - 0x8000;
            phd_RotY(compass_angle);
        }

        if (mesh_bits & item->mesh_bits)
        {
            if (overlay)
            {
                phd_PutPolygons_inv(*mesh, inventory_drawX, inventory_drawY, inventory_light);
            }
            else
            {
                DWORD item_backup = item_color;
                if (shade <= 127)
                    shade *= 2;
                else
                    shade = 255;
                item_color = shade << 25;
                phd_PutPolygons_inv(*mesh, inventory_drawX, inventory_drawY, inventory_light);
                item_color = item_backup;
            }
        }

        bone += 4;
    }

    phd_PopMatrix();
}

int go_and_load_game(void)
{
    return LoadGameInfo();
}

int go_and_save_game(void)
{
    return SaveGameInfo();
}

void construct_combine_object_list(void)
{
    ring_2d[RING_COMBINE]->num_obj_in_list = NULL;
    for (int i = 0; i < MAX_INVOBJ; i++)
        ring_2d[RING_COMBINE]->current_obj_list[i].inv_item = NO_ITEM;

    if (CHK_NOP(gfLevelFlags, SLEV_YOUNG_LARA))
    {
        if (CHK_ANY(lara.revolver_type_carried, CR_PRESENT))
        {
            if (CHK_ANY(lara.revolver_type_carried, CR_LASERSIGHT))
                insert_object_into_list_combine(INV_REVOLVER_LASER);
            else
                insert_object_into_list_combine(INV_REVOLVER);
        }

        if (CHK_ANY(lara.crossbow_type_carried, CR_PRESENT))
        {
            if (CHK_ANY(lara.crossbow_type_carried, CR_LASERSIGHT))
                insert_object_into_list_combine(INV_CROSSBOW_LASER);
            else
                insert_object_into_list_combine(INV_CROSSBOW);
        }

        if (lara.lasersight)
            insert_object_into_list_combine(INV_LASERSIGHT);
    }

    if (lara.water_skin_empty1)
        insert_object_into_list_combine(lara.water_skin_empty1 + (INV_WATERSKIN_EMPTY - 1));
    if (lara.water_skin_empty2)
        insert_object_into_list_combine(lara.water_skin_empty2 + (INV_WATERSKIN_EMPTY - 1));

    for (int i = 0; i < 16; i++)
    {
        if (lara.puzzleitemscombo & (1 << i))
            insert_object_into_list_combine(INV_PUZZLE1_COMBO1 + i);
    }

    for (int i = 0; i < 16; i++)
    {
        if (lara.keyitemscombo & (1 << i))
            insert_object_into_list_combine(INV_KEY1_COMBO1 + i);
    }

    for (int i = 0; i < 8; i++)
    {
        if (lara.pickupitemscombo & (1 << i))
            insert_object_into_list_combine(INV_PICKUP1_COMBO1 + i);
    }

    if (lara.clockwork_beetle & CLOCK_COMBO1_PRESENT)
        insert_object_into_list_combine(INV_CLOCKWORK_BEETLE_COMBO1);
    if (lara.clockwork_beetle & CLOCK_COMBO2_PRESENT)
        insert_object_into_list_combine(INV_CLOCKWORK_BEETLE_COMBO2);

    ring_2d[RING_COMBINE]->obj_list_movement = 0;
    ring_2d[RING_COMBINE]->cur_obj_in_list = 0;
    ring_2d[RING_COMBINE]->active = FALSE;
}

void insert_object_into_list_combine(short num)
{
    if (options_table[num] & 9)
    {
        if (ring_2d[RING_INVENTORY]->current_obj_list[ring_2d[RING_INVENTORY]->cur_obj_in_list].inv_item != num)
        {
            ring_2d[RING_COMBINE]->current_obj_list[ring_2d[RING_COMBINE]->num_obj_in_list].inv_item = num;
            ring_2d[RING_COMBINE]->current_obj_list[ring_2d[RING_COMBINE]->num_obj_in_list].yrot = 0;
            ring_2d[RING_COMBINE]->current_obj_list[ring_2d[RING_COMBINE]->num_obj_in_list].bright = 32; // RGBA_TO_DWORD(32, 32, 32, 0), in DrawInventoryItem()
            ring_2d[RING_COMBINE]->num_obj_in_list++;
        }
    }
}

void construct_object_list(void)
{
    ring_2d[RING_INVENTORY]->num_obj_in_list = NULL;
    for (int i = 0; i < MAX_INVOBJ; i++)
        ring_2d[RING_INVENTORY]->current_obj_list[i].inv_item = NO_ITEM;

    current_pistols_ammo_type = AINV_AMMO1;
    current_uzi_ammo_type = AINV_AMMO1;
    current_revolver_ammo_type = AINV_AMMO1;
    current_shotgun_ammo_type = AINV_AMMO1;
    current_grenadegun_ammo_type = AINV_AMMO1;
    current_crossbow_ammo_type = AINV_AMMO1;

    if (CHK_NOP(gfLevelFlags, SLEV_YOUNG_LARA))
    {
        /// PISTOLS
        if (CHK_ANY(lara.pistols_type_carried, CR_PRESENT))
            insert_object_into_list_inventory(INV_PISTOLS);

        /// UZI
        if (CHK_ANY(lara.uzi_type_carried, CR_PRESENT))
            insert_object_into_list_inventory(INV_UZI);
        else if (inv_uzi_ammo_count)
            insert_object_into_list_inventory(INV_UZI_AMMO);

        /// REVOLVER
        if (CHK_ANY(lara.revolver_type_carried, CR_PRESENT))
        {
            if (CHK_ANY(lara.revolver_type_carried, CR_LASERSIGHT))
                insert_object_into_list_inventory(INV_REVOLVER_LASER);
            else
                insert_object_into_list_inventory(INV_REVOLVER);
        }
        else if (inv_revolver_ammo_count)
        {
            insert_object_into_list_inventory(INV_REVOLVER_AMMO);
        }

        /// SHOTGUN
        if (CHK_ANY(lara.shotgun_type_carried, CR_PRESENT))
        {
            insert_object_into_list_inventory(INV_SHOTGUN);
            if (CHK_ANY(lara.shotgun_type_carried, CR_AMMO2))
                current_shotgun_ammo_type = AINV_AMMO2;
        }
        else
        {
            if (inv_shotgun_ammo1_count)
                insert_object_into_list_inventory(INV_SHOTGUN_AMMO1);
            if (inv_shotgun_ammo2_count)
                insert_object_into_list_inventory(INV_SHOTGUN_AMMO2);
        }

        /// GRENADEGUN
        if (CHK_ANY(lara.grenadegun_type_carried, CR_PRESENT))
        {
            insert_object_into_list_inventory(INV_GRENADEGUN);

            if (CHK_ANY(lara.grenadegun_type_carried, CR_AMMO2))
                current_grenadegun_ammo_type = AINV_AMMO2;
            else if (CHK_ANY(lara.grenadegun_type_carried, CR_AMMO3))
                current_grenadegun_ammo_type = AINV_AMMO3;
        }
        else
        {
            if (inv_grenadegun_ammo1_count)
                insert_object_into_list_inventory(INV_GRENADEGUN_AMMO1);
            if (inv_grenadegun_ammo2_count)
                insert_object_into_list_inventory(INV_GRENADEGUN_AMMO2);
            if (inv_grenadegun_ammo3_count)
                insert_object_into_list_inventory(INV_GRENADEGUN_AMMO3);
        }

        /// CROSSBOW
        if (CHK_ANY(lara.crossbow_type_carried, CR_PRESENT))
        {
            if (CHK_ANY(lara.crossbow_type_carried, CR_LASERSIGHT))
                insert_object_into_list_inventory(INV_CROSSBOW_LASER);
            else
                insert_object_into_list_inventory(INV_CROSSBOW);

            if (CHK_ANY(lara.crossbow_type_carried, CR_AMMO2))
                current_crossbow_ammo_type = AINV_AMMO2;
            else if (CHK_ANY(lara.crossbow_type_carried, CR_AMMO3))
                current_crossbow_ammo_type = AINV_AMMO3;
        }
        else
        {
            if (inv_crossbow_ammo1_count)
                insert_object_into_list_inventory(INV_CROSSBOW_AMMO1);
            if (inv_crossbow_ammo2_count)
                insert_object_into_list_inventory(INV_CROSSBOW_AMMO2);
            if (inv_crossbow_ammo3_count)
                insert_object_into_list_inventory(INV_CROSSBOW_AMMO3);
        }

        /// OTHER
        if (lara.lasersight)
            insert_object_into_list_inventory(INV_LASERSIGHT);
        if (lara.binoculars)
            insert_object_into_list_inventory(INV_BINOCULARS);
        if (lara.flare_count)
            insert_object_into_list_inventory(INV_FLARE);
    }

    if (lara.small_medipack_count)
        insert_object_into_list_inventory(INV_SMALLMEDI);
    if (lara.large_medipack_count)
        insert_object_into_list_inventory(INV_BIGMEDI);

    if (lara.water_skin_empty1)
        insert_object_into_list_inventory(lara.water_skin_empty1 + (INV_WATERSKIN_EMPTY - 1));
    if (lara.water_skin_empty2)
        insert_object_into_list_inventory(lara.water_skin_empty2 + (INV_WATERSKIN2_EMPTY - 1));

    if (lara.crowbar)
        insert_object_into_list_inventory(INV_CROWBAR);

    if (lara.clockwork_beetle & CLOCK_PRESENT)
        insert_object_into_list_inventory(INV_CLOCKWORK_BEETLE);
    if (lara.clockwork_beetle & CLOCK_COMBO1_PRESENT)
        insert_object_into_list_inventory(INV_CLOCKWORK_BEETLE_COMBO1);
    if (lara.clockwork_beetle & CLOCK_COMBO2_PRESENT)
        insert_object_into_list_inventory(INV_CLOCKWORK_BEETLE_COMBO2);

    for (int i = 0; i < 12; i++)
    {
        if (lara.puzzleitems[i])
            insert_object_into_list_inventory(INV_PUZZLE1 + i);
    }

    for (int i = 0; i < 8; i++)
    {
        if (lara.puzzleitemscombo & (1 << i))
            insert_object_into_list_inventory(INV_PUZZLE1_COMBO1 + i);
    }

    for (int i = 0; i < 12; i++)
    {
        if (lara.keyitems & (1 << i))
            insert_object_into_list_inventory(INV_KEY1 + i);
    }

    for (int i = 0; i < 8; i++)
    {
        if (lara.keyitemscombo & (1 << i))
            insert_object_into_list_inventory(INV_KEY1_COMBO1 + i);
    }

    for (int i = 0; i < 4; i++)
    {
        if (lara.pickupitems & (1 << i))
            insert_object_into_list_inventory(INV_PICKUP1 + i);
    }

    for (int i = 0; i < 8; i++)
    {
        if (lara.pickupitemscombo & (1 << i))
            insert_object_into_list_inventory(INV_PICKUP1_COMBO1 + i);
    }

    for (int i = 0; i < 6; i++)
    {
        if (lara.questitems & (1 << i))
            insert_object_into_list_inventory(INV_QUEST1 + i);
    }

    if (lara.examine1)
        insert_object_into_list_inventory(INV_EXAMINE1);
    if (lara.examine2)
        insert_object_into_list_inventory(INV_EXAMINE2);
    if (lara.examine3)
        insert_object_into_list_inventory(INV_EXAMINE3);

    if (gameflow->load_save_enabled)
    {
        insert_object_into_list_inventory(INV_MEMCARD_LOAD);
        insert_object_into_list_inventory(INV_MEMCARD_SAVE);
    }

    ring_2d[RING_INVENTORY]->obj_list_movement = 0;
    ring_2d[RING_INVENTORY]->cur_obj_in_list = 0;
    ring_2d[RING_COMBINE]->obj_list_movement = 0;
    ring_2d[RING_COMBINE]->cur_obj_in_list = 0;
    handle_object_changeover();
    ring_2d[RING_INVENTORY]->active = TRUE;
    ring_2d[RING_COMBINE]->active = FALSE;
    ammo_active = false;
}

void insert_object_into_list_inventory(short num)
{
    RING2D* normal = ring_2d[RING_INVENTORY];
    INVLIST* list_inv = normal->current_obj_list;

    list_inv[normal->num_obj_in_list].inv_item = num;
    list_inv[normal->num_obj_in_list].yrot = 0;
    list_inv[normal->num_obj_in_list].bright = 32; // RGBA_TO_DWORD(32, 32, 32, 0), in DrawInventoryItem()
    normal->num_obj_in_list++;
}

void draw_current_object_list(int ring_type)
{
    if (ring_2d[ring_type]->num_obj_in_list <= 0)
        return;
    
    if (ring_type == RING_COMBINE)
    {
        ammo_selector_fade_val = 0;
        ammo_selector_fade_dir = 0;

        if (combine_ring_fade_dir == 1)
        {
            if (combine_ring_fade_val < 128)
                combine_ring_fade_val += 32;

            if (combine_ring_fade_val > 128)
            {
                combine_ring_fade_val = 128;
                combine_ring_fade_dir = 0;
            }
        }
        else if (combine_ring_fade_dir == 2)
        {
            if (combine_ring_fade_val <= 0 || (combine_ring_fade_val -= 32, combine_ring_fade_val <= 0))
            {
                combine_ring_fade_val = 0;
                combine_ring_fade_dir = 0;
                if (combine_type_flag)
                {
                    normal_ring_fade_dir = 2;
                }
                else
                {
                    ring_2d[RING_INVENTORY]->active = TRUE;
                    menu_active = true;
                    ring_2d[RING_COMBINE]->active = FALSE;
                    handle_object_changeover();
                }
                ring_2d[RING_COMBINE]->active = FALSE;
            }
        }
    }
    else if (normal_ring_fade_dir == 1)
    {
        if (normal_ring_fade_val < 128)
            normal_ring_fade_val += 32;

        if (normal_ring_fade_val > 128)
        {
            normal_ring_fade_val = 128;
            normal_ring_fade_dir = 0;
            ring_2d[RING_INVENTORY]->active = TRUE;
            menu_active = true;
        }
    }
    else if (normal_ring_fade_dir == 2)
    {
        if (normal_ring_fade_val > 0)
            normal_ring_fade_val -= 32;

        if (normal_ring_fade_val <= 0)
        {
            normal_ring_fade_val = 0;
            normal_ring_fade_dir = 1;
            if (combine_type_flag == 1)
            {
                combine_type_flag = 0;
                combine_these_two_objects(combine_obj1, combine_obj2);
            }
            else if (combine_type_flag == 2)
            {
                combine_type_flag = 0;
                construct_object_list();
                setup_objectlist_startposition_invitem(combine_obj1);
            }
            else if (seperate_type_flag)
            {
                seperate_object(ring_2d[RING_INVENTORY]->current_obj_list[ring_2d[RING_INVENTORY]->cur_obj_in_list].inv_item);
            }
            handle_object_changeover();
        }
    }

    RING2D* ring = ring_2d[ring_type];
    int count, shade;
    int minobj, maxobj;
    int n, current_obj;
    int xoff, list_num, i;
    int y, centerx, centery;
    int inventoryX, inventoryY;
    int moving_turn;
    unsigned short pos_y;
    char buffer[128];

    count = 0;
    i = 0;
    n = 0;
    minobj = 0;
    maxobj = 0;
    list_num = ring->num_obj_in_list;
    xoff = 0;

    if (list_num != 1)
        xoff = objlist_spacing * ring->obj_list_movement >> 16;

    if (list_num == 2)
    {
        minobj = -1;
        maxobj = 0;
        n = ring->cur_obj_in_list - 1;
    }

    if (list_num == 3 || list_num == 4)
    {
        minobj = -2;
        maxobj = 1;
        n = ring->cur_obj_in_list - 2;
    }

    if (list_num >= 5)
    {
        minobj = -3;
        maxobj = 2;
        n = ring->cur_obj_in_list - 3;
    }

    if (n < 0)
        n += list_num;

    if (ring->obj_list_movement < 0)
        maxobj++;

    i = minobj;
    if (minobj <= maxobj)
    {
        while (i <= maxobj)
        {
            if (i == minobj)
            {
                if (ring->obj_list_movement < 0)
                    shade = 0;
                else
                    shade = ring->obj_list_movement << 7 >> 16;
            }
            else if (i != minobj + 1 || maxobj == minobj + 1)
            {
                if (i != maxobj)
                {
                    shade = 128;
                }
                else
                {
                    if (ring->obj_list_movement < 0)
                        shade = -(128 * ring->obj_list_movement) >> 16;
                    else
                        shade = 128 - (ring->obj_list_movement << 7 >> 16);
                }
            }
            else
            {
                if (ring->obj_list_movement >= 0)
                    shade = 128;
                else
                    shade = 128 - (-128 * ring->obj_list_movement >> 16);
            }

            if (minobj == 0 && maxobj == 0)
                shade = 128;

            switch (ring_type)
            {
                case RING_INVENTORY:
                    if (normal_ring_fade_val < 128 && shade)
                        shade = normal_ring_fade_val;
                    break;
                case RING_COMBINE:
                    if (combine_ring_fade_val < 128 && shade)
                        shade = combine_ring_fade_val;
                    break;
            }

            INVOBJ* obj = &inventry_objects_list[ring->current_obj_list[n].inv_item];
            if (i == 0)
            {
                switch (obj->object_number)
                {
                    case BIGMEDI_ITEM:
                        count = lara.large_medipack_count;
                        break;
                    case SMALLMEDI_ITEM:
                        count = lara.small_medipack_count;
                        break;
                    case FLARE_INV_ITEM:
                        count = lara.flare_count;
                        break;
                    case SHOTGUN_AMMO1_ITEM:
                        count = (lara.shotgun_ammo1_count != INFINITE_AMMO) ? (lara.shotgun_ammo1_count / 6) : (lara.shotgun_ammo1_count);
                        break;
                    case SHOTGUN_AMMO2_ITEM:
                        count = (lara.shotgun_ammo2_count != INFINITE_AMMO) ? (lara.shotgun_ammo2_count / 6) : (lara.shotgun_ammo2_count);
                        break;
                    case GRENADE_GUN_AMMO1_ITEM:
                        count = lara.grenade_ammo1_count;
                        break;
                    case GRENADE_GUN_AMMO2_ITEM:
                        count = lara.grenade_ammo2_count;
                        break;
                    case GRENADE_GUN_AMMO3_ITEM:
                        count = lara.grenade_ammo3_count;
                        break;
                    case CROSSBOW_AMMO1_ITEM:
                        count = lara.crossbow_ammo1_count;
                        break;
                    case CROSSBOW_AMMO2_ITEM:
                        count = lara.crossbow_ammo2_count;
                        break;
                    case CROSSBOW_AMMO3_ITEM:
                        count = lara.crossbow_ammo3_count;
                        break;
                    case REVOLVER_AMMO_ITEM:
                        count = lara.revolver_ammo_count;
                        break;
                    case UZI_AMMO_ITEM:
                        count = lara.uzi_ammo_count;
                        break;
                    default:
                        if ((obj->object_number - PUZZLE_ITEM1) < 12)
                            count = lara.puzzleitems[obj->object_number - PUZZLE_ITEM1];
                        break;
                }

                if (count != 0)
                {
                    if (count == INFINITE_AMMO)
                        sprintf(buffer, &gfStringWad[gfStringOffset[STR_UNLIMITED]], &gfStringWad[gfStringOffset[obj->name]]);
                    else
                        sprintf(buffer, "%d x %s", count, &gfStringWad[gfStringOffset[obj->name]]);
                }
                else
                {
                    sprintf(buffer, &gfStringWad[gfStringOffset[obj->name]]);
                }

                if (ring_type == RING_COMBINE)
                    pos_y = (WORD)(phd_centery + (phd_winymax + 1) * 0.0625 * 3.0);
                else
                    pos_y = (WORD)(phd_centery - (phd_winymax + 1) * 0.0625 * 3.0);
                PrintString(phd_centerx, pos_y, 8, buffer, FF_CENTER);
            }

            if (i == 0 && ring->obj_list_movement == 0)
            {
                current_obj = n;
                if (inventry_objects_list[ring->current_obj_list[n].inv_item].flags & 2)
                    ring->current_obj_list[n].yrot += 1022;
            }
            else
            {
                current_obj = n;
                spinback(&ring->current_obj_list[n].yrot);
            }

            INVLIST* list = &ring->current_obj_list[current_obj];
            if (ring->obj_list_movement != 0)
                moving_turn = ring->obj_list_movement <= 0 ? 1 : -1;
            else
                moving_turn = 0;

            if (i == moving_turn)
            {
                if (list->bright < 160)
                    list->bright += 16;
                if (list->bright > 160)
                    list->bright = 160;
            }
            else
            {
                if (list->bright > 32)
                    list->bright -= 16;
                if (list->bright < 32)
                    list->bright = 32;
            }

            if (ring_type == RING_INVENTORY)
                y = 42;
            else
                y = 190;

            centerx = int(phd_centerx * 0.00390625   * STEP_L);
            centery = int(phd_centery * 0.0083333338 * y);
            inventoryX = inventory_xpos + centerx + (i * objlist_spacing) + xoff;
            inventoryY = inventory_ypos + centery;
            DrawThreeDeeObject2D(inventoryX, inventoryY, ring->current_obj_list[current_obj].inv_item, shade, NULL, ring->current_obj_list[current_obj].yrot, NULL, ring->current_obj_list[current_obj].bright, FALSE);
            
            ++n;
            if (n >= ring->num_obj_in_list)
                n = 0;
            ++i;
        }

        if (ring->active && ring->num_obj_in_list != 1 && (ring_type != RING_COMBINE || combine_ring_fade_val == 128))
        {
            if (ring->obj_list_movement > 0)
                ring->obj_list_movement += 0x2000;
            if (ring->obj_list_movement < 0)
                ring->obj_list_movement -= 0x2000;

            if (go_left && ring->obj_list_movement == 0)
            {
                SoundEffect(SFX_MENU_ROTATE, nullptr, SFX_ALWAYS);
                ring->obj_list_movement += 0x2000;
                if (ammo_selector_flag)
                    ammo_selector_fade_dir = 2;
            }

            if (go_right && ring->obj_list_movement == 0)
            {
                SoundEffect(SFX_MENU_ROTATE, nullptr, SFX_ALWAYS);
                ring->obj_list_movement -= 0x2000;
                if (ammo_selector_flag)
                    ammo_selector_fade_dir = 2;
            }

            if (ring->obj_list_movement < MAXWORD)
            {
                if (ring->obj_list_movement < -MAXWORD)
                {
                    ring->cur_obj_in_list++;

                    if (ring->cur_obj_in_list >= ring->num_obj_in_list)
                        ring->cur_obj_in_list = 0;
                    ring->obj_list_movement = 0;
                    if (!ring_type)
                        handle_object_changeover();
                }
            }
            else
            {
                ring->cur_obj_in_list--;

                if (ring->cur_obj_in_list < 0)
                    ring->cur_obj_in_list = ring->num_obj_in_list - 1;
                ring->obj_list_movement = 0;
                if (!ring_type)
                    handle_object_changeover();
            }
        }
    }
}

void setup_ammo_selector(void)
{
    short opts = options_table[ring_2d[RING_INVENTORY]->current_obj_list[ring_2d[RING_INVENTORY]->cur_obj_in_list].inv_item];
    ammo_selector_flag = 0;
    num_ammo_slots = 0;

    if (!ring_2d[RING_AMMO]->active)
    {
        ammo_object_list[AINV_AMMO1].yrot = 0;
        ammo_object_list[AINV_AMMO2].yrot = 0;
        ammo_object_list[AINV_AMMO3].yrot = 0;

        if (CHK_ANY(opts, OPTION_ALL_WEAPONS))
        {
            ammo_selector_flag = 1;
            ammo_selector_fade_dir = 1;

            if (CHK_EXI(opts, OPTION_SHOTGUN))
            {
                ammo_object_list[0].inv_item = INV_SHOTGUN_AMMO1;
                ammo_object_list[0].amount = inv_shotgun_ammo1_count;
                ammo_object_list[1].inv_item = INV_SHOTGUN_AMMO2;
                ammo_object_list[1].amount = inv_shotgun_ammo2_count;
                current_ammo_type = current_shotgun_ammo_type;
                num_ammo_slots = 2;
            }

            if (CHK_EXI(opts, OPTION_CROSSBOW))
            {
                ammo_object_list[0].inv_item = INV_CROSSBOW_AMMO1;
                ammo_object_list[0].amount = inv_crossbow_ammo1_count;
                ammo_object_list[1].inv_item = INV_CROSSBOW_AMMO2;
                ammo_object_list[1].amount = inv_crossbow_ammo2_count;
                ammo_object_list[2].inv_item = INV_CROSSBOW_AMMO3;
                ammo_object_list[2].amount = inv_crossbow_ammo3_count;
                current_ammo_type = current_crossbow_ammo_type;
                num_ammo_slots = 3;
            }

            if (CHK_EXI(opts, OPTION_GRENADEGUN))
            {
                ammo_object_list[0].inv_item = INV_GRENADEGUN_AMMO1;
                ammo_object_list[0].amount = inv_grenadegun_ammo1_count;
                ammo_object_list[1].inv_item = INV_GRENADEGUN_AMMO2;
                ammo_object_list[1].amount = inv_grenadegun_ammo2_count;
                ammo_object_list[2].inv_item = INV_GRENADEGUN_AMMO3;
                ammo_object_list[2].amount = inv_grenadegun_ammo3_count;
                current_ammo_type = current_grenadegun_ammo_type;
                num_ammo_slots = 3;
            }

            if (CHK_EXI(opts, OPTION_UZIS))
            {
                ammo_object_list[0].inv_item = INV_UZI_AMMO;
                ammo_object_list[0].amount = inv_uzi_ammo_count;
                current_ammo_type = current_uzi_ammo_type;
                num_ammo_slots = 1;
            }

            if (CHK_EXI(opts, OPTION_PISTOLS))
            {
                ammo_object_list[0].inv_item = INV_PISTOLS_AMMO;
                ammo_object_list[0].amount = inv_pistols_ammo_count; // normally INFINITE_AMMO...
                current_ammo_type = current_pistols_ammo_type;
                num_ammo_slots = 1;
            }

            if (CHK_EXI(opts, OPTION_REVOLVER))
            {
                ammo_object_list[0].inv_item = INV_REVOLVER_AMMO;
                ammo_object_list[0].amount = inv_revolver_ammo_count;
                current_ammo_type = current_revolver_ammo_type;
                num_ammo_slots = 1;
            }
        }
    }
}

void handle_object_changeover(void)
{
    current_selected_option = 0;
    menu_active = true;
    setup_ammo_selector();
}

void handle_inventry_menu(void)
{
    RING2D* item = ring_2d[RING_INVENTORY];
    RING2D* ammo = ring_2d[RING_AMMO];
    INVOBJ* inv_ammo1 = &inventry_objects_list[ammo_object_list[0].inv_item];
    INVOBJ* inv_ammo2 = &inventry_objects_list[ammo_object_list[1].inv_item];
    INVOBJ* inv_ammo3 = &inventry_objects_list[ammo_object_list[2].inv_item];
    short inv_item = item->current_obj_list[item->cur_obj_in_list].inv_item;
    short inv_ammo = ammo->current_obj_list[ammo->cur_obj_in_list].inv_item;
    short opts;
    int posY;
    int num;

    for (int i = 0; i < MAX_INVOBJ; i++)
    {
        S_LogValue("id: %d, option_table: %d", i, options_table[i]);
    }

    if (ammo->active)
    {
        PrintString(phd_centerx, phd_centery, 1, &gfStringWad[gfStringOffset[option_name[125]]], FF_CENTER);

        if (item->obj_list_movement)
            return;
        if (ammo->obj_list_movement)
            return;

        if (go_select)
        {
            if (inv_item >= INV_WATERSKIN_EMPTY && inv_item <= INV_WATERSKIN1_3)
            {
                combine_waterskin(0);
                combine_type_flag = 2;
                combine_ring_fade_dir = 2;
                SoundEffect(SFX_MENU_COMBINE, nullptr, SFX_ALWAYS);
            }
            else if (inv_item >= INV_WATERSKIN2_1 && inv_item <= INV_WATERSKIN2_5)
            {
                combine_waterskin(1);
                combine_type_flag = 2;
                combine_ring_fade_dir = 2;
                SoundEffect(SFX_MENU_COMBINE, nullptr, SFX_ALWAYS);
            }
            else if (do_these_objects_combine(inv_item, inv_ammo))
            {
                combine_ring_fade_dir = 2;
                combine_type_flag = 1;
                combine_obj1 = inv_item;
                combine_obj2 = inv_ammo;
                SoundEffect(SFX_MENU_COMBINE, nullptr, SFX_ALWAYS);
            }
            else
            {
                combine_ring_fade_dir = 2;
                SayNo();
            }
        }

        if (go_deselect)
        {
            SoundEffect(SFX_MENU_SELECT, nullptr, SFX_ALWAYS);
            combine_ring_fade_dir = 2;
            go_deselect = false;
        }

        return;
    }
    else
    {
        for (int i = 0; i < 3; i++)
        {
            current_options[i].text = nullptr;
            current_options[i].type = 0;
        }

        num = 0;
        opts = options_table[inv_item];
        if (ammo_active)
        {
            if (CHK_ANY(opts, OPTION_SHOTGUN)
            ||  CHK_ANY(opts, OPTION_CROSSBOW)
            ||  CHK_ANY(opts, OPTION_GRENADEGUN)
            ||  CHK_ANY(opts, OPTION_PISTOLS)
            ||  CHK_ANY(opts, OPTION_REVOLVER)
            ||  CHK_ANY(opts, OPTION_UZIS))
            {
                /// AMMO1 TEXT
                num = 1;
                current_options[0].type = T_INVAMMO1;
                current_options[0].text = &gfStringWad[gfStringOffset[inv_ammo1->name]];
            }
            
            if (CHK_ANY(opts, OPTION_SHOTGUN)
            ||  CHK_ANY(opts, OPTION_CROSSBOW)
            ||  CHK_ANY(opts, OPTION_GRENADEGUN))
            {
                /// AMMO2 TEXT
                num = 2;
                current_options[1].type = T_INVAMMO2;
                current_options[1].text = &gfStringWad[gfStringOffset[inv_ammo2->name]];
            }

            if (CHK_ANY(opts, OPTION_CROSSBOW)
            ||  CHK_ANY(opts, OPTION_GRENADEGUN))
            {
                /// AMMO3 TEXT
                num = 3;
                current_options[2].type = T_INVAMMO3;
                current_options[2].text = &gfStringWad[gfStringOffset[inv_ammo3->name]];
            }

            current_selected_option = current_ammo_type;
        }
        else
        {
            if (CHK_EXI(opts, OPTION_LOAD))
            {
                num = 1;
                current_options[MENU_1].type = T_LOAD;
                current_options[MENU_1].text = &gfStringWad[gfStringOffset[options_table[STR_BUTTON_LOADGAME]]];
            }
            else if (CHK_EXI(opts, OPTION_SAVE))
            {
                num = 1;
                current_options[MENU_1].type = T_SAVE;
                current_options[MENU_1].text = &gfStringWad[gfStringOffset[options_table[STR_BUTTON_SAVEGAME]]];
            }
            else if (CHK_EXI(opts, OPTION_EXAMINE))
            {
                num = 1;
                current_options[MENU_1].type = T_EXAMINE;
                current_options[MENU_1].text = &gfStringWad[gfStringOffset[options_table[STR_BUTTON_EXAMINE]]];
            }

            if (CHK_EXI(opts, OPTION_USE))
            {
                current_options[num].type = T_USE;
                current_options[num++].text = &gfStringWad[gfStringOffset[options_table[STR_BUTTON_USE]]];
            }

            if (CHK_EXI(opts, OPTION_EQUIP))
            {
                current_options[num].type = T_EQUIP;
                current_options[num++].text = &gfStringWad[gfStringOffset[options_table[STR_BUTTON_EQUIP]]];
            }

            if (CHK_EXI(opts, OPTION_SHOTGUN) || CHK_EXI(opts, OPTION_CROSSBOW) || CHK_EXI(opts, OPTION_GRENADEGUN))
            {
                current_options[num].type = T_CHOOSE_AMMO;
                current_options[num++].text = &gfStringWad[gfStringOffset[options_table[STR_BUTTON_CHOOSE_AMMO]]];
            }

            if (CHK_EXI(opts, OPTION_COMBINE) && is_item_currently_combinable(inv_item))
            {
                current_options[num].type = T_COMBINE;
                current_options[num++].text = &gfStringWad[gfStringOffset[options_table[STR_BUTTON_COMBINE]]];
            }

            if (CHK_EXI(opts, OPTION_COMBINE_ITEM)) // not found yet but crash at combine button if i use my S_LogValue() (buffer overflow) to check the button name...
            {
                current_options[num].type = T_COMBINE;
                current_options[num++].text = &gfStringWad[gfStringOffset[options_table[STR_BUTTON_COMBINE]]];
            }

            if (CHK_EXI(opts, OPTION_SEPARATE))
            {
                current_options[num].type = T_SEPARATE;
                current_options[num++].text = &gfStringWad[gfStringOffset[options_table[STR_BUTTON_SEPARATE]]];
            }
        }

        switch (num)
        {
            case 1:
            case 2:
                posY = phd_centery - 20;
                break;
            case 3:
                posY = phd_centery - 40;
                break;
        }

        if (num > 0)
        {
            for (int i = 0; i < num; i++)
            {
                if (i == current_selected_option)
                {
                    PrintString(phd_centerx, posY, 1, current_options[i].text, FF_CENTER);
                    posY += 40;
                }
                else
                {
                    PrintString(phd_centerx, posY, 5, current_options[i].text, FF_CENTER);
                    posY += 40;
                }
            }
        }

        if (menu_active && !item->obj_list_movement && !ammo->obj_list_movement)
        {
            if (go_up && current_selected_option)
            {
                --current_selected_option;
                SoundEffect(SFX_MENU_SELECT, nullptr, SFX_ALWAYS);
            }
            else if (go_down && current_selected_option < num - 1)
            {
                ++current_selected_option;
                SoundEffect(SFX_MENU_SELECT, nullptr, SFX_ALWAYS);
            }

            if (ammo_active)
            {
                if (go_left && current_selected_option)
                {
                    --current_selected_option;
                    SoundEffect(SFX_MENU_SELECT, nullptr, SFX_ALWAYS);
                }

                if (go_right && current_selected_option < num - 1)
                {
                    ++current_selected_option;
                    SoundEffect(SFX_MENU_SELECT, nullptr, SFX_ALWAYS);
                }

                current_ammo_type = current_selected_option;
            }
            
            if (go_select)
            {
                MENU* menu = &current_options[current_selected_option];
                if (menu->type != 5 && menu->type != 1)
                    SoundEffect(SFX_MENU_CHOOSE, nullptr, SFX_ALWAYS);

                switch (menu->type)
                {
                    case 2:
                        item->active = FALSE;
                        save_current_selection = current_selected_option;
                        save_pistols_ammo_type = current_pistols_ammo_type;
                        save_uzi_ammo_type = current_uzi_ammo_type;
                        save_revolver_ammo_type = current_revolver_ammo_type;
                        save_shotgun_ammo_type = current_shotgun_ammo_type;
                        save_grenadegun_ammo_type = current_grenadegun_ammo_type;
                        save_crossbow_ammo_type = current_crossbow_ammo_type;
                        ammo_active = 1;
                        break;
                    case 9:
                        loading_or_saving = 1;
                        break;
                    case 10:
                        loading_or_saving = 2;
                        break;
                    case 11:
                        examine_mode = 1;
                        stats_mode = 0;
                        break;
                    case 6:
                    case 7:
                    case 8:
                        ammo_active = false;
                        item->active = true;
                        current_selected_option = 0;
                        break;
                    case 3: // TYPE_COMBINE ?
                        construct_combine_object_list();
                        item->active = FALSE;
                        ammo->active = TRUE;
                        ammo_selector_flag = 0;
                        menu_active = false;
                        combine_ring_fade_dir = 1;
                        break;
                    case 4:
                        seperate_type_flag = 1;
                        normal_ring_fade_dir = 2;
                        break;
                    case 5:
                    case 1:
                        menu_active = false;
                        use_items = true;
                        break;
                }
            }
            else if (go_deselect && ammo_active)
            {
                SoundEffect(SFX_MENU_SELECT, nullptr, SFX_ALWAYS);
                go_deselect = false;
                ammo_active = false;
                item->active = true;
                current_pistols_ammo_type = save_pistols_ammo_type;
                current_uzi_ammo_type = save_uzi_ammo_type;
                current_revolver_ammo_type = save_revolver_ammo_type;
                current_shotgun_ammo_type = save_shotgun_ammo_type;
                current_grenadegun_ammo_type = save_grenadegun_ammo_type;
                current_crossbow_ammo_type = save_crossbow_ammo_type;
                current_selected_option = save_current_selection;
            }
        }
    }
}

void fade_ammo_selector(void)
{
    if (ring_2d[RING_INVENTORY]->active && (right_repeat >= INVENTORY_INPUT_REPEAT || left_repeat >= INVENTORY_INPUT_REPEAT))
    {
        ammo_selector_fade_val = 0;
    }
    else if (ammo_selector_fade_dir == 1)
    {
        if (ammo_selector_fade_val < 128)
            ammo_selector_fade_val += 32;

        if (ammo_selector_fade_val > 128)
        {
            ammo_selector_fade_val = 128;
            ammo_selector_fade_dir = 0;
        }
    }
    else if (ammo_selector_fade_dir == 2)
    {
        if (ammo_selector_fade_val > 0)
            ammo_selector_fade_val -= 32;

        if (ammo_selector_fade_val < 0)
        {
            ammo_selector_fade_val = 0;
            ammo_selector_fade_dir = 0;
        }
    }
}

void draw_ammo_selector(void)
{
    if (ammo_selector_flag)
    {
        AMMOLIST* ammo;
        int space;
        int i;
        char buffer[128];

        space = ((2 * phd_centerx) - objlist_spacing) >> 1;
        switch (num_ammo_slots)
        {
            case 2:
                space += objlist_spacing / -2;
                break;
            case 3:
                space -= objlist_spacing;
                break;
        }

        i = 0;
        if (num_ammo_slots)
        {
            for (i = 0; i < num_ammo_slots; i++)
            {
                ammo = &ammo_object_list[i];
                if (i == current_ammo_type)
                {
                    if (inventry_objects_list[ammo->inv_item].flags & 2)
                        ammo->yrot += 1022;
                }
                else
                {
                    spinback(&ammo->yrot);
                }

                if (i == current_ammo_type)
                {
                    if (ammo->amount == -1)
                        sprintf(buffer, &gfStringWad[gfStringOffset[STR_UNLIMITED]], &gfStringWad[gfStringOffset[inventry_objects_list[ammo->inv_item].name]]);
                    else
                        sprintf(buffer, "%d x %s", ammo->amount, &gfStringWad[gfStringOffset[inventry_objects_list[ammo->inv_item].name]]);

                    if (ammo_selector_fade_val)
                        PrintString(phd_centerx, font_height + phd_centery + 2 * font_height - 9, 8, buffer, FF_CENTER);
                }

                DrawThreeDeeObject2D(
                    int(inventory_xpos + (phd_centerx * 0.00390625   * 64.0 ) + space),
                    int(inventory_ypos + (phd_centery * 0.0083333338 * 190.0)),
                    ammo->inv_item,
                    ammo_selector_fade_val,
                    NULL,
                    ammo->yrot,
                    NULL,
                    i != current_ammo_type,
                    FALSE);
                space += objlist_spacing;
            }
        }
    }
}

void spinback(WORD* yangle)
{
    WORD sYAngle = *yangle;
    WORD save, posi, nega;
    WORD result;

    save = sYAngle;
    if (sYAngle)
    {
        posi = sYAngle;
        if (sYAngle <= 0x8000)
        {
            if (sYAngle < 1022)
                sYAngle = 1022;
            if (sYAngle > 0x4000)
                sYAngle = 0x4000;
            result = posi - (save >> 3);
            if (result > 0x8000)
                result = 0;
        }
        else
        {
            nega = -sYAngle;
            if (nega < 1022)
                posi = 1022;
            if (nega > 0x4000)
                nega = 0x4000;
            result = (nega >> 3) + posi;
            if (result < 0x8000)
            {
                *yangle = 0;
                return;
            }
        }

        *yangle = result;
    }
}

void update_laras_weapons_status(void)
{
    if (CHK_ANY(lara.shotgun_type_carried, CR_PRESENT))
    {
        lara.shotgun_type_carried &= ~(CR_AMMOMASK); // delete all the ammo flag

        switch (current_shotgun_ammo_type)
        {
            case AINV_AMMO1:
                lara.shotgun_type_carried |= CR_AMMO1;
                break;
            case AINV_AMMO2:
                lara.shotgun_type_carried |= CR_AMMO2;
                break;
        }
    }

    if (CHK_ANY(lara.grenadegun_type_carried, CR_PRESENT))
    {
        lara.grenadegun_type_carried &= ~(CR_AMMOMASK);

        switch (current_grenadegun_ammo_type)
        {
            case AINV_AMMO1:
                lara.grenadegun_type_carried |= CR_AMMO1;
                break;
            case AINV_AMMO2:
                lara.grenadegun_type_carried |= CR_AMMO2;
                break;
            case AINV_AMMO3:
                lara.grenadegun_type_carried |= CR_AMMO3;
                break;
        }
    }

    if (CHK_ANY(lara.crossbow_type_carried, CR_PRESENT))
    {
        lara.crossbow_type_carried &= ~(CR_AMMOMASK);

        switch (current_crossbow_ammo_type)
        {
            case AINV_AMMO1:
                lara.crossbow_type_carried |= CR_AMMO1;
                break;
            case AINV_AMMO2:
                lara.crossbow_type_carried |= CR_AMMO2;
                break;
            case AINV_AMMO3:
                lara.crossbow_type_carried |= CR_AMMO3;
                break;
        }
    }
}

BOOL is_item_currently_combinable(short inv_item)
{
    int i;
    if (inv_item < INV_WATERSKIN_EMPTY || inv_item > INV_WATERSKIN2_5)
    {
        for (i = 0; i < MAX_COMBINE; i++)
        {
            short item_a = combine_table[i].item_a;
            short item_b = combine_table[i].item_b;
            
            if (item_a == inv_item)
            {
                if (have_i_got_item(item_b))
                    return TRUE;
            }
            else if (item_b == inv_item)
            {
                if (have_i_got_item(item_a))
                    return TRUE;
            }
        }
    }
    else if (inv_item > INV_WATERSKIN1_3)
    {
        for (i = 0; i < 4; i++)
        {
            if (have_i_got_item(i + INV_WATERSKIN_EMPTY))
                return TRUE;
        }
    }
    else
    {
        for (i = 0; i < 6; i++)
        {
            if (have_i_got_item(i + INV_WATERSKIN2_EMPTY))
                return TRUE;
        }
    }
    return FALSE;
}

BOOL have_i_got_item(short inv_item)
{
    RING2D* inv = ring_2d[RING_INVENTORY];

    for (int i = 0; i < MAX_INVOBJ; i++)
    {
        if (inv_item == inv->current_obj_list[i].inv_item)
            return TRUE;
    }

    return FALSE;
}

BOOL do_these_objects_combine(short item_a, short item_b)
{
    COMBINELIST* list;

    for (int i = 0; i < MAX_COMBINE; i++)
    {
        list = &combine_table[i];
        if (list->item_a == item_a && list->item_b == item_b)        // item_a == TRUE
            return TRUE;
        else if (list->item_a == item_b && list->item_b == item_a)   // item_b == TRUE
            return TRUE;
    }

    return FALSE;
}

void combine_these_two_objects(short item_a, short item_b)
{
    int i;
    COMBINELIST* list;
    for (i = 0; i < MAX_COMBINE; i++)
    {
        list = &combine_table[i];
        if (list->item_a == item_a && list->item_b == item_b)
            break;
        else if (list->item_a == item_b && list->item_b == item_a)
            break;
    }

    if (list != nullptr) // check for nullptr !!
    {
        list->combine_routine(CONSTRUCT_ITEM);
        construct_object_list();
        setup_objectlist_startposition_invitem(list->result);
        handle_object_changeover();
    }
}

void seperate_object(short inv_item)
{
    int i;
    COMBINELIST* list;
    for (i = 0; i < MAX_COMBINE; i++)
    {
        list = &combine_table[i];
        if (list->result == inv_item)
            break;
    }

    if (list != nullptr) // check for nullptr !!
    {
        list->combine_routine(DECONSTRUCT_ITEM);
        construct_object_list();
        setup_objectlist_startposition_invitem(list->item_a);
    }
}

void combine_revolver(int flag)
{
    switch (flag)
    {
        case CONSTRUCT_ITEM:
            lara.lasersight = FALSE;
            lara.revolver_type_carried |= CR_LASERSIGHT;
            break;
        case DECONSTRUCT_ITEM:
            lara.lasersight = TRUE;
            lara.revolver_type_carried &= ~(CR_LASERSIGHT);
            break;
    }

    if (lara.gun_status != LG_UNARMED && lara.gun_type == LG_REVOLVER)
    {
        undraw_pistols_left(LG_REVOLVER);
        draw_pistol_meshes(LG_REVOLVER);
    }
}

void combine_crossbow(int flag)
{
    switch (flag)
    {
        case CONSTRUCT_ITEM:
            lara.lasersight = FALSE;
            lara.crossbow_type_carried |= CR_LASERSIGHT;
            break;
        case DECONSTRUCT_ITEM:
            lara.lasersight = TRUE;
            lara.crossbow_type_carried &= ~(CR_LASERSIGHT);
            break;
    }

    if (lara.gun_status != LG_UNARMED && lara.gun_type == LG_CROSSBOW)
    {
        undraw_shotgun_meshes(LG_CROSSBOW);
        draw_shotgun_meshes(LG_CROSSBOW);
    }
}

void combine_puzzle_item1(int flag)
{
    lara.puzzleitemscombo &= ~(INV_COMBO1);
    lara.puzzleitems[0] = 1;
}

void combine_puzzle_item2(int flag)
{
    lara.puzzleitemscombo &= ~(INV_COMBO2);
    lara.puzzleitems[1] = 1;
}

void combine_puzzle_item3(int flag)
{
    lara.puzzleitemscombo &= ~(INV_COMBO3);
    lara.puzzleitems[2] = 1;
}

void combine_puzzle_item4(int flag)
{
    lara.puzzleitemscombo &= ~(INV_COMBO4);
    lara.puzzleitems[3] = 1;
}

void combine_puzzle_item5(int flag)
{
    lara.puzzleitemscombo &= ~(INV_COMBO4);
    lara.puzzleitems[4] = 1;
}

void combine_puzzle_item6(int flag)
{
    lara.puzzleitemscombo &= ~(INV_COMBO5);
    lara.puzzleitems[5] = 1;
}

void combine_puzzle_item7(int flag)
{
    lara.puzzleitemscombo &= ~(INV_COMBO6);
    lara.puzzleitems[6] = 1;
}

void combine_puzzle_item8(int flag)
{
    lara.puzzleitemscombo &= ~(INV_COMBO7);
    lara.puzzleitems[7] = 1;
}

void combine_key_item1(int flag)
{
    lara.keyitems |= 1;
    lara.keyitemscombo &= ~(INV_COMBO1);
}

void combine_key_item2(int flag)
{
    lara.keyitems |= 2;
    lara.keyitemscombo &= ~(INV_COMBO2);
}

void combine_key_item3(int flag)
{
    lara.keyitems |= 4;
    lara.keyitemscombo &= ~(INV_COMBO3);
}

void combine_key_item4(int flag)
{
    lara.keyitems |= 8;
    lara.keyitemscombo &= ~(INV_COMBO4);
}

void combine_key_item5(int flag)
{
    lara.keyitems |= 16;
    lara.keyitemscombo &= ~(INV_COMBO5);
}

void combine_key_item6(int flag)
{
    lara.keyitems |= 32;
    lara.keyitemscombo &= ~(INV_COMBO6);
}

void combine_key_item7(int flag)
{
    lara.keyitems |= 64;
    lara.keyitemscombo &= ~(INV_COMBO7);
}

void combine_key_item8(int flag)
{
    lara.keyitems |= 128;
    lara.keyitemscombo &= ~(INV_COMBO8);
}

void combine_pickup_item1(int flag)
{
    lara.pickupitems |= 1;
    lara.pickupitemscombo &= ~(INV_COMBO1);
}

void combine_pickup_item2(int flag)
{
    lara.pickupitems |= 2;
    lara.pickupitemscombo &= ~(INV_COMBO2);
}

void combine_pickup_item3(int flag)
{
    lara.pickupitems |= 4;
    lara.pickupitemscombo &= ~(INV_COMBO3);
}

void combine_pickup_item4(int flag)
{
    lara.pickupitems |= 8;
    lara.pickupitemscombo &= ~(INV_COMBO4);
}

void combine_clockwork(int flag)
{
    lara.clockwork_beetle = 1;
}

void combine_waterskin(int flag)
{
    int empty_count;
    int empty2_count;
    int slotcount_1;
    int slotcount_2;
    int i, j;

    empty_count = lara.water_skin_empty1 - 1;
    empty2_count = lara.water_skin_empty2 - 1;
    slotcount_1 = 3 - empty_count;
    slotcount_2 = 5 - empty2_count;

    if (flag)
    {
        if (lara.water_skin_empty2 != 1 && empty_count != 3)
        {
            i = lara.water_skin_empty2 - 1;
            do
            {
                if (slotcount_1)
                {
                    empty_count++;
                    empty2_count--;
                    --slotcount_1;
                }
                --i;
            } while (i);
            lara.water_skin_empty1 = empty_count + 1;
            lara.water_skin_empty2 = empty2_count + 1;
            combine_obj1 = (empty_count + 1) + (INV_WATERSKIN_EMPTY - 1);
        }
    }
    else if (lara.water_skin_empty1 != 1 && empty2_count != 5)
    {
        j = lara.water_skin_empty1 - 1;
        do
        {
            if (slotcount_2)
            {
                empty_count = empty_count - 1;
                empty2_count = empty2_count + 1;
                --slotcount_2;
            }
            --j;
        } while (j);
        lara.water_skin_empty1 = empty_count + 1;
        lara.water_skin_empty2 = empty2_count + 1;
        combine_obj1 = (empty2_count + 1) + (INV_WATERSKIN2_EMPTY - 1);
    }
}

void setup_objectlist_startposition_invitem(short inv_item)
{
    RING2D* inv = ring_2d[RING_INVENTORY];

    for (int i = 0; i < MAX_INVOBJ; i++)
    {
        if (inv_item == inv->current_obj_list[i].inv_item)
        {
            inv->cur_obj_in_list = i;
            inv = ring_2d[RING_INVENTORY]; // reset the old ring...
        }
    }
}

void setup_objectlist_startposition_objnumber(short object_number)
{
    RING2D* inv = ring_2d[RING_INVENTORY];

    for (int i = 0; i < MAX_INVOBJ; i++)
    {
        if (object_number == inventry_objects_list[inv->current_obj_list[i].inv_item].object_number)
        {
            inv->cur_obj_in_list = i;
            inv = ring_2d[RING_INVENTORY]; // reset the old ring...
        }
    }
}

void use_current_item(void)
{
    int rangeBackup = BinocularRange;
    BinocularRange = 0;
    old_lara_busy = false;
    lara_item->mesh_bits = -1;

    short inv_item = ring_2d[RING_INVENTORY]->current_obj_list[ring_2d[RING_INVENTORY]->cur_obj_in_list].inv_item;
    short object_number = inventry_objects_list[inv_item].object_number;

    /// WEAPON
    if (lara.water_status == LWS_ABOVEWATER || lara.water_status == LWS_WADE)
    {
        switch (object_number)
        {
            case PISTOLS_ITEM:
                lara.gun_request_type = LG_PISTOLS;
                if (lara.gun_status == LHS_ARMLESS && lara.gun_type == LG_PISTOLS)
                    lara.gun_status = LHS_DRAW;
                break;
            case SHOTGUN_ITEM:
                lara.gun_request_type = LG_SHOTGUN;
                if (lara.gun_status == LHS_ARMLESS && lara.gun_type == LG_SHOTGUN)
                    lara.gun_status = LHS_DRAW;
                break;
            case UZI_ITEM:
                lara.gun_request_type = LG_UZIS;
                if (lara.gun_status == LHS_ARMLESS && lara.gun_type == LG_UZIS)
                    lara.gun_status = LHS_DRAW;
                break;
            case REVOLVER_ITEM:
                lara.gun_request_type = LG_REVOLVER;
                if (lara.gun_status == LHS_ARMLESS && lara.gun_type == LG_REVOLVER)
                    lara.gun_status = LHS_DRAW;
                break;
            case GRENADE_GUN_ITEM:
                lara.gun_request_type = LG_GRENADEGUN;
                if (lara.gun_status == LHS_ARMLESS && lara.gun_type == LG_GRENADEGUN)
                    lara.gun_status = LHS_DRAW;
                break;
            case CROSSBOW_ITEM:
                lara.gun_request_type = LG_CROSSBOW;
                if (lara.gun_status == LHS_ARMLESS && lara.gun_type == LG_CROSSBOW)
                    lara.gun_status = LHS_DRAW;
                break;
        }
    }

    /// FLARE
    if (object_number == FLARE_INV_ITEM)
    {
        if (lara.gun_status == LHS_ARMLESS)
        {
            if (lara_item->state_current != STATE_LARA_CRAWL_IDLE
            &&  lara_item->state_current != STATE_LARA_CRAWL_FORWARD
            &&  lara_item->state_current != STATE_LARA_CRAWL_TURN_LEFT
            &&  lara_item->state_current != STATE_LARA_CRAWL_TURN_RIGHT
            &&  lara_item->state_current != STATE_LARA_CRAWL_BACK
            &&  lara_item->state_current != STATE_LARA_CRAWL_TO_CLIMB)
            {
                if (lara.gun_type != LG_FLARE)
                {
                    TrInput = IN_FLARE;
                    LaraGun();
                    TrInput = IN_NONE;
                    GLOBAL_invitemchosen = object_number;
                }
                return;
            }
        }
        else
        {
            SayNo();
        }
    }

    /// MEDIKIT
    if (object_number == BIGMEDI_ITEM)
    {
        if ((lara_item->hit_points > 0 && lara_item->hit_points < LARA_HITPOINTS) || lara.poisoned)
        {
            if (lara.large_medipack_count != INFINITE_AMMO)
                lara.large_medipack_count--;

            lara.poisoned_2 = 0;
            lara_item->hit_points += LARA_HITPOINTS;
            if (lara_item->hit_points > LARA_HITPOINTS)
                lara_item->hit_points = LARA_HITPOINTS;
            SoundEffect(SFX_MENU_MEDI, nullptr, SFX_ALWAYS);
            savegame_level.health_used++;
        }
        else
        {
            SayNo();
        }
    }

    if (object_number == SMALLMEDI_ITEM)
    {
        if ((lara_item->hit_points > 0 && lara_item->hit_points < LARA_HITPOINTS) || lara.poisoned)
        {
            if (lara.small_medipack_count != INFINITE_AMMO)
                lara.small_medipack_count--;

            lara.poisoned_2 = 0;
            lara_item->hit_points += LARA_HITPOINTS / 2;
            if (lara_item->hit_points > LARA_HITPOINTS)
                lara_item->hit_points = LARA_HITPOINTS;
            SoundEffect(SFX_MENU_MEDI, nullptr, SFX_ALWAYS);
            savegame_level.health_used++;
        }
        else
        {
            SayNo();
        }
    }
    else if (object_number != BINOCULARS_ITEM)
    {
        GLOBAL_invitemchosen = object_number;
        return;
    }

    /// BINOCULARS
    if (object_number == BINOCULARS_ITEM)
    {
        if ((lara_item->state_current == STATE_LARA_IDLE && lara_item->current_anim == ANIMATION_LARA_STAY_IDLE) || (lara.is_ducked && CHK_NOP(TrInput, IN_DUCK)))
        {
            old_lara_busy = true;
            BinocularRange = 128;
            if (lara.gun_status != LHS_ARMLESS)
                lara.gun_status = LHS_UNDRAW;
            GLOBAL_invitemchosen = object_number;
        }
    }

    if (rangeBackup)
        BinocularRange = rangeBackup;
    else
        BinocularOldCamera = camera.old_type;
}

void picked_up_object(short object_number)
{
    switch (object_number)
    {
        case WATERSKIN1_EMPTY:
            lara.water_skin_empty1 = 1;
            break;
        case WATERSKIN2_EMPTY:
            lara.water_skin_empty2 = 1;
            break;
        case UZI_ITEM:
            if (CHK_NOP(lara.uzi_type_carried, CR_PRESENT))
                lara.uzi_type_carried |= (CR_PRESENT | CR_AMMO1);
            if (lara.uzi_ammo_count != INFINITE_AMMO)
                lara.uzi_ammo_count += 30;
            break;
        case PISTOLS_ITEM:
            if (CHK_NOP(lara.pistols_type_carried, CR_PRESENT))
                lara.pistols_type_carried |= (CR_PRESENT | CR_AMMO1);
            lara.pistol_ammo_count = INFINITE_AMMO;
            return;
        case SHOTGUN_ITEM:
            if (CHK_NOP(lara.shotgun_type_carried, CR_PRESENT))
                lara.shotgun_type_carried |= (CR_PRESENT | CR_AMMO1);
            if (lara.shotgun_ammo1_count != INFINITE_AMMO)
                lara.shotgun_ammo1_count += (6 * 6);
            break;
        case REVOLVER_ITEM:
            if (CHK_NOP(lara.revolver_type_carried, CR_PRESENT))
                lara.revolver_type_carried |= (CR_PRESENT | CR_AMMO1);
            if (lara.revolver_ammo_count != INFINITE_AMMO)
                lara.revolver_ammo_count += 6;
            break;
        case CROSSBOW_ITEM:
            if (CHK_NOP(lara.crossbow_type_carried, CR_PRESENT))
                lara.crossbow_type_carried |= (CR_PRESENT | CR_AMMO1);
            if (lara.crossbow_ammo1_count != INFINITE_AMMO)
                lara.crossbow_ammo1_count += 10;
            break;
        case GRENADE_GUN_ITEM:
            if (CHK_NOP(lara.grenadegun_type_carried, CR_PRESENT))
                lara.grenadegun_type_carried |= (CR_PRESENT | CR_AMMO1);
            if (lara.grenade_ammo1_count != INFINITE_AMMO)
                lara.grenade_ammo1_count += 10;
            break;
        case SHOTGUN_AMMO1_ITEM:
            if (lara.shotgun_ammo1_count != INFINITE_AMMO)
                lara.shotgun_ammo1_count += (6 * 6);
            break;
        case SHOTGUN_AMMO2_ITEM:
            if (lara.shotgun_ammo2_count != INFINITE_AMMO)
                lara.shotgun_ammo2_count += (6 * 6);
            break;
        case GRENADE_GUN_AMMO1_ITEM:
            if (lara.grenade_ammo1_count != INFINITE_AMMO)
                lara.grenade_ammo1_count += 4;
            break;
        case GRENADE_GUN_AMMO2_ITEM:
            if (lara.grenade_ammo2_count != INFINITE_AMMO)
                lara.grenade_ammo2_count += 4;
            break;
        case GRENADE_GUN_AMMO3_ITEM:
            if (lara.grenade_ammo3_count != INFINITE_AMMO)
                lara.grenade_ammo3_count += 4;
            break;
        case CROSSBOW_AMMO1_ITEM:
            if (lara.crossbow_ammo1_count != INFINITE_AMMO)
                lara.crossbow_ammo1_count += 10;
            break;
        case CROSSBOW_AMMO2_ITEM:
            if (lara.crossbow_ammo2_count != INFINITE_AMMO)
                lara.crossbow_ammo2_count += 10;
            break;
        case CROSSBOW_AMMO3_ITEM:
            if (lara.crossbow_ammo3_count != INFINITE_AMMO)
                lara.crossbow_ammo3_count += 10;
            break;
        case REVOLVER_AMMO_ITEM:
            if (lara.revolver_ammo_count != INFINITE_AMMO)
                lara.revolver_ammo_count += 6;
            break;
        case UZI_AMMO_ITEM:
            if (lara.uzi_ammo_count != INFINITE_AMMO)
                lara.uzi_ammo_count += 30;
            break;
        case FLARE_INV_ITEM:
            if (lara.flare_count != INFINITE_AMMO)
                lara.flare_count += 12;
            break;
        case LASERSIGHT_ITEM:
            if (CHK_NOP(lara.pistols_type_carried, CR_LASERSIGHT)
            ||  CHK_NOP(lara.uzi_type_carried, CR_LASERSIGHT)
            ||  CHK_NOP(lara.shotgun_type_carried, CR_LASERSIGHT)
            ||  CHK_NOP(lara.grenadegun_type_carried, CR_LASERSIGHT)
            ||  CHK_NOP(lara.crossbow_type_carried, CR_LASERSIGHT)
            ||  CHK_NOP(lara.revolver_type_carried, CR_LASERSIGHT))
            {
                lara.lasersight = 1;
            }
            break;
        case BIGMEDI_ITEM:
            if (lara.large_medipack_count != INFINITE_AMMO)
                lara.large_medipack_count++;
            break;
        case SMALLMEDI_ITEM:
            if (lara.small_medipack_count != INFINITE_AMMO)
                lara.small_medipack_count++;
            break;
        case BINOCULARS_ITEM:
            lara.binoculars = 1;
            break;
        case CROWBAR_ITEM:
            lara.crowbar = 1;
            break;
        case CLOCKWORK_BEETLE:
            lara.clockwork_beetle |= 1;
            break;
        case CLOCKWORK_BEETLE_COMBO1:
            lara.clockwork_beetle |= 2;
            break;
        case CLOCKWORK_BEETLE_COMBO2:
            lara.clockwork_beetle |= 4;
            break;
        case EXAMINE1:
            lara.examine1 = 1;
            break;
        case EXAMINE2:
            lara.examine2 = 1;
            break;
        case EXAMINE3:
            lara.examine3 = 1;
            break;
        case PUZZLE_ITEM1:
        case PUZZLE_ITEM2:
        case PUZZLE_ITEM3:
        case PUZZLE_ITEM4:
        case PUZZLE_ITEM5:
        case PUZZLE_ITEM6:
        case PUZZLE_ITEM7:
        case PUZZLE_ITEM8:
        case PUZZLE_ITEM9:
        case PUZZLE_ITEM10:
        case PUZZLE_ITEM11:
        case PUZZLE_ITEM12:
            lara.puzzleitems[object_number - PUZZLE_ITEM1]++;
            break;
        case PUZZLE_ITEM1_COMBO1:
        case PUZZLE_ITEM1_COMBO2:
        case PUZZLE_ITEM2_COMBO1:
        case PUZZLE_ITEM2_COMBO2:
        case PUZZLE_ITEM3_COMBO1:
        case PUZZLE_ITEM3_COMBO2:
        case PUZZLE_ITEM4_COMBO1:
        case PUZZLE_ITEM4_COMBO2:
        case PUZZLE_ITEM5_COMBO1:
        case PUZZLE_ITEM5_COMBO2:
        case PUZZLE_ITEM6_COMBO1:
        case PUZZLE_ITEM6_COMBO2:
        case PUZZLE_ITEM8_COMBO1:
        case PUZZLE_ITEM8_COMBO2:
            lara.puzzleitemscombo |= (1 << (object_number + 69));
            break;
        case KEY_ITEM1:
        case KEY_ITEM2:
        case KEY_ITEM3:
        case KEY_ITEM4:
        case KEY_ITEM5:
        case KEY_ITEM6:
        case KEY_ITEM7:
        case KEY_ITEM8:
        case KEY_ITEM9:
        case KEY_ITEM10:
        case KEY_ITEM11:
        case KEY_ITEM12:
            lara.keyitems |= (1 << (object_number + 53));
            break;
        case KEY_ITEM1_COMBO1:
        case KEY_ITEM1_COMBO2:
        case KEY_ITEM2_COMBO1:
        case KEY_ITEM2_COMBO2:
        case KEY_ITEM3_COMBO1:
        case KEY_ITEM3_COMBO2:
        case KEY_ITEM4_COMBO1:
        case KEY_ITEM4_COMBO2:
        case KEY_ITEM5_COMBO1:
        case KEY_ITEM5_COMBO2:
        case KEY_ITEM6_COMBO1:
        case KEY_ITEM6_COMBO2:
        case KEY_ITEM7_COMBO1:
        case KEY_ITEM7_COMBO2:
        case KEY_ITEM8_COMBO1:
        case KEY_ITEM8_COMBO2:
            lara.keyitemscombo |= (1 << (object_number + 41));
            break;
        case PICKUP_ITEM1:
        case PICKUP_ITEM2:
        case PICKUP_ITEM3:
        case PICKUP_ITEM4:
            lara.pickupitems |= (1 << (object_number + 25));
            break;
        case PICKUP_ITEM1_COMBO1:
        case PICKUP_ITEM1_COMBO2:
        case PICKUP_ITEM2_COMBO1:
        case PICKUP_ITEM2_COMBO2:
        case PICKUP_ITEM3_COMBO1:
        case PICKUP_ITEM3_COMBO2:
        case PICKUP_ITEM4_COMBO1:
        case PICKUP_ITEM4_COMBO2:
            lara.pickupitemscombo |= 1 << (object_number + 21);
            break;
        case QUEST_ITEM1:
        case QUEST_ITEM2:
        case QUEST_ITEM3:
        case QUEST_ITEM4:
        case QUEST_ITEM5:
        case QUEST_ITEM6:
            lara.questitems |= (1 << (object_number + 4));
            break;
        default:
            S_Log(LT_Function, "not found any variables to store this object_number: %d", false, object_number); // just in case i missed something or missed implemented a new item...
            break;
    }
}

BOOL have_i_got_object(short object_number)
{
    if (object_number >= PUZZLE_ITEM1_COMBO1 && object_number <= PUZZLE_ITEM8_COMBO2)
        return (lara.puzzleitemscombo >> (object_number + 69)) & 1;
    else if (object_number >= PUZZLE_ITEM1 && object_number <= PUZZLE_ITEM12)
        return (lara.puzzleitems[object_number - PUZZLE_ITEM1]);
    else if (object_number >= KEY_ITEM1_COMBO1 && object_number <= KEY_ITEM8_COMBO2)
        return (lara.keyitemscombo >> (object_number + 41)) & 1;
    else if (object_number >= KEY_ITEM1 && object_number <= KEY_ITEM12)
        return (lara.keyitems >> (object_number + 53)) & 1;
    else if (object_number >= PICKUP_ITEM1_COMBO1 && object_number <= PICKUP_ITEM4_COMBO2)
        return (lara.pickupitemscombo >> (object_number + 21)) & 1;
    else if (object_number >= PICKUP_ITEM1 && object_number <= PICKUP_ITEM4)
        return (lara.pickupitems >> (object_number + 25)) & 1;
    else if (object_number >= QUEST_ITEM1 && object_number <= QUEST_ITEM6)
        return (lara.questitems >> (object_number + 4)) & 1;
    else if (object_number == CROWBAR_ITEM)
        return (lara.crowbar);
    else
        return 0;
}

void found_item_with_detector(short object_number)
{
    switch (object_number)
    {
        case PUZZLE_ITEM1:
        case PUZZLE_ITEM2:
        case PUZZLE_ITEM3:
        case PUZZLE_ITEM4:
        case PUZZLE_ITEM5:
        case PUZZLE_ITEM6:
        case PUZZLE_ITEM7:
        case PUZZLE_ITEM8:
        case PUZZLE_ITEM9:
        case PUZZLE_ITEM10:
        case PUZZLE_ITEM11:
        case PUZZLE_ITEM12:
            lara.puzzleitems[object_number - PUZZLE_ITEM1]--;
            break;
        case PUZZLE_ITEM1_COMBO1:
        case PUZZLE_ITEM1_COMBO2:
        case PUZZLE_ITEM2_COMBO1:
        case PUZZLE_ITEM2_COMBO2:
        case PUZZLE_ITEM3_COMBO1:
        case PUZZLE_ITEM3_COMBO2:
        case PUZZLE_ITEM4_COMBO1:
        case PUZZLE_ITEM4_COMBO2:
        case PUZZLE_ITEM5_COMBO1:
        case PUZZLE_ITEM5_COMBO2:
        case PUZZLE_ITEM6_COMBO1:
        case PUZZLE_ITEM6_COMBO2:
        case PUZZLE_ITEM8_COMBO1:
        case PUZZLE_ITEM8_COMBO2:
            lara.puzzleitemscombo &= ~(1 << (object_number + 69));
            break;
        case KEY_ITEM1:
        case KEY_ITEM2:
        case KEY_ITEM3:
        case KEY_ITEM4:
        case KEY_ITEM5:
        case KEY_ITEM6:
        case KEY_ITEM7:
        case KEY_ITEM8:
        case KEY_ITEM9:
        case KEY_ITEM10:
        case KEY_ITEM11:
        case KEY_ITEM12:
            lara.keyitems &= ~(1 << (object_number + 53));
            break;
        case KEY_ITEM1_COMBO1:
        case KEY_ITEM1_COMBO2:
        case KEY_ITEM2_COMBO1:
        case KEY_ITEM2_COMBO2:
        case KEY_ITEM3_COMBO1:
        case KEY_ITEM3_COMBO2:
        case KEY_ITEM4_COMBO1:
        case KEY_ITEM4_COMBO2:
        case KEY_ITEM5_COMBO1:
        case KEY_ITEM5_COMBO2:
        case KEY_ITEM6_COMBO1:
        case KEY_ITEM6_COMBO2:
        case KEY_ITEM7_COMBO1:
        case KEY_ITEM7_COMBO2:
        case KEY_ITEM8_COMBO1:
        case KEY_ITEM8_COMBO2:
            lara.keyitemscombo &= ~(1 << (object_number + 41));
            break;
        case PICKUP_ITEM1:
        case PICKUP_ITEM2:
        case PICKUP_ITEM3:
        case PICKUP_ITEM4:
            lara.pickupitems &= ~(1 << (object_number + 25));
            break;
        case PICKUP_ITEM1_COMBO1:
        case PICKUP_ITEM1_COMBO2:
        case PICKUP_ITEM2_COMBO1:
        case PICKUP_ITEM2_COMBO2:
        case PICKUP_ITEM3_COMBO1:
        case PICKUP_ITEM3_COMBO2:
        case PICKUP_ITEM4_COMBO1:
        case PICKUP_ITEM4_COMBO2:
            lara.pickupitemscombo &= ~(1 << (object_number + 21));
            break;
        case QUEST_ITEM1:
        case QUEST_ITEM2:
        case QUEST_ITEM3:
        case QUEST_ITEM4:
        case QUEST_ITEM5:
        case QUEST_ITEM6:
            lara.questitems &= ~(1 << (object_number + 4));
            break;
    }
}

int convert_obj_to_invobj(short object_number)
{
    for (int i = 0; i < MAX_INVOBJ; i++)
    {
        INVOBJ* list = &inventry_objects_list[i];
        if (object_number == list->object_number)
            return i;
    }

    return NO_ITEM;
}

void do_compass_mode(void)
{
    DrawThreeDeeObject2D(
        int(inventory_xpos + (phd_centerx * 0.00390625 * 60.0)),
        int(inventory_ypos + (phd_centery)),
        INV_COMPASS,
        128,
        0,
        0,
        0,
        0,
        FALSE);
}

void do_examine_mode(void)
{
    static int examine2_y = font_height;
    static int examine3_y = font_height * 5;
    short saved_scale;
    short inv_item = ring_2d[RING_INVENTORY]->current_obj_list[ring_2d[RING_INVENTORY]->cur_obj_in_list].inv_item;
    INVOBJ* obj = &inventry_objects_list[inv_item];


    if (examine_mode < 128)
        examine_mode += 8;
    if (examine_mode > 128)
        examine_mode = 128;

    switch (inv_item)
    {
        case INV_EXAMINE1:
            saved_scale = obj->scale;
            obj->scale = 300;

            DrawThreeDeeObject2D(
                int(inventory_xpos + (phd_centerx * 0.00390625   * STEP_L)),
                int(inventory_ypos + (phd_centery * 0.0083333338 * STEP_L) / 2),
                INV_EXAMINE1,
                examine_mode,
                0x8000,
                0x4000,
                0x4000,
                96,
                FALSE);

            obj->scale = saved_scale;
            break;
        case INV_EXAMINE2:
            saved_scale = obj->scale;
            obj->scale = 300;

            DrawThreeDeeObject2D(
                int(inventory_xpos + (phd_centerx * 0.00390625   * STEP_L)),
                int(inventory_ypos + (phd_centery * 0.0083333338 * STEP_L) / 2),
                INV_EXAMINE2,
                examine_mode,
                0,
                0,
                0,
                0,
                0);

            obj->scale = saved_scale;

            PrintString(phd_centerx, examine2_y,                     5, &gfStringWad[gfStringOffset[201]], FF_CENTER);
            PrintString(phd_centerx, examine2_y + phd_winheight / 2, 5, &gfStringWad[gfStringOffset[202]], FF_CENTER);
            break;
        case INV_EXAMINE3:
            saved_scale = obj->scale;
            obj->scale = 400;

            DrawThreeDeeObject2D(
                int(inventory_xpos + (phd_centerx * 0.00390625   * STEP_L)),
                int(inventory_ypos + (phd_centery * 0.0083333338 * STEP_L) / 2 - 8),
                INV_EXAMINE3,
                examine_mode,
                0x8000,
                0x4000,
                0x4000,
                96,
                0);

            obj->scale = saved_scale;

            PrintString(phd_centerx, examine3_y, 8, &gfStringWad[gfStringOffset[203]], FF_CENTER);
            break;
    }

    if (go_deselect)
    {
        SoundEffect(SFX_MENU_SELECT, nullptr, SFX_ALWAYS);
        go_deselect = false;
        examine_mode = 0;
    }
}

#ifdef DLL_INJECT
void injector::inject_inventory()
{
    this->inject(0x0043B760, show_inventory_2d);
    this->inject(0x0043B9B0, construct_inventory_2d);
    this->inject(0x0043BC30, do_debounced_input);
    this->inject(0x0043BD80, DrawThreeDeeObject2D);
    this->inject(0x0043BF10, DrawInventoryItem);
    this->inject(0x0043C400, go_and_load_game);
    this->inject(0x0043C410, go_and_save_game);
    this->inject(0x0043C420, construct_combine_object_list);
    this->inject(0x0043C5A0, insert_object_into_list_combine);
    this->inject(0x0043C610, construct_object_list);
    this->inject(0x0043CA90, insert_object_into_list_inventory);
    this->inject(0x0043CAE0, draw_current_object_list);
    this->inject(0x0043D450, handle_object_changeover);
    this->inject(0x0043D470, handle_inventry_menu);
    this->inject(0x0043DE30, fade_ammo_selector);
    this->inject(0x0043DED0, draw_ammo_selector);
    this->inject(0x0043E140, spinback);
    this->inject(0x0043E1B0, update_laras_weapons_status);
    this->inject(0x0043E250, is_item_currently_combinable);
    this->inject(0x0043E2F0, have_i_got_item);
    this->inject(0x0043E320, do_these_objects_combine);
    this->inject(0x0043E360, combine_these_two_objects);
    this->inject(0x0043E3D0, seperate_object);
    this->inject(0x0043E420, combine_revolver);
    this->inject(0x0043E480, combine_crossbow);
    this->inject(0x0043E4E0, combine_puzzle_item1);
    this->inject(0x0043E500, combine_puzzle_item2);
    this->inject(0x0043E520, combine_puzzle_item3);
    this->inject(0x0043E540, combine_puzzle_item4);
    this->inject(0x0043E560, combine_puzzle_item5);
    this->inject(0x0043E580, combine_puzzle_item6);
    this->inject(0x0043E5A0, combine_puzzle_item7);
    this->inject(0x0043E5C0, combine_puzzle_item8);
    this->inject(0x0043E5E0, combine_key_item1);
    this->inject(0x0043E600, combine_key_item2);
    this->inject(0x0043E620, combine_key_item3);
    this->inject(0x0043E640, combine_key_item4);
    this->inject(0x0043E660, combine_key_item5);
    this->inject(0x0043E680, combine_key_item6);
    this->inject(0x0043E6A0, combine_key_item7);
    this->inject(0x0043E6C0, combine_key_item8);
    this->inject(0x0043E6E0, combine_pickup_item1);
    this->inject(0x0043E700, combine_pickup_item2);
    this->inject(0x0043E720, combine_pickup_item3);
    this->inject(0x0043E740, combine_pickup_item4);
    this->inject(0x0043E760, combine_clockwork);
    this->inject(0x0043E770, combine_waterskin);
    this->inject(0x0043E830, setup_objectlist_startposition_invitem);
    this->inject(0x0043E860, setup_objectlist_startposition_objnumber);
    this->inject(0x0043E8A0, use_current_item);
    this->inject(0x0043EB80, picked_up_object);
    this->inject(0x0043EF60, have_i_got_object);
    this->inject(0x0043F050, found_item_with_detector);
    this->inject(0x0043F150, convert_obj_to_invobj);
    this->inject(0x0043F180, do_compass_mode);
    this->inject(0x0043F1E0, do_examine_mode);
}
#endif