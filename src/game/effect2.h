#pragma once

#ifdef DLL_INJECT
#define DoBloodSplat ((short(__cdecl*)(int x, int y, int z, short speed, short y_rot, short roomNumber)) 0x00437180)
#define TriggerGunSmoke ((void(__cdecl*)(int x, int y, int z, int xv, int yv, int zv, int initial, int weapon_type, int count)) 0x00438940)
#define TriggerGunShell ((void(__cdecl*)(BOOL right, short objNumber, int weapon_type)) 0x004390F0)
#define TriggerDynamic ((void(__cdecl*)(int x, int y, int z, int intensity, BYTE red, BYTE green, BYTE blue)) 0x004361A0)
#define TriggerDynamic_Mirror ((void(__cdecl*)(int x, int y, int z, int intensity, BYTE red, BYTE green, BYTE blue)) 0x00436250)
#endif