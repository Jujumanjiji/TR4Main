#include "framework.h"
#include "lara.h"
#include "3d_gen.h"
#include "draw.h"
#include "camera.h"
#include "collide.h"
#include "control.h"
#include "items.h"
#include "rope.h"
#include "sound.h"
#include "utils.h"

short LaraFloorFront(ITEM_INFO* item, short angle, int distance)
{
    FLOOR_INFO* floor;
    int newX, newY, newZ;
    int sin, cos;
    int height;
    short roomNumber;

    sin = (distance * SIN(angle)) >> W2V_SHIFT;
    cos = (distance * COS(angle)) >> W2V_SHIFT;
    newX = item->pos.xPos + sin;
    newY = item->pos.yPos - LARA_HITE;
    newZ = item->pos.zPos + cos;
    roomNumber = item->roomNumber;
    floor = GetFloor(newX, newY, newZ, &roomNumber);
    height = GetFloorHeight(floor, newX, newY, newZ);
    if (height != -NO_HEIGHT)
        height -= item->pos.yPos;
    return short(height);
}

short LaraCeilingFront(ITEM_INFO *item, short angle, int distance, int height)
{
    FLOOR_INFO* floor;
    int newX, newY, newZ;
    int sin, cos;
    int ceiling;
    short roomNumber;

    sin = (distance * SIN(angle)) >> W2V_SHIFT;
    cos = (distance * COS(angle)) >> W2V_SHIFT;
    newX = item->pos.xPos + sin;
    newY = item->pos.yPos - height;
    newZ = item->pos.zPos + cos;
    roomNumber = item->roomNumber;
    floor = GetFloor(newX, newY, newZ, &roomNumber);
    ceiling = GetCeiling(floor, newX, newY, newZ);
    if (ceiling != -NO_HEIGHT)
        ceiling += newY;
    return short(ceiling);
}

BOOL LaraHitCeiling(ITEM_INFO* item, COLL_INFO* coll)
{
    if (coll->collType != COLL_TOP || coll->collType != COLL_CLAMP)
        return FALSE;

    item->pos.xPos = coll->old.x;
    item->pos.yPos = coll->old.y;
    item->pos.zPos = coll->old.z;
    item->speed = 0;
    item->fallspeed = 0;
    item->gravityStatus = FALSE;
    SetAnimationForItemAS(item, ANIMATION_LARA_STAY_SOLID, STATE_LARA_IDLE);
    return TRUE;
}

BOOL LaraHangTest(ITEM_INFO* item, COLL_INFO* coll)
{
    bool hit;
    bool isLeftRightBlocked;
    int xColl = 0, zColl = 0;
    short* bounds;
    short coll_dist;
    short front, ceiling;
    short old_coll_front, coll_front;
    short old_move_angle;

    hit = false;
    old_move_angle = Lara.moveAngle;

    // always 0 for coll_dist ?
    if (Lara.moveAngle == (item->pos.yRot - 0x4000))
        coll_dist = -LARA_RAD;
    else if (Lara.moveAngle == (item->pos.yRot + 0x4000))
        coll_dist = LARA_RAD;
    else
        coll_dist = 0;

    front = LaraFloorFront(item, Lara.moveAngle, LARA_RAD);
    if (front < 200)
        hit = true;

    ceiling = LaraCeilingFront(item, Lara.moveAngle, LARA_RAD, 0);
    SHORT angle = (USHORT)(item->pos.yRot + 0x2000) / 0x4000;
    // allow lara to grab (else it's a loop grab/ungrab)
    switch (angle)
    {
        case NORTH:
            item->pos.zPos += 4;
            break;
        case EAST:
            item->pos.xPos += 4;
            break;
        case SOUTH:
            item->pos.zPos -= 4;
            break;
        case WEST:
            item->pos.xPos -= 4;
            break;
    }

    Lara.moveAngle = item->pos.yRot;
    coll->badPos = NO_HEIGHT;
    coll->badNeg = -STEPUP_HEIGHT;
    coll->badCeiling = 0;
    GetLaraCollisionInfo(item, coll);

    if (Lara.climbStatus)
    {
        if (CHK_NOP(TrInput, IN_ACTION) || item->hitPoints <= 0)
        {
            SetAnimationForItemAS(item, ANIMATION_LARA_FREE_FALL_FORWARD, STATE_LARA_JUMP_FORWARD);
            item->pos.yPos += STEP_L;
            item->speed = 2;
            item->fallspeed = 1;
            item->gravityStatus = TRUE;
            Lara.gunStatus = LHS_ARMLESS;
            return FALSE;
        }

        Lara.moveAngle = old_move_angle;
        
        if (!LaraTestHangOnClimbWall(item, coll))
        {
            if (item->animNumber != ANIMATION_LARA_LADDER_TO_HANDS_LEFT && item->animNumber != ANIMATION_LARA_LADDER_TO_HANDS_RIGHT)
            {
                SnapLaraToEdgeOfBlock(item, coll, angle);
                item->pos.yPos = coll->old.y;
                SetAnimationForItemASF(item, ANIMATION_LARA_HANG_IDLE, STATE_LARA_HANG, 21);
            }
            return TRUE;
        }
        else if (item->animNumber == ANIMATION_LARA_HANG_IDLE && item->frameNumber == (Anims[ANIMATION_LARA_HANG_IDLE].frameBase + 21) && LaraTestClimbStance(item, coll))
            item->goalAnimState = STATE_LARA_LADDER_IDLE;

        return FALSE;
    }

    if (CHK_NOP(TrInput, IN_ACTION) || item->hitPoints <= 0 || coll->front.floor > 0)
    {
        SetAnimationForItemASF(item, ANIMATION_LARA_TRY_HANG_VERTICAL, STATE_LARA_JUMP_UP, 9);
        item->pos.xPos += coll->shift.x;
        item->pos.yPos = coll->old.y + (STEP_L / 2); // more accurate than bounds collision (STEP_L/2 fix the height when action released!)
        item->pos.zPos += coll->shift.z;
        item->speed = 2;
        item->fallspeed = 1;
        item->gravityStatus = TRUE;
        Lara.gunStatus = LHS_ARMLESS;
        return FALSE;
    }

    if (hit && front > 0)
    {
        if (coll_dist > 0 && coll->frontLeft.floor > coll->frontRight.floor)
            hit = false;
        else if (coll_dist < 0 && coll->frontLeft.floor < coll->frontRight.floor)
            hit = false;
    }

    bounds = GetBoundsAccurate(item);
    old_coll_front = coll->front.floor;
    coll_front = coll->front.floor - bounds[2];
    isLeftRightBlocked = false;
    switch (angle)
    {
        case NORTH:
            xColl = item->pos.xPos + coll_dist;
            zColl = item->pos.zPos;
            break;
        case EAST:
            xColl = item->pos.xPos;
            zColl = item->pos.zPos - coll_dist;
            break;
        case SOUTH:
            xColl = item->pos.xPos - coll_dist;
            zColl = item->pos.zPos;
            break;
        case WEST:
            xColl = item->pos.xPos;
            zColl = item->pos.zPos + coll_dist;
            break;
    }

    Lara.moveAngle = old_move_angle;
    if (GetClimbTrigger(xColl, item->pos.yPos, zColl, item->roomNumber) & (STEP_L << angle))
    {
        if (!LaraTestHangOnClimbWall(item, coll))
            coll_front = 0;
    }
    else
    {
        if (abs(coll->backLeft.floor - coll->backRight.floor) >= SLOPE_DIF)
        {
            if ((coll_dist < 0 || coll_dist > 0)
            && ((coll->backLeft.floor  != coll->front.floor)
            ||  (coll->backRight.floor != coll->front.floor)))
            {
                isLeftRightBlocked = true;
            }
            else if (coll_dist == 0 // this "else if" not exist ! so i added it.
            &&     ((coll->backLeft.floor != coll->front.floor)
            ||     (coll->backRight.floor != coll->front.floor)))
            {
                isLeftRightBlocked = true;
            }
        }
    }

    coll->front.floor = old_coll_front;
    if (isLeftRightBlocked
    ||  coll->middle.ceiling >= 0
    ||  coll->collType != COLL_FRONT
    ||  hit
    ||  coll->hitStatic
    ||  ceiling > -950
    ||  coll_front < -SLOPE_DIF
    ||  coll_front > SLOPE_DIF)
    {
        item->pos.xPos = coll->old.x;
        item->pos.yPos = coll->old.y;
        item->pos.zPos = coll->old.z;
        if (item->currentAnimState == STATE_LARA_SHIMMY_LEFT || item->currentAnimState == STATE_LARA_SHIMMY_RIGHT)
            SetAnimationForItemASF(item, ANIMATION_LARA_HANG_IDLE, STATE_LARA_HANG, LARA_HANG_IDLE_FRAME_AFTERLEFTRIGHT);
        return TRUE;
    }

    switch (angle)
    {
        case NORTH:
        case SOUTH:
            item->pos.zPos += coll->shift.z;
            item->pos.yPos += coll_front;
            break;
        case EAST:
        case WEST:
            item->pos.xPos += coll->shift.x;
            item->pos.yPos += coll_front;
            break;
        default:// just in case since it's on default in idapro...
            item->pos.yPos += coll_front;
            break;
    }

    // return false by default there and not in switch !
    return FALSE;
}

BOOL LaraDeflectEdge(ITEM_INFO* item, COLL_INFO* coll)
{
    switch (coll->collType)
    {
        case COLL_FRONT:
        case COLL_TOPFRONT:
            ShiftItem(item, coll);
            item->goalAnimState = STATE_LARA_IDLE;
            item->speed = 0;
            item->gravityStatus = FALSE;
            return TRUE;

        case COLL_LEFT:
            ShiftItem(item, coll);
            item->pos.yRot += ANGLE(5);
            break;
        case COLL_RIGHT:
            ShiftItem(item, coll);
            item->pos.yRot -= ANGLE(5);
            break;
    }

    return FALSE;
}

BOOL LaraDeflectEdgeDuck(ITEM_INFO* item, COLL_INFO* coll)
{
    switch (coll->collType)
    {
        case COLL_FRONT:
        case COLL_TOPFRONT:
            ShiftItem(item, coll);
            item->speed = 0;
            item->gravityStatus = FALSE;
            return TRUE;

        case COLL_LEFT:
            ShiftItem(item, coll);
            item->pos.yRot += ANGLE(2);
            break;
        case COLL_RIGHT:
            ShiftItem(item, coll);
            item->pos.yRot -= ANGLE(2);
            break;
    }

    return FALSE;
}

void LaraDeflectEdgeJump(ITEM_INFO* item, COLL_INFO* coll)
{
    ShiftItem(item, coll);
    switch (coll->collType)
    {
        case COLL_FRONT:
        case COLL_TOPFRONT:
            if (!Lara.climbStatus || item->speed != 2)
            {
                if (coll->middle.floor <= (STEP_L / 2))
                    SetAnimationForItemAS(item, ANIMATION_LARA_LANDING_LIGHT, STATE_LARA_GRAB_TO_FALL);
                else
                    SetAnimationForItemASF(item, ANIMATION_LARA_SMASH_JUMP, STATE_LARA_FREEFALL, 1);
                item->speed /= 4;
                Lara.moveAngle = item->pos.yRot - 0x8000;
                if (item->fallspeed <= 0)
                    item->fallspeed = 1;
            }
            break;

        case COLL_LEFT:
            item->pos.yRot += ANGLE(5);
            break;
        case COLL_RIGHT:
            item->pos.yRot -= ANGLE(5);
            break;

        case COLL_TOP:
            if (item->fallspeed <= 0)
                item->fallspeed = 1;
            break;

        case COLL_CLAMP:
            item->pos.xPos -= LARA_RAD * SIN(coll->facing) >> W2V_SHIFT;
            item->pos.zPos -= LARA_RAD * COS(coll->facing) >> W2V_SHIFT;
            item->speed = 0;
            coll->middle.floor = 0;
            if (item->fallspeed <= 0)
                item->fallspeed = 16;
            break;
    }
}

BOOL LaraFallen(ITEM_INFO* item, COLL_INFO* coll)
{
    if (Lara.waterStatus != LWS_WADE && coll->middle.floor > STEPUP_HEIGHT)
    {
        SetAnimationForItemAS(item, ANIMATION_LARA_FREE_FALL_FORWARD, STATE_LARA_JUMP_FORWARD);
        item->fallspeed = 0;
        item->gravityStatus = TRUE;
        return TRUE;
    }

    return FALSE;
}

int IsValidHangPos(ITEM_INFO* item, COLL_INFO* coll)
{
    SHORT angle;
    short front;

    front = LaraFloorFront(item, Lara.moveAngle, LARA_RAD);
    if (front < 200)
        return FALSE;

    angle = (USHORT)(item->pos.yRot + 0x2000) / 0x4000;
    // allow lara to grab (else it's a loop grab/ungrab)
    switch (angle)
    {
        case NORTH:
            item->pos.zPos += 4;
            break;
        case EAST:
            item->pos.xPos += 4;
            break;
        case SOUTH:
            item->pos.zPos -= 4;
            break;
        case WEST:
            item->pos.xPos -= 4;
            break;
    }

    Lara.moveAngle = item->pos.yRot;
    coll->badPos = NO_HEIGHT;
    coll->badNeg = -(STEP_L * 2);
    coll->badCeiling = 0;
    GetLaraCollisionInfo(item, coll);

    if (coll->middle.ceiling >= 0 || coll->collType != COLL_FRONT || coll->hitStatic)
        return FALSE;

    if (abs(coll->front.floor - coll->backRight.floor) >= SLOPE_DIF)
        return FALSE;
    else
        return TRUE;
}

int LaraHangRightCornerTest(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->animNumber != ANIMATION_LARA_HANG_IDLE)
        return 0;
    if (coll->hitStatic)
        return 0;

    int x, z;
    int old_x = item->pos.xPos;
    int old_z = item->pos.zPos;
    int old_front = coll->front.floor;
    SHORT old_angle = item->pos.yRot;
    SHORT angle = (USHORT)(item->pos.yRot + ANGLE(45)) / ANGLE(90);

    if (angle && angle != 2)
    {
        x = (item->pos.xPos & ~(WALL_L - 1)) - (item->pos.zPos & (WALL_L - 1)) + SECTOR(1);
        z = (item->pos.zPos & ~(WALL_L - 1)) - (item->pos.xPos & (WALL_L - 1)) + SECTOR(1);
    }
    else
    {
        x = item->pos.xPos ^ (item->pos.xPos ^ item->pos.zPos) & (WALL_L - 1);
        z = item->pos.zPos ^ (item->pos.xPos ^ item->pos.zPos) & (WALL_L - 1);
    }

    item->pos.xPos = x;
    Lara.cornerX = x;
    item->pos.zPos = z;
    Lara.cornerZ = z;
    item->pos.yRot += ANGLE(90);
    int result = -IsValidHangPos(item, coll);
    if (result)
    {
        if (Lara.climbStatus)
        {
            if (GetClimbTrigger(x, item->pos.yPos, z, item->roomNumber) & LeftClimbTab[angle])
            {
                item->pos.xPos = old_x;
                item->pos.zPos = old_z;
                item->pos.yRot = old_angle;
                Lara.moveAngle = old_angle;
                return result;
            }
        }
        else
        {
            if (abs(old_front - coll->front.floor) <= SLOPE_DIF)
            {
                item->pos.xPos = old_x;
                item->pos.zPos = old_z;
                item->pos.yRot = old_angle;
                Lara.moveAngle = old_angle;
                return result;
            }
        }
    }

    item->pos.xPos = old_x;
    item->pos.zPos = old_z;
    item->pos.yRot = old_angle;
    Lara.moveAngle = old_angle;
    if (LaraFloorFront(item, old_angle + ANGLE(90), LARA_RAD + 16) < 0)
        return 0;

    switch (angle)
    {
        case NORTH:
            x = ((item->pos.xPos + SECTOR(1)) & ~(WALL_L - 1)) - (item->pos.zPos & (WALL_L - 1)) + SECTOR(1);
            z = ((item->pos.zPos + SECTOR(1)) & ~(WALL_L - 1)) - (item->pos.xPos & (WALL_L - 1)) + SECTOR(1);
            break;
        case SOUTH:
            x = ((item->pos.xPos - SECTOR(1)) & ~(WALL_L - 1)) - (item->pos.zPos & (WALL_L - 1)) + SECTOR(1);
            z = ((item->pos.zPos - SECTOR(1)) & ~(WALL_L - 1)) - (item->pos.xPos & (WALL_L - 1)) + SECTOR(1);
            break;
        case WEST:
            x = (item->pos.xPos ^ (item->pos.xPos ^ item->pos.zPos) & (WALL_L - 1)) - SECTOR(1);
            z = (item->pos.xPos ^ item->pos.zPos) & (WALL_L - 1) ^ (item->pos.zPos + SECTOR(1));
            break;
        default:
        case EAST:
            x = ((item->pos.xPos ^ item->pos.zPos) & (WALL_L - 1)) ^ (item->pos.xPos + SECTOR(1));
            z = (item->pos.zPos ^ ((item->pos.xPos ^ item->pos.zPos) & (WALL_L - 1))) - SECTOR(1);
            break;
    }

    item->pos.xPos = x;
    Lara.cornerX = x;
    item->pos.zPos = z;
    Lara.cornerZ = z;
    item->pos.yRot -= ANGLE(90);

    result = IsValidHangPos(item, coll);
    if (!result)
    {
        item->pos.xPos = old_x;
        item->pos.zPos = old_z;
        item->pos.yRot = old_angle;
        Lara.moveAngle = old_angle;
        return 0;
    }

    item->pos.xPos = old_x;
    item->pos.zPos = old_z;
    item->pos.yRot = old_angle;
    Lara.moveAngle = old_angle;

    if (!Lara.climbStatus)
    {
        if (abs(old_front - coll->front.floor) <= SLOPE_DIF)
        {
            switch (angle)
            {
                case NORTH:
                    if ((old_x & (WALL_L - 1)) < 512)
                        return 0;
                    break;
                case EAST:
                    if ((old_z & (WALL_L - 1)) > 512)
                        return 0;
                    break;
                case SOUTH:
                    if ((old_x & (WALL_L - 1)) > 512)
                        return 0;
                    break;
                case WEST:
                    if ((old_z & (WALL_L - 1)) < 512)
                        return 0;
                    break;
                default:
                    return result;
            }

            return result;
        }
        
        return 0;
    }

    if (GetClimbTrigger(x, item->pos.yPos, z, item->roomNumber) & RightClimbTab[angle])
        return result;

    short front = LaraFloorFront(item, item->pos.yRot, LARA_RAD + 16);
    if (abs(front - coll->front.floor) > SLOPE_DIF)
        return 0;

    if (front < -768)
        return 0;

    return result;
}

int LaraHangLeftCornerTest(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->animNumber != ANIMATION_LARA_HANG_IDLE)
        return 0;
    if (coll->hitStatic)
        return 0;

    int x, z;
    int old_x = item->pos.xPos;
    int old_z = item->pos.zPos;
    int old_front = coll->front.floor;
    SHORT old_angle = item->pos.yRot;
    SHORT angle = (USHORT)(item->pos.yRot + ANGLE(45)) / ANGLE(90);

    if (angle && angle != 2)
    {
        x = item->pos.xPos ^ (item->pos.xPos ^ item->pos.zPos) & (WALL_L - 1);
        z = item->pos.zPos ^ (item->pos.xPos ^ item->pos.zPos) & (WALL_L - 1);
    }
    else
    {
        x = (item->pos.xPos & ~(WALL_L - 1)) - (item->pos.zPos & (WALL_L - 1)) + SECTOR(1);
        z = (item->pos.zPos & ~(WALL_L - 1)) - (item->pos.xPos & (WALL_L - 1)) + SECTOR(1);
    }

    item->pos.xPos = x;
    Lara.cornerX = x;
    item->pos.zPos = z;
    Lara.cornerZ = z;
    item->pos.yRot -= ANGLE(90);

    int result = -IsValidHangPos(item, coll);
    if (result)
    {
        if (Lara.climbStatus)
        {
            if (GetClimbTrigger(x, item->pos.yPos, z, item->roomNumber) & RightClimbTab[angle])
            {
                item->pos.xPos = old_x;
                item->pos.zPos = old_z;
                item->pos.yRot = old_angle;
                Lara.moveAngle = old_angle;
                return result;
            }
        }
        else
        {
            if (abs(old_front - coll->front.floor) <= SLOPE_DIF)
            {
                item->pos.xPos = old_x;
                item->pos.zPos = old_z;
                item->pos.yRot = old_angle;
                Lara.moveAngle = old_angle;
                return result;
            }
        }
    }

    item->pos.xPos = old_x;
    item->pos.zPos = old_z;
    item->pos.yRot = old_angle;
    Lara.moveAngle = old_angle;
    if (LaraFloorFront(item, old_angle - 0x4000, LARA_RAD + 16) < 0)
        return 0;

    switch (angle)
    {
        case NORTH:
            x = (item->pos.xPos ^ ((item->pos.xPos ^ item->pos.zPos) & (WALL_L - 1))) - SECTOR(1);
            z = ((item->pos.xPos ^ item->pos.zPos) & (WALL_L - 1)) ^ (item->pos.zPos + SECTOR(1));
            break;
        case SOUTH:
            x = ((item->pos.xPos ^ item->pos.zPos) & (WALL_L - 1)) ^ (item->pos.xPos + SECTOR(1));
            z = ((item->pos.xPos ^ item->pos.zPos) & (WALL_L - 1)) ^ (item->pos.zPos - SECTOR(1));
            break;
        case WEST:
            x = (item->pos.xPos & ~(WALL_L - 1)) - (item->pos.zPos & (WALL_L - 1));
            z = (item->pos.zPos & ~(WALL_L - 1)) - (item->pos.xPos & (WALL_L - 1));
            break;
        default:
        case EAST:
            x = ((item->pos.xPos + SECTOR(1)) & ~(WALL_L - 1)) - (item->pos.zPos & (WALL_L - 1)) + SECTOR(1);
            z = ((item->pos.zPos + SECTOR(1)) & ~(WALL_L - 1)) - (item->pos.xPos & (WALL_L - 1)) + SECTOR(1);
            break;
    }

    item->pos.xPos = x;
    Lara.cornerX = x;
    item->pos.zPos = z;
    Lara.cornerZ = z;
    item->pos.yRot += ANGLE(90);
    result = IsValidHangPos(item, coll);
    if (!result)
    {
        item->pos.xPos = old_x;
        item->pos.zPos = old_z;
        item->pos.yRot = old_angle;
        Lara.moveAngle = old_angle;
        return 0;
    }

    item->pos.xPos = old_x;
    item->pos.zPos = old_z;
    item->pos.yRot = old_angle;
    Lara.moveAngle = old_angle;
    if (!Lara.climbStatus)
    {
        if (abs(old_front - coll->front.floor) <= SLOPE_DIF)
        {
            switch (angle)
            {
                case NORTH:
                    if ((old_x & (WALL_L - 1)) > 512)
                        return 0;
                    break;
                case EAST:
                    if ((old_z & (WALL_L - 1)) < 512)
                        return 0;
                    break;
                case SOUTH:
                    if ((old_x & (WALL_L - 1)) < 512)
                        return 0;
                    break;
                case WEST:
                    if ((old_z & (WALL_L - 1)) > 512)
                        return 0;
                    break;
            }

            return result;
        }

        return 0;
    }

    if (GetClimbTrigger(x, item->pos.yPos, z, item->roomNumber) & LeftClimbTab[angle])
        return result;

    short front = LaraFloorFront(item, item->pos.yRot, LARA_RAD + 16);
    if (abs(coll->front.floor - front) > 0)
        return 0;

    if (front < -768)
        return 0;

    return result;
}

int LaraClimbLeftCornerTest(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->animNumber != ANIMATION_LARA_LADDER_LEFT)
        return 0;

    int x, z;
    int old_x, old_z;
    int result = 0;
    int shift;
    SHORT old_angle;
    SHORT angle;

    old_x = item->pos.xPos;
    old_z = item->pos.zPos;
    old_angle = item->pos.yRot;
    angle = (USHORT)(old_angle + ANGLE(45)) / ANGLE(90);
    
    if (angle && angle != 2)
    {
        x = item->pos.xPos ^ (item->pos.xPos ^ item->pos.zPos) & (WALL_L - 1);
        z = item->pos.zPos ^ (item->pos.xPos ^ item->pos.zPos) & (WALL_L - 1);
    }
    else
    {
        x = (item->pos.xPos & ~(WALL_L - 1)) - (item->pos.zPos & (WALL_L - 1)) + SECTOR(1);
        z = (item->pos.zPos & ~(WALL_L - 1)) - (item->pos.xPos & (WALL_L - 1)) + SECTOR(1);
    }

    if (GetClimbTrigger(x, item->pos.yPos, z, item->roomNumber) & LeftIntRightExtTab[angle])
    {
        item->pos.xPos = x;
        Lara.cornerX = x;
        item->pos.zPos = z;
        Lara.cornerZ = z;
        item->pos.yRot -= 0x4000;
        Lara.moveAngle = item->pos.yRot;
        result = LaraTestClimbPos(item, coll->radius, -(coll->radius + CLIMB_WIDTHL), -CLIMB_HITE, CLIMB_HITE, &shift);
        item->itemFlags[3] = result;
    }
    
    if (!result)
    {
        item->pos.xPos = old_x;
        item->pos.zPos = old_z;
        item->pos.yRot = old_angle;
        Lara.moveAngle = old_angle;

        switch (angle)
        {
            case NORTH:
                x = (item->pos.xPos ^ ((item->pos.zPos ^ item->pos.xPos) & (WALL_L - 1))) - SECTOR(1);
                z = ((item->pos.zPos ^ item->pos.xPos) & (WALL_L - 1)) ^ (item->pos.zPos + SECTOR(1));
                break;

            case SOUTH:
                x = ((item->pos.zPos ^ item->pos.xPos) & (WALL_L - 1)) ^ (item->pos.xPos + SECTOR(1));
                z = ((item->pos.zPos ^ item->pos.xPos) & (WALL_L - 1)) ^ (item->pos.zPos - SECTOR(1));
                break;

            case EAST:
                x = ((item->pos.xPos + SECTOR(1)) & ~(WALL_L - 1)) - (item->pos.zPos & (WALL_L - 1)) + SECTOR(1);
                z = ((item->pos.zPos + SECTOR(1)) & ~(WALL_L - 1)) - (item->pos.xPos & (WALL_L - 1)) + SECTOR(1);
                break;

            case WEST:
            default:
                x = (item->pos.xPos & ~(WALL_L - 1)) - (item->pos.zPos & (WALL_L - 1));
                z = (item->pos.zPos & ~(WALL_L - 1)) - (item->pos.xPos & (WALL_L - 1));
                break;
        }

        if (GetClimbTrigger(x, item->pos.yPos, z, item->roomNumber) & LeftExtRightIntTab[angle])
        {
            item->pos.xPos = x;
            Lara.cornerX = x;
            item->pos.zPos = z;
            Lara.cornerZ = z;
            item->pos.yRot += 0x4000;
            Lara.moveAngle = item->pos.yRot;
            item->itemFlags[3] = LaraTestClimbPos(item, coll->radius, -(coll->radius + CLIMB_WIDTHL), -CLIMB_HITE, CLIMB_HITE, &shift);
            result = item->itemFlags[3] != 0;
        }
    }
    else
    {
        result = -1;
    }

    item->pos.xPos = old_x;
    item->pos.zPos = old_z;
    item->pos.yRot = old_angle;
    Lara.moveAngle = old_angle;
    return result;
}

int LaraClimbRightCornerTest(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->animNumber != ANIMATION_LARA_LADDER_RIGHT)
        return 0;

    int x, z;
    int old_x, old_z;
    int result = 0;
    int shift;
    SHORT old_angle;
    SHORT angle;

    old_x = item->pos.xPos;
    old_z = item->pos.zPos;
    old_angle = item->pos.yRot;
    angle = (USHORT)(old_angle + ANGLE(45)) / ANGLE(90);

    if (angle && angle != 2)
    {
        x = (item->pos.xPos & ~(WALL_L - 1)) - (item->pos.zPos & (WALL_L - 1)) + SECTOR(1);
        z = (item->pos.zPos & ~(WALL_L - 1)) - (item->pos.xPos & (WALL_L - 1)) + SECTOR(1);
    }
    else
    {
        x = item->pos.xPos ^ (item->pos.xPos ^ item->pos.zPos) & (WALL_L - 1);
        z = item->pos.zPos ^ (item->pos.xPos ^ item->pos.zPos) & (WALL_L - 1);
    }

    if (GetClimbTrigger(x, item->pos.yPos, z, item->roomNumber) & LeftExtRightIntTab[angle])
    {
        item->pos.xPos = x;
        Lara.cornerX = x;
        item->pos.zPos = z;
        Lara.cornerZ = z;
        item->pos.yRot += 0x4000;
        Lara.moveAngle = item->pos.yRot;
        result = LaraTestClimbPos(item, coll->radius, coll->radius + CLIMB_WIDTHR, -CLIMB_HITE, CLIMB_HITE, &shift);
    }

    if (!result)
    {
        item->pos.xPos = old_x;
        item->pos.zPos = old_z;
        item->pos.yRot = old_angle;
        Lara.moveAngle = old_angle;

        switch (angle)
        {
            case NORTH:
                x = ((item->pos.xPos + SECTOR(1)) & ~(WALL_L - 1)) - (item->pos.zPos % SECTOR(1)) + SECTOR(1);
                z = ((item->pos.zPos + SECTOR(1)) & ~(WALL_L - 1)) - (item->pos.xPos % SECTOR(1)) + SECTOR(1);
                break;

            case SOUTH:
                x = ((item->pos.xPos - SECTOR(1)) & ~(WALL_L - 1)) - (item->pos.zPos % SECTOR(1)) + SECTOR(1);
                z = ((item->pos.zPos - SECTOR(1)) & ~(WALL_L - 1)) - (item->pos.xPos % SECTOR(1)) + SECTOR(1);
                break;

            case EAST:
                x = ((item->pos.zPos ^ item->pos.xPos) % SECTOR(1)) ^ (item->pos.xPos + SECTOR(1));
                z = (item->pos.zPos ^ ((item->pos.zPos ^ item->pos.xPos) % SECTOR(1))) - SECTOR(1);
                break;

            case WEST:
            default:
                x = (item->pos.xPos ^ (item->pos.zPos ^ item->pos.xPos) % SECTOR(1)) - SECTOR(1);
                z = ((item->pos.zPos ^ item->pos.xPos) % SECTOR(1)) ^ (item->pos.zPos + SECTOR(1));
                break;
        }

        if (GetClimbTrigger(x, item->pos.yPos, z, item->roomNumber) & LeftIntRightExtTab[angle])
        {
            item->pos.xPos = x;
            Lara.cornerX = x;
            item->pos.zPos = z;
            Lara.cornerZ = z;
            item->pos.yRot -= 0x4000;
            Lara.moveAngle = item->pos.yRot;
            result = LaraTestClimbPos(item, coll->radius, coll->radius + CLIMB_WIDTHR, -CLIMB_HITE, CLIMB_HITE, &shift);
        }
    }
    else
    {
        result = -1;
    }

    item->pos.xPos = old_x;
    item->pos.zPos = old_z;
    item->pos.yRot = old_angle;
    Lara.moveAngle = old_angle;
    return result;
}

void LaraSlideEdgeJump(ITEM_INFO* item, COLL_INFO* coll)
{
    ShiftItem(item, coll);

    switch (coll->collType)
    {
        case COLL_LEFT:
            item->pos.yRot += ANGLE(5);
            break;
        case COLL_RIGHT:
            item->pos.yRot -= ANGLE(5);
            break;
        case COLL_TOP:
        case COLL_TOPFRONT:
            if (item->fallspeed <= 0)
                item->fallspeed = 1;
            break;
        case COLL_CLAMP:
            item->pos.xPos -= LARA_RAD * SIN(coll->facing) >> W2V_SHIFT;
            item->pos.zPos -= LARA_RAD * COS(coll->facing) >> W2V_SHIFT;
            item->speed = 0;
            coll->middle.floor = 0;
            if (item->fallspeed <= 0)
                item->fallspeed = 16;
            break;
    }
}

BOOL LaraTestClimbStance(ITEM_INFO* item, COLL_INFO* coll)
{
    int result_r, result_l, shift_r, shift_l;

    result_r = LaraTestClimbPos(item, coll->radius, coll->radius + CLIMB_WIDTHR, -CLIMB_STANCE, CLIMB_HITE, &shift_r);
    if (result_r != 1)
        return FALSE;

    result_l = LaraTestClimbPos(item, coll->radius, -(coll->radius + CLIMB_WIDTHL), -CLIMB_STANCE, CLIMB_HITE, &shift_l);
    if (result_l != 1)
        return FALSE;

    if (shift_r)
    {
        if (shift_l)
        {
            if ((shift_l < 0) ^ (shift_r < 0))
                return FALSE;

            if (shift_r < 0 && shift_l < shift_r)
                shift_r = shift_l;
            else if (shift_r > 0 && shift_l > shift_r)
                shift_r = shift_l;
        }

        item->pos.yPos += shift_r;
    }
    else if (shift_l)
    {
        item->pos.yPos += shift_l;
    }

    return TRUE;
}

int LaraTestEdgeCatch(ITEM_INFO* item, COLL_INFO* coll, int* edge)
{
    short* bounds = GetBoundsAccurate(item);
    int hdif = coll->front.floor - bounds[2];

    if (hdif < 0 && (hdif + item->fallspeed) < 0
    ||  hdif > 0 && (hdif + item->fallspeed) > 0)
    {
        hdif = item->pos.yPos + bounds[2];

        if ((hdif ^ (hdif + item->fallspeed)) & ~(STEP_L - 1))
        {
            if (item->fallspeed > 0)
                *edge = (hdif + item->fallspeed) & ~(STEP_L - 1);
            else
                *edge =  hdif & ~(STEP_L - 1);
            return -1;
        }

        return 0;
    }

    if (abs(coll->backLeft.floor - coll->backRight.floor) >= SLOPE_DIF)
        return 0;

    return 1;
}

BOOL LaraTestHangOnClimbWall(ITEM_INFO* item, COLL_INFO* coll)
{
    if (!Lara.climbStatus || item->fallspeed < 0)
        return FALSE;

    SHORT angle = (USHORT)(item->pos.yRot + ANGLE(45)) / ANGLE(90);
    switch (angle)
    {
        case NORTH:
        case SOUTH:
            item->pos.zPos += coll->shift.z;
            break;
        case EAST:
        case WEST:
            item->pos.xPos += coll->shift.x;
            break;
    }

    if (Lara.moveAngle != item->pos.yRot)
    {
        int ceiling_item = LaraCeilingFront(item, item->pos.yRot, 0, 0);
        int ceiling_lara = LaraCeilingFront(item, Lara.moveAngle, (STEP_L / 2), 0);
        if (abs(ceiling_item - ceiling_lara) > SLOPE_DIF)
            return FALSE;
    }

    int shift;
    short* bounds = GetBoundsAccurate(item);
    if (!LaraTestClimbPos(item, coll->radius, coll->radius, bounds[2], (bounds[3] - bounds[2]), &shift)
    ||  !LaraTestClimbPos(item, coll->radius, -coll->radius, bounds[2], (bounds[3] - bounds[2]), &shift))
        return FALSE;

    int result = LaraTestClimbPos(item, coll->radius, 0, bounds[2], (bounds[3] - bounds[2]), &shift);
    if (!result)
        return 0;
    else if (result == TRUE) // saying climb OK, but not really valid test (so ignore y shift)
        return TRUE;

    item->pos.yPos += shift;
    return TRUE;
}

void LaraSlideSlope(ITEM_INFO* item, COLL_INFO* coll)
{
    coll->badPos = NO_HEIGHT;
    coll->badNeg = -(STEP_L * 2);
    coll->badCeiling = 0;
    GetLaraCollisionInfo(item, coll);

    if (!LaraHitCeiling(item, coll))
    {
        LaraDeflectEdge(item, coll);
        if (coll->middle.floor <= 200)
        {
            TestLaraSlide(item, coll);
            item->pos.yPos += coll->middle.floor;

            char tiltX = coll->tiltX;
            char tiltZ = coll->tiltZ;
            if ((tiltX & 128) != 0)
                tiltX = -tiltX;
            if ((tiltZ & 128) != 0)
                tiltZ = -tiltZ;

            if (tiltX <= 2 && tiltZ <= 2)
            {
                item->goalAnimState = STATE_LARA_IDLE;
                item->pos.xRot = 0;
                StopSoundEffect(SFX_LARA_SLIPPING);
            }
        }
        else
        {
            if (item->currentAnimState == STATE_LARA_SLIDE_FORWARD)
                SetAnimationForItemAS(item, ANIMATION_LARA_FREE_FALL_FORWARD, STATE_LARA_JUMP_FORWARD);
            else
                SetAnimationForItemAS(item, ANIMATION_LARA_FREE_FALL_BACK, STATE_LARA_FALL_BACKWARD);
            item->fallspeed = 0;
            item->gravityStatus = TRUE;
            StopSoundEffect(SFX_LARA_SLIPPING);
        }
    }
}

void LaraCollideJump(ITEM_INFO* item, COLL_INFO* coll)
{
    coll->badPos = NO_HEIGHT;
    coll->badNeg = -STEPUP_HEIGHT;
    coll->badCeiling = BAD_JUMP_CEILING;
    GetLaraCollisionInfo(item, coll);
    LaraDeflectEdgeJump(item, coll);
    if (item->fallspeed > 0 && coll->middle.floor <= 0)
    {
        if (LaraLandedBad(item))
            item->goalAnimState = STATE_LARA_DEATH;
        else
            item->goalAnimState = STATE_LARA_IDLE;
        item->fallspeed = 0;
        item->gravityStatus = FALSE;
        if (coll->middle.floor != -NO_HEIGHT)
            item->pos.yPos += coll->middle.floor;
    }
}

BOOL LaraCheckForLetGo(ITEM_INFO* item, COLL_INFO* coll)
{
    FLOOR_INFO* floor;
    short roomNumber;

    item->fallspeed = 0;
    item->gravityStatus = FALSE;

    roomNumber = item->roomNumber;
    floor = GetFloor(item->pos.xPos, item->pos.yPos, item->pos.zPos, &roomNumber);
    GetFloorHeight(floor, item->pos.xPos, item->pos.yPos, item->pos.zPos);
    coll->trigger = TriggerIndex;

    if (CHK_NOP(TrInput, IN_ACTION) || item->hitPoints <= 0)
    {
        Lara.torsoYrot = 0;
        Lara.torsoXrot = 0;
        Lara.headYrot = 0;
        Lara.headXrot = 0;
        Lara.gunStatus = LHS_ARMLESS;
        item->speed = 2;
        item->fallspeed = 1;
        item->gravityStatus = TRUE;
        SetAnimationForItemAS(item, ANIMATION_LARA_FREE_FALL_FORWARD, STATE_LARA_JUMP_FORWARD);
        return TRUE;
    }

    return FALSE;
}

int LaraTestClimbUpPos(ITEM_INFO* item, int front, int right, int* shift, int* ledge)
{
    FLOOR_INFO *floor;
    SHORT angle;
    short roomNumber;
    int x, y, z;
    int height, ceiling;
    int xfront = 0, zfront = 0;

    y = (item->pos.yPos - CLIMB_HITE) - STEP_L;
    angle = (USHORT)(item->pos.yRot + ANGLE(45)) / ANGLE(90);
    switch (angle)
    {
        case NORTH:
            z = item->pos.zPos + front;
            x = item->pos.xPos + right;
            zfront = 4;
            break;
        case EAST:
            x = item->pos.xPos + front;
            z = item->pos.zPos - right;
            xfront = 4;
            break;
        case SOUTH:
            z = item->pos.zPos - front;
            x = item->pos.xPos - right;
            zfront = -4;
            break;
        default:
        case WEST:
            x = item->pos.xPos - front;
            z = item->pos.zPos + right;
            xfront = -4;
            break;
    }

    *shift = 0;
    roomNumber = item->roomNumber;
    floor = GetFloor(x, y, z, &roomNumber);
    ceiling = GetCeiling(floor, x, y, z) - (y - STEP_L);

    if (ceiling > CLIMB_SHIFT)
        return 0;
    else if (ceiling > 0)
        *shift = ceiling;

    floor = GetFloor(x + xfront, y, z + zfront, &roomNumber);
    height = GetFloorHeight(floor, x + xfront, y, z + zfront);
    if (height == NO_HEIGHT)
    {
        *ledge = NO_HEIGHT;
        return 1;
    }

    height -= y;
    *ledge = height;
    if (height > (STEP_L / 2))
    {
        ceiling = GetCeiling(floor, x + xfront, y, z + zfront) - y;
        if (ceiling >= CLIMB_HITE)
        {
            return 1;
        }
        else if ((height - ceiling) > LARA_HITE)
        {
            *shift = height;
            return -1;
        }
        else
        {
            return 0;
        }
    }
    else if (height > 0)
    {
        if (height > *shift)
            *shift = height;
    }

    roomNumber = item->roomNumber;
    floor = GetFloor(x, y + CLIMB_HITE, z, &roomNumber); // establish correct room
    floor = GetFloor(x + xfront, y + CLIMB_HITE, z + zfront, &roomNumber);
    ceiling = GetCeiling(floor, x + xfront, y + CLIMB_HITE, z + zfront) - y;
    if (ceiling <= height)
        return 1;

    if (ceiling >= CLIMB_HITE)
        return 1;
    else
        return 0;
}

int LaraTestClimbPos(ITEM_INFO* item, int front, int right, int origin, int height, int * shift)
{
    SHORT angle;
    int x, z;
    int xfront = 0, zfront = 0;

    angle = (USHORT)(item->pos.yRot + ANGLE(45)) / ANGLE(90);
    switch (angle)
    {
        case NORTH:
            z = item->pos.zPos + front;
            x = item->pos.xPos + right;
            zfront = 4;
            break;
        case EAST:
            x = item->pos.xPos + front;
            z = item->pos.zPos - right;
            xfront = 4;
            break;
        case SOUTH:
            z = item->pos.zPos - front;
            x = item->pos.xPos - right;
            zfront = -4;
            break;
        default:
        case WEST:
            x = item->pos.xPos - front;
            z = item->pos.zPos + right;
            xfront = -4;
            break;
    }

    return LaraTestClimb(x, item->pos.yPos + origin, z, xfront, zfront, height, item->roomNumber, shift);
}

int LaraTestClimb(int x, int y, int z, int xfront, int zfront, int item_height, short item_room, int *shift)
{
    FLOOR_INFO *floor;
    BOOL hang = TRUE;
    int height;
    int ceiling, ceiling2;
    int old_xfront, old_zfront;
    short roomNumber;

    *shift = 0;
    if (!Lara.climbStatus)
        return 0;
    roomNumber = item_room;
    floor = GetFloor(x, y - (STEP_L / 2), z, &roomNumber);
    height = GetFloorHeight(floor, x, y, z);
    if (height == -NO_HEIGHT)
        return 0;

    height -= (y + item_height + (STEP_L / 2));
    if (height < -CLIMB_SHIFT)
        return 0;
    else if (height < 0)
        *shift = height;

    ceiling = GetCeiling(floor, x, y, z) - y;
    if (ceiling > CLIMB_SHIFT)
    {
        return 0;
    }
    else if (ceiling > 0)
    {
        if (*shift)
            return 0;
        *shift = ceiling;
    }

    if ((item_height + height) < CLIMB_HANG)
        hang = FALSE;

    old_zfront = zfront + z;
    old_xfront = xfront + x;
    floor = GetFloor(xfront + x, y, zfront + z, &roomNumber);
    height = GetFloorHeight(floor, xfront + x, y, zfront + z);
    if (height != -NO_HEIGHT)
        height -= y;

    if (height <= CLIMB_SHIFT)
    {
        if (height > 0)
        {
            if (*shift < 0)
                return 0;
            if (height > *shift)
                *shift = height;
        }

        roomNumber = item_room;
        GetFloor(x, y + item_height, z, &roomNumber);
        floor = GetFloor(old_xfront, y + item_height, old_zfront, &roomNumber);
        ceiling = GetCeiling(floor, old_xfront, y + item_height, old_zfront);
        if (ceiling == -NO_HEIGHT)
            return 1;

        ceiling2 = ceiling - y;
        if (ceiling2 <= height)
            return 1;
        else if (ceiling2 >= CLIMB_HITE)
            return 1;
        else if (ceiling2 <= (CLIMB_HITE - CLIMB_SHIFT))
            return -(hang != 0);
        
        if (*shift > 0)
            return -(hang != 0);
        *shift = (ceiling2 - CLIMB_HITE);
        return 1;
    }
    else
    {
        ceiling2 = GetCeiling(floor, old_xfront, y, old_zfront) - y;
        if (ceiling2 >= CLIMB_HITE)
        {
            return 1;
        }
        else if (ceiling2 > (CLIMB_HITE - CLIMB_SHIFT))
        {
            if (*shift > 0)
                return -(hang != 0);
            *shift = (ceiling2 - CLIMB_HITE);
            return 1;
        }
        else if (ceiling2 > 0)
        {
            return -(hang != 0);
        }
        else if (ceiling2 <= -CLIMB_SHIFT || !hang || *shift > 0)
        {
            return 0;
        }

        if (*shift > ceiling2)
            *shift = ceiling2;
    }
    
    return -1;
}

void LaraDoClimbLeftRight(ITEM_INFO* item, COLL_INFO* coll, int result, int shift)
{
    if (result == TRUE)
    {
        if (CHK_EXI(TrInput, IN_LEFT))
            item->goalAnimState = STATE_LARA_LADDER_LEFT;
        else if (CHK_EXI(TrInput, IN_RIGHT))
            item->goalAnimState = STATE_LARA_LADDER_RIGHT;
        else
            item->goalAnimState = STATE_LARA_LADDER_IDLE;

        item->pos.yPos += shift;
        return;
    }

    if (result != 0)
    {
        item->goalAnimState = STATE_LARA_HANG;
        do
        {
            AnimateItem(item);
        } while (item->currentAnimState != STATE_LARA_HANG);
        item->pos.xPos = coll->old.x;
        item->pos.zPos = coll->old.z;
        return;
    }

    item->pos.xPos = coll->old.x;
    item->pos.zPos = coll->old.z;
    item->goalAnimState = STATE_LARA_LADDER_IDLE;
    item->currentAnimState = STATE_LARA_LADDER_IDLE;

    if (coll->oldAnimState != STATE_LARA_LADDER_IDLE)
    {
        item->animNumber = ANIMATION_LARA_LADDER_IDLE;
        item->frameNumber = Anims[item->animNumber].frameBase;
        return;
    }

    if (CHK_EXI(TrInput, IN_LEFT))
    {
        int test_left = LaraClimbLeftCornerTest(item, coll);
        if (test_left != 0)
        {
            if (test_left <= 0)
                SetAnimationForItemAS(item, ANIMATION_LARA_LADDER_AROUND_LEFT_INNER_BEGIN, STATE_LARA_CLIMB_CORNER_LEFT_INNER);
            else
                SetAnimationForItemAS(item, ANIMATION_LARA_LADDER_AROUND_LEFT_OUTER_BEGIN, STATE_LARA_CLIMB_CORNER_LEFT_OUTER);
            return;
        }
    }
    else if (CHK_EXI(TrInput, IN_RIGHT))
    {
        int test_right = LaraClimbRightCornerTest(item, coll);
        if (test_right != 0)
        {
            if (test_right <= 0)
                SetAnimationForItemAS(item, ANIMATION_LARA_LADDER_AROUND_RIGHT_INNER_BEGIN, STATE_LARA_CLIMB_CORNER_RIGHT_INNER);
            else
                SetAnimationForItemAS(item, ANIMATION_LARA_LADDER_AROUND_RIGHT_OUTER_BEGIN, STATE_LARA_CLIMB_CORNER_RIGHT_OUTER);
            return;
        }
    }

    item->frameNumber = coll->oldFrameNumber;
    item->animNumber = coll->oldAnimNumber;
    AnimateLara(item);
}

BOOL LaraTestWaterClimbOut(ITEM_INFO* item, COLL_INFO* coll)
{
    int hdif;
    short angle;

    if (coll->collType != COLL_FRONT || CHK_NOP(TrInput, IN_ACTION) || abs(coll->backLeft.floor - coll->backRight.floor) >= SLOPE_DIF)
        return FALSE;

    if (Lara.gunStatus != LHS_ARMLESS && !(Lara.gunStatus == LHS_READY && Lara.gunType == LG_FLARE))
        return FALSE;

    if (coll->front.ceiling > 0 || coll->middle.ceiling > -STEPUP_HEIGHT)
        return FALSE;

    hdif = coll->front.floor + SURF_HITE;
    if (hdif <= -(STEP_L * 2) || hdif > SURF_HITE - STEPUP_HEIGHT)
        return FALSE;

    angle = GetCatchAngle(item, HANG_ANGLE);
    if (angle & 0x3FFF)
        return FALSE;

    item->pos.yPos += (hdif - 5);
    UpdateLaraRoom(item, -(LARA_HITE / 2));

    switch (angle)
    {
        case 0:
            item->pos.zPos = (item->pos.zPos & ~(WALL_L - 1)) + WALL_L + LARA_RAD;
            break;
        case 0x4000:
            item->pos.xPos = (item->pos.xPos & ~(WALL_L - 1)) + WALL_L + LARA_RAD;
            break;
        case -0x8000:
            item->pos.zPos = (item->pos.zPos & ~(WALL_L - 1)) - LARA_RAD;
            break;
        case -0x4000:
            item->pos.xPos = (item->pos.xPos & ~(WALL_L - 1)) - LARA_RAD;
            break;
    }

    if (hdif < -128)
    {
        item->animNumber = ANIMATION_LARA_CLIMB_OUT_OF_WATER;
        item->frameNumber = Anims[item->animNumber].frameBase;
    }
    else if (hdif < 128)
    {
        item->animNumber = ANIMATION_LARA_ONWATER_TO_LAND_LOW;
        item->frameNumber = Anims[item->animNumber].frameBase;
    }
    else
    {
        item->animNumber = ANIMATION_LARA_ONWATER_TO_WADE;
        item->frameNumber = Anims[item->animNumber].frameBase;
    }

    Lara.gunStatus = LHS_HANDBUSY;
    Lara.waterStatus = LWS_ABOVEWATER;
    item->currentAnimState = STATE_LARA_ONWATER_EXIT;
    item->goalAnimState = STATE_LARA_IDLE;
    item->pos.yRot = angle;
    item->pos.xRot = 0;
    item->pos.zRot = 0;
    item->speed = 0;
    item->fallspeed = 0;
    item->gravityStatus = FALSE;
    return TRUE;
}

BOOL LaraTestWaterStepOut(ITEM_INFO* item, COLL_INFO* coll)
{
    if (coll->collType == COLL_FRONT || coll->middle.type == BIG_SLOPE || coll->middle.type == DIAGONAL || coll->middle.floor >= 0)
        return FALSE;

    if (coll->middle.floor < -(STEP_L / 2))
    {
        SetAnimationForItemASS(item, ANIMATION_LARA_ONWATER_TO_WADE_DEEP, STATE_LARA_ONWATER_EXIT, STATE_LARA_IDLE);
    }
    else
    {
        if (item->goalAnimState == STATE_LARA_ONWATER_LEFT)
            item->goalAnimState = STATE_LARA_WALK_LEFT;
        else if (item->goalAnimState == STATE_LARA_ONWATER_RIGHT)
            item->goalAnimState = STATE_LARA_WALK_RIGHT;
        else
            SetAnimationForItemAS(item, ANIMATION_LARA_WADE, STATE_LARA_WADE_FORWARD);
    }

    Lara.waterStatus = LWS_WADE;
    item->pos.yPos += coll->front.floor + (SURF_HITE - 5);
    item->pos.xRot = 0;
    item->pos.zRot = 0;
    item->speed = 0;
    item->fallspeed = 0;
    item->gravityStatus = FALSE;
    UpdateLaraRoom(item, -(LARA_HITE / 2));
    return TRUE;
}

void LaraTestWaterDepth(ITEM_INFO* item, COLL_INFO* coll)
{
    FLOOR_INFO* floor;
    int water_depth;
    short roomNumber;

    roomNumber = item->roomNumber;
    floor = GetFloor(item->pos.xPos, item->pos.yPos, item->pos.zPos, &roomNumber);
    water_depth = GetWaterDepth(item->pos.xPos, item->pos.yPos, item->pos.zPos, roomNumber);

    if (water_depth == -NO_HEIGHT)
    {
        item->pos.xPos = coll->old.x;
        item->pos.yPos = coll->old.y;
        item->pos.zPos = coll->old.z;
        item->fallspeed = 0;
    }
    else if (water_depth <= (STEP_L * 2))
    {
        Lara.waterStatus = LWS_WADE;
        item->pos.yPos = GetFloorHeight(floor, item->pos.xPos, item->pos.yPos, item->pos.zPos);
        item->pos.xRot = 0;
        item->pos.zRot = 0;
        item->speed = 0;
        item->fallspeed = 0;
        item->gravityStatus = FALSE;
        SetAnimationForItemASS(item, ANIMATION_LARA_UNDERWATER_TO_WADE, STATE_LARA_ONWATER_EXIT, STATE_LARA_IDLE);
    }
}

bool LaraLandedBad(ITEM_INFO* item)
{
    int landspeed = item->fallspeed - DAMAGE_START;

    if (landspeed <= 0)
        return false;

    if (landspeed <= DAMAGE_LENGTH)
        item->hitPoints -= (landspeed * landspeed * LARA_HITPOINTS) / (DAMAGE_LENGTH * DAMAGE_LENGTH);
    else
        item->hitPoints = -1;

    return item->hitPoints <= 0;
}

void LaraCollideStop(ITEM_INFO* item, COLL_INFO* coll)
{
    switch (coll->oldAnimState)
    {
        case STATE_LARA_IDLE:
        case STATE_LARA_TURN_RIGHT_SLOW:
        case STATE_LARA_TURN_LEFT_SLOW:
        case STATE_LARA_TURN_FAST:
            item->currentAnimState = coll->oldAnimState;
            item->animNumber = coll->oldAnimNumber;
            item->frameNumber = coll->oldFrameNumber;

            if (CHK_EXI(TrInput, IN_LEFT))
                item->goalAnimState = STATE_LARA_TURN_LEFT_SLOW;
            else if (CHK_EXI(TrInput, IN_RIGHT))
                item->goalAnimState = STATE_LARA_TURN_RIGHT_SLOW;
            else
                item->goalAnimState = STATE_LARA_IDLE;

            AnimateLara(item);
            break;
        default:
            item->animNumber = ANIMATION_LARA_STAY_SOLID;
            item->frameNumber = Anims[item->animNumber].frameBase;
            break;
    }
}

void LaraSwimCollision(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->pos.xRot >= -0x4000 && item->pos.xRot <= 0x4000)
        coll->facing = Lara.moveAngle = item->pos.yRot;
    else
        coll->facing = Lara.moveAngle = item->pos.yRot - 0x8000;

    int height = abs(LARA_HITE * SIN(item->pos.xRot) >> W2V_SHIFT);
    if (height < 200)
        height = 200;

    coll->badNeg = -height; // fixed ! the old value is -64 (same as TR3)
    GetCollisionInfo(coll, item->pos.xPos, item->pos.yPos + height / 2, item->pos.zPos, item->roomNumber, height);
    ShiftItem(item, coll);

    switch (coll->collType)
    {
        case COLL_FRONT:
            if (item->pos.xRot > ANGLE(45))
                item->pos.xRot += UW_WALLDEFLECT;
            else if (item->pos.xRot < -ANGLE(45))
                item->pos.xRot -= UW_WALLDEFLECT;
            else
                item->fallspeed = 0;
            break;
        case COLL_TOP:
            if (item->pos.xRot >= -ANGLE(45))
                item->pos.xRot -= UW_WALLDEFLECT;
            break;
        case COLL_TOPFRONT:
            item->fallspeed = 0;
            break;
        case COLL_LEFT:
            item->pos.yRot += ANGLE(5);
            break;
        case COLL_RIGHT:
            item->pos.yRot -= ANGLE(5);
            break;
        case COLL_CLAMP:
            item->pos.xPos = coll->old.x;
            item->pos.yPos = coll->old.y;
            item->pos.zPos = coll->old.z;
            item->fallspeed = 0;
            return;
    }

    if (coll->middle.floor < 0 && coll->middle.floor != -NO_HEIGHT)
    {
        item->pos.xRot += UW_WALLDEFLECT;
        item->pos.yPos     += coll->middle.floor;
    }

    if (Lara.waterStatus != LWS_CHEAT)
        LaraTestWaterDepth(item, coll);
}

void LaraSurfaceCollision(ITEM_INFO* item, COLL_INFO* coll)
{
    coll->facing = Lara.moveAngle;
    GetCollisionInfo(coll, item->pos.xPos, item->pos.yPos + SURF_HITE, item->pos.zPos, item->roomNumber, (SURF_HITE + LARA_RAD));
    ShiftItem(item, coll);

    if ((coll->collType & (COLL_FRONT | COLL_TOP | COLL_CLAMP | COLL_TOPFRONT)) || coll->middle.floor < 0 && (coll->middle.type == BIG_SLOPE || coll->middle.type == DIAGONAL))
    {
        item->pos.xPos = coll->old.x;
        item->pos.yPos = coll->old.y;
        item->pos.zPos = coll->old.z;
        item->fallspeed = 0;
    }
    else if (coll->collType == COLL_LEFT)
    {
        item->pos.yRot += ANGLE(5);
    }
    else if (coll->collType == COLL_RIGHT)
    {
        item->pos.yRot -= ANGLE(5);
    }

    int water_level = GetWaterHeight(item->pos.xPos, item->pos.yPos, item->pos.zPos, item->roomNumber) - item->pos.yPos;
    if (water_level <= -100)
    {
        SetAnimationForItemASS(item, ANIMATION_LARA_FREE_FALL_TO_UNDERWATER_ALTERNATE, STATE_LARA_UNDERWATER_DIVING, STATE_LARA_UNDERWATER_FORWARD);
        item->pos.xRot = -ANGLE(45);
        item->fallspeed = 80;
        Lara.waterStatus = LWS_UNDERWATER;
        return;
    }

    LaraTestWaterStepOut(item, coll);
}

// TODO: need decompiling !!!
//BOOL UseSpecialItem(ITEM_INFO* item)
//{
//    return 0;
//}

BOOL TestLaraSlide(ITEM_INFO* item, COLL_INFO* coll)
{
    static short old_angle = 1;
    short adif, angle;
    char a, b;

    a = ABS(coll->tiltX);
    b = ABS(coll->tiltZ);
    if (a <= 2 && b <= 2)
        return FALSE;

    angle = 0;
    if (coll->tiltX > 2)
        angle = -0x4000;
    else if (coll->tiltX < -2)
        angle = 0x4000;

    if (coll->tiltZ > 2 && coll->tiltZ > a)
        angle = -0x8000;
    else if (coll->tiltZ < -2 && -coll->tiltZ < a)
        angle = 0;

    adif = angle - item->pos.yRot;
    ShiftItem(item, coll);

    if (adif >= -0x4000 && adif <= 0x4000)
    {
        if (item->currentAnimState != STATE_LARA_SLIDE_FORWARD || old_angle != angle)
        {
            SetAnimationForItemAS(item, ANIMATION_LARA_SLIDE_FORWARD, STATE_LARA_SLIDE_FORWARD);
            item->pos.yRot = angle;
            Lara.moveAngle = angle;
            old_angle = angle;
        }
    }
    else
    {
        if (item->currentAnimState != STATE_LARA_SLIDE_BACK || old_angle != angle)
        {
            SetAnimationForItemAS(item, ANIMATION_LARA_SLIDE_BACKWARD, STATE_LARA_SLIDE_BACK);
            item->pos.yRot = angle - 0x8000;
            Lara.moveAngle = angle;
            old_angle = angle;
        }
    }

    // TODO: add LaraSlideAngle() here !
    //LaraSlideAngle(item, coll, adif, angle);
    return TRUE;
}

BOOL TestLaraVault(ITEM_INFO* item, COLL_INFO* coll)
{
    if (CHK_NOP(TrInput, IN_ACTION) || Lara.gunStatus != LHS_ARMLESS)
        return FALSE;

    if (coll->collType == COLL_FRONT)
    {
        int hdif, slope;
        short angle = GetCatchAngle(item, VAULT_ANGLE);
        if (angle & 0x3FFF)
            return FALSE;

        hdif = coll->front.floor;
        slope = abs(coll->backLeft.floor - coll->backRight.floor) >= SLOPE_DIF;

        if (VAULT_CLICK(hdif, 2, >=, -) && VAULT_CLICK(hdif, 2, <= , +))
        {
            if (slope
            || (coll->front.floor - coll->front.ceiling) < 0
            || (coll->backLeft.floor - coll->backLeft.ceiling) < 0
            || (coll->backRight.floor - coll->backRight.ceiling) < 0)
            {
                return FALSE;
            }

            SetAnimationForItemASS(item, ANIMATION_LARA_CLIMB_2CLICK, STATE_LARA_GRABBING, STATE_LARA_IDLE);
            item->pos.yPos += hdif + (STEP_L * 2);
            Lara.gunStatus = LHS_HANDBUSY;
        }
        else if (VAULT_CLICK(hdif, 3, >=, -) && VAULT_CLICK(hdif, 3, <=, +))
        {
            if (slope
            || (coll->front.floor - coll->front.ceiling) < 0
            || (coll->backLeft.floor - coll->backLeft.ceiling) < 0
            || (coll->backRight.floor - coll->backRight.ceiling) < 0)
            {
                return FALSE;
            }

            //if (lara.water_surface_dist < -(STEP_L * 3))
            //    return FALSE;

            SetAnimationForItemASS(item, ANIMATION_LARA_CLIMB_3CLICK, STATE_LARA_GRABBING, STATE_LARA_IDLE);
            item->pos.yPos += hdif + (STEP_L * 3);
            Lara.gunStatus = LHS_HANDBUSY;
        }
        else if (!slope && VAULT_CLICK(hdif, 7, >=, -) && VAULT_CLICK(hdif, 4, <=, +))
        {
            SetAnimationForItemASS(item, ANIMATION_LARA_STAY_SOLID, STATE_LARA_IDLE, STATE_LARA_JUMP_UP);
            Lara.calcFallspeed = -3 - phd_sqrt(-9600 - 12 * hdif);
            AnimateLara(item);
        }
        else if (Lara.climbStatus
        && VAULT_CLICK(hdif, 8, <=, +)
        && Lara.waterStatus != LWS_WADE
        && VAULT_CLICK(coll->backLeft.floor, 8, <=, +)
        && coll->backRight.floor <= -(STEP_L * 8)
        && VAULT_CLICK(coll->middle.ceiling, 8, <=, +))
        {
            SetAnimationForItemASS(item, ANIMATION_LARA_STAY_SOLID, STATE_LARA_IDLE, STATE_LARA_JUMP_UP);
            Lara.calcFallspeed = -116;
            AnimateLara(item);
        }
        else if (Lara.climbStatus
        && (hdif < -(STEP_L * 4) || coll->front.ceiling >= (LARA_HITE - STEP_L))
        && coll->middle.ceiling <= (-(STEP_L * 5) + LARA_HITE))
        {
            ShiftItem(item, coll);

            if (LaraTestClimbStance(item, coll))
            {
                SetAnimationForItemASS(item, ANIMATION_LARA_STAY_SOLID, STATE_LARA_IDLE, STATE_LARA_LADDER_IDLE);
                item->pos.yRot = angle;
                Lara.gunStatus = LHS_HANDBUSY;
                AnimateLara(item);
                return TRUE;
            }
            return FALSE;
        }

        item->pos.yRot = angle;
        ShiftItem(item, coll);

        SHORT resultAngle = (USHORT)(item->pos.yRot + ANGLE(45)) / ANGLE(90);
        switch (resultAngle)
        {
            case NORTH:
                item->pos.zPos = (item->pos.zPos |  (WALL_L - 1)) - LARA_RAD;
                break;
            case EAST:
                item->pos.xPos = (item->pos.xPos |  (WALL_L - 1)) - LARA_RAD;
                break;
            case SOUTH:
                item->pos.zPos = (item->pos.zPos & ~(WALL_L - 1)) + LARA_RAD;
                break;
            case WEST:
                item->pos.xPos = (item->pos.xPos & ~(WALL_L - 1)) + LARA_RAD;
                break;
        }

        return TRUE;
    }

    return FALSE;
}

BOOL TestMonkeyLeft(ITEM_INFO* item, COLL_INFO* coll)
{
    Lara.moveAngle = item->pos.yRot - 0x4000;
    coll->badPos = NO_HEIGHT;
    coll->badNeg = -NO_HEIGHT;
    coll->badCeiling = 0;
    coll->facing = Lara.moveAngle;
    coll->radius = LARA_RAD;
    coll->slopesAreWalls = FALSE;
    GetCollisionInfo(coll, item->pos.xPos, item->pos.yPos, item->pos.zPos, item->roomNumber, LARA_HANG_HEIGHT);

    if (abs(coll->middle.ceiling - coll->front.ceiling) > 50)
        return FALSE;

    if (coll->collType)
    {
        short octant = GetDirOctant(item->pos.yRot);
        if (((octant == 0) && (coll->collType == COLL_FRONT))
		||  ((octant == 0) && (coll->collType == COLL_LEFT))
		||  ((octant == 1) && (coll->collType == COLL_RIGHT))
		||  ((octant == 1) && (coll->collType == COLL_LEFT)))
			return FALSE;
    }

    return TRUE;
}

BOOL TestMonkeyRight(ITEM_INFO* item, COLL_INFO* coll)
{
    Lara.moveAngle = item->pos.yRot + 0x4000;
    coll->badPos = NO_HEIGHT;
    coll->badNeg = -STEPUP_HEIGHT;
    coll->badCeiling = 0;
    coll->facing = Lara.moveAngle;
    coll->radius = LARA_RAD;
    coll->slopesAreWalls = FALSE;
    GetCollisionInfo(coll, item->pos.xPos, item->pos.yPos, item->pos.zPos, item->roomNumber, LARA_HANG_HEIGHT);

    if (abs(coll->middle.ceiling - coll->front.ceiling) > 50)
        return FALSE;

    if (coll->collType)
    {
        short octant = GetDirOctant(item->pos.yRot);
        if (((octant == 0) && (coll->collType == COLL_FRONT))
		||  ((octant == 1) && (coll->collType == COLL_FRONT))
		||  ((octant == 1) && (coll->collType == COLL_RIGHT))
		||  ((octant == 1) && (coll->collType == COLL_LEFT)))
			return 0;
    }

    return TRUE;
}

BOOL TestHangSwingIn(ITEM_INFO* item, short angle)
{
    FLOOR_INFO *floor;
    int x, y, z;
    int height, ceiling;
    short roomNumber;

    x = item->pos.xPos;
    y = item->pos.yPos;
    z = item->pos.zPos;
    roomNumber = item->roomNumber;

    switch (angle)
    {
        case 0:
            z += STEP_L;
            break;
        case 0x4000:
            x += STEP_L;
            break;
        case -0x4000:
            x -= STEP_L;
            break;
        case -0x8000:
            z -= STEP_L;
            break;
    }

    floor = GetFloor(x, y, z, &roomNumber);
    height = GetFloorHeight(floor, x, y, z);
    ceiling = GetCeiling(floor, x, y, z);

    if (height != -NO_HEIGHT)
    {
        if ((height - y) > 0 && (ceiling - y) < -400 && (y - ceiling - 819) > -72)
            return TRUE;
    }

    return FALSE;
}

BOOL TestWall(ITEM_INFO* item, int front, int right, int height)
{
    FLOOR_INFO* floor;
    SHORT angle;
    int ceiling;
    int x, y, z;
    short roomNumber;

    angle = (USHORT)(item->pos.yRot + ANGLE(45)) / ANGLE(90);
    x = item->pos.xPos;
    y = height + item->pos.yPos;
    z = item->pos.zPos;
    roomNumber = item->roomNumber;

    switch (angle)
    {
        case NORTH:
            x -= right;
            break;
        case EAST:
            z -= right;
            break;
        case SOUTH:
            x += right;
            break;
        case WEST:
            z += right;
            break;
    }

    GetFloor(x, y, z, &roomNumber);

    switch (angle)
    {
        case NORTH:
            z += front;
            break;
        case EAST:
            x += front;
            break;
        case SOUTH:
            z -= front;
            break;
        case WEST:
            x -= front;
            break;
    }

    floor = GetFloor(x, y, z, &roomNumber);
    right = GetFloorHeight(floor, x, y, z);
    ceiling = GetCeiling(floor, x, y, z);
    if (right == -NO_HEIGHT)
        return 1;
    if (right - y <= 0 || ceiling - y >= 0)
        return 2;
    return 0;
}

void SwimTurn(ITEM_INFO* item)
{
    if (CHK_EXI(TrInput, IN_FORWARD))
        item->pos.xRot -= 2 * ONE_DEGREE;
    else if (CHK_EXI(TrInput, IN_BACK))
        item->pos.xRot += 2 * ONE_DEGREE;

    if (CHK_EXI(TrInput, IN_LEFT))
    {
        Lara.turnRate -= LARA_TURN_RATE;
        if (Lara.turnRate < -LARA_MED_TURN)
            Lara.turnRate = -LARA_MED_TURN;
        item->pos.zRot -= (LARA_LEAN_RATE * 2);
    }
    else if (CHK_EXI(TrInput, IN_RIGHT))
    {
        Lara.turnRate += LARA_TURN_RATE;
        if (Lara.turnRate > LARA_MED_TURN)
            Lara.turnRate = LARA_MED_TURN;
        item->pos.zRot += (LARA_LEAN_RATE * 2);
    }
}

void MonkeySwingFall(ITEM_INFO* item)
{
    SetAnimationForItemASF(item, ANIMATION_LARA_TRY_HANG_VERTICAL, STATE_LARA_JUMP_UP, 9);
    //item->pos.y += STEP_L; // deleted this line, it's not necessary since gravity_status is TRUE !
    item->speed = 2;
    item->fallspeed = 1;
    item->gravityStatus = TRUE;
    Lara.gunStatus = LHS_ARMLESS;
}

void MonkeySwingSnap(ITEM_INFO* item, COLL_INFO* coll)
{
    FLOOR_INFO* floor;
    short* bounds;
    int ceiling;
    short roomNumber;

    bounds = GetBoundsAccurate(item);
    roomNumber = item->roomNumber;
    floor = GetFloor(item->pos.xPos, item->pos.yPos, item->pos.zPos, &roomNumber);
    ceiling = GetCeiling(floor, item->pos.xPos, item->pos.yPos, item->pos.zPos);
    item->pos.yPos = ceiling - bounds[2]; // based on collision now !
}

void FallFromRope(ITEM_INFO* item)
{
    SetAnimationForItemAS(item, ANIMATION_LARA_FREE_FALL_FORWARD, STATE_LARA_JUMP_FORWARD);
    item->speed = (abs(current_pendulum.vel.x >> 16) + abs(current_pendulum.vel.z >> 16)) >> 1;
    item->pos.xRot = 0;
    item->pos.yPos += 320;
    item->fallspeed = 0;
    item->gravityStatus = TRUE;
    Lara.gunStatus = LHS_ARMLESS;
    Lara.ropePtr = NO_ROPE;
}

void UpdateRopeSwing(ITEM_INFO* item)
{
    bool rope_swing = false;

    if (Lara.ropeMaxXForward > 9000)
        Lara.ropeMaxXForward = 9000;
    if (Lara.ropeMaxXBackward > 9000)
        Lara.ropeMaxXBackward = 9000;

    if (Lara.ropeDirection)
    {
        if (item->pos.xRot > 0 && item->pos.xRot - Lara.ropeLastX < -100)
        {
            Lara.ropeArcFront = Lara.ropeLastX;
            Lara.ropeDirection = 0;
            Lara.ropeMaxXBackward = 0;
            short frame = 15 * Lara.ropeMaxXForward / 18000 + Anims[ANIMATION_LARA_ROPE_SWING_FORWARD_SEMIHARD].frameBase + 47 << 8;
            if (frame > Lara.ropeDFrame)
            {
                Lara.ropeDFrame = frame;
                rope_swing = true;
            }
            else
            {
                rope_swing = false;
            }

            SoundEffect(SFX_LARA_ROPE_CREAK, &item->pos, 0);
        }
        else if (Lara.ropeLastX < 0 && Lara.ropeFrame == Lara.ropeDFrame)
        {
            Lara.ropeDFrame = 15 * Lara.ropeMaxXBackward / 18000 + Anims[ANIMATION_LARA_ROPE_SWING_FORWARD_SEMIHARD].frameBase + 47 << 8;
            Lara.ropeFrameRate = 15 * Lara.ropeMaxXBackward / 9000 + 1;
            rope_swing = false;
        }
        else if (Lara.ropeFrameRate < 512)
        {
            int framerate = rope_swing ? 32 : 8;
            Lara.ropeFrameRate += framerate * Lara.ropeMaxXBackward / 9000 + 1;
        }
    }
    else
    {
        if (item->pos.xRot < 0 && (item->pos.xRot - Lara.ropeLastX) > 100)
        {
            Lara.ropeArcBack = Lara.ropeLastX;
            Lara.ropeDirection = 1;
            Lara.ropeMaxXForward = 0;
            short frame = Anims[ANIMATION_LARA_ROPE_SWING_FORWARD_SEMIHARD].frameBase - 15 * Lara.ropeMaxXBackward / 18000 + 17 << 8;
            if (frame > Lara.ropeDFrame)
            {
                Lara.ropeDFrame = frame;
                rope_swing = true;
            }
            else
            {
                rope_swing = false;
            }

            SoundEffect(SFX_LARA_ROPE_CREAK, &item->pos, 0);
        }
        else if (Lara.ropeLastX > 0 && Lara.ropeFrame == Lara.ropeDFrame)
        {
            Lara.ropeDFrame = Anims[ANIMATION_LARA_ROPE_SWING_FORWARD_SEMIHARD].frameBase - 15 * Lara.ropeMaxXForward / 18000 + 17 << 8;
            Lara.ropeFrameRate = 15 * Lara.ropeMaxXForward / 9000 + 1;
            rope_swing = false;
        }
        else if (Lara.ropeFrameRate < 512)
        {
            int framerate = rope_swing ? 32 : 8;
            Lara.ropeFrameRate += framerate * Lara.ropeMaxXForward / 9000 + 1;
        }
    }

    Lara.ropeLastX = item->pos.xRot;
    if (Lara.ropeDirection)
    {
        if (item->pos.xRot > Lara.ropeMaxXForward)
            Lara.ropeMaxXForward = item->pos.xRot;
    }
    else
    {
        if (item->pos.xRot < -Lara.ropeMaxXBackward)
            Lara.ropeMaxXBackward = abs(item->pos.xRot);
    }
}

void ApplyVelocityToRope(int node, short angle, short n)
{
    SetPendulumVelocity((USHORT)n * SIN(angle) >> 2, 0, (USHORT)n * COS(angle) >> 2); // TODO: z not need unsigned short, it cause a glitch !
}

void SetPendulumVelocity(int x, int y, int z)
{
    if ((current_pendulum.node & ~1) < 24)
    {
        int vel = 4096 / ((12 - (current_pendulum.node >> 1)) << 9 >> 8) << 8;
        x = (long long)vel * x >> 16;
        y = (long long)vel * y >> 16;
        z = (long long)vel * z >> 16;
    }

    current_pendulum.vel.x += x;
    current_pendulum.vel.y += y;
    current_pendulum.vel.z += z;
}

void JumpOffRope(ITEM_INFO* item)
{
    if (Lara.ropePtr == NO_ROPE)
        return;

    if (item->pos.xRot >= 0)
    {
        item->fallspeed = -112;
        item->speed = item->pos.xRot / (STEP_L / 2);
    }
    else
    {
        item->fallspeed = -20;
        item->speed = 0;
    }

    item->pos.xRot = 0;
    item->gravityStatus = TRUE;
    Lara.gunStatus = LHS_ARMLESS;

    short frame = (item->frameNumber - Anims[ANIMATION_LARA_ROPE_SWING_FORWARD_SEMIHARD].frameBase);
    if (frame > 21)
        item->animNumber = ANIMATION_LARA_ROPE_SWING_TO_TRY_HANG_MIDDLE;
    else if (frame > 42)
        item->animNumber = ANIMATION_LARA_ROPE_SWING_TO_TRY_HANG_FRONT2;
    else
        item->animNumber = ANIMATION_LARA_ROPE_SWING_TO_TRY_HANG_BACK;

    item->frameNumber = Anims[item->animNumber].frameBase;
    item->currentAnimState = STATE_LARA_REACH;
    item->goalAnimState = STATE_LARA_REACH;
    Lara.ropePtr = NO_ROPE;
}

void SetCornerAnim(ITEM_INFO* item, COLL_INFO* coll, short angle, short flip)
{
    Lara.look = FALSE;
    coll->enableSpaz = FALSE;
    coll->enableBaddiePush = FALSE;

    if (CHK_NOP(TrInput, IN_ACTION) || item->hitPoints <= 0)
    {
        SetAnimationForItemAS(item, ANIMATION_LARA_FREE_FALL_FORWARD, STATE_LARA_JUMP_FORWARD);
        item->pos.yPos += STEP_L;
        item->pos.yRot += angle / 2;
        item->speed = 2;
        item->fallspeed = 1;
        item->gravityStatus = TRUE;
        Lara.gunStatus = LHS_ARMLESS;
        return;
    }

    if (!flip)
        return;

    if (Lara.isClimbing)
        SetAnimationForItemAS(item, ANIMATION_LARA_LADDER_IDLE, STATE_LARA_LADDER_IDLE);
    else
        SetAnimationForItemASF(item, ANIMATION_LARA_HANG_IDLE, STATE_LARA_HANG, 21);

    item->pos.xPos = Lara.cornerX;
    item->pos.zPos = Lara.cornerZ;
    coll->old.x = Lara.cornerX;
    coll->old.z = Lara.cornerZ;
    item->pos.yRot += angle;
}

BOOL CanLaraHangSideways(ITEM_INFO* item, COLL_INFO* coll, short angle)
{
    BOOL hang_test;
    SHORT world_angle;
    int old_x;
    int old_z;
    int x;
    int z;
    
    old_x = item->pos.xPos;
    old_z = item->pos.zPos;
    Lara.moveAngle = angle + item->pos.yRot;
    x = item->pos.xPos;
    z = item->pos.zPos;
    world_angle = (USHORT)(Lara.moveAngle + ANGLE(45)) / ANGLE(90);

    switch (world_angle)
    {
        case NORTH:
            z += 16;
            break;
        case EAST:
            x += 16;
            break;
        case SOUTH:
            z -= 16;
            break;
        case WEST:
            x -= 16;
            break;
    }

    item->pos.xPos = x;
    item->pos.zPos = z;
    coll->old.y = item->pos.yPos;
    hang_test = LaraHangTest(item, coll);
    item->pos.xPos = old_x;
    item->pos.zPos = old_z;
    Lara.moveAngle = angle + item->pos.yRot;
    return !hang_test;
}

short GetDirOctant(int angle)
{
    if (abs(angle) >= 0x2000 && abs(angle) <= 0x6000)
        return 1;
    else
        return 0;
}

short GetClimbTrigger(int x, int y, int z, short roomNumber)
{
    FLOOR_INFO* floor;
    short* index;

    floor = GetFloor(x, y, z, &roomNumber);
    GetFloorHeight(floor, x, y, z);
    index = TriggerIndex;

    if (!index)
        return 0;

    if ((*index & 0x1F) == 5) // climbing trigger ?
    {
        if (((*index >> 8) & 0x80u) != 0)
            return 0;
        index = TriggerIndex + 1;
    }

    return (*index & 0x1F) == 6 ? *index : NULL;
}

void SnapLaraToEdgeOfBlock(ITEM_INFO* item, COLL_INFO* coll, short angle)
{
    if (item->currentAnimState == STATE_LARA_SHIMMY_RIGHT)
    {
        switch (angle)
        {
            case NORTH:
                item->pos.xPos = coll->old.x & ~111 | 912;
                break;
            case EAST:
                item->pos.xPos = coll->old.x & ~911 | 112;
                break;
            case SOUTH:
                item->pos.xPos = coll->old.x & ~911 | 112;
                break;
            case WEST:
                item->pos.xPos = coll->old.x & ~111 | 912;
                break;
        }
    }
    else if (item->currentAnimState == STATE_LARA_SHIMMY_LEFT)
    {
        switch (angle)
        {
            case NORTH:
                item->pos.xPos = coll->old.x & ~911 | 112;
                break;
            case EAST:
                item->pos.xPos = coll->old.x & ~111 | 911;
                break;
            case SOUTH:
                item->pos.xPos = coll->old.x & ~111 | 911;
                break;
            case WEST:
                item->pos.xPos = coll->old.x & ~911 | 112;
                break;
        }
    }
}

void GetLaraCollisionInfo(ITEM_INFO* item, COLL_INFO* coll)
{
    coll->facing = Lara.moveAngle;
    GetCollisionInfo(coll, item->pos.xPos, item->pos.yPos, item->pos.zPos, item->roomNumber, LARA_HITE);
}

void AnimateLara(ITEM_INFO* item)
{
    ANIM_STRUCT* anim;
    WORD num, type;
    short* command;
    int speed;

    item->frameNumber++;
    anim = &Anims[item->animNumber];

    // anim transition
    if (anim->numberChanges > 0)
    {
        if (GetChange(item, anim))
        {
            anim = &Anims[item->animNumber];
            item->currentAnimState = anim->currentStateAnim;
        }
    }

    // command
    if (item->frameNumber > anim->frameEnd)
    {
        if (anim->numberCommands > 0)
        {
            command = &commands[anim->commandIndex];
            for (short i = anim->numberCommands; i > 0; i--)
            {
                switch (*(command++))
                {
                    case COMMAND_MOVE_ORIGIN:
                        TranslateItem(item, (int)*(command), (int)*(command + 1), (int)*(command + 2));
                        UpdateLaraRoom(item, -(LARA_HITE / 2));
                        command += 3;
                        break;
                    case COMMAND_JUMP_VELOCITY:
                        item->fallspeed = *(command++);
                        item->speed = *(command++);
                        item->gravityStatus = TRUE;
                        if (Lara.calcFallspeed)
                        {
                            item->fallspeed = Lara.calcFallspeed;
                            Lara.calcFallspeed = 0;
                        }
                        break;
                    case COMMAND_ATTACK_READY:
                        if (Lara.gunStatus != LHS_SPECIAL)
                            Lara.gunStatus = LHS_ARMLESS;
                        break;
                    case COMMAND_DEACTIVATE:
                        break;
                    case COMMAND_SOUND_FX:
                    case COMMAND_EFFECT:
                        command += 2;
                        break;
                }
            }
        }

        item->animNumber = anim->jumpAnimNum;
        item->frameNumber = anim->jumpFrameNum;
        anim = &Anims[item->animNumber];
        item->currentAnimState = anim->currentStateAnim;
    }

    // sound or effect.
    if (anim->numberCommands > 0)
    {
        command = &commands[anim->commandIndex];
        for (short i = anim->numberCommands; i > 0; i--)
        {
            switch (*command++)
            {
                case COMMAND_MOVE_ORIGIN:
                    command += 3;
                    break;

                case COMMAND_JUMP_VELOCITY:
                    command += 2;
                    break;

                case COMMAND_ATTACK_READY:
                case COMMAND_DEACTIVATE:
                    break;

                case COMMAND_SOUND_FX:
                    if (item->frameNumber == *(command))
                    {
                        type = *(command + 1) & 0xC000;
                        if  (type == SFX_LANDANDWATER ||
                            (type == SFX_LANDONLY  && (Lara.waterSurfaceDist >= 0 || Lara.waterSurfaceDist == -NO_HEIGHT)) ||
                            (type == SFX_WATERONLY &&  Lara.waterSurfaceDist <  0 && Lara.waterSurfaceDist != -NO_HEIGHT))
						{
							num = *(command + 1) & 0x3FFF;
							SoundEffect((int)num, &item->pos, SFX_ALWAYS);
						}
                    }
                    command += 2;
                    break;

                case COMMAND_EFFECT:
                    if (item->frameNumber == *(command))
                    {
                        FXType = command[1] & 0xC000;
                        effect_routines[command[1] & 0x3FFF](item);
                    }
                    command += 2;
                    break;
            }
        }
    }

    // velocity
    if (item->gravityStatus)
    {
        speed = anim->ySpeed + anim->yFallspeed * (item->frameNumber - anim->frameBase - 1);
        item->speed -= (short)(speed >> 16);
        speed += anim->yFallspeed;
        item->speed += (short)(speed >> 16);
        item->fallspeed += (item->fallspeed < FASTFALL_SPEED) ? GRAVITY : 1;
        item->pos.yPos += item->fallspeed;
    }
    else
    {
        speed = anim->ySpeed;
        if (anim->yFallspeed)
            speed += (anim->yFallspeed * (item->frameNumber - anim->frameBase));
        item->speed = (short)(speed >> 16);
    }

    // rope
    if (Lara.ropePtr != NO_ROPE)
        AlignLaraToRope(item);

    if (!Lara.isMoving)
    {
        item->pos.xPos += (item->speed * SIN(Lara.moveAngle)) >> W2V_SHIFT;
        item->pos.zPos += (item->speed * COS(Lara.moveAngle)) >> W2V_SHIFT;
    }
}

BOOL LaraTestHangJump(ITEM_INFO* item, COLL_INFO* coll)
{
    short* bounds;
    int edge_catch, edge;
    short angle;

    if (CHK_NOP(TrInput, IN_ACTION) || Lara.gunStatus == LHS_HANDBUSY || coll->hitStatic)
        return FALSE;

    if (Lara.canMonkeySwing && coll->collType == COLL_TOP)
    {
        Lara.headYrot = 0;
        Lara.headXrot = 0;
        Lara.torsoYrot = 0;
        Lara.torsoXrot = 0;
        item->fallspeed = 0;
        item->speed = 0;
        item->gravityStatus = FALSE;
        Lara.gunStatus = LHS_HANDBUSY;
        SetAnimationForItemAS(item, ANIMATION_LARA_OSCILLATE_HANG_ON, STATE_LARA_MONKEYSWING_IDLE);
        return TRUE;
    }

    if (coll->middle.ceiling > -STEPUP_HEIGHT || coll->middle.floor < 200 || coll->collType != COLL_FRONT)
        return FALSE;

    edge_catch = LaraTestEdgeCatch(item, coll, &edge);
    if (!edge_catch)
        return FALSE;
    if (edge_catch < 0 && !LaraTestHangOnClimbWall(item, coll))
        return FALSE;

    angle = GetCatchAngle(item, HANG_ANGLE);
    if (angle & 0x3FFF)
        return FALSE;

    if (TestHangSwingIn(item, angle))
    {
        Lara.headYrot = 0;
        Lara.headXrot = 0;
        Lara.torsoYrot = 0;
        Lara.torsoXrot = 0;
        SetAnimationForItemAS(item, ANIMATION_LARA_OSCILLATE_HANG_ON, STATE_LARA_MONKEYSWING_IDLE);
    }
    else
    {
        SetAnimationForItemAS(item, ANIMATION_LARA_HANG_IDLE, STATE_LARA_HANG);
    }

    bounds = GetBoundsAccurate(item);
    if (edge_catch > 0)
    {
        item->pos.yPos += coll->front.floor - bounds[2]; // catch edge

        SHORT rotationAngle = (USHORT)(item->pos.yRot + 0x2000) / 0x4000;
        switch (rotationAngle)
        {
        case NORTH:
            item->pos.zPos = (item->pos.zPos |  (WALL_L - 1)) - LARA_RAD;
            item->pos.xPos += coll->shift.x;
            break;
        case EAST:
            item->pos.xPos = (item->pos.xPos |  (WALL_L - 1)) - LARA_RAD;
            item->pos.zPos += coll->shift.z;
            break;
        case SOUTH:
            item->pos.zPos = (item->pos.zPos & ~(WALL_L - 1)) + LARA_RAD;
            item->pos.xPos += coll->shift.x;
            break;
        case WEST:
            item->pos.xPos = (item->pos.xPos & ~(WALL_L - 1)) + LARA_RAD;
            item->pos.zPos += coll->shift.z;
            break;
        }
    }
    else
    {
        item->pos.yPos = edge - bounds[2];
    }

    item->pos.yRot = angle;
    item->gravityStatus = TRUE;
    item->speed = 2;
    item->fallspeed = 1;
    Lara.gunStatus = LHS_HANDBUSY;
    return TRUE;
}

BOOL LaraTestHangJumpUp(ITEM_INFO* item, COLL_INFO* coll)
{
    short* bounds;
    int edge, edge_catch;
    short angle;

    if (CHK_NOP(TrInput, IN_ACTION) || Lara.gunStatus != LHS_ARMLESS || coll->hitStatic)
        return FALSE;

    if (Lara.canMonkeySwing && coll->collType == COLL_TOP)
    {
        SetAnimationForItemAS(item, ANIMATION_LARA_MONKEY_GRAB, STATE_LARA_MONKEYSWING_IDLE);
        item->speed = 0;
        item->fallspeed = 0;
        item->gravityStatus = FALSE;
        Lara.gunStatus = LHS_HANDBUSY;
        MonkeySwingSnap(item, coll);
        return TRUE;
    }

    if (coll->collType != COLL_FRONT || coll->middle.ceiling > -STEPUP_HEIGHT)
        return FALSE;

    edge_catch = LaraTestEdgeCatch(item, coll, &edge);
    if (!edge_catch)
        return FALSE;

    if (edge_catch < 0 && !LaraTestHangOnClimbWall(item, coll))
        return FALSE;

    angle = GetCatchAngle(item, HANG_ANGLE);
    if (angle & 0x3FFF)
        return FALSE;

    if (TestHangSwingIn(item, angle))
        SetAnimationForItemAS(item, ANIMATION_LARA_MONKEY_GRAB, STATE_LARA_MONKEYSWING_IDLE);
    else
        SetAnimationForItemASF(item, ANIMATION_LARA_HANG_IDLE, STATE_LARA_HANG, LARA_HANG_IDLE_FRAME);

    bounds = GetBoundsAccurate(item);
    if (edge_catch > 0)
        item->pos.yPos += coll->front.floor - bounds[2];
    else
        item->pos.yPos = edge - bounds[2];

    item->pos.xPos += coll->shift.x;
    item->pos.zPos += coll->shift.z;
    item->pos.yRot = angle;
    item->speed = 0;
    item->fallspeed = 0;
    item->gravityStatus = FALSE;
    Lara.torsoYrot = 0;
    Lara.torsoXrot = 0;
    Lara.gunStatus = LHS_HANDBUSY;
    return TRUE;
}

#ifdef DLL_INJECT
void injector::f_game::inject_larautils()
{
    inject(0x00421620, LaraFloorFront);
    inject(0x00420A80, LaraCeilingFront);
    inject(0x00422390, LaraHitCeiling);
    inject(0x004230E0, LaraHangTest);
    inject(0x00422400, LaraDeflectEdge);
    inject(0x00421880, LaraDeflectEdgeDuck);
    inject(0x00422C50, LaraDeflectEdgeJump);
    inject(0x00420FE0, LaraFallen);
    inject(0x00426600, IsValidHangPos);
    inject(0x00426230, LaraHangRightCornerTest);
    inject(0x004266E0, LaraHangLeftCornerTest);
    inject(0x0042CEE0, LaraClimbLeftCornerTest);
    inject(0x0042D160, LaraClimbRightCornerTest);
    inject(0x004259C0, LaraSlideEdgeJump);
    inject(0x00422810, LaraTestClimbStance);
    inject(0x00421DE0, LaraTestEdgeCatch);
    inject(0x00428470, LaraSlideSlope);
    inject(0x00428B20, LaraCollideJump);
    inject(0x0042C980, LaraCheckForLetGo);
    inject(0x0042CA60, LaraTestClimbUpPos);
    inject(0x0042C3C0, LaraTestClimbPos);
    inject(0x0042C470, LaraTestClimb);
    inject(0x0042CD20, LaraDoClimbLeftRight);
    inject(0x00431E90, LaraTestWaterClimbOut);
    inject(0x004321B0, LaraTestWaterStepOut);
    inject(0x00432A30, LaraTestWaterDepth);
    inject(0x00422BE0, LaraLandedBad);
    inject(0x004229D0, LaraCollideStop);
    inject(0x004328E0, LaraSwimCollision);
    inject(0x004320B0, LaraSurfaceCollision);
    //inject(0x00424E90, UseSpecialItem);
    inject(0x00420EA0, TestLaraSlide);
    inject(0x00422480, TestLaraVault);
    inject(0x004236B0, TestMonkeyLeft);
    inject(0x004237B0, TestMonkeyRight);
    inject(0x00421FF0, TestHangSwingIn);
    inject(0x004228D0, TestWall);
    inject(0x00432690, SwimTurn);
    inject(0x00422E50, MonkeySwingFall);
    inject(0x00423660, MonkeySwingSnap);
    inject(0x004241D0, FallFromRope);
    inject(0x00424320, UpdateRopeSwing);
    inject(0x00424150, ApplyVelocityToRope);
    inject(0x004597D0, SetPendulumVelocity);
    inject(0x00426C50, SetCornerAnim);
    inject(0x00426AB0, CanLaraHangSideways);
    inject(0x00423780, GetDirOctant);
    inject(0x0042D7D0, GetClimbTrigger);
    inject(0x004235B0, SnapLaraToEdgeOfBlock);
    inject(0x00422350, GetLaraCollisionInfo);
    inject(0x00430B60, AnimateLara);
}
#endif