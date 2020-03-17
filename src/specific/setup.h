#pragma once

extern void InitialiseObjects(void);
extern void BaddyObjects(void);
extern void ObjectObjects(void);
extern void TrapObjects(void);
extern void GetCarriedItems(void);
extern void GetAIPickups(void);

#ifdef DLL_INJECT
#define InitialiseHair ((void(__cdecl*)(void)) 0x0041FDD0)
#define InitialiseEffects ((void(__cdecl*)(void)) 0x004538A0)
#endif