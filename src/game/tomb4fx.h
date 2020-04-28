#pragma once

#ifdef DLL_INJECT
#define FadeScreenHeight        VAR_I_(0x004BF380, short, 0)
#define DestFadeScreenHeight    VAR_I_(0x004BF382, short, 0)
#define DestFadeScreenSpeed     VAR_I_(0x004BF384, short, 0)
#define ScreenFadedOut          VAR_I_(0x004BF37A, short, 0)
#define ScreenFading            VAR_I_(0x004BF37C, short, 0)
#define ScreenFadeBack          VAR_I_(0x004BF378, short, 0)
#define ScreenFadeSpeed         VAR_I_(0x004ADF58, short, 8)
#define dScreenFade             VAR_I_(0x004BF376, short, 0)
#define ScreenFade              VAR_I_(0x004BF374, short, 0)

#define SetUpLensFlare ((void(__cdecl*)(int x, int y, int z, GAME_VECTOR* bulb)) 0x0048BC30)
#endif