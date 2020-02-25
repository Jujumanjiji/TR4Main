#include "framework.h"
#include "lara.h"
#include "3dsystem/3d_gen.h"
#include "lara_utils.h"
#include "camera.h"
#include "sound.h"
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
            if (fheight >= (STEPUP_HEIGHT - 1) || fheight <= -(STEPUP_HEIGHT - 1))
            {
                lara.move_angle = item->pos.y_rot;
                coll->bad_pos = NO_HEIGHT;
                coll->bad_neg = -STEPUP_HEIGHT;
                coll->bad_ceiling = 0;
                coll->slopes_are_walls = TRUE;
                coll->radius = LARA_RAD + 2;
                GetLaraCollisionInfo(item, coll);
                if (!TestLaraVault(item, coll))
                    coll->radius = LARA_RAD;
            }
            else
            {
                legacy_lara_as_wade(item, coll);
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
                coll->bad_pos = NO_HEIGHT;
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

        if (CHK_ANY(TrInput, (IN_ROLL | IN_BACK)))
            item->state_next = STATE_LARA_JUMP_ROLL;

        if (CHK_ANY(TrInput, IN_WALK) && lara.gun_status == LHS_ARMLESS)
            item->state_next = STATE_LARA_SWANDIVE_BEGIN;

        if (item->fallspeed > LARA_FASTFALL_SPEED)
            item->state_next = STATE_LARA_FREEFALL;
    }

    if (CHK_ANY(TrInput, IN_LEFT))
    {
        LARA_CLAMPN(lara.turn_rate, LARA_TURN_RATE, LARA_JUMP_TURN);
    }
    else if (CHK_ANY(TrInput, IN_RIGHT))
    {
        LARA_CLAMPP(lara.turn_rate, LARA_TURN_RATE, LARA_JUMP_TURN);
    }
}

void lara_as_fastback(ITEM_INFO* item, COLL_INFO* coll)
{
    item->state_next = STATE_LARA_STOP;

    if (CHK_ANY(TrInput, IN_LEFT))
    {
        LARA_CLAMPN(lara.turn_rate, LARA_TURN_RATE, LARA_MED_TURN);
    }
    else if (CHK_ANY(TrInput, IN_RIGHT))
    {
        LARA_CLAMPP(lara.turn_rate, LARA_TURN_RATE, LARA_MED_TURN);
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

    if (CHK_NOP(TrInput, IN_DRAW))
    {
        if (CHK_NOP(TrInput, IN_BACK) || (CHK_NOP(TrInput, IN_WALK) && lara.water_status == LWS_WADE))
            item->state_next = STATE_LARA_STOP;
        else
            item->state_next = STATE_LARA_WALK_BACK;

        if (CHK_ANY(TrInput, IN_LEFT))
        {
            LARA_CLAMPN(lara.turn_rate, LARA_TURN_RATE, LARA_SLOW_TURN);
        }
        else if (CHK_ANY(TrInput, IN_RIGHT))
        {
            LARA_CLAMPP(lara.turn_rate, LARA_TURN_RATE, LARA_SLOW_TURN);
        }
    }
}

void injector::inject_lara()
{
    /// CONTROL ROUTINES:
    this->inject(legacy_lara_as_walk);
    this->inject(legacy_lara_as_run);
    this->inject(legacy_lara_as_stop);
    this->inject(legacy_lara_as_forwardjump);
    this->inject(legacy_lara_as_fastback);
    this->inject(legacy_lara_as_turnr);
    this->inject(legacy_lara_as_turnl);
    this->inject(legacy_lara_as_death);
    this->inject(legacy_lara_as_fastfall);
    this->inject(legacy_lara_as_hang);
    this->inject(legacy_lara_as_reach);
    this->inject(legacy_lara_as_splat);
    this->inject(legacy_lara_as_tread);
    this->inject(legacy_lara_as_compress);
    this->inject(legacy_lara_as_back);

    /// COLLISION ROUTINES:

}