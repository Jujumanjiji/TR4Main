#pragma once

#ifdef DLL_INJECT
#define HairControl ((void(__cdecl*)(BOOL inWater, BOOL isYoung, short* frame)) 0x0041FE50)
#endif
