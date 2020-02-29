#pragma once
extern int baddie_count;

extern void InitialiseLOT(BOOL allocateBaddie);
extern void DisableBaddieAI(short itemNumber);
extern BOOL EnableBaddieAI(short itemNumber, BOOL always);
extern void InitialiseSlot(short itemNumber, int slots);
extern void CreateZone(ITEM_INFO* item);
extern void ClearLOT(LOT_INFO* LOT);

#ifdef DLL_INJECT
#define legacy_InitialiseLOT            _FUNCV(0x00455200, (BOOL allocateBaddie), InitialiseLOT)
#define legacy_DisableBaddieAI          _FUNCV(0x00455270, (short itemNumber), DisableBaddieAI)
#define legacy_EnableBaddieAI           _FUNCR(0x004552B0, BOOL, (short itemNumber, BOOL always), EnableBaddieAI)
#define legacy_InitialiseSlot           _FUNCV(0x00455470, (short itemNumber, int slots), InitialiseSlot)
#define legacy_CreateZone               _FUNCV(0x004556A0, (ITEM_INFO* item), CreateZone)
#define legacy_ClearLOT                 _FUNCV(0x004557B0, (LOT_INFO* LOT), CreateZone)
#endif