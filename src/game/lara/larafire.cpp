#include "framework.h"
#include "lara.h"

int weapon_object(int weapon_type)
{
    switch (weapon_type)
    {
        case LG_REVOLVER:
            return SIXSHOOTER_ANIM;
        case LG_UZIS:
            return UZI_ANIM;
        case LG_SHOTGUN:
            return SHOTGUN_ANIM;
        case LG_GRENADEGUN:
            return GRENADE_GUN_ANIM;
        case LG_CROSSBOW:
            return CROSSBOW_ANIM;
        case LG_PISTOLS:
        default:
            return PISTOLS_ANIM;
    }
}


#ifdef DLL_INJECT
void injector::inject_larafire()
{
    this->inject(ADDRESS_STRUCT(0x0042EA70, weapon_object));
}
#endif