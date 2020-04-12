#pragma once

enum GAR_TYPES
{
    GAR_YXZPACK,
    GAR_X,
    GAR_Y,
    GAR_Z
};

extern int DrawPhaseGame(void);
extern void InitInterpolate(int frac, int rate);
extern void DrawEffect(short fx_number);

extern void phd_PopMatrix_I(void);
extern void phd_PushMatrix_I(void);
extern void phd_RotY_I(short ang);
extern void phd_RotX_I(short ang);
extern void phd_RotZ_I(short ang);
extern void phd_TranslateRel_I(int x, int y, int z);
extern void phd_TranslateRel_ID(int x1, int y1, int z1, int x2, int y2, int z2);
extern void phd_RotYXZ_I(short y, short x, short z);
extern void gar_RotYXZsuperpack_I(short** pprot1, short** pprot2, int skip);
extern void gar_RotYXZsuperpack(short** pprot, int skip);
extern void phd_PutPolygons_I(short* ptr, int clip = -1); // -1 = no clipping
extern void InterpolateMatrix(void);
extern void InterpolateArmMatrix(void);
///extern void CalculateObjectLighting(ITEM_INFO* item, short* frame);
extern int GetFrames(ITEM_INFO* item, short* frame[], int* rate);
extern short* GetBoundsAccurate(ITEM_INFO* item);
extern short* GetBestFrame(ITEM_INFO* item);

#ifdef DLL_INJECT
#define SetLaraUnderwaterNodes ((void(__cdecl*)(void)) 0x0041F260)
#define DrawRooms ((void(__cdecl*)(short currentRoom)) 0x0044EC10)
#define PrintRooms ((void(__cdecl*)(short roomNumber)) 0x0044F2D0)
#define PrintObjects ((void(__cdecl*)(short roomNumber)) 0x0044F330)
#define GetRoomBounds ((void(__cdecl*)(void)) 0x0044F5D0)
#define SetRoomBounds ((void(__cdecl*)(short *point, short roomNumber, ROOM_INFO* parent)) 0x0044F790)
///#define DrawEffect ((void(__cdecl*)(short fxNumber)) 0x0044FB10)
#define DrawMovingItem ((void(__cdecl*)(ITEM_INFO* item, short *frame)) 0x0044FC00)
#define DrawAnimatingItem ((void(__cdecl*)(ITEM_INFO* item)) 0x0044FF60)
#define DrawLara ((void(__cdecl*)(ITEM_INFO* item, BOOL lightDisabled)) 0x0041D140)
#define DrawLara_Mirror ((void(__cdecl*)(void)) 0x00455800)
#define CalculateObjectLighting ((void(__cdecl*)(ITEM_INFO* item, short* frame)) 0x00450BB0)
#define CalculateObjectLightingLara ((void(__cdecl*)(void)) 0x00450CB0)
#endif