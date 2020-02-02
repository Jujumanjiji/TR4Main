#include "framework.h"
#include "utils.h"
#include "game/oldobjects.h"
#include "3dsystem/3d_gen.h"
#include "game/control.h"
#include "game/draw.h"
#include "game/items.h"
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
