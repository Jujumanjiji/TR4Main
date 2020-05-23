#pragma once

#ifdef DLL_INJECT
#define GetCollisionAnim ((int(__cdecl*)(ITEM_INFO* item, PHD_VECTOR* moved)) 0x00468AE0)
#define DoShift ((int(_cdecl*)(ITEM_INFO *item, PHD_VECTOR *pos, PHD_VECTOR *old)) 0x00468B80)

#define MotorBikeStaticCollision ((void(__cdecl*)(int x, int y, int z, short roomNumber, int distance)) 0x00465F80)
#define MotorBikeBaddieCollision ((void(__cdecl*)(ITEM_INFO *item)) 0x00466290)
#endif
