#pragma once

#ifdef DLL_INJECT
#define SetUpLensFlare ((void(__cdecl*)(int x, int y, int z, GAME_VECTOR* bulb)) 0x0048BC30)
#endif