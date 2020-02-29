#pragma once

#ifdef DLL_INJECT
#define PuzzleControl CONTROL(0x00456050)
#define PuzzleHoleCollision COLLISION(0x004582A0)
#define PuzzleDoneCollision COLLISION(0x00458260)
#endif