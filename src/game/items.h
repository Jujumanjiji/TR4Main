#pragma once

#ifdef DLL_INJECT
#define InitialiseItemArray ((void(__cdecl*)(short itemNumber)) 0x00454140)
#define KillItem ((void(__cdecl*)(short itemNumber)) 0x004541B0)
#define CreateItem ((short(__cdecl*)(void)) 0x00454360)
#define InitialiseItem ((void(__cdecl*)(short itemNumber)) 0x004543B0)
#define RemoveActiveItem ((void(__cdecl*)(short itemNumber)) 0x004545F0)
#define RemoveDrawnItem ((void(__cdecl*)(short itemNumber)) 0x004546A0)
#define AddActiveItem ((void(__cdecl*)(short itemNumber)) 0x00454740)
#define ItemNewRoom ((void(__cdecl*)(short itemNumber, short roomNumber)) 0x004547B0)
#define InitialiseFXArray ((void(__cdecl*)(BOOL allocateEffect)) 0x004548B0)
#define CreateEffect ((short(__cdecl*)(short roomNumber)) 0x00454900)
#define KillEffect ((void(__cdecl*)(short fxNumber)) 0x00454970)
#define EffectNewRoom ((void(__cdecl*)(short fxNumber, short roomNumber)) 0x00454A90)
#endif