#include "framework.h"
#include "lara.h"
#include "control.h"
#include "effect2.h"
#include "items.h"
#include "sphere.h"
#include "sound.h"
#include "utils.h"

void DrawShotgunMeshes(int weaponType)
{
    Lara.backGun = LG_UNARMED;
    Lara.mesh.hand_r = ClassicMeshes(WeaponMeshes(weaponType), HAND_R);
}

void UndrawShotgunMeshes(int weaponType)
{
    Lara.backGun = WeaponObject(weaponType);
    Lara.mesh.hand_r = ClassicMeshes(LARA, HAND_R);
}

void ReadyShotgun(int weaponType)
{
    short *frame = Objects[WeaponObject(weaponType)].frameBase;
    Lara.gunStatus = LHS_READY;
    Lara.target = NULL;

    Lara.leftArm.frameBase = frame;
    Lara.leftArm.xRot = 0;
    Lara.leftArm.yRot = 0;
    Lara.leftArm.zRot = 0;
    Lara.leftArm.lock = FALSE;
    Lara.leftArm.flash_gun = 0;

    Lara.rightArm.frameBase = frame;
    Lara.rightArm.xRot = 0;
    Lara.rightArm.yRot = 0;
    Lara.rightArm.zRot = 0;
    Lara.rightArm.lock = FALSE;
    Lara.rightArm.flash_gun = 0;
}

void DrawShotgun(int weaponType)
{
    ITEM_INFO* item;

    // create the back item if not exist.
    if (Lara.weaponItem == NO_ITEM)
    {
        Lara.weaponItem = CreateItem();
        item = &Items[Lara.weaponItem];
        item->objectNumber = WeaponObject(weaponType);
        item->animNumber = (weaponType == LG_GRENADEGUN) ? (Objects[item->objectNumber].animIndex) : (Objects[item->objectNumber].animIndex + 1);
        item->frameNumber = Anims[item->animNumber].frameBase;
        item->currentAnimState = W_DRAW;
        item->goalAnimState = W_DRAW;
        item->status = FITEM_ACTIVE;
        item->roomNumber = NO_ROOM;
        Lara.leftArm.frameBase = Lara.rightArm.frameBase = Objects[item->objectNumber].frameBase;
    }
    else
    {
        item = &Items[Lara.weaponItem];
    }

    AnimateItem(item);

    if (item->currentAnimState == W_AIM || item->currentAnimState == W_UAIM)
        ReadyShotgun(weaponType);
    else if ((item->frameNumber - Anims[item->animNumber].frameBase) == Weapons[weaponType].drawFrame)
        DrawShotgunMeshes(weaponType);
    else if (Lara.waterStatus == LWS_UNDERWATER)
        item->goalAnimState = W_UAIM;

    Lara.leftArm.frameBase = Lara.rightArm.frameBase = Anims[item->animNumber].framePtr;
    Lara.leftArm.frameNumber = Lara.rightArm.frameNumber = item->frameNumber - Anims[item->animNumber].frameBase;
    Lara.leftArm.animNumber  = Lara.rightArm.animNumber  = item->animNumber;
}

void UndrawShotgun(int weapon_type)
{
    ITEM_INFO* item = &Items[Lara.weaponItem];
    if (Lara.waterStatus == LWS_SURFACE)
        item->goalAnimState = W_SURF_UNDRAW;
    else
        item->goalAnimState = W_UNDRAW;

    AnimateItem(item);

    if (item->status == FITEM_DEACTIVATED)
    {
        Lara.gunStatus = LG_UNARMED;
        Lara.target = NULL;
        Lara.rightArm.yRot = 0;
        Lara.leftArm.yRot = 0;
        KillItem(Lara.weaponItem);
        Lara.weaponItem = NO_ITEM;
        Lara.rightArm.frameNumber = 0;
        Lara.leftArm.frameNumber = 0;
    }
    else if (item->currentAnimState == W_UNDRAW && GetCurrentFrame(item) == 21)
    {
        UndrawShotgunMeshes(weapon_type);
    }

    Lara.leftArm.frameBase   = Lara.rightArm.frameBase   = Anims[item->animNumber].framePtr;
    Lara.leftArm.frameNumber = Lara.rightArm.frameNumber = GetCurrentFrame(item);
    Lara.leftArm.animNumber  = Lara.rightArm.animNumber  = item->animNumber;
}

void ShotgunHandler(int weaponType)
{
    WEAPON_INFO* winfo = &Weapons[weaponType];
    PHD_VECTOR pos;

    LaraGetNewTarget(winfo);
    if (TrInput & IN_ACTION)
        LaraTargetInfo(winfo);
    AimWeapon(winfo, &Lara.leftArm);

    if (Lara.leftArm.lock)
    {
        Lara.torsoXrot = Lara.leftArm.xRot;
        Lara.torsoYrot = Lara.leftArm.yRot;
        if (camera.old_type != LOOK_CAMERA && !BinocularRange)
        {
            Lara.headXrot = 0;
            Lara.headYrot = 0;
        }
    }

#ifdef DEBUG_CHEAT
    if (CHK_EXI(TrInput, IN_SPRINT))
    {
        Lara.revolver_ammo_count = INFINITE_AMMO;
        Lara.shotgun_ammo1_count = INFINITE_AMMO;
        Lara.shotgun_ammo2_count = INFINITE_AMMO;
        Lara.grenade_ammo1_count = INFINITE_AMMO;
        Lara.grenade_ammo2_count = INFINITE_AMMO;
        Lara.grenade_ammo3_count = INFINITE_AMMO;
        Lara.crossbow_ammo1_count = INFINITE_AMMO;
        Lara.crossbow_ammo2_count = INFINITE_AMMO;
        Lara.crossbow_ammo3_count = INFINITE_AMMO;
    }
#endif

    if (weaponType == LG_REVOLVER)
        AnimatePistols(LG_REVOLVER);
    else
        AnimateShotgun(weaponType);

    if (Lara.rightArm.flash_gun)
    {
        switch (weaponType)
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

void AnimateShotgun(int weaponType)
{
    ITEM_INFO* item = &Items[Lara.weaponItem];
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
            else if (Lara.waterStatus == LWS_UNDERWATER)
                item->goalAnimState = W_UAIM;
            else if ((CHK_EXI(TrInput, IN_ACTION) && !Lara.target) || Lara.leftArm.lock)
                item->goalAnimState = W_RECOIL;
            else
                item->goalAnimState = W_UNAIM;
            break;
        case W_RECOIL:
            if (frame == 0)
            {
                item->goalAnimState = W_UNAIM;

                if (Lara.waterStatus != LWS_UNDERWATER && !harpoon_reload)
                {
                    if (CHK_EXI(TrInput, IN_ACTION) && (!Lara.target || Lara.leftArm.lock))
                    {
                        switch (weaponType)
                        {
                            case LG_SHOTGUN:
                                FireShotgun();
                                break;
                            case LG_GRENADEGUN:
                                FireGrenade();
                                break;
                            case LG_CROSSBOW:
                                if (!LaserSight) // dont delete more ammo that necessary !
                                    FireCrossbow(nullptr);
                                break;
                        }

                        item->goalAnimState = W_RECOIL;
                    }
                    else if (Lara.leftArm.lock)
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
            else if (weaponType == LG_SHOTGUN && CHK_NOP(TrInput, IN_ACTION) && !Lara.leftArm.lock)
            {
                item->goalAnimState = W_UNAIM;
            }

            if (frame == 12 && weaponType == LG_SHOTGUN)
                TriggerGunShell(TRUE, SHOTGUNSHELL, LG_SHOTGUN);
            break;

        // TODO: add WATER states !
        case W_UAIM:
            break;
        case W_URECOIL:
            break;
    }

    AnimateItem(item);
    Lara.leftArm.frameBase   = Lara.rightArm.frameBase   = Anims[item->animNumber].framePtr;
    Lara.leftArm.frameNumber = Lara.rightArm.frameNumber = GetCurrentFrame(item);
    Lara.leftArm.animNumber  = Lara.rightArm.animNumber  = item->animNumber;
}

#ifdef DLL_INJECT
void injector::f_game::inject_lara1gun()
{
    inject(0x00428E40, DrawShotgunMeshes);
    inject(0x00428E70, UndrawShotgunMeshes);
    inject(0x00428EA0, ReadyShotgun);
    inject(0x0042AE50, DrawShotgun);
    inject(0x0042AFE0, UndrawShotgun);
    inject(0x00428F10, ShotgunHandler);
    inject(0x0042B100, AnimateShotgun);
}
#endif