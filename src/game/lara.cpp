#include "framework.h"
#include "lara.h"
#include "lara_constants.h"
#include "utils/utils.h"

void lara_as_walk(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hit_points <= 0)
    {
        item->state_next = STATE_LARA_STOP;
        return;
    }

    if (lara.is_moving)
    {
        return;
    }

    if (CHK_ANY(TrInput, IN_LEFT))
    {
        LARA_CLAMPN(lara.turn_rate, LARA_TURN_RATE, LARA_SLOW_TURN);
    }
    else if (CHK_ANY(TrInput, IN_RIGHT))
    {
        LARA_CLAMPP(lara.turn_rate, LARA_TURN_RATE, LARA_SLOW_TURN);
    }

    if (CHK_ANY(TrInput, IN_FORWARD))
    {
        if (lara.water_status == LWS_WADE)
        {
            item->state_next = STATE_LARA_WADE_FORWARD;
        }
        else
        {
            if (CHK_NOP(TrInput, IN_WALK))
                item->state_next = STATE_LARA_RUN_FORWARD;
            else
                item->state_next = STATE_LARA_WALK_FORWARD;
        }
    }
    else
    {
        item->state_next = STATE_LARA_STOP;
    }
}

void lara_as_run(ITEM_INFO* item, COLL_INFO* coll)
{
    static bool jump_ok = true;

    if (item->hit_points <= 0)
    {
        item->state_next = STATE_LARA_DEATH;
        return;
    }

    if (CHK_ANY(TrInput, IN_ROLL))
    {
        SetAnimationForItem(item, ANIMATION_LARA_ROLL_BEGIN, STATE_LARA_ROLL_FORWARD, STATE_LARA_STOP, LARA_SPRINT_TO_ROLL_FRAME);
        return;
    }

    if (CHK_ANY(TrInput, IN_SPRINT) && lara.dash_timer)
    {
        item->state_next = STATE_LARA_SPRINT;
        return;
    }

    if (CHK_ANY(TrInput, IN_DUCK) && lara.water_status != LWS_WADE)
    {
        if (lara.gun_status == LHS_ARMLESS
        ||  lara.gun_type == LG_UNARMED
        ||  lara.gun_type == LG_PISTOLS
        ||  lara.gun_type == LG_REVOLVER
        ||  lara.gun_type == LG_UZIS
        ||  lara.gun_type == LG_FLARE)
        {
            item->state_next = STATE_LARA_CROUCH_IDLE;
            return;
        }
    }

    if (CHK_ANY(TrInput, IN_LEFT))
    {
        LARA_CLAMPN(lara.turn_rate, LARA_TURN_RATE, LARA_FAST_TURN);
        LARA_CLAMPN(item->pos.z_rot, LARA_LEAN_RATE, LARA_LEAN_MAX);
    }
    else if (CHK_ANY(TrInput, IN_RIGHT))
    {
        LARA_CLAMPP(lara.turn_rate, LARA_TURN_RATE, LARA_FAST_TURN);
        LARA_CLAMPP(item->pos.z_rot, LARA_LEAN_RATE, LARA_LEAN_MAX);
    }

    switch (item->current_anim)
    {
        case ANIMATION_LARA_STAY_TO_RUN:
            jump_ok = false;
            break;

        case ANIMATION_LARA_RUN:
            if (GetCurrentFrame(item) == 4)
                jump_ok = true;
            else
                jump_ok = false;
            break;

        default:
            jump_ok = true;
            break;
    }

    if (CHK_ANY(TrInput, IN_JUMP) && jump_ok && !item->gravity_status)
    {
        item->state_next = STATE_LARA_JUMP_FORWARD;
    }
    else if (CHK_ANY(TrInput, IN_FORWARD))
    {
        if (lara.water_status == LWS_WADE)
        {
            item->state_next = STATE_LARA_WADE_FORWARD;
        }
        else
        {
            if (CHK_ANY(TrInput, IN_WALK))
                item->state_next = STATE_LARA_WALK_FORWARD;
            else
                item->state_next = STATE_LARA_RUN_FORWARD;
        }
    }
    else
    {
        item->state_next = STATE_LARA_STOP;
    }
}

void injector::inject_lara()
{
    this->inject(legacy_lara_as_walk);
    this->inject(legacy_lara_as_run);
}