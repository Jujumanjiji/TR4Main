#pragma once
extern int BaddieCount;

extern void InitialiseLOT(BOOL allocateBaddie);
extern void DisableBaddieAI(short itemNumber);
extern BOOL EnableBaddieAI(short itemNumber, BOOL always);
extern void InitialiseSlot(short itemNumber, int slots);
extern void CreateZone(ITEM_INFO* item);
extern void ClearLOT(LOT_INFO* LOT);