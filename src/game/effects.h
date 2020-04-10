#pragma once

#ifdef DLL_INJECT
#define Splash ((void(__cdecl*)(ITEM_INFO* item)) 0x004372A0)
#define WadeSplash ((void(__cdecl*)(ITEM_INFO* item, int water_height, int water_depth)) 0x00437390)
#endif