#include "framework.h"
#include "lara.h"
#include "3d_gen.h"
#include "camera.h"
#include "sound.h"
#include "utils.h"

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
        LaraClampN(lara.turn_rate, LARA_TURN_RATE, LARA_SLOW_TURN);
    }
    else if (CHK_ANY(TrInput, IN_RIGHT))
    {
        LaraClampP(lara.turn_rate, LARA_TURN_RATE, LARA_SLOW_TURN);
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
        LaraClampN(lara.turn_rate, LARA_TURN_RATE, LARA_FAST_TURN);
        LaraClampN(item->pos.z_rot, LARA_LEAN_RATE, LARA_LEAN_MAX);
    }
    else if (CHK_ANY(TrInput, IN_RIGHT))
    {
        LaraClampP(lara.turn_rate, LARA_TURN_RATE, LARA_FAST_TURN);
        LaraClampP(item->pos.z_rot, LARA_LEAN_RATE, LARA_LEAN_MAX);
    }

    static bool jump_ok = true;
    short frame = GetCurrentFrame(item);
    switch (item->current_anim)
    {
        case ANIMATION_LARA_RUN:
            if (frame == 4)
                jump_ok = true;
            break;

        case ANIMATION_LARA_WALK_FORWARD:
            if (frame == 4)
                jump_ok = true;
            break;

        case ANIMATION_LARA_STAY_TO_RUN:
            jump_ok = false;
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

void lara_as_stop(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hit_points <= 0)
    {
        item->state_next = STATE_LARA_DEATH;
        return;
    }

    /// normal
    if (item->current_anim != ANIMATION_LARA_SPRINT_SLIDE_STAND_RIGHT
    &&  item->current_anim != ANIMATION_LARA_SPRINT_SLIDE_STAND_LEFT)
    {
        StopSoundEffect(SFX_LARA_SLIPPING);
    }

    if (UseSpecialItem(item))
    {
        return;
    }

    if (CHK_ANY(TrInput, IN_ROLL) && lara.water_status != LWS_WADE)
    {
        SetAnimationForItem(item, ANIMATION_LARA_ROLL_BEGIN, STATE_LARA_ROLL_FORWARD, STATE_LARA_STOP, LARA_STAND_TO_ROLL_FRAME);
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

    item->state_next = STATE_LARA_STOP;

    if (CHK_ANY(TrInput, IN_LOOK))
        LookUpDown();

    short fheight = -NO_HEIGHT;
    short rheight = -NO_HEIGHT;
    if (CHK_ANY(TrInput, IN_FORWARD))
        fheight = LaraFloorFront(item, item->pos.y_rot, LARA_RAD + 4);
    else if (CHK_ANY(TrInput, IN_BACK))
        rheight = LaraFloorFront(item, item->pos.y_rot + 32768, LARA_RAD + 4);

    if (CHK_ANY(TrInput, IN_LSTEP))
    {
        short height = LaraFloorFront(item, item->pos.y_rot - 16384, LARA_RAD + 16);
        short ceiling = LaraCeilingFront(item, item->pos.y_rot - 16384, LARA_RAD + 16, LARA_HITE);

        if (height < 128 && height > -128 && height_type != BIG_SLOPE && ceiling <= 0)
            item->state_next = STATE_LARA_WALK_LEFT;
    }
    else if (CHK_ANY(TrInput, IN_RSTEP))
    {
        short height = LaraFloorFront(item, item->pos.y_rot + 16384, LARA_RAD + 16);
        short ceiling = LaraCeilingFront(item, item->pos.y_rot + 16384, LARA_RAD + 16, LARA_HITE);

        if (height < 128 && height > -128 && height_type != BIG_SLOPE && ceiling <= 0)
            item->state_next = STATE_LARA_WALK_RIGHT;
    }
    else if (CHK_ANY(TrInput, IN_LEFT))
    {
        item->state_next = STATE_LARA_TURN_LEFT_SLOW;
    }
    else if (CHK_ANY(TrInput, IN_RIGHT))
    {
        item->state_next = STATE_LARA_TURN_RIGHT_SLOW;
    }

    /// wading
    if (lara.water_status == LWS_WADE)
    {
        if (CHK_ANY(TrInput, IN_JUMP))
        {
            item->state_next = STATE_LARA_JUMP_PREPARE;
        }
        else if (CHK_ANY(TrInput, IN_FORWARD))
        {
            if (fheight < (STEPUP_HEIGHT - 1) && fheight > -(STEPUP_HEIGHT - 1))
            {
                lara_as_wade(item, coll);
            }
            else
            {
                lara.move_angle = item->pos.y_rot;
                coll->bad_pos = NO_BAD_POS;
                coll->bad_neg = -STEPUP_HEIGHT;
                coll->bad_ceiling = 0;
                coll->slopes_are_walls = TRUE;
                coll->radius = LARA_RAD + 2;
                GetLaraCollisionInfo(item, coll);
                if (!TestLaraVault(item, coll))
                    coll->radius = LARA_RAD;
            }
        }
        else if (CHK_ANY(TrInput, IN_BACK) && rheight < (STEPUP_HEIGHT - 1) && rheight > -(STEPUP_HEIGHT - 1))
        {
            lara_as_back(item, coll);
        }
    }
    else
    {
        /// other
        if (CHK_ANY(TrInput, IN_JUMP))
        {
            item->state_next = STATE_LARA_JUMP_PREPARE;
        }
        else if (CHK_ANY(TrInput, IN_FORWARD))
        {
            short height = LaraFloorFront(item, item->pos.y_rot, LARA_RAD + 4);
            short ceiling = LaraCeilingFront(item, item->pos.y_rot, LARA_RAD + 4, LARA_HITE);

            if ((height < 0 || ceiling > 0) && (height_type == BIG_SLOPE || height_type == DIAGONAL))
            {
                item->state_next = STATE_LARA_STOP;
                return;
            }

            if (height >= -STEP_L || fheight >= -STEP_L)
            {
                if (CHK_NOP(TrInput, IN_WALK))
                    lara_as_run(item, coll);
                else
                    lara_as_walk(item, coll);
            }
            else
            {
                lara.move_angle = item->pos.y_rot;
                coll->bad_pos = NO_BAD_POS;
                coll->bad_neg = -STEPUP_HEIGHT;
                coll->bad_ceiling = 0;
                coll->radius = LARA_RAD + 2;
                coll->slopes_are_walls = TRUE;

                GetLaraCollisionInfo(item, coll);
                if (!TestLaraVault(item, coll))
                {
                    coll->radius = LARA_RAD;
                    item->state_next = STATE_LARA_STOP;
                }
            }
        }
        else if (CHK_ANY(TrInput, IN_BACK))
        {
            if (CHK_ANY(TrInput, IN_WALK))
            {
                if ((rheight < (STEPUP_HEIGHT - 1)) && (rheight > -(STEPUP_HEIGHT - 1)) && (height_type != BIG_SLOPE))
                    lara_as_back(item, coll);
            }
            else
            {
                if (rheight > -(STEPUP_HEIGHT - 1))
                    item->state_next = STATE_LARA_RUN_BACK;
            }
        }
    }
}

void lara_as_forwardjump(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->state_next == STATE_LARA_SWANDIVE_BEGIN
    ||  item->state_next == STATE_LARA_REACH)
    {
        item->state_next = STATE_LARA_JUMP_FORWARD;
    }

    if (item->state_next != STATE_LARA_DEATH
    &&  item->state_next != STATE_LARA_STOP
    &&  item->state_next != STATE_LARA_RUN_FORWARD)
    {
        if (CHK_ANY(TrInput, IN_ACTION) && lara.gun_status == LHS_ARMLESS)
            item->state_next = STATE_LARA_REACH;

        if (CHK_ANY(TrInput, IN_BACK) || CHK_ANY(TrInput, IN_ROLL))
            item->state_next = STATE_LARA_JUMP_ROLL;

        if (CHK_ANY(TrInput, IN_WALK) && lara.gun_status == LHS_ARMLESS)
            item->state_next = STATE_LARA_SWANDIVE_BEGIN;

        if (item->fallspeed > LARA_FASTFALL_SPEED)
            item->state_next = STATE_LARA_FREEFALL;
    }

    if (CHK_ANY(TrInput, IN_LEFT))
    {
        LaraClampN(lara.turn_rate, LARA_TURN_RATE, LARA_JUMP_TURN);
    }
    else if (CHK_ANY(TrInput, IN_RIGHT))
    {
        LaraClampP(lara.turn_rate, LARA_TURN_RATE, LARA_JUMP_TURN);
    }
}

void lara_as_fastback(ITEM_INFO* item, COLL_INFO* coll)
{
    item->state_next = STATE_LARA_STOP;

    if (CHK_ANY(TrInput, IN_LEFT))
    {
        LaraClampN(lara.turn_rate, LARA_TURN_RATE, LARA_MED_TURN);
    }
    else if (CHK_ANY(TrInput, IN_RIGHT))
    {
        LaraClampP(lara.turn_rate, LARA_TURN_RATE, LARA_MED_TURN);
    }
}

void lara_as_turnr(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hit_points <= 0)
    {
        item->state_next = STATE_LARA_STOP;
        return;
    }

    lara.turn_rate += LARA_TURN_RATE;
    if (lara.gun_status == LHS_READY && lara.water_status != LWS_WADE)
    {
        item->state_next = STATE_LARA_TURN_FAST;
    }
    else if (lara.turn_rate > LARA_SLOW_TURN)
    {
        if (CHK_NOP(TrInput, IN_WALK) && lara.water_status != LWS_WADE)
            item->state_next = STATE_LARA_TURN_FAST;
        else
            lara.turn_rate = LARA_SLOW_TURN;
    }

    if (CHK_ANY(TrInput, IN_FORWARD))
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
    else if (CHK_NOP(TrInput, IN_RIGHT))
    {
        item->state_next = STATE_LARA_STOP;
    }
}

void lara_as_turnl(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hit_points <= 0)
    {
        item->state_next = STATE_LARA_STOP;
        return;
    }

    lara.turn_rate -= LARA_TURN_RATE;
    if (lara.gun_status == LHS_READY && lara.water_status != LWS_WADE)
    {
        item->state_next = STATE_LARA_TURN_FAST;
    }
    else if (lara.turn_rate < -LARA_SLOW_TURN)
    {
        if (CHK_NOP(TrInput, IN_WALK) && lara.water_status != LWS_WADE)
            item->state_next = STATE_LARA_TURN_FAST;
        else
            lara.turn_rate = -LARA_SLOW_TURN;
    }

    if (CHK_ANY(TrInput, IN_FORWARD))
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
    else if (CHK_NOP(TrInput, IN_LEFT))
    {
        item->state_next = STATE_LARA_STOP;
    }
}

void lara_as_death(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.look = FALSE;
    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = FALSE;

    if (BinocularRange)
    {
        BinocularRange = 0;
        LaserSight = 0;
        AlterFOV(DEFAULT_FOV);
        lara_item->mesh_bits = NO_MESH;
        lara.busy = FALSE;
    }
}

void lara_as_fastfall(ITEM_INFO* item, COLL_INFO* coll)
{
    item->speed = FASTFALL_FRICTION(item);
    if (item->fallspeed == (DAMAGE_START + DAMAGE_LENGTH))
        SoundEffect(SFX_LARA_FALL, &lara_item->pos, 0);
}

void lara_as_hang(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.is_climbing = FALSE;

    if (item->hit_points <= 0)
    {
        item->state_next = STATE_LARA_STOP;
        return;
    }

    if (CHK_ANY(TrInput, IN_LOOK))
        LookUpDown();

    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = FALSE;
    camera.target_angle = CAM_A_HANG;
    camera.target_elevation = CAM_E_HANG;
}

void lara_as_reach(ITEM_INFO* item, COLL_INFO* coll)
{
    camera.target_angle = ANGLE(85);
    if (item->fallspeed > LARA_FASTFALL_SPEED)
        item->state_next = STATE_LARA_FREEFALL;
}

void lara_as_splat(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.look = FALSE;
}

void lara_as_compress(ITEM_INFO* item, COLL_INFO* coll)
{
    if (lara.water_status != LWS_WADE)
    {
        if (CHK_ANY(TrInput, IN_FORWARD) && LaraFloorFront(item, item->pos.y_rot, STEP_L) >= -STEPUP_HEIGHT)
        {
            item->state_next = STATE_LARA_JUMP_FORWARD;
            lara.move_angle = item->pos.y_rot;
        }
        else if (CHK_ANY(TrInput, IN_RIGHT) && LaraFloorFront(item, item->pos.y_rot + 16384, STEP_L) >= -STEPUP_HEIGHT)
        {
            item->state_next = STATE_LARA_JUMP_RIGHT;
            lara.move_angle = item->pos.y_rot + 16384;
        }
        else if (CHK_ANY(TrInput, IN_LEFT) && LaraFloorFront(item, item->pos.y_rot - 16384, STEP_L) >= -STEPUP_HEIGHT)
        {
            item->state_next = STATE_LARA_JUMP_LEFT;
            lara.move_angle = item->pos.y_rot - 16384;
        }
        else if (CHK_ANY(TrInput, IN_BACK) && LaraFloorFront(item, item->pos.y_rot - 32768, STEP_L) >= -STEPUP_HEIGHT)
        {
            item->state_next = STATE_LARA_JUMP_BACK;
            lara.move_angle = item->pos.y_rot - 32768;
        }
    }

    if (item->fallspeed > LARA_FASTFALL_SPEED)
        item->state_next = STATE_LARA_FREEFALL;
}

void lara_as_back(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hit_points <= 0)
    {
        item->state_next = STATE_LARA_STOP;
        return;
    }

    if (!lara.is_moving)
    {
        if (CHK_ANY(TrInput, IN_BACK) && (CHK_ANY(TrInput, IN_WALK) || lara.water_status == LWS_WADE))
            item->state_next = STATE_LARA_WALK_BACK;
        else
            item->state_next = STATE_LARA_STOP;

        if (CHK_ANY(TrInput, IN_LEFT))
        {
            LaraClampN(lara.turn_rate, LARA_TURN_RATE, LARA_SLOW_TURN);
        }
        else if (CHK_ANY(TrInput, IN_RIGHT))
        {
            LaraClampP(lara.turn_rate, LARA_TURN_RATE, LARA_SLOW_TURN);
        }
    }
}

void lara_as_null(ITEM_INFO* item, COLL_INFO* coll)
{
    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = FALSE;
}

void lara_as_fastturn(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hit_points <= 0)
    {
        item->state_next = STATE_LARA_STOP;
        return;
    }

    if (lara.turn_rate < 0)
    {
        lara.turn_rate = -LARA_FAST_TURN;
        if (CHK_NOP(TrInput, IN_LEFT))
            item->state_next = STATE_LARA_STOP;
    }
    else
    {
        lara.turn_rate = LARA_FAST_TURN;
        if (CHK_NOP(TrInput, IN_RIGHT))
            item->state_next = STATE_LARA_STOP;
    }
}

void lara_as_stepright(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.look = FALSE;

    if (item->hit_points <= 0)
    {
        item->state_next = STATE_LARA_STOP;
        return;
    }

    if (CHK_NOP(TrInput, IN_RSTEP))
    {
        item->state_next = STATE_LARA_STOP;
    }

    if (CHK_ANY(TrInput, IN_LEFT))
    {
        LaraClampN(lara.turn_rate, LARA_TURN_RATE, LARA_SLOW_TURN);
    }
    else if (CHK_ANY(TrInput, IN_RIGHT))
    {
        LaraClampP(lara.turn_rate, LARA_TURN_RATE, LARA_SLOW_TURN);
    }
}

void lara_as_stepleft(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.look = FALSE;

    if (item->hit_points <= 0)
    {
        item->state_next = STATE_LARA_STOP;
        return;
    }

    if (CHK_NOP(TrInput, IN_LSTEP))
    {
        item->state_next = STATE_LARA_STOP;
    }

    if (CHK_ANY(TrInput, IN_LEFT))
    {
        LaraClampN(lara.turn_rate, LARA_TURN_RATE, LARA_SLOW_TURN);
    }
    else if (CHK_ANY(TrInput, IN_RIGHT))
    {
        LaraClampP(lara.turn_rate, LARA_TURN_RATE, LARA_SLOW_TURN);
    }
}

void lara_as_slide(ITEM_INFO* item, COLL_INFO* coll)
{
    camera.target_elevation = -ANGLE(45);
    if (CHK_ANY(TrInput, IN_JUMP) && CHK_NOP(TrInput, IN_BACK))
        item->state_next = STATE_LARA_JUMP_FORWARD;
}

void lara_as_backjump(ITEM_INFO* item, COLL_INFO* coll)
{
    camera.target_angle = ANGLE(135);
    if (item->fallspeed > LARA_FASTFALL_SPEED)
        item->state_next = STATE_LARA_FREEFALL;
    else if (item->state_next == STATE_LARA_RUN_FORWARD)
        item->state_next = STATE_LARA_STOP;
    else if ((CHK_ANY(TrInput, IN_FORWARD) || CHK_ANY(TrInput, IN_ROLL)) && item->state_next != STATE_LARA_STOP)
        item->state_next = STATE_LARA_JUMP_ROLL;
}

void lara_as_rightjump(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.look = FALSE;
    if (item->fallspeed > LARA_FASTFALL_SPEED)
        item->state_next = STATE_LARA_FREEFALL;
    else if (CHK_ANY(TrInput, IN_LEFT) && item->state_next != STATE_LARA_STOP)
        item->state_next = STATE_LARA_JUMP_ROLL;
}

void lara_as_leftjump(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.look = FALSE;
    if (item->fallspeed > LARA_FASTFALL_SPEED)
        item->state_next = STATE_LARA_FREEFALL;
    else if (CHK_ANY(TrInput, IN_RIGHT) && item->state_next != STATE_LARA_STOP)
        item->state_next = STATE_LARA_JUMP_ROLL;
}

void lara_as_upjump(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->fallspeed > LARA_FASTFALL_SPEED)
        item->state_next = STATE_LARA_FREEFALL;
}

void lara_as_fallback(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->fallspeed > LARA_FASTFALL_SPEED)
        item->state_next = STATE_LARA_FREEFALL;

    if (CHK_ANY(TrInput, IN_ACTION) && lara.gun_status == LHS_ARMLESS)
        item->state_next = STATE_LARA_REACH;
}

void lara_as_hangleft(ITEM_INFO* item, COLL_INFO* coll)
{
    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = FALSE;
    camera.target_angle = CAM_A_HANG;
    camera.target_elevation = CAM_E_HANG;
    if (CHK_NOP(TrInput, IN_LEFT) || CHK_NOP(TrInput, IN_LSTEP))
        item->state_next = STATE_LARA_HANG;
}

void lara_as_hangright(ITEM_INFO* item, COLL_INFO* coll)
{
    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = FALSE;
    camera.target_angle = CAM_A_HANG;
    camera.target_elevation = CAM_E_HANG;
    if (CHK_NOP(TrInput, IN_RIGHT) || CHK_NOP(TrInput, IN_RSTEP))
        item->state_next = STATE_LARA_HANG;
}

void lara_as_slideback(ITEM_INFO* item, COLL_INFO* coll)
{
    if (CHK_ANY(TrInput, IN_JUMP) && CHK_NOP(TrInput, IN_FORWARD))
        item->state_next = STATE_LARA_JUMP_BACK;
}

void lara_as_pushblock(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.look = FALSE;
    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = FALSE;
    camera.flags = FOLLOW_CENTRE;
    camera.target_angle = ANGLE(45);
    camera.target_elevation = -ANGLE(25);
}

void lara_as_pullblock(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.look = FALSE;
    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = FALSE;
    camera.flags = FOLLOW_CENTRE;
    camera.target_angle = ANGLE(45);
    camera.target_elevation = -ANGLE(25);
}

void lara_as_ppready(ITEM_INFO* item, COLL_INFO* coll)
{
    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = FALSE;
    if (CHK_NOP(TrInput, IN_ACTION))
        item->state_next = STATE_LARA_STOP;
}

void lara_as_pickup(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.look = FALSE;
    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = FALSE;
    camera.target_angle = -ANGLE(130);
    camera.target_elevation = -ANGLE(15);
    camera.target_distance = WALL_L;
}

void lara_as_switchon(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.look = FALSE;
    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = FALSE;
    camera.target_angle = ANGLE(80);
    camera.target_elevation = -ANGLE(25);
    camera.target_distance = WALL_L;
    camera.speed = 6;
}

void lara_as_switchoff(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.look = FALSE;
    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = FALSE;
    camera.target_angle = ANGLE(80);
    camera.target_elevation = -ANGLE(25);
    camera.target_distance = WALL_L;
    camera.speed = 6;
}

void lara_as_usekey(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.look = FALSE;
    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = FALSE;
    camera.target_angle = -ANGLE(80);
    camera.target_elevation = -ANGLE(25);
    camera.target_distance = WALL_L;
}

void lara_as_usepuzzle(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.look = FALSE;
    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = FALSE;
    camera.target_angle = -ANGLE(80);
    camera.target_elevation = -ANGLE(25);
    camera.target_distance = WALL_L;

    if (item->current_frame == anims[item->current_anim].frame_end)
    {
        if (item->reserved_1)
            SetAnimationForItem(item, item->reserved_1, STATE_LARA_MISC_CONTROL);
    }
}

void lara_as_special(ITEM_INFO* item, COLL_INFO* coll)
{
    camera.flags = FOLLOW_CENTRE;
    camera.target_angle = ANGLE(170);
    camera.target_elevation = -ANGLE(25);
}

void lara_as_swandive(ITEM_INFO* item, COLL_INFO* coll)
{
    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = TRUE;
    if (item->fallspeed > LARA_FASTFALL_SPEED && item->state_next != STATE_LARA_UNDERWATER_DIVING)
        item->state_next = STATE_LARA_SWANDIVE_END;
}

void lara_as_fastdive(ITEM_INFO* item, COLL_INFO* coll)
{
    if (CHK_ANY(TrInput, IN_ROLL) && item->state_next == STATE_LARA_SWANDIVE_END)
        item->state_next = STATE_LARA_JUMP_ROLL;
    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = TRUE;
    item->speed = FASTFALL_FRICTION(item);
}

void lara_as_waterout(ITEM_INFO* item, COLL_INFO* coll)
{
    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = FALSE;
    camera.flags = FOLLOW_CENTRE;
}

void lara_as_wade(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hit_points <= 0)
    {
        item->state_next = STATE_LARA_STOP;
        return;
    }

    camera.target_elevation = -ANGLE(22);
    if (CHK_ANY(TrInput, IN_LEFT))
    {
        LaraClampN(lara.turn_rate, LARA_TURN_RATE, LARA_FAST_TURN);
        LaraClampN(item->pos.z_rot, LARA_LEAN_RATE, LARA_LEAN_MAX);
    }
    else if (CHK_ANY(TrInput, IN_RIGHT))
    {
        LaraClampP(lara.turn_rate, LARA_TURN_RATE, LARA_FAST_TURN);
        LaraClampP(item->pos.z_rot, LARA_LEAN_RATE, LARA_LEAN_MAX);
    }

    if (CHK_ANY(TrInput, IN_FORWARD))
    {
        if (lara.water_status == LWS_ABOVEWATER)
            item->state_next = STATE_LARA_RUN_FORWARD;
        else
            item->state_next = STATE_LARA_WADE_FORWARD;
    }
    else
    {
        item->state_next = STATE_LARA_STOP;
    }
}

void lara_as_pickupflare(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.look = FALSE;
    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = FALSE;
    camera.target_angle = ANGLE(130);
    camera.target_elevation = -ANGLE(15);
    camera.target_distance = WALL_L;
    if (item->current_frame == (anims[item->current_anim].frame_end - 1))
        lara.gun_status = LHS_ARMLESS;
}

void lara_as_deathslide(ITEM_INFO* item, COLL_INFO* coll)
{
    camera.target_angle = ANGLE(70);
    TestTriggersCollision(item, coll);

    if (CHK_NOP(TrInput, IN_ACTION))
    {
        item->state_next = STATE_LARA_JUMP_FORWARD;
        lara.move_angle = item->pos.y_rot;
        lara_item->gravity_status = TRUE;
        lara_item->speed = 100;
        lara_item->fallspeed = 40;
        AnimateLara(item);
    }
}

void lara_as_duck(ITEM_INFO* item, COLL_INFO* coll)
{
    coll->enable_spaz = TRUE;
    coll->enable_baddie_push = TRUE;
    lara.is_ducked = TRUE;

    if (item->hit_points <= 0)
    {
        item->state_next = STATE_LARA_CRAWL_IDLE;
        return;
    }

    if (TestLaraSlide(item, coll))
    {
        return;
    }

    if (CHK_ANY(TrInput, IN_LOOK))
    {
        LookUpDown();
    }

    if (CHK_ANY(TrInput, IN_FORWARD) || CHK_ANY(TrInput, IN_BACK) && (CHK_ANY(TrInput, IN_DUCK) || lara.keep_ducked) && lara.gun_status == LHS_ARMLESS && lara.water_status != LWS_WADE)
    {
        if ((item->current_anim == ANIMATION_LARA_CROUCH_IDLE || item->current_anim == ANIMATION_LARA_CROUCH_PREPARE)
        &&   CHK_NOP(TrInput, (IN_FLARE | IN_DRAW))
        &&  (lara.gun_type != LG_FLARE || (lara.flare_age < 900 && lara.flare_age)))
        {
            lara.torso_y_rot = 0;
            lara.torso_x_rot = 0;
            item->state_next = STATE_LARA_CRAWL_IDLE;
        }
    }
}

void lara_as_dash(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hit_points <= 0)
    {
        item->state_next = STATE_LARA_RUN_FORWARD;
        return;
    }

    if (!lara.dash_timer || CHK_NOP(TrInput, IN_SPRINT) || lara.water_status == LWS_WADE)
    {
        item->state_next = STATE_LARA_RUN_FORWARD;
        return;
    }

    lara.dash_timer--;
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
        LaraClampN(lara.turn_rate, LARA_DASH_TURN_RATE, LARA_DASH_TURN_MAX);
        LaraClampN(item->pos.z_rot, LARA_DASH_LEAN_RATE, LARA_DASH_LEAN_MAX);
    }
    else if (CHK_ANY(TrInput, IN_RIGHT))
    {
        LaraClampP(lara.turn_rate, LARA_DASH_TURN_RATE, LARA_DASH_TURN_MAX);
        LaraClampP(item->pos.z_rot, LARA_DASH_LEAN_RATE, LARA_DASH_LEAN_MAX);
    }

    if (CHK_ANY(TrInput, IN_JUMP) && !item->gravity_status)
    {
        item->state_next = STATE_LARA_SPRINT_ROLL;
    }
    else if (CHK_ANY(TrInput, IN_FORWARD))
    {
        if (CHK_ANY(TrInput, IN_WALK))
            item->state_next = STATE_LARA_WALK_FORWARD;
        else
            item->state_next = STATE_LARA_SPRINT;
    }
    else if (CHK_NOP(TrInput, (IN_LEFT | IN_RIGHT)))
    {
        item->state_next = STATE_LARA_STOP;
    }
}

void lara_as_dashdive(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->state_next != STATE_LARA_DEATH
    &&  item->state_next != STATE_LARA_STOP
    &&  item->state_next != STATE_LARA_RUN_FORWARD)
    {
        if (item->fallspeed > LARA_FASTFALL_SPEED)
            item->state_next = STATE_LARA_FREEFALL;
    }
}

void lara_as_hang2(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hit_points <= 0)
    {
        item->state_next = STATE_LARA_STOP;
        return;
    }

    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = FALSE;
    lara.torso_y_rot = 0;
    lara.torso_x_rot = 0;

    if (lara.can_monkey_swing)
    {
        if (CHK_NOP(TrInput, IN_ACTION) || item->hit_points <= 0)
        {
            MonkeySwingFall(item);
            return;
        }

        camera.target_angle = CAM_A_HANG;
        camera.target_elevation = CAM_E_HANG;
    }
    else
    {
        if (CHK_ANY(TrInput, IN_LOOK))
            LookUpDown();
    }
}

void lara_as_monkeyswing(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hit_points <= 0)
    {
        item->state_next = STATE_LARA_MONKEYSWING_IDLE;
        return;
    }

    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = FALSE;
    lara.torso_y_rot = 0;
    lara.torso_x_rot = 0;

    if (CHK_ANY(TrInput, IN_LOOK))
        LookUpDown();

    if (CHK_ANY(TrInput, IN_FORWARD))
        item->state_next = STATE_LARA_MONKEYSWING_FORWARD;
    else
        item->state_next = STATE_LARA_MONKEYSWING_IDLE;

    if (CHK_ANY(TrInput, IN_LEFT))
    {
        LaraClampN(lara.turn_rate, LARA_TURN_RATE, LARA_JUMP_TURN);
    }
    else if (CHK_ANY(TrInput, IN_RIGHT))
    {
        LaraClampP(lara.turn_rate, LARA_TURN_RATE, LARA_JUMP_TURN);
    }
}

void lara_as_monkeyl(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hit_points <= 0)
    {
        item->state_next = STATE_LARA_MONKEYSWING_IDLE;
        return;
    }

    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = FALSE;
    lara.torso_y_rot = 0;
    lara.torso_x_rot = 0;

    if (CHK_ANY(TrInput, IN_LSTEP))
        item->state_next = STATE_LARA_MONKEYSWING_LEFT;
    else
        item->state_next = STATE_LARA_MONKEYSWING_IDLE;

    camera.target_elevation = ANGLE(10);
}

void lara_as_monkeyr(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hit_points <= 0)
    {
        item->state_next = STATE_LARA_MONKEYSWING_IDLE;
        return;
    }

    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = FALSE;
    lara.torso_y_rot = 0;
    lara.torso_x_rot = 0;

    if (CHK_ANY(TrInput, IN_RSTEP))
        item->state_next = STATE_LARA_MONKEYSWING_RIGHT;
    else
        item->state_next = STATE_LARA_MONKEYSWING_IDLE;

    camera.target_elevation = ANGLE(10);
}

void lara_as_monkey180(ITEM_INFO* item, COLL_INFO* coll)
{
    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = FALSE;
    item->state_next = STATE_LARA_MONKEYSWING_IDLE;
}

void lara_as_all4s(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hit_points <= 0)
    {
        item->state_next = STATE_LARA_DEATH;
        return;
    }

    if (TrInput & IN_LOOK)
        LookUpDown();

    lara.torso_y_rot = 0;
    lara.torso_x_rot = 0;
    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = TRUE;

    if (item->current_anim == ANIMATION_LARA_CROUCH_TO_CRAWL_BEGIN)
        lara.gun_status = LHS_HANDBUSY;

    camera.target_elevation = -ANGLE(23);
}

void lara_as_crawl(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hit_points <= 0)
    {
        item->state_next = STATE_LARA_CRAWL_IDLE;
        return;
    }

    if (TrInput & IN_LOOK)
        LookUpDown();

    lara.torso_y_rot = 0;
    lara.torso_x_rot = 0;
    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = TRUE;
    camera.target_elevation = -ANGLE(23);

    if (CHK_ANY(TrInput, IN_FORWARD) && (CHK_ANY(TrInput, IN_DUCK) || lara.keep_ducked) && lara.water_status != LWS_WADE)
    {
        if (CHK_ANY(TrInput, IN_LEFT))
        {
            LaraClampN(lara.turn_rate, LARA_TURN_RATE, LARA_JUMP_TURN);
        }
        else if (CHK_ANY(TrInput, IN_RIGHT))
        {
            LaraClampP(lara.turn_rate, LARA_TURN_RATE, LARA_JUMP_TURN);
        }
    }
    else
    {
        item->state_next = STATE_LARA_CRAWL_IDLE;
    }
}

void lara_as_hangturnl(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hit_points <= 0)
    {
        item->state_next = STATE_LARA_MONKEYSWING_IDLE;
        return;
    }

    camera.target_elevation = ANGLE(10);
    lara.torso_y_rot = 0;
    lara.torso_x_rot = 0;
    item->pos.y_rot -= (ONE_DEGREE + (ONE_DEGREE / 2));

    if (CHK_NOP(TrInput, IN_LEFT))
        item->state_next = STATE_LARA_MONKEYSWING_IDLE;
}

void lara_as_hangturnr(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hit_points <= 0)
    {
        item->state_next = STATE_LARA_MONKEYSWING_IDLE;
        return;
    }

    camera.target_elevation = ANGLE(10);
    lara.torso_y_rot = 0;
    lara.torso_x_rot = 0;
    item->pos.y_rot += (ONE_DEGREE + (ONE_DEGREE / 2));

    if (CHK_NOP(TrInput, IN_RIGHT))
        item->state_next = STATE_LARA_MONKEYSWING_IDLE;
}

void lara_as_all4turnl(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hit_points <= 0)
    {
        item->state_next = STATE_LARA_CRAWL_IDLE;
        return;
    }

    camera.target_elevation = -ANGLE(23);
    lara.torso_y_rot = 0;
    lara.torso_x_rot = 0;
    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = TRUE;
    item->pos.y_rot -= (ONE_DEGREE + (ONE_DEGREE / 2));

    if (CHK_NOP(TrInput, IN_LEFT))
        item->state_next = STATE_LARA_CRAWL_IDLE;
}

void lara_as_all4turnr(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hit_points <= 0)
    {
        item->state_next = STATE_LARA_CRAWL_IDLE;
        return;
    }

    camera.target_elevation = -ANGLE(23);
    lara.torso_y_rot = 0;
    lara.torso_x_rot = 0;
    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = TRUE;
    item->pos.y_rot += (ONE_DEGREE + (ONE_DEGREE / 2));

    if (CHK_NOP(TrInput, IN_RIGHT))
        item->state_next = STATE_LARA_CRAWL_IDLE;
}

void lara_as_crawlb(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hit_points <= 0 || lara.water_status == LWS_WADE)
    {
        item->state_next = STATE_LARA_CRAWL_IDLE;
        return;
    }

    if (CHK_ANY(TrInput, IN_LOOK))
        LookUpDown();

    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = TRUE;
    lara.torso_y_rot = 0;
    lara.torso_x_rot = 0;
    camera.target_elevation = -ANGLE(23);

    if (CHK_NOP(TrInput, IN_BACK))
    {
        item->state_next = STATE_LARA_CRAWL_IDLE;
    }
    else if (CHK_ANY(TrInput, IN_LEFT))
    {
        LaraClampN(lara.turn_rate, LARA_TURN_RATE, LARA_JUMP_TURN);
    }
    else if (CHK_ANY(TrInput, IN_RIGHT))
    {
        LaraClampP(lara.turn_rate, LARA_TURN_RATE, LARA_JUMP_TURN);
    }
}

void lara_as_controlled(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.look = FALSE;
    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = FALSE;

    if (item->current_anim == ANIMATION_LARA_HARP_PLAY && (item->current_frame == anims[ANIMATION_LARA_HARP_PLAY].frame_base + 130))
    {
        S_CDPlay(6, FALSE);
    }

    if (item->current_anim == ANIMATION_LARA_DETONATOR_USE)
    {
        short meshIndex;
        if (item->current_frame == (anims[ANIMATION_LARA_DETONATOR_USE].frame_base + 16))
            meshIndex = objects[MESHSWAP3].mesh_index;
        else if (item->current_frame == (anims[ANIMATION_LARA_DETONATOR_USE].frame_base + 118))
            meshIndex = objects[LARA_SKIN].mesh_index;
        lara.mesh.hand_r = meshes[meshIndex + HAND_R * 2];
    }

    if (item->current_frame == (anims[item->current_anim].frame_end - 1))
    {
        if (item->current_anim == ANIMATION_LARA_HARP_PLAY)
            S_CDPlay(19, FALSE);
        lara.gun_status = LHS_ARMLESS;
        if (UseForcedFixedCamera)
            UseForcedFixedCamera = FALSE;
    }
}

void lara_as_ropel(ITEM_INFO* item, COLL_INFO* coll)
{
    if (CHK_ANY(TrInput, IN_ACTION))
    {
        if (CHK_ANY(TrInput, IN_LEFT))
            lara.rope_y += STEP_L;
        else
            item->state_next = STATE_LARA_ROPE_IDLE;
    }
    else
    {
        FallFromRope(item);
    }
}

void lara_as_roper(ITEM_INFO* item, COLL_INFO* coll)
{
    if (CHK_ANY(TrInput, IN_ACTION))
    {
        if (CHK_ANY(TrInput, IN_RIGHT))
            lara.rope_y -= STEP_L;
        else
            item->state_next = STATE_LARA_ROPE_IDLE;
    }
    else
    {
        FallFromRope(item);
    }
}

void lara_as_controlledl(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.look = FALSE;
    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = FALSE;
}

void lara_as_poleleft(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hit_points <= 0)
    {
        item->state_next = STATE_LARA_POLE_IDLE;
        return;
    }

    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = FALSE;

    if (CHK_NOP(TrInput, IN_LEFT) || CHK_NOP(TrInput, IN_ACTION) || CHK_ANY(TrInput, (IN_FORWARD | IN_BACK)))
        item->state_next = STATE_LARA_POLE_IDLE;
    else
        item->pos.y_rot += STEP_L;
}

void lara_as_poleright(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hit_points <= 0)
    {
        item->state_next = STATE_LARA_POLE_IDLE;
        return;
    }

    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = FALSE;

    if (CHK_NOP(TrInput, IN_RIGHT) || CHK_NOP(TrInput, IN_ACTION) || CHK_ANY(TrInput, (IN_FORWARD | IN_BACK)))
        item->state_next = STATE_LARA_POLE_IDLE;
    else
        item->pos.y_rot -= STEP_L;
}

void lara_as_pulley(ITEM_INFO* item, COLL_INFO* coll)
{
    if (lara.general_ptr == nullptr) // Dont assign it to ITEM_INFO pointer if general_ptr is NULL !
        return;
    ITEM_INFO* gen = (ITEM_INFO*)lara.general_ptr;

    lara.look = FALSE;
    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = FALSE;

    if (CHK_ANY(TrInput, IN_ACTION) && gen->ocb_bits)
        item->state_next = STATE_LARA_PULLEY;
    else
        item->state_next = STATE_LARA_STOP;

    if (item->current_anim == ANIMATION_LARA_PULLEY_PULL && item->current_frame == (anims[ANIMATION_LARA_PULLEY_PULL].frame_base + 44))
    {
        if (gen->ocb_bits && !gen->reserved_2)
        {
            gen->ocb_bits--;

            if (gen->ocb_bits && gen->reserved_3)
            {
                gen->reserved_3 = 0;
                gen->status = FITEM_DEACTIVATED;
            }
            else
            {
                gen->status = FITEM_DEACTIVATED;
                gen->reserved_3 = 1;
                if (gen->reserved_4 >= 0)
                    gen->ocb_bits = abs(gen->reserved_4);
                else
                    gen->reserved_1 = 1;
            }
        }
    }

    if (item->current_anim == ANIMATION_LARA_PULLEY_UNGRAB && item->current_frame == (anims[ANIMATION_LARA_PULLEY_UNGRAB].frame_end - 1))
        lara.gun_status = LHS_ARMLESS;
}

void lara_as_duckl(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hit_points <= 0)
    {
        item->state_next = STATE_LARA_CROUCH_IDLE;
        return;
    }

    if (CHK_NOP(TrInput, IN_LEFT) || CHK_NOP(TrInput, IN_DUCK))
        item->state_next = STATE_LARA_CROUCH_IDLE;

    item->pos.y_rot -= (ONE_DEGREE + (ONE_DEGREE / 2));
}

void lara_as_duckr(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hit_points <= 0)
    {
        item->state_next = STATE_LARA_CROUCH_IDLE;
        return;
    }

    if (CHK_NOP(TrInput, IN_RIGHT) || CHK_NOP(TrInput, IN_DUCK))
        item->state_next = STATE_LARA_CROUCH_IDLE;

    item->pos.y_rot += (ONE_DEGREE + (ONE_DEGREE / 2));
}

void lara_as_extcornerl(ITEM_INFO* item, COLL_INFO* coll)
{
    camera.target_angle = 0x4000;
    camera.target_elevation = -ANGLE(33);
    SetCornerAnim(item, coll, camera.target_angle, item->current_anim == ANIMATION_LARA_HANG_AROUND_LEFT_OUTER_END || item->current_anim == ANIMATION_LARA_LADDER_AROUND_LEFT_OUTER_END);
}

void lara_as_extcornerr(ITEM_INFO* item, COLL_INFO* coll)
{
    camera.target_angle = -0x4000;
    camera.target_elevation = -ANGLE(33);
    SetCornerAnim(item, coll, camera.target_angle, item->current_anim == ANIMATION_LARA_HANG_AROUND_RIGHT_OUTER_END || item->current_anim == ANIMATION_LARA_LADDER_AROUND_RIGHT_OUTER_END);
}

void lara_as_intcornerl(ITEM_INFO* item, COLL_INFO* coll)
{
    camera.target_angle = -0x4000;
    camera.target_elevation = -ANGLE(33);
    SetCornerAnim(item, coll, camera.target_angle, item->current_anim == ANIMATION_LARA_HANG_AROUND_LEFT_INNER_END || item->current_anim == ANIMATION_LARA_LADDER_AROUND_LEFT_INNER_END);
}

void lara_as_intcornerr(ITEM_INFO* item, COLL_INFO* coll)
{
    camera.target_angle = 0x4000;
    camera.target_elevation = -ANGLE(33);
    SetCornerAnim(item, coll, camera.target_angle, item->current_anim == ANIMATION_LARA_HANG_AROUND_RIGHT_INNER_END || item->current_anim == ANIMATION_LARA_LADDER_AROUND_RIGHT_INNER_END);
}

void lara_as_rope(ITEM_INFO* item, COLL_INFO* coll)
{
    if (CHK_NOP(TrInput, IN_ACTION))
        FallFromRope(item);

    if (CHK_ANY(TrInput, IN_LOOK))
        LookUpDown();
}

void lara_as_climbrope(ITEM_INFO* item, COLL_INFO* coll)
{
    if (CHK_ANY(TrInput, IN_ROLL))
    {
        FallFromRope(item);
        return;
    }

    camera.target_angle = ANGLE(30);

    if (item->current_frame == anims[item->current_anim].frame_end)
    {
        item->current_frame = anims[item->current_anim].frame_base;
        lara.rope_segment -= 2;
    }

    if (CHK_NOP(TrInput, IN_FORWARD) || lara.rope_segment <= 4)
        item->state_next = STATE_LARA_ROPE_IDLE;
}

void lara_as_climbroped(ITEM_INFO* item, COLL_INFO* coll)
{
    if (CHK_NOP(TrInput, IN_ACTION))
    {
        FallFromRope(item);
        return;
    }

    camera.target_angle = ANGLE(30);

    if (lara.rope_count && !lara.rope_flag)
    {
        lara.rope_count--;
        lara.rope_offset += lara.rope_downvel;
        lara.rope_flag = 1;
        return;
    }
    else if (!lara.rope_flag)
    {
        ROPE_STRUCT* rope = &ropes[lara.rope_ptr];
        lara.rope_offset = 0;
        lara.rope_downvel = (DWORD)(rope->mesh[lara.rope_segment + 1].y - rope->mesh[lara.rope_segment].y) >> 17;
        lara.rope_count = 0;
        lara.rope_offset += lara.rope_downvel;
        lara.rope_flag = 1;
        return;
    }

    if (item->current_anim == ANIMATION_LARA_ROPE_DOWN && item->current_frame == anims[ANIMATION_LARA_ROPE_DOWN].frame_end)
    {
        item->current_frame = anims[ANIMATION_LARA_ROPE_DOWN].frame_base;
        lara.rope_flag = 0;
        lara.rope_segment++;
        lara.rope_offset = 0;
    }

    if (CHK_NOP(TrInput, IN_BACK) || lara.rope_segment >= 21)
        item->state_next = STATE_LARA_ROPE_IDLE;
}