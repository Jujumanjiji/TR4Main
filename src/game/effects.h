#pragma once

extern void Splash(ITEM_INFO* item);

#ifdef DLL_INJECT
#define splash_setup VAR_U_(0x00804F40, SPLASH_STRUCT)
#define splash_count VAR_I_(0x004BF36C, int, 0)

#define WadeSplash ((void(__cdecl*)(ITEM_INFO* item, int water_height, int water_depth)) 0x00437390)
#define SetupSplash ((void(__cdecl*)(SPLASH_STRUCT *splash)) 0x00435920)
#endif