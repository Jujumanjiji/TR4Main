#pragma once

#define weapon_object ((int(__cdecl*)(int weapon_type)) 0x0042EA70)
#define weapon_meshes ((int(__cdecl*)(int weapon_type)) 0x0042EAC0)

#define fire_shotgun ((void(__cdecl*)(void)) 0x00429260)
#define fire_grenade ((void(__cdecl*)(void)) 0x00429480)
#define fire_crossbow ((void(__cdecl*)(PHD_3DPOS* haveDefinedPos)) 0x0042A270)

#define InitialiseNewWeapons ((void(__cdecl*)(void)) 0x0042DDF0)
#define LaraTargetInfo ((void(__cdecl*)(WEAPON_INFO* winfo)) 0x0042DF30)
#define LaraGetNewTarget ((void(__cdecl*)(WEAPON_INFO* winfo)) 0x0042E0D0)
#define AimWeapon ((void(__cdecl*)(WEAPON_INFO* winfo, LARA_ARM* arm)) 0x0042E560)
#define FireWeapon ((BOOL(__cdecl*)(int weapon_type, ITEM_INFO* target, ITEM_INFO* laraitem, short angles[2])) 0x0042E630)