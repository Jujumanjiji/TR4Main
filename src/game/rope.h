#pragma once

#ifdef DLL_INJECT
#define InitialiseRopeArray ((void(__cdecl*)(void)) 0x00453F30)
#define InitialiseRope INITIALISE(0x00454070)
#define RopeControl CONTROL(0x004593B0)
#define RopeCollision COLLISION(0x00459100)
#endif