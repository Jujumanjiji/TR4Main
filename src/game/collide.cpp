#include "framework.h"
#include "collide.h"
#include "3d_gen.h"
#include "3d_gen_a.h"
#include "control.h"
#include "items.h"
#include "lara.h"

void ShiftItem(ITEM_INFO* item, COLL_INFO* coll)
{
    item->pos.x += coll->shift.x;
    item->pos.y += coll->shift.y;
    item->pos.z += coll->shift.z;
    coll->shift.x = 0;
    coll->shift.y = 0;
    coll->shift.z = 0;
}

void UpdateLaraRoom(ITEM_INFO* item, int height)
{
    FLOOR_INFO *floor;
    short room_number;
    int x, y, z;

    x = item->pos.x;
    y = item->pos.y + height;
    z = item->pos.z;
    room_number = item->room_number;
    floor = GetFloor(x, y, z, &room_number);
    item->floor = GetHeight(floor, x, y, z);
    if (item->room_number != room_number)
        ItemNewRoom(lara.item_number, room_number);
}

BOOL TestLaraPosition(BOUNDARY* bounds, ITEM_INFO* item, ITEM_INFO* laraitem)
{
    PHD_MATRIX* mptr;
    int x, y, z, x2, y2, z2;
    short rot_x, rot_y, rot_z;

    rot_x = laraitem->pos.x_rot - item->pos.x_rot;
    rot_y = laraitem->pos.y_rot - item->pos.y_rot;
    rot_z = laraitem->pos.z_rot - item->pos.z_rot;
    if (rot_x < bounds->xrot_min || rot_x > bounds->xrot_max)
        return FALSE;
    if (rot_y < bounds->yrot_min || rot_y > bounds->yrot_max)
        return FALSE;
    if (rot_z < bounds->zrot_min || rot_z > bounds->zrot_max)
        return FALSE;

    phd_PushUnitMatrix();
    phd_RotYXZ(item->pos.y_rot, item->pos.x_rot, item->pos.z_rot);
    x = laraitem->pos.x - item->pos.x;
    y = laraitem->pos.y - item->pos.y;
    z = laraitem->pos.z - item->pos.z;
    mptr = phd_mxptr;
    x2 = ((mptr->m00 * x) + (mptr->m01 * y) + (mptr->m02 * z)) >> W2V_SHIFT;
    y2 = ((mptr->m10 * x) + (mptr->m11 * y) + (mptr->m12 * z)) >> W2V_SHIFT;
    z2 = ((mptr->m20 * x) + (mptr->m21 * y) + (mptr->m22 * z)) >> W2V_SHIFT;
    phd_PopMatrix();

    if (x2 < bounds->x_min || x2 > bounds->x_max)
        return FALSE;
    if (y2 < bounds->y_min || y2 > bounds->y_max)
        return FALSE;
    if (z2 < bounds->z_min || z2 > bounds->z_max)
        return FALSE;

    return TRUE;
}

void AlignLaraPosition(PHD_VECTOR* pos, ITEM_INFO* item, ITEM_INFO* laraitem)
{
    PHD_MATRIX* mptr;
    int x, y, z;

    laraitem->pos.x_rot = item->pos.x_rot;
    laraitem->pos.y_rot = item->pos.y_rot;
    laraitem->pos.z_rot = item->pos.z_rot;

    phd_PushUnitMatrix();
    phd_RotYXZ(item->pos.y_rot, item->pos.x_rot, item->pos.z_rot);
    mptr = phd_mxptr;
    x = item->pos.x + ((mptr->m00 * pos->x) + (mptr->m01 * pos->y) + (mptr->m02 * pos->z)) >> W2V_SHIFT;
    y = item->pos.y + ((mptr->m10 * pos->x) + (mptr->m11 * pos->y) + (mptr->m12 * pos->z)) >> W2V_SHIFT;
    z = item->pos.z + ((mptr->m20 * pos->x) + (mptr->m21 * pos->y) + (mptr->m22 * pos->z)) >> W2V_SHIFT;
    phd_PopMatrix();

    laraitem->pos.x = x;
    laraitem->pos.y = y;
    laraitem->pos.z = z;
}

BOOL MoveLaraPosition(PHD_VECTOR* pos, ITEM_INFO* item, ITEM_INFO* laraitem)
{
    PHD_MATRIX* mptr;
    FLOOR_INFO* floor;
    PHD_3DPOS dest;
    int height;
    short room_number;

    phd_PushUnitMatrix();
    phd_RotYXZ(item->pos.y_rot, item->pos.x_rot, item->pos.z_rot);
    mptr = phd_mxptr;
    dest.x = item->pos.x + ((mptr->m00 * pos->x) + (mptr->m01 * pos->y) + (mptr->m02 * pos->z)) >> W2V_SHIFT;
    dest.y = item->pos.y + ((mptr->m10 * pos->x) + (mptr->m11 * pos->y) + (mptr->m12 * pos->z)) >> W2V_SHIFT;
    dest.z = item->pos.z + ((mptr->m20 * pos->x) + (mptr->m21 * pos->y) + (mptr->m22 * pos->z)) >> W2V_SHIFT;
    dest.x_rot = item->pos.x_rot;
    dest.y_rot = item->pos.y_rot;
    dest.z_rot = item->pos.z_rot;
    phd_PopMatrix();

    if (item->object_number != FLARE_ITEM && item->object_number != BURNING_TORCH_ITEM && item->object_number != CLOCKWORK_BEETLE)
        return Move3DPosTo3DPos(&laraitem->pos, &dest, LARA_VELOCITY, ANGLE(2));

    room_number = laraitem->room_number;
    floor = GetFloor(dest.x, dest.y, dest.z, &room_number);
    height = GetHeight(floor, dest.x, dest.y, dest.z);

    if (abs(height - laraitem->pos.y) < CLICK(2))
    {
        if (phd_sqrt(SQUARE(dest.x - laraitem->pos.x) + SQUARE(dest.y - laraitem->pos.y) + SQUARE(dest.z - laraitem->pos.z)) < (CLICK(1) / 2))
            return TRUE;
        return Move3DPosTo3DPos(&laraitem->pos, &dest, LARA_VELOCITY, ANGLE(2));
    }

    if (lara.is_moving)
    {
        lara.is_moving = FALSE;
        lara.gun_status = LHS_ARMLESS;
    }

    return FALSE;
}

BOOL Move3DPosTo3DPos(PHD_3DPOS* src, PHD_3DPOS* dest, int velocity, short angadd)
{
    DWORD matrix_angle;
    int x, y, z, dist;
    short angdif;
    short angle_src, angle_target;
    short cardinal_point;

    x = dest->x - src->x;
    y = dest->y - src->y;
    z = dest->z - src->z;
    dist = phd_sqrt(SQUARE(x) + SQUARE(y) + SQUARE(z));

    if (velocity < dist)
    {
        src->x += velocity * x / dist;
        src->y += velocity * y / dist;
        src->z += velocity * z / dist;
    }
    else
    {
        src->x = dest->x;
        src->y = dest->y;
        src->z = dest->z;
    }

    if (!lara.is_moving)
    {
        if (lara.water_status != LWS_UNDERWATER)
        {
            matrix_angle = mGetAngle(dest->x, dest->z, src->x, src->z);
            angle_src = (unsigned short)((matrix_angle + ANGLE(45)) / ANGLE(90));
            angle_target = (unsigned short)((dest->y_rot + ANGLE(45)) / ANGLE(90));
            cardinal_point = (angle_src - angle_target) & 3;
            switch (cardinal_point)
            {
            case NORTH:
                lara_item->anim_number = ANIMATION_LARA_WALK_LEFT;
                lara_item->frame_number = anims[lara_item->anim_number].frame_base;
                lara_item->state_current = STATE_LARA_WALK_LEFT;
                lara_item->state_next = STATE_LARA_WALK_LEFT;
                break;
            case EAST:
                lara_item->anim_number = ANIMATION_LARA_WALK_FORWARD;
                lara_item->frame_number = anims[lara_item->anim_number].frame_base;
                lara_item->state_current = STATE_LARA_WALK_FORWARD;
                lara_item->state_next = STATE_LARA_WALK_FORWARD;
                break;
            case SOUTH:
                lara_item->anim_number = ANIMATION_LARA_WALK_RIGHT;
                lara_item->frame_number = anims[lara_item->anim_number].frame_base;
                lara_item->state_current = STATE_LARA_WALK_RIGHT;
                lara_item->state_next = STATE_LARA_WALK_RIGHT;
                break;
            case WEST:
            default:
                lara_item->anim_number = ANIMATION_LARA_WALK_BACK;
                lara_item->frame_number = anims[lara_item->anim_number].frame_base;
                lara_item->state_current = STATE_LARA_WALK_BACK;
                lara_item->state_next = STATE_LARA_WALK_BACK;
                break;
            }
        }

        lara.gun_status = LHS_HANDBUSY;
        lara.is_moving = TRUE;
        lara.move_count = 0;
    }
    
    // Do x rotation alignement
    angdif = (dest->x_rot - src->x_rot);
    if (angdif > angadd)
        src->x_rot += angadd;
    else if (angdif < -angadd)
        src->x_rot -= angadd;
    else
        src->x_rot = dest->x_rot;

    // Do y rotation alignement
    angdif = (dest->y_rot - src->y_rot);
    if (angdif > angadd)
        src->y_rot += angadd;
    else if (angdif < -angadd)
        src->y_rot -= angadd;
    else
        src->y_rot = dest->y_rot;

    // Do z rotation alignement
    angdif = (dest->z_rot - src->z_rot);
    if (angdif > angadd)
        src->z_rot += angadd;
    else if (angdif < -angadd)
        src->z_rot -= angadd;
    else
        src->z_rot = dest->z_rot;

    return (src->x == dest->x
         && src->y == dest->y
         && src->z == dest->z
         && src->x_rot == dest->x_rot
         && src->y_rot == dest->y_rot
         && src->z_rot == dest->z_rot);
}

#ifdef DLL_INJECT
void injector::inject_collide()
{
    this->inject(0x00446F70, ShiftItem);
    this->inject(0x00446FB0, UpdateLaraRoom);
    this->inject(0x00447F30, TestLaraPosition);
    this->inject(0x00448070, AlignLaraPosition);
    this->inject(0x004483E0, MoveLaraPosition);
    this->inject(0x00448140, Move3DPosTo3DPos);
}
#endif