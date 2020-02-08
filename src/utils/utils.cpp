#include "framework.h"
#include "utils.h"
#include "3dsystem/3d_gen.h"
#include "3dsystem/3d_gen_a.h"
#include "game/box.h"
#include "game/control.h"
#include "game/draw.h"
#include "game/effect2.h"
#include "game/items.h"
#include "game/oldobjects.h"
#include "game/sphere.h"
#include "specific/json/reader.h"
#include "specific/drawprimitive.h"

bool isFolderExists(LPCSTR folder_name)
{
    struct stat st;
    int ret = stat(folder_name, &st);
    return (ret == 0) && (st.st_mode & S_IFDIR);
}

bool isFileExists(LPCSTR file_name)
{
    struct stat st;
    return (stat(file_name, &st) == FALSE);
}

void createFolders(LPCSTR folder_name)
{
    _mkdir(folder_name);
}

short GetCurrentFrame(ITEM_INFO * item)
{
    return (item->current_frame - anims[item->current_anim].frame_base);
}

void phd_SwapMatrix(int* dest, int* src)
{
    dest[M00] = src[M00];
    dest[M01] = src[M01];
    dest[M02] = src[M02];
    dest[M03] = src[M03];
    dest[M10] = src[M10];
    dest[M11] = src[M11];
    dest[M12] = src[M12];
    dest[M13] = src[M13];
    dest[M20] = src[M20];
    dest[M21] = src[M21];
    dest[M22] = src[M22];
    dest[M23] = src[M23];
}

void phd_SwapGunMatrix(PHD_MATRIX& dest, int *src, bool swap)
{
    if (swap)
    {
        src[M00] = dest.m00;
        src[M01] = dest.m01;
        src[M02] = dest.m02;
        src[M03] = dest.m03;
        src[M10] = dest.m10;
        src[M11] = dest.m11;
        src[M12] = dest.m12;
        src[M13] = dest.m13;
        src[M20] = dest.m20;
        src[M21] = dest.m21;
        src[M22] = dest.m22;
        src[M23] = dest.m23;
    }
    else
    {
        dest.m00 = src[M00];
        dest.m01 = src[M01];
        dest.m02 = src[M02];
        dest.m03 = src[M03];
        dest.m10 = src[M10];
        dest.m11 = src[M11];
        dest.m12 = src[M12];
        dest.m13 = src[M13];
        dest.m20 = src[M20];
        dest.m21 = src[M21];
        dest.m22 = src[M22];
        dest.m23 = src[M23];
    }
}

void SpawnPickup(ITEM_INFO *item)
{
    ITEM_INFO* pickup;
    FLOOR_INFO* floor;
    PHD_VECTOR pos; // custom position
    short pickup_number;

    pos.x = 0;
    pos.y = 0;
    pos.z = 0;

    // custom item position based on the JointPos else ItemPos
    switch (item->object_number)
    {
        case BADDY_1: 
        case BADDY_2:
            GetJointAbsPosition(item, &pos, 6);
            break;

        default:
            pos.x = item->pos.x;
            pos.y = item->pos.y;
            pos.z = item->pos.z;
            break;
    }

    pickup_number = item->carried_item;
    do
    {
        if (pickup_number == NO_ITEM)
            break;

        pickup = &items[pickup_number];
        floor = GetFloor(pos.x, pos.y, pos.z, &item->room_number);

        pickup->pos.x = pos.x;
        pickup->pos.z = pos.z;
        pickup->pos.y = GetHeight(floor, pickup->pos.x, item->pos.y, pickup->pos.z);
        pickup->pos.y -= GetBoundsAccurate(pickup)[3];
        
        if (pickup->object_number == BIGMEDI_ITEM)
            pickup->pos.y_rot = item->pos.y_rot + 0x8000;
        else
            pickup->pos.y_rot = item->pos.y_rot;

        ItemNewRoom(pickup_number, item->room_number);
        pickup_number = pickup->carried_item;
    }
    while (pickup_number != NO_ITEM);
}

#define PISTOLS_GUNPOS_X 10
#define PISTOLS_GUNPOS_Y 190
#define PISTOLS_GUNPOS_Z 40

#define UZIS_GUNPOS_X 0
#define UZIS_GUNPOS_Y 190
#define UZIS_GUNPOS_Z 50

#define REVOLVER_GUNPOS_X 0
#define REVOLVER_GUNPOS_Y 215
#define REVOLVER_GUNPOS_Z 65

void SetGunFlash_Left(int weapon_type)
{
    int pos[3];
    pos[0] = 0; // X
    pos[1] = 0; // Y
    pos[2] = 0; // Z

    switch (weapon_type)
    {
        case LG_PISTOLS: // PISTOLS
            pos[0] = PISTOLS_GUNPOS_X;
            pos[1] = PISTOLS_GUNPOS_Y;
            pos[2] = PISTOLS_GUNPOS_Z;
            break;
        case LG_REVOLVER: // REVOLVER
            pos[0] = REVOLVER_GUNPOS_X;
            pos[1] = REVOLVER_GUNPOS_Y;
            pos[2] = REVOLVER_GUNPOS_Z;
            break;
        case LG_UZIS: // UZI
            pos[0] = UZIS_GUNPOS_X;
            pos[1] = UZIS_GUNPOS_Y;
            pos[2] = UZIS_GUNPOS_Z;
            break;
        case LG_SHOTGUN: // SHOTGUN
        case LG_GRENADEGUN:
        case LG_CROSSBOW:
        case LG_FLARE:
            return;
    }

    SetupGunFlash(pos);
}

void SetGunFlash_Right(int weapon_type)
{
    int pos[3];
    pos[0] = 0; // X
    pos[1] = 0; // Y
    pos[2] = 0; // Z

    switch (weapon_type)
    {
        case LG_PISTOLS: // PISTOLS
            pos[0] = -PISTOLS_GUNPOS_X;
            pos[1] = PISTOLS_GUNPOS_Y;
            pos[2] = PISTOLS_GUNPOS_Z;
            break;
        case LG_REVOLVER: // REVOLVER
            pos[0] = -REVOLVER_GUNPOS_X;
            pos[1] = REVOLVER_GUNPOS_Y;
            pos[2] = REVOLVER_GUNPOS_Z;
            break;
        case LG_UZIS: // UZI
            pos[0] = -UZIS_GUNPOS_X;
            pos[1] = UZIS_GUNPOS_Y;
            pos[2] = UZIS_GUNPOS_Z;
            break;
        case LG_SHOTGUN: // SHOTGUN
        case LG_GRENADEGUN:
        case LG_CROSSBOW:
        case LG_FLARE:
            return;
    }

    SetupGunFlash(pos);
}

ITEM_INFO* FoundTarget(short itemNumber, ITEM_INFO* src, CREATURE_INFO* creature, short objectToTarget)
{
    ITEM_INFO* target;
    CREATURE_INFO* cinfo;
    int dist, bestdist;
    int x, z;
    int i;

    bestdist = 0x7FFFFFFF;
    cinfo = baddie_slots;
    for (i = 0; i < NUM_SLOTS; i++, cinfo++)
    {
        if (cinfo->item_number != NO_ITEM && cinfo->item_number != itemNumber)
        {
            target = &items[cinfo->item_number];
            if (target->status == FITEM_ACTIVE && target->object_number == objectToTarget)
            {
                x = target->pos.x - src->pos.x;
                z = target->pos.z - src->pos.z;
                dist = SQUARE(x) + SQUARE(z);
                if (dist < bestdist)
                {
                    bestdist = dist;
                    return target;
                }
            }
        }
    }

    // target is not found ?
    // so select lara !
    target = lara_item;
    return target;
}

short initHealth(short objNumber)
{
#ifndef DEBUG_MODE
    switch (objNumber)
    {
        case LARA:
            return 1000;
        case SKELETON:
            return 15;
        case VON_CROY:
            return 15;
        case GUIDE:
            return DONT_TARGET;
        case BADDY_1:
            return 25;
        case BADDY_2:
            return 35;
        case SCORPION:
            return 80;
        case SMALL_SCORPION:
            return 8;
        case MUMMY:
            return 15;
        case KNIGHTS_TEMPLAR:
            return 15;
        case SPHINX:
            return 1000;
        case SETHA:
            return 500;
        case LARA_DOUBLE:
            return 1000;
        case HORSEMAN:
            return 25;
        case HAMMERHEAD:
            return 45;
        case CROCODILE:
            return 36;
        case DEMIGOD1:
            return 200;
        case DEMIGOD2:
            return 200;
        case DEMIGOD3:
            return 200;
        case MUTANT:
            return 15;
        case TROOPS:
            return 40;
        case SAS:
            return 40;
        case HARPY:
            return 60;
        case WILD_BOAR:
            return 40;
        case DOG:
            return 16;
        case AHMET:
            return 80;
        case BABOON_NORMAL:
        case BABOON_INV:
        case BABOON_SILENT:
            return 30;
        case ENEMY_JEEP:
            return 40;
        case BAT:
            return 5;
        case BIG_BEETLE:
            return 30;
        case SENTRY_GUN:
            return 30;
        default: // if not found !
            return 1;
    }
#else
    if (objNumber == LARA)
        return 1000;
    if (objNumber == GUIDE)
        return DONT_TARGET;

    return 1; // only 1 hp for entity in debug mode ! (immortal entity ignored !)
#endif
}

void DrawFlashWithSmoke(ITEM_INFO* item, BITE_INFO* bite)
{
    if (item->fired_weapon)
    {
        PHD_VECTOR pos;
        pos.x = bite->x;
        pos.y = bite->y;
        pos.z = bite->z;
        GetJointAbsPosition(item, &pos, bite->mesh);

        TriggerDynamic(pos.x, pos.y, pos.z, 12, 24, 16, 4);
        // TODO: (for later: TriggerGunSmoke())
    }
}

ENTITY_JUMP CheckJumpPossibility(ITEM_INFO* item, CREATURE_INFO* creature)
{
    ENTITY_JUMP jump;
    FLOOR_INFO* floor;
    int height = 0, height1, height2, height3;
    int x, y, z;
    int cos, sin;
    short roomNumber;

    x = item->pos.x;
    y = item->pos.y;
    z = item->pos.z;
    sin = 942 * SIN(item->pos.y_rot) >> W2V_SHIFT;
    cos = 942 * COS(item->pos.y_rot) >> W2V_SHIFT;

    x += sin;
    z += cos;

    roomNumber = item->room_number;
    floor = GetFloor(x, y, z, &roomNumber);
    height1 = GetHeight(floor, x, y, z);

    x += sin;
    z += cos;

    roomNumber = item->room_number;
    floor = GetFloor(x, y, z, &roomNumber);
    height2 = GetHeight(floor, x, y, z);

    x += sin;
    z += cos;

    roomNumber = item->room_number;
    floor = GetFloor(x, y, z, &roomNumber);
    height3 = GetHeight(floor, x, y, z);

    jump.can_jump_1click = true;
    if (creature->enemy && item->box_number == creature->enemy->box_number
    ||  y >= height1 - STEP_L
    ||  y >= height2 + STEP_L
    ||  y <= height2 - STEP_L)
    {
        height = height2;
        jump.can_jump_1click = false;
    }

    jump.can_jump_2click = true;
    if (creature->enemy && item->box_number == creature->enemy->box_number
    ||  y >= height1 - STEPUP_HEIGHT
    ||  y >= height - STEPUP_HEIGHT
    ||  y >= height3 + STEP_L
    ||  y <= height3 - STEP_L)
    {
        jump.can_jump_2click = false;
    }

    return jump;
}

void phd_SwapPushMatrix(int frac)
{
    if (frac) // interpolate
        phd_PushMatrix_I();
    else
        phd_PushMatrix();
}

void phd_SwapPopMatrix(int frac)
{
    if (frac)
        phd_PopMatrix_I();
    else
        phd_PopMatrix();
}

void phd_SwapTranslateRel(int frac, int bone1, int bone2, int bone3, short* frame1, short* frame2, bool start)
{
    if (frac)
    {
        if (start)
            phd_TranslateRel_ID((int)*(frame1 + 6), (int)*(frame1 + 7), (int)*(frame1 + 8), (int)*(frame2 + 6), (int)*(frame2 + 7), (int)*(frame2 + 8));
        else
            phd_TranslateRel_I(bone1, bone2, bone3);
    }
    else
    {
        if (start)
            phd_TranslateRel((int)*(frame1 + 6), (int)*(frame1 + 7), (int)*(frame1 + 8));
        else
            phd_TranslateRel(bone1, bone2, bone3);
    }
}

void phd_SwapGarYXZsuperpack(int frac, short** rotation1, short** rotation2)
{
    if (frac)
        gar_RotYXZsuperpack_I(rotation1, rotation2, 0);
    else
        gar_RotYXZsuperpack(rotation1, 0);
}

void phd_SwapRotY(int frac, short y)
{
    if (frac)
        phd_RotY_I(y);
    else
        phd_RotY(y);
}

void phd_SwapRotX(int frac, short x)
{
    if (frac)
        phd_RotX_I(x);
    else
        phd_RotX(x);
}

void phd_SwapRotZ(int frac, short z)
{
    if (frac)
        phd_RotZ_I(z);
    else
        phd_RotZ(z);
}

void phd_SwapRotYXZ(int frac, short y, short x, short z)
{
    if (frac)
        phd_RotYXZ_I(y, x, z);
    else
        phd_RotYXZ(y, x, z);
}

void phd_SwapPutPolygons(int frac, int clip, short** mesh)
{
    if (frac)
        phd_PutPolygons_I(*mesh, clip);
    else
        phd_PutPolygons(*mesh, clip);
}
