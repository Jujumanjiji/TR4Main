#include "framework.h"
#include "lara2gun.h"
#include "control.h"
#include "effect2.h"
#include "larafire.h"
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
            return LARA_HOLSTERS_SIXSHOOTER;
    }
}

void set_arm_info(LARA_ARM* arm, short frame)
{
    PISTOL_DEF* table;
    short animIndex;

    table = &pistols_table[lara.gun_type];
    animIndex = objects[table->object_number].anim_index;

    if (frame < table->draw1_anim)
        arm->anim_curr = animIndex + 0;
    if (frame < table->draw2_anim)
        arm->anim_curr = animIndex + 1;
    else if (frame < table->recoil_anim)
        arm->anim_curr = animIndex + 2;
    else
        arm->anim_curr = animIndex + 3;

    arm->frame_base = anims[arm->anim_curr].frame_ptr;
    arm->frame_curr = frame;
}

void draw_pistol_meshes(int weapon_type)
{
    lara.holsters = LARA_HOLSTERS;
    lara.mesh.hand_r = assign_meshes(weapon_meshes(weapon_type), HAND_R);
    if (weapon_type != LG_REVOLVER)
        lara.mesh.hand_l = assign_meshes(weapon_meshes(weapon_type), HAND_L);
}

void undraw_pistols_left(int weapon_type)
{
    if (weapon_type != LG_REVOLVER)
    {
        lara.mesh.hand_l = assign_meshes(LARA, HAND_L);
        lara.holsters = weapon_holsters(weapon_type);
    }
}

void undraw_pistols_right(int weapon_type)
{
    lara.mesh.hand_r = assign_meshes(LARA, HAND_R);
    lara.holsters = weapon_holsters(weapon_type);
}

void draw_pistols(int weapon_type)
{
    PISTOL_DEF* table;
    short frame;

    table = &pistols_table[lara.gun_type];
    frame = lara.l_arm.frame_curr + 1;

    if ((frame < table->draw1_anim) || (frame > table->recoil_anim - 1))
    {
        frame = table->draw1_anim;
    }
    else if (frame == table->draw2_anim)
    {
        draw_pistol_meshes(weapon_type);
        SoundEffect(SFX_LARA_DRAW, &lara_item->pos, 0);
    }
    else if (frame == (table->recoil_anim - 1))
    {
        ready_pistols(weapon_type);
        frame = 0;
    }

    set_arm_info(&lara.r_arm, frame);
    set_arm_info(&lara.l_arm, frame);
}

void undraw_pistols(int weapon_type)
{
    PISTOL_DEF* table;
    short frameR, frameL;
    
    table = &pistols_table[lara.gun_type];

    // frame left
    frameL = lara.l_arm.frame_curr;
    if (frameL >= table->recoil_anim)
    {
        frameL = table->draw1_anim2;
    }
    else if ((frameL > 0) && (frameL < table->draw1_anim))
    {
        lara.l_arm.x_rot -= (lara.l_arm.x_rot / lara.l_arm.frame_curr);
        lara.l_arm.y_rot -= (lara.l_arm.y_rot / lara.l_arm.frame_curr);
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
    frameR = lara.r_arm.frame_curr;
    if (frameR >= table->recoil_anim)
    {
        frameR = table->draw1_anim2;
    }
    else if ((frameR > 0) && (frameR < table->draw1_anim))
    {
        lara.r_arm.x_rot -= (lara.r_arm.x_rot / lara.r_arm.frame_curr);
        lara.r_arm.y_rot -= (lara.r_arm.y_rot / lara.r_arm.frame_curr);
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
        lara.l_arm.frame_curr = 0;
        lara.r_arm.frame_curr = 0;
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
    lara.l_arm.z_rot = 0;
    lara.l_arm.y_rot = 0;
    lara.l_arm.x_rot = 0;
    lara.r_arm.z_rot = 0;
    lara.r_arm.y_rot = 0;
    lara.r_arm.x_rot = 0;
    lara.r_arm.frame_curr = 0;
    lara.l_arm.frame_curr = 0;
    lara.r_arm.lock = FALSE;
    lara.l_arm.lock = FALSE;
    lara.r_arm.frame_base = objects[weapon_object(weapon_type)].frame_base;
    lara.l_arm.frame_base = lara.r_arm.frame_base;
}

void pistol_handler(int weapon_type)
{
    WEAPON_INFO* winfo = &weapons[weapon_type];
    PHD_VECTOR pos;

    LaraGetNewTarget(winfo);
    if (CHK_ANY(TrInput, IN_ACTION))
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
    PISTOL_DEF* table;
    short angles[2];
    short frameL, frameR;
    bool uzi_left = false, uzi_right = false, has_fired;

    has_fired = false;
    winfo = &weapons[weapon_type];
    table = &pistols_table[lara.gun_type];
    frameL = lara.l_arm.frame_curr;
    frameR = lara.r_arm.frame_curr;

    set_gun_smoke_left(weapon_type);
    set_gun_smoke_right(weapon_type);

    /// process right arm
    if (lara.r_arm.lock || (CHK_ANY(TrInput, IN_ACTION) && !lara.target))
    {
        if ((frameR >= 0) && (frameR < table->draw1_anim2))
        {
            frameR++;
        }
        else if (frameR == table->draw1_anim2)
        {
            if (CHK_ANY(TrInput, IN_ACTION))
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
                        ++savegame_ammoused;
                    }
                }

                frameR = table->recoil_anim;
            }
            else if (uzi_right)
            {
                SoundEffect(winfo->sample_id + 1, &lara_item->pos, 0);
            }
        }
        else if (frameR >= table->recoil_anim)
        {
            if (weapon_type == LG_UZIS)
            {
                SoundEffect(winfo->sample_id, &lara_item->pos, 0);
                uzi_right = true;
            }

            frameR++;
            if (frameR == (table->recoil_anim + winfo->recoil_frame))
                frameR = table->draw1_anim2;
        }
    }
    else
    {
        if (frameR >= table->recoil_anim)
            frameR = table->draw1_anim2;
        else if ((frameR > 0) && (frameR <= table->draw1_anim2))
            frameR--;

        if (uzi_right)
        {
            SoundEffect(winfo->sample_id + 1, &lara_item->pos, 0);
            uzi_right = false;
        }
    }
    set_arm_info(&lara.r_arm, frameR);

    /// process left arm
    if (lara.l_arm.lock || (CHK_ANY(TrInput, IN_ACTION) && !lara.target))
    {
        if ((frameL >= 0) && (frameL < table->draw1_anim2))
        {
            frameL++;
        }
        else if (frameL == table->draw1_anim2)
        {
            if (CHK_ANY(TrInput, IN_ACTION))
            {
                if (weapon_type != LG_REVOLVER)
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
                        ++savegame_ammoused;
                    }
                }

                frameL = table->recoil_anim;
            }
            else if (uzi_left)
            {
                SoundEffect(winfo->sample_id + 1, &lara_item->pos, 0);
            }
        }
        else if (frameL >= table->recoil_anim)
        {
            if (weapon_type == LG_UZIS)
            {
                SoundEffect(winfo->sample_id, &lara_item->pos, 0);
                uzi_left = true;
            }

            frameL++;
            if (frameL == (table->recoil_anim + winfo->recoil_frame))
                frameL = table->draw1_anim2;
        }
    }
    else
    {
        if (frameL >= table->recoil_anim)
            frameL = table->draw1_anim2;
        else if ((frameL > 0) && (frameL <= table->draw1_anim2))
            frameL--;

        if (uzi_left)
        {
            SoundEffect(winfo->sample_id + 1, &lara_item->pos, 0);
            uzi_left = false;
        }
    }
    set_arm_info(&lara.l_arm, frameL);
}

void injector::inject_lara2gun()
{
    this->inject(legacy_set_arm_info);
    this->inject(legacy_draw_pistol_meshes);
    this->inject(legacy_undraw_pistols_left);
    this->inject(legacy_undraw_pistols_right);
    this->inject(legacy_draw_pistols);
    this->inject(legacy_undraw_pistols);
    this->inject(legacy_ready_pistols);
    this->inject(legacy_pistol_handler);
    this->inject(legacy_animate_pistols);
}