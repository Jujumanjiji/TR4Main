#pragma once

#ifdef DLL_INJECT
#define CalcLaraMatrices ((void(__cdecl*)(int flags)) 0x0041D010)
#endif