#pragma once

#define GetRandomControl ((int(__cdecl*)(void)) 0x00490EE0)
#define GetRandomDraw ((int(__cdecl*)(void)) 0x00490F00)
#define SeedRandomControl ((void(__cdecl*)(int seed)) 0x00490F20)
#define SeedRandomDraw ((void(__cdecl*)(int seed)) 0x00490F30)

#define GetFloor ((FLOOR_INFO*(__cdecl*)(int x, int y, int z, short* roomNumber)) 0x0044A1A0)
#define GetWaterHeight ((int(__cdecl*)(int x, int y, int z, short roomNumber)) 0x0044A390)
#define GetHeight ((int(__cdecl*)(FLOOR_INFO* floor, int x, int y, int z)) 0x0044A530)

#define UpdateLightning ((void(__cdecl*)(void)) 0x00451180)