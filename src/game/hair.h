#pragma once

enum HAIR_TYPES
{
    HAIR_NORMAL,
    HAIR_YOUNG
};

#define HAIR_SEGMENTS 6             // number of mesh the hair have...
#define HAIR_COUNT 2                // normal, young


#ifdef DLL_INJECT
#define first_hair ARRAY_(0x0080E140, BOOL, [HAIR_COUNT])
#define hairs ARRAY_(0x0080E160, HAIR_STRUCT, [HAIR_COUNT][HAIR_SEGMENTS + 1])

#define InitialiseHair ((void(__cdecl*)(void)) 0x0041FDD0)
#define HairControl ((void(__cdecl*)(BOOL inWater, BOOL isYoung, sint16* frame)) 0x0041FE50)
#define DrawHair ((void(__cdecl*)(void)) 0x00420920)
#endif
