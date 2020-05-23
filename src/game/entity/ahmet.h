#pragma once

extern void InitialiseAhmet(short itemNumber);
extern void AhmetControl(short itemNumber);

#ifdef DLL_INJECT
///#define InitialiseAhmet ((void(__cdecl*)(short itemNumber)) 0x00401000)
///#define AhmetControl ((void(__cdecl*)(short itemNumber)) 0x00401080)
///#define TriggerAhmetDeathEffect ((void(__cdecl*)(ITEM_INFO* item)) 0x00401730)
#endif
