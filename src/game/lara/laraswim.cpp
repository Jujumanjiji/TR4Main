#include "framework.h"
#include "lara.h"
#include "3d_gen.h"
#include "camera.h"
#include "sound.h"
#include "utils.h"

void lara_as_tread(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_WATER_DEATH;
        return;
    }

    if (CHK_EXI(TrInput, IN_ROLL))
    {
        SetAnimationForItemAS(item, ANIMATION_LARA_UNDERWATER_ROLL_BEGIN, STATE_LARA_UNDERWATER_TURNAROUND);
        return;
    }

    if (CHK_EXI(TrInput, IN_LOOK))
        LookUpDown();

    SwimTurn(item);

    if (CHK_EXI(TrInput, IN_JUMP))
        item->goalAnimState = STATE_LARA_UNDERWATER_FORWARD;

    LaraClampN(item->fallspeed, WATER_FRICTION, 0);

    if (Lara.gunStatus == LHS_HANDBUSY)
        Lara.gunStatus = LHS_ARMLESS;
}

void lara_as_swim(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_WATER_DEATH;
        return;
    }

    if (CHK_EXI(TrInput, IN_ROLL))
    {
        SetAnimationForItemAS(item, ANIMATION_LARA_UNDERWATER_ROLL_BEGIN, STATE_LARA_UNDERWATER_TURNAROUND);
        return;
    }

    SwimTurn(item);
    LaraClampP(item->fallspeed, UW_FRICTION, UW_MAXSPEED);

    if (CHK_NOP(TrInput, IN_JUMP))
        item->goalAnimState = STATE_LARA_UNDERWATER_INERTIA;
}

void lara_as_glide(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_WATER_DEATH;
        return;
    }

    if (CHK_EXI(TrInput, IN_ROLL))
    {
        SetAnimationForItemAS(item, ANIMATION_LARA_UNDERWATER_ROLL_BEGIN, STATE_LARA_UNDERWATER_TURNAROUND);
        return;
    }

    if (CHK_EXI(TrInput, IN_JUMP))
        item->goalAnimState = STATE_LARA_UNDERWATER_FORWARD;

    SwimTurn(item);
    LaraClampN(item->fallspeed, WATER_FRICTION, 0);

    if (item->fallspeed <= ((UW_MAXSPEED * 2) / 3))
        item->goalAnimState = STATE_LARA_UNDERWATER_STOP;
}

void lara_as_dive(ITEM_INFO* item, COLL_INFO* coll)
{
    if (CHK_EXI(TrInput, IN_FORWARD))
        item->pos.xRot -= ONE_DEGREE;
}

void lara_as_uwdeath(ITEM_INFO* item, COLL_INFO* coll)
{
    Lara.look = FALSE;

    item->fallspeed -= UW_FRICTION;
    if (item->fallspeed <= 0)
        item->fallspeed = 0;

    if (item->pos.xRot >= -ANGLE(2) && item->pos.xRot <= ANGLE(2))
        item->pos.xRot = 0;
    else if (item->pos.xRot < 0)
        item->pos.xRot += ANGLE(2);
    else
        item->pos.xRot -= ANGLE(2);
}

void lara_as_waterroll(ITEM_INFO* item, COLL_INFO* coll)
{
    item->fallspeed = 0;
}