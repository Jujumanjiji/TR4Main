#pragma once

extern void GetLaraJointAbsPosition(PHD_VECTOR* pos, int joint);

#ifdef DLL_INJECT
#define GetJointAbsPosition ((void(__cdecl*)(ITEM_INFO* item, PHD_VECTOR* pos, int joint)) 0x0045FF80)
#define GetSpheres ((int(__cdecl*)(ITEM_INFO* item, SPHERE *ptr, int worldspace)) 0x0045FC90)
#endif