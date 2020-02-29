#pragma once

#ifdef DLL_INJECT
#define InitialisePickup INITIALISE(0x00453E90)
#define PickupControl CONTROL(0x00457610)
#define PickupCollision COLLISION(0x00457720)
#define KeyHoleCollision COLLISION(0x00458090)
#endif