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
    newX = item->pos.x + sin;
    newY = item->pos.y - LARA_HITE;
    newZ = item->pos.z + cos;
    roomNumber = item->room_number;
    floor = GetFloor(newX, newY, newZ, &roomNumber);
    height = GetHeight(floor, newX, newY, newZ);
    if (height != -NO_HEIGHT)
        height -= item->pos.y;
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
    newX = item->pos.x + sin;
    newY = item->pos.y - height;
    newZ = item->pos.z + cos;
    roomNumber = item->room_number;
    floor = GetFloor(newX, newY, newZ, &roomNumber);
    ceiling = GetCeiling(floor, newX, newY, newZ);
    if (ceiling != -NO_HEIGHT)
        ceiling += newY;
    return short(ceiling);
}

BOOL LaraHitCeiling(ITEM_INFO* item, COLL_INFO* coll)
{
    if (coll->coll_type != COLL_TOP || coll->coll_type != COLL_CLAMP)
        return FALSE;

    item->pos.x = coll->old.x;
    item->pos.y = coll->old.y;
    item->pos.z = coll->old.z;
    item->speed = 0;
    item->fallspeed = 0;
    item->gravity_status = FALSE;
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
    old_move_angle = lara.move_angle;

    // always 0 for coll_dist ?
    if (lara.move_angle == (item->pos.y_rot - 0x4000))
        coll_dist = -LARA_RAD;
    else if (lara.move_angle == (item->pos.y_rot + 0x4000))
        coll_dist = LARA_RAD;
    else
        coll_dist = 0;

    front = LaraFloorFront(item, lara.move_angle, LARA_RAD);
    if (front < 200)
        hit = true;

    ceiling = LaraCeilingFront(item, lara.move_angle, LARA_RAD, 0);
    SHORT angle = (USHORT)(item->pos.y_rot + 0x2000) / 0x4000;
    // allow lara to grab (else it's a loop grab/ungrab)
    switch (angle)
    {
        case NORTH:
            item->pos.z += 4;
            break;
        case EAST:
            item->pos.x += 4;
            break;
        case SOUTH:
            item->pos.z -= 4;
            break;
        case WEST:
            item->pos.x -= 4;
            break;
    }

    lara.move_angle = item->pos.y_rot;
    coll->bad_pos = NO_HEIGHT;
    coll->bad_neg = -STEPUP_HEIGHT;
    coll->bad_ceiling = 0;
    GetLaraCollisionInfo(item, coll);

    if (lara.climb_status)
    {
        if (CHK_NOP(TrInput, IN_ACTION) || item->hit_points <= 0)
        {
            SetAnimationForItemAS(item, ANIMATION_LARA_FREE_FALL_FORWARD, STATE_LARA_JUMP_FORWARD);
            item->pos.y += STEP_L;
            item->speed = 2;
            item->fallspeed = 1;
            item->gravity_status = TRUE;
            lara.gun_status = LHS_ARMLESS;
            return FALSE;
        }

        lara.move_angle = old_move_angle;
        
        if (!LaraTestHangOnClimbWall(item, coll))
        {
            if (item->current_anim != ANIMATION_LARA_LADDER_TO_HANDS_LEFT && item->current_anim != ANIMATION_LARA_LADDER_TO_HANDS_RIGHT)
            {
                SnapLaraToEdgeOfBlock(item, coll, angle);
                item->pos.y = coll->old.y;
                SetAnimationForItemASF(item, ANIMATION_LARA_HANG_IDLE, STATE_LARA_HANG, 21);
            }
            return TRUE;
        }
        else if (item->current_anim == ANIMATION_LARA_HANG_IDLE && item->current_frame == (anims[ANIMATION_LARA_HANG_IDLE].frame_base + 21) && LaraTestClimbStance(item, coll))
            item->state_next = STATE_LARA_LADDER_IDLE;

        return FALSE;
    }

    if (CHK_NOP(TrInput, IN_ACTION) || item->hit_points <= 0 || coll->front_floor > 0)
    {
        SetAnimationForItemASF(item, ANIMATION_LARA_TRY_HANG_VERTICAL, STATE_LARA_JUMP_UP, 9);
        item->pos.x += coll->shift.x;
        item->pos.y = coll->old.y + (STEP_L / 2); // more accurate than bounds collision (STEP_L/2 fix the height when action released!)
        item->pos.z += coll->shift.z;
        item->speed = 2;
        item->fallspeed = 1;
        item->gravity_status = TRUE;
        lara.gun_status = LHS_ARMLESS;
        return FALSE;
    }

    if (hit && front > 0)
    {
        if (coll_dist > 0 && coll->left_floor > coll->right_floor)
            hit = false;
        else if (coll_dist < 0 && coll->left_floor < coll->right_floor)
            hit = false;
    }

    bounds = GetBoundsAccurate(item);
    old_coll_front = coll->front_floor;
    coll_front = coll->front_floor - bounds[2];
    isLeftRightBlocked = false;
    switch (angle)
    {
        case NORTH:
            xColl = item->pos.x + coll_dist;
            zColl = item->pos.z;
            break;
        case EAST:
            xColl = item->pos.x;
            zColl = item->pos.z - coll_dist;
            break;
        case SOUTH:
            xColl = item->pos.x - coll_dist;
            zColl = item->pos.z;
            break;
        case WEST:
            xColl = item->pos.x;
            zColl = item->pos.z + coll_dist;
            break;
    }

    lara.move_angle = old_move_angle;
    if (GetClimbTrigger(xColl, item->pos.y, zColl, item->room_number) & (STEP_L << angle))
    {
        if (!LaraTestHangOnClimbWall(item, coll))
            coll_front = 0;
    }
    else
    {
        if (abs(coll->left_floor2 - coll->right_floor2) >= SLOPE_DIF)
        {
            if ((coll_dist < 0 || coll_dist > 0)
            && ((coll->left_floor2  != coll->front_floor)
            ||  (coll->right_floor2 != coll->front_floor)))
            {
                isLeftRightBlocked = true;
            }
            else if (coll_dist == 0 // this "else if" not exist ! so i added it.
            &&     ((coll->left_floor2 != coll->front_floor)
            ||     (coll->right_floor2 != coll->front_floor)))
            {
                isLeftRightBlocked = true;
            }
        }
    }

    coll->front_floor = old_coll_front;
    if (isLeftRightBlocked
    ||  coll->mid_ceiling >= 0
    ||  coll->coll_type != COLL_FRONT
    ||  hit
    ||  coll->hit_static
    ||  ceiling > -950
    ||  coll_front < -SLOPE_DIF
    ||  coll_front > SLOPE_DIF)
    {
        item->pos.x = coll->old.x;
        item->pos.y = coll->old.y;
        item->pos.z = coll->old.z;
        if (item->state_current == STATE_LARA_SHIMMY_LEFT || item->state_current == STATE_LARA_SHIMMY_RIGHT)
            SetAnimationForItemASF(item, ANIMATION_LARA_HANG_IDLE, STATE_LARA_HANG, LARA_HANG_IDLE_FRAME_AFTERLEFTRIGHT);
        return TRUE;
    }

    switch (angle)
    {
        case NORTH:
        case SOUTH:
            item->pos.z += coll->shift.z;
            item->pos.y += coll_front;
            break;
        case EAST:
        case WEST:
            item->pos.x += coll->shift.x;
            item->pos.y += coll_front;
            break;
        default:// just in case since it's on default in idapro...
            item->pos.y += coll_front;
            break;
    }

    // return false by default there and not in switch !
    return FALSE;
}

BOOL LaraDeflectEdge(ITEM_INFO* item, COLL_INFO* coll)
{
    switch (coll->coll_type)
    {
        case COLL_FRONT:
        case COLL_TOPFRONT:
            ShiftItem(item, coll);
            item->state_next = STATE_LARA_IDLE;
            item->speed = 0;
            item->gravity_status = FALSE;
            return TRUE;

        case COLL_LEFT:
            ShiftItem(item, coll);
            item->pos.y_rot += ANGLE(5);
            break;
        case COLL_RIGHT:
            ShiftItem(item, coll);
            item->pos.y_rot -= ANGLE(5);
            break;
    }

    return FALSE;
}

BOOL LaraDeflectEdgeDuck(ITEM_INFO* item, COLL_INFO* coll)
{
    switch (coll->coll_type)
    {
        case COLL_FRONT:
        case COLL_TOPFRONT:
            ShiftItem(item, coll);
            item->speed = 0;
            item->gravity_status = FALSE;
            return TRUE;

        case COLL_LEFT:
            ShiftItem(item, coll);
            item->pos.y_rot += ANGLE(2);
            break;
        case COLL_RIGHT:
            ShiftItem(item, coll);
            item->pos.y_rot -= ANGLE(2);
            break;
    }

    return FALSE;
}

void LaraDeflectEdgeJump(ITEM_INFO* item, COLL_INFO* coll)
{
    ShiftItem(item, coll);
    switch (coll->coll_type)
    {
        case COLL_FRONT:
        case COLL_TOPFRONT:
            if (!lara.climb_status || item->speed != 2)
            {
                if (coll->mid_floor <= (STEP_L / 2))
                    SetAnimationForItemAS(item, ANIMATION_LARA_LANDING_LIGHT, STATE_LARA_GRAB_TO_FALL);
                else
                    SetAnimationForItemASF(item, ANIMATION_LARA_SMASH_JUMP, STATE_LARA_FREEFALL, 1);
                item->speed /= 4;
                lara.move_angle = item->pos.y_rot - 0x8000;
                if (item->fallspeed <= 0)
                    item->fallspeed = 1;
            }
            break;

        case COLL_LEFT:
            item->pos.y_rot += ANGLE(5);
            break;
        case COLL_RIGHT:
            item->pos.y_rot -= ANGLE(5);
            break;

        case COLL_TOP:
            if (item->fallspeed <= 0)
                item->fallspeed = 1;
            break;

        case COLL_CLAMP:
            item->pos.x -= LARA_RAD * SIN(coll->facing) >> W2V_SHIFT;
            item->pos.z -= LARA_RAD * COS(coll->facing) >> W2V_SHIFT;
            item->speed = 0;
            coll->mid_floor = 0;
            if (item->fallspeed <= 0)
                item->fallspeed = 16;
            break;
    }
}

BOOL LaraFallen(ITEM_INFO* item, COLL_INFO* coll)
{
    if (lara.water_status != LWS_WADE && coll->mid_floor > STEPUP_HEIGHT)
    {
        SetAnimationForItemAS(item, ANIMATION_LARA_FREE_FALL_FORWARD, STATE_LARA_JUMP_FORWARD);
        item->fallspeed = 0;
        item->gravity_status = TRUE;
        return TRUE;
    }

    return FALSE;
}

int IsValidHangPos(ITEM_INFO* item, COLL_INFO* coll)
{
    SHORT angle;
    short front;

    front = LaraFloorFront(item, lara.move_angle, LARA_RAD);
    if (front < 200)
        return FALSE;

    angle = (USHORT)(item->pos.y_rot + 0x2000) / 0x4000;
    // allow lara to grab (else it's a loop grab/ungrab)
    switch (angle)
    {
        case NORTH:
            item->pos.z += 4;
            break;
        case EAST:
            item->pos.x += 4;
            break;
        case SOUTH:
            item->pos.z -= 4;
            break;
        case WEST:
            item->pos.x -= 4;
            break;
    }

    lara.move_angle = item->pos.y_rot;
    coll->bad_pos = NO_HEIGHT;
    coll->bad_neg = -(STEP_L * 2);
    coll->bad_ceiling = 0;
    GetLaraCollisionInfo(item, coll);

    if (coll->mid_ceiling >= 0 || coll->coll_type != COLL_FRONT || coll->hit_static)
        return FALSE;

    if (abs(coll->front_floor - coll->right_floor2) >= SLOPE_DIF)
        return FALSE;
    else
        return TRUE;
}

int LaraHangRightCornerTest(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->current_anim != ANIMATION_LARA_HANG_IDLE)
        return 0;
    if (coll->hit_static)
        return 0;

    int x, z;
    int old_x = item->pos.x;
    int old_z = item->pos.z;
    int old_front_floor = coll->front_floor;
    SHORT old_angle = item->pos.y_rot;
    SHORT angle = (USHORT)(item->pos.y_rot + ANGLE(45)) / ANGLE(90);

    if (angle && angle != 2)
    {
        x = (item->pos.x & ~(WALL_L - 1)) - (item->pos.z & (WALL_L - 1)) + SECTOR(1);
        z = (item->pos.z & ~(WALL_L - 1)) - (item->pos.x & (WALL_L - 1)) + SECTOR(1);
    }
    else
    {
        x = item->pos.x ^ (item->pos.x ^ item->pos.z) & (WALL_L - 1);
        z = item->pos.z ^ (item->pos.x ^ item->pos.z) & (WALL_L - 1);
    }

    item->pos.x = x;
    lara.corner_x = x;
    item->pos.z = z;
    lara.corner_z = z;
    item->pos.y_rot += ANGLE(90);
    int result = -IsValidHangPos(item, coll);
    if (result)
    {
        if (lara.climb_status)
        {
            if (GetClimbTrigger(x, item->pos.y, z, item->room_number) & LeftClimbTab[angle])
            {
                item->pos.x = old_x;
                item->pos.z = old_z;
                item->pos.y_rot = old_angle;
                lara.move_angle = old_angle;
                return result;
            }
        }
        else
        {
            if (abs(old_front_floor - coll->front_floor) <= SLOPE_DIF)
            {
                item->pos.x = old_x;
                item->pos.z = old_z;
                item->pos.y_rot = old_angle;
                lara.move_angle = old_angle;
                return result;
            }
        }
    }

    item->pos.x = old_x;
    item->pos.z = old_z;
    item->pos.y_rot = old_angle;
    lara.move_angle = old_angle;
    if (LaraFloorFront(item, old_angle + ANGLE(90), LARA_RAD + 16) < 0)
        return 0;

    switch (angle)
    {
        case NORTH:
            x = ((item->pos.x + SECTOR(1)) & ~(WALL_L - 1)) - (item->pos.z & (WALL_L - 1)) + SECTOR(1);
            z = ((item->pos.z + SECTOR(1)) & ~(WALL_L - 1)) - (item->pos.x & (WALL_L - 1)) + SECTOR(1);
            break;
        case SOUTH:
            x = ((item->pos.x - SECTOR(1)) & ~(WALL_L - 1)) - (item->pos.z & (WALL_L - 1)) + SECTOR(1);
            z = ((item->pos.z - SECTOR(1)) & ~(WALL_L - 1)) - (item->pos.x & (WALL_L - 1)) + SECTOR(1);
            break;
        case WEST:
            x = (item->pos.x ^ (item->pos.x ^ item->pos.z) & (WALL_L - 1)) - SECTOR(1);
            z = (item->pos.x ^ item->pos.z) & (WALL_L - 1) ^ (item->pos.z + SECTOR(1));
            break;
        default:
        case EAST:
            x = ((item->pos.x ^ item->pos.z) & (WALL_L - 1)) ^ (item->pos.x + SECTOR(1));
            z = (item->pos.z ^ ((item->pos.x ^ item->pos.z) & (WALL_L - 1))) - SECTOR(1);
            break;
    }

    item->pos.x = x;
    lara.corner_x = x;
    item->pos.z = z;
    lara.corner_z = z;
    item->pos.y_rot -= ANGLE(90);

    result = IsValidHangPos(item, coll);
    if (!result)
    {
        item->pos.x = old_x;
        item->pos.z = old_z;
        item->pos.y_rot = old_angle;
        lara.move_angle = old_angle;
        return 0;
    }

    item->pos.x = old_x;
    item->pos.z = old_z;
    item->pos.y_rot = old_angle;
    lara.move_angle = old_angle;

    if (!lara.climb_status)
    {
        if (abs(old_front_floor - coll->front_floor) <= SLOPE_DIF)
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

    if (GetClimbTrigger(x, item->pos.y, z, item->room_number) & RightClimbTab[angle])
        return result;

    short front = LaraFloorFront(item, item->pos.y_rot, LARA_RAD + 16);
    if (abs(front - coll->front_floor) > SLOPE_DIF)
        return 0;

    if (front < -768)
        return 0;

    return result;
}

int LaraHangLeftCornerTest(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->current_anim != ANIMATION_LARA_HANG_IDLE)
        return 0;
    if (coll->hit_static)
        return 0;

    int x, z;
    int old_x = item->pos.x;
    int old_z = item->pos.z;
    int old_front_floor = coll->front_floor;
    SHORT old_angle = item->pos.y_rot;
    SHORT angle = (USHORT)(item->pos.y_rot + ANGLE(45)) / ANGLE(90);

    if (angle && angle != 2)
    {
        x = item->pos.x ^ (item->pos.x ^ item->pos.z) & (WALL_L - 1);
        z = item->pos.z ^ (item->pos.x ^ item->pos.z) & (WALL_L - 1);
    }
    else
    {
        x = (item->pos.x & ~(WALL_L - 1)) - (item->pos.z & (WALL_L - 1)) + SECTOR(1);
        z = (item->pos.z & ~(WALL_L - 1)) - (item->pos.x & (WALL_L - 1)) + SECTOR(1);
    }

    item->pos.x = x;
    lara.corner_x = x;
    item->pos.z = z;
    lara.corner_z = z;
    item->pos.y_rot -= ANGLE(90);

    int result = -IsValidHangPos(item, coll);
    if (result)
    {
        if (lara.climb_status)
        {
            if (GetClimbTrigger(x, item->pos.y, z, item->room_number) & RightClimbTab[angle])
            {
                item->pos.x = old_x;
                item->pos.z = old_z;
                item->pos.y_rot = old_angle;
                lara.move_angle = old_angle;
                return result;
            }
        }
        else
        {
            if (abs(old_front_floor - coll->front_floor) <= SLOPE_DIF)
            {
                item->pos.x = old_x;
                item->pos.z = old_z;
                item->pos.y_rot = old_angle;
                lara.move_angle = old_angle;
                return result;
            }
        }
    }

    item->pos.x = old_x;
    item->pos.z = old_z;
    item->pos.y_rot = old_angle;
    lara.move_angle = old_angle;
    if (LaraFloorFront(item, old_angle - 0x4000, LARA_RAD + 16) < 0)
        return 0;

    switch (angle)
    {
        case NORTH:
            x = (item->pos.x ^ ((item->pos.x ^ item->pos.z) & (WALL_L - 1))) - SECTOR(1);
            z = ((item->pos.x ^ item->pos.z) & (WALL_L - 1)) ^ (item->pos.z + SECTOR(1));
            break;
        case SOUTH:
            x = ((item->pos.x ^ item->pos.z) & (WALL_L - 1)) ^ (item->pos.x + SECTOR(1));
            z = ((item->pos.x ^ item->pos.z) & (WALL_L - 1)) ^ (item->pos.z - SECTOR(1));
            break;
        case WEST:
            x = (item->pos.x & ~(WALL_L - 1)) - (item->pos.z & (WALL_L - 1));
            z = (item->pos.z & ~(WALL_L - 1)) - (item->pos.x & (WALL_L - 1));
            break;
        default:
        case EAST:
            x = ((item->pos.x + SECTOR(1)) & ~(WALL_L - 1)) - (item->pos.z & (WALL_L - 1)) + SECTOR(1);
            z = ((item->pos.z + SECTOR(1)) & ~(WALL_L - 1)) - (item->pos.x & (WALL_L - 1)) + SECTOR(1);
            break;
    }

    item->pos.x = x;
    lara.corner_x = x;
    item->pos.z = z;
    lara.corner_z = z;
    item->pos.y_rot += ANGLE(90);
    result = IsValidHangPos(item, coll);
    if (!result)
    {
        item->pos.x = old_x;
        item->pos.z = old_z;
        item->pos.y_rot = old_angle;
        lara.move_angle = old_angle;
        return 0;
    }

    item->pos.x = old_x;
    item->pos.z = old_z;
    item->pos.y_rot = old_angle;
    lara.move_angle = old_angle;
    if (!lara.climb_status)
    {
        if (abs(old_front_floor - coll->front_floor) <= SLOPE_DIF)
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

    if (GetClimbTrigger(x, item->pos.y, z, item->room_number) & LeftClimbTab[angle])
        return result;

    short front = LaraFloorFront(item, item->pos.y_rot, LARA_RAD + 16);
    if (abs(coll->front_floor - front) > 0)
        return 0;

    if (front < -768)
        return 0;

    return result;
}

int LaraClimbLeftCornerTest(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->current_anim != ANIMATION_LARA_LADDER_LEFT)
        return 0;

    int x, z;
    int old_x, old_z;
    int result = 0;
    int shift;
    SHORT old_angle;
    SHORT angle;

    old_x = item->pos.x;
    old_z = item->pos.z;
    old_angle = item->pos.y_rot;
    angle = (USHORT)(old_angle + ANGLE(45)) / ANGLE(90);
    
    if (angle && angle != 2)
    {
        x = item->pos.x ^ (item->pos.x ^ item->pos.z) & (WALL_L - 1);
        z = item->pos.z ^ (item->pos.x ^ item->pos.z) & (WALL_L - 1);
    }
    else
    {
        x = (item->pos.x & ~(WALL_L - 1)) - (item->pos.z & (WALL_L - 1)) + SECTOR(1);
        z = (item->pos.z & ~(WALL_L - 1)) - (item->pos.x & (WALL_L - 1)) + SECTOR(1);
    }

    if (GetClimbTrigger(x, item->pos.y, z, item->room_number) & LeftIntRightExtTab[angle])
    {
        item->pos.x = x;
        lara.corner_x = x;
        item->pos.z = z;
        lara.corner_z = z;
        item->pos.y_rot -= 0x4000;
        lara.move_angle = item->pos.y_rot;
        result = LaraTestClimbPos(item, coll->radius, -(coll->radius + CLIMB_WIDTHL), -CLIMB_HITE, CLIMB_HITE, &shift);
        item->reserved_4 = result;
    }
    
    if (!result)
    {
        item->pos.x = old_x;
        item->pos.z = old_z;
        item->pos.y_rot = old_angle;
        lara.move_angle = old_angle;

        switch (angle)
        {
            case NORTH:
                x = (item->pos.x ^ ((item->pos.z ^ item->pos.x) & (WALL_L - 1))) - SECTOR(1);
                z = ((item->pos.z ^ item->pos.x) & (WALL_L - 1)) ^ (item->pos.z + SECTOR(1));
                break;

            case SOUTH:
                x = ((item->pos.z ^ item->pos.x) & (WALL_L - 1)) ^ (item->pos.x + SECTOR(1));
                z = ((item->pos.z ^ item->pos.x) & (WALL_L - 1)) ^ (item->pos.z - SECTOR(1));
                break;

            case EAST:
                x = ((item->pos.x + SECTOR(1)) & ~(WALL_L - 1)) - (item->pos.z & (WALL_L - 1)) + SECTOR(1);
                z = ((item->pos.z + SECTOR(1)) & ~(WALL_L - 1)) - (item->pos.x & (WALL_L - 1)) + SECTOR(1);
                break;

            case WEST:
            default:
                x = (item->pos.x & ~(WALL_L - 1)) - (item->pos.z & (WALL_L - 1));
                z = (item->pos.z & ~(WALL_L - 1)) - (item->pos.x & (WALL_L - 1));
                break;
        }

        if (GetClimbTrigger(x, item->pos.y, z, item->room_number) & LeftExtRightIntTab[angle])
        {
            item->pos.x = x;
            lara.corner_x = x;
            item->pos.z = z;
            lara.corner_z = z;
            item->pos.y_rot += 0x4000;
            lara.move_angle = item->pos.y_rot;
            item->reserved_4 = LaraTestClimbPos(item, coll->radius, -(coll->radius + CLIMB_WIDTHL), -CLIMB_HITE, CLIMB_HITE, &shift);
            result = item->reserved_4 != 0;
        }
    }
    else
    {
        result = -1;
    }

    item->pos.x = old_x;
    item->pos.z = old_z;
    item->pos.y_rot = old_angle;
    lara.move_angle = old_angle;
    return result;
}

int LaraClimbRightCornerTest(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->current_anim != ANIMATION_LARA_LADDER_RIGHT)
        return 0;

    int x, z;
    int old_x, old_z;
    int result = 0;
    int shift;
    SHORT old_angle;
    SHORT angle;

    old_x = item->pos.x;
    old_z = item->pos.z;
    old_angle = item->pos.y_rot;
    angle = (USHORT)(old_angle + ANGLE(45)) / ANGLE(90);

    if (angle && angle != 2)
    {
        x = (item->pos.x & ~(WALL_L - 1)) - (item->pos.z & (WALL_L - 1)) + SECTOR(1);
        z = (item->pos.z & ~(WALL_L - 1)) - (item->pos.x & (WALL_L - 1)) + SECTOR(1);
    }
    else
    {
        x = item->pos.x ^ (item->pos.x ^ item->pos.z) & (WALL_L - 1);
        z = item->pos.z ^ (item->pos.x ^ item->pos.z) & (WALL_L - 1);
    }

    if (GetClimbTrigger(x, item->pos.y, z, item->room_number) & LeftExtRightIntTab[angle])
    {
        item->pos.x = x;
        lara.corner_x = x;
        item->pos.z = z;
        lara.corner_z = z;
        item->pos.y_rot += 0x4000;
        lara.move_angle = item->pos.y_rot;
        result = LaraTestClimbPos(item, coll->radius, coll->radius + CLIMB_WIDTHR, -CLIMB_HITE, CLIMB_HITE, &shift);
    }

    if (!result)
    {
        item->pos.x = old_x;
        item->pos.z = old_z;
        item->pos.y_rot = old_angle;
        lara.move_angle = old_angle;

        switch (angle)
        {
            case NORTH:
                x = ((item->pos.x + SECTOR(1)) & ~(WALL_L - 1)) - (item->pos.z % SECTOR(1)) + SECTOR(1);
                z = ((item->pos.z + SECTOR(1)) & ~(WALL_L - 1)) - (item->pos.x % SECTOR(1)) + SECTOR(1);
                break;

            case SOUTH:
                x = ((item->pos.x - SECTOR(1)) & ~(WALL_L - 1)) - (item->pos.z % SECTOR(1)) + SECTOR(1);
                z = ((item->pos.z - SECTOR(1)) & ~(WALL_L - 1)) - (item->pos.x % SECTOR(1)) + SECTOR(1);
                break;

            case EAST:
                x = ((item->pos.z ^ item->pos.x) % SECTOR(1)) ^ (item->pos.x + SECTOR(1));
                z = (item->pos.z ^ ((item->pos.z ^ item->pos.x) % SECTOR(1))) - SECTOR(1);
                break;

            case WEST:
            default:
                x = (item->pos.x ^ (item->pos.z ^ item->pos.x) % SECTOR(1)) - SECTOR(1);
                z = ((item->pos.z ^ item->pos.x) % SECTOR(1)) ^ (item->pos.z + SECTOR(1));
                break;
        }

        if (GetClimbTrigger(x, item->pos.y, z, item->room_number) & LeftIntRightExtTab[angle])
        {
            item->pos.x = x;
            lara.corner_x = x;
            item->pos.z = z;
            lara.corner_z = z;
            item->pos.y_rot -= 0x4000;
            lara.move_angle = item->pos.y_rot;
            result = LaraTestClimbPos(item, coll->radius, coll->radius + CLIMB_WIDTHR, -CLIMB_HITE, CLIMB_HITE, &shift);
        }
    }
    else
    {
        result = -1;
    }

    item->pos.x = old_x;
    item->pos.z = old_z;
    item->pos.y_rot = old_angle;
    lara.move_angle = old_angle;
    return result;
}

void LaraSlideEdgeJump(ITEM_INFO* item, COLL_INFO* coll)
{
    ShiftItem(item, coll);

    switch (coll->coll_type)
    {
        case COLL_LEFT:
            item->pos.y_rot += ANGLE(5);
            break;
        case COLL_RIGHT:
            item->pos.y_rot -= ANGLE(5);
            break;
        case COLL_TOP:
        case COLL_TOPFRONT:
            if (item->fallspeed <= 0)
                item->fallspeed = 1;
            break;
        case COLL_CLAMP:
            item->pos.x -= LARA_RAD * SIN(coll->facing) >> W2V_SHIFT;
            item->pos.z -= LARA_RAD * COS(coll->facing) >> W2V_SHIFT;
            item->speed = 0;
            coll->mid_floor = 0;
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

        item->pos.y += shift_r;
    }
    else if (shift_l)
    {
        item->pos.y += shift_l;
    }

    return TRUE;
}

int LaraTestEdgeCatch(ITEM_INFO* item, COLL_INFO* coll, int* edge)
{
    short* bounds = GetBoundsAccurate(item);
    int hdif = coll->front_floor - bounds[2];

    if (hdif < 0 && (hdif + item->fallspeed) < 0
    ||  hdif > 0 && (hdif + item->fallspeed) > 0)
    {
        hdif = item->pos.y + bounds[2];

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

    if (abs(coll->left_floor2 - coll->right_floor2) >= SLOPE_DIF)
        return 0;

    return 1;
}

BOOL LaraTestHangOnClimbWall(ITEM_INFO* item, COLL_INFO* coll)
{
    if (!lara.climb_status || item->fallspeed < 0)
        return FALSE;

    SHORT angle = (USHORT)(item->pos.y_rot + ANGLE(45)) / ANGLE(90);
    switch (angle)
    {
        case NORTH:
        case SOUTH:
            item->pos.z += coll->shift.z;
            break;
        case EAST:
        case WEST:
            item->pos.x += coll->shift.x;
            break;
    }

    if (lara.move_angle != item->pos.y_rot)
    {
        int ceiling_item = LaraCeilingFront(item, item->pos.y_rot, 0, 0);
        int ceiling_lara = LaraCeilingFront(item, lara.move_angle, (STEP_L / 2), 0);
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

    item->pos.y += shift;
    return TRUE;
}

void LaraSlideSlope(ITEM_INFO* item, COLL_INFO* coll)
{
    coll->bad_pos = NO_HEIGHT;
    coll->bad_neg = -(STEP_L * 2);
    coll->bad_ceiling = 0;
    GetLaraCollisionInfo(item, coll);

    if (!LaraHitCeiling(item, coll))
    {
        LaraDeflectEdge(item, coll);
        if (coll->mid_floor <= 200)
        {
            TestLaraSlide(item, coll);
            item->pos.y += coll->mid_floor;

            char tiltX = coll->tilt_x;
            char tiltZ = coll->tilt_z;
            if ((tiltX & 128) != 0)
                tiltX = -tiltX;
            if ((tiltZ & 128) != 0)
                tiltZ = -tiltZ;

            if (tiltX <= 2 && tiltZ <= 2)
            {
                item->state_next = STATE_LARA_IDLE;
                item->pos.x_rot = 0;
                StopSoundEffect(SFX_LARA_SLIPPING);
            }
        }
        else
        {
            if (item->state_current == STATE_LARA_SLIDE_FORWARD)
                SetAnimationForItemAS(item, ANIMATION_LARA_FREE_FALL_FORWARD, STATE_LARA_JUMP_FORWARD);
            else
                SetAnimationForItemAS(item, ANIMATION_LARA_FREE_FALL_BACK, STATE_LARA_FALL_BACKWARD);
            item->fallspeed = 0;
            item->gravity_status = TRUE;
            StopSoundEffect(SFX_LARA_SLIPPING);
        }
    }
}

void LaraCollideJump(ITEM_INFO* item, COLL_INFO* coll)
{
    coll->bad_pos = NO_HEIGHT;
    coll->bad_neg = -STEPUP_HEIGHT;
    coll->bad_ceiling = BAD_JUMP_CEILING;
    GetLaraCollisionInfo(item, coll);
    LaraDeflectEdgeJump(item, coll);
    if (item->fallspeed > 0 && coll->mid_floor <= 0)
    {
        if (LaraLandedBad(item))
            item->state_next = STATE_LARA_DEATH;
        else
            item->state_next = STATE_LARA_IDLE;
        item->fallspeed = 0;
        item->gravity_status = FALSE;
        if (coll->mid_floor != -NO_HEIGHT)
            item->pos.y += coll->mid_floor;
    }
}

BOOL LaraCheckForLetGo(ITEM_INFO* item, COLL_INFO* coll)
{
    FLOOR_INFO* floor;
    short roomNumber;

    item->fallspeed = 0;
    item->gravity_status = FALSE;

    roomNumber = item->room_number;
    floor = GetFloor(item->pos.x, item->pos.y, item->pos.z, &roomNumber);
    GetHeight(floor, item->pos.x, item->pos.y, item->pos.z);
    coll->trigger = trigger_index;

    if (CHK_NOP(TrInput, IN_ACTION) || item->hit_points <= 0)
    {
        lara.torso_y_rot = 0;
        lara.torso_x_rot = 0;
        lara.head_y_rot = 0;
        lara.head_x_rot = 0;
        lara.gun_status = LHS_ARMLESS;
        item->speed = 2;
        item->fallspeed = 1;
        item->gravity_status = TRUE;
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

    y = (item->pos.y - CLIMB_HITE) - STEP_L;
    angle = (USHORT)(item->pos.y_rot + ANGLE(45)) / ANGLE(90);
    switch (angle)
    {
        case NORTH:
            z = item->pos.z + front;
            x = item->pos.x + right;
            zfront = 4;
            break;
        case EAST:
            x = item->pos.x + front;
            z = item->pos.z - right;
            xfront = 4;
            break;
        case SOUTH:
            z = item->pos.z - front;
            x = item->pos.x - right;
            zfront = -4;
            break;
        default:
        case WEST:
            x = item->pos.x - front;
            z = item->pos.z + right;
            xfront = -4;
            break;
    }

    *shift = 0;
    roomNumber = item->room_number;
    floor = GetFloor(x, y, z, &roomNumber);
    ceiling = GetCeiling(floor, x, y, z) - (y - STEP_L);

    if (ceiling > CLIMB_SHIFT)
        return 0;
    else if (ceiling > 0)
        *shift = ceiling;

    floor = GetFloor(x + xfront, y, z + zfront, &roomNumber);
    height = GetHeight(floor, x + xfront, y, z + zfront);
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

    roomNumber = item->room_number;
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

    angle = (USHORT)(item->pos.y_rot + ANGLE(45)) / ANGLE(90);
    switch (angle)
    {
        case NORTH:
            z = item->pos.z + front;
            x = item->pos.x + right;
            zfront = 4;
            break;
        case EAST:
            x = item->pos.x + front;
            z = item->pos.z - right;
            xfront = 4;
            break;
        case SOUTH:
            z = item->pos.z - front;
            x = item->pos.x - right;
            zfront = -4;
            break;
        default:
        case WEST:
            x = item->pos.x - front;
            z = item->pos.z + right;
            xfront = -4;
            break;
    }

    return LaraTestClimb(x, item->pos.y + origin, z, xfront, zfront, height, item->room_number, shift);
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
    if (!lara.climb_status)
        return 0;
    roomNumber = item_room;
    floor = GetFloor(x, y - (STEP_L / 2), z, &roomNumber);
    height = GetHeight(floor, x, y, z);
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
    height = GetHeight(floor, xfront + x, y, zfront + z);
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
        if (CHK_ANY(TrInput, IN_LEFT))
            item->state_next = STATE_LARA_LADDER_LEFT;
        else if (CHK_ANY(TrInput, IN_RIGHT))
            item->state_next = STATE_LARA_LADDER_RIGHT;
        else
            item->state_next = STATE_LARA_LADDER_IDLE;

        item->pos.y += shift;
        return;
    }

    if (result != 0)
    {
        item->state_next = STATE_LARA_HANG;
        do
        {
            AnimateItem(item);
        } while (item->state_current != STATE_LARA_HANG);
        item->pos.x = coll->old.x;
        item->pos.z = coll->old.z;
        return;
    }

    item->pos.x = coll->old.x;
    item->pos.z = coll->old.z;
    item->state_next = STATE_LARA_LADDER_IDLE;
    item->state_current = STATE_LARA_LADDER_IDLE;

    if (coll->old_anim_state != STATE_LARA_LADDER_IDLE)
    {
        item->current_anim = ANIMATION_LARA_LADDER_IDLE;
        item->current_frame = anims[item->current_anim].frame_base;
        return;
    }

    if (CHK_ANY(TrInput, IN_LEFT))
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
    else if (CHK_ANY(TrInput, IN_RIGHT))
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

    item->current_frame = coll->old_frame_number;
    item->current_anim = coll->old_anim_number;
    AnimateLara(item);
}

BOOL LaraTestWaterClimbOut(ITEM_INFO* item, COLL_INFO* coll)
{
    int hdif;
    short angle;

    if (coll->coll_type != COLL_FRONT || CHK_NOP(TrInput, IN_ACTION) || abs(coll->left_floor2 - coll->right_floor2) >= SLOPE_DIF)
        return FALSE;

    if (lara.gun_status != LHS_ARMLESS && !(lara.gun_status == LHS_READY && lara.gun_type == LG_FLARE))
        return FALSE;

    if (coll->front_ceiling > 0 || coll->mid_ceiling > -STEPUP_HEIGHT)
        return FALSE;

    hdif = coll->front_floor + SURF_HITE;
    if (hdif <= -(STEP_L * 2) || hdif > SURF_HITE - STEPUP_HEIGHT)
        return FALSE;

    angle = GetCatchAngle(item, HANG_ANGLE);
    if (angle & 0x3FFF)
        return FALSE;

    item->pos.y += (hdif - 5);
    UpdateLaraRoom(item, -(LARA_HITE / 2));

    switch (angle)
    {
        case 0:
            item->pos.z = (item->pos.z & ~(WALL_L - 1)) + WALL_L + LARA_RAD;
            break;
        case 0x4000:
            item->pos.x = (item->pos.x & ~(WALL_L - 1)) + WALL_L + LARA_RAD;
            break;
        case -0x8000:
            item->pos.z = (item->pos.z & ~(WALL_L - 1)) - LARA_RAD;
            break;
        case -0x4000:
            item->pos.x = (item->pos.x & ~(WALL_L - 1)) - LARA_RAD;
            break;
    }

    if (hdif < -128)
    {
        item->current_anim = ANIMATION_LARA_CLIMB_OUT_OF_WATER;
        item->current_frame = anims[ANIMATION_LARA_CLIMB_OUT_OF_WATER].frame_base;
    }
    else if (hdif < 128)
    {
        item->current_anim = ANIMATION_LARA_ONWATER_TO_LAND_LOW;
        item->current_frame = anims[ANIMATION_LARA_ONWATER_TO_LAND_LOW].frame_base;
    }
    else
    {
        item->current_anim = ANIMATION_LARA_ONWATER_TO_WADE;
        item->current_frame = anims[ANIMATION_LARA_ONWATER_TO_WADE].frame_base;
    }

    lara.gun_status = LHS_HANDBUSY;
    lara.water_status = LWS_ABOVEWATER;
    item->state_current = STATE_LARA_ONWATER_EXIT;
    item->state_next = STATE_LARA_IDLE;
    item->pos.y_rot = angle;
    item->pos.x_rot = 0;
    item->pos.z_rot = 0;
    item->speed = 0;
    item->fallspeed = 0;
    item->gravity_status = FALSE;
    return TRUE;
}

BOOL LaraTestWaterStepOut(ITEM_INFO* item, COLL_INFO* coll)
{
    if (coll->coll_type == COLL_FRONT || coll->mid_type == BIG_SLOPE || coll->mid_type == DIAGONAL || coll->mid_floor >= 0)
        return FALSE;

    if (coll->mid_floor < -(STEP_L / 2))
    {
        SetAnimationForItemASS(item, ANIMATION_LARA_ONWATER_TO_WADE_DEEP, STATE_LARA_ONWATER_EXIT, STATE_LARA_IDLE);
    }
    else
    {
        if (item->state_next == STATE_LARA_ONWATER_LEFT)
            item->state_next = STATE_LARA_WALK_LEFT;
        else if (item->state_next == STATE_LARA_ONWATER_RIGHT)
            item->state_next = STATE_LARA_WALK_RIGHT;
        else
            SetAnimationForItemAS(item, ANIMATION_LARA_WADE, STATE_LARA_WADE_FORWARD);
    }

    lara.water_status = LWS_WADE;
    item->pos.y += coll->front_floor + (SURF_HITE - 5);
    item->pos.x_rot = 0;
    item->pos.z_rot = 0;
    item->speed = 0;
    item->fallspeed = 0;
    item->gravity_status = FALSE;
    UpdateLaraRoom(item, -(LARA_HITE / 2));
    return TRUE;
}

void LaraTestWaterDepth(ITEM_INFO* item, COLL_INFO* coll)
{
    FLOOR_INFO* floor;
    int water_depth;
    short roomNumber;

    roomNumber = item->room_number;
    floor = GetFloor(item->pos.x, item->pos.y, item->pos.z, &roomNumber);
    water_depth = GetWaterDepth(item->pos.x, item->pos.y, item->pos.z, roomNumber);

    if (water_depth == -NO_HEIGHT)
    {
        item->pos.x = coll->old.x;
        item->pos.y = coll->old.y;
        item->pos.z = coll->old.z;
        item->fallspeed = 0;
    }
    else if (water_depth <= (STEP_L * 2))
    {
        lara.water_status = LWS_WADE;
        item->pos.y = GetHeight(floor, item->pos.x, item->pos.y, item->pos.z);
        item->pos.x_rot = 0;
        item->pos.z_rot = 0;
        item->speed = 0;
        item->fallspeed = 0;
        item->gravity_status = FALSE;
        SetAnimationForItemASS(item, ANIMATION_LARA_UNDERWATER_TO_WADE, STATE_LARA_ONWATER_EXIT, STATE_LARA_IDLE);
    }
}

bool LaraLandedBad(ITEM_INFO* item)
{
    int landspeed = item->fallspeed - DAMAGE_START;

    if (landspeed <= 0)
        return false;

    if (landspeed <= DAMAGE_LENGTH)
        item->hit_points -= (landspeed * landspeed * LARA_HITPOINTS) / (DAMAGE_LENGTH * DAMAGE_LENGTH);
    else
        item->hit_points = -1;

    return item->hit_points <= 0;
}

void LaraCollideStop(ITEM_INFO* item, COLL_INFO* coll)
{
    switch (coll->old_anim_state)
    {
        case STATE_LARA_IDLE:
        case STATE_LARA_TURN_RIGHT_SLOW:
        case STATE_LARA_TURN_LEFT_SLOW:
        case STATE_LARA_TURN_FAST:
            item->state_current = coll->old_anim_state;
            item->current_anim = coll->old_anim_number;
            item->current_frame = coll->old_frame_number;

            if (CHK_ANY(TrInput, IN_LEFT))
                item->state_next = STATE_LARA_TURN_LEFT_SLOW;
            else if (CHK_ANY(TrInput, IN_RIGHT))
                item->state_next = STATE_LARA_TURN_RIGHT_SLOW;
            else
                item->state_next = STATE_LARA_IDLE;

            AnimateLara(item);
            break;
        default:
            item->current_anim = ANIMATION_LARA_STAY_SOLID;
            item->current_frame = anims[item->current_anim].frame_base;
            break;
    }
}

void LaraSwimCollision(ITEM_INFO* item, COLL_INFO* coll)
{
    if (item->pos.x_rot >= -0x4000 && item->pos.x_rot <= 0x4000)
        coll->facing = lara.move_angle = item->pos.y_rot;
    else
        coll->facing = lara.move_angle = item->pos.y_rot - 0x8000;

    int height = abs(LARA_HITE * SIN(item->pos.x_rot) >> W2V_SHIFT);
    if (height < 200)
        height = 200;

    coll->bad_neg = -height; // fixed ! the old value is -64 (same as TR3)
    GetCollisionInfo(coll, item->pos.x, item->pos.y + height / 2, item->pos.z, item->room_number, height);
    ShiftItem(item, coll);

    switch (coll->coll_type)
    {
        case COLL_FRONT:
            if (item->pos.x_rot > ANGLE(45))
                item->pos.x_rot += UW_WALLDEFLECT;
            else if (item->pos.x_rot < -ANGLE(45))
                item->pos.x_rot -= UW_WALLDEFLECT;
            else
                item->fallspeed = 0;
            break;
        case COLL_TOP:
            if (item->pos.x_rot >= -ANGLE(45))
                item->pos.x_rot -= UW_WALLDEFLECT;
            break;
        case COLL_TOPFRONT:
            item->fallspeed = 0;
            break;
        case COLL_LEFT:
            item->pos.y_rot += ANGLE(5);
            break;
        case COLL_RIGHT:
            item->pos.y_rot -= ANGLE(5);
            break;
        case COLL_CLAMP:
            item->pos.x = coll->old.x;
            item->pos.y = coll->old.y;
            item->pos.z = coll->old.z;
            item->fallspeed = 0;
            return;
    }

    if (coll->mid_floor < 0 && coll->mid_floor != -NO_HEIGHT)
    {
        item->pos.x_rot += UW_WALLDEFLECT;
        item->pos.y     += coll->mid_floor;
    }

    if (lara.water_status != LWS_CHEAT)
        LaraTestWaterDepth(item, coll);
}

void LaraSurfaceCollision(ITEM_INFO* item, COLL_INFO* coll)
{
    coll->facing = lara.move_angle;
    GetCollisionInfo(coll, item->pos.x, item->pos.y + SURF_HITE, item->pos.z, item->room_number, (SURF_HITE + LARA_RAD));
    ShiftItem(item, coll);

    if ((coll->coll_type & (COLL_FRONT | COLL_TOP | COLL_CLAMP | COLL_TOPFRONT)) || coll->mid_floor < 0 && (coll->mid_type == BIG_SLOPE || coll->mid_type == DIAGONAL))
    {
        item->pos.x = coll->old.x;
        item->pos.y = coll->old.y;
        item->pos.z = coll->old.z;
        item->fallspeed = 0;
    }
    else if (coll->coll_type == COLL_LEFT)
    {
        item->pos.y_rot += ANGLE(5);
    }
    else if (coll->coll_type == COLL_RIGHT)
    {
        item->pos.y_rot -= ANGLE(5);
    }

    int water_level = GetWaterHeight(item->pos.x, item->pos.y, item->pos.z, item->room_number) - item->pos.y;
    if (water_level <= -100)
    {
        SetAnimationForItemASS(item, ANIMATION_LARA_FREE_FALL_TO_UNDERWATER_ALTERNATE, STATE_LARA_UNDERWATER_DIVING, STATE_LARA_UNDERWATER_FORWARD);
        item->pos.x_rot = -ANGLE(45);
        item->fallspeed = 80;
        lara.water_status = LWS_UNDERWATER;
        return;
    }

    LaraTestWaterStepOut(item, coll);
}

// TODO: need decompiling !!!
/*
BOOL UseSpecialItem(ITEM_INFO* item)
{
    return 0;
}
*/

BOOL TestLaraSlide(ITEM_INFO* item, COLL_INFO* coll)
{
    static short old_angle = 1;
    short adif, angle;
    char a, b;

    a = ABS(coll->tilt_x);
    b = ABS(coll->tilt_z);
    if (a <= 2 && b <= 2)
        return FALSE;

    angle = 0;
    if (coll->tilt_x > 2)
        angle = -0x4000;
    else if (coll->tilt_x < -2)
        angle = 0x4000;

    if (coll->tilt_z > 2 && coll->tilt_z > a)
        angle = -0x8000;
    else if (coll->tilt_z < -2 && -coll->tilt_z < a)
        angle = 0;

    adif = angle - item->pos.y_rot;
    ShiftItem(item, coll);

    if (adif >= -0x4000 && adif <= 0x4000)
    {
        if (item->state_current != STATE_LARA_SLIDE_FORWARD || old_angle != angle)
        {
            SetAnimationForItemAS(item, ANIMATION_LARA_SLIDE_FORWARD, STATE_LARA_SLIDE_FORWARD);
            item->pos.y_rot = angle;
            lara.move_angle = angle;
            old_angle = angle;
        }
    }
    else
    {
        if (item->state_current != STATE_LARA_SLIDE_BACK || old_angle != angle)
        {
            SetAnimationForItemAS(item, ANIMATION_LARA_SLIDE_BACKWARD, STATE_LARA_SLIDE_BACK);
            item->pos.y_rot = angle - 0x8000;
            lara.move_angle = angle;
            old_angle = angle;
        }
    }

    // TODO: add LaraSlideAngle() here !
    //LaraSlideAngle(item, coll, adif, angle);
    return TRUE;
}

BOOL TestLaraVault(ITEM_INFO* item, COLL_INFO* coll)
{
    if (CHK_NOP(TrInput, IN_ACTION) || lara.gun_status != LHS_ARMLESS)
        return FALSE;

    if (coll->coll_type == COLL_FRONT)
    {
        int hdif, slope;
        short angle = GetCatchAngle(item, VAULT_ANGLE);
        if (angle & 0x3FFF)
            return FALSE;

        hdif = coll->front_floor;
        slope = abs(coll->left_floor2 - coll->right_floor2) >= SLOPE_DIF;

        if (VAULT_CLICK(hdif, 2, >=, -) && VAULT_CLICK(hdif, 2, <= , +))
        {
            if (slope
            || (coll->front_floor - coll->front_ceiling) < 0
            || (coll->left_floor2 - coll->left_ceiling2) < 0
            || (coll->right_floor2 - coll->right_ceiling2) < 0)
            {
                return FALSE;
            }

            SetAnimationForItemASS(item, ANIMATION_LARA_CLIMB_2CLICK, STATE_LARA_GRABBING, STATE_LARA_IDLE);
            item->pos.y += hdif + (STEP_L * 2);
            lara.gun_status = LHS_HANDBUSY;
        }
        else if (VAULT_CLICK(hdif, 3, >=, -) && VAULT_CLICK(hdif, 3, <=, +))
        {
            if (slope
            || (coll->front_floor - coll->front_ceiling) < 0
            || (coll->left_floor2 - coll->left_ceiling2) < 0
            || (coll->right_floor2 - coll->right_ceiling2) < 0)
            {
                return FALSE;
            }

            //if (lara.water_surface_dist < -(STEP_L * 3))
            //    return FALSE;

            SetAnimationForItemASS(item, ANIMATION_LARA_CLIMB_3CLICK, STATE_LARA_GRABBING, STATE_LARA_IDLE);
            item->pos.y += hdif + (STEP_L * 3);
            lara.gun_status = LHS_HANDBUSY;
        }
        else if (!slope && VAULT_CLICK(hdif, 7, >=, -) && VAULT_CLICK(hdif, 4, <=, +))
        {
            SetAnimationForItemASS(item, ANIMATION_LARA_STAY_SOLID, STATE_LARA_IDLE, STATE_LARA_JUMP_UP);
            lara.calc_fallspeed = -3 - phd_sqrt(-9600 - 12 * hdif);
            AnimateLara(item);
        }
        else if (lara.climb_status
        && VAULT_CLICK(hdif, 8, <=, +)
        && lara.water_status != LWS_WADE
        && VAULT_CLICK(coll->left_floor2, 8, <=, +)
        && coll->right_floor2 <= -(STEP_L * 8)
        && VAULT_CLICK(coll->mid_ceiling, 8, <=, +))
        {
            SetAnimationForItemASS(item, ANIMATION_LARA_STAY_SOLID, STATE_LARA_IDLE, STATE_LARA_JUMP_UP);
            lara.calc_fallspeed = -116;
            AnimateLara(item);
        }
        else if (lara.climb_status
        && (hdif < -(STEP_L * 4) || coll->front_ceiling >= (LARA_HITE - STEP_L))
        && coll->mid_ceiling <= (-(STEP_L * 5) + LARA_HITE))
        {
            ShiftItem(item, coll);

            if (LaraTestClimbStance(item, coll))
            {
                SetAnimationForItemASS(item, ANIMATION_LARA_STAY_SOLID, STATE_LARA_IDLE, STATE_LARA_LADDER_IDLE);
                item->pos.y_rot = angle;
                lara.gun_status = LHS_HANDBUSY;
                AnimateLara(item);
                return TRUE;
            }
            return FALSE;
        }

        item->pos.y_rot = angle;
        ShiftItem(item, coll);

        SHORT resultAngle = (USHORT)(item->pos.y_rot + ANGLE(45)) / ANGLE(90);
        switch (resultAngle)
        {
            case NORTH:
                item->pos.z = (item->pos.z |  (WALL_L - 1)) - LARA_RAD;
                break;
            case EAST:
                item->pos.x = (item->pos.x |  (WALL_L - 1)) - LARA_RAD;
                break;
            case SOUTH:
                item->pos.z = (item->pos.z & ~(WALL_L - 1)) + LARA_RAD;
                break;
            case WEST:
                item->pos.x = (item->pos.x & ~(WALL_L - 1)) + LARA_RAD;
                break;
        }

        return TRUE;
    }

    return FALSE;
}

BOOL TestMonkeyLeft(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.y_rot - 0x4000;
    coll->bad_pos = NO_HEIGHT;
    coll->bad_neg = -NO_HEIGHT;
    coll->bad_ceiling = 0;
    coll->facing = lara.move_angle;
    coll->radius = LARA_RAD;
    coll->slopes_are_walls = FALSE;
    GetCollisionInfo(coll, item->pos.x, item->pos.y, item->pos.z, item->room_number, LARA_HANG_HEIGHT);

    if (abs(coll->mid_ceiling - coll->front_ceiling) > 50)
        return FALSE;

    if (coll->coll_type)
    {
        short octant = GetDirOctant(item->pos.y_rot);
        if (((octant == 0) && (coll->coll_type == COLL_FRONT))
		||  ((octant == 0) && (coll->coll_type == COLL_LEFT))
		||  ((octant == 1) && (coll->coll_type == COLL_RIGHT))
		||  ((octant == 1) && (coll->coll_type == COLL_LEFT)))
			return FALSE;
    }

    return TRUE;
}

BOOL TestMonkeyRight(ITEM_INFO* item, COLL_INFO* coll)
{
    lara.move_angle = item->pos.y_rot + 0x4000;
    coll->bad_pos = NO_HEIGHT;
    coll->bad_neg = -STEPUP_HEIGHT;
    coll->bad_ceiling = 0;
    coll->facing = lara.move_angle;
    coll->radius = LARA_RAD;
    coll->slopes_are_walls = FALSE;
    GetCollisionInfo(coll, item->pos.x, item->pos.y, item->pos.z, item->room_number, LARA_HANG_HEIGHT);

    if (abs(coll->mid_ceiling - coll->front_ceiling) > 50)
        return FALSE;

    if (coll->coll_type)
    {
        short octant = GetDirOctant(item->pos.y_rot);
        if (((octant == 0) && (coll->coll_type == COLL_FRONT))
		||  ((octant == 1) && (coll->coll_type == COLL_FRONT))
		||  ((octant == 1) && (coll->coll_type == COLL_RIGHT))
		||  ((octant == 1) && (coll->coll_type == COLL_LEFT)))
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

    x = item->pos.x;
    y = item->pos.y;
    z = item->pos.z;
    roomNumber = item->room_number;

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
    height = GetHeight(floor, x, y, z);
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

    angle = (USHORT)(item->pos.y_rot + ANGLE(45)) / ANGLE(90);
    x = item->pos.x;
    y = height + item->pos.y;
    z = item->pos.z;
    roomNumber = item->room_number;

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
    right = GetHeight(floor, x, y, z);
    ceiling = GetCeiling(floor, x, y, z);
    if (right == -NO_HEIGHT)
        return 1;
    if (right - y <= 0 || ceiling - y >= 0)
        return 2;
    return 0;
}

void SwimTurn(ITEM_INFO* item)
{
    if (CHK_ANY(TrInput, IN_FORWARD))
        item->pos.x_rot -= 2 * ONE_DEGREE;
    else if (CHK_ANY(TrInput, IN_BACK))
        item->pos.x_rot += 2 * ONE_DEGREE;

    if (CHK_ANY(TrInput, IN_LEFT))
    {
        lara.turn_rate -= LARA_TURN_RATE;
        if (lara.turn_rate < -LARA_MED_TURN)
            lara.turn_rate = -LARA_MED_TURN;
        item->pos.z_rot -= (LARA_LEAN_RATE * 2);
    }
    else if (CHK_ANY(TrInput, IN_RIGHT))
    {
        lara.turn_rate += LARA_TURN_RATE;
        if (lara.turn_rate > LARA_MED_TURN)
            lara.turn_rate = LARA_MED_TURN;
        item->pos.z_rot += (LARA_LEAN_RATE * 2);
    }
}

void MonkeySwingFall(ITEM_INFO* item)
{
    SetAnimationForItemASF(item, ANIMATION_LARA_TRY_HANG_VERTICAL, STATE_LARA_JUMP_UP, 9);
    //item->pos.y += STEP_L; // deleted this line, it's not necessary since gravity_status is TRUE !
    item->speed = 2;
    item->fallspeed = 1;
    item->gravity_status = TRUE;
    lara.gun_status = LHS_ARMLESS;
}

void MonkeySwingSnap(ITEM_INFO* item, COLL_INFO* coll)
{
    FLOOR_INFO* floor;
    short* bounds;
    int ceiling;
    short roomNumber;

    bounds = GetBoundsAccurate(item);
    roomNumber = item->room_number;
    floor = GetFloor(item->pos.x, item->pos.y, item->pos.z, &roomNumber);
    ceiling = GetCeiling(floor, item->pos.x, item->pos.y, item->pos.z);
    item->pos.y = ceiling - bounds[2]; // based on collision now !
}

void FallFromRope(ITEM_INFO* item)
{
    SetAnimationForItemAS(item, ANIMATION_LARA_FREE_FALL_FORWARD, STATE_LARA_JUMP_FORWARD);
    item->speed = (abs(current_pendulum.vel.x >> 16) + abs(current_pendulum.vel.z >> 16)) >> 1;
    item->pos.x_rot = 0;
    item->pos.y += 320;
    item->fallspeed = 0;
    item->gravity_status = TRUE;
    lara.gun_status = LHS_ARMLESS;
    lara.rope_ptr = NO_ROPE;
}

void UpdateRopeSwing(ITEM_INFO* item)
{
    bool rope_swing = false;

    if (lara.rope_maxX_forward > 9000)
        lara.rope_maxX_forward = 9000;
    if (lara.rope_maxX_backward > 9000)
        lara.rope_maxX_backward = 9000;

    if (lara.rope_direction)
    {
        if (item->pos.x_rot > 0 && item->pos.x_rot - lara.rope_last_x < -100)
        {
            lara.rope_arc_front = lara.rope_last_x;
            lara.rope_direction = 0;
            lara.rope_maxX_backward = 0;
            short frame = 15 * lara.rope_maxX_forward / 18000 + anims[ANIMATION_LARA_ROPE_SWING_FORWARD_SEMIHARD].frame_base + 47 << 8;
            if (frame > lara.rope_dframe)
            {
                lara.rope_dframe = frame;
                rope_swing = true;
            }
            else
            {
                rope_swing = false;
            }

            SoundEffect(SFX_LARA_ROPE_CREAK, &item->pos, 0);
        }
        else if (lara.rope_last_x < 0 && lara.rope_frame == lara.rope_dframe)
        {
            lara.rope_dframe = 15 * lara.rope_maxX_backward / 18000 + anims[ANIMATION_LARA_ROPE_SWING_FORWARD_SEMIHARD].frame_base + 47 << 8;
            lara.rope_framerate = 15 * lara.rope_maxX_backward / 9000 + 1;
            rope_swing = false;
        }
        else if (lara.rope_framerate < 512)
        {
            int framerate = rope_swing ? 32 : 8;
            lara.rope_framerate += framerate * lara.rope_maxX_backward / 9000 + 1;
        }
    }
    else
    {
        if (item->pos.x_rot < 0 && (item->pos.x_rot - lara.rope_last_x) > 100)
        {
            lara.rope_arc_back = lara.rope_last_x;
            lara.rope_direction = 1;
            lara.rope_maxX_forward = 0;
            short frame = anims[ANIMATION_LARA_ROPE_SWING_FORWARD_SEMIHARD].frame_base - 15 * lara.rope_maxX_backward / 18000 + 17 << 8;
            if (frame > lara.rope_dframe)
            {
                lara.rope_dframe = frame;
                rope_swing = true;
            }
            else
            {
                rope_swing = false;
            }

            SoundEffect(SFX_LARA_ROPE_CREAK, &item->pos, 0);
        }
        else if (lara.rope_last_x > 0 && lara.rope_frame == lara.rope_dframe)
        {
            lara.rope_dframe = anims[ANIMATION_LARA_ROPE_SWING_FORWARD_SEMIHARD].frame_base - 15 * lara.rope_maxX_forward / 18000 + 17 << 8;
            lara.rope_framerate = 15 * lara.rope_maxX_forward / 9000 + 1;
            rope_swing = false;
        }
        else if (lara.rope_framerate < 512)
        {
            int framerate = rope_swing ? 32 : 8;
            lara.rope_framerate += framerate * lara.rope_maxX_forward / 9000 + 1;
        }
    }

    lara.rope_last_x = item->pos.x_rot;
    if (lara.rope_direction)
    {
        if (item->pos.x_rot > lara.rope_maxX_forward)
            lara.rope_maxX_forward = item->pos.x_rot;
    }
    else
    {
        if (item->pos.x_rot < -lara.rope_maxX_backward)
            lara.rope_maxX_backward = abs(item->pos.x_rot);
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
    if (lara.rope_ptr == NO_ROPE)
        return;

    if (item->pos.x_rot >= 0)
    {
        item->fallspeed = -112;
        item->speed = item->pos.x_rot / (STEP_L / 2);
    }
    else
    {
        item->fallspeed = -20;
        item->speed = 0;
    }

    item->pos.x_rot = 0;
    item->gravity_status = TRUE;
    lara.gun_status = LHS_ARMLESS;

    short frame = (item->current_frame - anims[ANIMATION_LARA_ROPE_SWING_FORWARD_SEMIHARD].frame_base);
    if (frame > 21)
        item->current_anim = ANIMATION_LARA_ROPE_SWING_TO_TRY_HANG_MIDDLE;
    else if (frame > 42)
        item->current_anim = ANIMATION_LARA_ROPE_SWING_TO_TRY_HANG_FRONT2;
    else
        item->current_anim = ANIMATION_LARA_ROPE_SWING_TO_TRY_HANG_BACK;

    item->current_frame = anims[item->current_anim].frame_base;
    item->state_current = STATE_LARA_REACH;
    item->state_next = STATE_LARA_REACH;
    lara.rope_ptr = NO_ROPE;
}

void SetCornerAnim(ITEM_INFO* item, COLL_INFO* coll, short angle, short flip)
{
    lara.look = FALSE;
    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = FALSE;

    if (CHK_NOP(TrInput, IN_ACTION) || item->hit_points <= 0)
    {
        SetAnimationForItemAS(item, ANIMATION_LARA_FREE_FALL_FORWARD, STATE_LARA_JUMP_FORWARD);
        item->pos.y += STEP_L;
        item->pos.y_rot += angle / 2;
        item->speed = 2;
        item->fallspeed = 1;
        item->gravity_status = TRUE;
        lara.gun_status = LHS_ARMLESS;
        return;
    }

    if (!flip)
        return;

    if (lara.is_climbing)
        SetAnimationForItemAS(item, ANIMATION_LARA_LADDER_IDLE, STATE_LARA_LADDER_IDLE);
    else
        SetAnimationForItemASF(item, ANIMATION_LARA_HANG_IDLE, STATE_LARA_HANG, 21);

    item->pos.x = lara.corner_x;
    item->pos.z = lara.corner_z;
    coll->old.x = lara.corner_x;
    coll->old.z = lara.corner_z;
    item->pos.y_rot += angle;
}

BOOL CanLaraHangSideways(ITEM_INFO* item, COLL_INFO* coll, short angle)
{
    BOOL hang_test;
    SHORT world_angle;
    int old_x;
    int old_z;
    int x;
    int z;
    
    old_x = item->pos.x;
    old_z = item->pos.z;
    lara.move_angle = angle + item->pos.y_rot;
    x = item->pos.x;
    z = item->pos.z;
    world_angle = (USHORT)(lara.move_angle + ANGLE(45)) / ANGLE(90);

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

    item->pos.x = x;
    item->pos.z = z;
    coll->old.y = item->pos.y;
    hang_test = LaraHangTest(item, coll);
    item->pos.x = old_x;
    item->pos.z = old_z;
    lara.move_angle = angle + item->pos.y_rot;
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
    GetHeight(floor, x, y, z);
    index = trigger_index;

    if (!index)
        return 0;

    if ((*index & 0x1F) == 5) // climbing trigger ?
    {
        if (((*index >> 8) & 0x80u) != 0)
            return 0;
        index = trigger_index + 1;
    }

    return (*index & 0x1F) == 6 ? *index : NULL;
}

void SnapLaraToEdgeOfBlock(ITEM_INFO* item, COLL_INFO* coll, short angle)
{
    if (item->state_current == STATE_LARA_SHIMMY_RIGHT)
    {
        switch (angle)
        {
            case NORTH:
                item->pos.x = coll->old.x & ~111 | 912;
                break;
            case EAST:
                item->pos.x = coll->old.x & ~911 | 112;
                break;
            case SOUTH:
                item->pos.x = coll->old.x & ~911 | 112;
                break;
            case WEST:
                item->pos.x = coll->old.x & ~111 | 912;
                break;
        }
    }
    else if (item->state_current == STATE_LARA_SHIMMY_LEFT)
    {
        switch (angle)
        {
            case NORTH:
                item->pos.x = coll->old.x & ~911 | 112;
                break;
            case EAST:
                item->pos.x = coll->old.x & ~111 | 911;
                break;
            case SOUTH:
                item->pos.x = coll->old.x & ~111 | 911;
                break;
            case WEST:
                item->pos.x = coll->old.x & ~911 | 112;
                break;
        }
    }
}

void GetLaraCollisionInfo(ITEM_INFO* item, COLL_INFO* coll)
{
    coll->facing = lara.move_angle;
    GetCollisionInfo(coll, item->pos.x, item->pos.y, item->pos.z, item->room_number, LARA_HITE);
}

void AnimateLara(ITEM_INFO* item)
{
    ANIM_STRUCT* anim;
    WORD num, type;
    short* command;
    int speed;

    item->current_frame++;
    anim = &anims[item->current_anim];

    // anim transition
    if (anim->number_changes > 0)
    {
        if (GetChange(item, anim))
        {
            anim = &anims[item->current_anim];
            item->state_current = anim->state_current;
        }
    }

    // command
    if (item->current_frame > anim->frame_end)
    {
        if (anim->number_commands > 0)
        {
            command = &commands[anim->command_index];
            for (short i = anim->number_commands; i > 0; i--)
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
                        item->gravity_status = TRUE;
                        if (lara.calc_fallspeed)
                        {
                            item->fallspeed = lara.calc_fallspeed;
                            lara.calc_fallspeed = 0;
                        }
                        break;
                    case COMMAND_ATTACK_READY:
                        if (lara.gun_status != LHS_SPECIAL)
                            lara.gun_status = LHS_ARMLESS;
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

        item->current_anim = anim->jump_anim_num;
        item->current_frame = anim->jump_frame_num;
        anim = &anims[item->current_anim];
        item->state_current = anim->state_current;
    }

    // sound or effect.
    if (anim->number_commands > 0)
    {
        command = &commands[anim->command_index];
        for (short i = anim->number_commands; i > 0; i--)
        {
            switch (*(command++))
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
                    if (item->current_frame == *(command))
                    {
                        type = *(command + 1) & 0xC000;
                        if  (type == SFX_LANDANDWATER ||
                            (type == SFX_LANDONLY  && (lara.water_surface_dist >= 0 || lara.water_surface_dist == -NO_HEIGHT)) ||
                            (type == SFX_WATERONLY &&  lara.water_surface_dist <  0 && lara.water_surface_dist != -NO_HEIGHT))
						{
							num = *(command + 1) & 0x3FFF;
							SoundEffect((int)num, &item->pos, SFX_ALWAYS);
						}
                    }
                    command += 2;
                    break;

                case COMMAND_EFFECT:
                    if (item->current_frame == *(command))
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
    if (item->gravity_status)
    {
        speed = anim->yspeed + anim->yfallspeed * (item->current_frame - anim->frame_base - 1);
        item->speed -= (short)(speed >> 16);
        speed += anim->yfallspeed;
        item->speed += (short)(speed >> 16);
        item->fallspeed += (item->fallspeed < FASTFALL_SPEED) ? GRAVITY : 1;
        item->pos.y += item->fallspeed;
    }
    else
    {
        speed = anim->yspeed;
        if (anim->yfallspeed)
            speed += (anim->yfallspeed * (item->current_frame - anim->frame_base));
        item->speed = (short)(speed >> 16);
    }

    // rope
    if (lara.rope_ptr != NO_ROPE)
        AlignLaraToRope(item);

    if (!lara.is_moving)
    {
        item->pos.x += (item->speed * SIN(lara.move_angle)) >> W2V_SHIFT;
        item->pos.z += (item->speed * COS(lara.move_angle)) >> W2V_SHIFT;
    }
}

BOOL LaraTestHangJump(ITEM_INFO* item, COLL_INFO* coll)
{
    short* bounds;
    int edge_catch, edge;
    short angle;

    if (CHK_NOP(TrInput, IN_ACTION) || lara.gun_status == LHS_HANDBUSY || coll->hit_static)
        return FALSE;

    if (lara.can_monkey_swing && coll->coll_type == COLL_TOP)
    {
        lara.head_y_rot = 0;
        lara.head_x_rot = 0;
        lara.torso_y_rot = 0;
        lara.torso_x_rot = 0;
        item->fallspeed = 0;
        item->speed = 0;
        item->gravity_status = FALSE;
        lara.gun_status = LHS_HANDBUSY;
        SetAnimationForItemAS(item, ANIMATION_LARA_OSCILLATE_HANG_ON, STATE_LARA_MONKEYSWING_IDLE);
        return TRUE;
    }

    if (coll->mid_ceiling > -STEPUP_HEIGHT || coll->mid_floor < 200 || coll->coll_type != COLL_FRONT)
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
        item->pos.y += coll->front_floor - bounds[2]; // catch edge

        SHORT rotationAngle = (USHORT)(item->pos.y_rot + 0x2000) / 0x4000;
        switch (rotationAngle)
        {
        case NORTH:
            item->pos.z = (item->pos.z |  (WALL_L - 1)) - LARA_RAD;
            item->pos.x += coll->shift.x;
            break;
        case EAST:
            item->pos.x = (item->pos.x |  (WALL_L - 1)) - LARA_RAD;
            item->pos.z += coll->shift.z;
            break;
        case SOUTH:
            item->pos.z = (item->pos.z & ~(WALL_L - 1)) + LARA_RAD;
            item->pos.x += coll->shift.x;
            break;
        case WEST:
            item->pos.x = (item->pos.x & ~(WALL_L - 1)) + LARA_RAD;
            item->pos.z += coll->shift.z;
            break;
        }
    }
    else
    {
        item->pos.y = edge - bounds[2];
    }

    item->pos.y_rot = angle;
    item->gravity_status = TRUE;
    item->speed = 2;
    item->fallspeed = 1;
    lara.gun_status = LHS_HANDBUSY;
    return TRUE;
}

BOOL LaraTestHangJumpUp(ITEM_INFO* item, COLL_INFO* coll)
{
    short* bounds;
    int edge, edge_catch;
    short angle;

    if (CHK_NOP(TrInput, IN_ACTION) || lara.gun_status != LHS_ARMLESS || coll->hit_static)
        return FALSE;

    if (lara.can_monkey_swing && coll->coll_type == COLL_TOP)
    {
        SetAnimationForItemAS(item, ANIMATION_LARA_MONKEY_GRAB, STATE_LARA_MONKEYSWING_IDLE);
        item->speed = 0;
        item->fallspeed = 0;
        item->gravity_status = FALSE;
        lara.gun_status = LHS_HANDBUSY;
        MonkeySwingSnap(item, coll);
        return TRUE;
    }

    if (coll->coll_type != COLL_FRONT || coll->mid_ceiling > -STEPUP_HEIGHT)
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
        item->pos.y += coll->front_floor - bounds[2];
    else
        item->pos.y = edge - bounds[2];

    item->pos.x += coll->shift.x;
    item->pos.z += coll->shift.z;
    item->pos.y_rot = angle;
    item->speed = 0;
    item->fallspeed = 0;
    item->gravity_status = FALSE;
    lara.torso_y_rot = 0;
    lara.torso_x_rot = 0;
    lara.gun_status = LHS_HANDBUSY;
    return TRUE;
}

#ifdef DLL_INJECT
void injector::inject_lara_utils()
{
    this->inject(ADDRESS_STRUCT(0x00421620, LaraFloorFront));
    this->inject(ADDRESS_STRUCT(0x00420A80, LaraCeilingFront));
    this->inject(ADDRESS_STRUCT(0x00422390, LaraHitCeiling));
    this->inject(ADDRESS_STRUCT(0x004230E0, LaraHangTest));
    this->inject(ADDRESS_STRUCT(0x00422400, LaraDeflectEdge));
    this->inject(ADDRESS_STRUCT(0x00421880, LaraDeflectEdgeDuck));
    this->inject(ADDRESS_STRUCT(0x00422C50, LaraDeflectEdgeJump));
    this->inject(ADDRESS_STRUCT(0x00420FE0, LaraFallen));
    this->inject(ADDRESS_STRUCT(0x00426600, IsValidHangPos));
    this->inject(ADDRESS_STRUCT(0x00426230, LaraHangRightCornerTest));
    this->inject(ADDRESS_STRUCT(0x004266E0, LaraHangLeftCornerTest));
    this->inject(ADDRESS_STRUCT(0x0042CEE0, LaraClimbLeftCornerTest));
    this->inject(ADDRESS_STRUCT(0x0042D160, LaraClimbRightCornerTest));
    this->inject(ADDRESS_STRUCT(0x004259C0, LaraSlideEdgeJump));
    this->inject(ADDRESS_STRUCT(0x00422810, LaraTestClimbStance));
    this->inject(ADDRESS_STRUCT(0x00421DE0, LaraTestEdgeCatch));
    this->inject(ADDRESS_STRUCT(0x00428470, LaraSlideSlope));
    this->inject(ADDRESS_STRUCT(0x00428B20, LaraCollideJump));
    this->inject(ADDRESS_STRUCT(0x0042C980, LaraCheckForLetGo));
    this->inject(ADDRESS_STRUCT(0x0042CA60, LaraTestClimbUpPos));
    this->inject(ADDRESS_STRUCT(0x0042C3C0, LaraTestClimbPos));
    this->inject(ADDRESS_STRUCT(0x0042C470, LaraTestClimb));
    this->inject(ADDRESS_STRUCT(0x0042CD20, LaraDoClimbLeftRight));
    this->inject(ADDRESS_STRUCT(0x00431E90, LaraTestWaterClimbOut));
    this->inject(ADDRESS_STRUCT(0x004321B0, LaraTestWaterStepOut));
    this->inject(ADDRESS_STRUCT(0x00432A30, LaraTestWaterDepth));
    this->inject(ADDRESS_STRUCT(0x00422BE0, LaraLandedBad));
    this->inject(ADDRESS_STRUCT(0x004229D0, LaraCollideStop));
    this->inject(ADDRESS_STRUCT(0x004328E0, LaraSwimCollision));
    this->inject(ADDRESS_STRUCT(0x004320B0, LaraSurfaceCollision));
    ///this->inject(ADDRESS_STRUCT(0x00424E90, UseSpecialItem));
    this->inject(ADDRESS_STRUCT(0x00420EA0, TestLaraSlide));
    this->inject(ADDRESS_STRUCT(0x00422480, TestLaraVault));
    this->inject(ADDRESS_STRUCT(0x004236B0, TestMonkeyLeft));
    this->inject(ADDRESS_STRUCT(0x004237B0, TestMonkeyRight));
    this->inject(ADDRESS_STRUCT(0x00421FF0, TestHangSwingIn));
    this->inject(ADDRESS_STRUCT(0x004228D0, TestWall));
    this->inject(ADDRESS_STRUCT(0x00432690, SwimTurn));
    this->inject(ADDRESS_STRUCT(0x00422E50, MonkeySwingFall));
    this->inject(ADDRESS_STRUCT(0x00423660, MonkeySwingSnap));
    this->inject(ADDRESS_STRUCT(0x004241D0, FallFromRope));
    this->inject(ADDRESS_STRUCT(0x00424320, UpdateRopeSwing));
    this->inject(ADDRESS_STRUCT(0x00424150, ApplyVelocityToRope));
    this->inject(ADDRESS_STRUCT(0x004597D0, SetPendulumVelocity));
    this->inject(ADDRESS_STRUCT(0x00426C50, SetCornerAnim));
    this->inject(ADDRESS_STRUCT(0x00426AB0, CanLaraHangSideways));
    this->inject(ADDRESS_STRUCT(0x00423780, GetDirOctant));
    this->inject(ADDRESS_STRUCT(0x0042D7D0, GetClimbTrigger));
    this->inject(ADDRESS_STRUCT(0x004235B0, SnapLaraToEdgeOfBlock));
    this->inject(ADDRESS_STRUCT(0x00422350, GetLaraCollisionInfo));
    this->inject(ADDRESS_STRUCT(0x00430B60, AnimateLara));

}
#endif