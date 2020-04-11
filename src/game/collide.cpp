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
    int* mptr;
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
    x2 = ((x * *(mptr + M00)) + (y * *(mptr + M01)) + (z * *(mptr + M02))) >> W2V_SHIFT;
    y2 = ((x * *(mptr + M10)) + (y * *(mptr + M11)) + (z * *(mptr + M12))) >> W2V_SHIFT;
    z2 = ((x * *(mptr + M20)) + (y * *(mptr + M21)) + (z * *(mptr + M22))) >> W2V_SHIFT;
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
    int x, y, z;

    laraitem->pos.x_rot = item->pos.x_rot;
    laraitem->pos.y_rot = item->pos.y_rot;
    laraitem->pos.z_rot = item->pos.z_rot;

    phd_PushUnitMatrix();
    phd_RotYXZ(item->pos.y_rot, item->pos.x_rot, item->pos.z_rot);
    int* mptr = phd_mxptr;
    x = item->pos.x + ((pos->z * *(mptr + M02) + pos->y * *(mptr + M01) + pos->x * *(mptr + M00)) >> W2V_SHIFT);
    y = item->pos.y + ((pos->z * *(mptr + M12) + pos->y * *(mptr + M11) + pos->x * *(mptr + M10)) >> W2V_SHIFT);
    z = item->pos.z + ((pos->z * *(mptr + M22) + pos->y * *(mptr + M21) + pos->x * *(mptr + M20)) >> W2V_SHIFT);
    phd_PopMatrix();

    laraitem->pos.x = x;
    laraitem->pos.y = y;
    laraitem->pos.z = z;
}

BOOL MoveLaraPosition(PHD_VECTOR* pos, ITEM_INFO* item, ITEM_INFO* laraitem)
{
    FLOOR_INFO* floor;
    PHD_3DPOS dest;
    int height;
    short room_number;

    phd_PushUnitMatrix();
    phd_RotYXZ(item->pos.y_rot, item->pos.x_rot, item->pos.z_rot);
    int* mptr = phd_mxptr;
    dest.x = item->pos.x + ((pos->z * *(mptr + M02) + pos->y * *(mptr + M01) + pos->x * *(mptr + M00)) >> W2V_SHIFT);
    dest.y = item->pos.y + ((pos->z * *(mptr + M12) + pos->y * *(mptr + M11) + pos->x * *(mptr + M10)) >> W2V_SHIFT);
    dest.z = item->pos.z + ((pos->z * *(mptr + M22) + pos->y * *(mptr + M21) + pos->x * *(mptr + M20)) >> W2V_SHIFT);
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

#ifdef DLL_INJECT
void injector::inject_collide()
{
    this->inject(0x00446F70, ShiftItem);
    this->inject(0x00446FB0, UpdateLaraRoom);
    this->inject(0x00447F30, TestLaraPosition);
    this->inject(0x00448070, AlignLaraPosition);
    this->inject(0x004483E0, MoveLaraPosition);
}
#endif