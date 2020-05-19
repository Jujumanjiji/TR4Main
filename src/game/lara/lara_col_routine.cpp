#include "framework.h"
#include "lara.h"
#include "3d_gen.h"
#include "draw.h"
#include "camera.h"
#include "collide.h"
#include "control.h"
#include "items.h"
#include "sound.h"
#include "utils.h"

void lara_default_col(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.yRot;
    coll->bad_pos = STEPUP_HEIGHT;
    coll->bad_neg = -STEPUP_HEIGHT;
    coll->bad_ceiling = 0;
    coll->slopes_are_pits = TRUE;
    coll->slopes_are_walls = TRUE;
    GetLaraCollisionInfo(item, coll);
}

void lara_col_walk(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.yRot;
    coll->bad_pos = STEPUP_HEIGHT;
    coll->bad_neg = -STEPUP_HEIGHT;
    coll->bad_ceiling = 0;
    coll->lava_is_pit = TRUE;
    coll->slopes_are_pits = TRUE;
    coll->slopes_are_walls = TRUE;
    item->fallspeed = 0;
    item->gravityStatus = FALSE;
    GetLaraCollisionInfo(item, coll);

    if (LaraHitCeiling(item, coll))
        return;
    if (TestLaraVault(item, coll))
        return;
    
    if (LaraDeflectEdge(item, coll))
    {
        if (item->frameNumber >= 29 && item->frameNumber <= 47)
        {
            item->animNumber = ANIMATION_LARA_END_WALK_LEFT;
            item->frameNumber = Anims[item->animNumber].frameBase;
        }
        else if ((item->frameNumber >= 22 && item->frameNumber <= 28) || (item->frameNumber >= 48 && item->frameNumber <= 57))
        {
            item->animNumber = ANIMATION_LARA_END_WALK_RIGHT;
            item->frameNumber = Anims[item->animNumber].frameBase;
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
            if (item->frameNumber >= 28 && item->frameNumber <= 45)
            {
                item->animNumber = ANIMATION_LARA_WALK_DOWN_LEFT;
                item->frameNumber = Anims[item->animNumber].frameBase;
            }
            else
            {
                item->animNumber = ANIMATION_LARA_WALK_DOWN_RIGHT;
                item->frameNumber = Anims[item->animNumber].frameBase;
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
            if (item->frameNumber >= 27 && item->frameNumber <= 44)
            {
                item->animNumber = ANIMATION_LARA_WALK_UP_STEP_LEFT;
                item->frameNumber = Anims[item->animNumber].frameBase;
            }
            else
            {
                item->animNumber = ANIMATION_LARA_WALK_UP_STEP_RIGHT;
                item->frameNumber = Anims[item->animNumber].frameBase;
            }
        }
    }

    if (TestLaraSlide(item, coll))
        return;

    if (coll->mid_floor != -NO_HEIGHT)
        item->pos.yPos += coll->mid_floor;
}

void lara_col_run(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.yRot;
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
        item->pos.zRot = 0;

        if (item->animNumber != ANIMATION_LARA_STAY_TO_RUN && TestWall(item, STEP_L, 0, -LARA_SHOULDER_HITE))
        {
            item->currentAnimState = STATE_LARA_SPLAT;
            
            if (item->frameNumber >= 0 && item->frameNumber <= 9)
            {
                item->animNumber = ANIMATION_LARA_WALL_SMASH_LEFT;
                item->frameNumber = Anims[item->animNumber].frameBase;
                return;
            }
            
            if (item->frameNumber >= 10 && item->frameNumber <= 21)
            {
                item->animNumber = ANIMATION_LARA_WALL_SMASH_RIGHT;
                item->frameNumber = Anims[item->animNumber].frameBase;
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
            if (item->frameNumber >= 3 && item->frameNumber <= 14)
            {
                item->animNumber = ANIMATION_LARA_RUN_UP_STEP_LEFT;
                item->frameNumber = Anims[item->animNumber].frameBase;
            }
            else
            {
                item->animNumber = ANIMATION_LARA_RUN_UP_STEP_RIGHT;
                item->frameNumber = Anims[item->animNumber].frameBase;
            }
        }
    }

    if (TestLaraSlide(item, coll))
        return;

    // let lara falldown from a smallish step.
    if (coll->mid_floor >= 50)
    {
        item->pos.yPos += 50;
        return;
    }

    if (coll->mid_floor != -NO_HEIGHT)
        item->pos.yPos += coll->mid_floor;
}

void lara_col_stop(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.yRot;
    coll->bad_pos = STEPUP_HEIGHT;
    coll->bad_neg = -STEPUP_HEIGHT;
    coll->bad_ceiling = 0;
    coll->slopes_are_pits = TRUE;
    coll->slopes_are_walls = TRUE;
    item->fallspeed = 0;
    item->gravityStatus = FALSE;
    GetLaraCollisionInfo(item, coll);

    if (LaraHitCeiling(item, coll))
        return;
    if (LaraFallen(item, coll))
        return;
    if (TestLaraSlide(item, coll))
        return;

    ShiftItem(item, coll);

    if (coll->mid_floor != -NO_HEIGHT)
        item->pos.yPos += coll->mid_floor;
}

void lara_col_forwardjump(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.yRot;
    coll->bad_pos = NO_HEIGHT;
    coll->bad_neg = -STEPUP_HEIGHT;
    coll->bad_ceiling = BAD_JUMP_CEILING;
    GetLaraCollisionInfo(item, coll);
    LaraDeflectEdgeJump(item, coll);

    if (coll->mid_floor <= 0 && item->fallspeed > 0)
    {
        if (LaraLandedBad(item))
        {
            item->goalAnimState = STATE_LARA_DEATH;
        }
        else
        {
            long water_level = -lara.water_surface_dist;
            if (lara.water_status == LWS_WADE && water_level > WADE_DEPTH)
            {
                item->goalAnimState = STATE_LARA_IDLE;
            }
            else
            {
                if (CHK_EXI(TrInput, IN_FORWARD) && CHK_NOP(TrInput, IN_WALK))
                    item->goalAnimState = STATE_LARA_RUN_FORWARD;
                else
                    item->goalAnimState = STATE_LARA_IDLE;
            }
        }

        item->fallspeed = 0;
        item->speed = 0;
        item->gravityStatus = FALSE;

        if (coll->mid_floor != -NO_HEIGHT)
            item->pos.yPos += coll->mid_floor;

        AnimateLara(item);
    }
}

void lara_col_fastback(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.yRot - 0x8000;
    coll->bad_pos = NO_HEIGHT;
    coll->bad_neg = -STEPUP_HEIGHT;
    coll->bad_ceiling = 0;
    coll->slopes_are_pits = TRUE;
    coll->slopes_are_walls = FALSE;
    item->fallspeed = 0;
    item->gravityStatus = FALSE;
    GetLaraCollisionInfo(item, coll);

    if (LaraHitCeiling(item, coll))
        return;

    if (coll->mid_floor > 200)
    {
        item->animNumber = ANIMATION_LARA_FREE_FALL_BACK;
        item->frameNumber = Anims[item->animNumber].frameBase;
        item->currentAnimState = STATE_LARA_FALL_BACKWARD;
        item->goalAnimState = STATE_LARA_FALL_BACKWARD;
        item->fallspeed = 0;
        item->gravityStatus = TRUE;
        return;
    }

    if (TestLaraSlide(item, coll))
        return;

    if (LaraDeflectEdge(item, coll))
        LaraCollideStop(item, coll);

    if (coll->mid_floor != -NO_HEIGHT)
        item->pos.yPos += coll->mid_floor;
}

void lara_col_turn(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.yRot;
    coll->bad_pos = STEPUP_HEIGHT;
    coll->bad_neg = -STEPUP_HEIGHT;
    coll->bad_ceiling = 0;
    item->fallspeed = 0;
    item->gravityStatus = FALSE;
    GetLaraCollisionInfo(item, coll);

    if (coll->mid_floor > 100)
    {
        item->animNumber = ANIMATION_LARA_FREE_FALL_FORWARD;
        item->frameNumber = Anims[item->animNumber].frameBase;
        item->currentAnimState = STATE_LARA_JUMP_FORWARD;
        item->goalAnimState = STATE_LARA_JUMP_FORWARD;
        item->fallspeed = 0;
        item->gravityStatus = TRUE;
        return;
    }

    if (TestLaraSlide(item, coll))
        return;

    if (coll->mid_floor != -NO_HEIGHT)
        item->pos.yPos += coll->mid_floor;
}

void lara_col_death(ITEM_INFO* item, COLL_INFO* coll)
{
    StopSoundEffect(SFX_LARA_FALL);
    lara.move_angle = item->pos.yRot;
    coll->bad_pos = STEPUP_HEIGHT;
    coll->bad_neg = -STEPUP_HEIGHT;
    coll->bad_ceiling = 0;
    coll->radius = LARA_RAD * 4;
    GetLaraCollisionInfo(item, coll);

    ShiftItem(item, coll);

    item->hitPoints = -1;
    lara.air = -1;

    if (coll->mid_floor != -NO_HEIGHT)
        item->pos.yPos += coll->mid_floor;
}

void lara_col_fastfall(ITEM_INFO* item, COLL_INFO* coll)
{
    coll->bad_pos = NO_HEIGHT;
    coll->bad_neg = -STEPUP_HEIGHT;
    coll->bad_ceiling = BAD_JUMP_CEILING;
    item->gravityStatus = TRUE;
    GetLaraCollisionInfo(item, coll);
    LaraSlideEdgeJump(item, coll);

    if (coll->mid_floor <= 0)
    {
        if (LaraLandedBad(item))
        {
            item->goalAnimState = STATE_LARA_DEATH;
        }
        else
        {
            item->animNumber = ANIMATION_LARA_LANDING_HARD;
            item->frameNumber = Anims[item->animNumber].frameBase;
            item->goalAnimState = STATE_LARA_IDLE;
            item->currentAnimState = STATE_LARA_IDLE;
        }

        StopSoundEffect(SFX_LARA_FALL);
        item->fallspeed = 0;
        item->gravityStatus = FALSE;
        if (coll->mid_floor != -NO_HEIGHT)
            item->pos.yPos += coll->mid_floor;
    }
}

void lara_col_hang(ITEM_INFO* item, COLL_INFO* coll)
{
    item->fallspeed = 0;
    item->gravityStatus = FALSE;

    if (item->animNumber == ANIMATION_LARA_HANG_IDLE && item->frameNumber == Anims[ANIMATION_LARA_HANG_IDLE].frameBase + 21)
    {
        if (CHK_EXI(TrInput, (IN_LEFT | IN_LSTEP)))
        {
            if (CanLaraHangSideways(item, coll, -ANGLE(90)))
            {
                item->goalAnimState = STATE_LARA_SHIMMY_LEFT;
                return;
            }
            
            int left_test = LaraHangLeftCornerTest(item, coll);
            if (left_test != 0)
            {
                if (left_test <= 0)
                    item->goalAnimState = STATE_LARA_CLIMB_CORNER_LEFT_INNER;
                else
                    item->goalAnimState = STATE_LARA_CLIMB_CORNER_LEFT_OUTER;
                return;
            }
        }
        else if (CHK_EXI(TrInput, (IN_RIGHT | IN_RSTEP)))
        {
            if (CanLaraHangSideways(item, coll, ANGLE(90)))
            {
                item->goalAnimState = STATE_LARA_SHIMMY_RIGHT;
                return;
            }

            int right_test = LaraHangRightCornerTest(item, coll);
            if (right_test != 0)
            {
                if (right_test <= 0)
                    item->goalAnimState = STATE_LARA_CLIMB_CORNER_RIGHT_INNER;
                else
                    item->goalAnimState = STATE_LARA_CLIMB_CORNER_RIGHT_OUTER;
                return;
            }
        }
    }

    lara.move_angle = item->pos.yRot;
    LaraHangTest(item, coll);

    // TODO: enable a customize/json to disable the delay to climb up !
    if (item->animNumber == ANIMATION_LARA_HANG_IDLE && item->frameNumber == Anims[ANIMATION_LARA_HANG_IDLE].frameBase + 21)
    {
        if (CHK_EXI(TrInput, IN_FORWARD))
        {
            if (coll->front_floor > -850
            &&  coll->front_floor < -650
            &&  coll->front_floor - coll->front_ceiling >= 0
            &&  coll->front_floor - coll->left_ceiling2 >= 0
            &&  coll->front_floor - coll->right_ceiling2 >= 0)
            {
                if (abs(coll->left_floor2 - coll->right_floor2) < SLOPE_DIF && !coll->hit_static)
                {
                    if (CHK_EXI(TrInput, IN_DUCK))
                    {
                        item->goalAnimState = STATE_LARA_CLIMB_TO_CRAWL;
                        item->requiredAnimState = STATE_LARA_CROUCH_IDLE;
                    }
                    else if (CHK_EXI(TrInput, IN_WALK))
                    {
                        item->goalAnimState = STATE_LARA_HANDSTAND;
                    }
                    else
                    {
                        item->goalAnimState = STATE_LARA_GRABBING;
                    }
                }
            }
            else if (lara.climb_status && coll->mid_ceiling <= -STEP_L)
            {
                if (abs(coll->left_ceiling2 - coll->right_ceiling2) < SLOPE_DIF)
                {
                    if (LaraTestClimbStance(item, coll))
                        item->goalAnimState = STATE_LARA_LADDER_IDLE;
                    else
                        SetAnimationForItemAS(item, ANIMATION_LARA_LADDER_UP_HANDS, STATE_LARA_HANG);
                }
            }
        }
        else if (CHK_EXI(TrInput, IN_BACK) && lara.climb_status && coll->mid_floor > 344)
        {
            if (LaraTestClimbStance(item, coll))
                item->goalAnimState = STATE_LARA_LADDER_IDLE;
            else
                SetAnimationForItemAS(item, ANIMATION_LARA_LADDER_DOWN_HANDS, STATE_LARA_HANG);
        }
    }
}

void lara_col_reach(ITEM_INFO* item, COLL_INFO* coll)
{
    if (lara.rope_ptr == NO_ROPE)
        item->gravityStatus = TRUE;

    lara.move_angle = item->pos.yRot;
    coll->bad_pos = NO_HEIGHT;
    coll->bad_neg = 0;
    coll->bad_ceiling = BAD_JUMP_CEILING;
    GetLaraCollisionInfo(item, coll);

    if (LaraTestHangJump(item, coll))
        return;

    LaraSlideEdgeJump(item, coll);
    GetLaraCollisionInfo(item, coll);
    ShiftItem(item, coll);

    if (item->fallspeed > 0 && coll->mid_floor <= 0)
    {
        if (LaraLandedBad(item))
            item->goalAnimState = STATE_LARA_DEATH;
        else
            item->goalAnimState = STATE_LARA_IDLE;
        item->fallspeed = 0;
        item->gravityStatus = FALSE;
        if (coll->mid_floor != -NO_HEIGHT)
            item->pos.yPos += coll->mid_floor;
    }
}

void lara_col_splat(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.yRot;
    coll->bad_pos = STEPUP_HEIGHT;
    coll->bad_neg = -STEPUP_HEIGHT;
    coll->bad_ceiling = 0;
    coll->slopes_are_pits = TRUE;
    coll->slopes_are_walls = TRUE;
    GetLaraCollisionInfo(item, coll);
    ShiftItem(item, coll);

    if (coll->mid_floor >= -STEP_L && coll->mid_floor <= STEP_L)
        item->pos.yPos += coll->mid_floor;
}

void lara_col_tread(ITEM_INFO* item, COLL_INFO* coll)
{
    LaraSwimCollision(item, coll);
}

void lara_col_compress(ITEM_INFO* item, COLL_INFO* coll)
{
    coll->bad_pos = NO_HEIGHT;
    coll->bad_neg = -NO_HEIGHT;
    coll->bad_ceiling = 0;
    item->fallspeed = 0;
    item->gravityStatus = FALSE;
    GetLaraCollisionInfo(item, coll);

    if (LaraFallen(item, coll))
        return;

    if (coll->mid_ceiling > -100)
    {
        item->pos.xPos = coll->old.x;
        item->pos.yPos = coll->old.y;
        item->pos.zPos = coll->old.z;
        item->speed = 0;
        item->fallspeed = 0;
        item->gravityStatus = FALSE;
        SetAnimationForItemAS(item, ANIMATION_LARA_STAY_SOLID, STATE_LARA_IDLE);
    }

    if (coll->mid_floor > -STEP_L && coll->mid_floor < STEP_L)
        item->pos.yPos += coll->mid_floor;
}

void lara_col_back(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.yRot - 0x8000;
    long water_level = -lara.water_surface_dist;
    if (lara.water_status == LWS_WADE && water_level > WADE_DEPTH)
        coll->bad_pos = NO_HEIGHT;
    else
        coll->bad_pos = STEPUP_HEIGHT;
    coll->bad_neg = -STEPUP_HEIGHT;
    coll->bad_ceiling = 0;
    coll->slopes_are_pits = TRUE;
    coll->slopes_are_walls = TRUE;
    item->fallspeed = 0;
    item->gravityStatus = FALSE;
    GetLaraCollisionInfo(item, coll);

    if (LaraHitCeiling(item, coll))
        return;
    
    if (LaraDeflectEdge(item, coll))
        LaraCollideStop(item, coll);

    if (LaraFallen(item, coll))
        return;

    if (coll->mid_floor > (STEP_L / 2) && coll->mid_floor < STEPUP_HEIGHT)
    {
        if (item->frameNumber >= 964 && item->frameNumber <= 993)
        {
            item->animNumber = ANIMATION_LARA_WALK_DOWN_BACK_RIGHT;
            item->frameNumber = Anims[item->animNumber].frameBase;
        }
        else
        {
            item->animNumber = ANIMATION_LARA_WALK_DOWN_BACK_LEFT;
            item->frameNumber = Anims[item->animNumber].frameBase;
        }
    }

    if (TestLaraSlide(item, coll))
        return;

    if (coll->mid_floor != -NO_HEIGHT)
        item->pos.yPos += coll->mid_floor;
}

void lara_col_swim(ITEM_INFO* item, COLL_INFO* coll)
{
    LaraSwimCollision(item, coll);
}

void lara_col_glide(ITEM_INFO* item, COLL_INFO* coll)
{
    LaraSwimCollision(item, coll);
}

void lara_col_step(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->currentAnimState == STATE_LARA_WALK_RIGHT)
        lara.move_angle = item->pos.yRot + 0x4000;
    else
        lara.move_angle = item->pos.yRot - 0x4000;
    
    long water_level = -lara.water_surface_dist;
    if (lara.water_status == LWS_WADE && water_level > WADE_DEPTH)
        coll->bad_pos = NO_HEIGHT;
    else
        coll->bad_pos = (STEP_L / 2);
    coll->bad_neg = -(STEP_L / 2);
    coll->bad_ceiling = 0;
    coll->slopes_are_pits = TRUE;
    coll->slopes_are_walls = TRUE;
    item->fallspeed = 0;
    item->gravityStatus = FALSE;
    GetLaraCollisionInfo(item, coll);

    if (LaraHitCeiling(item, coll))
        return;

    if (LaraDeflectEdge(item, coll))
        LaraCollideStop(item, coll);

    if (LaraFallen(item, coll))
        return;
    if (TestLaraSlide(item, coll))
        return;

    if (coll->mid_floor != -NO_HEIGHT)
        item->pos.yPos += coll->mid_floor;
}

void lara_col_roll(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.yRot;
    coll->bad_pos = NO_HEIGHT;
    coll->bad_neg = -STEPUP_HEIGHT;
    coll->bad_ceiling = 0;
    coll->slopes_are_walls = TRUE;
    item->fallspeed = 0;
    item->gravityStatus = FALSE;
    GetLaraCollisionInfo(item, coll);

    if (LaraHitCeiling(item, coll))
        return;
    if (TestLaraSlide(item, coll))
        return;
    if (LaraFallen(item, coll))
        return;

    ShiftItem(item, coll);

    if (coll->mid_floor != -NO_HEIGHT)
        item->pos.yPos += coll->mid_floor;
}

void lara_col_slide(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.yRot;
    LaraSlideSlope(item, coll);
}

void lara_col_backjump(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.yRot - 0x8000;
    LaraCollideJump(item, coll);
}

void lara_col_rightjump(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.yRot + 0x4000;
    LaraCollideJump(item, coll);
}

void lara_col_leftjump(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.yRot - 0x4000;
    LaraCollideJump(item, coll);
}

void lara_col_upjump(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_IDLE;
        return;
    }

    lara.move_angle = item->pos.yRot;
    coll->bad_pos = NO_HEIGHT;
    coll->bad_neg = -STEPUP_HEIGHT;
    coll->bad_ceiling = BAD_JUMP_CEILING;
    coll->facing = (item->speed < 0) ? lara.move_angle - 0x8000 : lara.move_angle;
    coll->hit_ceiling = FALSE;
    GetCollisionInfo(coll, item->pos.xPos, item->pos.yPos, item->pos.zPos, item->roomNumber, LARA_JUMPUP_HITE);

    if (LaraTestHangJumpUp(item, coll))
        return;

    ShiftItem(item, coll);

    if (coll->coll_type == COLL_CLAMP || coll->coll_type == COLL_TOP || coll->coll_type == COLL_TOPFRONT || coll->hit_ceiling)
        item->fallspeed = 1;

    if (coll->coll_type)
    {
        item->speed = (item->speed > 0) ? 2 : -2;
    }
    else if (item->fallspeed < -70)
    {
        if (CHK_EXI(TrInput, IN_FORWARD) && item->speed < 5)
            item->speed++;
        else if (CHK_EXI(TrInput, IN_BACK) && item->speed > -5)
            item->speed -= 2;
    }

    if (item->fallspeed > 0 && coll->mid_floor <= 0)
    {
        if (LaraLandedBad(item))
            item->goalAnimState = STATE_LARA_DEATH;
        else
            item->goalAnimState = STATE_LARA_IDLE;

        item->fallspeed = 0;
        item->gravityStatus = FALSE;
        
        if (coll->mid_floor != -NO_HEIGHT)
            item->pos.yPos += coll->mid_floor;
    }
}

void lara_col_fallback(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.yRot - 0x8000;
    coll->bad_pos = NO_HEIGHT;
    coll->bad_neg = -STEPUP_HEIGHT;
    coll->bad_ceiling = BAD_JUMP_CEILING;
    GetLaraCollisionInfo(item, coll);
    LaraDeflectEdgeJump(item, coll);

    if (item->fallspeed > 0 && coll->mid_floor <= 0)
    {
        if (LaraLandedBad(item))
            item->goalAnimState = STATE_LARA_DEATH;
        else
            item->goalAnimState = STATE_LARA_IDLE;

        item->fallspeed = 0;
        item->gravityStatus = FALSE;

        if (coll->mid_floor != -NO_HEIGHT)
            item->pos.yPos += coll->mid_floor;
    }
}

void lara_col_hangleft(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.yRot - 0x4000;
    coll->radius = LARA_RAD + 2;
    LaraHangTest(item, coll);
    lara.move_angle = item->pos.yRot - 0x4000;
}

void lara_col_hangright(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.yRot + 0x4000;
    coll->radius = LARA_RAD + 2;
    LaraHangTest(item, coll);
    lara.move_angle = item->pos.yRot + 0x4000;
}

void lara_col_slideback(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.yRot - 0x8000;
    LaraSlideSlope(item, coll);
}

void lara_col_surftread(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->goalAnimState == STATE_LARA_UNDERWATER_FORWARD)
    {
        SetAnimationForItemASN(item, ANIMATION_LARA_FREE_FALL_TO_UNDERWATER_ALTERNATE, STATE_LARA_UNDERWATER_DIVING, false);
        item->pos.xRot = -ANGLE(45);
        item->fallspeed = 80;
        lara.water_status = LWS_UNDERWATER;
    }

    lara.move_angle = item->pos.yRot;
    LaraSurfaceCollision(item, coll);
}

void lara_col_surfswim(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.yRot;
    coll->bad_neg = -STEPUP_HEIGHT;
    LaraSurfaceCollision(item, coll);
    LaraTestWaterClimbOut(item, coll);
}

void lara_col_dive(ITEM_INFO* item, COLL_INFO* coll)
{
    LaraSwimCollision(item, coll);
}

void lara_col_uwdeath(ITEM_INFO* item, COLL_INFO* coll)
{
    item->hitPoints = -1;
    lara.air = -1;
    lara.gun_status = LHS_HANDBUSY;

    int water_level = GetWaterHeight(item->pos.xPos, item->pos.yPos, item->pos.zPos, item->roomNumber);
    if (water_level != -NO_HEIGHT && water_level < (item->pos.yPos - SURF_RADIUS))
        item->pos.yPos -= 5;

    LaraSwimCollision(item, coll);
}

void lara_col_roll2(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.yRot - 0x8000;
    coll->bad_pos = NO_HEIGHT;
    coll->bad_neg = -STEPUP_HEIGHT;
    coll->bad_ceiling = 0;
    coll->slopes_are_walls = TRUE;
    item->fallspeed = 0;
    item->gravityStatus = FALSE;
    GetLaraCollisionInfo(item, coll);

    if (LaraHitCeiling(item, coll))
        return;
    if (TestLaraSlide(item, coll))
        return;

    if (coll->mid_floor > 200)
    {
        SetAnimationForItemAS(item, ANIMATION_LARA_FREE_FALL_BACK, STATE_LARA_FALL_BACKWARD);
        item->fallspeed = 0;
        item->gravityStatus = TRUE;
        return;
    }

    ShiftItem(item, coll);

    if (coll->mid_floor != -NO_HEIGHT)
        item->pos.yPos += coll->mid_floor;
}

void lara_col_surfback(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.yRot - 0x8000;
    LaraSurfaceCollision(item, coll);
}

void lara_col_surfleft(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.yRot - 0x4000;
    LaraSurfaceCollision(item, coll);
}

void lara_col_surfright(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.yRot + 0x4000;
    LaraSurfaceCollision(item, coll);
}

void lara_col_swandive(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.yRot;
    coll->bad_pos = NO_HEIGHT;
    coll->bad_neg = -STEPUP_HEIGHT;
    coll->bad_ceiling = BAD_JUMP_CEILING;
    GetLaraCollisionInfo(item, coll);
    LaraDeflectEdgeJump(item, coll);

    if (coll->mid_floor <= 0 && item->fallspeed > 0)
    {
        item->goalAnimState = STATE_LARA_IDLE;
        item->fallspeed = 0;
        item->gravityStatus = FALSE;

        if (coll->mid_floor != -NO_HEIGHT)
            item->pos.yPos += coll->mid_floor;
    }
}

void lara_col_fastdive(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.yRot;
    coll->bad_pos = NO_HEIGHT;
    coll->bad_neg = -STEPUP_HEIGHT;
    coll->bad_ceiling = BAD_JUMP_CEILING;
    GetLaraCollisionInfo(item, coll);
    LaraDeflectEdgeJump(item, coll);

    if (coll->mid_floor <= 0 && item->fallspeed > 0)
    {
        if (item->fallspeed > 133)
            item->goalAnimState = STATE_LARA_DEATH;
        else
            item->goalAnimState = STATE_LARA_IDLE;

        item->fallspeed = 0;
        item->gravityStatus = FALSE;

        if (coll->mid_floor != -NO_HEIGHT)
            item->pos.yPos += coll->mid_floor;
    }
}

void lara_col_climbstnc(ITEM_INFO* item, COLL_INFO* coll)
{
    if (LaraCheckForLetGo(item, coll))
        return;

    if (item->animNumber != ANIMATION_LARA_LADDER_IDLE)
        return;

    int result_r, result_l;
    int shift_r, shift_l;
    int ledge_r, ledge_l;

    if (CHK_EXI(TrInput, IN_FORWARD))
    {
        if (item->goalAnimState == STATE_LARA_GRABBING)
            return;
        item->goalAnimState = STATE_LARA_LADDER_IDLE;

        result_r = LaraTestClimbUpPos(item, coll->radius, coll->radius + CLIMB_WIDTHR, &shift_r, &ledge_r);
        result_l = LaraTestClimbUpPos(item, coll->radius, -(coll->radius + CLIMB_WIDTHL), &shift_l, &ledge_l);

        if (!result_r || !result_l)
            return;

        if (result_r < 0 || result_l < 0)
        {
            if (abs(ledge_l - ledge_r) <= CLIMB_WIDTH)
            {
                item->goalAnimState = STATE_LARA_GRABBING;
                item->pos.yPos += (ledge_l + ledge_r) / 2 - STEP_L;
                return;
            }
        }

        if (shift_r)
        {
            if (shift_l)
            {
                if ((shift_r < 0) ^ (shift_l < 0))
                    return;

                if ((shift_r < 0) && (shift_r < shift_l))
                    shift_l = shift_r;
                else if ((shift_r > 0) && (shift_r > shift_l))
                    shift_l = shift_r;
            }
            else
            {
                shift_l = shift_r;
            }
        }

        item->goalAnimState = STATE_LARA_LADDER_UP;
        item->pos.yPos += shift_l;
    }
    else if (CHK_EXI(TrInput, IN_BACK))
    {
        if (item->goalAnimState == STATE_LARA_HANG)
            return;
        item->goalAnimState = STATE_LARA_LADDER_IDLE;
        item->pos.yPos += STEP_L;
        result_r = LaraTestClimbPos(item, coll->radius, coll->radius + CLIMB_WIDTHR, -CLIMB_HITE, CLIMB_HITE, &shift_r);
        result_l = LaraTestClimbPos(item, coll->radius, -(coll->radius + CLIMB_WIDTHL), -CLIMB_HITE, CLIMB_HITE, &shift_l);
        item->pos.yPos -= STEP_L;

        if (result_r && result_l)
        {
            if (shift_r && shift_l)
            {
                if ((shift_r < 0) ^ (shift_l < 0))
                    return;

                if ((shift_r < 0) && (shift_r < shift_l))
                    shift_l = shift_r;
                else if ((shift_r > 0) && (shift_r > shift_l))
                    shift_l = shift_r;
            }

            if (result_r == 1 && result_l == 1)
            {
                item->goalAnimState = STATE_LARA_LADDER_DOWN;
                item->pos.yPos += shift_l;
            }
            else
            {
                item->goalAnimState = STATE_LARA_HANG;
            }
        }
    }
}

void lara_col_climbup(ITEM_INFO* item, COLL_INFO* coll)
{
    if (LaraCheckForLetGo(item, coll))
        return;

    int result_r, result_l;
    int shift_r, shift_l;
    int ledge_r, ledge_l;
    int yshift, frame;

    if (item->animNumber == ANIMATION_LARA_LADDER_UP)
    {
        frame = item->frameNumber - Anims[ANIMATION_LARA_LADDER_UP].frameBase;

        if (frame == 0)
            yshift = 0;
        else if (frame == (LARA_CLIMB_LNK1 - 1) || frame == LARA_CLIMB_LNK1)
            yshift = -(STEP_L);
        else if (frame == (LARA_CLIMB_LNK2 - 1))
            yshift = -(STEP_L * 2);
        else
            return;
    }
    else
    {
        return;
    }

    item->pos.yPos += yshift - STEP_L;
    result_r = LaraTestClimbUpPos(item, coll->radius, coll->radius + CLIMB_WIDTHR, &shift_r, &ledge_r);
    result_l = LaraTestClimbUpPos(item, coll->radius, -(coll->radius + CLIMB_WIDTHL), &shift_l, &ledge_l);
    item->pos.yPos += STEP_L;

    if (!result_r || !result_l || CHK_NOP(TrInput, IN_FORWARD))
    {
        item->goalAnimState = STATE_LARA_LADDER_IDLE;
        if (yshift != 0)
            AnimateLara(item);
        return;
    }

    if (result_r < 0 || result_l < 0)
    {
        item->goalAnimState = STATE_LARA_LADDER_IDLE;
        AnimateLara(item);

        if (abs(ledge_l - ledge_r) <= CLIMB_WIDTH)
        {
            item->goalAnimState = STATE_LARA_GRABBING;
            item->pos.yPos += (ledge_r + ledge_l) / 2 - STEP_L;
        }
        return;
    }

    // continue climbing OK
    item->goalAnimState = STATE_LARA_LADDER_UP;
    item->pos.yPos -= yshift;
}

void lara_col_climbleft(ITEM_INFO* item, COLL_INFO* coll)
{
    if (LaraCheckForLetGo(item, coll))
        return;
    int shift = 0;
    lara.move_angle = item->pos.yRot - ANGLE(90);
    LaraDoClimbLeftRight(item, coll, LaraTestClimbPos(item, coll->radius, -(coll->radius + CLIMB_WIDTHL), -CLIMB_HITE, CLIMB_HITE, &shift), shift);
}

void lara_col_climbright(ITEM_INFO* item, COLL_INFO* coll)
{
    if (LaraCheckForLetGo(item, coll))
        return;
    int shift = 0;
    lara.move_angle = item->pos.yRot + ANGLE(90);
    LaraDoClimbLeftRight(item, coll, LaraTestClimbPos(item, coll->radius, coll->radius + CLIMB_WIDTHR, -CLIMB_HITE, CLIMB_HITE, &shift), shift);
}

void lara_col_climbdown(ITEM_INFO* item, COLL_INFO* coll)
{
    if (LaraCheckForLetGo(item, coll))
        return;

    int result_r, result_l;
    int shift_r, shift_l;
    int yshift, frame;

    if (item->animNumber == ANIMATION_LARA_LADDER_DOWN)
    {
        frame = item->frameNumber - Anims[ANIMATION_LARA_LADDER_DOWN].frameBase;

        if (frame == 0)
            yshift = 0;
        else if (frame == (LARA_CLIMB_LNK1 - 1) || frame == LARA_CLIMB_LNK1)
            yshift = (STEP_L);
        else if (frame == (LARA_CLIMB_LNK2 - 1))
            yshift = (STEP_L * 2);
        else
            return;
    }
    else
    {
        return;
    }

    item->pos.yPos += yshift + STEP_L;
    result_r = LaraTestClimbPos(item, coll->radius, coll->radius + CLIMB_WIDTHR, -CLIMB_HITE, CLIMB_HITE, &shift_r);
    result_l = LaraTestClimbPos(item, coll->radius, -(coll->radius + CLIMB_WIDTHL), -CLIMB_HITE, CLIMB_HITE, &shift_l);
    item->pos.yPos -= STEP_L;

    if (!result_r || !result_l || CHK_NOP(TrInput, IN_BACK))
    {
        item->goalAnimState = STATE_LARA_LADDER_IDLE;
        if (yshift)
            AnimateLara(item);
        return;
    }

    if (shift_r && shift_l)
    {
        if ((shift_r < 0) ^ (shift_l < 0))
        {
            item->goalAnimState = STATE_LARA_LADDER_IDLE;
            AnimateLara(item);
            return;
        }

        if (shift_r < 0 && shift_r < shift_l)
            shift_l = shift_r;
        else if (shift_r > 0 && shift_r > shift_l)
            shift_l = shift_r;
    }

    if (result_r == -1 || result_l == -1)
    {
        SetAnimationForItemASS(item, ANIMATION_LARA_LADDER_IDLE, STATE_LARA_LADDER_IDLE, STATE_LARA_HANG);
        AnimateLara(item);
        return;
    }

    // continue climbing OK
    item->goalAnimState = STATE_LARA_LADDER_DOWN;
    item->pos.yPos -= yshift;
}

void lara_col_wade(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.yRot;
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
        item->pos.zRot = 0;

        if ((coll->front_type == WALL || coll->front_type == SPLIT_TRI) && coll->front_floor < -((STEP_L * 5) / 2))
        {
            item->currentAnimState = STATE_LARA_SPLAT;

            if (item->frameNumber >= 0 && item->frameNumber <= 9)
            {
                item->animNumber = ANIMATION_LARA_WALL_SMASH_LEFT;
                item->frameNumber = Anims[item->animNumber].frameBase;
                return;
            }
            if (item->frameNumber >= 10 && item->frameNumber <= 21)
            {
                item->animNumber = ANIMATION_LARA_WALL_SMASH_RIGHT;
                item->frameNumber = Anims[item->animNumber].frameBase;
                return;
            }
        }

        LaraCollideStop(item, coll);
    }

    if (coll->mid_floor >= -STEPUP_HEIGHT && coll->mid_floor < -(STEP_L / 2))
    {
        if (item->frameNumber >= 3 && item->frameNumber <= 14)
        {
            item->animNumber = ANIMATION_LARA_RUN_UP_STEP_LEFT;
            item->frameNumber = Anims[item->animNumber].frameBase;
        }
        else
        {
            item->animNumber = ANIMATION_LARA_RUN_UP_STEP_RIGHT;
            item->frameNumber = Anims[item->animNumber].frameBase;
        }
    }

    if (coll->mid_floor >= 50)
    {
        item->pos.yPos += 50;
        return;
    }

    if (coll->mid_floor != -NO_HEIGHT)
        item->pos.yPos += coll->mid_floor;
}

void lara_col_waterroll(ITEM_INFO* item, COLL_INFO* coll)
{
    LaraSwimCollision(item, coll);
}

void lara_col_duck(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.yRot;
    coll->bad_pos = STEPUP_HEIGHT;
    coll->bad_neg = -STEPUP_HEIGHT;
    coll->bad_ceiling = 0;
    coll->facing = item->pos.yRot;
    coll->slopes_are_walls = TRUE;
    item->fallspeed = 0;
    item->gravityStatus = FALSE;
    GetCollisionInfo(coll, item->pos.xPos, item->pos.yPos, item->pos.zPos, item->roomNumber, LARA_DUCK_HEIGHT);

    if (LaraFallen(item, coll))
    {
        lara.gun_status = LHS_ARMLESS;
        return;
    }

    if (TestLaraSlide(item, coll))
        return;

    if (coll->mid_ceiling < -(LARA_HITE - LARA_DUCK_HEIGHT))
        lara.keep_ducked = FALSE;
    else
        lara.keep_ducked = TRUE;

    ShiftItem(item, coll);
    if (coll->mid_floor != -NO_HEIGHT)
        item->pos.yPos += coll->mid_floor;

    long water_level = -lara.water_surface_dist;
    if ((CHK_NOP(TrInput, IN_DUCK) || (lara.water_status == LWS_WADE && water_level > WADE_DEPTH)) && !lara.keep_ducked && item->animNumber == ANIMATION_LARA_CROUCH_IDLE)
    {
        item->goalAnimState = STATE_LARA_IDLE;
        return;
    }

    if (CHK_EXI(TrInput, IN_LEFT))
        item->goalAnimState = STATE_LARA_CROUCH_TURN_LEFT;
    else if (CHK_EXI(TrInput, IN_RIGHT))
        item->goalAnimState = STATE_LARA_CROUCH_TURN_RIGHT;
}

void lara_col_dash(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.yRot;
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
        item->pos.zRot = 0;
        if (TestWall(item, STEP_L, 0, -LARA_SHOULDER_HITE))
        {
            SetAnimationForItemASN(item, ANIMATION_LARA_WALL_SMASH_LEFT, STATE_LARA_SPLAT, false);
            return;
        }
        LaraCollideStop(item, coll);
    }

    if (LaraFallen(item, coll))
        return;

    if (coll->mid_floor >= -STEPUP_HEIGHT && coll->mid_floor < -(STEP_L / 2))
    {
        if (item->frameNumber >= 3 && item->frameNumber <= 14)
        {
            item->animNumber = ANIMATION_LARA_RUN_UP_STEP_LEFT;
            item->frameNumber = Anims[item->animNumber].frameBase;
        }
        else
        {
            item->animNumber = ANIMATION_LARA_RUN_UP_STEP_RIGHT;
            item->frameNumber = Anims[item->animNumber].frameBase;
        }
    }

    if (TestLaraSlide(item, coll))
        return;

    if (coll->mid_floor >= 50)
    {
        item->pos.yPos += 50;
        return;
    }

    if (coll->mid_floor != -NO_HEIGHT)
        item->pos.yPos += coll->mid_floor;
}

void lara_col_dashdive(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->speed < 0)
        lara.move_angle = item->pos.yRot - 0x8000;
    else
        lara.move_angle = item->pos.yRot;
    coll->bad_pos = NO_HEIGHT;
    coll->bad_neg = -STEP_L;
    coll->bad_ceiling = BAD_JUMP_CEILING;
    coll->slopes_are_walls = TRUE;
    GetLaraCollisionInfo(item, coll);
    LaraDeflectEdgeJump(item, coll);

    if (LaraFallen(item, coll))
        return;

    if (item->speed < 0)
        lara.move_angle = item->pos.yRot;

    if (coll->mid_floor <= 0 && item->fallspeed > 0)
    {
        if (LaraLandedBad(item))
        {
            item->goalAnimState = STATE_LARA_DEATH;
        }
        else
        {
            if (CHK_EXI(TrInput, IN_FORWARD) && CHK_NOP(TrInput, IN_WALK))
                item->goalAnimState = STATE_LARA_RUN_FORWARD;
            else
                item->goalAnimState = STATE_LARA_IDLE;
        }

        item->pos.yPos += coll->mid_floor;
        item->speed = 0;
        item->fallspeed = 0;
        item->gravityStatus = FALSE;
        AnimateLara(item);
    }

    ShiftItem(item, coll);
    if (coll->mid_floor != -NO_HEIGHT)
        item->pos.yPos += coll->mid_floor;
}

void lara_col_hang2(ITEM_INFO* item, COLL_INFO* coll)
{
    item->fallspeed = 0;
    item->gravityStatus = FALSE;

    if (lara.can_monkey_swing)
    {
        lara.move_angle = item->pos.yRot;
        coll->bad_pos = NO_HEIGHT;
        coll->bad_neg = -NO_HEIGHT;
        coll->bad_ceiling = 0;
        coll->facing = item->pos.yRot;
        coll->radius = LARA_RAD;
        coll->lava_is_pit = FALSE;
        coll->enable_spaz = FALSE;
        coll->enable_baddie_push = FALSE;
        coll->slopes_are_pits = FALSE;
        coll->slopes_are_walls = FALSE;
        coll->hit_ceiling = FALSE;
        GetCollisionInfo(coll, item->pos.xPos, item->pos.yPos, item->pos.zPos, item->roomNumber, LARA_HANG_HEIGHT);

        if (CHK_EXI(TrInput, IN_FORWARD) && coll->coll_type != COLL_FRONT && abs(coll->mid_ceiling - coll->front_ceiling) < 50)
        {
            item->goalAnimState = STATE_LARA_MONKEYSWING_FORWARD;
        }
        else if (CHK_EXI(TrInput, IN_LSTEP) && TestMonkeyLeft(item, coll))
        {
            item->goalAnimState = STATE_LARA_MONKEYSWING_LEFT;
        }
        else if (CHK_EXI(TrInput, IN_RSTEP) && TestMonkeyRight(item, coll))
        {
            item->goalAnimState = STATE_LARA_MONKEYSWING_RIGHT;
        }
        else if (CHK_EXI(TrInput, IN_LEFT))
        {
            item->goalAnimState = STATE_LARA_MONKEYSWING_TURN_LEFT;
        }
        else if (CHK_EXI(TrInput, IN_RIGHT))
        {
            item->goalAnimState = STATE_LARA_MONKEYSWING_TURN_RIGHT;
        }

        MonkeySwingSnap(item, coll);
    }
    else
    {
        LaraHangTest(item, coll);

        if (item->goalAnimState == STATE_LARA_MONKEYSWING_IDLE)
        {
            if (CHK_EXI(TrInput, IN_FORWARD))
            {
                if (coll->front_floor > -850
                &&  coll->front_floor < -650
                &&  coll->front_floor - coll->front_ceiling >= 0
                &&  coll->left_floor2 - coll->left_ceiling2 >= 0
                &&  coll->right_floor2 - coll->right_ceiling2 >= 0
                &&  !coll->hit_static)
                {
                    if (CHK_EXI(TrInput, IN_WALK))
                        item->goalAnimState = STATE_LARA_HANDSTAND;
                    else
                        item->goalAnimState = STATE_LARA_GRABBING;
                }
            }

            if (CHK_EXI(TrInput, (IN_DUCK | IN_FORWARD)))
            {
                if (coll->front_floor > -850
                &&  coll->front_floor < -650
                &&  coll->front_floor - coll->front_ceiling >= -STEP_L
                &&  coll->left_floor2 - coll->left_ceiling2 >= -STEP_L
                &&  coll->right_floor2 - coll->right_ceiling2 >= -STEP_L
                &&  !coll->hit_static)
                {
                    item->goalAnimState = STATE_LARA_CLIMB_TO_CRAWL;
                    item->requiredAnimState = STATE_LARA_CROUCH_IDLE;
                }
            }
            else if (CHK_EXI(TrInput, IN_LEFT) || CHK_EXI(TrInput, IN_LSTEP))
            {
                item->goalAnimState = STATE_LARA_SHIMMY_LEFT;
            }
            else if (CHK_EXI(TrInput, IN_RIGHT) || CHK_EXI(TrInput, IN_RSTEP))
            {
                item->goalAnimState = STATE_LARA_SHIMMY_LEFT;
            }
        }
    }
}

void lara_col_monkeyswing(ITEM_INFO* item, COLL_INFO* coll)
{
    if (CHK_NOP(TrInput, IN_ACTION) || !lara.can_monkey_swing)
    {
        MonkeySwingFall(item);
        return;
    }

    lara.move_angle = item->pos.yRot;
    coll->bad_pos = NO_HEIGHT;
    coll->bad_neg = -NO_HEIGHT;
    coll->bad_ceiling = 0;
    coll->facing = item->pos.yRot;
    coll->radius = LARA_RAD;
    coll->slopes_are_walls = FALSE;
    GetCollisionInfo(coll, item->pos.xPos, item->pos.yPos, item->pos.zPos, item->roomNumber, LARA_HANG_HEIGHT);

    if (coll->coll_type == COLL_FRONT || abs(coll->mid_ceiling - coll->front_ceiling) > 50)
    {
        SetAnimationForItemAS(item, ANIMATION_LARA_MONKEY_IDLE, STATE_LARA_MONKEYSWING_IDLE);
        return;
    }

    // shift lara if it collide with the right/left corner of the ceiling if needed...
    if (abs(coll->mid_ceiling - coll->left_ceiling2) > 50)
    {
        ShiftItem(item, coll);
        item->pos.yRot += ANGLE(5);
    }
    else if (abs(coll->mid_ceiling - coll->right_ceiling2) > 50)
    {
        ShiftItem(item, coll);
        item->pos.yRot -= ANGLE(5);
    }

    camera.target_elevation = ANGLE(10);
    MonkeySwingSnap(item, coll);
}

void lara_col_monkeyl(ITEM_INFO* item, COLL_INFO* coll)
{
    if (CHK_NOP(TrInput, IN_ACTION) || !lara.can_monkey_swing)
    {
        MonkeySwingFall(item);
        return;
    }

    if (!TestMonkeyLeft(item, coll))
    {
        SetAnimationForItemAS(item, ANIMATION_LARA_MONKEY_IDLE, STATE_LARA_MONKEYSWING_IDLE);
        return;
    }

    MonkeySwingSnap(item, coll);
}

void lara_col_monkeyr(ITEM_INFO* item, COLL_INFO* coll)
{
    if (CHK_NOP(TrInput, IN_ACTION) || !lara.can_monkey_swing)
    {
        MonkeySwingFall(item);
        return;
    }

    if (!TestMonkeyRight(item, coll))
    {
        SetAnimationForItemAS(item, ANIMATION_LARA_MONKEY_IDLE, STATE_LARA_MONKEYSWING_IDLE);
        return;
    }

    MonkeySwingSnap(item, coll);
}

void lara_col_monkey180(ITEM_INFO* item, COLL_INFO* coll)
{
    lara_col_monkeyswing(item, coll);
}

void lara_col_all4s(ITEM_INFO* item, COLL_INFO* coll)
{
    item->fallspeed = 0;
    item->gravityStatus = FALSE;

    if (item->goalAnimState == STATE_LARA_CRAWL_TO_CLIMB)
        return;

    lara.move_angle = item->pos.yRot;
    coll->bad_pos = (STEP_L - 1);
    coll->bad_neg = -(STEP_L - 1);
    coll->bad_ceiling = LARA_DUCK_HEIGHT;
    coll->facing = item->pos.yRot;
    coll->radius = LARA_RAD * 2;
    coll->slopes_are_pits = TRUE;
    coll->slopes_are_walls = TRUE;
    GetCollisionInfo(coll, item->pos.xPos, item->pos.yPos, item->pos.zPos, item->roomNumber, LARA_DUCK_HEIGHT);

    if (LaraFallen(item, coll))
    {
        lara.gun_status = LHS_ARMLESS;
        return;
    }

    if (TestLaraSlide(item, coll))
        return;

    short front, back;
    short ceiling;
    int slope = abs(coll->left_floor2 - coll->right_floor2);

    if (coll->mid_ceiling < -(LARA_HITE - LARA_DUCK_HEIGHT))
        lara.keep_ducked = FALSE;
    else
        lara.keep_ducked = TRUE;

    ShiftItem(item, coll);
    if (coll->mid_floor != -NO_HEIGHT && coll->mid_floor > -STEP_L)
        item->pos.yPos += coll->mid_floor;

    long water_level = -lara.water_surface_dist;
    if ((CHK_NOP(TrInput, IN_DUCK) && !lara.keep_ducked) || (CHK_EXI(TrInput, (IN_FLARE | IN_DRAW))) || (lara.water_status == LWS_WADE && water_level > WADE_DEPTH))
    {
        item->goalAnimState = STATE_LARA_CROUCH_IDLE;
    }
    else if (item->animNumber == ANIMATION_LARA_CRAWL_IDLE || item->animNumber == ANIMATION_LARA_CROUCH_TO_CRAWL_END)
    {
        if (CHK_EXI(TrInput, IN_FORWARD))
        {
            front = LaraFloorFront(item, item->pos.yRot, LARA_DUCK_FRONT);
            if (front < (STEP_L - 1) && front > -(STEP_L - 1) && height_type != BIG_SLOPE)
                item->goalAnimState = STATE_LARA_CRAWL_FORWARD;
        }
        else if (CHK_EXI(TrInput, IN_BACK))
        {
            ceiling = LaraCeilingFront(item, item->pos.yRot, -LARA_CRAWL_FRONT, (STEP_L / 2));
            if (ceiling == -NO_HEIGHT || ceiling > STEP_L)
                return;

            back = LaraFloorFront(item, item->pos.yRot, -LARA_CRAWL_FRONT);
            if (back < (STEP_L - 1) && back > -(STEP_L - 1) && height_type != BIG_SLOPE)
            {
                item->goalAnimState = STATE_LARA_CRAWL_BACK;
            }
            else if (CHK_EXI(TrInput, IN_ACTION))
            {
                if (back > (STEP_L * 3) && slope < 120)
                {
                    item->pos.xPos -= LARA_RAD * SIN(coll->facing) >> W2V_SHIFT;
                    item->pos.zPos -= LARA_RAD * COS(coll->facing) >> W2V_SHIFT;

                    if (!GetCollidedObjects(item, LARA_RAD, NULL, collide_item, collide_meshes, NULL))
                    {
                        SHORT angle = (USHORT)(item->pos.yRot + 0x2000) / 0x4000;

                        switch (angle)
                        {
                            case NORTH:
                                item->pos.yRot = 0;
                                item->pos.zPos = (item->pos.zPos & ~(WALL_L - 1)) + 225;
                                break;
                            case EAST:
                                item->pos.yRot = 0x4000;
                                item->pos.xPos = (item->pos.xPos & ~(WALL_L - 1)) + 225;
                                break;
                            case SOUTH:
                                item->pos.yRot = -0x8000;
                                item->pos.zPos = (item->pos.zPos | (WALL_L - 1)) - 225;
                                break;
                            case WEST:
                                item->pos.yRot = -0x4000;
                                item->pos.xPos = (item->pos.xPos | (WALL_L - 1)) - 225;
                                break;
                        }

                        item->goalAnimState = STATE_LARA_CRAWL_TO_CLIMB;
                    }
                }
            }
        }
        else if (CHK_EXI(TrInput, IN_LEFT))
        {
            SetAnimationForItemAS(item, ANIMATION_LARA_CRAWL_TURN_LEFT, STATE_LARA_CRAWL_TURN_LEFT);
        }
        else if (CHK_EXI(TrInput, IN_RIGHT))
        {
            SetAnimationForItemAS(item, ANIMATION_LARA_CRAWL_TURN_RIGHT, STATE_LARA_CRAWL_TURN_RIGHT);
        }
    }
}

void lara_col_crawl(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.yRot;
    coll->bad_pos = (STEP_L - 1);
    coll->bad_neg = -(STEP_L - 1);
    coll->bad_ceiling = LARA_DUCK_HEIGHT;
    coll->facing = item->pos.yRot;
    coll->radius = LARA_RAD * 2;
    coll->slopes_are_pits = TRUE;
    coll->slopes_are_walls = TRUE;
    item->fallspeed = 0;
    item->gravityStatus = FALSE;
    GetCollisionInfo(coll, item->pos.xPos, item->pos.yPos, item->pos.zPos, item->roomNumber, LARA_DUCK_HEIGHT);

    if (LaraDeflectEdgeDuck(item, coll))
    {
        item->currentAnimState = STATE_LARA_CRAWL_IDLE;
        item->goalAnimState = STATE_LARA_CRAWL_IDLE;
        if (item->animNumber != ANIMATION_LARA_CRAWL_IDLE)
        {
            item->animNumber = ANIMATION_LARA_CRAWL_IDLE;
            item->frameNumber = Anims[item->animNumber].frameBase;
        }
        return;
    }

    if (LaraFallen(item, coll))
    {
        lara.gun_status = LHS_ARMLESS;
        return;
    }

    if (TestLaraSlide(item, coll))
        return;

    ShiftItem(item, coll);
    if (coll->mid_floor != -NO_HEIGHT && coll->mid_floor > -STEP_L)
        item->pos.yPos += coll->mid_floor;
}

void lara_col_hangturn(ITEM_INFO* item, COLL_INFO* coll)
{
    if (CHK_NOP(TrInput, IN_ACTION) || !lara.can_monkey_swing)
    {
        MonkeySwingFall(item);
        return;
    }

    lara.move_angle = item->pos.yRot;
    coll->bad_pos = NO_HEIGHT;
    coll->bad_neg = -STEPUP_HEIGHT;
    coll->bad_ceiling = 0;
    coll->facing = item->pos.yRot;
    coll->radius = LARA_RAD;
    coll->slopes_are_walls = TRUE;
    GetCollisionInfo(coll, item->pos.xPos, item->pos.yPos, item->pos.zPos, item->roomNumber, LARA_HANG_HEIGHT);

    MonkeySwingSnap(item, coll);
}

void lara_col_all4turn(ITEM_INFO* item, COLL_INFO* coll)
{
    GetCollisionInfo(coll, item->pos.xPos, item->pos.yPos, item->pos.zPos, item->roomNumber, LARA_DUCK_HEIGHT);
    if (TestLaraSlide(item, coll))
        return;
    if (coll->mid_floor != -NO_HEIGHT && coll->mid_floor > -STEP_L)
        item->pos.yPos += coll->mid_floor;
}

void lara_col_crawlb(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.yRot - 0x8000;
    coll->bad_pos = (STEP_L - 1);
    coll->bad_neg = -(STEP_L - 1);
    coll->bad_ceiling = LARA_DUCK_HEIGHT;
    coll->facing = item->pos.yRot - 0x8000;
    coll->radius = LARA_CRAWLB_RAD;
    coll->slopes_are_pits = TRUE;
    coll->slopes_are_walls = TRUE;
    item->fallspeed = 0;
    item->gravityStatus = FALSE;
    GetCollisionInfo(coll, item->pos.xPos, item->pos.yPos, item->pos.zPos, item->roomNumber, LARA_DUCK_HEIGHT);

    if (LaraDeflectEdgeDuck(item, coll))
    {
        item->currentAnimState = STATE_LARA_CRAWL_IDLE;
        item->goalAnimState = STATE_LARA_CRAWL_IDLE;
        if (item->animNumber != ANIMATION_LARA_CRAWL_IDLE)
        {
            item->animNumber = ANIMATION_LARA_CRAWL_IDLE;
            item->frameNumber = Anims[item->animNumber].frameBase;
        }
        return;
    }

    if (LaraFallen(item, coll))
    {
        lara.gun_status = LHS_ARMLESS;
        return;
    }

    if (TestLaraSlide(item, coll))
        return;

    ShiftItem(item, coll);
    if (coll->mid_floor != -NO_HEIGHT && coll->mid_floor > -STEP_L)
        item->pos.yPos += coll->mid_floor;
    lara.move_angle = item->pos.yRot;
}

void lara_col_crawl2hang(ITEM_INFO* item, COLL_INFO* coll)
{
    short* bounds;
    int edge, edge_catch;
    short angle;

    camera.target_angle = 0;
    camera.target_elevation = -10920;

    if (item->animNumber == ANIMATION_LARA_CRAWL_TO_HANG_END)
    {
        lara.move_angle = item->pos.yRot;
        coll->bad_pos = NO_HEIGHT;
        coll->bad_neg = -STEPUP_HEIGHT;
        coll->bad_ceiling = BAD_JUMP_CEILING;
        coll->facing = item->pos.yRot;
        item->fallspeed = 512;
        item->pos.yPos += (STEP_L - 1);
        GetCollisionInfo(coll, item->pos.xPos, item->pos.yPos, item->pos.zPos, item->roomNumber, LARA_JUMPUP_HITE);

        edge_catch = LaraTestEdgeCatch(item, coll, &edge);
        if (!edge_catch)
            return;
        if (edge_catch < 0 && !LaraTestHangOnClimbWall(item, coll))
            return;

        angle = GetCatchAngle(item, HANG_ANGLE);
        if (angle & 0x3FFF)
            return;

        if (TestHangSwingIn(item, angle))
        {
            lara.head_y_rot = 0;
            lara.head_x_rot = 0;
            lara.torso_y_rot = 0;
            lara.torso_x_rot = 0;
            SetAnimationForItemAS(item, ANIMATION_LARA_OSCILLATE_HANG_ON, STATE_LARA_MONKEYSWING_IDLE);
        }
        else
        {
            SetAnimationForItemAS(item, ANIMATION_LARA_HANG_IDLE, STATE_LARA_HANG);
        }

        bounds = GetBoundsAccurate(item);
        if (edge_catch > 0)
        {
            item->pos.yPos += coll->front_floor - bounds[2];
            item->pos.xPos += coll->shift.x;
            item->pos.zPos += coll->shift.z;
        }
        else
        {
            item->pos.yPos = edge - bounds[2];
        }

        item->pos.yRot = angle;
        item->gravityStatus = TRUE;
        item->speed = 2;
        item->fallspeed = 1;
        lara.gun_status = LHS_HANDBUSY;
    }
}

void lara_col_turnswitch(ITEM_INFO* item, COLL_INFO* coll)
{
    if (coll->old.x != item->pos.xPos || coll->old.z != item->pos.zPos)
    {
        if (item->animNumber == ANIMATION_LARA_ROUND_HANDLE_PUSH_LEFT_CONTINUE)
        {
            item->pos.yRot -= 0x4000;
            item->animNumber = ANIMATION_LARA_ROUND_HANDLE_PUSH_LEFT_END;
            item->frameNumber = Anims[item->animNumber].frameBase;
        }
        else if (item->animNumber == ANIMATION_LARA_ROUND_HANDLE_PUSH_RIGHT_CONTINUE)
        {
            item->pos.yRot += 0x4000;
            item->animNumber = ANIMATION_LARA_ROUND_HANDLE_PUSH_RIGHT_END;
            item->frameNumber = Anims[item->animNumber].frameBase;
        }
    }
}

void lara_col_polestat(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_FREEFALL;
        return;
    }

    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = FALSE;

    if (item->animNumber == ANIMATION_LARA_POLE_IDLE)
    {
        lara.move_angle = item->pos.yRot;
        coll->bad_pos = NO_HEIGHT;
        coll->bad_neg = -STEPUP_HEIGHT;
        coll->bad_ceiling = BAD_JUMP_CEILING;
        coll->facing = item->pos.yRot;
        coll->radius = LARA_RAD;
        coll->slopes_are_walls = TRUE;
        GetLaraCollisionInfo(item, coll);

        if (CHK_EXI(TrInput, IN_ACTION))
        {
            item->goalAnimState = STATE_LARA_POLE_IDLE;

            if (CHK_EXI(TrInput, IN_LEFT))
                item->goalAnimState = STATE_LARA_POLE_TURN_LEFT;
            else if (CHK_EXI(TrInput, IN_RIGHT))
                item->goalAnimState = STATE_LARA_POLE_TURN_RIGHT;

            if (CHK_EXI(TrInput, IN_LOOK))
                LookUpDown();

            if (CHK_EXI(TrInput, IN_FORWARD))
            {
                FLOOR_INFO* floor;
                int ceiling;
                short roomNumber;

                roomNumber = item->roomNumber;
                floor = GetFloor(item->pos.xPos, item->pos.yPos, item->pos.zPos, &roomNumber);
                ceiling = GetCeiling(floor, item->pos.xPos, item->pos.yPos, item->pos.zPos);
                if ((item->pos.yPos - ceiling) > WALL_L)
                    item->goalAnimState = STATE_LARA_POLE_UP;
            }
            else if (CHK_EXI(TrInput, IN_BACK) && coll->mid_floor > 0)
            {
                item->goalAnimState = STATE_LARA_POLE_DOWN;
                item->itemFlags[2] = 0;
            }

            if (CHK_EXI(TrInput, IN_JUMP))
                item->goalAnimState = STATE_LARA_JUMP_BACK;
        }
        else if (coll->mid_floor <= 0)
        {
            item->goalAnimState = STATE_LARA_IDLE;
        }
        else
        {
            item->pos.xPos -= SINN(item->pos.yRot) << 8 >> W2V_SHIFT;
            item->pos.zPos -= COSN(item->pos.yRot) << 8 >> W2V_SHIFT;
            item->goalAnimState = STATE_LARA_FREEFALL;
        }
    }
}

void lara_col_poleup(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_POLE_IDLE;
        return;
    }

    FLOOR_INFO* floor;
    int ceiling;
    short roomNumber;

    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = FALSE;

    if (CHK_EXI(TrInput, IN_LOOK))
        LookUpDown();

    if (CHK_NOP(TrInput, IN_FORWARD) || CHK_NOP(TrInput, IN_ACTION))
        item->goalAnimState = STATE_LARA_POLE_IDLE;

    roomNumber = item->roomNumber;
    floor = GetFloor(item->pos.xPos, item->pos.yPos, item->pos.zPos, &roomNumber);
    ceiling = GetCeiling(floor, item->pos.xPos, item->pos.yPos, item->pos.zPos);
    if ((item->pos.yPos - ceiling) < WALL_L)
        item->goalAnimState = STATE_LARA_POLE_IDLE;
}

void lara_col_poledown(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_POLE_IDLE;
        return;
    }

    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = FALSE;

    if (CHK_NOP(TrInput, IN_BACK) || CHK_NOP(TrInput, IN_ACTION))
        item->goalAnimState = STATE_LARA_POLE_IDLE;

    FLOOR_INFO* floor;
    int height;
    short roomNumber;

    lara.move_angle = item->pos.yRot;
    coll->bad_pos = NO_HEIGHT;
    coll->bad_neg = -STEPUP_HEIGHT;
    coll->bad_ceiling = 0;
    coll->facing = item->pos.yRot;
    coll->radius = LARA_RAD;
    coll->slopes_are_walls = TRUE;
    GetLaraCollisionInfo(item, coll);

    if (coll->mid_floor < 0)
    {
        roomNumber = item->roomNumber;
        floor = GetFloor(item->pos.xPos, item->pos.yPos, item->pos.zPos, &roomNumber);
        height = GetHeight(floor, item->pos.xPos, item->pos.yPos, item->pos.zPos);
        item->pos.yPos = height;
        item->floor = height;
        item->itemFlags[2] = 0;
        item->goalAnimState = STATE_LARA_POLE_IDLE;
    }

    if (CHK_EXI(TrInput, IN_LEFT))
        item->pos.yRot += STEP_L;
    else if (CHK_EXI(TrInput, IN_RIGHT))
        item->pos.yRot -= STEP_L;

    if (item->animNumber == ANIMATION_LARA_POLE_CLIMB_DOWN_TO_IDLE)
        item->itemFlags[2] -= WALL_L;
    else
        item->itemFlags[2] += STEP_L;

    SoundEffect(SFX_LARA_POLE_LOOP, &item->pos, NULL);

    if (item->itemFlags[2] <= 0x4000)
    {
        if (item->itemFlags[2] < 0)
            item->itemFlags[2] = 0;
        item->pos.yPos += (item->itemFlags[2] >> 8);
    }
    else
    {
        item->itemFlags[2] = 0x4000;
        item->pos.yPos += (item->itemFlags[2] >> 8);
    }
}

void lara_col_duck2(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.is_ducked = TRUE;
    if (CHK_EXI(TrInput, IN_LOOK))
        LookUpDown();

    lara.move_angle = item->pos.yRot;
    coll->bad_pos = STEPUP_HEIGHT;
    coll->bad_neg = -STEPUP_HEIGHT;
    coll->bad_ceiling = 0;
    coll->facing = item->pos.yRot;
    coll->slopes_are_walls = TRUE;
    item->fallspeed = 0;
    item->gravityStatus = FALSE;
    GetCollisionInfo(coll, item->pos.xPos, item->pos.yPos, item->pos.zPos, item->roomNumber, LARA_DUCK_HEIGHT);

    if (LaraFallen(item, coll))
    {
        lara.gun_status = LHS_ARMLESS;
        return;
    }

    if (TestLaraSlide(item, coll))
        return;

    if (coll->mid_ceiling < -(LARA_HITE - LARA_DUCK_HEIGHT))
        lara.keep_ducked = FALSE;
    else
        lara.keep_ducked = TRUE;

    ShiftItem(item, coll);
    if (coll->mid_floor != -NO_HEIGHT)
        item->pos.yPos += coll->mid_floor;
}

void lara_col_rope(ITEM_INFO* item, COLL_INFO* coll)
{
    if (CHK_NOP(TrInput, IN_ACTION))
    {
        FallFromRope(item);
        return;
    }

    UpdateRopeSwing(item);

    if (CHK_EXI(TrInput, IN_SPRINT))
    {
        lara.rope_dframe = (Anims[ANIMATION_LARA_ROPE_SWING_FORWARD_SEMIHARD].frameBase + 32) << 8;
        lara.rope_frame = lara.rope_dframe;
        item->goalAnimState = STATE_LARA_ROPE_SWING;
    }
    else if (CHK_EXI(TrInput, IN_FORWARD) && lara.rope_segment > 4)
    {
        item->goalAnimState = STATE_LARA_ROPE_CLIMB_UP;
    }
    else if (CHK_EXI(TrInput, IN_BACK) && lara.rope_segment < 21)
    {
        item->goalAnimState = STATE_LARA_ROPE_CLIMB_DOWN;
        lara.rope_flag = 0;
        lara.rope_count = 0;
    }
    else if (CHK_EXI(TrInput, IN_LEFT))
    {
        item->goalAnimState = STATE_LARA_ROPE_TURN_LEFT;
    }
    else if (CHK_EXI(TrInput, IN_RIGHT))
    {
        item->goalAnimState = STATE_LARA_ROPE_TURN_RIGHT;
    }
}

void lara_col_ropefwd(ITEM_INFO* item, COLL_INFO* coll)
{
    int n;
    camera.target_distance = (WALL_L * 2);
    UpdateRopeSwing(item);

    if (item->animNumber == ANIMATION_LARA_ROPE_SWING_FORWARD_SEMIHARD)
    {
        if (CHK_EXI(TrInput, IN_SPRINT))
        {
            int absX = abs(lara.rope_last_x);
            if (absX < 9000)
                n = 192 * (9000 - absX) / 9000;
            else
                n = 0;
            ApplyVelocityToRope(lara.rope_segment - 2, item->pos.yRot + (lara.rope_direction != 0 ? ANGLE(0) : ANGLE(180)), n >> 5);
        }

        if (lara.rope_frame > lara.rope_dframe)
        {
            lara.rope_frame -= (WORD)lara.rope_framerate;
            if (lara.rope_frame < lara.rope_dframe)
                lara.rope_frame = lara.rope_dframe;
        }
        else
        {
            lara.rope_frame += (WORD)lara.rope_framerate;
            if (lara.rope_frame > lara.rope_dframe)
                lara.rope_frame = lara.rope_dframe;
        }

        item->frameNumber = (short)(lara.rope_frame >> 8);

        if (CHK_NOP(TrInput, IN_SPRINT)
        && (lara.rope_frame >> 8) == (Anims[ANIMATION_LARA_ROPE_SWING_FORWARD_SEMIHARD].frameBase + 32)
        &&  lara.rope_maxX_backward < 6750
        &&  lara.rope_maxX_forward < 6750)
        {
            SetAnimationForItemAS(item, ANIMATION_LARA_MONKEY_TO_ROPE_END, STATE_LARA_ROPE_IDLE);
        }

        if (CHK_EXI(TrInput, IN_JUMP))
            JumpOffRope(item);
    }
    else if (item->frameNumber == (Anims[ANIMATION_LARA_ROPE_IDLE_TO_SWING].frameBase + 15))
    {
        ApplyVelocityToRope(lara.rope_segment, item->pos.yRot, STEP_L);
    }
}