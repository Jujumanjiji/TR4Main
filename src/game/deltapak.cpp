#include "framework.h"
#include "deltapak.h"
#include "3d_gen_a.h"
#include "delstuff.h"
#include "gameflow.h"
#include "hair.h"

// used for cutscene to render lara !
void frigup_lara(void)
{
    LaraItem->pos.xPos = ActorCutPos[1];
    LaraItem->pos.yPos = ActorCutPos[2];
    LaraItem->pos.zPos = ActorCutPos[3];
    int* bone = &Bones[Objects[LaraItem->objectNumber].boneIndex];

    UpdateAnimFrame(ActorPlayerNodes, 16, ActorFrame);
    CalcLaraMatrices_Cutscene(ActorFrame, bone, 0);
    phd_PushUnitMatrix();
    CalcLaraMatrices_Cutscene(ActorFrame, bone, 1);
    phd_PopMatrix();
    
    HairControl(FALSE, FALSE, ActorFrame);
    if (CHK_EXI(gfLevelFlags, SLEV_YOUNG_LARA))
        HairControl(FALSE, TRUE, ActorFrame);

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
