#pragma once

#define DoBloodSplat ((short(__cdecl*)(int x, int y, int z, short speed, short y_rot, short roomNumber)) 0x00437180)

#define TriggerDynamic ((void(__cdecl*)(int x, int y, int z, int intensity, BYTE red, BYTE green, BYTE blue)) 0x004361A0)