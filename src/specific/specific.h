#pragma once

#ifdef DLL_INJECT
#define S_OutputPolyList ((void(__cdecl*)(void)) 0x0047FA10)
#define S_AnimateTextures ((void(__cdecl*)(int nframe)) 0x00480070)
#define S_GetObjectBounds ((int(__cdecl*)(short* bounds)) 0x0047FCF0)
#define SetFade ((void(__cdecl*)(int start, int end)) 0x0048C240)
#define SetFadeClip ((void(__cdecl*)(short height, short speed)) 0x00439D40)
#define SetScreenFadeIn ((void(__cdecl*)(short speed)) 0x00439E00)
#define SetFogColor ((void(__cdecl*)(BYTE red, BYTE green, BYTE blue)) 0x00476E80)
#define ClearFXFogBulbs ((void(__cdecl*)(void)) 0x004818A0)
#define S_DumpScreen ((int(__cdecl*)(void)) 0x0047FCA0)
#define S_SoundStopAllSamples ((void(__cdecl*)(void)) 0x00473AB0)
#endif
