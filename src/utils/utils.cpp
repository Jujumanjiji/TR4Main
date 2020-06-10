#include "framework.h"
#include "utils.h"
#include "3d_gen.h"
#include "3d_gen_a.h"
#include "box.h"
#include "control.h"
#include "draw.h"
#include "effect2.h"
#include "gameflow.h"
#include "items.h"
#include "inventory_2d.h"
#include "lara.h"
#include "lot.h"
#include "oldobjects.h"
#include "sphere.h"
#include "drawprimitive.h"
#include "gameflow_utils.h"
#include <dirent.h>

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

bool isFolderExists(std::string folderName)
{
    struct stat st;
    int ret = stat(folderName.c_str(), &st);
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
    SHORT angle = (USHORT)(item->pos.yRot + 0x2000) / 0x4000;
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
    item->animNumber = animation;
    item->frameNumber = Anims[item->animNumber].frameBase;
    item->currentAnimState = state;
    item->goalAnimState = state;
}

void SetAnimationForItemASN(ITEM_INFO* item, int animation, int state, bool needStateNext)
{
    item->animNumber = animation;
    item->frameNumber = Anims[item->animNumber].frameBase;
    item->currentAnimState = state;
    if (needStateNext)
        item->goalAnimState = state;
}

void SetAnimationForItemASF(ITEM_INFO* item, int animation, int state, int frameNow)
{
    item->animNumber = animation;
    item->frameNumber = Anims[item->animNumber].frameBase + frameNow;
    item->currentAnimState = state;
    item->goalAnimState = state;
}

void SetAnimationForItemASS(ITEM_INFO* item, int animation, int state_current, int state_next)
{
    item->animNumber = animation;
    item->frameNumber = Anims[item->animNumber].frameBase;
    item->currentAnimState = state_current;
    item->goalAnimState = state_next;
}

void SetAnimationForItemASSF(ITEM_INFO* item, int animation, int state_current, int state_next, int frameNow)
{
    item->animNumber = animation;
    item->frameNumber = Anims[item->animNumber].frameBase + frameNow;
    item->currentAnimState = state_current;
    item->goalAnimState = state_next;
}

short GetCurrentFrame(ITEM_INFO* item)
{
    return (item->frameNumber - Anims[item->animNumber].frameBase);
}

void TriggerDynamicSwap(int x, int y, int z, int intensity, BYTE red, BYTE green, BYTE blue)
{
    if (CHK_ANY(gfLevelFlags, SLEV_MIRROR) && LaraItem->roomNumber == short(gfMirrorRoom))
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
    switch (item->objectNumber)
    {
        case BADDY_1: 
        case BADDY_2:
            GetJointAbsPosition(item, &pos, 6);
            break;

        default:
            pos.x = item->pos.xPos;
            pos.y = item->pos.yPos;
            pos.z = item->pos.zPos;
            break;
    }

    pickup_number = item->carriedItem;
    do
    {
        if (pickup_number == NO_ITEM)
            break;

        pickup = &Items[pickup_number];
        floor = GetFloor(pos.x, pos.y, pos.z, &item->roomNumber);

        pickup->pos.xPos = pos.x;
        pickup->pos.zPos = pos.z;
        pickup->pos.yPos = GetFloorHeight(floor, pickup->pos.xPos, item->pos.yPos, pickup->pos.zPos);
        pickup->pos.yPos -= GetBoundsAccurate(pickup)[3]; // maxY
        
        if (pickup->objectNumber == BIGMEDI_ITEM) // bigmedi item is not oriented correctly.
            pickup->pos.yRot = item->pos.yRot + 0x8000;
        else
            pickup->pos.yRot = item->pos.yRot;

        ItemNewRoom(pickup_number, item->roomNumber);
        pickup_number = pickup->carriedItem;
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
    PHD_VECTOR pos = { 0, 0, 0 };
    
    if (!right)
    {
        switch (weapon_type)
        {
            case LG_PISTOLS:
                pos.x = PISTOLS_GUNPOS_X;
                pos.y = PISTOLS_GUNPOS_Y;
                pos.z = PISTOLS_GUNPOS_Z;
                GetLaraJointAbsPosition(&pos, JHAND_L);
                break;
            case LG_UZIS:
                pos.x = UZIS_GUNPOS_X;
                pos.y = UZIS_GUNPOS_Y;
                pos.z = UZIS_GUNPOS_Z;
                GetLaraJointAbsPosition(&pos, JHAND_L);
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
                GetLaraJointAbsPosition(&pos, JHAND_R);
                break;
            case LG_UZIS:
                pos.x = -UZIS_GUNPOS_X;
                pos.y = UZIS_GUNPOS_Y;
                pos.z = UZIS_GUNPOS_Z;
                GetLaraJointAbsPosition(&pos, JHAND_R);
                break;
        }
    }

    return pos;
}

void set_gun_smoke_left(int weapon_type)
{
    if (LaraItem->meshBits && SmokeCountL)
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

        GetLaraJointAbsPosition(&pos, JHAND_L);
        TriggerGunSmoke(pos.x, pos.y, pos.z, 0, 0, 0, 0, SmokeWeapon, SmokeCountL);
    }
}

void set_gun_smoke_right(int weapon_type)
{
    if (LaraItem->meshBits && SmokeCountR)
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

        GetLaraJointAbsPosition(&pos, JHAND_R);
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
    cinfo = BaddieSlots;
    for (i = 0; i < NUM_SLOTS; i++, cinfo++)
    {
        if (cinfo->itemNum != NO_ITEM && cinfo->itemNum != itemNumber)
        {
            target = &Items[cinfo->itemNum];
            if (target->status == FITEM_ACTIVE && target->objectNumber == objectToTarget)
            {
                x = target->pos.xPos - src->pos.xPos;
                z = target->pos.zPos - src->pos.zPos;
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
    target = LaraItem;
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
    if (item->firedWeapon)
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

    x = item->pos.xPos;
    y = item->pos.yPos;
    z = item->pos.zPos;
    sin = 942 * SIN(item->pos.yRot) >> W2V_SHIFT;
    cos = 942 * COS(item->pos.yRot) >> W2V_SHIFT;

    x += sin;
    z += cos;

    roomNumber = item->roomNumber;
    floor = GetFloor(x, y, z, &roomNumber);
    height1 = GetFloorHeight(floor, x, y, z);

    x += sin;
    z += cos;

    roomNumber = item->roomNumber;
    floor = GetFloor(x, y, z, &roomNumber);
    height2 = GetFloorHeight(floor, x, y, z);

    x += sin;
    z += cos;

    roomNumber = item->roomNumber;
    floor = GetFloor(x, y, z, &roomNumber);
    height3 = GetFloorHeight(floor, x, y, z);

    jump.can_jump_1click = true;
    if (creature->enemy && item->boxNumber == creature->enemy->boxNumber
    ||  y >= height1 - STEP_L
    ||  y >= height2 + STEP_L
    ||  y <= height2 - STEP_L)
    {
        height = height2;
        jump.can_jump_1click = false;
    }

    jump.can_jump_2click = true;
    if (creature->enemy && item->boxNumber == creature->enemy->boxNumber
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
    sin = ((942 * SIN(item->pos.yRot - 8192)) >> W2V_SHIFT);
    cos = ((942 * COS(item->pos.yRot - 8192)) >> W2V_SHIFT);
    x = item->pos.xPos + sin;
    y = item->pos.yPos;
    z = item->pos.zPos + cos;
    roomNumber = item->roomNumber;
    floor = GetFloor(x, y, z, &roomNumber);
    height = GetFloorHeight(floor, x, y, z);

    // second height
    sin = ((942 * SIN(item->pos.yRot - 14336)) >> W2V_SHIFT);
    cos = ((942 * COS(item->pos.yRot - 14336)) >> W2V_SHIFT);
    x = item->pos.xPos + sin;
    y = item->pos.yPos;
    z = item->pos.zPos + cos;
    roomNumber = item->roomNumber;
    floor = GetFloor(x, y, z, &roomNumber);
    height2 = GetFloorHeight(floor, x, y, z) - y;

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
    sin = ((942 * SIN(item->pos.yRot + 8192)) >> W2V_SHIFT);
    cos = ((942 * COS(item->pos.yRot + 8192)) >> W2V_SHIFT);
    x = item->pos.xPos + sin;
    y = item->pos.yPos;
    z = item->pos.zPos + cos;
    roomNumber = item->roomNumber;
    floor = GetFloor(x, y, z, &roomNumber);
    height = GetFloorHeight(floor, x, y, z);

    // second height
    sin = ((942 * SIN(item->pos.yRot + 14336)) >> W2V_SHIFT);
    cos = ((942 * COS(item->pos.yRot + 14336)) >> W2V_SHIFT);
    x = item->pos.xPos + sin;
    y = item->pos.yPos;
    z = item->pos.zPos + cos;
    roomNumber = item->roomNumber;
    floor = GetFloor(x, y, z, &roomNumber);
    height2 = GetFloorHeight(floor, x, y, z) - y;

    if (abs(height2) <= STEP_L || ((height + (STEP_L * 2)) < y))
        return false;

    return true;
}

AIOBJECT* FoundAIObject(short objectNumber)
{
    for (int i = 0; i < nAIObjects; i++)
    {
        AIOBJECT* ai = &AIObjects[i];
        if (ai->objectNumber == objectNumber && ai->roomNumber != NO_ROOM)
            return ai;
    }

    return nullptr;
}

AIOBJECT* FoundAIObjectWithOCB(short objectNumber, short triggerFlags)
{
    for (int i = 0; i < nAIObjects; i++)
    {
        AIOBJECT* ai = &AIObjects[i];
        if (ai->objectNumber == objectNumber && ai->triggerFlags == triggerFlags && ai->roomNumber != NO_ROOM)
            return ai;
    }

    return nullptr;
}

OBJECT_FOUND FoundItem(ITEM_INFO* src, short slotID)
{
    ITEM_INFO* target;
    OBJECT_FOUND obj;
    int i;

    target = &Items[0];
    for (i = 0; i < level_items; i++, target++)
    {
        if (target != nullptr)
        {
            if (target->objectNumber == slotID && target->roomNumber != NO_ROOM && CHK_NOP(target->flags, IFLAG_KILLED_ITEM))
            {
                obj.itemNumber = i;
                obj.target = target;
                return obj;
            }
        }
    }

    // default target
    obj.itemNumber = NO_ITEM;
    obj.target = nullptr;
    return obj;
}

OBJECT_FOUND* FoundItemWithOCB(ITEM_INFO* src, short primaryID, short ocb)
{
    ITEM_INFO* target;
    OBJECT_FOUND* obj = nullptr;
    ROOM_INFO* r;
    short target_number;

    GetEntityByRooms(src, SECTOR(7), NULL);
    for (int lp = 0; lp < draw_rooms_number; lp++)
    {
        r = &Rooms[draw_rooms[lp]];
        for (target_number = Rooms[draw_rooms[lp]].itemNumber; target_number != NO_ITEM;)
        {
            target = &Items[target_number];
            S_LogValue("Found Item: %d", target->objectNumber);
            target_number = target->nextItem;
        }
    }

    return nullptr;
}

OBJECT_FOUND FoundItemWithOCB(ITEM_INFO* src, short primaryID, short secondID, short triggerFlags)
{
    ITEM_INFO* target;
    OBJECT_FOUND obj;
    int i;

    target = &Items[0];
    for (i = 0; i < level_items; i++, target++)
    {
        if (target != nullptr)
        {
            if ((target->objectNumber == primaryID || target->objectNumber == secondID) && target->roomNumber != NO_ROOM && target->triggerFlags == triggerFlags && CHK_NOP(target->flags, IFLAG_KILLED_ITEM))
            {
                obj.itemNumber = i;
                obj.target = target;
                return obj;
            }
        }
    }

    // default target
    obj.itemNumber = NO_ITEM;
    obj.target = nullptr;
    return obj;
}

void TargetNearestEntity(ITEM_INFO* item, CREATURE_INFO* creature)
{
    ITEM_INFO* target;
    int bestdistance;
    int distance;
    int x, z;

    bestdistance = MAXINT;
    for (int i = 0; i < level_items; i++)
    {
        target = &Items[i];
        if (target != nullptr)
        {
            if (target != item && target->hitPoints > 0 && target->status != FITEM_INVISIBLE)
            {
                x = target->pos.xPos - item->pos.xPos;
                z = target->pos.zPos - item->pos.zPos;
                distance = SQUARE(z) + SQUARE(x);
                if (distance < bestdistance)
                {
                    creature->enemy = target;
                    bestdistance = distance;
                }
            }
        }
    }
}

ITEM_INFO* FoundNearestEntity(PHD_3DPOS* pos)
{
    ITEM_INFO* target;
    int bestdistance;
    int distance;
    int x, z;

    bestdistance = MAXINT;
    for (int i = 0; i < level_items; i++)
    {
        target = &Items[i];
        if (target != nullptr)
        {
            if (target->hitPoints > 0 && target->status != FITEM_INVISIBLE)
            {
                x = target->pos.xPos - pos->xPos;
                z = target->pos.zPos - pos->zPos;
                distance = SQUARE(z) + SQUARE(x);
                if (distance < bestdistance)
                {
                    bestdistance = distance;
                    return target;
                }
            }
        }
    }

    return nullptr;
}

OBJECT_FOUND FoundEntity(short slotID)
{
    ITEM_INFO* target;
    OBJECT_INFO* obj;
    OBJECT_FOUND entity;

    target = &Items[0];
    for (int i = 0; i < level_items; i++, target++)
    {
        if (target != nullptr)
        {
            // check if the entity are in a correct room and not dead already.
            // check if the ocb is not the same as the current src to not get the wrong ocb.
            if (target->objectNumber == slotID && target->roomNumber != NO_ROOM && CHK_NOP(target->flags, IFLAG_KILLED_ITEM))
            {
                obj = &Objects[target->objectNumber];
                if (!obj->intelligent || slotID != LARA)
                    break;
                entity.itemNumber = i;
                entity.target = target;
                return entity;
            }
        }
    }

    entity.itemNumber = NO_ITEM;
    entity.target = nullptr;
    return entity;
}

OBJECT_FOUND FoundEntityWithOCB(ITEM_INFO* item, short slotID, short ocb)
{
    ITEM_INFO* target;
    OBJECT_INFO* obj;
    OBJECT_FOUND entity;

    target = &Items[0];
    for (int i = 0; i < level_items; i++, target++)
    {
        // check if the entity are in a correct room and not dead already.
        // check if the ocb is not the same as the current src to not get the wrong ocb.
        if (item != target && target->objectNumber == slotID && target->triggerFlags == ocb && target->roomNumber != NO_ROOM && CHK_NOP(target->flags, IFLAG_KILLED_ITEM))
        {
            obj = &Objects[target->objectNumber];
            if (!obj->intelligent)
                break;
            entity.itemNumber = i;
            entity.target = target;
            return entity;
        }
    }

    entity.itemNumber = NO_ITEM;
    entity.target = nullptr;
    return entity;
}

static void GetNewRooms(int x, int y, int z, short roomNumber)
{
    GetFloor(x, y, z, &roomNumber);

    int i;
    for (i = 0; i < draw_rooms_number; i++)
    {
        if (draw_rooms[i] == roomNumber)
            break;
    }

    if (i == draw_rooms_number)
        draw_rooms[draw_rooms_number++] = roomNumber;
}

// stored at draw_rooms/draw_rooms_number
void GetEntityByRooms(ITEM_INFO* item, int radius, int height)
{
    GAME_VECTOR pos;
    pos.x = item->pos.xPos;
    pos.y = item->pos.yPos;
    pos.z = item->pos.zPos;
    pos.roomNumber = item->roomNumber;
    draw_rooms[0] = pos.roomNumber;
    draw_rooms_number = 1;
    GetNewRooms(pos.x + radius, pos.y, pos.z + radius, pos.roomNumber);
    GetNewRooms(pos.x - radius, pos.y, pos.z + radius, pos.roomNumber);
    GetNewRooms(pos.x + radius, pos.y, pos.z - radius, pos.roomNumber);
    GetNewRooms(pos.x - radius, pos.y, pos.z - radius, pos.roomNumber);
    GetNewRooms(pos.x + radius, pos.y - height, pos.z + radius, pos.roomNumber);
    GetNewRooms(pos.x - radius, pos.y - height, pos.z + radius, pos.roomNumber);
    GetNewRooms(pos.x + radius, pos.y - height, pos.z - radius, pos.roomNumber);
    GetNewRooms(pos.x - radius, pos.y - height, pos.z - radius, pos.roomNumber);
}

bool AlignItemToTarget(ITEM_INFO* src, ITEM_INFO* target)
{
    if (src->pos.xPos != target->pos.xPos)
        src->pos.xPos = target->pos.xPos;
    if (src->pos.yPos != target->pos.yPos)
        src->pos.yPos = target->pos.yPos;
    if (src->pos.zPos != target->pos.zPos)
        src->pos.zPos = target->pos.zPos;
    
    if (src->pos.xPos == target->pos.xPos
    &&  src->pos.yPos == target->pos.yPos
    &&  src->pos.zPos == target->pos.zPos)
        return true;
    else
        return false;
}

void MoveEntityToAnother(PHD_3DPOS* src, PHD_3DPOS* target, ANIM_FRAME* bounds, int speed)
{
    int x, y, z, dist;
    x = target->xPos - src->xPos;
    y = (target->yPos + (bounds->minY - bounds->maxY)) - src->yPos;
    z = target->zPos - src->zPos;
    dist = phd_sqrt(SQUARE(x) + SQUARE(y) + SQUARE(z));

    if (speed < dist)
    {
        src->xPos += (speed * x) / dist;
        src->yPos += (speed * y) / dist;
        src->zPos += (speed * z) / dist;
    }
    else
    {
        src->xPos = target->xPos;
        src->yPos = target->yPos;
        src->zPos = target->zPos;
    }
}

void ActivateEntity(short itemNumber)
{
    ITEM_INFO* item = &Items[itemNumber];

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
    if (baddy_spawn.itemNumber != NO_ITEM && baddy_spawn.target != nullptr)
    {
        if (baddy_spawn.target->status == FITEM_INVISIBLE)
        {
            ActivateEntity(baddy_spawn.itemNumber);
            return true;
        }
    }

    return false;
}

void Spawner(ITEM_INFO* item)
{
    short real_ocb = item->triggerFlags;

    // delete the other ocb
    if (item->triggerFlags & (1 | 2 | 3 | 4))
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
    x = item->pos.xPos - LaraItem->pos.xPos;
    y = item->pos.yPos - LaraItem->pos.yPos;
    z = item->pos.zPos - LaraItem->pos.zPos;
    distance = SQUARE(x) + SQUARE(y) + SQUARE(z);
    return distance;
}

int CalculateItemDistanceToTarget(ITEM_INFO* src, ITEM_INFO* target)
{
    int x, y, z, distance;
    x = src->pos.xPos - target->pos.xPos;
    y = src->pos.yPos - target->pos.yPos;
    z = src->pos.zPos - target->pos.zPos;
    distance = SQUARE(x) + SQUARE(y) + SQUARE(z);
    return distance;
}

void classic_meshes(short objNumber, short meshID, short* new_meshes)
{
    Meshes[Objects[objNumber].meshIndex + meshID * 2] = new_meshes;
}

short* classic_meshes(short objNumber, short meshID)
{
    return Meshes[Objects[objNumber].meshIndex + meshID * 2];
}



void TestTriggersCollision(ITEM_INFO* item, COLL_INFO* coll)
{
    FLOOR_INFO* floor;
    short roomNumber;

    roomNumber = item->roomNumber;
    floor = GetFloor(item->pos.xPos, item->pos.yPos, item->pos.zPos, &roomNumber);
    GetFloorHeight(floor, item->pos.xPos, item->pos.yPos, item->pos.zPos);
    coll->trigger = TriggerIndex;
}

short GetCatchAngle(ITEM_INFO * item, short angleToCheck)
{
    short angle = item->pos.yRot;
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
    S_LogValue("adif: %d, coll->tilt_x: %d, coll->tilt_z: %d, angle: %d, world_angle: %s, item->pos.x_rot: %d, lara.move_angle: %d, item->pos.y_rot: %d", adif, coll->tiltX, coll->tiltZ, angle, WriteWorldItemAngle(item), item->pos.xRot, Lara.moveAngle, item->pos.yRot);

    switch (Lara.moveAngle)
    {
        case 0:       // NORTH

            break;
        case 0x4000:  // EAST
            switch (item->pos.yRot)
            {
            case 0x4000:
                if (coll->tiltX == -3)
                    item->pos.xRot = -SLIDE_SLOPE3;
                else if (coll->tiltX == -4)
                    item->pos.xRot = -SLIDE_SLOPE4;
                break;
            case -0x4000:
                if (coll->tiltX == -3)
                    item->pos.xRot = SLIDE_SLOPE3_INV;
                else if (coll->tiltX == -4)
                    item->pos.xRot = SLIDE_SLOPE4_INV;
                break;
            }
            break;
        case -0x4000: // WEST

            break;
        case -0x8000: // SOUTH

            break;
    }
}

LPCSTR InventoryItemDebugString(short inv_item)
{
    switch (inv_item)
    {
    case INV_UZI:
        return "INV_UZI";
    case INV_PISTOLS:
        return "INV_PISTOLS";
    case INV_SHOTGUN:
        return "INV_SHOTGUN";
    case INV_REVOLVER:
        return "INV_REVOLVER";
    case INV_REVOLVER_LASER:
        return "INV_REVOLVER_LASER";
    case INV_CROSSBOW:
        return "INV_CROSSBOW";
    case INV_CROSSBOW_LASER:
        return "INV_CROSSBOW_LASER";
    case INV_GRENADEGUN:
        return "INV_GRENADEGUN";
    case INV_SHOTGUN_AMMO1:
        return "INV_SHOTGUN_AMMO1";
    case INV_SHOTGUN_AMMO2:
        return "INV_SHOTGUN_AMMO2";
    case INV_GRENADEGUN_AMMO1:
        return "INV_GRENADEGUN_AMMO1";
    case INV_GRENADEGUN_AMMO2:
        return "INV_GRENADEGUN_AMMO2";
    case INV_GRENADEGUN_AMMO3:
        return "INV_GRENADEGUN_AMMO3";
    case INV_CROSSBOW_AMMO1:
        return "INV_CROSSBOW_AMMO1";
    case INV_CROSSBOW_AMMO2:
        return "INV_CROSSBOW_AMMO2";
    case INV_CROSSBOW_AMMO3:
        return "INV_CROSSBOW_AMMO3";
    case INV_UZI_AMMO:
        return "INV_UZI_AMMO";
    case INV_PISTOLS_AMMO:
        return "INV_PISTOLS_AMMO";
    case INV_LASERSIGHT:
        return "INV_LASERSIGHT";
    case INV_BIGMEDI:
        return "INV_BIGMEDI";
    case INV_SMALLMEDI:
        return "INV_SMALLMEDI";
    case INV_BINOCULARS:
        return "INV_BINOCULARS";
    case INV_FLARE:
        return "INV_FLARE";
    case INV_COMPASS:
        return "INV_COMPASS";
    case INV_MEMCARD_LOAD:
        return "INV_MEMCARD_LOAD";
    case INV_MEMCARD_SAVE:
        return "INV_MEMCARD_SAVE";
    case INV_WATERSKIN_EMPTY:
        return "INV_WATERSKIN_EMPTY";
    case INV_WATERSKIN1_1:
        return "INV_WATERSKIN1_1";
    case INV_WATERSKIN1_2:
        return "INV_WATERSKIN1_2";
    case INV_WATERSKIN1_3:
        return "INV_WATERSKIN1_3";
    case INV_WATERSKIN2_EMPTY:
        return "INV_WATERSKIN2_EMPTY";
    case INV_WATERSKIN2_1:
        return "INV_WATERSKIN2_1";
    case INV_WATERSKIN2_2:
        return "INV_WATERSKIN2_2";
    case INV_WATERSKIN2_3:
        return "INV_WATERSKIN2_3";
    case INV_WATERSKIN2_4:
        return "INV_WATERSKIN2_4";
    case INV_WATERSKIN2_5:
        return "INV_WATERSKIN2_5";
    case INV_PUZZLE1:
        return "INV_PUZZLE1";
    case INV_PUZZLE2:
        return "INV_PUZZLE2";
    case INV_PUZZLE3:
        return "INV_PUZZLE3";
    case INV_PUZZLE4:
        return "INV_PUZZLE4";
    case INV_PUZZLE5:
        return "INV_PUZZLE5";
    case INV_PUZZLE6:
        return "INV_PUZZLE6";
    case INV_PUZZLE7:
        return "INV_PUZZLE7";
    case INV_PUZZLE8:
        return "INV_PUZZLE8";
    case INV_PUZZLE9:
        return "INV_PUZZLE9";
    case INV_PUZZLE10:
        return "INV_PUZZLE10";
    case INV_PUZZLE11:
        return "INV_PUZZLE11";
    case INV_PUZZLE12:
        return "INV_PUZZLE12";
    case INV_PUZZLE1_COMBO1:
        return "INV_PUZZLE1_COMBO1";
    case INV_PUZZLE1_COMBO2:
        return "INV_PUZZLE1_COMBO2";
    case INV_PUZZLE2_COMBO1:
        return "INV_PUZZLE2_COMBO1";
    case INV_PUZZLE2_COMBO2:
        return "INV_PUZZLE2_COMBO2";
    case INV_PUZZLE3_COMBO1:
        return "INV_PUZZLE3_COMBO1";
    case INV_PUZZLE3_COMBO2:
        return "INV_PUZZLE3_COMBO2";
    case INV_PUZZLE4_COMBO1:
        return "INV_PUZZLE4_COMBO1";
    case INV_PUZZLE4_COMBO2:
        return "INV_PUZZLE4_COMBO2";
    case INV_PUZZLE5_COMBO1:
        return "INV_PUZZLE5_COMBO1";
    case INV_PUZZLE5_COMBO2:
        return "INV_PUZZLE5_COMBO2";
    case INV_PUZZLE6_COMBO1:
        return "INV_PUZZLE6_COMBO1";
    case INV_PUZZLE6_COMBO2:
        return "INV_PUZZLE6_COMBO2";
    case INV_PUZZLE7_COMBO1:
        return "INV_PUZZLE7_COMBO1";
    case INV_PUZZLE7_COMBO2:
        return "INV_PUZZLE7_COMBO2";
    case INV_PUZZLE8_COMBO1:
        return "INV_PUZZLE8_COMBO1";
    case INV_PUZZLE8_COMBO2:
        return "INV_PUZZLE8_COMBO2";
    case INV_KEY1:
        return "INV_KEY1";
    case INV_KEY2:
        return "INV_KEY2";
    case INV_KEY3:
        return "INV_KEY3";
    case INV_KEY4:
        return "INV_KEY4";
    case INV_KEY5:
        return "INV_KEY5";
    case INV_KEY6:
        return "INV_KEY6";
    case INV_KEY7:
        return "INV_KEY7";
    case INV_KEY8:
        return "INV_KEY8";
    case INV_KEY9:
        return "INV_KEY9";
    case INV_KEY10:
        return "INV_KEY10";
    case INV_KEY11:
        return "INV_KEY11";
    case INV_KEY12:
        return "INV_KEY12";
    case INV_KEY1_COMBO1:
        return "INV_KEY1_COMBO1";
    case INV_KEY1_COMBO2:
        return "INV_KEY1_COMBO2";
    case INV_KEY2_COMBO1:
        return "INV_KEY2_COMBO1";
    case INV_KEY2_COMBO2:
        return "INV_KEY2_COMBO2";
    case INV_KEY3_COMBO1:
        return "INV_KEY3_COMBO1";
    case INV_KEY3_COMBO2:
        return "INV_KEY3_COMBO2";
    case INV_KEY4_COMBO1:
        return "INV_KEY4_COMBO1";
    case INV_KEY4_COMBO2:
        return "INV_KEY4_COMBO2";
    case INV_KEY5_COMBO1:
        return "INV_KEY5_COMBO1";
    case INV_KEY5_COMBO2:
        return "INV_KEY5_COMBO2";
    case INV_KEY6_COMBO1:
        return "INV_KEY6_COMBO1";
    case INV_KEY6_COMBO2:
        return "INV_KEY6_COMBO2";
    case INV_KEY7_COMBO1:
        return "INV_KEY7_COMBO1";
    case INV_KEY7_COMBO2:
        return "INV_KEY7_COMBO2";
    case INV_KEY8_COMBO1:
        return "INV_KEY8_COMBO1";
    case INV_KEY8_COMBO2:
        return "INV_KEY8_COMBO2";
    case INV_PICKUP1:
        return "INV_PICKUP1";
    case INV_PICKUP2:
        return "INV_PICKUP2";
    case INV_PICKUP3:
        return "INV_PICKUP3";
    case INV_PICKUP4:
        return "INV_PICKUP4";
    case INV_PICKUP1_COMBO1:
        return "INV_PICKUP1_COMBO1";
    case INV_PICKUP1_COMBO2:
        return "INV_PICKUP1_COMBO2";
    case INV_PICKUP2_COMBO1:
        return "INV_PICKUP2_COMBO1";
    case INV_PICKUP2_COMBO2:
        return "INV_PICKUP2_COMBO2";
    case INV_PICKUP3_COMBO1:
        return "INV_PICKUP3_COMBO1";
    case INV_PICKUP3_COMBO2:
        return "INV_PICKUP3_COMBO2";
    case INV_PICKUP4_COMBO1:
        return "INV_PICKUP4_COMBO1";
    case INV_PICKUP4_COMBO2:
        return "INV_PICKUP4_COMBO2";
    case INV_QUEST1:
        return "INV_QUEST1";
    case INV_QUEST2:
        return "INV_QUEST2";
    case INV_QUEST3:
        return "INV_QUEST3";
    case INV_QUEST4:
        return "INV_QUEST4";
    case INV_QUEST5:
        return "INV_QUEST5";
    case INV_QUEST6:
        return "INV_QUEST6";
    case INV_BURNING_TORCH:
        return "INV_BURNING_TORCH";
    case INV_CROWBAR:
        return "INV_CROWBAR";
    case INV_CLOCKWORK_BEETLE:
        return "INV_CLOCKWORK_BEETLE";
    case INV_CLOCKWORK_BEETLE_COMBO2:
        return "INV_CLOCKWORK_BEETLE_COMBO2";
    case INV_CLOCKWORK_BEETLE_COMBO1:
        return "INV_CLOCKWORK_BEETLE_COMBO1";
    case INV_EXAMINE1:
        return "INV_EXAMINE1";
    case INV_EXAMINE2:
        return "INV_EXAMINE2";
    case INV_EXAMINE3:
        return "INV_EXAMINE3";
    }

    return "INV_NOT_FOUND_ITEM";
}

void ResetLaraMeshSkin(void)
{
    for (int i = 0; i < MAX_LARA_MESHES; i++)
        classic_meshes(LARA, i, classic_meshes(LARA_SKIN, i));

    short mesh_target = LARA_SKIN;
    Lara.mesh.hips = classic_meshes(mesh_target, HIPS);
    Lara.mesh.thigh_l = classic_meshes(mesh_target, THIGH_L);
    Lara.mesh.calf_l = classic_meshes(mesh_target, CALF_L);
    Lara.mesh.foot_l = classic_meshes(mesh_target, FOOT_L);
    Lara.mesh.thigh_r = classic_meshes(mesh_target, THIGH_R);
    Lara.mesh.calf_r = classic_meshes(mesh_target, CALF_R);
    Lara.mesh.foot_r = classic_meshes(mesh_target, FOOT_R);
    Lara.mesh.torso = classic_meshes(mesh_target, TORSO);
    Lara.mesh.uarm_r = classic_meshes(mesh_target, UARM_R);
    Lara.mesh.larm_r = classic_meshes(mesh_target, LARM_R);
    Lara.mesh.hand_r = classic_meshes(mesh_target, HAND_R);
    Lara.mesh.uarm_l = classic_meshes(mesh_target, UARM_L);
    Lara.mesh.larm_l = classic_meshes(mesh_target, LARM_L);
    Lara.mesh.hand_l = classic_meshes(mesh_target, HAND_L);
    Lara.mesh.head = classic_meshes(mesh_target, HEAD);
}

CREATURE_INFO* GetCreatureInfo(ITEM_INFO* item)
{
    return (CREATURE_INFO*)item->data;
}

short ConvertToDegrees(short angle)
{
    return short((unsigned short)angle * 360.0f / 65536.0f);
}

bool DX_TRY(HRESULT errorThrow)
{
    if (SUCCEEDED(errorThrow))
    {
        return true;
    }
    else if (FAILED(errorThrow))
    {
        S_LogValue("ERROR: %s", DXErrorText(errorThrow));
        return false;
    }
    else
    {
        return false;
    }
}

CVECTOR DWORD_TO_RGBA(DWORD color)
{
    CVECTOR result;
    result.r = color & MAXBYTE; color >>= 8;
    result.g = color & MAXBYTE; color >>= 8;
    result.b = color & MAXBYTE; color >>= 8;
    result.cd = color & MAXBYTE;
    return result;
}

DWORD RGBA_VECTORGET(CVECTOR color)
{
    return (((((color.cd << 8) + color.r) << 8) + color.g) << 8) + color.b;
}

DWORD RGBA_VECTORGET(BYTE r, BYTE g, BYTE b)
{
    return RGBA_VECTORGET(r, g, b, 0);
}

DWORD RGBA_VECTORGET(BYTE r, BYTE g, BYTE b, BYTE a)
{
    return (((((a << 8) + r) << 8) + g) << 8) + b;
}

void InterpolateArm()
{
    phd_mxptr->m00 = phd_mxptr[-2].m00;
    phd_mxptr->m01 = phd_mxptr[-2].m01;
    phd_mxptr->m02 = phd_mxptr[-2].m02;
    phd_mxptr->m10 = phd_mxptr[-2].m10;
    phd_mxptr->m11 = phd_mxptr[-2].m11;
    phd_mxptr->m12 = phd_mxptr[-2].m12;
    phd_mxptr->m20 = phd_mxptr[-2].m20;
    phd_mxptr->m21 = phd_mxptr[-2].m21;
    phd_mxptr->m22 = phd_mxptr[-2].m22;
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

void phd_SwapGarYXZsuperpack(int frac, short** rotation1, short** rotation2, int skip)
{
    if (frac)
        gar_RotYXZsuperpack_I(rotation1, rotation2, skip);
    else
        gar_RotYXZsuperpack(rotation1, skip);
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

void phd_PutMatrix(PHD_MATRIX* matrix)
{
    *matrix = *phd_mxptr;
}

void phd_PutMatrix_I(PHD_MATRIX* matrix)
{
    phd_PushMatrix();
    InterpolateMatrix();
    *matrix = *phd_mxptr;
    phd_PopMatrix();
}

WEAPON_AMMO weapon_ammo;
short WEAPON_AMMO::get(int weapon_type)
{
    switch (weapon_type)
    {
        case LG_REVOLVER:
            return Lara.revolver_ammo_count;

        case LG_UZIS:
            return Lara.uzi_ammo_count;

        case LG_SHOTGUN:
            if (CHK_ANY(Lara.shotgun_type_carried, CR_AMMO1))
                return Lara.shotgun_ammo1_count;
            else
                return Lara.shotgun_ammo2_count;
            break;
        case LG_GRENADEGUN:
            if (CHK_ANY(Lara.grenadegun_type_carried, CR_AMMO1))
                return Lara.grenade_ammo1_count;
            else if (CHK_ANY(Lara.grenadegun_type_carried, CR_AMMO2))
                return Lara.grenade_ammo2_count;
            else if (CHK_ANY(Lara.grenadegun_type_carried, CR_AMMO3))
                return Lara.grenade_ammo3_count;
            break;

        case LG_CROSSBOW:
            if (CHK_ANY(Lara.crossbow_type_carried, CR_AMMO1))
                return Lara.crossbow_ammo1_count;
            else if (CHK_ANY(Lara.crossbow_type_carried, CR_AMMO2))
                return Lara.crossbow_ammo2_count;
            else if (CHK_ANY(Lara.crossbow_type_carried, CR_AMMO3))
                return Lara.crossbow_ammo3_count;
            break;

        case LG_PISTOLS:
        default:
            return Lara.pistol_ammo_count;
    }

    return -2; // NOT FOUND!
}

void WEAPON_AMMO::increase(int weapon_type, int value)
{
    switch (weapon_type)
    {
        case LG_REVOLVER:
            Lara.revolver_ammo_count += value;
            break;

        case LG_UZIS:
            Lara.uzi_ammo_count += value;
            break;

        case LG_SHOTGUN:
            if (CHK_ANY(Lara.shotgun_type_carried, CR_AMMO1))
                Lara.shotgun_ammo1_count += value;
            else
                Lara.shotgun_ammo2_count += value;
            break;

        case LG_GRENADEGUN:
            if (CHK_ANY(Lara.grenadegun_type_carried, CR_AMMO1))
                Lara.grenade_ammo1_count += value;
            else if (CHK_ANY(Lara.grenadegun_type_carried, CR_AMMO2))
                Lara.grenade_ammo2_count += value;
            else if (CHK_ANY(Lara.grenadegun_type_carried, CR_AMMO3))
                Lara.grenade_ammo3_count += value;
            break;

        case LG_CROSSBOW:
            if (CHK_ANY(Lara.crossbow_type_carried, CR_AMMO1))
                Lara.crossbow_ammo1_count += value;
            else if (CHK_ANY(Lara.crossbow_type_carried, CR_AMMO2))
                Lara.crossbow_ammo2_count += value;
            else if (CHK_ANY(Lara.crossbow_type_carried, CR_AMMO3))
                Lara.crossbow_ammo3_count += value;
            break;

        case LG_PISTOLS:
        default:
            Lara.pistol_ammo_count += value;
            break;
    }
}

void WEAPON_AMMO::decrease(int weapon_type, int value)
{
    switch (weapon_type)
    {
        case LG_REVOLVER:
            Lara.revolver_ammo_count -= value;
            break;

        case LG_UZIS:
            Lara.uzi_ammo_count -= value;
            break;

        case LG_SHOTGUN:
            if (CHK_ANY(Lara.shotgun_type_carried, CR_AMMO1))
                Lara.shotgun_ammo1_count -= value;
            else
                Lara.shotgun_ammo2_count -= value;
            break;

        case LG_GRENADEGUN:
            if (CHK_ANY(Lara.grenadegun_type_carried, CR_AMMO1))
                Lara.grenade_ammo1_count -= value;
            else if (CHK_ANY(Lara.grenadegun_type_carried, CR_AMMO2))
                Lara.grenade_ammo2_count -= value;
            else if (CHK_ANY(Lara.grenadegun_type_carried, CR_AMMO3))
                Lara.grenade_ammo3_count -= value;
            break;

        case LG_CROSSBOW:
            if (CHK_ANY(Lara.crossbow_type_carried, CR_AMMO1))
                Lara.crossbow_ammo1_count -= value;
            else if (CHK_ANY(Lara.crossbow_type_carried, CR_AMMO2))
                Lara.crossbow_ammo2_count -= value;
            else if (CHK_ANY(Lara.crossbow_type_carried, CR_AMMO3))
                Lara.crossbow_ammo3_count -= value;
            break;

        case LG_PISTOLS:
        default:
            Lara.pistol_ammo_count -= value;
            break;
    }
}

short ANGLEF(float angle)
{
    return short(angle * 65536.0f / 360.0f); // else warning !
}