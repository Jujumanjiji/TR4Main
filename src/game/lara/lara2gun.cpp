#include "framework.h"
#include "lara.h"
#include "control.h"
#include "effect2.h"
#include "sphere.h"
#include "sound.h"
#include "utils.h"

int WeaponHolsters(int weaponType)
{
    switch (weaponType)
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

void SetArmInfo(LARA_ARM* arm, short frame)
{
    PISTOL_DEF* table;
    short animBase;

    table = &PistolsTable[Lara.gunType];
    animBase = Objects[table->objectNumber].animIndex;

    if (frame < table->draw1Anim)
        arm->animNumber = animBase + 0;
    else if (frame < table->draw2Anim)
        arm->animNumber = animBase + 1;
    else if (frame < table->recoilAnim)
        arm->animNumber = animBase + 2;
    else
        arm->animNumber = animBase + 3;

    arm->frameBase = Anims[arm->animNumber].framePtr;
    arm->frameNumber = frame;
}

void DrawPistolsMeshes(int weaponType)
{
    Lara.holsters = LARA_HOLSTERS;
    Lara.mesh.hand_r = ClassicMeshes(WeaponMeshes(weaponType), HAND_R);
    if (weaponType != LG_REVOLVER)
        Lara.mesh.hand_l = ClassicMeshes(WeaponMeshes(weaponType), HAND_L);
}

void UndrawPistolsLeft(int weaponType)
{
    if (weaponType != LG_REVOLVER)
    {
        Lara.mesh.hand_l = ClassicMeshes(LARA, HAND_L);
        Lara.holsters = WeaponHolsters(weaponType);
    }
}

void UndrawPistolsRight(int weaponType)
{
    Lara.mesh.hand_r = ClassicMeshes(LARA, HAND_R);
    Lara.holsters = WeaponHolsters(weaponType);
}

void DrawPistols(int weaponType)
{
    PISTOL_DEF* table;
    short frame;

    table = &PistolsTable[Lara.gunType];
    frame = Lara.leftArm.frameNumber + 1;

    if (frame < table->draw1Anim || frame > (table->recoilAnim - 1))
    {
        frame = table->draw1Anim;
    }
    else if (frame == table->draw2Anim)
    {
        DrawPistolsMeshes(weaponType);
        SoundEffect(SFX_LARA_DRAW, &LaraItem->pos, 0);
    }
    else if (frame == (table->recoilAnim - 1))
    {
        ReadyPistols(weaponType);
        frame = 0;
    }

    SetArmInfo(&Lara.rightArm, frame);
    SetArmInfo(&Lara.leftArm, frame);
}

void UndrawPistols(int weaponType)
{
    PISTOL_DEF* table;
    short frameR, frameL;
    
    table = &PistolsTable[Lara.gunType];

    // frame left
    frameL = Lara.leftArm.frameNumber;
    if (frameL >= table->recoilAnim)
    {
        frameL = table->draw1Anim2;
    }
    else if ((frameL > 0) && (frameL < table->draw1Anim))
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
        frameL = table->recoilAnim - 1;
    }
    else if (frameL > table->draw1Anim)
    {
        frameL--;
        if ((frameL - 1) == (table->draw2Anim - 1))
        {
            UndrawPistolsLeft(weaponType);
            SoundEffect(SFX_LARA_UNDRAW, &LaraItem->pos, 0);
        }
    }
    SetArmInfo(&Lara.leftArm, frameL);

    // frame right
    frameR = Lara.rightArm.frameNumber;
    if (frameR >= table->recoilAnim)
    {
        frameR = table->draw1Anim2;
    }
    else if ((frameR > 0) && (frameR < table->draw1Anim))
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
        frameR = table->recoilAnim - 1;
    }
    else if (frameR > table->draw1Anim)
    {
        frameR--;
        if ((frameR - 1) == (table->draw2Anim - 1))
        {
            UndrawPistolsRight(weaponType);
            SoundEffect(SFX_LARA_UNDRAW, &LaraItem->pos, 0);
        }
    }
    SetArmInfo(&Lara.rightArm, frameR);

    // end:
    if (frameL == table->draw1Anim && frameR == table->draw1Anim)
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

void ReadyPistols(int weaponType)
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

    Lara.leftArm.frameBase = Lara.rightArm.frameBase = Objects[WeaponObject(weaponType)].frameBase;
}

void PistolHandler(int weaponType)
{
    WEAPON_INFO* winfo = &Weapons[weaponType];
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

    AnimatePistols(weaponType);

    if (Lara.leftArm.flash_gun)
    {
        pos = GetGunFlashPosition(weaponType, false);
        TriggerDynamicSwap(pos.x, pos.y, pos.z, 10, (GetRandomControl() & 0x3F) - 64, (GetRandomControl() & 0x1F) + 128, (GetRandomControl() & 0x3F));
    }

    if (Lara.rightArm.flash_gun)
    {
        pos = GetGunFlashPosition(weaponType, true);
        TriggerDynamicSwap(pos.x, pos.y, pos.z, 10, (GetRandomControl() & 0x3F) - 64, (GetRandomControl() & 0x1F) + 128, (GetRandomControl() & 0x3F));
    }
}

void AnimatePistols(int weaponType)
{
    WEAPON_INFO* winfo;
    PISTOL_DEF* p;
    short angles[2];
    short frameL, frameR;
    bool uzi_left = false, uzi_right = false, has_fired = false;

    SetGunSmokeLeft(weaponType);
    SetGunSmokeRight(weaponType);

    winfo = &Weapons[weaponType];
    p = &PistolsTable[Lara.gunType];

    /// process right arm
    frameR = Lara.rightArm.frameNumber;
    if (Lara.rightArm.lock || (CHK_EXI(TrInput, IN_ACTION) && !Lara.target))
    {
        if ((frameR >= 0) && (frameR < p->draw1Anim2))
        {
            frameR++;
        }
        else if (frameR == p->draw1Anim2)
        {
            if (CHK_EXI(TrInput, IN_ACTION))
            {
                if (weaponType != LG_REVOLVER)
                {
                    angles[0] = Lara.rightArm.yRot + LaraItem->pos.yRot;
                    angles[1] = Lara.rightArm.xRot;

                    if (FireWeapon(weaponType, Lara.target, LaraItem, angles))
                    {
                        SmokeCountR = 28;
                        SmokeWeapon = weaponType;
                        TriggerGunShell(TRUE, GUNSHELL, weaponType);
                        Lara.rightArm.flash_gun = winfo->flashTime;
                        SoundEffect(SFX_EXPLOSION1, &LaraItem->pos, PITCH_SHIFT | 0x2000000);
                        SoundEffect(winfo->sampleNum, &LaraItem->pos, 0);
                        has_fired = true;
                        if (weaponType == LG_UZIS)
                            uzi_right = true;
                        savegame_level.ammo_used++;
                    }
                }

                frameR = p->recoilAnim;
            }
            else if (uzi_right)
            {
                SoundEffect(winfo->sampleNum + 1, &LaraItem->pos, 0);
                uzi_right = false;
            }
        }
        else if (frameR >= p->recoilAnim)
        {
            if (weaponType == LG_UZIS)
            {
                SoundEffect(winfo->sampleNum, &LaraItem->pos, 0);
                uzi_right = true;
            }

            frameR++;
            if (frameR == (p->recoilAnim + winfo->recoilFrame))
                frameR = p->draw1Anim2;
        }
    }
    else
    {
        if (frameR >= p->recoilAnim)
            frameR = p->draw1Anim2;
        else if ((frameR > 0) && (frameR <= p->draw1Anim2))
            frameR--;

        if (uzi_right)
        {
            SoundEffect(winfo->sampleNum + 1, &LaraItem->pos, 0);
            uzi_right = false;
        }
    }
    SetArmInfo(&Lara.rightArm, frameR);

    /// process left arm
    frameL = Lara.leftArm.frameNumber;
    if (Lara.leftArm.lock || (CHK_EXI(TrInput, IN_ACTION) && !Lara.target))
    {
        if ((frameL >= 0) && (frameL < p->draw1Anim2))
        {
            frameL++;
        }
        else if (frameL == p->draw1Anim2)
        {
            if (CHK_EXI(TrInput, IN_ACTION))
            {
                angles[0] = Lara.leftArm.yRot + LaraItem->pos.yRot;
                angles[1] = Lara.leftArm.xRot;

                if (FireWeapon(weaponType, Lara.target, LaraItem, angles))
                {
                    SmokeCountL = 28;
                    SmokeWeapon = weaponType;
                    TriggerGunShell(FALSE, GUNSHELL, weaponType);
                    Lara.leftArm.flash_gun = winfo->flashTime;

                    if (!has_fired)
                    {
                        SoundEffect(SFX_EXPLOSION1, &LaraItem->pos, PITCH_SHIFT | 0x2000000);
                        SoundEffect(winfo->sampleNum, &LaraItem->pos, 0);
                    }

                    if (weaponType == LG_UZIS)
                        uzi_left = true;
                    savegame_level.ammo_used++;
                }

                frameL = p->recoilAnim;
            }
            else if (uzi_left)
            {
                SoundEffect(winfo->sampleNum + 1, &LaraItem->pos, 0);
                uzi_left = false;
            }
        }
        else if (frameL >= p->recoilAnim)
        {
            if (weaponType == LG_UZIS)
            {
                SoundEffect(winfo->sampleNum, &LaraItem->pos, 0);
                uzi_left = true;
            }

            frameL++;
            if (frameL == (p->recoilAnim + winfo->recoilFrame))
                frameL = p->draw1Anim2;
        }
    }
    else
    {
        if (frameL >= p->recoilAnim)
            frameL = p->draw1Anim2;
        else if ((frameL > 0) && (frameL <= p->draw1Anim2))
            frameL--;

        if (uzi_left)
        {
            SoundEffect(winfo->sampleNum + 1, &LaraItem->pos, 0);
            uzi_left = false;
        }
    }
    SetArmInfo(&Lara.leftArm, frameL);
}

#ifdef DLL_INJECT
void injector::f_game::inject_lara2gun()
{
    inject(0x0042B7C0, SetArmInfo);
    inject(0x0042BB20, DrawPistolsMeshes);
    inject(0x0042BB70, UndrawPistolsLeft);
    inject(0x0042BBB0, UndrawPistolsRight);
    inject(0x0042B720, DrawPistols);
    inject(0x0042B840, UndrawPistols);
    inject(0x0042BAB0, ReadyPistols);
    inject(0x0042BBF0, PistolHandler);
    inject(0x0042BE40, AnimatePistols);
}
#endif