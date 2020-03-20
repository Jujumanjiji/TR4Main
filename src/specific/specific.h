#pragma once

#ifdef DLL_INJECT
#define FadeType VAR_U_(0x00672E60, int)
#define FadeStart VAR_U_(0x00672E58, int)
#define FadeSpeed VAR_U_(0x00670E48, int)
#define FadeTime VAR_U_(0x00670E4C, int)
#define FadeEnd VAR_U_(0x00670E50, int)

#define S_OutputPolyList ((void(__cdecl*)(void)) 0x0047FA10)
#define S_AnimateTextures ((void(__cdecl*)(int nframes)) 0x00480070)
#define S_GetObjectBounds ((int(__cdecl*)(short* bounds)) 0x0047FCF0)
#define SetFade ((void(__cdecl*)(int start, int end)) 0x0048C240)
#endif