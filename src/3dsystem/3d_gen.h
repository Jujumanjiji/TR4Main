#pragma once

#ifdef DLL_INJECT
#define AlterFOV ((void(__cdecl*)(short fov)) 0x0048F9D0)
#define phd_GenerateW2V ((void(__cdecl*)(PHD_3DPOS* viewpos)) 0x0048FDC0)
#define phd_LookAt ((void(__cdecl*)(int xsrc, int ysrc, int zsrc, int xtar, int ytar, int ztar, int roll)) 0x00490110)
#define mGetAngle ((DWORD(__cdecl*)(DWORD srcX, DWORD srcZ, DWORD targetX, DWORD targetZ)) 0x0048FD40)
#define phd_TranslateRel ((void(__cdecl*)(int x, int y, int z)) 0x00490350)
#define phd_TranslateAbs ((void(__cdecl*)(int x, int y, int z)) 0x00490A10)
#define phd_RotX ((void(__cdecl*)(short ang)) 0x00490400)
#define phd_RotY ((void(__cdecl*)(short ang)) 0x004904B0)
#define phd_RotZ ((void(__cdecl*)(short ang)) 0x00490560)
#define phd_RotYXZpack ((void(__cdecl*)(int ang)) 0x00490610)
#define phd_RotYXZ ((void(__cdecl*)(short y, short x, short z)) 0x00490820)
#define phd_GetVectorAngles ((void(__cdecl*)(int x, int y, int z, short* dest)) 0x00490A90)
#define phd_PutPolygons ((void(__cdecl*)(short* ptr, int clip)) 0x0047DA60)
#define phd_atan ((int(__cdecl*)(int x, int y)) 0x00490210)
#define phd_sqrt ((int(__cdecl*)(int x)) 0x00490280)
#define ShiftMatrixBasedPos ((void(__cdecl*)(PHD_VECTOR* shift)) 0x0048FB60)
#define phd_InitWindow ((void(__cdecl*)(int x, int y, int width, int height, int nearz, int farz, int view_angle)) 0x0048FC10)
#endif