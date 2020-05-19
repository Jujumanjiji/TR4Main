#include "framework.h"
#include "lara.h"
#include "control.h"
#include "effect2.h"
#include "items.h"
#include "sphere.h"
#include "sound.h"
#include "utils.h"

void draw_shotgun_meshes(int weapon_type)
{
    lara.back_gun = LG_UNARMED;
    lara.mesh.hand_r = classic_meshes(weapon_meshes(weapon_type), HAND_R);
}

void undraw_shotgun_meshes(int weapon_type)
{
    lara.back_gun = weapon_object(weapon_type);
    lara.mesh.hand_r = classic_meshes(LARA, HAND_R);
}

void ready_shotgun(int weapon_type)
{
    short *frame = Objects[weapon_object(weapon_type)].frameBase;
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

    // create the back item if not exist.
    if (lara.weapon_item == NO_ITEM)
    {
        lara.weapon_item = CreateItem();
        item = &Items[lara.weapon_item];
        item->objectNumber = weapon_object(weapon_type);
        item->animNumber = (weapon_type == LG_GRENADEGUN) ? (Objects[item->objectNumber].animIndex) : (Objects[item->objectNumber].animIndex + 1);
        item->frameNumber = Anims[item->animNumber].frameBase;
        item->currentAnimState = W_DRAW;
        item->goalAnimState = W_DRAW;
        item->status = FITEM_ACTIVE;
        item->roomNumber = NO_ROOM;
        lara.l_arm.frame_base = lara.r_arm.frame_base = Objects[item->objectNumber].frameBase;
    }
    else
    {
        item = &Items[lara.weapon_item];
    }

    AnimateItem(item);

    if (item->currentAnimState == W_AIM || item->currentAnimState == W_UAIM)
        ready_shotgun(weapon_type);
    else if ((item->frameNumber - Anims[item->animNumber].frameBase) == weapons[weapon_type].draw_frame)
        draw_shotgun_meshes(weapon_type);
    else if (lara.water_status == LWS_UNDERWATER)
        item->goalAnimState = W_UAIM;

    lara.l_arm.frame_base = lara.r_arm.frame_base = Anims[item->animNumber].framePtr;
    lara.l_arm.frame_number = lara.r_arm.frame_number = item->frameNumber - Anims[item->animNumber].frameBase;
    lara.l_arm.anim_number  = lara.r_arm.anim_number  = item->animNumber;
}

void undraw_shotgun(int weapon_type)
{
    ITEM_INFO* item = &Items[lara.weapon_item];
    if (lara.water_status == LWS_SURFACE)
        item->goalAnimState = W_SURF_UNDRAW;
    else
        item->goalAnimState = W_UNDRAW;

    AnimateItem(item);

    if (item->status == FITEM_DEACTIVATED)
    {
        lara.gun_status = LG_UNARMED;
        lara.target = NULL;
        lara.r_arm.y_rot = 0;
        lara.l_arm.y_rot = 0;
        KillItem(lara.weapon_item);
        lara.weapon_item = NO_ITEM;
        lara.r_arm.frame_number = 0;
        lara.l_arm.frame_number = 0;
    }
    else if (item->currentAnimState == W_UNDRAW && GetCurrentFrame(item) == 21)
    {
        undraw_shotgun_meshes(weapon_type);
    }

    lara.l_arm.frame_base   = lara.r_arm.frame_base   = Anims[item->animNumber].framePtr;
    lara.l_arm.frame_number = lara.r_arm.frame_number = item->frameNumber - Anims[item->animNumber].frameBase;
    lara.l_arm.anim_number  = lara.r_arm.anim_number  = item->animNumber;
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

#ifdef DEBUG_CHEAT
    if (CHK_EXI(TrInput, IN_SPRINT))
    {
        lara.revolver_ammo_count = INFINITE_AMMO;
        lara.shotgun_ammo1_count = INFINITE_AMMO;
        lara.shotgun_ammo2_count = INFINITE_AMMO;
        lara.grenade_ammo1_count = INFINITE_AMMO;
        lara.grenade_ammo2_count = INFINITE_AMMO;
        lara.grenade_ammo3_count = INFINITE_AMMO;
        lara.crossbow_ammo1_count = INFINITE_AMMO;
        lara.crossbow_ammo2_count = INFINITE_AMMO;
        lara.crossbow_ammo3_count = INFINITE_AMMO;
    }
#endif

    if (weapon_type == LG_REVOLVER)
        animate_pistols(weapon_type);
    else
        animate_shotgun(weapon_type);

    if (lara.r_arm.flash_gun)
    {
        switch (weapon_type)
        {
            case LG_SHOTGUN:
                pos.x = SHOTGUN_GUNPOS_X;
                pos.y = SHOTGUN_GUNPOS_Y;
                pos.z = SHOTGUN_GUNPOS_Z;
                GetLaraJointAbsPosition(&pos, JHAND_R);
                TriggerDynamicSwap(pos.x, pos.y, pos.z, 12, (GetRandomControl() & 0x3F) + 192, (GetRandomControl() & 0x1F) + 128, (GetRandomControl() & 0x3F));
                break;
            case LG_REVOLVER:
                pos.x = REVOLVER_GUNPOS_X;
                pos.y = REVOLVER_GUNPOS_Y;
                pos.z = REVOLVER_GUNPOS_Z;
                GetLaraJointAbsPosition(&pos, JHAND_R);
                TriggerDynamicSwap(pos.x, pos.y, pos.z, 12, (GetRandomControl() & 0x3F) - 64, (GetRandomControl() & 0x1F) + 128, (GetRandomControl() & 0x3F));
                break;
        }
    }
}

void animate_shotgun(int weapon_type)
{
    ITEM_INFO* item = &Items[lara.weapon_item];
    PHD_VECTOR pos;
    static BOOL m16_fired = FALSE, harpoon_reload = FALSE;
    short frame;

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
            default:
                pos.x = 0;
                pos.y = 0;
                pos.z = 0;
                break;
        }

        GetLaraJointAbsPosition(&pos, JHAND_R);
        if (LaraItem->meshBits)
            TriggerGunSmoke(pos.x, pos.y, pos.z, 0, 0, 0, 0, SmokeWeapon, SmokeCountL);
    }

    frame = GetCurrentFrame(item);
    switch (item->currentAnimState)
    {
        case W_AIM:
            m16_fired = FALSE;

            if (harpoon_reload)
            {
                item->goalAnimState = W_RELOAD;
                harpoon_reload = FALSE;
            }
            else if (lara.water_status == LWS_UNDERWATER)
                item->goalAnimState = W_UAIM;
            else if ((CHK_EXI(TrInput, IN_ACTION) && !lara.target) || lara.l_arm.lock)
                item->goalAnimState = W_RECOIL;
            else
                item->goalAnimState = W_UNAIM;
            break;
        case W_RECOIL:
            if (frame == 0)
            {
                item->goalAnimState = W_UNAIM;

                if (lara.water_status != LWS_UNDERWATER && !harpoon_reload)
                {
                    if (CHK_EXI(TrInput, IN_ACTION) && (!lara.target || lara.l_arm.lock))
                    {
                        switch (weapon_type)
                        {
                            case LG_SHOTGUN:
                                fire_shotgun();
                                break;
                            case LG_GRENADEGUN:
                                fire_grenade();
                                break;
                            case LG_CROSSBOW:
                                if (!LaserSight) // dont delete more ammo that necessary !
                                    fire_crossbow(nullptr);
                                break;
                        }

                        item->goalAnimState = W_RECOIL;
                    }
                    else if (lara.l_arm.lock)
                    {
                        item->goalAnimState = W_AIM;
                    }
                }

                if (item->goalAnimState != W_RECOIL && m16_fired)
                {
                    SoundEffect(SFX_EXPLOSION1, &LaraItem->pos, PITCH_SHIFT | 0x5000000);
                    m16_fired = FALSE;
                }
            }
            else if (m16_fired)
            {
                SoundEffect(SFX_EXPLOSION1, &LaraItem->pos, PITCH_SHIFT | 0x5000000);
                SoundEffect(SFX_HECKLER_KOCH_FIRE, &LaraItem->pos, 0);
            }
            else if (weapon_type == LG_SHOTGUN && CHK_NOP(TrInput, IN_ACTION) && !lara.l_arm.lock)
            {
                item->goalAnimState = W_UNAIM;
            }

            if (frame == 12 && weapon_type == LG_SHOTGUN)
                TriggerGunShell(TRUE, SHOTGUNSHELL, LG_SHOTGUN);
            break;

        // TODO: add WATER states !
        case W_UAIM:
            break;
        case W_URECOIL:
            break;
    }

    AnimateItem(item);
    lara.l_arm.frame_base = lara.r_arm.frame_base = Anims[item->animNumber].framePtr;
    lara.l_arm.frame_number = lara.r_arm.frame_number = item->frameNumber - Anims[item->animNumber].frameBase;
    lara.l_arm.anim_number  = lara.r_arm.anim_number  = item->animNumber;
}

#ifdef DLL_INJECT
void injector::f_game::inject_lara1gun()
{
    inject(0x00428E40, draw_shotgun_meshes);
    inject(0x00428E70, undraw_shotgun_meshes);
    inject(0x00428EA0, ready_shotgun);
    inject(0x0042AE50, draw_shotgun);
    inject(0x0042AFE0, undraw_shotgun);
    inject(0x00428F10, shotgun_handler);
    inject(0x0042B100, animate_shotgun);
}
#endif