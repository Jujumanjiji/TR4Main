#include "framework.h"
#include "deltapak.h"
#include "3d_gen_a.h"
#include "delstuff.h"
#include "gameflow.h"
#include "hair.h"

// used for cutscene to render lara !
void frigup_lara(void)
{
    LaraItem->pos.xPos = GLOBAL_cutpos[1];
    LaraItem->pos.yPos = GLOBAL_cutpos[2];
    LaraItem->pos.zPos = GLOBAL_cutpos[3];
    BONE_STRUCT* bone = (BONE_STRUCT*)&bones[Objects[LaraItem->objectNumber].boneIndex];

    UpdateAnimFrame(actor_pnodes, 16, actor_frame);
    CalcLaraMatrices_Cutscene(actor_frame, bone, 0);
    phd_PushUnitMatrix();
    CalcLaraMatrices_Cutscene(actor_frame, bone, 1);
    phd_PopMatrix();
    
    HairControl(FALSE, FALSE, actor_frame);
    if (CHK_ANY(gfLevelFlags, SLEV_YOUNG_LARA))
        HairControl(FALSE, TRUE, actor_frame);

    lara_shadow_bbox = frig_shadow_bbox;
    if (cutseq_num == 12)
        lara_shadow_bbox = frig2_shadow_bbox;
}

#ifdef DLL_INJECT
void injector::f_game::inject_deltapak()
{
    inject(0x0046B140, frigup_lara);
}
#endif
