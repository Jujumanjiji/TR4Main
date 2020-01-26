#pragma once

///#define BaddyObjects ((void(__cdecl*)(void)) 0x0045C1E0)
///#define ObjectObjects ((void(__cdecl*)(void)) 0x0045E1F0)
///#define TrapObjects ((void(__cdecl*)(void)) 0x0045DC10)
#define InitialiseHair ((void(__cdecl*)(void)) 0x0041FDD0)
#define InitialiseEffects ((void(__cdecl*)(void)) 0x004538A0)

extern void InitialiseObjects(void);
extern void BaddyObjects(void);
extern void ObjectObjects(void);
extern void TrapObjects(void);
extern void GetCarriedItems(void);
extern void GetAIPickups(void);