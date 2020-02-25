#include "framework.h"
#include "lara.h"
#include "3dsystem/3d_gen.h"
#include "lara_utils.h"
#include "camera.h"
#include "sound.h"
#include "utils/utils.h"

void lara_as_surftread(ITEM_INFO* item, COLL_INFO* coll)
{
    LaraClampN(item->fallspeed, SURF_FRICTION, 0);

    if (item->hit_points <= 0)
    {
        item->state_next = STATE_LARA_WATER_DEATH;
        return;
    }

    if (CHK_ANY(TrInput, IN_LOOK))
    {
        LookUpDown();
        return;
    }

    if (CHK_ANY(TrInput, IN_LEFT))
        item->pos.y_rot -= LARA_SLOW_TURN;
    else if (CHK_ANY(TrInput, IN_RIGHT))
        item->pos.y_rot += LARA_SLOW_TURN;
    else if (CHK_ANY(TrInput, IN_FORWARD))
        item->state_next = STATE_LARA_ONWATER_FORWARD;
    else if (CHK_ANY(TrInput, IN_BACK))
        item->state_next = STATE_LARA_ONWATER_BACK;
    else if (CHK_ANY(TrInput, IN_LSTEP))
        item->state_next = STATE_LARA_ONWATER_LEFT;
    else if (CHK_ANY(TrInput, IN_RSTEP))
        item->state_next = STATE_LARA_ONWATER_RIGHT;

    if (CHK_ANY(TrInput, IN_JUMP))
    {
        lara.dive_count++;
        if (lara.dive_count == DIVE_COUNT)
            item->state_next = STATE_LARA_UNDERWATER_FORWARD;
    }
    else
    {
        lara.dive_count = 0;
    }
}

void lara_as_surfswim(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hit_points <= 0)
    {
        item->state_next = STATE_LARA_WATER_DEATH;
        return;
    }

    lara.dive_count = 0;
    if (CHK_ANY(TrInput, IN_LEFT))
        item->pos.y_rot -= LARA_SLOW_TURN;
    else if (CHK_ANY(TrInput, IN_RIGHT))
        item->pos.y_rot += LARA_SLOW_TURN;

    if (CHK_NOP(TrInput, IN_FORWARD))
        item->state_next = STATE_LARA_ONWATER_STOP;
    if (CHK_ANY(TrInput, IN_JUMP))
        item->state_next = STATE_LARA_ONWATER_STOP;

    LaraClampP(item->fallspeed, SURFMOVE_FRICTION, SURF_MAXSPEED);
}

void lara_as_surfback(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hit_points <= 0)
    {
        item->state_next = STATE_LARA_WATER_DEATH;
        return;
    }

    lara.dive_count = 0;
    if (CHK_ANY(TrInput, IN_LEFT))
        item->pos.y_rot -= LARA_SLOW_TURN / 2;
    else if (CHK_ANY(TrInput, IN_RIGHT))
        item->pos.y_rot += LARA_SLOW_TURN / 2;

    if (CHK_NOP(TrInput, IN_BACK))
        item->state_next = STATE_LARA_ONWATER_STOP;

    LaraClampP(item->fallspeed, SURFMOVE_FRICTION, SURF_MAXSPEED);
}

void lara_as_surfleft(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hit_points <= 0)
    {
        item->state_next = STATE_LARA_WATER_DEATH;
        return;
    }

    lara.dive_count = 0;
    if (CHK_ANY(TrInput, IN_LEFT))
        item->pos.y_rot -= LARA_SLOW_TURN / 2;
    else if (CHK_ANY(TrInput, IN_RIGHT))
        item->pos.y_rot += LARA_SLOW_TURN / 2;

    if (CHK_NOP(TrInput, IN_LSTEP))
        item->state_next = STATE_LARA_ONWATER_STOP;

    LaraClampP(item->fallspeed, SURFMOVE_FRICTION, SURF_MAXSPEED);
}

void lara_as_surfright(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hit_points <= 0)
    {
        item->state_next = STATE_LARA_WATER_DEATH;
        return;
    }

    lara.dive_count = 0;
    if (CHK_ANY(TrInput, IN_LEFT))
        item->pos.y_rot -= LARA_SLOW_TURN / 2;
    else if (CHK_ANY(TrInput, IN_RIGHT))
        item->pos.y_rot += LARA_SLOW_TURN / 2;

    if (CHK_NOP(TrInput, IN_RSTEP))
        item->state_next = STATE_LARA_ONWATER_STOP;

    LaraClampP(item->fallspeed, SURFMOVE_FRICTION, SURF_MAXSPEED);
}