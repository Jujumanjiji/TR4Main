#pragma once

#ifdef DLL_INJECT
#define MyReadFile ((size_t(__cdecl*)(void* data, size_t size, size_t count, FILE* file)) 0x004A0831)
#define S_LoadLevelFile ((BOOL(__cdecl*)(char *filename)) 0x004768C0)
#define FreeLevel ((void(__cdecl*)(void)) 0x00476790)

#define AdjustUV ((void(__cdecl*)(int numTexture)) 0x004752A0)
#define LoadTextures ((int(__cdecl)(int numRoom, int numObj, int numBump)) 0x00473F20)
#define LoadRooms ((int(__cdecl*)(void)) 0x004749C0)
#define LoadObjects ((int(__cdecl*)(void)) 0x00474E10)
#define LoadSprites ((int(__cdecl*)(void)) 0x00475730)
#define LoadCameras ((int(__cdecl*)(void)) 0x00475970)
#define LoadSoundEffects ((int(__cdecl*)(void)) 0x00475A30)
#define LoadBoxes ((int(__cdecl*)(void)) 0x00475AC0)
#define LoadAnimatedTextures ((int(__cdecl*)(void)) 0x00475C70)
#define LoadTextureInfos ((int(__cdecl*)(void)) 0x00475CE0)
#define LoadItems ((int(__cdecl*)(void)) 0x00475EE0)
#define LoadDemoData ((int(__cdecl*)(void)) 0x004761E0)
#define LoadAIObjects ((int(__cdecl*)(void)) 0x004761F0)
#define LoadSamples ((int(__cdecl*)(void)) 0x00476260)
#define LoadWaterfall ((int(__cdecl*)(void)) 0x00476410)
#define LoadLevel ((int(__cdecl*)(char* filename)) 0x00476470)

#define FixUpRoom ((void(__cdecl*)(ROOM_INFO* room)) 0x00471420)
#define BuildOutsideRoom ((void(__cdecl*)(void)) 0x0045ED30)
#define CreateSkinningData ((void(__cdecl*)(void)) 0x00430F70)
#define ProcessMeshData ((void(__cdecl*)(int nmeshes)) 0x00471040)
#endif