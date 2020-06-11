#include "framework.h"
#include "delstuff.h"
#include "lara_render.h"

void CalcLaraMatrices(short pprot)
{
    // lara render (mesh + weapon) (normal and interpolate)
    lara_render.initialise(false, nullptr, pprot);
    lara_render.check_hitdirection(NULL);
    lara_render.create_rotation(NULL);
    lara_render.create_shadow(NULL);
    lara_render.assign_weapon();

    // start rendering...
    lara_render.start_world();
    lara_render.start_list();

    lara_render.mesh_hips(NULL);
    lara_render.mesh_thight_l();
    lara_render.mesh_calf_l();
    lara_render.mesh_foot_l();
    lara_render.mesh_thight_r();
    lara_render.mesh_calf_r();
    lara_render.mesh_foot_r();
    lara_render.mesh_torso();
    lara_render.mesh_head();
    lara_render.mesh_unarmed();
    lara_render.mesh_2gun();
    lara_render.mesh_1gun();
    lara_render.mesh_revolver();

    lara_render.end_list();
    lara_render.end_world();
}

void CalcLaraMatrices_Cutscene(short *frame, int *bone, short pprot)
{
    // lara render (mesh + weapon) (normal and interpolate)
    lara_render.initialise(true, bone, pprot);
    lara_render.check_hitdirection(frame);
    lara_render.create_rotation(frame);
    lara_render.create_shadow(frame);
    lara_render.assign_weapon();

    // start rendering...
    lara_render.start_world();
    lara_render.start_list();

    lara_render.mesh_hips(frame);
    lara_render.mesh_thight_l();
    lara_render.mesh_calf_l();
    lara_render.mesh_foot_l();
    lara_render.mesh_thight_r();
    lara_render.mesh_calf_r();
    lara_render.mesh_foot_r();
    lara_render.mesh_torso();
    lara_render.mesh_head();
    lara_render.mesh_unarmed();
    lara_render.mesh_2gun();
    lara_render.mesh_1gun();
    lara_render.mesh_revolver();

    lara_render.end_list();
    lara_render.end_world();
}

void DEL_CalcLaraMatrices_Interpolate(short *frame1, short *frame2, int frac, int rate, int *bone, short pprot)
{
    // not used anymore since CalcLaraMatrices() contains all the render !
}

#ifdef DLL_INJECT
void injector::f_game::inject_delstuff()
{
    inject(0x0041D010, CalcLaraMatrices);
    inject(0x0041DAF0, CalcLaraMatrices_Cutscene);
    inject(0x0041E630, DEL_CalcLaraMatrices_Interpolate);
}
#endif
