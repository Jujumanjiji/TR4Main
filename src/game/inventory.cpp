#include "framework.h"
#include "inventory.h"
#include "3d_gen.h"
#include "3d_gen_a.h"
#include "draw.h"
#include "lara.h"
#include "sound.h"
#include "loadsave.h"
#include "text.h"
#include "utils.h"

void construct_inventory_2D(void)
{
    inventory_camera_angle = SECTOR(4);
    examine_mode = FALSE;
    stats_mode = FALSE;

    AlterFOV(DEFAULT_FOV);
    lara.busy = FALSE;

    GLOBAL_invitemchosen = NO_ITEM;
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
    loading_or_saving = false;
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
                setup_objectlist_startposition2(GLOBAL_invitemlastchosen);
            }
            else
            {
                if (GLOBAL_invitemlastchosen >= INV_WATERSKIN_EMPTY && GLOBAL_invitemlastchosen <= INV_WATERSKIN1_3)
                {
                    for (int i = INV_WATERSKIN_EMPTY; i <= INV_WATERSKIN1_3; i++)
                    {
                        if (have_i_got_item(i))
                            setup_objectlist_startposition2(i);
                    }
                }
                else if (GLOBAL_invitemlastchosen >= INV_WATERSKIN2_EMPTY && GLOBAL_invitemlastchosen <= INV_WATERSKIN2_5)
                {
                    for (int i = INV_WATERSKIN2_EMPTY; i <= INV_WATERSKIN2_5; i++)
                    {
                        if (have_i_got_item(i))
                            setup_objectlist_startposition2(i);
                    }
                }
            }

            GLOBAL_invitemlastchosen = NO_ITEM;
        }
    }
    else
    {
        if (have_i_got_object(GLOBAL_invEnter))
            setup_objectlist_startposition(GLOBAL_invEnter);
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
            short compass_speed = (short)(inventory_camera_angle * 4 * rcossin_tbl[(compass_needle & 63) << 10 >> 3] >> W2V_SHIFT);
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
    return LoadGame();
}

int go_and_save_game(void)
{
    return SaveGame();
}

void construct_combine_object_list(void)
{
    ring2D[RING_COMBINE]->num_obj_in_list = NULL;
    for (int i = 0; i < MAX_INVOBJ; i++)
        ring2D[RING_COMBINE]->current_obj_list[i].inv_item = NO_ITEM;

    if (CHK_NOP(gf_level_flags, SLEV_YOUNG_LARA))
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

    if (CHK_ANY(lara.clockwork_beetle, CLOCK_COMBO1_PRESENT))
        insert_object_into_list_combine(INV_CLOCKWORK_BEETLE_COMBO1);
    if (CHK_ANY(lara.clockwork_beetle, CLOCK_COMBO2_PRESENT))
        insert_object_into_list_combine(INV_CLOCKWORK_BEETLE_COMBO2);

    ring2D[RING_COMBINE]->obj_list_movement = 0;
    ring2D[RING_COMBINE]->cur_obj_in_list = 0;
    ring2D[RING_COMBINE]->active = FALSE;
}

void insert_object_into_list_combine(short num)
{
    RING2D* normal = ring2D[RING_INVENTORY];
    RING2D* combine = ring2D[RING_COMBINE];
    INVLIST* list_inv = normal->current_obj_list;
    INVLIST* list_cmb = combine->current_obj_list;

    if (options_table[num] & 9)
    {
        if (list_inv[normal->cur_obj_in_list].inv_item != num)
        {
            list_cmb[combine->num_obj_in_list].inv_item = num;
            list_cmb[combine->num_obj_in_list].yrot = 0;
            list_cmb[combine->num_obj_in_list].bright = 32; // RGBA_TO_DWORD(32, 32, 32, 0), in DrawInventoryItem()
            combine->num_obj_in_list++;
        }
    }
}

void construct_object_list(void)
{
    ring2D[RING_INVENTORY]->num_obj_in_list = NULL;
    for (int i = 0; i < MAX_INVOBJ; i++)
        ring2D[RING_INVENTORY]->current_obj_list[i].inv_item = NO_ITEM;

    current_pistols_ammo_type = NULL;
    current_uzi_ammo_type = NULL;
    current_revolver_ammo_type = NULL;
    current_shotgun_ammo_type = NULL;
    current_grenadegun_ammo_type = NULL;
    current_crossbow_ammo_type = NULL;

    if (CHK_NOP(gf_level_flags, SLEV_YOUNG_LARA))
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
                current_crossbow_ammo_type = AINV_AMMO2;
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

    ring2D[RING_INVENTORY]->obj_list_movement = 0;
    ring2D[RING_INVENTORY]->cur_obj_in_list = 0;
    ring2D[RING_COMBINE]->obj_list_movement = 0;
    ring2D[RING_COMBINE]->cur_obj_in_list = 0;
    handle_object_changeover();
    ring2D[RING_INVENTORY]->active = TRUE;
    ring2D[RING_COMBINE]->active = FALSE;
    ammo_active = false;
}

void insert_object_into_list_inventory(short num)
{
    RING2D* normal = ring2D[RING_INVENTORY];
    INVLIST* list_inv = normal->current_obj_list;

    list_inv[normal->num_obj_in_list].inv_item = num;
    list_inv[normal->num_obj_in_list].yrot = 0;
    list_inv[normal->num_obj_in_list].bright = 32; // RGBA_TO_DWORD(32, 32, 32, 0), in DrawInventoryItem()
    normal->num_obj_in_list++;
}

void draw_current_object_list(INVENTORY_RING_TYPE ring_type)
{
    RING2D* normal = ring2D[ring_type];
    if (normal->num_obj_in_list <= 0)
        return;

    INVOBJ* inv_item;
    INVLIST* inv_list;
    WORD pos_y;
    char buffer[128];
    int shade = 0;
    int count = 0;
    int minobj = 0;
    int maxobj = 0;
    int n, current_obj = 0;
    int xoff = 0;
    int i = 0;
    
    switch (ring_type)
    {
        case RING_INVENTORY:
            switch (normal_ring_fade_dir)
            {
                case 1:
                    if (normal_ring_fade_val < 128)
                        normal_ring_fade_val += 32;
                    
                    if (normal_ring_fade_val > 128)
                    {
                        normal_ring_fade_val = 128;
                        normal_ring_fade_dir = 0;

                        normal->active = TRUE;
                        menu_active = true;
                    }
                    break;
                case 2:
                    if (normal_ring_fade_val > 0)
                        normal_ring_fade_val -= 32;

                    if (normal_ring_fade_val < 0)
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
                            setup_objectlist_startposition2(combine_obj1);
                        }
                        else if (seperate_type_flag)
                        {
                            seperate_object(normal->current_obj_list[normal->cur_obj_in_list].inv_item);
                        }

                        handle_object_changeover();
                    }
                    break;
            }
            break;
        case RING_COMBINE:
            ammo_selector_fade_val = 0;
            ammo_selector_fade_dir = 0;

            switch (combine_ring_fade_dir)
            {
                case 1:

                    break;
                case 2:
                    
                    break;
            }
            break;
    }

    if (normal->num_obj_in_list != 1)
        xoff = (normal->obj_list_movement * objlist_spacing) >> 16;

    switch (normal->num_obj_in_list)
    {
    case 2:
        minobj = -1;
        maxobj = 0;
        n = normal->cur_obj_in_list - 1;
        break;
    case 3:
    case 4:
        minobj = -2;
        maxobj = 1;
        n = normal->cur_obj_in_list - 2;
        break;
    default:
        if (normal->num_obj_in_list >= 5)
        {
            minobj = -3;
            maxobj = 2;
            n = normal->cur_obj_in_list - 3;
        }
        break;
    }

    if (n < 0)
        n += normal->num_obj_in_list;

    if (normal->obj_list_movement < 0)
        maxobj++;

    i = minobj;
    if (maxobj > minobj)
    {
        while(maxobj >= i)
        {
            if (minobj == i)
            {
                if (normal->obj_list_movement < 0)
                    shade = 0;
                else
                    shade = (normal->obj_list_movement << 7) >> 16;
            }
            else
            {
                if (i == minobj + 1 && maxobj != i)
                {
                    shade = 128;
                    if (normal->obj_list_movement < 0)
                        shade = -128 * normal->obj_list_movement >> 16;
                    else
                        shade = 128 - ((normal->obj_list_movement << 7) >> 16);
                }
                else
                {
                    shade = 128;
                    if (minobj == maxobj)
                    {
                        if (normal->obj_list_movement >= 0)
                            shade = 128;
                        else
                            shade = 128 - (-(128 * normal->obj_list_movement) >> 16);
                    }
                }
            }

            if (minobj == 0 && maxobj == 0)
                shade = 128;

            switch (ring_type)
            {
                case RING_INVENTORY:
                    if (normal_ring_fade_val < 128 && shade != 0)
                        shade = normal_ring_fade_val;
                    break;
                case RING_COMBINE:
                    if (combine_ring_fade_val < 128 && shade != 0)
                        shade = combine_ring_fade_val;
                    break;
            }

            inv_item = &inventry_objects_list[normal->current_obj_list[n].inv_item];
            if (i == 0)
            {
                switch (inv_item->object_number)
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
                        if (lara.shotgun_ammo1_count != INFINITE_AMMO)
                            count = lara.shotgun_ammo1_count / 6;
                        else
                            count = lara.shotgun_ammo1_count;
                        break;
                    case SHOTGUN_AMMO2_ITEM:
                        if (lara.shotgun_ammo2_count != INFINITE_AMMO)
                            count = lara.shotgun_ammo2_count / 6;
                        else
                            count = lara.shotgun_ammo2_count;
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
                        if ((inv_item->object_number - PUZZLE_ITEM1) < 12)
                            count = lara.puzzleitems[inv_item->object_number - PUZZLE_ITEM1];
                        //if ((inv_item->object_number - KEY_ITEM1) < 12)
                        //    count = lara.keyitems & 1 << (inv_item->object_number - KEY_ITEM1);
                        break;
                }

                if (count != 0)
                {
                    if (count == INFINITE_AMMO)
                        sprintf(buffer, &gfStringWad[gfStringOffset[STR_UNLIMITED]], &gfStringWad[gfStringOffset[inv_item->name]]);
                    else
                        sprintf(buffer, "%d x %s", count, &gfStringWad[gfStringOffset[inv_item->name]]);
                }
                else
                {
                    sprintf(buffer, &gfStringWad[gfStringOffset[inv_item->name]]);
                }

                if (ring_type == RING_COMBINE)
                    pos_y = phd_centery + (phd_winymax + 1) * 0.0625 * 3.0;
                else
                    pos_y = phd_centery - (phd_winymax + 1) * 0.0625 * 3.0;
                PrintString(phd_centerx, pos_y, 8, buffer, FF_CENTER);
            }

            if (i == 0 && normal->obj_list_movement == 0)
            {
                current_obj = n;
                if (inv_item->flags & 2)
                    normal->current_obj_list[n].yrot += 1023;
            }
            else
            {
                current_obj = n;
                spinback(&normal->current_obj_list[n].yrot);
            }

            inv_list = &normal->current_obj_list[current_obj];
            int moving_turn;
            if (normal->obj_list_movement != 0)
                moving_turn = normal->obj_list_movement > 0 ? -1 : 1;
            else
                moving_turn = 0;

            if (i == moving_turn)
            {
                if (inv_list->bright < 160)
                    inv_list->bright += 16;
                if (inv_list->bright > 160)
                    inv_list->bright = 160;
            }
            else
            {
                if (inv_list->bright > 32)
                    inv_list->bright -= 16;
                if (inv_list->bright < 32)
                    inv_list->bright = 32;
            }

            int y, centerx, centery;
            int inventoryX, inventoryY;
            if (ring_type == RING_INVENTORY)
                y = 42;
            else
                y = 190;

            centerx = (phd_centerx * 0.00390625   * STEP_L);
            centery = (phd_centery * 0.0083333338 * y);
            inventoryX = inventory_xpos + centerx + (i * objlist_spacing) + xoff;
            inventoryY = inventory_ypos + centery;
            DrawThreeDeeObject2D(inventoryX, inventoryY, inv_list->inv_item, shade, NULL, inv_list->yrot, NULL, inv_list->bright, FALSE);

            ++n;
            if (n >= normal->num_obj_in_list)
                n = 0;

            ++i;
        }

        if (normal->active && normal->num_obj_in_list != 1)
        {
            if (ring_type != RING_COMBINE || combine_ring_fade_val == 128)
            {
                if (normal->obj_list_movement > 0)
                    normal->obj_list_movement += 0x2000;
                else if (normal->obj_list_movement < 0)
                    normal->obj_list_movement -= 0x2000;

                if (go_left && normal->obj_list_movement == 0)
                {
                    SoundEffect(SFX_MENU_ROTATE, nullptr, SFX_ALWAYS);
                    normal->obj_list_movement += 0x2000;
                    if (ammo_selector_flag)
                        ammo_selector_fade_dir = 2;
                }
                
                if (go_right && normal->obj_list_movement == 0)
                {
                    SoundEffect(SFX_MENU_ROTATE, nullptr, SFX_ALWAYS);
                    normal->obj_list_movement -= 0x2000;
                    if (ammo_selector_flag)
                        ammo_selector_fade_dir = 2;
                }

                if (normal->obj_list_movement < MAXWORD)
                {
                    if (normal->obj_list_movement < -MAXWORD)
                    {
                        normal->cur_obj_in_list++;

                        if (normal->cur_obj_in_list >= normal->num_obj_in_list)
                            normal->cur_obj_in_list = 0;
                        normal->obj_list_movement = 0;
                        if (!ring_type)
                            handle_object_changeover();
                    }
                }
                else
                {
                    normal->cur_obj_in_list--;

                    if (normal->cur_obj_in_list < 0)
                        normal->cur_obj_in_list = normal->num_obj_in_list - 1;
                    normal->obj_list_movement = 0;
                    if (!ring_type)
                        handle_object_changeover();
                }
            }
        }
    }
}

#ifdef DLL_INJECT
void injector::inject_inventory()
{
    this->inject(0x0043B9B0, construct_inventory_2D);
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
}
#endif