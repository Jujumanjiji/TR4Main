#pragma once

#ifdef DLL_INJECT
#define S_OutputPolyList ((void(__cdecl*)(void)) 0x0047FA10)
#define S_AnimateTextures ((void(__cdecl*)(int nframes)) 0x00480070)
#define S_GetObjectBounds ((int(__cdecl*)(short* bounds)) 0x0047FCF0)
#endif