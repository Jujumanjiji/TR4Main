#pragma once

#define SetLaraUnderwaterNodes ((void(__cdecl*)(void)) 0x0041F260)
#define DrawPhaseGame ((int(__cdecl*)(void)) 0x0044EBA0)
#define DrawRooms ((void(__cdecl*)(short currentRoom)) 0x0044EC10)
#define PrintRooms ((void(__cdecl*)(short roomNumber)) 0x0044F2D0)
#define PrintObjects ((void(__cdecl*)(short roomNumber)) 0x0044F330)
#define GetRoomBounds ((void(__cdecl*)(void)) 0x0044F5D0)
#define SetRoomBounds ((void(__cdecl*)(short *point, short roomNumber, ROOM_INFO* parent)) 0x0044F790)
#define DrawEffect ((void(__cdecl*)(short fxNumber)) 0x0044FB10)
#define DrawMovingItem ((void(__cdecl*)(ITEM_INFO* item, short *frame)) 0x0044FC00)
#define DrawAnimatingItem ((void(__cdecl*)(ITEM_INFO* item)) 0x0044FF60)
#define InitInterpolate ((void(__cdecl*)(int frac, int rate)) 0x00450520)
#define phd_PopMatrix_I ((void(__cdecl*)(void)) 0x00450560)
#define phd_PushMatrix_I ((void(__cdecl*)(void)) 0x00450590)
#define phd_RotY_I ((void(__cdecl*)(short ang)) 0x004505C0)
#define phd_RotX_I ((void(__cdecl*)(short ang)) 0x004505F0)
#define phd_RotZ_I ((void(__cdecl*)(short ang)) 0x00450620)
#define phd_TranslateRel_I ((void(__cdecl*)(int x, int y, int z)) 0x00450650)
#define phd_TranslateRel_ID ((void(__cdecl*)(int x1, int y1, int z1, int x2, int y2, int z2)) 0x00450690)
#define phd_RotYXZ_I ((void(__cdecl*)(short y, short x, short z)) 0x004506E0)
#define gar_RotYXZsuperpack_I ((void(__cdecl*)(short **pprot1, short **pprot2, int skip)) 0x00450720)
#define gar_RotYXZsuperpack ((void(__cdecl*)(short **pprot, int skip)) 0x00450760)
#define phd_PutPolygons_I ((void(__cdecl*)(short* ptr, int clip)) 0x00450810)
#define InterpolateMatrix ((void(__cdecl*)(void)) 0x00450840)
#define InterpolateArmMatrix ((void(__cdecl*)(void)) 0x00450AB0)
#define CalculateObjectLighting ((void(__cdecl*)(ITEM_INFO* item, short* frame)) 0x00450BB0)
#define CalculateObjectLightingLara ((void(__cdecl*)(void)) 0x00450CB0)	
#define GetFrames ((int(__cdecl*)(ITEM_INFO* item, short *frame[], int *rate)) 0x00450DC0)
#define GetBoundsAccurate ((short*(__cdecl*)(ITEM_INFO* item)) 0x00450E60)
#define GetBestFrame ((short*(__cdecl*)(ITEM_INFO* item)) 0x00450EE0)

///extern void DrawRooms(short current_room);
///extern void PrintRooms(short roomNumber);
///extern void PrintObjects(short roomNumber);
///extern void GetRoomBounds(void);
///extern void SetRoomBounds(short* door, int roomNumber, ROOM_INFO* parent);
///extern void DrawEffect(short fxNumber);
///extern void DrawAnimatingItem(ITEM_INFO* item);