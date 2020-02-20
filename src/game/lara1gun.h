#pragma once

enum LARA_SHOTGUN_STATE
{
    LSS_LAND_AIM,
    LSS_LAND_DRAW,
    LSS_LAND_RECOIL,
    LSS_LAND_UNDRAW,
    LSS_LAND_UNAIM,
    LSS_LAND_RELOAD,
    LSS_WATER_AIM,
    LSS_WATER_UNAIM,
    LSS_WATER_FIRE,
    LSS_WATER_UNDRAW,
    LSS_WATER_WADE_UNDRAW,
};

///#define draw_shotgun_meshes ((void(__cdecl*)(int weapon_type)) 0x00428E40)
///#define undraw_shotgun_meshes ((void(__cdecl*)(int weapon_type)) 0x00428E70)
///#define ready_shotgun ((void(__cdecl*)(int weapon_type)) 0x00428EA0)
///#define draw_shotgun ((void(__cdecl*)(int weapon_type)) 0x0042AE50)
///#define undraw_shotgun ((void(__cdecl*)(int weapon_type)) 0x0042AFE0)
///#define shotgun_handler ((void(__cdecl*)(int weapon_type)) 0x00428F10)
///#define animate_shotgun ((void(__cdecl*)(int weapon_type)) 0x0042B100)

extern void draw_shotgun_meshes(int weapon_type);
extern void undraw_shotgun_meshes(int weapon_type);
extern void ready_shotgun(int weapon_type);
extern void draw_shotgun(int weapon_type);
extern void undraw_shotgun(int weapon_type);
extern void shotgun_handler(int weapon_type);
extern void animate_shotgun(int weapon_type);