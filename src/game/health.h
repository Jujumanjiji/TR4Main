#pragma once

#define FlashIt ((int(__cdecl*)(void)) 0x00452C70)
#define DrawGameInfo ((void(__cdecl*)(int state)) 0x00452CA0)
#define DrawHealthBar ((void(__cdecl*)(int state)) 0x00452DE0)
#define DrawAirBar ((void(__cdecl*)(int state)) 0x00452EA0)
#define DrawPickups ((void(__cdecl*)(int state)) 0x00452F80)