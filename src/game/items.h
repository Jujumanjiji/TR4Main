#pragma once

extern BOOL ItemNearLara(PHD_VECTOR* pos, int radius);
extern bool ItemNearTarget(PHD_3DPOS* src, ITEM_INFO* target, int radius);

#ifdef DLL_INJECT
#define next_item_active            VAR_U_(0x007FD20A, short)
#define next_item_free              VAR_U_(0x007FD208, short)
#define InItemControlLoop           VAR_I_(0x004BF3F5, unsigned char, 0)
#define ItemNewRooms                ARRAY_(0x007FE3A0, short, [512])
#define ItemNewRoomsNo              VAR_I_(0x004BF3F6, short, 0)

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
//#define ItemNearLara ((BOOL(__cdecl*)(PHD_VECTOR* pos, int radius)) 0x00437050)
#endif