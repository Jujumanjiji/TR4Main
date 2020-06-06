#include "framework.h"
#include "lara.h"
#include "3d_gen.h"
#include "camera.h"
#include "sound.h"
#include "utils.h"

void lara_as_surftread(ITEM_INFO* item, COLL_INFO* coll)
{
    LaraClampN(item->fallspeed, SURF_FRICTION, 0);

    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_WATER_DEATH;
        return;
    }

    if (CHK_EXI(TrInput, IN_LOOK))
    {
        LookUpDown();
        return;
    }

    if (CHK_EXI(TrInput, IN_LEFT))
        item->pos.yRot -= LARA_SLOW_TURN;
    else if (CHK_EXI(TrInput, IN_RIGHT))
        item->pos.yRot += LARA_SLOW_TURN;
    else if (CHK_EXI(TrInput, IN_FORWARD))
        item->goalAnimState = STATE_LARA_ONWATER_FORWARD;
    else if (CHK_EXI(TrInput, IN_BACK))
        item->goalAnimState = STATE_LARA_ONWATER_BACK;
    else if (CHK_EXI(TrInput, IN_LSTEP))
        item->goalAnimState = STATE_LARA_ONWATER_LEFT;
    else if (CHK_EXI(TrInput, IN_RSTEP))
        item->goalAnimState = STATE_LARA_ONWATER_RIGHT;

    if (CHK_EXI(TrInput, IN_JUMP))
    {
        Lara.diveCount++;
        if (Lara.diveCount == DIVE_COUNT)
            item->goalAnimState = STATE_LARA_UNDERWATER_FORWARD;
    }
    else
    {
        Lara.diveCount = 0;
    }
}

void lara_as_surfswim(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_WATER_DEATH;
        return;
    }

    Lara.diveCount = 0;
    if (CHK_EXI(TrInput, IN_LEFT))
        item->pos.yRot -= LARA_SLOW_TURN;
    else if (CHK_EXI(TrInput, IN_RIGHT))
        item->pos.yRot += LARA_SLOW_TURN;

    if (CHK_NOP(TrInput, IN_FORWARD))
        item->goalAnimState = STATE_LARA_ONWATER_STOP;
    if (CHK_EXI(TrInput, IN_JUMP))
        item->goalAnimState = STATE_LARA_ONWATER_STOP;

    LaraClampP(item->fallspeed, SURFMOVE_FRICTION, SURF_MAXSPEED);
}

void lara_as_surfback(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_WATER_DEATH;
        return;
    }

    Lara.diveCount = 0;
    if (CHK_EXI(TrInput, IN_LEFT))
        item->pos.yRot -= LARA_SLOW_TURN / 2;
    else if (CHK_EXI(TrInput, IN_RIGHT))
        item->pos.yRot += LARA_SLOW_TURN / 2;

    if (CHK_NOP(TrInput, IN_BACK))
        item->goalAnimState = STATE_LARA_ONWATER_STOP;

    LaraClampP(item->fallspeed, SURFMOVE_FRICTION, SURF_MAXSPEED);
}

void lara_as_surfleft(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_WATER_DEATH;
        return;
    }

    Lara.diveCount = 0;
    if (CHK_EXI(TrInput, IN_LEFT))
        item->pos.yRot -= LARA_SLOW_TURN / 2;
    else if (CHK_EXI(TrInput, IN_RIGHT))
        item->pos.yRot += LARA_SLOW_TURN / 2;

    if (CHK_NOP(TrInput, IN_LSTEP))
        item->goalAnimState = STATE_LARA_ONWATER_STOP;

    LaraClampP(item->fallspeed, SURFMOVE_FRICTION, SURF_MAXSPEED);
}

void lara_as_surfright(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_WATER_DEATH;
        return;
    }

    Lara.diveCount = 0;
    if (CHK_EXI(TrInput, IN_LEFT))
        item->pos.yRot -= LARA_SLOW_TURN / 2;
    else if (CHK_EXI(TrInput, IN_RIGHT))
        item->pos.yRot += LARA_SLOW_TURN / 2;

    if (CHK_NOP(TrInput, IN_RSTEP))
        item->goalAnimState = STATE_LARA_ONWATER_STOP;

    LaraClampP(item->fallspeed, SURFMOVE_FRICTION, SURF_MAXSPEED);
}