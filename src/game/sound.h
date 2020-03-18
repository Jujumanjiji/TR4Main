#pragma once

#ifdef DLL_INJECT
#define SayNo ((void(__cdecl*)(void)) 0x0045FAD0)
#define CDPlay ((void(__cdecl*)(int index, int mode)) 0x0046E180)
#define S_CDPlay ((void(__cdecl*)(int index, int mode)) 0x0046D610)
#define S_CDStop ((void(__cdecl*)(void)) 0x0046D6B0)
#define S_CDPlayEx ((void(__cdecl*)(int index)) 0x0046D790)
#define SoundEffect ((int(__cdecl*)(int sfxID, PHD_3DPOS* pos, int flags)) 0x0045F1F0)
#define StopSoundEffect ((void(__cdecl*)(int sfxID)) 0x0045FA10)
#define Sound_LoadSample ((bool(__cdecl*)(char* pointer, int compSize, int uncompSize, int index)) 0x00473710)
#define Sound_FreeSamples ((void(__cdecl*)(void)) 0x00473B40)
#define SOUND_Init ((void(__cdecl*)(void)) 0x0045FAA0)
#define SOUND_Stop ((void(__cdecl*)(void)) 0x0045FA70)
#endif