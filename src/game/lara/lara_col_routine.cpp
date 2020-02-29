#include "framework.h"
#include "lara.h"
#include "3d_gen.h"
#include "camera.h"
#include "collide.h"
#include "items.h"
#include "sound.h"
#include "utils.h"

void lara_default_col(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.y_rot;
    coll->bad_pos = STEPUP_HEIGHT;
    coll->bad_neg = -STEPUP_HEIGHT;
    coll->bad_ceiling = 0;
    coll->slopes_are_pits = TRUE;
    coll->slopes_are_walls = TRUE;
    GetLaraCollisionInfo(item, coll);
}

void lara_col_walk(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.y_rot;
    item->fallspeed = 0;
    item->gravity_status = FALSE;
    coll->bad_pos = STEPUP_HEIGHT;
    coll->bad_neg = -STEPUP_HEIGHT;
    coll->bad_ceiling = 0;
    coll->lava_is_pit = TRUE;
    coll->slopes_are_pits = TRUE;
    coll->slopes_are_walls = TRUE;
    GetLaraCollisionInfo(item, coll);

    if (LaraHitCeiling(item, coll))
        return;

    if (TestLaraVault(item, coll))
        return;
    
    if (LaraDeflectEdge(item, coll))
    {
        if (item->current_frame >= 29 && item->current_frame <= 47)
        {
            item->current_anim = ANIMATION_LARA_END_WALK_RIGHT;
            item->current_frame = anims[item->current_anim].frame_base;
        }
        else if ((item->current_frame >= 22 && item->current_frame <= 28) || (item->current_frame >= 48 && item->current_frame <= 57))
        {
            item->current_anim = ANIMATION_LARA_END_WALK_LEFT;
            item->current_frame = anims[item->current_anim].frame_base;
        }
        else
        {
            LaraCollideStop(item, coll);
        }
    }

    if (LaraFallen(item, coll))
        return;

    // step down
    if (coll->mid_floor > (STEP_L / 2))
    {
        if (coll->front_floor == -NO_HEIGHT || coll->front_floor <= (STEP_L / 2))
        {
            coll->mid_floor = 0;
        }
        else
        {
            if (item->current_frame >= 28 && item->current_frame <= 45)
            {
                item->current_anim = ANIMATION_LARA_WALK_DOWN_LEFT;
                item->current_frame = anims[item->current_anim].frame_base;
            }
            else
            {
                item->current_anim = ANIMATION_LARA_WALK_DOWN_RIGHT;
                item->current_frame = anims[item->current_anim].frame_base;
            }
        }
    }

    // step up
    if (coll->mid_floor >= -STEPUP_HEIGHT && coll->mid_floor < -(STEP_L / 2))
    {
        if (coll->front_floor == -NO_HEIGHT || coll->front_floor < -STEPUP_HEIGHT || coll->front_floor >= -(STEP_L / 2))
        {
            coll->mid_floor = 0;
        }
        else
        {
            if (item->current_frame >= 27 && item->current_frame <= 44)
            {
                item->current_anim = ANIMATION_LARA_WALK_UP_STEP_LEFT;
                item->current_frame = anims[item->current_anim].frame_base;
            }
            else
            {
                item->current_anim = ANIMATION_LARA_WALK_UP_STEP_RIGHT;
                item->current_frame = anims[item->current_anim].frame_base;
            }
        }
    }

    if (TestLaraSlide(item, coll))
        return;

    if (coll->mid_floor != -NO_HEIGHT)
        item->pos.y += coll->mid_floor;
}

void lara_col_run(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.y_rot;
    coll->bad_pos = NO_HEIGHT;
    coll->bad_neg = -STEPUP_HEIGHT;
    coll->bad_ceiling = 0;
    coll->slopes_are_walls = TRUE;
    GetLaraCollisionInfo(item, coll);

    if (LaraHitCeiling(item, coll))
        return;
    if (TestLaraVault(item, coll))
        return;

    if (LaraDeflectEdge(item, coll))
    {
        item->pos.z_rot = 0;

        if (item->current_anim != ANIMATION_LARA_STAY_TO_RUN && TestWall(item, STEP_L, 0, -LARA_SHOULDER_HITE))
        {
            item->state_current = STATE_LARA_SPLAT;
            
            if (item->current_frame >= 0 && item->current_frame <= 9)
            {
                item->current_anim = ANIMATION_LARA_WALL_SMASH_LEFT;
                item->current_frame = anims[item->current_anim].frame_base;
                return;
            }
            
            if (item->current_frame >= 10 && item->current_frame <= 21)
            {
                item->current_anim = ANIMATION_LARA_WALL_SMASH_RIGHT;
                item->current_frame = anims[item->current_anim].frame_base;
                return;
            }
        }
    }

    if (LaraFallen(item, coll))
        return;

    if (coll->mid_floor >= -STEPUP_HEIGHT && coll->mid_floor < -(STEP_L / 2))
    {
        if (coll->front_floor == -NO_HEIGHT || coll->front_floor < -STEPUP_HEIGHT || coll->front_floor >= -(STEP_L / 2))
        {
            coll->mid_floor = 0;
        }
        else
        {
            if (item->current_frame >= 3 && item->current_frame <= 14)
            {
                item->current_anim = ANIMATION_LARA_RUN_UP_STEP_LEFT;
                item->current_frame = anims[item->current_anim].frame_base;
            }
            else
            {
                item->current_anim = ANIMATION_LARA_RUN_UP_STEP_RIGHT;
                item->current_frame = anims[item->current_anim].frame_base;
            }
        }
    }

    if (TestLaraSlide(item, coll))
        return;

    // let lara falldown from a smallish step.
    if (coll->mid_floor >= 50)
    {
        item->pos.y += 50;
        return;
    }

    if (coll->mid_floor != -NO_HEIGHT)
        item->pos.y += coll->mid_floor;
}

void lara_col_stop(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.y_rot;
    coll->bad_pos = STEPUP_HEIGHT;
    coll->bad_neg = -STEPUP_HEIGHT;
    coll->bad_ceiling = 0;
    coll->slopes_are_pits = TRUE;
    coll->slopes_are_walls = TRUE;
    item->fallspeed = 0;
    item->gravity_status = FALSE;
    GetLaraCollisionInfo(item, coll);

    if (LaraHitCeiling(item, coll))
        return;
    if (LaraFallen(item, coll))
        return;
    if (TestLaraSlide(item, coll))
        return;

    ShiftItem(item, coll);

    if (coll->mid_floor != -NO_HEIGHT)
        item->pos.y += coll->mid_floor;
}

void lara_col_forwardjump(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.y_rot;
    coll->bad_pos = NO_HEIGHT;
    coll->bad_neg = -STEPUP_HEIGHT;
    coll->bad_ceiling = BAD_JUMP_CEILING;
    GetLaraCollisionInfo(item, coll);
    LaraDeflectEdgeJump(item, coll);

    if (coll->mid_floor <= 0 && item->fallspeed > 0)
    {
        if (LaraLandedBad(item))
        {
            item->state_next = STATE_LARA_DEATH;
        }
        else
        {
            if (lara.water_status == LWS_WADE)
            {
                item->state_next = STATE_LARA_STOP;
            }
            else
            {
                if (CHK_ANY(TrInput, IN_FORWARD) && CHK_NOP(TrInput, IN_WALK))
                    item->state_next = STATE_LARA_RUN_FORWARD;
                else
                    item->state_next = STATE_LARA_STOP;
            }
        }

        item->fallspeed = 0;
        item->speed = 0;
        item->gravity_status = FALSE;

        if (coll->mid_floor != -NO_HEIGHT)
            item->pos.y += coll->mid_floor;

        AnimateLara(item);
    }
}

void lara_col_fastback(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.y_rot - 0x8000;
    coll->bad_pos = NO_HEIGHT;
    coll->bad_neg = -STEPUP_HEIGHT;
    coll->bad_ceiling = 0;
    coll->slopes_are_pits = TRUE;
    coll->slopes_are_walls = FALSE;
    item->fallspeed = 0;
    item->gravity_status = FALSE;
    GetLaraCollisionInfo(item, coll);

    if (LaraHitCeiling(item, coll))
        return;

    if (coll->mid_floor > 200)
    {
        item->current_anim = ANIMATION_LARA_FREE_FALL_BACK;
        item->current_frame = anims[item->current_anim].frame_base;
        item->state_current = STATE_LARA_FALL_BACKWARD;
        item->state_next = STATE_LARA_FALL_BACKWARD;
        item->fallspeed = 0;
        item->gravity_status = TRUE;
        return;
    }

    if (TestLaraSlide(item, coll))
        return;

    if (LaraDeflectEdge(item, coll))
        LaraCollideStop(item, coll);

    if (coll->mid_floor != -NO_HEIGHT)
        item->pos.y += coll->mid_floor;
}

void lara_col_turn(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.y_rot;
    coll->bad_pos = STEPUP_HEIGHT;
    coll->bad_neg = -STEPUP_HEIGHT;
    coll->bad_ceiling = 0;
    item->fallspeed = 0;
    item->gravity_status = FALSE;
    GetLaraCollisionInfo(item, coll);

    if (coll->mid_floor > 100)
    {
        item->current_anim = ANIMATION_LARA_FREE_FALL_FORWARD;
        item->current_frame = anims[item->current_anim].frame_base;
        item->state_current = STATE_LARA_JUMP_FORWARD;
        item->state_next = STATE_LARA_JUMP_FORWARD;
        item->fallspeed = 0;
        item->gravity_status = TRUE;
        return;
    }

    if (TestLaraSlide(item, coll))
        return;

    if (coll->mid_floor != -NO_HEIGHT)
        item->pos.y += coll->mid_floor;
}

void lara_col_death(ITEM_INFO* item, COLL_INFO* coll)
{
    StopSoundEffect(SFX_LARA_FALL);
    lara.move_angle = item->pos.y_rot;
    coll->bad_pos = STEPUP_HEIGHT;
    coll->bad_neg = -STEPUP_HEIGHT;
    coll->bad_ceiling = 0;
    coll->radius = LARA_RAD * 4;
    GetLaraCollisionInfo(item, coll);

    ShiftItem(item, coll);

    item->hit_points = -1;
    lara.air = -1;

    if (coll->mid_floor != -NO_HEIGHT)
        item->pos.y += coll->mid_floor;
}

void lara_col_fastfall(ITEM_INFO* item, COLL_INFO* coll)
{
    coll->bad_pos = NO_HEIGHT;
    coll->bad_neg = -STEPUP_HEIGHT;
    coll->bad_ceiling = BAD_JUMP_CEILING;
    item->gravity_status = TRUE;
    GetLaraCollisionInfo(item, coll);
    LaraSlideEdgeJump(item, coll);

    if (coll->mid_floor <= 0)
    {
        if (LaraLandedBad(item))
        {
            item->state_next = STATE_LARA_DEATH;
        }
        else
        {
            item->current_anim = ANIMATION_LARA_LANDING_HARD;
            item->current_frame = anims[item->current_anim].frame_base;
            item->state_next = STATE_LARA_STOP;
            item->state_current = STATE_LARA_STOP;
        }

        StopSoundEffect(SFX_LARA_FALL);
        item->fallspeed = 0;
        item->gravity_status = FALSE;
        if (coll->mid_floor != -NO_HEIGHT)
            item->pos.y += coll->mid_floor;
    }
}

void lara_col_hang(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.y_rot;
    item->gravity_status = FALSE;
    item->fallspeed = 0;
    LaraHangTest(item, coll);

    if (item->current_anim == ANIMATION_LARA_HANG_IDLE && item->current_frame == anims[ANIMATION_LARA_HANG_IDLE].frame_base + 21)
    {
        if (CHK_ANY(TrInput, (IN_LEFT | IN_LSTEP)))
        {
            if (CanLaraHangSideways(item, coll, -0x4000))
            {
                item->state_next = STATE_LARA_SHIMMY_LEFT;
                return;
            }
            
            int leftTest = LaraHangLeftCornerTest(item, coll);
            if (leftTest != 0)
            {
                if (leftTest <= 0)
                    item->state_next = STATE_LARA_CLIMB_CORNER_LEFT_INNER;
                else
                    item->state_next = STATE_LARA_CLIMB_CORNER_LEFT_OUTER;
            }
        }
        else if (CHK_ANY(TrInput, (IN_RIGHT | IN_RSTEP)))
        {
            if (CanLaraHangSideways(item, coll, 0x4000))
            {
                item->state_next = STATE_LARA_SHIMMY_RIGHT;
                return;
            }

            int rightTest = LaraHangRightCornerTest(item, coll);
            if (rightTest != 0)
            {
                if (rightTest <= 0)
                    item->state_next = STATE_LARA_CLIMB_CORNER_RIGHT_INNER;
                else
                    item->state_next = STATE_LARA_CLIMB_CORNER_RIGHT_OUTER;
            }
        }
        else if (CHK_ANY(TrInput, IN_FORWARD))
        {
            if (coll->front_floor > -850
            &&  coll->front_floor < -650
            &&  coll->front_floor - coll->front_ceiling >= 0
            &&  coll->front_floor - coll->left_ceiling2 >= 0
            &&  coll->front_floor - coll->right_ceiling2 >= 0)
            {
                int resultFloor = ABS2(coll->left_floor2, coll->right_floor2);
                if (resultFloor < SLOPE_DIF && !coll->hit_static)
                {
                    if (CHK_ANY(TrInput, IN_DUCK))
                    {
                        item->state_next = STATE_LARA_CLIMB_TO_CRAWL;
                        item->state_required = STATE_LARA_CROUCH_IDLE;
                    }
                    else if (CHK_ANY(TrInput, IN_WALK))
                    {
                        item->state_next = STATE_LARA_HANDSTAND;
                    }
                    else
                    {
                        item->state_next = STATE_LARA_GRABBING;
                    }
                }
            }
            else if (lara.climb_status && coll->mid_ceiling <= -STEP_L)
            {
                int resultCeiling = ABS2(coll->left_ceiling2, coll->right_ceiling2);
                if (resultCeiling < SLOPE_DIF)
                {
                    if (LaraTestClimbStance(item, coll))
                        item->state_next = STATE_LARA_LADDER_IDLE;
                    else
                        SetAnimationForItem(item, ANIMATION_LARA_LADDER_UP_HANDS, STATE_LARA_HANG, STATE_LARA_HANG);
                }
            }
        }
        else if (CHK_ANY(TrInput, IN_BACK) && lara.climb_status && coll->mid_floor > 344)
        {
            if (LaraTestClimbStance(item, coll))
                item->state_next = STATE_LARA_LADDER_IDLE;
            else
                SetAnimationForItem(item, ANIMATION_LARA_LADDER_DOWN_HANDS, STATE_LARA_HANG, STATE_LARA_HANG);
        }
    }
}