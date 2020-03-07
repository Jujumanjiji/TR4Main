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
            if (item->current_anim == ANIMATION_LARA_LADDER_TO_HANDS_LEFT || item->current_anim == ANIMATION_LARA_LADDER_TO_HANDS_RIGHT)
                return TRUE;
            SnapLaraToEdgeOfBlock(item, coll, angle);
            item->pos.y = coll->old.y;
            SetAnimationForItemASF(item, ANIMATION_LARA_HANG_IDLE, STATE_LARA_HANG, LARA_HANG_IDLE_FRAME_AFTERLEFTRIGHT);
            return TRUE;
        }
        
        if (item->current_anim == ANIMATION_LARA_HANG_IDLE && item->current_frame == (anims[ANIMATION_LARA_HANG_IDLE].frame_base + LARA_HANG_IDLE_FRAME_AFTERLEFTRIGHT) && LaraTestClimbStance(item, coll))
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
    if (GetClimbableWalls(xColl, item->pos.y, zColl, item->room_number) & (STEP_L << angle))
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
            if (GetClimbableWalls(x, item->pos.y, z, item->room_number) & LeftClimbTab[angle])
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

    if (GetClimbableWalls(x, item->pos.y, z, item->room_number) & RightClimbTab[angle])
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
            if (GetClimbableWalls(x, item->pos.y, z, item->room_number) & RightClimbTab[angle])
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

    if (GetClimbableWalls(x, item->pos.y, z, item->room_number) & LeftClimbTab[angle])
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

    if (GetClimbableWalls(x, item->pos.y, z, item->room_number) & LeftIntRightExtTab[angle])
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

        if (GetClimbableWalls(x, item->pos.y, z, item->room_number) & LeftExtRightIntTab[angle])
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

    angle = GetCatchAngle(item);
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
                item->pos.z = (item->pos.z | (WALL_L - 1)) - LARA_RAD;
                item->pos.x += coll->shift.x;
                break;
            case EAST:
                item->pos.x = (item->pos.x | (WALL_L - 1)) - LARA_RAD;
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

    angle = GetCatchAngle(item);
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

    this->inject(ADDRESS_STRUCT(0x00421FF0, TestHangSwingIn));
}
#endif