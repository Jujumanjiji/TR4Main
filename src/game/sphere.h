#pragma once

extern void GetLaraHandAbsPosition(PHD_VECTOR* pos, int joint);

#ifdef DLL_INJECT
///#define GetLaraHandAbsPosition ((void(__cdecl*)(PHD_VECTOR* pos, int joint)) 0x0041D9A0)
#define GetJointAbsPosition ((void(__cdecl*)(ITEM_INFO* item, PHD_VECTOR* pos, int joint)) 0x0045FF80)
#define GetSpheres ((int(__cdecl*)(ITEM_INFO* item, SPHERE *ptr, int worldspace)) 0x0045FC90)
#endif