#include "framework.h"
#include "lara.h"
#include "control.h"
#include "effect2.h"
#include "sphere.h"
#include "sound.h"
#include "utils.h"

int weapon_holsters(int weapon_type)
{
    switch (weapon_type)
    {
        default:
            return LARA_HOLSTERS;
        case LG_PISTOLS:
            return LARA_HOLSTERS_PISTOLS;
        case LG_UZIS:
            return LARA_HOLSTERS_UZIS;
        case LG_REVOLVER:
            return LARA_HOLSTERS_REVOLVER;
    }
}

void set_arm_info(LARA_ARM* arm, short frame)
{
    PISTOL_DEF* p;
    short anim_base;

    p = &pistols_table[lara.gun_type];
    anim_base = objects[p->object_number].anim_index;

    if (frame < p->draw1_anim)
        arm->anim_number = anim_base + 0;
    else if (frame < p->draw2_anim)
        arm->anim_number = anim_base + 1;
    else if (frame < p->recoil_anim)
        arm->anim_number = anim_base + 2;
    else
        arm->anim_number = anim_base + 3;

    arm->frame_base = anims[arm->anim_number].frame_ptr;
    arm->frame_number = frame;
}

void draw_pistol_meshes(int weapon_type)
{
    lara.holsters = LARA_HOLSTERS;
    lara.mesh.hand_r = classic_meshes(weapon_meshes(weapon_type), HAND_R);
    if (weapon_type != LG_REVOLVER)
        lara.mesh.hand_l = classic_meshes(weapon_meshes(weapon_type), HAND_L);
}

void undraw_pistols_left(int weapon_type)
{
    if (weapon_type != LG_REVOLVER)
    {
        lara.mesh.hand_l = classic_meshes(LARA, HAND_L);
        lara.holsters = weapon_holsters(weapon_type);
    }
}

void undraw_pistols_right(int weapon_type)
{
    lara.mesh.hand_r = classic_meshes(LARA, HAND_R);
    lara.holsters = weapon_holsters(weapon_type);
}

void draw_pistols(int weapon_type)
{
    PISTOL_DEF* table;
    short ani;

    table = &pistols_table[lara.gun_type];
    ani = lara.l_arm.frame_number + 1;

    if (ani < table->draw1_anim || ani > (table->recoil_anim - 1))
    {
        ani = table->draw1_anim;
    }
    else if (ani == table->draw2_anim)
    {
        draw_pistol_meshes(weapon_type);
        SoundEffect(SFX_LARA_DRAW, &lara_item->pos, 0);
    }
    else if (ani == (table->recoil_anim - 1))
    {
        ready_pistols(weapon_type);
        ani = 0;
    }

    set_arm_info(&lara.r_arm, ani);
    set_arm_info(&lara.l_arm, ani);
}

void undraw_pistols(int weapon_type)
{
    PISTOL_DEF* table;
    short frameR, frameL;
    
    table = &pistols_table[lara.gun_type];

    // frame left
    frameL = lara.l_arm.frame_number;
    if (frameL >= table->recoil_anim)
    {
        frameL = table->draw1_anim2;
    }
    else if ((frameL > 0) && (frameL < table->draw1_anim))
    {
        lara.l_arm.x_rot -= (lara.l_arm.x_rot / lara.l_arm.frame_number);
        lara.l_arm.y_rot -= (lara.l_arm.y_rot / lara.l_arm.frame_number);
        frameL--;
    }
    else if (frameL == 0)
    {
        lara.l_arm.x_rot = 0;
        lara.l_arm.y_rot = 0;
        lara.l_arm.z_rot = 0;
        frameL = table->recoil_anim - 1;
    }
    else if (frameL > table->draw1_anim)
    {
        frameL--;
        if ((frameL - 1) == (table->draw2_anim - 1))
        {
            undraw_pistols_left(weapon_type);
            SoundEffect(SFX_LARA_UNDRAW, &lara_item->pos, 0);
        }
    }
    set_arm_info(&lara.l_arm, frameL);

    // frame right
    frameR = lara.r_arm.frame_number;
    if (frameR >= table->recoil_anim)
    {
        frameR = table->draw1_anim2;
    }
    else if ((frameR > 0) && (frameR < table->draw1_anim))
    {
        lara.r_arm.x_rot -= (lara.r_arm.x_rot / lara.r_arm.frame_number);
        lara.r_arm.y_rot -= (lara.r_arm.y_rot / lara.r_arm.frame_number);
        frameR--;
    }
    else if (frameR == 0)
    {
        lara.r_arm.x_rot = 0;
        lara.r_arm.y_rot = 0;
        lara.r_arm.z_rot = 0;
        frameR = table->recoil_anim - 1;
    }
    else if (frameR > table->draw1_anim)
    {
        frameR--;
        if ((frameR - 1) == (table->draw2_anim - 1))
        {
            undraw_pistols_right(weapon_type);
            SoundEffect(SFX_LARA_UNDRAW, &lara_item->pos, 0);
        }
    }
    set_arm_info(&lara.r_arm, frameR);

    // end:
    if (frameL == table->draw1_anim && frameR == table->draw1_anim)
    {
        lara.gun_status = LHS_ARMLESS;
        lara.l_arm.frame_number = 0;
        lara.r_arm.frame_number = 0;
        lara.l_arm.lock = FALSE;
        lara.r_arm.lock = FALSE;
        lara.target = NULL;
    }

    if (CHK_NOP(TrInput, IN_LOOK))
    {
        lara.torso_x_rot = (lara.l_arm.x_rot + lara.r_arm.x_rot) / 4;
        lara.torso_y_rot = (lara.l_arm.y_rot + lara.r_arm.y_rot) / 4;
        lara.head_x_rot = (lara.l_arm.x_rot + lara.r_arm.x_rot) / 4;
        lara.head_y_rot = (lara.l_arm.y_rot + lara.r_arm.y_rot) / 4;
    }
}

void ready_pistols(int weapon_type)
{
    lara.gun_status = LHS_READY;
    lara.target = NULL;

    lara.l_arm.frame_number = 0;
    lara.l_arm.z_rot = 0;
    lara.l_arm.y_rot = 0;
    lara.l_arm.x_rot = 0;
    lara.l_arm.lock = FALSE;

    lara.r_arm.frame_number = 0;
    lara.r_arm.z_rot = 0;
    lara.r_arm.y_rot = 0;
    lara.r_arm.x_rot = 0;
    lara.r_arm.lock = FALSE;

    lara.l_arm.frame_base = lara.r_arm.frame_base = objects[weapon_object(weapon_type)].frame_base;
}

void pistol_handler(int weapon_type)
{
    WEAPON_INFO* winfo = &weapons[weapon_type];
    PHD_VECTOR pos;

    LaraGetNewTarget(winfo);
    if (CHK_EXI(TrInput, IN_ACTION))
        LaraTargetInfo(winfo);

    AimWeapon(winfo, &lara.l_arm);
    AimWeapon(winfo, &lara.r_arm);

    if (lara.l_arm.lock && !lara.r_arm.lock)
    {
        lara.torso_y_rot = lara.l_arm.y_rot / 2;
        lara.torso_x_rot = lara.l_arm.x_rot / 2;
        if (camera.old_type != LOOK_CAMERA)
        {
            lara.head_y_rot = lara.torso_y_rot;
            lara.head_x_rot = lara.torso_x_rot;
        }
    }
    else if (!lara.l_arm.lock && lara.r_arm.lock)
    {
        lara.torso_y_rot = lara.r_arm.y_rot / 2;
        lara.torso_x_rot = lara.r_arm.x_rot / 2;
        if (camera.old_type != LOOK_CAMERA)
        {
            lara.head_y_rot = lara.torso_y_rot;
            lara.head_x_rot = lara.torso_x_rot;
        }
    }
    else if (lara.l_arm.lock && lara.r_arm.lock)
    {
        lara.torso_y_rot = (lara.l_arm.y_rot + lara.r_arm.y_rot) / 4;
        lara.torso_x_rot = (lara.l_arm.x_rot + lara.r_arm.x_rot) / 4;
        if (camera.old_type != LOOK_CAMERA)
        {
            lara.head_y_rot = lara.torso_y_rot;
            lara.head_x_rot = lara.torso_x_rot;
        }
    }

    animate_pistols(weapon_type);

    if (lara.l_arm.flash_gun)
    {
        pos = GetGunFlashPosition(weapon_type, false);
        TriggerDynamicSwap(pos.x, pos.y, pos.z, 10, (GetRandomControl() & 0x3F) - 64, (GetRandomControl() & 0x1F) + 128, (GetRandomControl() & 0x3F));
    }

    if (lara.r_arm.flash_gun)
    {
        pos = GetGunFlashPosition(weapon_type, true);
        TriggerDynamicSwap(pos.x, pos.y, pos.z, 10, (GetRandomControl() & 0x3F) - 64, (GetRandomControl() & 0x1F) + 128, (GetRandomControl() & 0x3F));
    }
}

void animate_pistols(int weapon_type)
{
    WEAPON_INFO* winfo;
    PISTOL_DEF* p;
    short angles[2];
    short frameL, frameR;
    bool uzi_left = false, uzi_right = false, has_fired = false;

    set_gun_smoke_left(weapon_type);
    set_gun_smoke_right(weapon_type);

    winfo = &weapons[weapon_type];
    p = &pistols_table[lara.gun_type];

    /// process right arm
    frameR = lara.r_arm.frame_number;
    if (lara.r_arm.lock || (CHK_EXI(TrInput, IN_ACTION) && !lara.target))
    {
        if ((frameR >= 0) && (frameR < p->draw1_anim2))
        {
            frameR++;
        }
        else if (frameR == p->draw1_anim2)
        {
            if (CHK_EXI(TrInput, IN_ACTION))
            {
                if (weapon_type != LG_REVOLVER)
                {
                    angles[0] = lara.r_arm.y_rot + lara_item->pos.y_rot;
                    angles[1] = lara.r_arm.x_rot;

                    if (FireWeapon(weapon_type, lara.target, lara_item, angles))
                    {
                        SmokeCountR = 28;
                        SmokeWeapon = weapon_type;
                        TriggerGunShell(TRUE, GUNSHELL, weapon_type);
                        lara.r_arm.flash_gun = winfo->flash_time;
                        SoundEffect(SFX_EXPLOSION1, &lara_item->pos, PITCH_SHIFT | 0x2000000);
                        SoundEffect(winfo->sample_id, &lara_item->pos, 0);
                        has_fired = true;
                        if (weapon_type == LG_UZIS)
                            uzi_right = true;
                        savegame_level.ammo_used++;
                    }
                }

                frameR = p->recoil_anim;
            }
            else if (uzi_right)
            {
                SoundEffect(winfo->sample_id + 1, &lara_item->pos, 0);
                uzi_right = false;
            }
        }
        else if (frameR >= p->recoil_anim)
        {
            if (weapon_type == LG_UZIS)
            {
                SoundEffect(winfo->sample_id, &lara_item->pos, 0);
                uzi_right = true;
            }

            frameR++;
            if (frameR == (p->recoil_anim + winfo->recoil_frame))
                frameR = p->draw1_anim2;
        }
    }
    else
    {
        if (frameR >= p->recoil_anim)
            frameR = p->draw1_anim2;
        else if ((frameR > 0) && (frameR <= p->draw1_anim2))
            frameR--;

        if (uzi_right)
        {
            SoundEffect(winfo->sample_id + 1, &lara_item->pos, 0);
            uzi_right = false;
        }
    }
    set_arm_info(&lara.r_arm, frameR);

    /// process left arm
    frameL = lara.l_arm.frame_number;
    if (lara.l_arm.lock || (CHK_EXI(TrInput, IN_ACTION) && !lara.target))
    {
        if ((frameL >= 0) && (frameL < p->draw1_anim2))
        {
            frameL++;
        }
        else if (frameL == p->draw1_anim2)
        {
            if (CHK_EXI(TrInput, IN_ACTION))
            {
                angles[0] = lara.l_arm.y_rot + lara_item->pos.y_rot;
                angles[1] = lara.l_arm.x_rot;

                if (FireWeapon(weapon_type, lara.target, lara_item, angles))
                {
                    SmokeCountL = 28;
                    SmokeWeapon = weapon_type;
                    TriggerGunShell(FALSE, GUNSHELL, weapon_type);
                    lara.l_arm.flash_gun = winfo->flash_time;

                    if (!has_fired)
                    {
                        SoundEffect(SFX_EXPLOSION1, &lara_item->pos, PITCH_SHIFT | 0x2000000);
                        SoundEffect(winfo->sample_id, &lara_item->pos, 0);
                    }

                    if (weapon_type == LG_UZIS)
                        uzi_left = true;
                    savegame_level.ammo_used++;
                }

                frameL = p->recoil_anim;
            }
            else if (uzi_left)
            {
                SoundEffect(winfo->sample_id + 1, &lara_item->pos, 0);
                uzi_left = false;
            }
        }
        else if (frameL >= p->recoil_anim)
        {
            if (weapon_type == LG_UZIS)
            {
                SoundEffect(winfo->sample_id, &lara_item->pos, 0);
                uzi_left = true;
            }

            frameL++;
            if (frameL == (p->recoil_anim + winfo->recoil_frame))
                frameL = p->draw1_anim2;
        }
    }
    else
    {
        if (frameL >= p->recoil_anim)
            frameL = p->draw1_anim2;
        else if ((frameL > 0) && (frameL <= p->draw1_anim2))
            frameL--;

        if (uzi_left)
        {
            SoundEffect(winfo->sample_id + 1, &lara_item->pos, 0);
            uzi_left = false;
        }
    }
    set_arm_info(&lara.l_arm, frameL);
}

#ifdef DLL_INJECT
void injector::f_game::inject_lara2gun()
{
    inject(0x0042B7C0, set_arm_info);
    inject(0x0042BB20, draw_pistol_meshes);
    inject(0x0042BB70, undraw_pistols_left);
    inject(0x0042BBB0, undraw_pistols_right);
    inject(0x0042B720, draw_pistols);
    inject(0x0042B840, undraw_pistols);
    inject(0x0042BAB0, ready_pistols);
    inject(0x0042BBF0, pistol_handler);
    inject(0x0042BE40, animate_pistols);
}
#endif