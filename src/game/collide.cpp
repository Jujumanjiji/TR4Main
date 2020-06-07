#include "framework.h"
#include "collide.h"
#include "3d_gen.h"
#include "3d_gen_a.h"
#include "control.h"
#include "draw.h"
#include "items.h"
#include "lara.h"

void ShiftItem(ITEM_INFO* item, COLL_INFO* coll)
{
    item->pos.xPos += coll->shift.x;
    item->pos.yPos += coll->shift.y;
    item->pos.zPos += coll->shift.z;
    coll->shift.x = 0;
    coll->shift.y = 0;
    coll->shift.z = 0;
}

void UpdateLaraRoom(ITEM_INFO* item, int height)
{
    FLOOR_INFO *floor;
    short roomNumber;
    int x, y, z;

    x = item->pos.xPos;
    y = item->pos.yPos + height;
    z = item->pos.zPos;
    roomNumber = item->roomNumber;
    floor = GetFloor(x, y, z, &roomNumber);
    item->floor = GetFloorHeight(floor, x, y, z);
    if (item->roomNumber != roomNumber)
        ItemNewRoom(Lara.itemNumber, roomNumber);
}

BOOL TestLaraPosition(BOUNDARY* bounds, ITEM_INFO* item, ITEM_INFO* laraitem)
{
    PHD_MATRIX* mptr;
    int x, y, z, x2, y2, z2;
    short rot_x, rot_y, rot_z;

    rot_x = laraitem->pos.xRot - item->pos.xRot;
    rot_y = laraitem->pos.yRot - item->pos.yRot;
    rot_z = laraitem->pos.zRot - item->pos.zRot;
    if (rot_x < bounds->xrot_min || rot_x > bounds->xrot_max)
        return FALSE;
    if (rot_y < bounds->yrot_min || rot_y > bounds->yrot_max)
        return FALSE;
    if (rot_z < bounds->zrot_min || rot_z > bounds->zrot_max)
        return FALSE;

    phd_PushUnitMatrix();
    phd_RotYXZ(item->pos.yRot, item->pos.xRot, item->pos.zRot);
    x = laraitem->pos.xPos - item->pos.xPos;
    y = laraitem->pos.yPos - item->pos.yPos;
    z = laraitem->pos.zPos - item->pos.zPos;
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

    laraitem->pos.xRot = item->pos.xRot;
    laraitem->pos.yRot = item->pos.yRot;
    laraitem->pos.zRot = item->pos.zRot;

    phd_PushUnitMatrix();
    phd_RotYXZ(item->pos.yRot, item->pos.xRot, item->pos.zRot);
    mptr = phd_mxptr;
    x = item->pos.xPos + ((mptr->m00 * pos->x) + (mptr->m01 * pos->y) + (mptr->m02 * pos->z) >> W2V_SHIFT);
    y = item->pos.yPos + ((mptr->m10 * pos->x) + (mptr->m11 * pos->y) + (mptr->m12 * pos->z) >> W2V_SHIFT);
    z = item->pos.zPos + ((mptr->m20 * pos->x) + (mptr->m21 * pos->y) + (mptr->m22 * pos->z) >> W2V_SHIFT);
    phd_PopMatrix();

    laraitem->pos.xPos = x;
    laraitem->pos.yPos = y;
    laraitem->pos.zPos = z;
}

BOOL MoveLaraPosition(PHD_VECTOR* pos, ITEM_INFO* item, ITEM_INFO* laraitem)
{
    PHD_MATRIX* mptr;
    FLOOR_INFO* floor;
    PHD_3DPOS dest;
    int height;
    short roomNumber;

    dest.xRot = item->pos.xRot;
    dest.yRot = item->pos.yRot;
    dest.zRot = item->pos.zRot;

    phd_PushUnitMatrix();
    phd_RotYXZ(item->pos.yRot, item->pos.xRot, item->pos.zRot);
    mptr = phd_mxptr;
    dest.xPos = item->pos.xPos + ((mptr->m00 * pos->x) + (mptr->m01 * pos->y) + (mptr->m02 * pos->z) >> W2V_SHIFT);
    dest.yPos = item->pos.yPos + ((mptr->m10 * pos->x) + (mptr->m11 * pos->y) + (mptr->m12 * pos->z) >> W2V_SHIFT);
    dest.zPos = item->pos.zPos + ((mptr->m20 * pos->x) + (mptr->m21 * pos->y) + (mptr->m22 * pos->z) >> W2V_SHIFT);
    phd_PopMatrix();

    if (item->objectNumber != FLARE_ITEM && item->objectNumber != BURNING_TORCH_ITEM && item->objectNumber != CLOCKWORK_BEETLE)
        return Move3DPosTo3DPos(&laraitem->pos, &dest, LARA_VELOCITY, ANGLE(2));

    roomNumber = laraitem->roomNumber;
    floor = GetFloor(dest.xPos, dest.yPos, dest.zPos, &roomNumber);
    height = GetFloorHeight(floor, dest.xPos, dest.yPos, dest.zPos);

    if (abs(height - laraitem->pos.yPos) < CLICK(2))
    {
        if (phd_sqrt(SQUARE(dest.xPos - laraitem->pos.xPos) + SQUARE(dest.yPos - laraitem->pos.yPos) + SQUARE(dest.zPos - laraitem->pos.zPos)) < (CLICK(1) / 2))
            return TRUE;
        return Move3DPosTo3DPos(&laraitem->pos, &dest, LARA_VELOCITY, ANGLE(2));
    }

    if (Lara.isMoving)
    {
        Lara.isMoving = FALSE;
        Lara.gunStatus = LHS_ARMLESS;
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

    x = dest->xPos - src->xPos;
    y = dest->yPos - src->yPos;
    z = dest->zPos - src->zPos;
    dist = phd_sqrt(SQUARE(x) + SQUARE(y) + SQUARE(z));

    if (velocity < dist)
    {
        src->xPos += (velocity * x) / dist;
        src->yPos += (velocity * y) / dist;
        src->zPos += (velocity * z) / dist;
    }
    else
    {
        src->xPos = dest->xPos;
        src->yPos = dest->yPos;
        src->zPos = dest->zPos;
    }

    if (!Lara.isMoving)
    {
        if (Lara.waterStatus != LWS_UNDERWATER)
        {
            matrix_angle = mGetAngle(dest->xPos, dest->zPos, src->xPos, src->zPos);
            angle_src = (WORD)((short(matrix_angle) + ANGLE(45)) / ANGLE(90));
            angle_target = (WORD)((dest->yRot + ANGLE(45)) / ANGLE(90));
            cardinal_point = (angle_src - angle_target) & 3;
            switch (cardinal_point)
            {
            case NORTH:
                LaraItem->animNumber = ANIMATION_LARA_WALK_LEFT;
                LaraItem->frameNumber = Anims[LaraItem->animNumber].frameBase;
                LaraItem->currentAnimState = STATE_LARA_WALK_LEFT;
                LaraItem->goalAnimState = STATE_LARA_WALK_LEFT;
                break;
            case EAST:
                LaraItem->animNumber = ANIMATION_LARA_WALK_FORWARD;
                LaraItem->frameNumber = Anims[LaraItem->animNumber].frameBase;
                LaraItem->currentAnimState = STATE_LARA_WALK_FORWARD;
                LaraItem->goalAnimState = STATE_LARA_WALK_FORWARD;
                break;
            case SOUTH:
                LaraItem->animNumber = ANIMATION_LARA_WALK_RIGHT;
                LaraItem->frameNumber = Anims[LaraItem->animNumber].frameBase;
                LaraItem->currentAnimState = STATE_LARA_WALK_RIGHT;
                LaraItem->goalAnimState = STATE_LARA_WALK_RIGHT;
                break;
            case WEST:
            default:
                LaraItem->animNumber = ANIMATION_LARA_WALK_BACK;
                LaraItem->frameNumber = Anims[LaraItem->animNumber].frameBase;
                LaraItem->currentAnimState = STATE_LARA_WALK_BACK;
                LaraItem->goalAnimState = STATE_LARA_WALK_BACK;
                break;
            }
        }

        Lara.gunStatus = LHS_HANDBUSY;
        Lara.isMoving = TRUE;
        Lara.moveCount = 0;
    }
    
    // Do x rotation alignement
    angdif = (dest->xRot - src->xRot);
    if (angdif > angadd)
        src->xRot += angadd;
    else if (angdif < -angadd)
        src->xRot -= angadd;
    else
        src->xRot = dest->xRot;

    // Do y rotation alignement
    angdif = (dest->yRot - src->yRot);
    if (angdif > angadd)
        src->yRot += angadd;
    else if (angdif < -angadd)
        src->yRot -= angadd;
    else
        src->yRot = dest->yRot;

    // Do z rotation alignement
    angdif = (dest->zRot - src->zRot);
    if (angdif > angadd)
        src->zRot += angadd;
    else if (angdif < -angadd)
        src->zRot -= angadd;
    else
        src->zRot = dest->zRot;

    return (src->xPos == dest->xPos
         && src->yPos == dest->yPos
         && src->zPos == dest->zPos
         && src->xRot == dest->xRot
         && src->yRot == dest->yRot
         && src->zRot == dest->zRot);
}

void CalcItemToFloorRotation(ITEM_INFO* item, int radiusDivide)
{
    FLOOR_INFO* floor;
    ANIM_FRAME* bounds;
    GAME_VECTOR pos;
    int ratioXZ, frontHDif, sideHDif;
    int frontX, frontZ, leftX, leftZ, rightX, rightZ;
    int frontHeight, backHeight, leftHeight, rightHeight;
    int radiusZ, radiusX;

    bounds = (ANIM_FRAME*)GetBoundsAccurate(item);
    pos.x = item->pos.xPos;
    pos.y = item->pos.yPos;
    pos.z = item->pos.zPos;
    pos.roomNumber = item->roomNumber;
    radiusX = bounds->maxX;
    radiusZ = bounds->maxZ / radiusDivide; // need divide in any case else it's too much !

    ratioXZ = radiusZ / radiusX;
    frontX = (SIN(item->pos.yRot) * radiusZ) >> W2V_SHIFT;
    frontZ = (COS(item->pos.yRot) * radiusZ) >> W2V_SHIFT;
    leftX = -frontZ * ratioXZ;
    leftZ = frontX * ratioXZ;
    rightX = frontZ * ratioXZ;
    rightZ = -frontX * ratioXZ;

    floor = GetFloor(pos.x + frontX, pos.y, pos.z + frontZ, &pos.roomNumber);
    frontHeight = GetFloorHeight(floor, pos.x + frontX, pos.y, pos.z + frontZ);
    floor = GetFloor(pos.x - frontX, pos.y, pos.z - frontZ, &pos.roomNumber);
    backHeight = GetFloorHeight(floor, pos.x - frontX, pos.y, pos.z - frontZ);
    floor = GetFloor(pos.x + leftX, pos.y, pos.z + leftZ, &pos.roomNumber);
    leftHeight = GetFloorHeight(floor, pos.x + leftX, pos.y, pos.z + leftZ);
    floor = GetFloor(pos.x + rightX, pos.y, pos.z + rightZ, &pos.roomNumber);
    rightHeight = GetFloorHeight(floor, pos.x + rightX, pos.y, pos.z + rightZ);

    frontHDif = backHeight - frontHeight;
    sideHDif = rightHeight - leftHeight;
    // NOTE: float(atan2()) is required, else warning about double !
    item->pos.xRot = ANGLEF(float(atan2(frontHDif, 2 * radiusZ)) / RADIAN);
    item->pos.zRot = ANGLEF(float(atan2(sideHDif, 2 * radiusX)) / RADIAN);
}

#ifdef DLL_INJECT
void injector::f_game::inject_collide()
{
    inject(0x00446F70, ShiftItem);
    inject(0x00446FB0, UpdateLaraRoom);
    inject(0x00447F30, TestLaraPosition);
    inject(0x00448070, AlignLaraPosition);
    inject(0x004483E0, MoveLaraPosition);
    inject(0x00448140, Move3DPosTo3DPos);
}
#endif
