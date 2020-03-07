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

#define legacy_InitialiseObjects    ADDRESS_STRUCT(0x0045C0D0, InitialiseObjects)
#define legacy_BaddyObjects         ADDRESS_STRUCT(0x0045C1E0, BaddyObjects)
#define legacy_ObjectObjects        ADDRESS_STRUCT(0x0045E1F0, ObjectObjects)
#define legacy_TrapObjects          ADDRESS_STRUCT(0x0045DC10, TrapObjects)
#define legacy_GetCarriedItems      ADDRESS_STRUCT(0x0045EB40, GetCarriedItems)
#define legacy_GetAIPickups         ADDRESS_STRUCT(0x0045EC50, GetAIPickups)
#endif