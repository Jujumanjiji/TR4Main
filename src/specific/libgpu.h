#pragma once

#ifdef DLL_INJECT
#define GPU_BeginScene ((void(__cdecl*)(void)) 0x0047D5B0)
#define CreateScreenSurface ((void(__cdecl*)(void)) 0x00479F20)

#define DeleteScreenSurface ((void(__cdecl*)(void)) 0x00479F40)
#endif