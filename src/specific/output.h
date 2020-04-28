#pragma once

#ifdef DLL_INJECT
#define S_InsertRoom ((void(__cdecl*)(ROOM_INFO* r)) 0x0047F950)
#define S_PrintShadow ((void(__cdecl*)(short shadowSize, short *frame, ITEM_INFO* item)) 0x004876C0)
#define S_CalculateStaticMeshLight ((void(__cdecl*)(int x, int y, int z, short shade, ROOM_INFO* r)) 0x00478520)
#endif
