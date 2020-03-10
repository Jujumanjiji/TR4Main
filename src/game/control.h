#pragma once

#ifdef DLL_INJECT
#define TranslateItem ((void(__cdecl*)(ITEM_INFO* item, int x, int y, int z)) 0x0044A060)
#define GetChange ((int(__cdecl*)(ITEM_INFO* item, ANIM_STRUCT* anim)) 0x00449FC0)
#define AnimateItem ((void(__cdecl*)(ITEM_INFO* item)) 0x00449B90)
#define GetRandomControl ((int(__cdecl*)(void)) 0x00490EE0)
#define GetRandomDraw ((int(__cdecl*)(void)) 0x00490F00)
#define SeedRandomControl ((void(__cdecl*)(int seed)) 0x00490F20)
#define SeedRandomDraw ((void(__cdecl*)(int seed)) 0x00490F30)
#define GetFloor ((FLOOR_INFO*(__cdecl*)(int x, int y, int z, short* roomNumber)) 0x0044A1A0)
#define GetWaterHeight ((int(__cdecl*)(int x, int y, int z, short roomNumber)) 0x0044A390)
#define GetHeight ((int(__cdecl*)(FLOOR_INFO* floor, int x, int y, int z)) 0x0044A530)
#define GetCeiling ((int(__cdecl*)(FLOOR_INFO* floor, int x, int y, int z)) 0x0044B690)
#define GetWaterDepth ((int(__cdecl*)(int x, int y, int z, short roomNumber)) 0x00432BF0)
#define TestTriggers ((void(__cdecl*)(short* data, BOOL heavy, int unknown)) 0x0044AB50)
#define UpdateLightning ((void(__cdecl*)(void)) 0x00451180)
#endif