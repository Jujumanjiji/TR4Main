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
    SHORT angle;
    BOOL hit;
    int xColl, zColl;
    short* bounds;
    short coll_dist;
    short front, back, ceiling;
    short old_coll_front, coll_front;
    short old_move_angle;

    hit = FALSE;
    coll_dist = 0;
    old_move_angle = lara.move_angle;
    if (lara.move_angle == (item->pos.y_rot - 0x4000))
        coll_dist = -LARA_RAD;
    else if (lara.move_angle == (item->pos.y_rot + 0x4000))
        coll_dist = LARA_RAD;

    front = LaraFloorFront(item, lara.move_angle, LARA_RAD);
    if (front < (LARA_RAD * 2))
        hit = TRUE;

    ceiling = LaraCeilingFront(item, lara.move_angle, LARA_RAD, 0);
    angle = (USHORT)(item->pos.y_rot + 0x2000) / 0x4000;

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
            if (item->current_anim != ANIMATION_LARA_LADDER_TO_HANDS_LEFT
            &&  item->current_anim != ANIMATION_LARA_LADDER_TO_HANDS_RIGHT)
            {
                SnapLaraToEdgeOfBlock(item, coll, angle);
                SetAnimationForItemASF(item, ANIMATION_LARA_HANG_IDLE, STATE_LARA_HANG, LARA_HANG_IDLE_FRAME_AFTERLEFTRIGHT);
            }
            return TRUE;
        }

        if (item->current_anim == ANIMATION_LARA_HANG_IDLE
        &&  item->current_frame == (anims[ANIMATION_LARA_HANG_IDLE].frame_base + LARA_HANG_IDLE_FRAME_AFTERLEFTRIGHT)
        &&  LaraTestClimbStance(item, coll))
        {
            item->state_next = STATE_LARA_LADDER_IDLE;
        }

        return FALSE;
    }

    if (CHK_NOP(TrInput, IN_ACTION) || item->hit_points <= 0 || coll->front_floor > 0)
    {
        SetAnimationForItemASF(item, ANIMATION_LARA_TRY_HANG_VERTICAL, STATE_LARA_JUMP_UP, 9);
        bounds = GetBoundsAccurate(item);
        item->pos.x += coll->shift.x;
        item->pos.y += bounds[3];
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
            hit = FALSE;
        else if (coll_dist < 0 && coll->left_floor < coll->right_floor)
            hit = FALSE;
    }

    bounds = GetBoundsAccurate(item);
    old_coll_front = coll->front_floor;
    coll_front = coll->front_floor - bounds[2];
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
    if (GetClimableWalls(xColl, item->pos.y, zColl, item->room_number) & (STEP_L << angle))
    {
        if (!LaraTestHangOnClimbWall(item, coll))
            coll_front = 0;
    }
    else
    {

    }

    coll->front_floor = old_coll_front;
    if (coll->mid_ceiling >= 0 || coll->coll_type != COLL_FRONT || hit || coll->hit_static || ceiling > -950 || coll_front < -60 || coll_front > 60)
    {
        item->pos.x = coll->old.x;
        item->pos.y = coll->old.y;
        item->pos.z = coll->old.z;
        if (item->state_current == STATE_LARA_SHIMMY_LEFT || item->state_current == STATE_LARA_SHIMMY_RIGHT)
            SetAnimationForItemASF(item, ANIMATION_LARA_HANG_IDLE, STATE_LARA_HANG, LARA_HANG_IDLE_FRAME_AFTERLEFTRIGHT);
        return TRUE;
    }
    else
    {
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
        }
    }

    return FALSE;
}

BOOL LaraTestHangJump(ITEM_INFO* item, COLL_INFO* coll)
{
    int edge_catch, edge;
    short *bounds, angle;

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
        item->pos.y += (coll->front_floor - bounds[2]); // catch edge

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
    int edge, edge_catch;
    short angle, *bounds;

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
    this->inject(legacy_LaraFloorFront);
    this->inject(legacy_LaraCeilingFront);
    this->inject(legacy_LaraHitCeiling);
    this->inject(legacy_LaraHangTest);



    this->inject(legacy_TestHangSwingIn);
}
#endif