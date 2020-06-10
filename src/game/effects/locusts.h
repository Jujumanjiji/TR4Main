#pragma once

struct LOCUST_INFO
{
    bool on;
    PHD_3DPOS pos;
    ITEM_INFO* target;
    short roomNumber;
    short randomRotation;
    short escapeXrot;
    short escapeYrot;
    short escapeZrot;
    BYTE counter;
};

constexpr auto MAX_LOCUSTS = 64;
extern LOCUST_INFO Locusts[MAX_LOCUSTS];

extern int CreateLocust(void);
extern void SpawnLocust(ITEM_INFO* item);
extern void InitialiseLocust(short itemNumber);
extern void LocustControl(short itemNumber);
extern void UpdateLocusts(void);
extern void DrawLocust(void);

#ifdef DLL_INJECT
#define phd_PutPolygonsLocust ((void(__cdecl*)(short* ptr, int clip)) 0x00480310)
#endif
