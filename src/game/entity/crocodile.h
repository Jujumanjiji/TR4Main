#pragma once

#ifdef DLL_INJECT
#define InitialiseCrocodile ((void(__cdecl*)(short itemNumber)) 0x00402D90)
#define CrocodileControl ((void(__cdecl*)(short itemNumber)) 0x00402E30)
#endif
