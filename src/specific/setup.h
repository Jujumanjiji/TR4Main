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

#define legacy_InitialiseObjects    _FUNCV(0x0045C0D0, (void), InitialiseObjects)
#define legacy_BaddyObjects         _FUNCV(0x0045C1E0, (void), BaddyObjects)
#define legacy_ObjectObjects        _FUNCV(0x0045E1F0, (void), ObjectObjects)
#define legacy_TrapObjects          _FUNCV(0x0045DC10, (void), TrapObjects)
#define legacy_GetCarriedItems      _FUNCV(0x0045EB40, (void), GetCarriedItems)
#define legacy_GetAIPickups         _FUNCV(0x0045EC50, (void), GetAIPickups)
#endif