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
        SetAnimationForItem(item, ANIMATION_LARA_OSCILLATE_HANG_ON, STATE_LARA_MONKEYSWING_IDLE);
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
        SetAnimationForItem(item, ANIMATION_LARA_OSCILLATE_HANG_ON, STATE_LARA_MONKEYSWING_IDLE);
    }
    else
    {
        SetAnimationForItem(item, ANIMATION_LARA_HANG_IDLE, STATE_LARA_HANG);
    }

    bounds = GetBoundsAccurate(item);
    if (edge_catch > 0)
    {
        item->pos.y += (coll->front_floor - bounds[2]); // catch edge

        SHORT rotationAngle = (USHORT)(item->pos.y_rot + 0x2000) >> W2V_SHIFT;
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
        item->pos.y = (edge - bounds[2]) - 22;
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
        SetAnimationForItem(item, ANIMATION_LARA_MONKEY_GRAB, STATE_LARA_MONKEYSWING_IDLE, STATE_LARA_MONKEYSWING_IDLE);
        item->speed = 0;
        item->fallspeed = 0;
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
        SetAnimationForItem(item, ANIMATION_LARA_MONKEY_GRAB, STATE_LARA_MONKEYSWING_IDLE);
    else
        SetAnimationForItem(item, ANIMATION_LARA_HANG_IDLE, STATE_LARA_HANG, LARA_HANG_IDLE_FRAME);

    bounds = GetBoundsAccurate(item);
    if (edge_catch > 0)
        item->pos.y += coll->front_floor - bounds[2];
    else
        item->pos.y += edge - bounds[2];

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
    this->inject(ADDRESS_STRUCT(0x00421FF0, TestHangSwingIn));
}
#endif