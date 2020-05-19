#pragma once

#ifdef DLL_INJECT
#define GetFreeSparks ((int(__cdecl*)(void)) 0x00433D30)
#define DetatchSpark ((void(__cdecl*)(short item_number, int SP_flags)) 0x00433C70)

#define CreateSpotLight ((void(__cdecl*)(PHD_VECTOR* start, PHD_VECTOR* target, short angle, int rnd)) 0x00445890)
#define TriggerUnderwaterExplosion ((void(__cdecl*)(ITEM_INFO* item, BOOL createBubble)) 0x0042B430)
#define TriggerExplosionSparks ((void(__cdecl*)(int x, int y, int z, int extraTrig, int dynamic, int uw, int roomNumber)) 0x004349F0)
#define DoBloodSplat ((short(__cdecl*)(int x, int y, int z, short speed, short y_rot, short roomNumber)) 0x00437180)
#define TriggerGunSmoke ((void(__cdecl*)(int x, int y, int z, int xv, int yv, int zv, int initial, int weapon_type, int count)) 0x00438940)
#define TriggerGunShell ((void(__cdecl*)(BOOL right, short objNumber, int weapon_type)) 0x004390F0)
#define TriggerRicochetSpark ((void(__cdecl*)(GAME_VECTOR *pos, int angle, int size, int flags)) 0x00434440)
#define TriggerDynamic ((void(__cdecl*)(int x, int y, int z, int intensity, BYTE red, BYTE green, BYTE blue)) 0x004361A0)
#define TriggerDynamic_Mirror ((void(__cdecl*)(int x, int y, int z, int intensity, BYTE red, BYTE green, BYTE blue)) 0x00436250)
#define TriggerFireFlame ((void(__cdecl*)(int x, int y, int z, int dynamic, int flags)) 0x00435130)
#endif