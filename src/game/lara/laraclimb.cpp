#include "framework.h"
#include "lara.h"
#include "3d_gen.h"
#include "camera.h"
#include "sound.h"
#include "utils.h"

void lara_as_climbstnc(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.is_climbing = TRUE;
    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = FALSE;
    camera.target_elevation = CAM_E_CLIMBS;

    if (CHK_EXI(TrInput, IN_LOOK))
    {
        LookUpDown();
    }

    if (CHK_EXI(TrInput, (IN_LEFT | IN_LSTEP)))
    {
        item->state_next = STATE_LARA_LADDER_LEFT;
        lara.move_angle = item->pos.y_rot - 0x4000;
    }
    else if (CHK_EXI(TrInput, (IN_RIGHT | IN_RSTEP)))
    {
        item->state_next = STATE_LARA_LADDER_RIGHT;
        lara.move_angle = item->pos.y_rot + 0x4000;
    }
    else if (CHK_EXI(TrInput, IN_JUMP) && item->current_anim == ANIMATION_LARA_LADDER_IDLE)
    {
        item->state_next = STATE_LARA_JUMP_BACK;
        lara.move_angle = item->pos.y_rot - 0x8000;
        lara.gun_status = LHS_ARMLESS;
    }
}

void lara_as_climbup(ITEM_INFO* item, COLL_INFO* coll)
{
    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = FALSE;
    camera.target_elevation = CAM_E_CLIMBU;
}

void lara_as_climbleft(ITEM_INFO* item, COLL_INFO* coll)
{
    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = FALSE;
    camera.target_angle = CAM_A_CLIMBL;
    camera.target_elevation = CAM_E_CLIMBL;
    if (CHK_NOP(TrInput, (IN_LEFT | IN_LSTEP)))
        item->state_next = STATE_LARA_LADDER_IDLE;
}

void lara_as_climbend(ITEM_INFO* item, COLL_INFO* coll)
{
    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = FALSE;
    camera.flags = FOLLOW_CENTRE;
    camera.target_angle = -ANGLE(45);
}

void lara_as_climbright(ITEM_INFO* item, COLL_INFO* coll)
{
    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = FALSE;
    camera.target_angle = CAM_A_CLIMBR;
    camera.target_elevation = CAM_E_CLIMBR;
    if (CHK_NOP(TrInput, (IN_RIGHT | IN_RSTEP)))
        item->state_next = STATE_LARA_LADDER_IDLE;
}

void lara_as_climbdown(ITEM_INFO* item, COLL_INFO* coll)
{
    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = FALSE;
    camera.target_elevation = CAM_E_CLIMBD;
}