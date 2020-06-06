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

    p = &pistols_table[Lara.gunType];
    anim_base = Objects[p->object_number].animIndex;

    if (frame < p->draw1_anim)
        arm->animNumber = anim_base + 0;
    else if (frame < p->draw2_anim)
        arm->animNumber = anim_base + 1;
    else if (frame < p->recoil_anim)
        arm->animNumber = anim_base + 2;
    else
        arm->animNumber = anim_base + 3;

    arm->frameBase = Anims[arm->animNumber].framePtr;
    arm->frameNumber = frame;
}

void draw_pistol_meshes(int weapon_type)
{
    Lara.holsters = LARA_HOLSTERS;
    Lara.mesh.hand_r = classic_meshes(weapon_meshes(weapon_type), HAND_R);
    if (weapon_type != LG_REVOLVER)
        Lara.mesh.hand_l = classic_meshes(weapon_meshes(weapon_type), HAND_L);
}

void undraw_pistols_left(int weapon_type)
{
    if (weapon_type != LG_REVOLVER)
    {
        Lara.mesh.hand_l = classic_meshes(LARA, HAND_L);
        Lara.holsters = weapon_holsters(weapon_type);
    }
}

void undraw_pistols_right(int weapon_type)
{
    Lara.mesh.hand_r = classic_meshes(LARA, HAND_R);
    Lara.holsters = weapon_holsters(weapon_type);
}

void draw_pistols(int weapon_type)
{
    PISTOL_DEF* table;
    short ani;

    table = &pistols_table[Lara.gunType];
    ani = Lara.leftArm.frameNumber + 1;

    if (ani < table->draw1_anim || ani > (table->recoil_anim - 1))
    {
        ani = table->draw1_anim;
    }
    else if (ani == table->draw2_anim)
    {
        draw_pistol_meshes(weapon_type);
        SoundEffect(SFX_LARA_DRAW, &LaraItem->pos, 0);
    }
    else if (ani == (table->recoil_anim - 1))
    {
        ready_pistols(weapon_type);
        ani = 0;
    }

    set_arm_info(&Lara.rightArm, ani);
    set_arm_info(&Lara.leftArm, ani);
}

void undraw_pistols(int weapon_type)
{
    PISTOL_DEF* table;
    short frameR, frameL;
    
    table = &pistols_table[Lara.gunType];

    // frame left
    frameL = Lara.leftArm.frameNumber;
    if (frameL >= table->recoil_anim)
    {
        frameL = table->draw1_anim2;
    }
    else if ((frameL > 0) && (frameL < table->draw1_anim))
    {
        Lara.leftArm.xRot -= (Lara.leftArm.xRot / Lara.leftArm.frameNumber);
        Lara.leftArm.yRot -= (Lara.leftArm.yRot / Lara.leftArm.frameNumber);
        frameL--;
    }
    else if (frameL == 0)
    {
        Lara.leftArm.xRot = 0;
        Lara.leftArm.yRot = 0;
        Lara.leftArm.zRot = 0;
        frameL = table->recoil_anim - 1;
    }
    else if (frameL > table->draw1_anim)
    {
        frameL--;
        if ((frameL - 1) == (table->draw2_anim - 1))
        {
            undraw_pistols_left(weapon_type);
            SoundEffect(SFX_LARA_UNDRAW, &LaraItem->pos, 0);
        }
    }
    set_arm_info(&Lara.leftArm, frameL);

    // frame right
    frameR = Lara.rightArm.frameNumber;
    if (frameR >= table->recoil_anim)
    {
        frameR = table->draw1_anim2;
    }
    else if ((frameR > 0) && (frameR < table->draw1_anim))
    {
        Lara.rightArm.xRot -= (Lara.rightArm.xRot / Lara.rightArm.frameNumber);
        Lara.rightArm.yRot -= (Lara.rightArm.yRot / Lara.rightArm.frameNumber);
        frameR--;
    }
    else if (frameR == 0)
    {
        Lara.rightArm.xRot = 0;
        Lara.rightArm.yRot = 0;
        Lara.rightArm.zRot = 0;
        frameR = table->recoil_anim - 1;
    }
    else if (frameR > table->draw1_anim)
    {
        frameR--;
        if ((frameR - 1) == (table->draw2_anim - 1))
        {
            undraw_pistols_right(weapon_type);
            SoundEffect(SFX_LARA_UNDRAW, &LaraItem->pos, 0);
        }
    }
    set_arm_info(&Lara.rightArm, frameR);

    // end:
    if (frameL == table->draw1_anim && frameR == table->draw1_anim)
    {
        Lara.gunStatus = LHS_ARMLESS;
        Lara.leftArm.frameNumber = 0;
        Lara.rightArm.frameNumber = 0;
        Lara.leftArm.lock = FALSE;
        Lara.rightArm.lock = FALSE;
        Lara.target = NULL;
    }

    if (CHK_NOP(TrInput, IN_LOOK))
    {
        Lara.torsoXrot = (Lara.leftArm.xRot + Lara.rightArm.xRot) / 4;
        Lara.torsoYrot = (Lara.leftArm.yRot + Lara.rightArm.yRot) / 4;
        Lara.headXrot = (Lara.leftArm.xRot + Lara.rightArm.xRot) / 4;
        Lara.headYrot = (Lara.leftArm.yRot + Lara.rightArm.yRot) / 4;
    }
}

void ready_pistols(int weapon_type)
{
    Lara.gunStatus = LHS_READY;
    Lara.target = NULL;

    Lara.leftArm.frameNumber = 0;
    Lara.leftArm.zRot = 0;
    Lara.leftArm.yRot = 0;
    Lara.leftArm.xRot = 0;
    Lara.leftArm.lock = FALSE;

    Lara.rightArm.frameNumber = 0;
    Lara.rightArm.zRot = 0;
    Lara.rightArm.yRot = 0;
    Lara.rightArm.xRot = 0;
    Lara.rightArm.lock = FALSE;

    Lara.leftArm.frameBase = Lara.rightArm.frameBase = Objects[WeaponObject(weapon_type)].frameBase;
}

void pistol_handler(int weapon_type)
{
    WEAPON_INFO* winfo = &weapons[weapon_type];
    PHD_VECTOR pos;

    LaraGetNewTarget(winfo);
    if (CHK_EXI(TrInput, IN_ACTION))
        LaraTargetInfo(winfo);

    AimWeapon(winfo, &Lara.leftArm);
    AimWeapon(winfo, &Lara.rightArm);

    if (Lara.leftArm.lock && !Lara.rightArm.lock)
    {
        Lara.torsoYrot = Lara.leftArm.yRot / 2;
        Lara.torsoXrot = Lara.leftArm.xRot / 2;
        if (camera.old_type != LOOK_CAMERA)
        {
            Lara.headYrot = Lara.torsoYrot;
            Lara.headXrot = Lara.torsoXrot;
        }
    }
    else if (!Lara.leftArm.lock && Lara.rightArm.lock)
    {
        Lara.torsoYrot = Lara.rightArm.yRot / 2;
        Lara.torsoXrot = Lara.rightArm.xRot / 2;
        if (camera.old_type != LOOK_CAMERA)
        {
            Lara.headYrot = Lara.torsoYrot;
            Lara.headXrot = Lara.torsoXrot;
        }
    }
    else if (Lara.leftArm.lock && Lara.rightArm.lock)
    {
        Lara.torsoYrot = (Lara.leftArm.yRot + Lara.rightArm.yRot) / 4;
        Lara.torsoXrot = (Lara.leftArm.xRot + Lara.rightArm.xRot) / 4;
        if (camera.old_type != LOOK_CAMERA)
        {
            Lara.headYrot = Lara.torsoYrot;
            Lara.headXrot = Lara.torsoXrot;
        }
    }

    animate_pistols(weapon_type);

    if (Lara.leftArm.flash_gun)
    {
        pos = GetGunFlashPosition(weapon_type, false);
        TriggerDynamicSwap(pos.x, pos.y, pos.z, 10, (GetRandomControl() & 0x3F) - 64, (GetRandomControl() & 0x1F) + 128, (GetRandomControl() & 0x3F));
    }

    if (Lara.rightArm.flash_gun)
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
    p = &pistols_table[Lara.gunType];

    /// process right arm
    frameR = Lara.rightArm.frameNumber;
    if (Lara.rightArm.lock || (CHK_EXI(TrInput, IN_ACTION) && !Lara.target))
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
                    angles[0] = Lara.rightArm.yRot + LaraItem->pos.yRot;
                    angles[1] = Lara.rightArm.xRot;

                    if (FireWeapon(weapon_type, Lara.target, LaraItem, angles))
                    {
                        SmokeCountR = 28;
                        SmokeWeapon = weapon_type;
                        TriggerGunShell(TRUE, GUNSHELL, weapon_type);
                        Lara.rightArm.flash_gun = winfo->flash_time;
                        SoundEffect(SFX_EXPLOSION1, &LaraItem->pos, PITCH_SHIFT | 0x2000000);
                        SoundEffect(winfo->sample_id, &LaraItem->pos, 0);
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
                SoundEffect(winfo->sample_id + 1, &LaraItem->pos, 0);
                uzi_right = false;
            }
        }
        else if (frameR >= p->recoil_anim)
        {
            if (weapon_type == LG_UZIS)
            {
                SoundEffect(winfo->sample_id, &LaraItem->pos, 0);
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
            SoundEffect(winfo->sample_id + 1, &LaraItem->pos, 0);
            uzi_right = false;
        }
    }
    set_arm_info(&Lara.rightArm, frameR);

    /// process left arm
    frameL = Lara.leftArm.frameNumber;
    if (Lara.leftArm.lock || (CHK_EXI(TrInput, IN_ACTION) && !Lara.target))
    {
        if ((frameL >= 0) && (frameL < p->draw1_anim2))
        {
            frameL++;
        }
        else if (frameL == p->draw1_anim2)
        {
            if (CHK_EXI(TrInput, IN_ACTION))
            {
                angles[0] = Lara.leftArm.yRot + LaraItem->pos.yRot;
                angles[1] = Lara.leftArm.xRot;

                if (FireWeapon(weapon_type, Lara.target, LaraItem, angles))
                {
                    SmokeCountL = 28;
                    SmokeWeapon = weapon_type;
                    TriggerGunShell(FALSE, GUNSHELL, weapon_type);
                    Lara.leftArm.flash_gun = winfo->flash_time;

                    if (!has_fired)
                    {
                        SoundEffect(SFX_EXPLOSION1, &LaraItem->pos, PITCH_SHIFT | 0x2000000);
                        SoundEffect(winfo->sample_id, &LaraItem->pos, 0);
                    }

                    if (weapon_type == LG_UZIS)
                        uzi_left = true;
                    savegame_level.ammo_used++;
                }

                frameL = p->recoil_anim;
            }
            else if (uzi_left)
            {
                SoundEffect(winfo->sample_id + 1, &LaraItem->pos, 0);
                uzi_left = false;
            }
        }
        else if (frameL >= p->recoil_anim)
        {
            if (weapon_type == LG_UZIS)
            {
                SoundEffect(winfo->sample_id, &LaraItem->pos, 0);
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
            SoundEffect(winfo->sample_id + 1, &LaraItem->pos, 0);
            uzi_left = false;
        }
    }
    set_arm_info(&Lara.leftArm, frameL);
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