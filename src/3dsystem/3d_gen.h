#pragma once

extern void SetupDXMatrixTransformState(void);
extern void SetupDXMatrix(D3DMATRIX *mptr);
extern void SetupDXW2V(D3DMATRIX* dest, PHD_MATRIX* pptr);
extern void phd_GenerateW2V(PHD_3DPOS* viewpos);
extern void phd_LookAt(int xsrc, int ysrc, int zsrc, int xtar, int ytar, int ztar, short roll);
extern BOOL phd_TranslateRel(int x, int y, int z);
extern void phd_TranslateAbs(int x, int y, int z);
extern void phd_RotX(short rx);
extern void phd_RotY(short ry);
extern void phd_RotZ(short rz);
extern void phd_RotYXZpack(int angs);
extern void phd_RotYXZ(short rx, short ry, short rz);
extern void phd_GetVectorAngles(int x, int y, int z, short* dest);

#ifdef DLL_INJECT
#define AlterFOV ((void(__cdecl*)(short fov)) 0x0048F9D0)

#define phd_PutPolygons ((void(__cdecl*)(short* ptr, int clip)) 0x0047DA60)
#define phd_PutPolygons_inv ((void(__cdecl*)(short* ptr, float x, float y, DWORD rgba)) 0x0047E8B0)
#define phd_atan ((int(__cdecl*)(int x, int y)) 0x00490210)
#define phd_sqrt ((int(__cdecl*)(int x)) 0x00490280)
#define ScaleCurrentMatrix ((void(__cdecl*)(PHD_VECTOR* shift)) 0x0048FB60)
#define phd_InitWindow ((void(__cdecl*)(int x, int y, int width, int height, int nearz, int farz, int view_angle)) 0x0048FC10)
#define SetD3DViewMatrix ((void(__cdecl*)(void)) 0x00490B30)
#define mGetAngle ((DWORD(__cdecl*)(DWORD srcX, DWORD srcZ, DWORD targetX, DWORD targetZ)) 0x0048FD40)
//#define SetupDXMatrixTransformState ((void(__cdecl*)(void)) 0x00490CF0)
#endif