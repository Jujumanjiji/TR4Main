#pragma once

extern void InitialiseAhmet(short item_number);
extern void AhmetControl(short item_number);

#ifdef DLL_INJECT
///#define InitialiseAhmet ((void(__cdecl*)(short item_number)) 0x00401000)
///#define AhmetControl ((void(__cdecl*)(short item_number)) 0x00401080)
///#define TriggerAhmetDeathEffect ((void(__cdecl*)(ITEM_INFO* item)) 0x00401730)
#endif
