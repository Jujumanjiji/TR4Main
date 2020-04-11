#pragma once

extern void phd_PushMatrix(void);
extern void phd_PushUnitMatrix(void);
extern void phd_PopMatrix(void);

///#define phd_PushMatrix ((void(__cdecl*)(void)) 0x004902B0)
///#define phd_PushUnitMatrix ((void(__cdecl*)(void)) 0x00490310)