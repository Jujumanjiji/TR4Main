#include "framework.h"
#include "lara1gun.h"
#include "lara2gun.h"
#include "control.h"
#include "effect2.h"
#include "items.h"
#include "larafire.h"
#include "sphere.h"
#include "sound.h"
#include "utils/utils.h"

void draw_shotgun_meshes(int weapon_type)
{
    lara.back_gun = LG_UNARMED;
    lara.mesh.hand_r = assign_meshes(weapon_meshes(weapon_type), HAND_R);
}

void undraw_shotgun_meshes(int weapon_type)
{
    lara.back_gun = weapon_object(weapon_type); // cause crash !!
    lara.mesh.hand_r = assign_meshes(LARA, HAND_R);
}

void ready_shotgun(int weapon_type)
{
    short *frame = objects[weapon_object(weapon_type)].frame_base;
    lara.gun_status = LHS_READY;
    lara.target = NULL;

    lara.l_arm.frame_base = frame;
    lara.l_arm.x_rot = 0;
    lara.l_arm.y_rot = 0;
    lara.l_arm.z_rot = 0;
    lara.l_arm.lock = FALSE;
    lara.l_arm.flash_gun = 0;

    lara.r_arm.frame_base = frame;
    lara.r_arm.x_rot = 0;
    lara.r_arm.y_rot = 0;
    lara.r_arm.z_rot = 0;
    lara.r_arm.lock = FALSE;
    lara.r_arm.flash_gun = 0;
}

void draw_shotgun(int weapon_type)
{
    ITEM_INFO* item;
    short itemNumber;

    // create the back item if not exist.
    if (lara.weapon_item == NO_ITEM)
    {
        itemNumber = CreateItem();
        if (itemNumber == NO_ITEM)
            MessageBox(NULL, "Error when creating shotgun type weapon, the CreateItem() returned NO_ITEM !", NULL, MB_OK|MB_ICONWARNING);
        lara.weapon_item = itemNumber;
        item = &items[itemNumber];
        item->object_number = weapon_object(weapon_type);
        item->current_anim = (weapon_type == LG_GRENADEGUN) ? (objects[item->object_number].anim_index) : (objects[item->object_number].anim_index + 1);
        item->current_frame = anims[item->current_anim].frame_base;
        /*if (lara.water_status == LWS_UNDERWATER) // TODO: for later (shotgun)
        {
            item->state_current = LSS_WATER_DRAW;
            item->state_next = LSS_WATER_DRAW;
        }*/
        item->state_current = LSS_LAND_DRAW;
        item->state_next = LSS_LAND_DRAW;
        item->status = FITEM_ACTIVE;
        item->room_number = NO_ROOM;
        lara.r_arm.frame_base = objects[item->object_number].frame_base;
        lara.l_arm.frame_base = lara.r_arm.frame_base;
    }
    else
    {
        item = &items[lara.weapon_item];
    }

    AnimateItem(item);
    if (item->state_current == LSS_LAND_DRAW)
    {
        if (weapons[weapon_type].draw_frame == GetCurrentFrame(item))
            draw_shotgun_meshes(weapon_type);
        else if (lara.water_status == LWS_UNDERWATER)
            item->state_next = LSS_WATER_AIM;
    }
    else if (item->state_current == LSS_LAND_AIM)
    {
        ready_shotgun(weapon_type);
    }

    lara.r_arm.frame_base = anims[item->current_anim].frame_ptr;
    lara.l_arm.frame_base = lara.r_arm.frame_base;
    lara.r_arm.frame_curr = GetCurrentFrame(item);
    lara.l_arm.frame_curr = lara.r_arm.frame_curr;
    lara.r_arm.anim_curr = item->current_anim;
    lara.l_arm.anim_curr = lara.r_arm.anim_curr;
}

void undraw_shotgun(int weapon_type)
{
    ITEM_INFO* item = &items[lara.weapon_item];

    switch (lara.water_status)
    {
        default:
        case LWS_SURFACE:
        case LWS_ABOVEWATER:
        case LWS_WADE: // TODO: change it to WATER_UNDRAW etc.. when you have a underwater weapon.
        case LWS_UNDERWATER:
            item->state_next = LSS_LAND_UNDRAW;
            break;
    }

    AnimateItem(item);

    if (item->status == FITEM_DEACTIVATED)
    {
        lara.gun_status = LG_UNARMED;
        lara.target = NULL;
        lara.r_arm.y_rot = 0;
        lara.l_arm.y_rot = 0;
        KillItem(lara.weapon_item);
        lara.weapon_item = NO_ITEM;
        lara.r_arm.frame_curr = 0;
        lara.l_arm.frame_curr = 0;
    }
    else if (item->state_current == LSS_LAND_UNDRAW && GetCurrentFrame(item) == 23)
    {
        undraw_shotgun_meshes(weapon_type);
    }

    lara.r_arm.frame_base = anims[item->current_anim].frame_ptr;
    lara.l_arm.frame_base = lara.r_arm.frame_base;
    lara.r_arm.frame_curr = GetCurrentFrame(item);
    lara.l_arm.frame_curr = lara.r_arm.frame_curr;
    lara.r_arm.anim_curr = item->current_anim;
    lara.l_arm.anim_curr = lara.r_arm.anim_curr;
}

void shotgun_handler(int weapon_type)
{
    WEAPON_INFO* winfo = &weapons[weapon_type];
    PHD_VECTOR pos;

    LaraGetNewTarget(winfo);
    if (TrInput & IN_ACTION)
        LaraTargetInfo(winfo);
    AimWeapon(winfo, &lara.l_arm);

    if (lara.l_arm.lock)
    {
        lara.torso_x_rot = lara.l_arm.x_rot;
        lara.torso_y_rot = lara.l_arm.y_rot;
        if (camera.old_type != LOOK_CAMERA && !BinocularRange)
        {
            lara.head_x_rot = 0;
            lara.head_y_rot = 0;
        }
    }

    if (weapon_type != LG_REVOLVER)
        animate_shotgun(weapon_type);
    else
        animate_pistols(LG_REVOLVER);

    if (lara.r_arm.flash_gun)
    {
        switch (weapon_type)
        {
            case LG_SHOTGUN:
                pos.x = SHOTGUN_GUNPOS_X;
                pos.y = SHOTGUN_GUNPOS_Y;
                pos.z = SHOTGUN_GUNPOS_Z;
                GetLaraHandAbsPosition(&pos, HAND_R);
                TriggerDynamicSwap(pos.x, pos.y, pos.z, 12, (GetRandomControl() & 0x3F) + 192, (GetRandomControl() & 0x1F) + 128, (GetRandomControl() & 0x3F));
                break;
            case LG_REVOLVER:
                pos.x = REVOLVER_GUNPOS_X;
                pos.y = REVOLVER_GUNPOS_Y;
                pos.z = REVOLVER_GUNPOS_Z;
                GetLaraHandAbsPosition(&pos, HAND_R);
                TriggerDynamicSwap(pos.x, pos.y, pos.z, 12, (GetRandomControl() & 0x3F) - 64, (GetRandomControl() & 0x1F) + 128, (GetRandomControl() & 0x3F));
                break;
        }
    }
}

void animate_shotgun(int weapon_type)
{
    ITEM_INFO* item = &items[lara.weapon_item];
    PHD_VECTOR pos;
    short frame;
    short fired = FALSE, reload = FALSE;

    if (SmokeCountL)
    {
        switch (SmokeWeapon)
        {
            case LG_SHOTGUN:
                pos.x = SHOTGUN_GUNPOS_X;
                pos.y = SHOTGUN_GUNPOS_Y;
                pos.z = SHOTGUN_GUNPOS_Z;
                break;
            case LG_GRENADEGUN:
                pos.x = GRENADEGUN_GUNPOS_X;
                pos.y = GRENADEGUN_GUNPOS_Y;
                pos.z = GRENADEGUN_GUNPOS_Z;
                break;
        }

        GetLaraHandAbsPosition(&pos, HAND_R);
        if (lara_item->mesh_bits)
            TriggerGunSmoke(pos.x, pos.y, pos.z, 0, 0, 0, 0, SmokeWeapon, SmokeCountL);
    }

    frame = GetCurrentFrame(item);
    switch (item->state_current)
    {
        case LSS_LAND_AIM:
            fired = FALSE;

            if (reload)
            {
                item->state_next = LSS_LAND_RELOAD;
                reload = FALSE;
            }
            else if (lara.water_status == LWS_UNDERWATER)
            {
                item->state_next = LSS_WATER_AIM;
            }
            else if ((CHK_ANY(TrInput, IN_ACTION) && !lara.target) || lara.l_arm.lock)
            {
                item->state_next = LSS_LAND_RECOIL;
            }
            else
            {
                item->state_next = LSS_LAND_UNAIM;
            }
            break;
        case LSS_LAND_RECOIL:
            if (frame == 0)
            {
                item->state_next = LSS_LAND_UNAIM;

                if (lara.water_status != LWS_UNDERWATER && !reload)
                {
                    if (CHK_ANY(TrInput, IN_ACTION) && (!lara.target || lara.l_arm.lock))
                    {
                        switch (weapon_type)
                        {
                            case LG_SHOTGUN:
                                fire_shotgun();
                                fired = TRUE;
                                break;
                            case LG_GRENADEGUN:
                                fire_grenade();
                                fired = FALSE;
                                reload = TRUE;
                                break;
                            case LG_CROSSBOW:
                                fire_crossbow(nullptr);
                                fired = FALSE;
                                break;
                        }

                        item->state_next = LSS_LAND_RECOIL;
                    }
                    else if (lara.l_arm.lock)
                    {
                        item->state_next = LSS_LAND_AIM;
                    }
                }

                if (item->state_next != LSS_LAND_RECOIL && fired)
                {
                    SoundEffect(EXPLOSION2, &lara_item->pos, PITCH_SHIFT | 0x5000000);
                    fired = FALSE;
                }
            }
            else if (fired)
            {
                SoundEffect(EXPLOSION2, &lara_item->pos, PITCH_SHIFT | 0x5000000);
                SoundEffect(WATERFALL_LOOP, &lara_item->pos, 0);
            }
            else if (weapon_type == LG_SHOTGUN && CHK_NOP(TrInput, IN_ACTION) && !lara.l_arm.lock)
            {
                item->state_next = LSS_LAND_UNAIM;
            }

            if (frame == 12 && weapon_type == LG_SHOTGUN)
                TriggerGunShell(TRUE, SHOTGUNSHELL, LG_SHOTGUN);
            break;

        // TODO: add WATER states !
    }

    AnimateItem(item);
    lara.r_arm.frame_base = anims[item->current_anim].frame_ptr;
    lara.l_arm.frame_base = lara.r_arm.frame_base;
    lara.r_arm.frame_curr = GetCurrentFrame(item);
    lara.l_arm.frame_curr = lara.r_arm.frame_curr;
    lara.r_arm.anim_curr = item->current_anim;
    lara.l_arm.anim_curr = lara.r_arm.anim_curr;
}

void injector::inject_lara1gun()
{
    this->inject(0x00428E40, draw_shotgun_meshes);
    this->inject(0x00428E70, undraw_shotgun_meshes);
    this->inject(0x00428EA0, ready_shotgun);
    this->inject(0x0042AE50, draw_shotgun);
    this->inject(0x0042AFE0, undraw_shotgun);
    this->inject(0x00428F10, shotgun_handler);
    this->inject(0x0042B100, animate_shotgun);
}