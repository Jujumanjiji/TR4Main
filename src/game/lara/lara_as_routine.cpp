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

void lara_as_walk(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_IDLE;
        return;
    }

    if (Lara.isMoving)
        return;

    if (CHK_EXI(TrInput, IN_LEFT))
    {
        LaraClampN(Lara.turnRate, LARA_TURN_RATE, LARA_SLOW_TURN);
    }
    else if (CHK_EXI(TrInput, IN_RIGHT))
    {
        LaraClampP(Lara.turnRate, LARA_TURN_RATE, LARA_SLOW_TURN);
    }

    if (CHK_EXI(TrInput, IN_FORWARD))
    {
        int water_level = -Lara.waterSurfaceDist;
        if (Lara.waterStatus == LWS_WADE && water_level > WADE_DEPTH)
        {
            item->goalAnimState = STATE_LARA_WADE_FORWARD;
        }
        else
        {
            if (CHK_NOP(TrInput, IN_WALK))
                item->goalAnimState = STATE_LARA_RUN_FORWARD;
            else
                item->goalAnimState = STATE_LARA_WALK_FORWARD;
        }
    }
    else
    {
        item->goalAnimState = STATE_LARA_IDLE;
    }
}

void lara_as_run(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_DEATH;
        return;
    }

    if (CHK_EXI(TrInput, IN_ROLL))
    {
        SetAnimationForItemASSF(item, ANIMATION_LARA_ROLL_BEGIN, STATE_LARA_ROLL_FORWARD, STATE_LARA_IDLE, LARA_SPRINT_TO_ROLL_FRAME);
        return;
    }

    if (CHK_EXI(TrInput, IN_SPRINT) && Lara.dashTimer)
    {
        item->goalAnimState = STATE_LARA_SPRINT;
        return;
    }

    if (CHK_EXI(TrInput, IN_DUCK) && Lara.waterStatus != LWS_WADE)
    {
        if (Lara.gunStatus == LHS_ARMLESS
        ||  Lara.gunType == LG_UNARMED
        ||  Lara.gunType == LG_PISTOLS
        ||  Lara.gunType == LG_REVOLVER
        ||  Lara.gunType == LG_UZIS
        ||  Lara.gunType == LG_FLARE)
        {
            item->goalAnimState = STATE_LARA_CROUCH_IDLE;
            return;
        }
    }

    if (CHK_EXI(TrInput, IN_LEFT))
    {
        LaraClampN(Lara.turnRate, LARA_TURN_RATE, LARA_FAST_TURN);
        LaraClampN(item->pos.zRot, LARA_LEAN_RATE, LARA_LEAN_MAX);
    }
    else if (CHK_EXI(TrInput, IN_RIGHT))
    {
        LaraClampP(Lara.turnRate, LARA_TURN_RATE, LARA_FAST_TURN);
        LaraClampP(item->pos.zRot, LARA_LEAN_RATE, LARA_LEAN_MAX);
    }

    static bool jump_ok = true;
    short frame = GetCurrentFrame(item);
    switch (item->animNumber)
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

    if (CHK_EXI(TrInput, IN_JUMP) && jump_ok && !item->gravityStatus)
    {
        item->goalAnimState = STATE_LARA_JUMP_FORWARD;
    }
    else if (CHK_EXI(TrInput, IN_FORWARD))
    {
        int water_level = -Lara.waterSurfaceDist;
        if (Lara.waterStatus == LWS_WADE && water_level > WADE_DEPTH)
        {
            item->goalAnimState = STATE_LARA_WADE_FORWARD;
        }
        else
        {
            if (CHK_EXI(TrInput, IN_WALK))
                item->goalAnimState = STATE_LARA_WALK_FORWARD;
            else
                item->goalAnimState = STATE_LARA_RUN_FORWARD;
        }
    }
    else
    {
        item->goalAnimState = STATE_LARA_IDLE;
    }
}

void lara_as_stop(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_DEATH;
        return;
    }

    /// normal
    if (item->animNumber != ANIMATION_LARA_SPRINT_SLIDE_STAND_RIGHT
    &&  item->animNumber != ANIMATION_LARA_SPRINT_SLIDE_STAND_LEFT)
    {
        StopSoundEffect(SFX_LARA_SLIPPING);
    }

    if (UseSpecialItem(item))
        return;

    if (CHK_EXI(TrInput, IN_ROLL) && Lara.waterStatus != LWS_WADE)
    {
        SetAnimationForItemASSF(item, ANIMATION_LARA_ROLL_BEGIN, STATE_LARA_ROLL_FORWARD, STATE_LARA_IDLE, LARA_STAND_TO_ROLL_FRAME);
        return;
    }

    if (CHK_EXI(TrInput, IN_DUCK) && Lara.waterStatus != LWS_WADE)
    {
        if (Lara.gunStatus == LHS_ARMLESS
        ||  Lara.gunType == LG_UNARMED
        ||  Lara.gunType == LG_PISTOLS
        ||  Lara.gunType == LG_REVOLVER
        ||  Lara.gunType == LG_UZIS
        ||  Lara.gunType == LG_FLARE)
        {
            item->goalAnimState = STATE_LARA_CROUCH_IDLE;
            return;
        }
    }

    item->goalAnimState = STATE_LARA_IDLE;

    if (CHK_EXI(TrInput, IN_LOOK))
        LookUpDown();

    short fheight = -NO_HEIGHT;
    short rheight = -NO_HEIGHT;
    if (CHK_EXI(TrInput, IN_FORWARD))
        fheight = LaraFloorFront(item, item->pos.yRot, LARA_RAD + 4);
    else if (CHK_EXI(TrInput, IN_BACK))
        rheight = LaraFloorFront(item, item->pos.yRot + 0x8000, LARA_RAD + 4);

    if (CHK_EXI(TrInput, IN_LSTEP))
    {
        short height = LaraFloorFront(item, item->pos.yRot - 0x4000, LARA_RAD + 16);
        short ceiling = LaraCeilingFront(item, item->pos.yRot - 0x4000, LARA_RAD + 16, LARA_HITE);

        if (height < 128 && height > -128 && height_type != BIG_SLOPE && ceiling <= 0)
            item->goalAnimState = STATE_LARA_WALK_LEFT;
    }
    else if (CHK_EXI(TrInput, IN_RSTEP))
    {
        short height = LaraFloorFront(item, item->pos.yRot + 0x4000, LARA_RAD + 16);
        short ceiling = LaraCeilingFront(item, item->pos.yRot + 0x4000, LARA_RAD + 16, LARA_HITE);

        if (height < 128 && height > -128 && height_type != BIG_SLOPE && ceiling <= 0)
            item->goalAnimState = STATE_LARA_WALK_RIGHT;
    }
    else if (CHK_EXI(TrInput, IN_LEFT))
    {
        item->goalAnimState = STATE_LARA_TURN_LEFT_SLOW;
    }
    else if (CHK_EXI(TrInput, IN_RIGHT))
    {
        item->goalAnimState = STATE_LARA_TURN_RIGHT_SLOW;
    }

    /// wading
    int water_level = -Lara.waterSurfaceDist;
    if (Lara.waterStatus == LWS_WADE && water_level > WADE_DEPTH)
    {
        if (CHK_EXI(TrInput, IN_JUMP))
        {
            item->goalAnimState = STATE_LARA_JUMP_PREPARE;
        }
        else if (CHK_EXI(TrInput, IN_FORWARD))
        {
            if (fheight < (STEPUP_HEIGHT - 1) && fheight > -(STEPUP_HEIGHT - 1))
            {
                lara_as_wade(item, coll);
            }
            else
            {
                Lara.moveAngle = item->pos.yRot;
                coll->badPos = NO_BAD_POS;
                coll->badNeg = -STEPUP_HEIGHT;
                coll->badCeiling = 0;
                coll->slopesAreWalls = TRUE;
                coll->radius = LARA_RAD + 2;
                GetLaraCollisionInfo(item, coll);
                if (!TestLaraVault(item, coll))
                    coll->radius = LARA_RAD;
            }
        }
        else if (CHK_EXI(TrInput, IN_BACK) && rheight < (STEPUP_HEIGHT - 1) && rheight > -(STEPUP_HEIGHT - 1))
        {
            lara_as_back(item, coll);
        }
    }
    else
    {
        /// other
        if (CHK_EXI(TrInput, IN_JUMP))
        {
            item->goalAnimState = STATE_LARA_JUMP_PREPARE;
        }
        else if (CHK_EXI(TrInput, IN_FORWARD))
        {
            short height = LaraFloorFront(item, item->pos.yRot, LARA_RAD + 4);
            short ceiling = LaraCeilingFront(item, item->pos.yRot, LARA_RAD + 4, LARA_HITE);

            if ((height < 0 || ceiling > 0) && (height_type == BIG_SLOPE || height_type == DIAGONAL))
            {
                item->goalAnimState = STATE_LARA_IDLE;
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
                Lara.moveAngle = item->pos.yRot;
                coll->badPos = NO_BAD_POS;
                coll->badNeg = -STEPUP_HEIGHT;
                coll->badCeiling = 0;
                coll->radius = LARA_RAD + 2;
                coll->slopesAreWalls = TRUE;

                GetLaraCollisionInfo(item, coll);
                if (!TestLaraVault(item, coll))
                {
                    coll->radius = LARA_RAD;
                    item->goalAnimState = STATE_LARA_IDLE;
                }
            }
        }
        else if (CHK_EXI(TrInput, IN_BACK))
        {
            if (CHK_EXI(TrInput, IN_WALK))
            {
                if ((rheight < (STEPUP_HEIGHT - 1)) && (rheight > -(STEPUP_HEIGHT - 1)) && (height_type != BIG_SLOPE))
                    lara_as_back(item, coll);
            }
            else
            {
                if (rheight > -(STEPUP_HEIGHT - 1))
                    item->goalAnimState = STATE_LARA_RUN_BACK;
            }
        }
    }
}

void lara_as_forwardjump(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->goalAnimState == STATE_LARA_SWANDIVE_BEGIN
    ||  item->goalAnimState == STATE_LARA_REACH)
    {
        item->goalAnimState = STATE_LARA_JUMP_FORWARD;
    }

    if (item->goalAnimState != STATE_LARA_DEATH
    &&  item->goalAnimState != STATE_LARA_IDLE
    &&  item->goalAnimState != STATE_LARA_RUN_FORWARD)
    {
        if (CHK_EXI(TrInput, IN_ACTION) && Lara.gunStatus == LHS_ARMLESS)
            item->goalAnimState = STATE_LARA_REACH;

        if (CHK_EXI(TrInput, IN_BACK) || CHK_EXI(TrInput, IN_ROLL))
            item->goalAnimState = STATE_LARA_JUMP_ROLL;

        if (CHK_EXI(TrInput, IN_WALK) && Lara.gunStatus == LHS_ARMLESS)
            item->goalAnimState = STATE_LARA_SWANDIVE_BEGIN;

        if (item->fallspeed > LARA_FASTFALL_SPEED)
            item->goalAnimState = STATE_LARA_FREEFALL;
    }

    if (CHK_EXI(TrInput, IN_LEFT))
    {
        LaraClampN(Lara.turnRate, LARA_TURN_RATE, LARA_JUMP_TURN);
    }
    else if (CHK_EXI(TrInput, IN_RIGHT))
    {
        LaraClampP(Lara.turnRate, LARA_TURN_RATE, LARA_JUMP_TURN);
    }
}

void lara_as_fastback(ITEM_INFO* item, COLL_INFO* coll)
{
    item->goalAnimState = STATE_LARA_IDLE;

    if (CHK_EXI(TrInput, IN_LEFT))
    {
        LaraClampN(Lara.turnRate, LARA_TURN_RATE, LARA_MED_TURN);
    }
    else if (CHK_EXI(TrInput, IN_RIGHT))
    {
        LaraClampP(Lara.turnRate, LARA_TURN_RATE, LARA_MED_TURN);
    }
}

void lara_as_turnr(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_IDLE;
        return;
    }

    Lara.turnRate += LARA_TURN_RATE;
    if (Lara.gunStatus == LHS_READY && Lara.waterStatus != LWS_WADE)
    {
        item->goalAnimState = STATE_LARA_TURN_FAST;
    }
    else if (Lara.turnRate > LARA_SLOW_TURN)
    {
        if (CHK_NOP(TrInput, IN_WALK) && Lara.waterStatus != LWS_WADE)
            item->goalAnimState = STATE_LARA_TURN_FAST;
        else
            Lara.turnRate = LARA_SLOW_TURN;
    }

    if (CHK_EXI(TrInput, IN_FORWARD))
    {
        int water_level = -Lara.waterSurfaceDist;
        if (Lara.waterStatus == LWS_WADE && water_level > WADE_DEPTH)
        {
            item->goalAnimState = STATE_LARA_WADE_FORWARD;
        }
        else
        {
            if (CHK_EXI(TrInput, IN_WALK))
                item->goalAnimState = STATE_LARA_WALK_FORWARD;
            else
                item->goalAnimState = STATE_LARA_RUN_FORWARD;
        }
    }
    else if (CHK_NOP(TrInput, IN_RIGHT))
    {
        item->goalAnimState = STATE_LARA_IDLE;
    }
}

void lara_as_turnl(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_IDLE;
        return;
    }

    Lara.turnRate -= LARA_TURN_RATE;
    if (Lara.gunStatus == LHS_READY && Lara.waterStatus != LWS_WADE)
    {
        item->goalAnimState = STATE_LARA_TURN_FAST;
    }
    else if (Lara.turnRate < -LARA_SLOW_TURN)
    {
        if (CHK_NOP(TrInput, IN_WALK) && Lara.waterStatus != LWS_WADE)
            item->goalAnimState = STATE_LARA_TURN_FAST;
        else
            Lara.turnRate = -LARA_SLOW_TURN;
    }

    if (CHK_EXI(TrInput, IN_FORWARD))
    {
        int water_level = -Lara.waterSurfaceDist;
        if (Lara.waterStatus == LWS_WADE && water_level > WADE_DEPTH)
        {
            item->goalAnimState = STATE_LARA_WADE_FORWARD;
        }
        else
        {
            if (CHK_EXI(TrInput, IN_WALK))
                item->goalAnimState = STATE_LARA_WALK_FORWARD;
            else
                item->goalAnimState = STATE_LARA_RUN_FORWARD;
        }
    }
    else if (CHK_NOP(TrInput, IN_LEFT))
    {
        item->goalAnimState = STATE_LARA_IDLE;
    }
}

void lara_as_death(ITEM_INFO* item, COLL_INFO* coll)
{
    Lara.look = FALSE;
    coll->enableSpaz = FALSE;
    coll->enableBaddiePush = FALSE;

    if (BinocularRange)
    {
        BinocularRange = 0;
        LaserSight = 0;
        AlterFOV(DEFAULT_FOV);
        LaraItem->meshBits = ALL_MESH;
        Lara.busy = FALSE;
    }
}

void lara_as_fastfall(ITEM_INFO* item, COLL_INFO* coll)
{
    item->speed = FASTFALL_FRICTION(item);
    if (item->fallspeed == (DAMAGE_START + DAMAGE_LENGTH))
        SoundEffect(SFX_LARA_FALL, &LaraItem->pos, 0);
}

void lara_as_hang(ITEM_INFO* item, COLL_INFO* coll)
{
    Lara.isClimbing = FALSE;

    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_IDLE;
        return;
    }

    if (CHK_EXI(TrInput, IN_LOOK))
        LookUpDown();

    coll->enableSpaz = FALSE;
    coll->enableBaddiePush = FALSE;
    camera.target_angle = CAM_A_HANG;
    camera.target_elevation = CAM_E_HANG;
}

void lara_as_reach(ITEM_INFO* item, COLL_INFO* coll)
{
    camera.target_angle = ANGLE(85);
    if (item->fallspeed > LARA_FASTFALL_SPEED)
        item->goalAnimState = STATE_LARA_FREEFALL;
}

void lara_as_splat(ITEM_INFO* item, COLL_INFO* coll)
{
    Lara.look = FALSE;
}

void lara_as_compress(ITEM_INFO* item, COLL_INFO* coll)
{
    if (Lara.waterStatus != LWS_WADE)
    {
        if (CHK_EXI(TrInput, IN_FORWARD) && LaraFloorFront(item, item->pos.yRot, STEP_L) >= -STEPUP_HEIGHT)
        {
            item->goalAnimState = STATE_LARA_JUMP_FORWARD;
            Lara.moveAngle = item->pos.yRot;
        }
        else if (CHK_EXI(TrInput, IN_RIGHT) && LaraFloorFront(item, item->pos.yRot + 16384, STEP_L) >= -STEPUP_HEIGHT)
        {
            item->goalAnimState = STATE_LARA_JUMP_RIGHT;
            Lara.moveAngle = item->pos.yRot + 16384;
        }
        else if (CHK_EXI(TrInput, IN_LEFT) && LaraFloorFront(item, item->pos.yRot - 16384, STEP_L) >= -STEPUP_HEIGHT)
        {
            item->goalAnimState = STATE_LARA_JUMP_LEFT;
            Lara.moveAngle = item->pos.yRot - 16384;
        }
        else if (CHK_EXI(TrInput, IN_BACK) && LaraFloorFront(item, item->pos.yRot - 32768, STEP_L) >= -STEPUP_HEIGHT)
        {
            item->goalAnimState = STATE_LARA_JUMP_BACK;
            Lara.moveAngle = item->pos.yRot - 32768;
        }
    }

    if (item->fallspeed > LARA_FASTFALL_SPEED)
        item->goalAnimState = STATE_LARA_FREEFALL;
}

void lara_as_back(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_IDLE;
        return;
    }

    if (!Lara.isMoving)
    {
        if (CHK_EXI(TrInput, IN_BACK) && (CHK_EXI(TrInput, IN_WALK) || Lara.waterStatus == LWS_WADE))
            item->goalAnimState = STATE_LARA_WALK_BACK;
        else
            item->goalAnimState = STATE_LARA_IDLE;

        if (CHK_EXI(TrInput, IN_LEFT))
        {
            LaraClampN(Lara.turnRate, LARA_TURN_RATE, LARA_SLOW_TURN);
        }
        else if (CHK_EXI(TrInput, IN_RIGHT))
        {
            LaraClampP(Lara.turnRate, LARA_TURN_RATE, LARA_SLOW_TURN);
        }
    }
}

void lara_as_null(ITEM_INFO* item, COLL_INFO* coll)
{
    coll->enableSpaz = FALSE;
    coll->enableBaddiePush = FALSE;
}

void lara_as_fastturn(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_IDLE;
        return;
    }

    if (Lara.turnRate < 0)
    {
        Lara.turnRate = -LARA_FAST_TURN;
        if (CHK_NOP(TrInput, IN_LEFT))
            item->goalAnimState = STATE_LARA_IDLE;
    }
    else
    {
        Lara.turnRate = LARA_FAST_TURN;
        if (CHK_NOP(TrInput, IN_RIGHT))
            item->goalAnimState = STATE_LARA_IDLE;
    }
}

void lara_as_stepright(ITEM_INFO* item, COLL_INFO* coll)
{
    Lara.look = FALSE;

    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_IDLE;
        return;
    }

    if (CHK_NOP(TrInput, IN_RSTEP))
    {
        item->goalAnimState = STATE_LARA_IDLE;
    }

    if (CHK_EXI(TrInput, IN_LEFT))
    {
        LaraClampN(Lara.turnRate, LARA_TURN_RATE, LARA_SLOW_TURN);
    }
    else if (CHK_EXI(TrInput, IN_RIGHT))
    {
        LaraClampP(Lara.turnRate, LARA_TURN_RATE, LARA_SLOW_TURN);
    }
}

void lara_as_stepleft(ITEM_INFO* item, COLL_INFO* coll)
{
    Lara.look = FALSE;

    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_IDLE;
        return;
    }

    if (CHK_NOP(TrInput, IN_LSTEP))
    {
        item->goalAnimState = STATE_LARA_IDLE;
    }

    if (CHK_EXI(TrInput, IN_LEFT))
    {
        LaraClampN(Lara.turnRate, LARA_TURN_RATE, LARA_SLOW_TURN);
    }
    else if (CHK_EXI(TrInput, IN_RIGHT))
    {
        LaraClampP(Lara.turnRate, LARA_TURN_RATE, LARA_SLOW_TURN);
    }
}

void lara_as_slide(ITEM_INFO* item, COLL_INFO* coll)
{
    camera.target_elevation = -ANGLE(45);
    if (CHK_EXI(TrInput, IN_JUMP) && CHK_NOP(TrInput, IN_BACK))
    {
        item->goalAnimState = STATE_LARA_JUMP_FORWARD;
        item->pos.xRot = 0; // just in case.
    }
}

void lara_as_backjump(ITEM_INFO* item, COLL_INFO* coll)
{
    camera.target_angle = ANGLE(135);
    if (item->fallspeed > LARA_FASTFALL_SPEED)
        item->goalAnimState = STATE_LARA_FREEFALL;
    else if (item->goalAnimState == STATE_LARA_RUN_FORWARD)
        item->goalAnimState = STATE_LARA_IDLE;
    else if ((CHK_EXI(TrInput, IN_FORWARD) || CHK_EXI(TrInput, IN_ROLL)) && item->goalAnimState != STATE_LARA_IDLE)
        item->goalAnimState = STATE_LARA_JUMP_ROLL;
}

void lara_as_rightjump(ITEM_INFO* item, COLL_INFO* coll)
{
    Lara.look = FALSE;
    if (item->fallspeed > LARA_FASTFALL_SPEED)
        item->goalAnimState = STATE_LARA_FREEFALL;
    else if (CHK_EXI(TrInput, IN_LEFT) && item->goalAnimState != STATE_LARA_IDLE)
        item->goalAnimState = STATE_LARA_JUMP_ROLL;
}

void lara_as_leftjump(ITEM_INFO* item, COLL_INFO* coll)
{
    Lara.look = FALSE;
    if (item->fallspeed > LARA_FASTFALL_SPEED)
        item->goalAnimState = STATE_LARA_FREEFALL;
    else if (CHK_EXI(TrInput, IN_RIGHT) && item->goalAnimState != STATE_LARA_IDLE)
        item->goalAnimState = STATE_LARA_JUMP_ROLL;
}

void lara_as_upjump(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->fallspeed > LARA_FASTFALL_SPEED)
        item->goalAnimState = STATE_LARA_FREEFALL;
}

void lara_as_fallback(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->fallspeed > LARA_FASTFALL_SPEED)
        item->goalAnimState = STATE_LARA_FREEFALL;

    if (CHK_EXI(TrInput, IN_ACTION) && Lara.gunStatus == LHS_ARMLESS)
        item->goalAnimState = STATE_LARA_REACH;
}

void lara_as_hangleft(ITEM_INFO* item, COLL_INFO* coll)
{
    coll->enableSpaz = FALSE;
    coll->enableBaddiePush = FALSE;
    camera.target_angle = CAM_A_HANG;
    camera.target_elevation = CAM_E_HANG;
    if (CHK_NOP(TrInput, IN_LEFT) || CHK_NOP(TrInput, IN_LSTEP))
        item->goalAnimState = STATE_LARA_HANG;
}

void lara_as_hangright(ITEM_INFO* item, COLL_INFO* coll)
{
    coll->enableSpaz = FALSE;
    coll->enableBaddiePush = FALSE;
    camera.target_angle = CAM_A_HANG;
    camera.target_elevation = CAM_E_HANG;
    if (CHK_NOP(TrInput, IN_RIGHT) || CHK_NOP(TrInput, IN_RSTEP))
        item->goalAnimState = STATE_LARA_HANG;
}

void lara_as_slideback(ITEM_INFO* item, COLL_INFO* coll)
{
    if (CHK_EXI(TrInput, IN_JUMP) && CHK_NOP(TrInput, IN_FORWARD))
        item->goalAnimState = STATE_LARA_JUMP_BACK;
}

void lara_as_pushblock(ITEM_INFO* item, COLL_INFO* coll)
{
    Lara.look = FALSE;
    coll->enableSpaz = FALSE;
    coll->enableBaddiePush = FALSE;
    camera.flags = FOLLOW_CENTRE;
    camera.target_angle = ANGLE(45);
    camera.target_elevation = -ANGLE(25);
}

void lara_as_pullblock(ITEM_INFO* item, COLL_INFO* coll)
{
    Lara.look = FALSE;
    coll->enableSpaz = FALSE;
    coll->enableBaddiePush = FALSE;
    camera.flags = FOLLOW_CENTRE;
    camera.target_angle = ANGLE(45);
    camera.target_elevation = -ANGLE(25);
}

void lara_as_ppready(ITEM_INFO* item, COLL_INFO* coll)
{
    coll->enableSpaz = FALSE;
    coll->enableBaddiePush = FALSE;
    if (CHK_NOP(TrInput, IN_ACTION))
        item->goalAnimState = STATE_LARA_IDLE;
}

void lara_as_pickup(ITEM_INFO* item, COLL_INFO* coll)
{
    Lara.look = FALSE;
    coll->enableSpaz = FALSE;
    coll->enableBaddiePush = FALSE;
    camera.target_angle = -ANGLE(130);
    camera.target_elevation = -ANGLE(15);
    camera.target_distance = WALL_L;
}

void lara_as_switchon(ITEM_INFO* item, COLL_INFO* coll)
{
    Lara.look = FALSE;
    coll->enableSpaz = FALSE;
    coll->enableBaddiePush = FALSE;
    camera.target_angle = ANGLE(80);
    camera.target_elevation = -ANGLE(25);
    camera.target_distance = WALL_L;
    camera.speed = 6;
}

void lara_as_switchoff(ITEM_INFO* item, COLL_INFO* coll)
{
    Lara.look = FALSE;
    coll->enableSpaz = FALSE;
    coll->enableBaddiePush = FALSE;
    camera.target_angle = ANGLE(80);
    camera.target_elevation = -ANGLE(25);
    camera.target_distance = WALL_L;
    camera.speed = 6;
}

void lara_as_usekey(ITEM_INFO* item, COLL_INFO* coll)
{
    Lara.look = FALSE;
    coll->enableSpaz = FALSE;
    coll->enableBaddiePush = FALSE;
    camera.target_angle = -ANGLE(80);
    camera.target_elevation = -ANGLE(25);
    camera.target_distance = WALL_L;
}

void lara_as_usepuzzle(ITEM_INFO* item, COLL_INFO* coll)
{
    Lara.look = FALSE;
    coll->enableSpaz = FALSE;
    coll->enableBaddiePush = FALSE;
    camera.target_angle = -ANGLE(80);
    camera.target_elevation = -ANGLE(25);
    camera.target_distance = WALL_L;

    if (item->frameNumber == Anims[item->animNumber].frameEnd)
    {
        if (item->itemFlags[0])
            SetAnimationForItemAS(item, item->itemFlags[0], STATE_LARA_MISC_CONTROL);
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
    coll->enableSpaz = FALSE;
    coll->enableBaddiePush = TRUE;
    if (item->fallspeed > LARA_FASTFALL_SPEED && item->goalAnimState != STATE_LARA_UNDERWATER_DIVING)
        item->goalAnimState = STATE_LARA_SWANDIVE_END;
}

void lara_as_fastdive(ITEM_INFO* item, COLL_INFO* coll)
{
    if (CHK_EXI(TrInput, IN_ROLL) && item->goalAnimState == STATE_LARA_SWANDIVE_END)
        item->goalAnimState = STATE_LARA_JUMP_ROLL;
    coll->enableSpaz = FALSE;
    coll->enableBaddiePush = TRUE;
    item->speed = FASTFALL_FRICTION(item);
}

void lara_as_waterout(ITEM_INFO* item, COLL_INFO* coll)
{
    coll->enableSpaz = FALSE;
    coll->enableBaddiePush = FALSE;
    camera.flags = FOLLOW_CENTRE;
}

void lara_as_wade(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_IDLE;
        return;
    }

    camera.target_elevation = -ANGLE(22);
    if (CHK_EXI(TrInput, IN_LEFT))
    {
        LaraClampN(Lara.turnRate, LARA_TURN_RATE, LARA_FAST_TURN);
        LaraClampN(item->pos.zRot, LARA_LEAN_RATE, LARA_LEAN_MAX);
    }
    else if (CHK_EXI(TrInput, IN_RIGHT))
    {
        LaraClampP(Lara.turnRate, LARA_TURN_RATE, LARA_FAST_TURN);
        LaraClampP(item->pos.zRot, LARA_LEAN_RATE, LARA_LEAN_MAX);
    }

    if (CHK_EXI(TrInput, IN_FORWARD))
    {
        int water_level = -Lara.waterSurfaceDist;
        if (Lara.waterStatus == LWS_ABOVEWATER && water_level <= WADE_DEPTH)
            item->goalAnimState = STATE_LARA_RUN_FORWARD;
        else
            item->goalAnimState = STATE_LARA_WADE_FORWARD;
    }
    else
    {
        item->goalAnimState = STATE_LARA_IDLE;
    }
}

void lara_as_pickupflare(ITEM_INFO* item, COLL_INFO* coll)
{
    Lara.look = FALSE;
    coll->enableSpaz = FALSE;
    coll->enableBaddiePush = FALSE;
    camera.target_angle = ANGLE(130);
    camera.target_elevation = -ANGLE(15);
    camera.target_distance = WALL_L;
    if (item->frameNumber == (Anims[item->animNumber].frameEnd - 1))
        Lara.gunStatus = LHS_ARMLESS;
}

void lara_as_deathslide(ITEM_INFO* item, COLL_INFO* coll)
{
    camera.target_angle = ANGLE(70);
    TestTriggersCollision(item, coll);

    if (CHK_NOP(TrInput, IN_ACTION))
    {
        item->goalAnimState = STATE_LARA_JUMP_FORWARD;
        Lara.moveAngle = item->pos.yRot;
        LaraItem->gravityStatus = TRUE;
        LaraItem->speed = 100;
        LaraItem->fallspeed = 40;
        AnimateLara(item);
    }
}

void lara_as_duck(ITEM_INFO* item, COLL_INFO* coll)
{
    coll->enableSpaz = TRUE;
    coll->enableBaddiePush = TRUE;
    Lara.isDucked = TRUE;

    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_CRAWL_IDLE;
        return;
    }

    if (TestLaraSlide(item, coll))
    {
        return;
    }

    if (CHK_EXI(TrInput, IN_LOOK))
    {
        LookUpDown();
    }

    if (CHK_EXI(TrInput, IN_FORWARD) || CHK_EXI(TrInput, IN_BACK) && (CHK_EXI(TrInput, IN_DUCK) || Lara.keepDucked) && Lara.gunStatus == LHS_ARMLESS && Lara.waterStatus != LWS_WADE)
    {
        if ((item->animNumber == ANIMATION_LARA_CROUCH_IDLE || item->animNumber == ANIMATION_LARA_CROUCH_PREPARE)
        &&   CHK_NOP(TrInput, (IN_FLARE | IN_DRAW))
        &&  (Lara.gunType != LG_FLARE || (Lara.flareAge < 900 && Lara.flareAge)))
        {
            Lara.torsoYrot = 0;
            Lara.torsoXrot = 0;
            item->goalAnimState = STATE_LARA_CRAWL_IDLE;
        }
    }
}

void lara_as_dash(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_RUN_FORWARD;
        return;
    }

    int water_level = -Lara.waterSurfaceDist;
    if (!Lara.dashTimer || CHK_NOP(TrInput, IN_SPRINT) || (Lara.waterStatus == LWS_WADE && water_level > WADE_DEPTH))
    {
        item->goalAnimState = STATE_LARA_RUN_FORWARD;
        return;
    }

    Lara.dashTimer--;
    if (CHK_EXI(TrInput, IN_DUCK) && Lara.waterStatus != LWS_WADE)
    {
        if (Lara.gunStatus == LHS_ARMLESS
        ||  Lara.gunType == LG_UNARMED
        ||  Lara.gunType == LG_PISTOLS
        ||  Lara.gunType == LG_REVOLVER
        ||  Lara.gunType == LG_UZIS
        ||  Lara.gunType == LG_FLARE)
        {
            item->goalAnimState = STATE_LARA_CROUCH_IDLE;
            return;
        }
    }

    if (CHK_EXI(TrInput, IN_LEFT))
    {
        LaraClampN(Lara.turnRate, LARA_DASH_TURN_RATE, LARA_DASH_TURN_MAX);
        LaraClampN(item->pos.zRot, LARA_DASH_LEAN_RATE, LARA_DASH_LEAN_MAX);
    }
    else if (CHK_EXI(TrInput, IN_RIGHT))
    {
        LaraClampP(Lara.turnRate, LARA_DASH_TURN_RATE, LARA_DASH_TURN_MAX);
        LaraClampP(item->pos.zRot, LARA_DASH_LEAN_RATE, LARA_DASH_LEAN_MAX);
    }

    if (CHK_EXI(TrInput, IN_JUMP) && !item->gravityStatus)
    {
        item->goalAnimState = STATE_LARA_SPRINT_ROLL;
    }
    else if (CHK_EXI(TrInput, IN_FORWARD))
    {
        if (CHK_EXI(TrInput, IN_WALK))
            item->goalAnimState = STATE_LARA_WALK_FORWARD;
        else
            item->goalAnimState = STATE_LARA_SPRINT;
    }
    else if (CHK_NOP(TrInput, (IN_LEFT | IN_RIGHT)))
    {
        item->goalAnimState = STATE_LARA_IDLE;
    }
}

void lara_as_dashdive(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->goalAnimState != STATE_LARA_DEATH
    &&  item->goalAnimState != STATE_LARA_IDLE
    &&  item->goalAnimState != STATE_LARA_RUN_FORWARD)
    {
        if (item->fallspeed > LARA_FASTFALL_SPEED)
            item->goalAnimState = STATE_LARA_FREEFALL;
    }
}

void lara_as_hang2(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_IDLE;
        return;
    }

    coll->enableSpaz = FALSE;
    coll->enableBaddiePush = FALSE;
    Lara.torsoYrot = 0;
    Lara.torsoXrot = 0;

    if (Lara.canMonkeySwing)
    {
        if (CHK_NOP(TrInput, IN_ACTION) || item->hitPoints <= 0)
        {
            MonkeySwingFall(item);
            return;
        }

        camera.target_angle = CAM_A_HANG;
        camera.target_elevation = CAM_E_HANG; // TODO: fix it ! (-ANGLE(60))
    }
    else
    {
        if (CHK_EXI(TrInput, IN_LOOK))
            LookUpDown();
    }
}

void lara_as_monkeyswing(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_MONKEYSWING_IDLE;
        return;
    }

    coll->enableSpaz = FALSE;
    coll->enableBaddiePush = FALSE;
    Lara.torsoYrot = 0;
    Lara.torsoXrot = 0;
    // TODO: there is camera.target_angle = 0, camera.target_elevation = ANGLE(10);

    if (CHK_EXI(TrInput, IN_LOOK))
        LookUpDown();

    if (CHK_EXI(TrInput, IN_FORWARD))
        item->goalAnimState = STATE_LARA_MONKEYSWING_FORWARD;
    else
        item->goalAnimState = STATE_LARA_MONKEYSWING_IDLE;

    if (CHK_EXI(TrInput, IN_LEFT))
    {
        LaraClampN(Lara.turnRate, LARA_TURN_RATE, LARA_JUMP_TURN);
    }
    else if (CHK_EXI(TrInput, IN_RIGHT))
    {
        LaraClampP(Lara.turnRate, LARA_TURN_RATE, LARA_JUMP_TURN);
    }
}

void lara_as_monkeyl(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_MONKEYSWING_IDLE;
        return;
    }

    coll->enableSpaz = FALSE;
    coll->enableBaddiePush = FALSE;
    Lara.torsoYrot = 0;
    Lara.torsoXrot = 0;

    if (CHK_EXI(TrInput, IN_LSTEP))
        item->goalAnimState = STATE_LARA_MONKEYSWING_LEFT;
    else
        item->goalAnimState = STATE_LARA_MONKEYSWING_IDLE;

    camera.target_elevation = ANGLE(10);
}

void lara_as_monkeyr(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_MONKEYSWING_IDLE;
        return;
    }

    coll->enableSpaz = FALSE;
    coll->enableBaddiePush = FALSE;
    Lara.torsoYrot = 0;
    Lara.torsoXrot = 0;

    if (CHK_EXI(TrInput, IN_RSTEP))
        item->goalAnimState = STATE_LARA_MONKEYSWING_RIGHT;
    else
        item->goalAnimState = STATE_LARA_MONKEYSWING_IDLE;

    camera.target_elevation = ANGLE(10);
}

void lara_as_monkey180(ITEM_INFO* item, COLL_INFO* coll)
{
    coll->enableSpaz = FALSE;
    coll->enableBaddiePush = FALSE;
    item->goalAnimState = STATE_LARA_MONKEYSWING_IDLE;
}

void lara_as_all4s(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_DEATH;
        return;
    }

    if (TrInput & IN_LOOK)
        LookUpDown();

    Lara.torsoYrot = 0;
    Lara.torsoXrot = 0;
    coll->enableSpaz = FALSE;
    coll->enableBaddiePush = TRUE;

    if (item->animNumber == ANIMATION_LARA_CROUCH_TO_CRAWL_BEGIN)
        Lara.gunStatus = LHS_HANDBUSY;

    camera.target_elevation = -ANGLE(23);
}

void lara_as_crawl(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_CRAWL_IDLE;
        return;
    }

    if (TrInput & IN_LOOK)
        LookUpDown();

    Lara.torsoYrot = 0;
    Lara.torsoXrot = 0;
    coll->enableSpaz = FALSE;
    coll->enableBaddiePush = TRUE;
    camera.target_elevation = -ANGLE(23);

    if (CHK_EXI(TrInput, IN_FORWARD) && (CHK_EXI(TrInput, IN_DUCK) || Lara.keepDucked) && Lara.waterStatus != LWS_WADE)
    {
        if (CHK_EXI(TrInput, IN_LEFT))
        {
            LaraClampN(Lara.turnRate, LARA_TURN_RATE, LARA_JUMP_TURN);
        }
        else if (CHK_EXI(TrInput, IN_RIGHT))
        {
            LaraClampP(Lara.turnRate, LARA_TURN_RATE, LARA_JUMP_TURN);
        }
    }
    else
    {
        item->goalAnimState = STATE_LARA_CRAWL_IDLE;
    }
}

void lara_as_hangturnl(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_MONKEYSWING_IDLE;
        return;
    }

    camera.target_elevation = ANGLE(10);
    Lara.torsoYrot = 0;
    Lara.torsoXrot = 0;
    item->pos.yRot -= (ONE_DEGREE + (ONE_DEGREE / 2));

    if (CHK_NOP(TrInput, IN_LEFT))
        item->goalAnimState = STATE_LARA_MONKEYSWING_IDLE;
}

void lara_as_hangturnr(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_MONKEYSWING_IDLE;
        return;
    }

    camera.target_elevation = ANGLE(10);
    Lara.torsoYrot = 0;
    Lara.torsoXrot = 0;
    item->pos.yRot += (ONE_DEGREE + (ONE_DEGREE / 2));

    if (CHK_NOP(TrInput, IN_RIGHT))
        item->goalAnimState = STATE_LARA_MONKEYSWING_IDLE;
}

void lara_as_all4turnl(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_CRAWL_IDLE;
        return;
    }

    camera.target_elevation = -ANGLE(23);
    Lara.torsoYrot = 0;
    Lara.torsoXrot = 0;
    coll->enableSpaz = FALSE;
    coll->enableBaddiePush = TRUE;
    item->pos.yRot -= (ONE_DEGREE + (ONE_DEGREE / 2));

    if (CHK_NOP(TrInput, IN_LEFT))
        item->goalAnimState = STATE_LARA_CRAWL_IDLE;
}

void lara_as_all4turnr(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_CRAWL_IDLE;
        return;
    }

    camera.target_elevation = -ANGLE(23);
    Lara.torsoYrot = 0;
    Lara.torsoXrot = 0;
    coll->enableSpaz = FALSE;
    coll->enableBaddiePush = TRUE;
    item->pos.yRot += (ONE_DEGREE + (ONE_DEGREE / 2));

    if (CHK_NOP(TrInput, IN_RIGHT))
        item->goalAnimState = STATE_LARA_CRAWL_IDLE;
}

void lara_as_crawlb(ITEM_INFO* item, COLL_INFO* coll)
{
    int water_level = -Lara.waterSurfaceDist;
    if (item->hitPoints <= 0 || (Lara.waterStatus == LWS_WADE && water_level > WADE_DEPTH))
    {
        item->goalAnimState = STATE_LARA_CRAWL_IDLE;
        return;
    }

    if (CHK_EXI(TrInput, IN_LOOK))
        LookUpDown();

    coll->enableSpaz = FALSE;
    coll->enableBaddiePush = TRUE;
    Lara.torsoYrot = 0;
    Lara.torsoXrot = 0;
    camera.target_elevation = -ANGLE(23);

    if (CHK_NOP(TrInput, IN_BACK))
    {
        item->goalAnimState = STATE_LARA_CRAWL_IDLE;
    }
    else if (CHK_EXI(TrInput, IN_LEFT))
    {
        LaraClampP(Lara.turnRate, LARA_TURN_RATE, LARA_JUMP_TURN);
    }
    else if (CHK_EXI(TrInput, IN_RIGHT))
    {
        LaraClampN(Lara.turnRate, LARA_TURN_RATE, LARA_JUMP_TURN);
    }
}

void lara_as_controlled(ITEM_INFO* item, COLL_INFO* coll)
{
    Lara.look = FALSE;
    coll->enableSpaz = FALSE;
    coll->enableBaddiePush = FALSE;

    if (item->animNumber == ANIMATION_LARA_HARP_PLAY && (item->frameNumber == Anims[ANIMATION_LARA_HARP_PLAY].frameBase + 130))
    {
        S_CDPlay(6, FALSE);
    }

    if (item->animNumber == ANIMATION_LARA_DETONATOR_USE)
    {
        short meshIndex;
        if (item->frameNumber == (Anims[ANIMATION_LARA_DETONATOR_USE].frameBase + 16))
            meshIndex = Objects[MESHSWAP3].meshIndex;
        else if (item->frameNumber == (Anims[ANIMATION_LARA_DETONATOR_USE].frameBase + 118))
            meshIndex = Objects[LARA_SKIN].meshIndex;
        Lara.mesh.hand_r = meshes[meshIndex + HAND_R * 2];
    }

    if (item->frameNumber == (Anims[item->animNumber].frameEnd - 1))
    {
        if (item->animNumber == ANIMATION_LARA_HARP_PLAY)
            S_CDPlay(19, FALSE);
        Lara.gunStatus = LHS_ARMLESS;
        if (force_fixed_camera)
            force_fixed_camera = FALSE;
    }
}

void lara_as_ropel(ITEM_INFO* item, COLL_INFO* coll)
{
    if (CHK_EXI(TrInput, IN_ACTION))
    {
        if (CHK_EXI(TrInput, IN_LEFT))
            Lara.ropeY += STEP_L;
        else
            item->goalAnimState = STATE_LARA_ROPE_IDLE;
    }
    else
    {
        FallFromRope(item);
    }
}

void lara_as_roper(ITEM_INFO* item, COLL_INFO* coll)
{
    if (CHK_EXI(TrInput, IN_ACTION))
    {
        if (CHK_EXI(TrInput, IN_RIGHT))
            Lara.ropeY -= STEP_L;
        else
            item->goalAnimState = STATE_LARA_ROPE_IDLE;
    }
    else
    {
        FallFromRope(item);
    }
}

void lara_as_controlledl(ITEM_INFO* item, COLL_INFO* coll)
{
    Lara.look = FALSE;
    coll->enableSpaz = FALSE;
    coll->enableBaddiePush = FALSE;
}

void lara_as_poleleft(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_POLE_IDLE;
        return;
    }

    coll->enableSpaz = FALSE;
    coll->enableBaddiePush = FALSE;

    if (CHK_NOP(TrInput, IN_LEFT) || CHK_NOP(TrInput, IN_ACTION) || CHK_EXI(TrInput, (IN_FORWARD | IN_BACK)))
        item->goalAnimState = STATE_LARA_POLE_IDLE;
    else
        item->pos.yRot += STEP_L;
}

void lara_as_poleright(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_POLE_IDLE;
        return;
    }

    coll->enableSpaz = FALSE;
    coll->enableBaddiePush = FALSE;

    if (CHK_NOP(TrInput, IN_RIGHT) || CHK_NOP(TrInput, IN_ACTION) || CHK_EXI(TrInput, (IN_FORWARD | IN_BACK)))
        item->goalAnimState = STATE_LARA_POLE_IDLE;
    else
        item->pos.yRot -= STEP_L;
}

void lara_as_pulley(ITEM_INFO* item, COLL_INFO* coll)
{
    if (Lara.generalPtr == nullptr) // Dont assign it to ITEM_INFO pointer if general_ptr is NULL !
        return;
    ITEM_INFO* gen = (ITEM_INFO*)Lara.generalPtr;

    Lara.look = FALSE;
    coll->enableSpaz = FALSE;
    coll->enableBaddiePush = FALSE;

    if (CHK_EXI(TrInput, IN_ACTION) && gen->triggerFlags)
        item->goalAnimState = STATE_LARA_PULLEY;
    else
        item->goalAnimState = STATE_LARA_IDLE;

    if (item->animNumber == ANIMATION_LARA_PULLEY_PULL && item->frameNumber == (Anims[ANIMATION_LARA_PULLEY_PULL].frameBase + 44))
    {
        if (gen->triggerFlags && !gen->itemFlags[1])
        {
            gen->triggerFlags--;

            if (gen->triggerFlags && gen->itemFlags[2])
            {
                gen->itemFlags[2] = 0;
                gen->status = FITEM_DEACTIVATED;
            }
            else
            {
                gen->status = FITEM_DEACTIVATED;
                gen->itemFlags[2] = 1;
                if (gen->itemFlags[3] >= 0)
                    gen->triggerFlags = abs(gen->itemFlags[3]);
                else
                    gen->itemFlags[0] = 1;
            }
        }
    }

    if (item->animNumber == ANIMATION_LARA_PULLEY_UNGRAB && item->frameNumber == (Anims[ANIMATION_LARA_PULLEY_UNGRAB].frameEnd - 1))
        Lara.gunStatus = LHS_ARMLESS;
}

void lara_as_duckl(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_CROUCH_IDLE;
        return;
    }

    if (CHK_NOP(TrInput, IN_LEFT) || CHK_NOP(TrInput, IN_DUCK))
        item->goalAnimState = STATE_LARA_CROUCH_IDLE;

    item->pos.yRot -= (ONE_DEGREE + (ONE_DEGREE / 2));
}

void lara_as_duckr(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->hitPoints <= 0)
    {
        item->goalAnimState = STATE_LARA_CROUCH_IDLE;
        return;
    }

    if (CHK_NOP(TrInput, IN_RIGHT) || CHK_NOP(TrInput, IN_DUCK))
        item->goalAnimState = STATE_LARA_CROUCH_IDLE;

    item->pos.yRot += (ONE_DEGREE + (ONE_DEGREE / 2));
}

void lara_as_extcornerl(ITEM_INFO* item, COLL_INFO* coll)
{
    camera.target_angle = 0x4000;
    camera.target_elevation = -ANGLE(33);
    SetCornerAnim(item, coll, camera.target_angle, item->animNumber == ANIMATION_LARA_HANG_AROUND_LEFT_OUTER_END || item->animNumber == ANIMATION_LARA_LADDER_AROUND_LEFT_OUTER_END);
}

void lara_as_extcornerr(ITEM_INFO* item, COLL_INFO* coll)
{
    camera.target_angle = -0x4000;
    camera.target_elevation = -ANGLE(33);
    SetCornerAnim(item, coll, camera.target_angle, item->animNumber == ANIMATION_LARA_HANG_AROUND_RIGHT_OUTER_END || item->animNumber == ANIMATION_LARA_LADDER_AROUND_RIGHT_OUTER_END);
}

void lara_as_intcornerl(ITEM_INFO* item, COLL_INFO* coll)
{
    camera.target_angle = -0x4000;
    camera.target_elevation = -ANGLE(33);
    SetCornerAnim(item, coll, camera.target_angle, item->animNumber == ANIMATION_LARA_HANG_AROUND_LEFT_INNER_END || item->animNumber == ANIMATION_LARA_LADDER_AROUND_LEFT_INNER_END);
}

void lara_as_intcornerr(ITEM_INFO* item, COLL_INFO* coll)
{
    camera.target_angle = 0x4000;
    camera.target_elevation = -ANGLE(33);
    SetCornerAnim(item, coll, camera.target_angle, item->animNumber == ANIMATION_LARA_HANG_AROUND_RIGHT_INNER_END || item->animNumber == ANIMATION_LARA_LADDER_AROUND_RIGHT_INNER_END);
}

void lara_as_rope(ITEM_INFO* item, COLL_INFO* coll)
{
    if (CHK_NOP(TrInput, IN_ACTION))
        FallFromRope(item);

    if (CHK_EXI(TrInput, IN_LOOK))
        LookUpDown();
}

void lara_as_climbrope(ITEM_INFO* item, COLL_INFO* coll)
{
    if (CHK_EXI(TrInput, IN_ROLL))
    {
        FallFromRope(item);
        return;
    }

    camera.target_angle = ANGLE(30);

    if (item->frameNumber == Anims[item->animNumber].frameEnd)
    {
        item->frameNumber = Anims[item->animNumber].frameBase;
        Lara.ropeSegment -= 2;
    }

    if (CHK_NOP(TrInput, IN_FORWARD) || Lara.ropeSegment <= 4)
        item->goalAnimState = STATE_LARA_ROPE_IDLE;
}

void lara_as_climbroped(ITEM_INFO* item, COLL_INFO* coll)
{
    if (CHK_NOP(TrInput, IN_ACTION))
    {
        FallFromRope(item);
        return;
    }

    camera.target_angle = ANGLE(30);

    if (Lara.ropeCount && !Lara.ropeFlag)
    {
        Lara.ropeCount--;
        Lara.ropeOffset += Lara.ropeDownVel;
        Lara.ropeFlag = 1;
        return;
    }
    else if (!Lara.ropeFlag)
    {
        ROPE_STRUCT* rope = &ropes[Lara.ropePtr];
        Lara.ropeOffset = 0;
        Lara.ropeDownVel = (DWORD)(rope->mesh[Lara.ropeSegment + 1].y - rope->mesh[Lara.ropeSegment].y) >> 17;
        Lara.ropeCount = 0;
        Lara.ropeOffset += Lara.ropeDownVel;
        Lara.ropeFlag = 1;
        return;
    }

    if (item->animNumber == ANIMATION_LARA_ROPE_DOWN && item->frameNumber == Anims[ANIMATION_LARA_ROPE_DOWN].frameEnd)
    {
        item->frameNumber = Anims[ANIMATION_LARA_ROPE_DOWN].frameBase;
        Lara.ropeFlag = 0;
        Lara.ropeSegment++;
        Lara.ropeOffset = 0;
    }

    if (CHK_NOP(TrInput, IN_BACK) || Lara.ropeSegment >= 21)
        item->goalAnimState = STATE_LARA_ROPE_IDLE;
}