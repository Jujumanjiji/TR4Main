#pragma once

#define WeaponObject ((int(__cdecl*)(int weapon_type)) 0x0042EA70)
#define WeaponObjectMeshes ((int(__cdecl*)(int weapon_type)) 0x0042EAC0)

#define fire_shotgun ((void(__cdecl*)(void)) 0x00429260)
#define fire_grenade ((void(__cdecl*)(void)) 0x00429480)
#define fire_crossbow ((void(__cdecl*)(PHD_3DPOS* haveDefinedPos)) 0x0042A270)