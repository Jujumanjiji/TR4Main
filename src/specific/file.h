#pragma once

//extern void LoadRooms(void);
extern void LoadObjects(void);

#ifdef DLL_INJECT
#define LoadRooms ((void(__cdecl*)(void)) 0x004749C0)
//#define LoadObjects ((void(__cdecl*)(void)) 0x00474E10)
#define FixUpRoom ((void(__cdecl*)(ROOM_INFO* room)) 0x00471420)
#define BuildOutsideRoom ((void(__cdecl*)(void)) 0x0045ED30)
#define CreateSkinningData ((void(__cdecl*)(void)) 0x00430F70)
#define ProcessMeshData ((void(__cdecl*)(int nmeshes)) 0x00471040)
#endif