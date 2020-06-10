#pragma once

extern BOOL TargetVisible(ITEM_INFO* item, AI_INFO* info);
extern BOOL TargetVisibleCustom(ITEM_INFO* item, AI_INFO* info, int height);

#ifdef DLL_INJECT
//#define TargetVisible ((BOOL(__cdecl*)(ITEM_INFO *item, AI_INFO *info)) 0x0040AEB0)
#define Targetable ((BOOL(__cdecl*)(ITEM_INFO *item, AI_INFO *info)) 0x0040AF80)
#define GunHit ((short(__cdecl*)(int x, int y, int z, short speed, short y_rot, short roomNumber)) 0x0040B060)
#define GunMiss ((short(__cdecl*)(int x, int y, int z, short speed, short y_rot, short roomNumber)) 0x0040B120)
#define GunShot NOT_EXIST
// angle is not used in TR4
#define ShotLara ((int(__cdecl*)(ITEM_INFO *item, AI_INFO *info, BITE_INFO *bite, short y_angle, int damage)) 0x0040B1D0)
#endif