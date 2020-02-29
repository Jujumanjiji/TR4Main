#pragma once

#ifdef DLL_INJECT
#define GetLaraHandAbsPosition ((void(__cdecl*)(PHD_VECTOR* pos, int joint)) 0x0041D9A0)
#define GetJointAbsPosition ((void(__cdecl*)(ITEM_INFO* item, PHD_VECTOR* pos, int joint)) 0x0045FF80)
#endif