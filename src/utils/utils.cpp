#include "framework.h"
#include "utils.h"
#include "3d_gen.h"
#include "3d_gen_a.h"
#include "box.h"
#include "control.h"
#include "draw.h"
#include "effect2.h"
#include "items.h"
#include "lara.h"
#include "lot.h"
#include "oldobjects.h"
#include "sphere.h"
#include "json/reader.h"
#include "drawprimitive.h"

ofstream flog;
LPCSTR lpWeaponName[12] = {
    "UNARMED",
    "PISTOLS",
    "REVOLVER",
    "UZIS",
    "SHOTGUN",
    "GRENADEGUN",
    "CROSSBOW",
    "FLARE",
    "TORCH",
    "TORCH_FIRE",
    "JEEP",
    "SIDECAR",
};

void dummy_func(ITEM_INFO* item, COLL_INFO* coll)
{
}

bool isFolderExists(LPCSTR folderName)
{
    struct stat st;
    int ret = stat(folderName, &st);
    return (ret == 0) && (st.st_mode & S_IFDIR);
}

bool isFileExists(LPCSTR fileName)
{
    struct stat st;
    return (stat(fileName, &st) == FALSE);
}

void createFolders(LPCSTR folderName)
{
    _mkdir(folderName);
}

static void S_LogStart(LPCSTR fileName)
{
    flog.open(fileName, ofstream::out | ofstream::app);
}

static void S_LogEnd()
{
    flog.close();
}

void S_Unimplemented(LPCSTR LT_Function_Flags)
{
    // if folder not exist !
    if (!isFolderExists(LOG_FOLDER))
        createFolders(LOG_FOLDER);

    S_LogStart(LOG_PATH);
    if (flog.is_open())
    {
        flog << "DLL[" << LT_Function_Flags << "] Unimplemented !" << "\n";
    }
    S_LogEnd();
}

void S_LogValue(LPCSTR content, ...)
{
#ifdef LOG_DEBUG
    char buffer[256];
    va_list args;

    // if folder not exist !
    if (!isFolderExists(LOG_FOLDER))
        createFolders(LOG_FOLDER);

    S_LogStart(LOG_PATH);
    if (flog.is_open())
    {
        va_start(args, content);
        _vsnprintf(buffer, sizeof(buffer), content, args);
        flog << "DLL[" << LT_Info << "] - " << buffer << "\n";
        va_end(args);
    }
    S_LogEnd();
#endif
}

void S_Log(LPCSTR content, ...)
{
#ifdef LOG_DEBUG
    char buffer[256];
    va_list args;

    // if folder not exist !
    if (!isFolderExists(LOG_FOLDER))
        createFolders(LOG_FOLDER);

    S_LogStart(LOG_PATH);
    if (flog.is_open())
    {
        va_start(args, content);
        _vsnprintf(buffer, sizeof(buffer), content, args);
        flog << "DLL[" << LT_Info << "] - " << buffer << "\n";
        va_end(args);
    }
    S_LogEnd();
#endif
}

void S_Log(LPCSTR content, bool isEntered, ...)
{
#ifdef LOG_DEBUG
    char buffer[256];
    va_list args;

    // if folder not exist !
    if (!isFolderExists(LOG_FOLDER))
        createFolders(LOG_FOLDER);

    S_LogStart(LOG_PATH);
    if (flog.is_open())
    {
        va_start(args, content);
        _vsnprintf(buffer, sizeof(buffer), content, args);
        if (isEntered)
            flog << "DLL[" << LT_Info << "] |    " << buffer << "\n";
        else
            flog << "DLL[" << LT_Info << "] - " << buffer << "\n";
        va_end(args);
    }
    S_LogEnd();
#endif
}

void S_Log(LPCSTR LT_flags, LPCSTR content, bool isEntered, ...)
{
#ifdef LOG_DEBUG
    char buffer[256];
    va_list args;

    // if folder not exist !
    if (!isFolderExists(LOG_FOLDER))
        createFolders(LOG_FOLDER);

    S_LogStart(LOG_PATH);
    if (flog.is_open())
    {
        va_start(args, content);
        _vsnprintf(buffer, sizeof(buffer), content, args);
        if (LT_flags != NULL)
        {
            if (isEntered)
                flog << "DLL[" << LT_flags << "] |    " << buffer << "\n";
            else
                flog << "DLL[" << LT_flags << "] - " << buffer << "\n";
        }
        else
        {
            if (isEntered)
                flog << "DLL[" << LT_Info << "] |   " << buffer << "\n";
            else
                flog << "DLL[" << LT_Info << "] - " << buffer << "\n";
        }
        va_end(args);
    }
    S_LogEnd();
#endif
}

LPCSTR WriteWorldItemAngle(ITEM_INFO* item)
{
    SHORT angle = (USHORT)(item->pos.y_rot + 0x2000) / 0x4000;
    switch (angle)
    {
        case NORTH:
            return "NORTH";
        case EAST:
            return "EAST";
        case SOUTH:
            return "SOUTH";
        case WEST:
            return "WEST";
        default:
            return "NOT_FOUND";
    }
}

void SetAnimationForItemAS(ITEM_INFO* item, int animation, int state)
{
    item->current_anim = animation;
    item->current_frame = anims[item->current_anim].frame_base;
    item->state_current = state;
    item->state_next = state;
}

void SetAnimationForItemASN(ITEM_INFO* item, int animation, int state, bool needStateNext)
{
    item->current_anim = animation;
    item->current_frame = anims[item->current_anim].frame_base;
    item->state_current = state;
    if (needStateNext)
        item->state_next = state;
}

void SetAnimationForItemASF(ITEM_INFO* item, int animation, int state, int frameNow)
{
    item->current_anim = animation;
    item->current_frame = anims[item->current_anim].frame_base + frameNow;
    item->state_current = state;
    item->state_next = state;
}

void SetAnimationForItemASS(ITEM_INFO* item, int animation, int state_current, int state_next)
{
    item->current_anim = animation;
    item->current_frame = anims[item->current_anim].frame_base;
    item->state_current = state_current;
    item->state_next = state_next;
}

void SetAnimationForItemASSF(ITEM_INFO* item, int animation, int state_current, int state_next, int frameNow)
{
    item->current_anim = animation;
    item->current_frame = anims[item->current_anim].frame_base + frameNow;
    item->state_current = state_current;
    item->state_next = state_next;
}

short GetCurrentFrame(ITEM_INFO* item)
{
    return (item->current_frame - anims[item->current_anim].frame_base);
}

void TriggerDynamicSwap(int x, int y, int z, int intensity, BYTE red, BYTE green, BYTE blue)
{
    if (CHK_ANY(gf_level_flags, SLEV_MIRROR) && lara_item->room_number == short(gf_mirror_room))
        TriggerDynamic_Mirror(x, y, z, intensity, red, green, blue);
    else
        TriggerDynamic(x, y, z, intensity, red, green, blue);
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
        pickup->pos.y -= GetBoundsAccurate(pickup)[3]; // maxY
        
        if (pickup->object_number == BIGMEDI_ITEM) // bigmedi item is not oriented correctly.
            pickup->pos.y_rot = item->pos.y_rot + 0x8000;
        else
            pickup->pos.y_rot = item->pos.y_rot;

        ItemNewRoom(pickup_number, item->room_number);
        pickup_number = pickup->carried_item;
    }
    while (pickup_number != NO_ITEM);
}

/*
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
*/

PHD_VECTOR GetGunFlashPosition(int weapon_type, bool right)
{
    PHD_VECTOR pos;
    
    if (!right)
    {
        switch (weapon_type)
        {
            case LG_PISTOLS:
                pos.x = PISTOLS_GUNPOS_X;
                pos.y = PISTOLS_GUNPOS_Y;
                pos.z = PISTOLS_GUNPOS_Z;
                GetLaraHandAbsPosition(&pos, HAND_L);
                break;
            case LG_UZIS:
                pos.x = UZIS_GUNPOS_X;
                pos.y = UZIS_GUNPOS_Y;
                pos.z = UZIS_GUNPOS_Z;
                GetLaraHandAbsPosition(&pos, HAND_L);
                break;
        }
    }
    else
    {
        switch (weapon_type)
        {
            case LG_PISTOLS:
                pos.x = -PISTOLS_GUNPOS_X;
                pos.y = PISTOLS_GUNPOS_Y;
                pos.z = PISTOLS_GUNPOS_Z;
                GetLaraHandAbsPosition(&pos, HAND_R);
                break;
            case LG_UZIS:
                pos.x = -UZIS_GUNPOS_X;
                pos.y = UZIS_GUNPOS_Y;
                pos.z = UZIS_GUNPOS_Z;
                GetLaraHandAbsPosition(&pos, HAND_R);
                break;
        }
    }

    return pos;
}

void set_gun_smoke_left(int weapon_type)
{
    if (lara_item->mesh_bits && SmokeCountL)
    {
        PHD_VECTOR pos;

        switch (SmokeWeapon)
        {
            case LG_PISTOLS:
                pos.x = PISTOLS_GUNPOS_X;
                pos.y = PISTOLS_GUNPOS_Y;
                pos.z = PISTOLS_GUNPOS_Z;
                break;
            case LG_UZIS:
                pos.x = UZIS_GUNPOS_X;
                pos.y = UZIS_GUNPOS_Y;
                pos.z = UZIS_GUNPOS_Z;
                break;
            default:
                pos.x = 0;
                pos.y = 0;
                pos.z = 0;
                break;
        }

        GetLaraHandAbsPosition(&pos, HAND_L);
        TriggerGunSmoke(pos.x, pos.y, pos.z, 0, 0, 0, 0, SmokeWeapon, SmokeCountL);
    }
}

void set_gun_smoke_right(int weapon_type)
{
    if (lara_item->mesh_bits && SmokeCountR)
    {
        PHD_VECTOR pos;

        switch (SmokeWeapon)
        {
            case LG_PISTOLS:
                pos.x = -PISTOLS_GUNPOS_X;
                pos.y = PISTOLS_GUNPOS_Y;
                pos.z = PISTOLS_GUNPOS_Z;
                break;
            case LG_UZIS:
                pos.x = -UZIS_GUNPOS_X;
                pos.y = UZIS_GUNPOS_Y;
                pos.z = UZIS_GUNPOS_Z;
                break;
            case LG_REVOLVER:
                pos.x = REVOLVER_GUNPOS_X;
                pos.y = REVOLVER_GUNPOS_Y;
                pos.z = REVOLVER_GUNPOS_Z;
                break;
            default:
                pos.x = 0;
                pos.y = 0;
                pos.z = 0;
                break;
        }

        GetLaraHandAbsPosition(&pos, HAND_R);
        TriggerGunSmoke(pos.x, pos.y, pos.z, 0, 0, 0, 0, SmokeWeapon, SmokeCountR);
    }
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

ENTITY_JUMP CheckMegaJumpPossibility(ITEM_INFO* item, CREATURE_INFO* creature)
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

bool CheckRollPossibility(ITEM_INFO* item)
{
    FLOOR_INFO* floor;
    int height, height2;
    int x, y, z;
    int cos, sin;
    short roomNumber;

    // first height
    sin = ((942 * SIN(item->pos.y_rot - 8192)) >> W2V_SHIFT);
    cos = ((942 * COS(item->pos.y_rot - 8192)) >> W2V_SHIFT);
    x = item->pos.x + sin;
    y = item->pos.y;
    z = item->pos.z + cos;
    roomNumber = item->room_number;
    floor = GetFloor(x, y, z, &roomNumber);
    height = GetHeight(floor, x, y, z);

    // second height
    sin = ((942 * SIN(item->pos.y_rot - 14336)) >> W2V_SHIFT);
    cos = ((942 * COS(item->pos.y_rot - 14336)) >> W2V_SHIFT);
    x = item->pos.x + sin;
    y = item->pos.y;
    z = item->pos.z + cos;
    roomNumber = item->room_number;
    floor = GetFloor(x, y, z, &roomNumber);
    height2 = GetHeight(floor, x, y, z) - y;

    if (abs(height2) > STEP_L || ((height + (STEP_L * 2)) >= y))
        return false;

    return true;
}

bool CheckJumpPossibility(ITEM_INFO* item)
{
    FLOOR_INFO* floor;
    int height, height2;
    int x, y, z;
    int cos, sin;
    short roomNumber;

    // first height
    sin = ((942 * SIN(item->pos.y_rot + 8192)) >> W2V_SHIFT);
    cos = ((942 * COS(item->pos.y_rot + 8192)) >> W2V_SHIFT);
    x = item->pos.x + sin;
    y = item->pos.y;
    z = item->pos.z + cos;
    roomNumber = item->room_number;
    floor = GetFloor(x, y, z, &roomNumber);
    height = GetHeight(floor, x, y, z);

    // second height
    sin = ((942 * SIN(item->pos.y_rot + 14336)) >> W2V_SHIFT);
    cos = ((942 * COS(item->pos.y_rot + 14336)) >> W2V_SHIFT);
    x = item->pos.x + sin;
    y = item->pos.y;
    z = item->pos.z + cos;
    roomNumber = item->room_number;
    floor = GetFloor(x, y, z, &roomNumber);
    height2 = GetHeight(floor, x, y, z) - y;

    if (abs(height2) <= STEP_L || ((height + (STEP_L * 2)) < y))
        return false;

    return true;
}

OBJECT_FOUND FoundItem(ITEM_INFO* src, CREATURE_INFO* creature, short primaryID, short secondID)
{
    ITEM_INFO* target;
    OBJECT_FOUND obj;
    int i;

    target = &items[0];
    for (i = 0; i < level_items; i++, target++)
    {
        if (target == nullptr)
            break;

        // check if the targeted item is less distant than lara...
        // now the entity will attack lara than pickup all item in the level. (maybe he will pickup the item if the distance is too long)
        if (!creature->hurt_by_lara && !src->ocb_bits) // check if the entity as no ocb (stop crash (but the baddy will not pickup a item at 100%))
        {
            if ((target->object_number == primaryID || target->object_number == secondID) && CHK_NOP(target->flags, IFLAG_KILLED_ITEM))
            {
                obj.item_number = i;
                obj.target = target;
                return obj;
            }
        }
    }

    // default target
    obj.item_number = NO_ITEM;
    obj.target = lara_item;
    return obj;
}

OBJECT_FOUND FoundEntityWithOCB(ITEM_INFO* item, short slotID, short ocb)
{
    ITEM_INFO* target;
    OBJECT_FOUND entity;

    target = &items[0];
    for (int i = 0; i < level_items; i++, target++)
    {
        // check if the entity are in a correct room and not dead already.
        // check if the ocb is not the same as the current src to not get the wrong ocb.
        if (item->ocb_bits != target->ocb_bits && target->object_number == slotID && target->ocb_bits == ocb && target->room_number != NO_ROOM && CHK_NOP(target->flags, IFLAG_KILLED_ITEM))
        {
            entity.item_number = i;
            entity.target = target;
            return entity;
        }
    }

    entity.item_number = NO_ITEM;
    entity.target = nullptr;
    return entity;
}

bool AlignItemToTarget(ITEM_INFO* src, ITEM_INFO* target)
{
    if (src->pos.x != target->pos.x)
        src->pos.x = target->pos.x;
    if (src->pos.y != target->pos.y)
        src->pos.y = target->pos.y;
    if (src->pos.z != target->pos.z)
        src->pos.z = target->pos.z;
    
    if (src->pos.x == target->pos.x
    &&  src->pos.y == target->pos.y
    &&  src->pos.z == target->pos.z)
        return true;
    else
        return false;
}

void ActivateEntity(short itemNumber)
{
    ITEM_INFO* item = &items[itemNumber];

    if (item->status == FITEM_INVISIBLE)
    {
        ///item->touch_bits = 0;
        if (EnableBaddieAI(itemNumber, FALSE))
            item->status = FITEM_ACTIVE;
        AddActiveItem(itemNumber);
    }
}

bool FoundEntityAndActivate(ITEM_INFO* item, short slotid, short ocb)
{
    OBJECT_FOUND baddy_spawn = FoundEntityWithOCB(item, slotid, ocb);
    if (baddy_spawn.item_number != NO_ITEM && baddy_spawn.target != nullptr)
    {
        if (baddy_spawn.target->status == FITEM_INVISIBLE)
        {
            ActivateEntity(baddy_spawn.item_number);
            return true;
        }
    }

    return false;
}

void Spawner(ITEM_INFO* item)
{
    short real_ocb = item->ocb_bits;

    // delete the other ocb
    if (item->ocb_bits & (1 | 2 | 3 | 4))
        real_ocb &= ~(1 | 2 | 3 | 4);

    for (int i = 1000; i < MAX_SPAWNER_ENTITY; i += 1000) // you can have 50 entity max with it !
    {
        if (real_ocb == i && FoundEntityAndActivate(item, BADDY_1, i + 1000))
            break; // not activate the other ocb !
    }
}

int CalculateLaraDistance(ITEM_INFO* item)
{
    int x, y, z, distance;
    x = item->pos.x - lara_item->pos.x;
    y = item->pos.y - lara_item->pos.y;
    z = item->pos.z - lara_item->pos.z;
    distance = SQUARE(x) + SQUARE(y) + SQUARE(z);
    return distance;
}

int CalculateItemDistanceToTarget(ITEM_INFO* src, ITEM_INFO* target)
{
    int x, y, z, distance;
    x = src->pos.x - target->pos.x;
    y = src->pos.y - target->pos.y;
    z = src->pos.z - target->pos.z;
    distance = SQUARE(x) + SQUARE(y) + SQUARE(z);
    return distance;
}

short* classic_meshes(short objNumber, short meshID)
{
    return meshes[objects[objNumber].mesh_index + meshID * 2];
}

void TestTriggersCollision(ITEM_INFO* item, COLL_INFO* coll)
{
    FLOOR_INFO* floor;
    short roomNumber;

    roomNumber = item->room_number;
    floor = GetFloor(item->pos.x, item->pos.y, item->pos.z, &roomNumber);
    GetHeight(floor, item->pos.x, item->pos.y, item->pos.z);
    coll->trigger = trigger_index;
}

short GetCatchAngle(ITEM_INFO * item, short angleToCheck)
{
    short angle = item->pos.y_rot;
    if (angle >= 0 - angleToCheck && angle <= 0 + angleToCheck)
        return 0;
    else if (angle >= 0x4000 - angleToCheck && angle <= 0x4000 + angleToCheck)
        return 0x4000;
    else if (angle >= 0x8000 - angleToCheck || angle <= -0x8000 + angleToCheck)
        return -0x8000;
    else if (angle >= -0x4000 - angleToCheck && angle <= -0x4000 + angleToCheck)
        return -0x4000;
    else
        return 0;
}

void LaraSlideAngle(ITEM_INFO* item, COLL_INFO* coll, short adif, short angle)
{
    // orient lara when slidding !
    // more realistic because the foot will touch the floor like this !
    // - maybe using animation instead of that would be great for customization ?
    S_LogValue("adif: %d, coll->tilt_x: %d, coll->tilt_z: %d, angle: %d, world_angle: %s, item->pos.x_rot: %d, lara.move_angle: %d, item->pos.y_rot: %d", adif, coll->tilt_x, coll->tilt_z, angle, WriteWorldItemAngle(item), item->pos.x_rot, lara.move_angle, item->pos.y_rot);

    switch (lara.move_angle)
    {
        case 0:       // NORTH

            break;
        case 0x4000:  // EAST
            switch (item->pos.y_rot)
            {
            case 0x4000:
                if (coll->tilt_x == -3)
                    item->pos.x_rot = -SLIDE_SLOPE3;
                else if (coll->tilt_x == -4)
                    item->pos.x_rot = -SLIDE_SLOPE4;
                break;
            case -0x4000:
                if (coll->tilt_x == -3)
                    item->pos.x_rot = SLIDE_SLOPE3_INV;
                else if (coll->tilt_x == -4)
                    item->pos.x_rot = SLIDE_SLOPE4_INV;
                break;
            }
            break;
        case -0x4000: // WEST

            break;
        case -0x8000: // SOUTH

            break;
    }
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

void phd_SwapTranslateRel(int frac, short* frame1, short* frame2)
{
    if (frac)
        phd_TranslateRel_ID((int)*(frame1 + 6), (int)*(frame1 + 7), (int)*(frame1 + 8), (int)*(frame2 + 6), (int)*(frame2 + 7), (int)*(frame2 + 8));
    else
        phd_TranslateRel((int)*(frame1 + 6), (int)*(frame1 + 7), (int)*(frame1 + 8));
}

void phd_SwapTranslateRel(int frac, BONE bone)
{
    if (frac)
        phd_TranslateRel_I(bone.bone1, bone.bone2, bone.bone3);
    else
        phd_TranslateRel(bone.bone1, bone.bone2, bone.bone3);
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
