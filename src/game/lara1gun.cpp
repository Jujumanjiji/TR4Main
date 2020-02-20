#include "framework.h"
#include "lara1gun.h"
#include "larafire.h"

void draw_shotgun_meshes(int weapon_type)
{
    lara.weapon_back_item = NULL;
    lara.mesh[HAND_R] = meshes[objects[WeaponObjectMeshes(weapon_type)].mesh_index + HAND_R * 2];
}

void undraw_shotgun_meshes(int weapon_type)
{
    lara.weapon_back_item = WeaponObject(weapon_type);
    lara.mesh[HAND_R] = meshes[objects[LARA].mesh_index + HAND_R * 2];
}

void injector::inject_lara1gun()
{
    this->inject(0x00428E40, draw_shotgun_meshes);
    this->inject(0x00428E70, undraw_shotgun_meshes);
    //this->inject(0x00428EA0, ready_shotgun);
    //this->inject(0x00428F10, shotgun_handler);
    //this->inject(0x0042AE50, draw_shotgun);
    //this->inject(0x0042AFE0, undraw_shotgun);
    //this->inject(0x0042B100, animate_shotgun);
}