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

extern void draw_shotgun_meshes(int weapon_type);
extern void undraw_shotgun_meshes(int weapon_type);
extern void ready_shotgun(int weapon_type);
extern void draw_shotgun(int weapon_type);
extern void undraw_shotgun(int weapon_type);
extern void shotgun_handler(int weapon_type);
extern void animate_shotgun(int weapon_type);

#define legacy_draw_shotgun_meshes        _FUNCV(0x00428E40, (int weapon_type), draw_shotgun_meshes)
#define legacy_undraw_shotgun_meshes      _FUNCV(0x00428E70, (int weapon_type), undraw_shotgun_meshes)
#define legacy_ready_shotgun              _FUNCV(0x00428EA0, (int weapon_type), ready_shotgun)
#define legacy_draw_shotgun               _FUNCV(0x0042AE50, (int weapon_type), draw_shotgun)
#define legacy_undraw_shotgun             _FUNCV(0x0042AFE0, (int weapon_type), undraw_shotgun)
#define legacy_shotgun_handler            _FUNCV(0x00428F10, (int weapon_type), shotgun_handler)
#define legacy_animate_shotgun            _FUNCV(0x0042B100, (int weapon_type), animate_shotgun)