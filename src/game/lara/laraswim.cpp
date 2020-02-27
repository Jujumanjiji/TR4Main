#include "framework.h"
#include "lara.h"
#include "3d_gen.h"
#include "lara_utils.h"
#include "camera.h"
#include "sound.h"
#include "utils.h"

void lara_as_tread(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hit_points <= 0)
    {
        item->state_next = STATE_LARA_WATER_DEATH;
        return;
    }

    if (CHK_ANY(TrInput, IN_ROLL))
    {
        SetAnimationForItem(item, ANIMATION_LARA_UNDERWATER_ROLL_BEGIN, STATE_LARA_UNDERWATER_TURNAROUND, STATE_LARA_UNDERWATER_TURNAROUND);
        return;
    }

    if (CHK_ANY(TrInput, IN_LOOK))
        LookUpDown();

    SwimTurn(item);

    if (CHK_ANY(TrInput, IN_JUMP))
        item->state_next = STATE_LARA_UNDERWATER_FORWARD;

    LaraClampN(item->fallspeed, WATER_FRICTION, 0);

    if (lara.gun_status == LHS_HANDBUSY)
        lara.gun_status = LHS_ARMLESS;
}

void lara_as_swim(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hit_points <= 0)
    {
        item->state_next = STATE_LARA_WATER_DEATH;
        return;
    }

    if (CHK_ANY(TrInput, IN_ROLL))
    {
        SetAnimationForItem(item, ANIMATION_LARA_UNDERWATER_ROLL_BEGIN, STATE_LARA_UNDERWATER_TURNAROUND, STATE_LARA_UNDERWATER_TURNAROUND);
        return;
    }

    SwimTurn(item);
    LaraClampP(item->fallspeed, UW_FRICTION, UW_MAXSPEED);

    if (CHK_NOP(TrInput, IN_JUMP))
        item->state_next = STATE_LARA_UNDERWATER_INERTIA;
}

void lara_as_glide(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hit_points <= 0)
    {
        item->state_next = STATE_LARA_WATER_DEATH;
        return;
    }

    if (CHK_ANY(TrInput, IN_ROLL))
    {
        SetAnimationForItem(item, ANIMATION_LARA_UNDERWATER_ROLL_BEGIN, STATE_LARA_UNDERWATER_TURNAROUND, STATE_LARA_UNDERWATER_TURNAROUND);
        return;
    }

    if (CHK_ANY(TrInput, IN_JUMP))
        item->state_next = STATE_LARA_UNDERWATER_FORWARD;

    SwimTurn(item);
    LaraClampN(item->fallspeed, WATER_FRICTION, 0);

    if (item->fallspeed <= ((UW_MAXSPEED * 2) / 3))
        item->state_next = STATE_LARA_UNDERWATER_STOP;
}

void lara_as_dive(ITEM_INFO* item, COLL_INFO* coll)
{
    if (CHK_ANY(TrInput, IN_FORWARD))
        item->pos.x_rot -= ONE_DEGREE;
}

void lara_as_uwdeath(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.look = FALSE;

    item->fallspeed -= UW_FRICTION;
    if (item->fallspeed <= 0)
        item->fallspeed = 0;

    if (item->pos.x_rot >= -ANGLE(2) && item->pos.x_rot <= ANGLE(2))
        item->pos.x_rot = 0;
    else if (item->pos.x_rot < 0)
        item->pos.x_rot += ANGLE(2);
    else
        item->pos.x_rot -= ANGLE(2);
}

void lara_as_waterroll(ITEM_INFO* item, COLL_INFO* coll)
{
    item->fallspeed = 0;
}