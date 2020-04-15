#pragma once

enum HAIR_TYPES
{
    HAIR_NORMAL,
    HAIR_YOUNG
};

#pragma pack(push, 1)
struct HAIR_STRUCT
{
    PHD_3DPOS pos;
    PHD_VECTOR vel;
};
#pragma pack(pop)

#define HAIR_SEGMENTS 6             // number of mesh the hair have...
#define HAIR_COUNT 2                // normal, young

/*
//extern BOOL first_hair[HAIR_TYPE];
//extern HAIR_STRUCT hairs[HAIR_TYPE][HAIR_SEGMENTS + 1];
*/
extern void InitialiseHair(void);
extern void HairControl(BOOL inWater, BOOL isYoung, short* frame);
//extern void DrawHair(void);


#ifdef DLL_INJECT
#define first_hair ARRAY_(0x0080E140, BOOL, [HAIR_COUNT])
#define hairs ARRAY_(0x0080E160, HAIR_STRUCT, [HAIR_COUNT][HAIR_SEGMENTS + 1])

//#define InitialiseHair ((void(__cdecl*)(void)) 0x0041FDD0)
//#define HairControl ((void(__cdecl*)(BOOL inWater, BOOL isYoung, short* frame)) 0x0041FE50)
#define DrawHair ((void(__cdecl*)(void)) 0x00420920)
#endif
