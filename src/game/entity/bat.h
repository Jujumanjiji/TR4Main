#pragma once

extern void InitialiseBat(short itemNumber);
extern void BatControl(short itemNumber);

#ifdef DLL_INJECT
///#define InitialiseBat ((void(__cdecl*)(short itemNumber)) 0x00402A10)
///#define BatControl ((void(__cdecl*)(short itemNumber)) 0x00402A70)
#endif
