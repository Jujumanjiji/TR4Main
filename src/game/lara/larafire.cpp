#include "framework.h"
#include "lara.h"
#include "3d_gen.h"
#include "debris.h"
#include "draw.h"
#include "control.h"
#include "effect2.h"
#include "gameflow.h"
#include "items.h"
#include "sphere.h"
#include "sound.h"
#include "switchs.h"
#include "utils.h"

#define NEAR_ANGLE ANGLE(15)

PISTOL_DEF PistolsTable[4] =
{
    { LARA, 0, 0, 0, 0 },
    { PISTOLS_ANIM, 4, 5, 13, 24 },
    { REVOLVER_ANIM, 7, 8, 15, 29 },
    { UZI_ANIM, 4, 5, 13, 24 }
};

WEAPON_INFO Weapons[LG_MAX_HOLD] =
{
    {   // UNARMED
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0
    },
    {   // PISTOLS
        { PISTOL_LOCK_YMIN, PISTOL_LOCK_YMAX, PISTOL_LOCK_XMIN, PISTOL_LOCK_XMAX },
        { PISTOL_LARM_YMIN, PISTOL_LARM_YMAX, PISTOL_LARM_XMIN, PISTOL_LARM_XMAX },
        { PISTOL_RARM_YMIN, PISTOL_RARM_YMAX, PISTOL_RARM_XMIN, PISTOL_RARM_XMAX },
        ANGLE(10),
        ANGLE(8),
        650,
        SECTOR(8),
        1,
        9,
        3,
        0,
        SFX_LARA_FIRE
    },
    {   // REVOLVER
        { REVOLVER_LOCK_YMIN, REVOLVER_LOCK_YMAX, REVOLVER_LOCK_XMIN, REVOLVER_LOCK_XMAX },
        { REVOLVER_LARM_YMIN, REVOLVER_LARM_YMAX, REVOLVER_LARM_XMIN, REVOLVER_LARM_XMAX },
        { REVOLVER_RARM_YMIN, REVOLVER_RARM_YMAX, REVOLVER_RARM_XMIN, REVOLVER_RARM_XMAX },
        ANGLE(10),
        ANGLE(4),
        650,
        SECTOR(8),
        21,
        16,
        3,
        0,
        SFX_REVOLVER_FIRE
    },
    {   // UZIS
        { PISTOL_LOCK_YMIN, PISTOL_LOCK_YMAX, PISTOL_LOCK_XMIN, PISTOL_LOCK_XMAX },
        { PISTOL_LARM_YMIN, PISTOL_LARM_YMAX, PISTOL_LARM_XMIN, PISTOL_LARM_XMAX },
        { PISTOL_RARM_YMIN, PISTOL_RARM_YMAX, PISTOL_RARM_XMIN, PISTOL_RARM_XMAX },
        ANGLE(10),
        ANGLE(8),
        650,
        SECTOR(8),
        1,
        3,
        3,
        0,
        SFX_LARA_UZI_FIRE
    },
    {   // SHOTGUN
        { SHOTGUN_LOCK_YMIN, SHOTGUN_LOCK_YMAX, SHOTGUN_LOCK_XMIN, SHOTGUN_LOCK_XMAX },
        { SHOTGUN_LARM_YMIN, SHOTGUN_LARM_YMAX, SHOTGUN_LARM_XMIN, SHOTGUN_LARM_XMAX },
        { SHOTGUN_RARM_YMIN, SHOTGUN_RARM_YMAX, SHOTGUN_RARM_XMIN, SHOTGUN_RARM_XMAX },
        ANGLE(10),
        0,              // pullet randomized !
        500,
        SECTOR(8),
        3,
        9,
        3,
        10,
        SFX_LARA_SHOTGUN
    },
    {   // GRENADEGUN
        { GRENADE_LOCK_YMIN, GRENADE_LOCK_YMAX, GRENADE_LOCK_XMIN, GRENADE_LOCK_XMAX },
        { GRENADE_LARM_YMIN, GRENADE_LARM_YMAX, GRENADE_LARM_XMIN, GRENADE_LARM_XMAX },
        { GRENADE_RARM_YMIN, GRENADE_RARM_YMAX, GRENADE_RARM_XMIN, GRENADE_RARM_XMAX },
        ANGLE(10),
        ANGLE(8),
        500,
        SECTOR(8),
        20,
        0,
        2,
        10,
        NULL // animation sound !
    },
    {   // CROSSBOW
        { CROSSBOW_LOCK_YMIN, CROSSBOW_LOCK_YMAX, CROSSBOW_LOCK_XMIN, CROSSBOW_LOCK_XMAX },
        { CROSSBOW_LARM_YMIN, CROSSBOW_LARM_YMAX, CROSSBOW_LARM_XMIN, CROSSBOW_LARM_XMAX },
        { CROSSBOW_RARM_YMIN, CROSSBOW_RARM_YMAX, CROSSBOW_RARM_XMIN, CROSSBOW_RARM_XMAX },
        ANGLE(10),
        ANGLE(8),
        500,
        SECTOR(8),
        5,
        0,
        2,
        10,
        NULL // animation sound !
    },
    {   // FLARE
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0
    },
    {   // TORCH
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0
    },
    {   // SKIDOO GUNS
        { SKID_LOCK_YMIN, SKID_LOCK_YMAX, SKID_LOCK_XMIN, SKID_LOCK_XMAX },
        { SKID_LARM_YMIN, SKID_LARM_YMAX, SKID_LARM_XMIN, SKID_LARM_XMAX },
        { SKID_RARM_YMIN, SKID_RARM_YMAX, SKID_RARM_XMIN, SKID_RARM_XMAX },
        ANGLE(10), // aiming speed
        ANGLE(8),  // randomness
        400,
        SECTOR(8),
        3, // like uzis
        0,
        2,
        0,
        SFX_LARA_UZI_FIRE
    },
};

// work nice :D
void TriggerShotgunSparks(int x, int y, int z, int xv, int yv, int zv)
{
    SPARKS *sptr = &Sparks[GetFreeSparks()];
    sptr->on = TRUE;
    sptr->sR = 255;
    sptr->sG = 255;
    sptr->sB = 0;
    sptr->dR = 255;
    sptr->dG = (GetRandomControl() & 127) + 64;
    sptr->dB = 0;
    sptr->colFadeSpeed = 3;
    sptr->fadeToBlack = 5;
    sptr->sLife = sptr->life = 10;
    sptr->transtype = TS_COLADD;
    sptr->dynamic = -1;
    sptr->x = x + (GetRandomControl() & 7) - 3;
    sptr->y = y + (GetRandomControl() & 7) - 3;
    sptr->z = z + (GetRandomControl() & 7) - 3;
    sptr->xVel = xv + ((GetRandomControl() & 511) - 256);
    sptr->yVel = yv + ((GetRandomControl() & 511) - 256);
    sptr->zVel = zv + ((GetRandomControl() & 511) - 256);
    sptr->friction = 0;
    sptr->flags = SPF_SCALE;
    sptr->scalar = 2;
    sptr->size = sptr->sSize = (GetRandomControl() & 3) + 4;
    sptr->dSize = 1;// 20 is more good !
    sptr->gravity = sptr->maxyvel = 0;
}

int WeaponObject(int weapon_type)
{
    switch (weapon_type)
    {
        case LG_REVOLVER:
            return REVOLVER_ANIM;
        case LG_UZIS:
            return UZI_ANIM;
        case LG_SHOTGUN:
            return SHOTGUN_ANIM;
        case LG_GRENADEGUN:
            return GRENADE_GUN_ANIM;
        case LG_CROSSBOW:
            return CROSSBOW_ANIM;
        case LG_PISTOLS:
        default:
            return PISTOLS_ANIM;
    }
}

int WeaponMeshes(int weapon_type)
{
    switch (weapon_type)
    {
        case LG_REVOLVER:
            return CHK_EXI(Lara.revolver_type_carried, CR_LASERSIGHT) ? LARA_REVOLVER_LASER : REVOLVER_ANIM;
        case LG_UZIS:
            return UZI_ANIM;
        case LG_SHOTGUN:
            return SHOTGUN_ANIM;
        case LG_GRENADEGUN:
            return GRENADE_GUN_ANIM;
        case LG_CROSSBOW:
            return CHK_EXI(Lara.crossbow_type_carried, CR_LASERSIGHT) ? LARA_CROSSBOW_LASER : CROSSBOW_ANIM;
        case LG_PISTOLS:
        default:
            return PISTOLS_ANIM;
    }
}

void FireShotgun(void)
{
    PHD_VECTOR pos;
    int pulletAccuraty;
    int x, y, z, r;
    short dangles[2];
    short angles[2];
    bool fired = false;

    dangles[0] = Lara.leftArm.yRot + LaraItem->pos.yRot;
    dangles[1] = Lara.leftArm.xRot;
    if (!Lara.leftArm.lock)
    {
        dangles[0] += Lara.torsoYrot;
        dangles[1] += Lara.torsoXrot;
    }

    pulletAccuraty = CHK_EXI(Lara.shotgun_type_carried, CR_AMMO1) ? SHOTGUN_RANDOM_AMMO1 : SHOTGUN_RANDOM_AMMO2;
    for (int i = 0; i < SHOTGUN_BULLET_MAX; i++)
    {
        r = (int)((GetRandomControl() - 0x4000) * pulletAccuraty / 0x10000);
        angles[0] = dangles[0] + short(r);
        r = (int)((GetRandomControl() - 0x4000) * pulletAccuraty / 0x10000);
        angles[1] = dangles[1] + short(r);
        if (FireWeapon(LG_SHOTGUN, Lara.target, LaraItem, angles))
            fired = true;
    }

    if (fired)
    {
        pos.x = SHOTGUN_GUNPOS_X;
        pos.y = SHOTGUN_GUNPOS_Y; // min
        pos.z = SHOTGUN_GUNPOS_Z;
        GetLaraJointAbsPosition(&pos, JHAND_R);
        x = pos.x;
        y = pos.y;
        z = pos.z;
        pos.x = SHOTGUN_GUNPOS_X;
        pos.y = SHOTGUN_GUNPOS_Y + SHOTGUN_GUNPOS_SMOKEDIST_MAX; // max
        pos.z = SHOTGUN_GUNPOS_Z;
        GetLaraJointAbsPosition(&pos, JHAND_R);

        SmokeCountL = 32;
        SmokeWeapon = LG_SHOTGUN;

        if (LaraItem->meshBits)
        {
            for (int i = 0; i < SHOTGUN_SMOKE_MAX; i++)
                TriggerGunSmoke(x, y, z, pos.x - x, pos.y - y, pos.z - z, 1, SmokeWeapon, SmokeCountL);
            // TODO: add customize to that !
            //for (int j = 0; j < SHOTGUN_SPARKS_MAX; j++)
            //    TriggerShotgunSparks(x, y, z, (pos.x - x) << 1, (pos.y - y) << 1, (pos.z - z) << 1);
        }

        Lara.rightArm.flash_gun = Weapons[LG_SHOTGUN].flashTime;
        SoundEffect(SFX_EXPLOSION1, &LaraItem->pos, PITCH_SHIFT | 0x1400000);
        SoundEffect(Weapons[LG_SHOTGUN].sampleNum, &LaraItem->pos, 0);
        savegame_level.ammo_used++;
    }
}

void FireGrenade(void)
{
    short ammo = weapon_ammo.get(LG_GRENADEGUN);
    if (ammo)
    {
        ITEM_INFO* item;
        FLOOR_INFO* floor;
        PHD_VECTOR pos;
        int x, y, z;
        int height;

        short itemNumber = CreateItem();
        Lara.hasFired = TRUE;
        if (itemNumber != NO_ITEM)
        {
            item = &Items[itemNumber];
            item->shade = -15856;
            item->objectNumber = GRENADE;
            item->roomNumber = LaraItem->roomNumber;

            pos.x = 0;
            pos.y = 276;
            pos.z = 80;
            GetLaraJointAbsPosition(&pos, JHAND_R);
            x = pos.x;
            y = pos.y;
            z = pos.z;
            floor = GetFloor(pos.x, pos.y, pos.z, &item->roomNumber);
            height = GetFloorHeight(floor, pos.x, pos.y, pos.z);
            if (height < pos.y)
            {
                item->pos.xPos = LaraItem->pos.xPos;
                item->pos.yPos = pos.y;
                item->pos.zPos = LaraItem->pos.zPos;
                item->roomNumber = LaraItem->roomNumber;
            }
            else
            {
                item->pos.xPos = pos.x;
                item->pos.yPos = pos.y;
                item->pos.zPos = pos.z;
            }

            pos.x = 0;
            pos.y = 1204;
            pos.z = 80;
            GetLaraJointAbsPosition(&pos, JHAND_R);

            SmokeCountL = 32;
            SmokeWeapon = LG_GRENADEGUN;

            if (LaraItem->meshBits)
            {
                for (int i = 0; i < GRENADEGUN_SMOKE_MAX; i++)
                    TriggerGunSmoke(x, y, z, pos.x - x, pos.y - y, pos.z - z, 1, SmokeWeapon, SmokeCountL);
            }

            InitialiseItem(itemNumber);
            item->pos.xRot = LaraItem->pos.xRot + Lara.leftArm.xRot;
            item->pos.yRot = LaraItem->pos.yRot + Lara.leftArm.yRot;
            item->pos.zRot = 0;

            if (!Lara.leftArm.lock)
            {
                item->pos.xRot += Lara.torsoXrot;
                item->pos.yRot += Lara.torsoYrot;
            }

            item->speed = 128;
            item->fallspeed = -item->speed * SIN(item->pos.xRot) >> W2V_SHIFT;
            item->currentAnimState = item->pos.xRot;    // x rotation
            item->goalAnimState = item->pos.yRot;       // y rotation
            item->requiredAnimState = FALSE;            // rolling on floor ?
            item->hitPoints = GRENADE_TIMER;
            AddActiveItem(itemNumber);

            if (ammo != INFINITE_AMMO)
                weapon_ammo.decrease(LG_GRENADEGUN, 1);

            if (CHK_EXI(Lara.grenadegun_type_carried, CR_AMMO1))
                item->itemFlags[0] = 1; // normal
            else if (CHK_EXI(Lara.grenadegun_type_carried, CR_AMMO2))
                item->itemFlags[0] = 2; // super
            else if (CHK_EXI(Lara.grenadegun_type_carried, CR_AMMO3))
                item->itemFlags[0] = 3; // flash
            savegame_level.ammo_used++;
        }
    }
}

void FireCrossbow(PHD_3DPOS* haveDefinedPos)
{
    short ammo = weapon_ammo.get(LG_CROSSBOW);
    if (ammo)
    {
        ITEM_INFO* item;
        FLOOR_INFO* floor;
        PHD_VECTOR pos;
        int height;

        short itemNumber = CreateItem();
        Lara.hasFired = TRUE;
        if (itemNumber != NO_ITEM)
        {
            item = &Items[itemNumber];
            item->objectNumber = CROSSBOW_BOLT;
            item->shade = -15856;

            if (haveDefinedPos)
            {
                item->pos.xPos = haveDefinedPos->xPos;
                item->pos.yPos = haveDefinedPos->yPos;
                item->pos.zPos = haveDefinedPos->zPos;
                item->roomNumber = LaraItem->roomNumber;
                InitialiseItem(itemNumber);
                item->pos.xRot = haveDefinedPos->xRot;
                item->pos.yRot = haveDefinedPos->yRot;
                item->pos.zRot = haveDefinedPos->zRot;
            }
            else
            {
                pos.x = 0;
                pos.y = 228;
                pos.z = 32;
                GetLaraJointAbsPosition(&pos, JHAND_R);
                item->roomNumber = LaraItem->roomNumber;
                floor = GetFloor(pos.x, pos.y, pos.z, &item->roomNumber);
                height = GetFloorHeight(floor, pos.x, pos.y, pos.z);

                if (height < pos.y)
                {
                    item->pos.xPos = LaraItem->pos.xPos;
                    item->pos.yPos = pos.y;
                    item->pos.zPos = LaraItem->pos.zPos;
                    item->roomNumber = LaraItem->roomNumber;
                }
                else
                {
                    item->pos.xPos = pos.x;
                    item->pos.yPos = pos.y;
                    item->pos.zPos = pos.z;
                }

                InitialiseItem(itemNumber);
                item->pos.xRot = LaraItem->pos.xRot + Lara.leftArm.xRot;
                item->pos.yRot = LaraItem->pos.yRot + Lara.leftArm.yRot;
                item->pos.zRot = 0;

                if (!Lara.leftArm.lock)
                {
                    item->pos.xRot += Lara.torsoXrot;
                    item->pos.yRot += Lara.torsoYrot;
                }
            }

            item->speed = 512;
            AddActiveItem(itemNumber);

            if (ammo != INFINITE_AMMO)
                weapon_ammo.decrease(LG_CROSSBOW, 1);

            if (CHK_EXI(Lara.crossbow_type_carried, CR_AMMO1))
                item->itemFlags[0] = 1; // normal
            else if (CHK_EXI(Lara.crossbow_type_carried, CR_AMMO2))
                item->itemFlags[0] = 2; // poison
            else if (CHK_EXI(Lara.crossbow_type_carried, CR_AMMO3))
                item->itemFlags[0] = 3; // explosive
            SoundEffect(SFX_LARA_CROSSBOW, &LaraItem->pos, 0);
            savegame_level.ammo_used++;
        }
    }
}

void FireCrossbowBoltFromLaserSight(GAME_VECTOR* dest, GAME_VECTOR* src)
{
    PHD_3DPOS pos;
    short angles[2];
    phd_GetVectorAngles(src->x - dest->x, src->y - dest->y, src->z - dest->z, angles);
    pos.xPos = dest->x;
    pos.yPos = dest->y;
    pos.zPos = dest->z;
    pos.xRot = angles[1];
    pos.yRot = angles[0];
    pos.zRot = 0;
    FireCrossbow(&pos);
}

void InitialiseNewWeapons(void)
{
    Lara.target = nullptr;

    Lara.leftArm.frameNumber = 0;
    Lara.leftArm.xRot = 0;
    Lara.leftArm.yRot = 0;
    Lara.leftArm.zRot = 0;
    Lara.leftArm.lock = FALSE;
    Lara.leftArm.flash_gun = 0;

    Lara.rightArm.frameNumber = 0;
    Lara.rightArm.xRot = 0;
    Lara.rightArm.yRot = 0;
    Lara.rightArm.zRot = 0;
    Lara.rightArm.lock = FALSE;
    Lara.rightArm.flash_gun = 0;

    switch (Lara.gunType)
    {
        case LG_PISTOLS:
        case LG_UZIS:
            Lara.rightArm.frameBase = Objects[PISTOLS_ANIM].frameBase;
            Lara.leftArm.frameBase = Objects[PISTOLS_ANIM].frameBase;
            if (Lara.gunStatus != LHS_ARMLESS)
                DrawPistolsMeshes(Lara.gunType);
            break;
        case LG_REVOLVER:
        case LG_SHOTGUN:
        case LG_GRENADEGUN:
            Lara.leftArm.frameBase = Lara.rightArm.frameBase = Objects[WeaponObject(Lara.gunType)].frameBase;
            if (Lara.gunStatus != LHS_ARMLESS)
                DrawShotgunMeshes(Lara.gunType);
            break;
        case LG_FLARE:
            Lara.leftArm.frameBase = Lara.rightArm.frameBase = Objects[FLARE_ANIM].frameBase;
            if (Lara.gunStatus != LHS_ARMLESS)
                DrawFlareMeshes();
            break;
        default:
            Lara.leftArm.frameBase = Lara.rightArm.frameBase = Anims[LaraItem->animNumber].framePtr;
            break;
    }
}

// dont used anymore...
// use weapon_ammo variable in utils.h
short* GetCurrentAmmo(int weapon_type)
{
    switch (weapon_type)
    {
        case LG_REVOLVER:
            return &Lara.revolver_ammo_count;

        case LG_UZIS:
            return &Lara.uzi_ammo_count;

        case LG_SHOTGUN:
            if (CHK_EXI(Lara.shotgun_type_carried, CR_AMMO1))
                return &Lara.shotgun_ammo1_count;
            else
                return &Lara.shotgun_ammo2_count;
            break;
        case LG_GRENADEGUN:
            if (CHK_EXI(Lara.grenadegun_type_carried, CR_AMMO1))
                return &Lara.grenade_ammo1_count;
            else if (CHK_EXI(Lara.grenadegun_type_carried, CR_AMMO2))
                return &Lara.grenade_ammo2_count;
            else
                return &Lara.grenade_ammo3_count;
            break;

        case LG_CROSSBOW:
            if (CHK_EXI(Lara.crossbow_type_carried, CR_AMMO1))
                return &Lara.crossbow_ammo1_count;
            else if (CHK_EXI(Lara.crossbow_type_carried, CR_AMMO2))
                return &Lara.crossbow_ammo2_count;
            else
                return &Lara.crossbow_ammo3_count;
            break;

        case LG_PISTOLS:
        default:
            return &Lara.pistol_ammo_count;
    }
}

void LaraTargetInfo(WEAPON_INFO* winfo)
{
    ITEM_INFO *item;
    GAME_VECTOR src;
    GAME_VECTOR target;
    short angles[2];

    item = Lara.target;
    if (!item)
    {
        Lara.rightArm.lock = FALSE;
        Lara.leftArm.lock = FALSE;
        Lara.targetAngles[1] = 0;
        Lara.targetAngles[0] = 0;
        return;
    }

    src.x = 0;
    src.y = 0;
    src.z = 0;
    GetLaraJointAbsPosition((PHD_VECTOR*)&src, JHAND_R);
    src.roomNumber = LaraItem->roomNumber;

    find_target_point(item, &target);
    phd_GetVectorAngles(target.x - src.x, target.y - src.y, target.z - src.z, angles);
    angles[0] -= LaraItem->pos.yRot;
    angles[1] -= LaraItem->pos.xRot;

    if (LOS(&src, &target))
    {
        if (angles[0] >= winfo->lockAngles[0]
        &&  angles[0] <= winfo->lockAngles[1]
        &&  angles[1] >= winfo->lockAngles[2]
        &&  angles[1] <= winfo->lockAngles[3])
        {
            Lara.rightArm.lock = TRUE;
            Lara.leftArm.lock = TRUE;
        }
        else
        {
            if (Lara.leftArm.lock)
            {
                if (angles[0] < winfo->leftAngles[0]
                ||  angles[0] > winfo->leftAngles[1]
                ||  angles[1] < winfo->leftAngles[2]
                ||  angles[1] > winfo->leftAngles[3])
                    Lara.leftArm.lock = FALSE;
            }

            if (Lara.rightArm.lock)
            {
                if (angles[0] < winfo->rightAngles[0]
                ||  angles[0] > winfo->rightAngles[1]
                ||  angles[1] < winfo->rightAngles[2]
                ||  angles[1] > winfo->rightAngles[3])
                    Lara.rightArm.lock = FALSE;
            }
        }
    }
    else
    {
        Lara.rightArm.lock = FALSE;
        Lara.leftArm.lock = FALSE;
    }

    Lara.targetAngles[0] = angles[0];
    Lara.targetAngles[1] = angles[1];
}

void LaraGetNewTarget(WEAPON_INFO* winfo)
{
    if (BinocularRange)
    {
        Lara.target = nullptr;
        return;
    }

    ITEM_INFO* item, *bestitem;
    CREATURE_INFO* creature;
    GAME_VECTOR src, target;
    int x, y, z;
    int dist, maxdist, maxdist2, bestdist;
    short angles[2];
    short bestyrot, yrot;

    src.x = LaraItem->pos.xPos;
    src.y = LaraItem->pos.yPos - 650;
    src.z = LaraItem->pos.zPos;
    src.roomNumber = LaraItem->roomNumber;
    item = nullptr;
    bestitem = nullptr;
    bestyrot = MAXSHORT;
    bestdist = MAXINT;
    maxdist = winfo->targetDist;
    maxdist2 = SQUARE(maxdist);

    creature = BaddieSlots;
    for (int slots = 0; slots < NUM_SLOTS; slots++, creature++)
    {
        if (creature->itemNum == NO_ITEM || creature->itemNum == Lara.itemNumber)
            continue;

        item = &Items[creature->itemNum];
        if (item->hitPoints <= 0)
            continue;
        if (!Objects[item->objectNumber].intelligent)
            continue;
        
        x = item->pos.xPos - src.x;
        y = item->pos.yPos - src.y;
        z = item->pos.zPos - src.z;

        if (abs(x) > maxdist || abs(y) > maxdist || abs(z) > maxdist)
            continue;

        dist = SQUARE(x) + SQUARE(y) + SQUARE(z);
        if (dist < maxdist2)
        {
            find_target_point(item, &target);

            if (LOS(&src, &target))
            {
                phd_GetVectorAngles(target.x - src.x, target.y - src.y, target.z - src.z, angles);
                angles[0] -= LaraItem->pos.yRot + Lara.torsoYrot;
                angles[1] -= LaraItem->pos.xRot = Lara.torsoXrot;

                if (angles[0] >= winfo->lockAngles[0]
                &&  angles[0] <= winfo->lockAngles[1]
                &&  angles[1] >= winfo->lockAngles[2]
                &&  angles[1] <= winfo->lockAngles[3])
                {
                    yrot = abs(angles[0]);
                    if (yrot < (bestyrot + NEAR_ANGLE) && dist < bestdist)
                    {
                        bestdist = dist;
                        bestyrot = yrot;
                        bestitem = item;
                    }
                }
            }
        }
    }

    Lara.target = bestitem;
    LaraTargetInfo(winfo);
}

void AimWeapon(WEAPON_INFO* winfo, LARA_ARM* arm)
{
    short curr, speed;
    short destx, desty;
    
    speed = winfo->aimSpeed;
    if (arm->lock)
    {
        desty = Lara.targetAngles[0];
        destx = Lara.targetAngles[1];
    }
    else
    {
        desty = 0;
        destx = 0;
    }

    // move y
    curr = arm->yRot;
    if (curr >= (desty - speed) && curr <= (desty + speed))
        curr = desty;
    else if (curr < desty)
        curr += speed;
    else
        curr -= speed;
    arm->yRot = curr;

    // move x
    curr = arm->xRot;
    if (curr >= (destx - speed) && curr <= (destx + speed))
        curr = destx;
    else if (curr < destx)
        curr += speed;
    else
        curr -= speed;
    arm->xRot = curr;
    arm->zRot = 0;
}

void find_target_point(ITEM_INFO* item, GAME_VECTOR* target)
{
    short* bounds;
    int x, y, z, c, s;

    bounds = GetBestFrame(item);
    x = (int)((bounds[0] + bounds[1]) / 2);
    y = (int)(bounds[2] + (bounds[3] - bounds[2]) / 3);
    z = (int)((bounds[4] + bounds[5]) / 2);
    c = COS(item->pos.yRot);
    s = SIN(item->pos.yRot);

    target->x = item->pos.xPos + ((c * x + s * z) >> W2V_SHIFT);
    target->y = item->pos.yPos + y;
    target->z = item->pos.zPos + ((c * z - s * x) >> W2V_SHIFT);
    target->roomNumber = item->roomNumber;
}

//  1 = BADDIE HIT
// -1 = BADDIE MISSED
//  0 = OUT OF AMMO
int FireWeapon(int weapon_type, ITEM_INFO* target, ITEM_INFO* src, short angles[2])
{
    PHD_MATRIX* mptr;
    PHD_3DPOS viewpos;
    GAME_VECTOR vsrc, vdest;
    WEAPON_INFO* winfo;
    SPHERE* sptr, slist[33];
    int r, i, nums, bestdist, best;
    short roomNumber;
    short ammo;
    
    winfo = &Weapons[weapon_type];
    ammo = weapon_ammo.get(weapon_type);
    if (ammo)
    {
        if (ammo != INFINITE_AMMO) // fix negative ammo !
            weapon_ammo.decrease(weapon_type, 1);

        viewpos.xPos = src->pos.xPos;
        viewpos.yPos = src->pos.yPos - winfo->gunHeight;
        viewpos.zPos = src->pos.zPos;
        r = (int)((GetRandomControl() - 0x4000) * winfo->shotAccuracy) / 0x10000;
        viewpos.xRot = angles[1] + (short)r;
        r = (int)((GetRandomControl() - 0x4000) * winfo->shotAccuracy) / 0x10000;
        viewpos.yRot = angles[0] + (short)r;
        viewpos.zRot = 0;

        phd_GenerateW2V(&viewpos);
        nums = GetSpheres(target, slist, 0);
        sptr = &slist[0];
        best = -1;
        bestdist = MAXINT;
        
        for (i = 0; i < nums; i++, sptr++)
        {
            r = sptr->r;

            if (abs(sptr->x) < r && abs(sptr->y) < r && sptr->z > r && (SQUARE(sptr->x) + SQUARE(sptr->y) <= SQUARE(r)))
            {
                if ((sptr->z - r) < bestdist)
                {
                    bestdist = sptr->z - r;
                    best = i;
                }
            }
        }

        roomNumber = src->roomNumber;
        GetFloor(viewpos.xPos, viewpos.yPos, viewpos.zPos, &roomNumber);

        Lara.hasFired = TRUE;
        vdest.x = viewpos.xPos;
        vdest.y = viewpos.yPos;
        vdest.z = viewpos.zPos;
        vdest.roomNumber = roomNumber;
        mptr = phd_mxptr;

        if (best < 0)
        {
            vsrc.x = vdest.x + ((SECTOR(20) * mptr->m20) >> W2V_SHIFT);
            vsrc.y = vdest.y + ((SECTOR(20) * mptr->m21) >> W2V_SHIFT);
            vsrc.z = vdest.z + ((SECTOR(20) * mptr->m22) >> W2V_SHIFT);
            GetTargetOnLOS(&vdest, &vsrc, FALSE, TRUE);
            return -1;
        }
        else
        {
            savegame_level.ammo_hits++;
            vsrc.x = vdest.x + ((bestdist * mptr->m20) >> W2V_SHIFT);
            vsrc.y = vdest.y + ((bestdist * mptr->m21) >> W2V_SHIFT);
            vsrc.z = vdest.z + ((bestdist * mptr->m22) >> W2V_SHIFT);
            if (!GetTargetOnLOS(&vdest, &vsrc, FALSE, TRUE))
                HitTarget(target, &vsrc, winfo->damage, 0);
            return 1;
        }
    }
    else
    {
        return 0; // no more ammo !
    }
}

int GetTargetOnLOS(GAME_VECTOR* dest, GAME_VECTOR* src, BOOL drawtarget, BOOL firing)
{
    ITEM_INFO* item;
    OBJECT_INFO* obj;
    MESH_INFO* mesh;
    FLOOR_INFO* floor;
    PHD_VECTOR vec;
    BOOL result;
    int los, closest;
    short roomNumber;
    short static_obj;

    los = LOS(dest, src);
    GetFloor(src->x, src->y, src->z, &src->roomNumber);
    if (firing && LaserSight && Lara.gunType == LG_REVOLVER)
        SoundEffect(SFX_REVOLVER_FIRE, &LaraItem->pos, 0);

    closest = ObjectOnLOS2(dest, src, &vec, &mesh);
    if (closest == 999)
    {
        if (Lara.gunType == LG_CROSSBOW)
        {
            if (firing && LaserSight)
                FireCrossbowBoltFromLaserSight(dest, src);
        }
        else
        {
            src->x -= (src->x - dest->x) >> 5;
            src->y -= (src->y - dest->y) >> 5;
            src->z -= (src->z - dest->z) >> 5;
            if (firing && !los)
                TriggerRicochetSpark(src, LaraItem->pos.yRot, 8, 0);
        }
        result = FALSE;
    }
    else
    {
        src->x = vec.x - ((vec.x - dest->x) >> 5);
        src->y = vec.y - ((vec.y - dest->y) >> 5);
        src->z = vec.z - ((vec.z - dest->z) >> 5);

        if (firing)
        {
            item = &Items[closest];
            obj = &Objects[item->objectNumber];

            if (Lara.gunType == LG_CROSSBOW)
            {
                if (LaserSight)
                    FireCrossbowBoltFromLaserSight(dest, src);
            }
            else if (closest < 0)
            {
                static_obj = mesh->static_number;
                if (static_obj >= SHATTER0 && static_obj <= SHATTER7)
                {
                    ShatterObject(nullptr, mesh, 128, src->roomNumber, FALSE);
                    short mesh_count = SmashedMeshCount++;
                    SmashedMeshRoom[mesh_count] = src->roomNumber;
                    SmashedMesh[mesh_count] = src;
                    mesh->ocb = MESH_INVISIBLE;
                    SoundEffect(SFX_HIT_ROCK, (PHD_3DPOS*)src, 0);
                }
                TriggerRicochetSpark(src, LaraItem->pos.yRot, 3, 0);
                TriggerRicochetSpark(src, LaraItem->pos.yRot, 3, 0);
            }
            else if (item->objectNumber != SWITCH_TYPE7 && item->objectNumber != SWITCH_TYPE8)
            {
                if (ShatterItem.bit & obj->explodableMeshBits && LaserSight)
                {
                    ShatterObject(&ShatterItem, nullptr, 128, src->roomNumber, FALSE);
                    item->meshBits &= ~ShatterItem.bit;
                    TriggerRicochetSpark(src, LaraItem->pos.yRot, 3, 0);
                }
                else if (drawtarget && Lara.gunType == LG_REVOLVER)
                {
                    if (obj->intelligent)
                        HitTarget(item, src, Weapons[LG_REVOLVER].damage, 0);
                }
                
                switch (obj->hitEffect)
                {
                    case HIT_BLOOD:
                        DoBloodSplat(src->x, src->y, src->z, (GetRandomControl() & 3) + 3, item->pos.yRot, item->roomNumber);
                        break;
                    case HIT_SMOKE:
                        TriggerRicochetSpark(src, LaraItem->pos.yRot, 3, -5); // no fragment, just smoke !
                        break;
                    case HIT_FRAGMENT:
                        TriggerRicochetSpark(src, LaraItem->pos.yRot, 3, 0);
                        break;
                }

                item->hitStatus = TRUE;
                if (!obj->undead && obj->hitPoints != DONT_TARGET)
                    item->hitPoints -= Weapons[Lara.gunType].damage;
            }
            else if (ShatterItem.bit == 1 << (obj->nMeshes - 1) && CHK_NOP(item->flags, IFLAG_SWITCH_ONESHOT))
            {
                if (item->objectNumber == SWITCH_TYPE7)
                    ExplodeItemNode(item, obj->nMeshes - 1, 0, 64);

                roomNumber = item->roomNumber;
                if (CHK_EXI(item->flags, IFLAG_CODEBITS) && CHK_EQA(item->flags, IFLAG_CODEBITS))
                {
                    floor = GetFloor(item->pos.xPos, item->pos.yPos - STEP_L, item->pos.zPos, &roomNumber);
                    GetFloorHeight(floor, item->pos.xPos, item->pos.yPos - STEP_L, item->pos.zPos);
                    TestTriggers(TriggerIndex, TRUE, CHK_EXI(item->flags, IFLAG_CODEBITS));
                }
                else
                {
                    short itemNumber;
                    short number_trigger = GetSwitchTrigger(item, &itemNumber, 1);
                    
                    for (int i = 0; i < number_trigger; i++)
                    {
                        AddActiveItem(itemNumber);
                        Items[itemNumber].flags |= IFLAG_CODEBITS | IFLAG_SWITCH_ONESHOT;
                        Items[itemNumber].status = FITEM_ACTIVE;
                    }
                }
            }
            else
            {
                TriggerRicochetSpark(src, LaraItem->pos.yRot, 3, 0);
            }
        }
        result = TRUE;
    }

    if (drawtarget && (result || !los))
    {
        LaserSightActive = TRUE;
        LaserSightPos.z = src->x;
        LaserSightPos.x = src->y;
        LaserSightPos.y = src->z;
        TriggerDynamic(src->x, src->y, src->z, 16, 32, 0, 0);
    }
    return result;
}

void HitTarget(ITEM_INFO* item, GAME_VECTOR* src, int damage, int flags)
{
    if (item->data == nullptr)
        return;

    CREATURE_INFO* creature = (CREATURE_INFO*)item->data;
    OBJECT_INFO* obj = &Objects[item->objectNumber];

    item->hitStatus = TRUE;
    if (creature && item != LaraItem)
        creature->hurtByLara = TRUE;

    if (src)
    {
        switch (obj->hitEffect)
        {
            case HIT_BLOOD:
                if (item->objectNumber == BADDY_2 && (item->currentAnimState == 8 || GetRandomControl() & 1) && (Lara.gunType == LG_PISTOLS || Lara.gunType == LG_SHOTGUN || Lara.gunType == LG_UZIS))
                {
                    SoundEffect(SFX_BAD_SWORD_RICO, &item->pos, 0);
                    TriggerRicochetSpark(src, LaraItem->pos.yRot, 3, 0);
                    return;
                }
                DoBloodSplat(src->x, src->y, src->z, (GetRandomControl() & 3) + 3, item->pos.yRot, item->roomNumber);
                break;
            case HIT_SMOKE:
                TriggerRicochetSpark(src, LaraItem->pos.yRot, 3, -5); // no fragment, just smoke !
                break;
            case HIT_FRAGMENT:
                TriggerRicochetSpark(src, LaraItem->pos.yRot, 3, 0);
                break;
        }
    }

    if (!obj->undead || flags || item->hitPoints != DONT_TARGET)
    {
        if (item->hitPoints > 0 && item->hitPoints <= damage)
            savegame_level.kills++;
        item->hitPoints -= damage;
    }
}

void LaraGun(void)
{
    if (Lara.leftArm.flash_gun > 0)
        Lara.leftArm.flash_gun--;
    if (Lara.rightArm.flash_gun > 0)
        Lara.rightArm.flash_gun--;

    if (Lara.gunType == LG_TORCH)
    {
        TorchControl();
        return;
    }

    if (LaraItem->hitPoints <= 0)
    {
        Lara.gunStatus = LHS_ARMLESS;
    }
    else if (Lara.gunStatus == LHS_ARMLESS)
    {
        if (CHK_EXI(TrInput, IN_DRAW))
        {
            Lara.requestGunType = Lara.lastGunType;
        }
        else if (CHK_EXI(TrInput, IN_FLARE) && CHK_NOP(gfLevelFlags, SLEV_YOUNG_LARA))
        {
            if (LaraItem->currentAnimState == STATE_LARA_CROUCH_IDLE && LaraItem->animNumber != ANIMATION_LARA_CROUCH_IDLE)
                return;

            if (Lara.gunType == LG_FLARE)
            {
                Lara.gunStatus = LHS_UNDRAW;
            }
            else if (Lara.flare_count != 0 || Lara.flare_count == INFINITE_AMMO)
            {
                if (Lara.flare_count != INFINITE_AMMO)
                    Lara.flare_count--;
                Lara.requestGunType = LG_FLARE;
            }
        }

        if (CHK_EXI(TrInput, IN_DRAW) || Lara.requestGunType != Lara.gunType)
        {
            short state_current = LaraItem->currentAnimState;
            if ((state_current == STATE_LARA_CROUCH_IDLE
            ||   state_current == STATE_LARA_CROUCH_TURN_LEFT
            ||   state_current == STATE_LARA_CROUCH_TURN_RIGHT)
            &&  (Lara.requestGunType == LG_SHOTGUN || Lara.requestGunType == LG_CROSSBOW || Lara.requestGunType == LG_GRENADEGUN))
                return;
            
            if (Lara.requestGunType == LG_FLARE
            ||  Lara.skidoo == NO_ITEM
            &&  Lara.waterStatus == LWS_ABOVEWATER
            && (Lara.waterStatus != LWS_WADE || Lara.waterSurfaceDist <= -Weapons[Lara.gunType].gunHeight))
            {
                if (Lara.gunType == LG_FLARE)
                {
                    CreateFlare(FLARE_ITEM, FALSE);
                    UndrawFlareMeshes();
                    Lara.flareControlLeft = FALSE;
                }

                Lara.gunType = Lara.requestGunType;
                InitialiseNewWeapons();
                Lara.gunStatus = LHS_DRAW;
                Lara.leftArm.frameNumber = 0;
                Lara.rightArm.frameNumber = 0;
            }
            else
            {
                Lara.lastGunType = Lara.requestGunType;
                if (Lara.gunType == LG_FLARE)
                    Lara.requestGunType = LG_FLARE;
                else
                    Lara.gunType = Lara.requestGunType;
            }
        }
    }
    else if (Lara.gunStatus == LHS_READY)
    {
        if (CHK_EXI(TrInput, IN_FLARE))
        {
            if (Lara.flare_count != -1)
            {
                Lara.flare_count--;
                Lara.requestGunType = LG_FLARE;
            }
        }

        if (CHK_EXI(TrInput, IN_DRAW) || Lara.requestGunType != Lara.gunType)
            Lara.gunStatus = LHS_UNDRAW;
        else if (Lara.waterStatus != LWS_ABOVEWATER
        &&      (Lara.waterStatus != LWS_WADE || Lara.waterSurfaceDist < -Weapons[Lara.gunType].gunHeight))
        {
            Lara.gunStatus = LHS_UNDRAW;
        }
    }

    switch (Lara.gunStatus)
    {
        case LHS_ARMLESS:
            if (Lara.gunType == LG_FLARE)
            {
                if (Lara.skidoo != NO_ITEM || CheckForHoldingState(LaraItem->currentAnimState))
                {
                    if (Lara.leftArm.frameNumber != FL_HOLD_F)
                    {
                        Lara.leftArm.frameNumber++;
                        if (Lara.leftArm.frameNumber == FL_END_F)
                            Lara.leftArm.frameNumber = FL_HOLD_F;
                    }
                    else if (!Lara.flareControlLeft)
                    {
                        Lara.flareControlLeft = TRUE;
                        Lara.leftArm.frameNumber = FL_2HOLD_F;
                    }
                }
                else
                {
                    Lara.flareControlLeft = FALSE;
                }

                DoFlareInHand(Lara.flareAge);
                SetFlareArm(Lara.leftArm.frameNumber);
            }
            break;
        case LHS_HANDBUSY:
            if (Lara.gunType == LG_FLARE && Lara.mesh.hand_l == ClassicMeshes(FLARE_ANIM, HAND_L))
            {
                Lara.flareControlLeft = (Lara.skidoo != NO_ITEM || CheckForHoldingState(LaraItem->currentAnimState));
                DoFlareInHand(Lara.flareAge);
                SetFlareArm(Lara.leftArm.frameNumber);
            }
            break;
        case LHS_DRAW:
            if (Lara.gunType != LG_FLARE && Lara.gunType != LG_UNARMED)
                Lara.lastGunType = Lara.gunType;

            switch (Lara.gunType)
            {
                case LG_PISTOLS:
                case LG_REVOLVER:
                case LG_UZIS:
                    if (camera.type != CINEMATIC_CAMERA && camera.type != LOOK_CAMERA && camera.type != HEAVY_CAMERA)
                        camera.type = COMBAT_CAMERA;
                    DrawPistols(Lara.gunType);
                    break;
                case LG_SHOTGUN:
                case LG_GRENADEGUN:
                case LG_CROSSBOW:
                    if (camera.type != CINEMATIC_CAMERA && camera.type != LOOK_CAMERA && camera.type != HEAVY_CAMERA)
                        camera.type = COMBAT_CAMERA;
                    DrawShotgun(Lara.gunType);
                    break;
                case LG_FLARE:
                    DrawFlare();
                    break;
                default:
                    Lara.gunStatus = LG_UNARMED;
                    break;
            }
            break;
        case LHS_UNDRAW:
            Lara.mesh.head = ClassicMeshes(LARA, HEAD);

            switch (Lara.gunType)
            {
                case LG_PISTOLS:
                case LG_REVOLVER:
                case LG_UZIS:
                    UndrawPistols(Lara.gunType);
                    break;
                case LG_SHOTGUN:
                case LG_GRENADEGUN:
                case LG_CROSSBOW:
                    UndrawShotgun(Lara.gunType);
                    break;
                case LG_FLARE:
                    UndrawFlare();
                    break;
            }
            break;
        case LHS_READY:
            if (CHK_EXI(TrInput, IN_ACTION))
                Lara.mesh.head = ClassicMeshes(LARA_SCREAM, HEAD);
            else
                Lara.mesh.head = ClassicMeshes(LARA, HEAD);

            if (camera.type != CINEMATIC_CAMERA && camera.type != LOOK_CAMERA && camera.type != HEAVY_CAMERA)
                camera.type = COMBAT_CAMERA;

            if (CHK_EXI(TrInput, IN_ACTION) && !weapon_ammo.get(Lara.gunType))
            {
                SoundEffect(SFX_SARLID_PALACES, &LaraItem->pos, 0);
                Lara.requestGunType = LG_PISTOLS;
                return;
            }

            switch (Lara.gunType)
            {
                case LG_PISTOLS:
                case LG_UZIS:
                    PistolHandler(Lara.gunType);
                    break;
                case LG_REVOLVER:
                case LG_SHOTGUN:
                case LG_GRENADEGUN:
                case LG_CROSSBOW:
                    ShotgunHandler(Lara.gunType);
                    break;
            }
            break;
        case LHS_SPECIAL:
            DrawFlare();
            break;
    }
}

#ifdef DLL_INJECT
void injector::f_game::inject_larafire()
{
    inject(0x0042EA70, WeaponObject);
    inject(0x0042EAC0, WeaponMeshes);
    inject(0x00429260, FireShotgun);
    inject(0x00429480, FireGrenade);
    inject(0x0042A270, FireCrossbow);
    inject(0x0044D820, FireCrossbowBoltFromLaserSight);
    inject(0x0042DDF0, InitialiseNewWeapons);
    inject(0x0042F480, GetCurrentAmmo);
    inject(0x0042DF30, LaraTargetInfo);
    inject(0x0042E0D0, LaraGetNewTarget);
    inject(0x0042E560, AimWeapon);
    inject(0x0042E4A0, find_target_point);
    inject(0x0042E630, FireWeapon);
    inject(0x0044D890, GetTargetOnLOS);
    inject(0x0042E920, HitTarget);
    inject(0x0042D840, LaraGun);
}
#endif