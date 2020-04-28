#include "framework.h"
#include "hair.h"
#include "3d_gen.h"
#include "3d_gen_a.h"
#include "draw.h"
#include "control.h"
#include "gameflow.h"

//BOOL first_hair[HAIR_TYPE];
//HAIR_STRUCT hairs[HAIR_TYPE][HAIR_SEGMENTS + 1];

enum SPHERE_TYPE
{
    S_HIPS,
    S_TORSO,
    S_HEAD,
    S_UARM_R,
    S_UARM_L,
    S_MAX
};

#ifdef DLL_INJECT
void injector::f_game::inject_hair()
{
    //inject(0x0041FDD0, InitialiseHair);
    //inject(0x0041FE50, HairControl);
    //inject(0x00420920, DrawHair);
}
#endif
