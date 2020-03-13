#pragma once

#ifdef DLL_INJECT
#define GetSwitchTrigger ((short(__cdecl*)(ITEM_INFO* item, short* index, int flags)) 0x00461BD0)

#define SwitchCollision COLLISION(0x00461D60)
#define SwitchControl CONTROL(0x00461CF0)
#define SequenceSwitchControl CONTROL(0x00463080)
#define SequenceSwitchCollision COLLISION(0x00463180)
#define SwitchUnderwater1Collision COLLISION(0x00461FF0)
#define SwitchUnderwater2Collision COLLISION(0x00462100)
#define TurnSwitchControl CONTROL(0x004624C0)
#define TurnSwitchCollision COLLISION(0x00462720)
#define LeverSwitchCollision COLLISION(0x00462AE0)
#define JumpSwitchCollision COLLISION(0x00462CB0)
#define CrowbarSwitchCollision COLLISION(0x00462DB0)
#define CogSwitchControl CONTROL(0x004632F0)
#define CogSwitchCollision COLLISION(0x00463400)
#define InitialisePulleySwitch INITIALISE(0x00453E40)
#define PulleySwitchCollision COLLISION(0x004622C0)
#endif