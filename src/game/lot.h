#pragma once

///#define InitialiseLOT ((void(__cdecl*)(BOOL allocateBaddie)) 0x00455200)
///#define DisableBaddieAI ((void(__cdecl*)(short itemNumber)) 0x00455270)
///#define EnableBaddieAI ((BOOL(__cdecl*)(short itemNumber, BOOL always)) 0x004552B0)
///#define InitialiseSlot ((void(__cdecl*)(short itemNumber, int slots)) 0x00455470)
///#define CreateZone ((void(__cdecl*)(ITEM_INFO* item)) 0x004556A0)
///#define ClearLOT ((void(__cdecl*)(LOT_INFO* LOT)) 0x004557B0)

extern int baddie_count;

extern void InitialiseLOT(BOOL allocateBaddie);
extern void DisableBaddieAI(short itemNumber);
extern BOOL EnableBaddieAI(short itemNumber, BOOL always);
extern void InitialiseSlot(short itemNumber, int slots);
extern void CreateZone(ITEM_INFO* item);
extern void ClearLOT(LOT_INFO* LOT);