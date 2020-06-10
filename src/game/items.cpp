#include "framework.h"
#include "items.h"
#include "draw.h"
#include "lara.h"

constexpr auto ITEM_RADIUS_YMAX = SECTOR(3);

static bool ItemCollide(int value, int radius)
{
    return value >= -radius && value <= radius;
}

static bool ItemInRange(int x, int z, int radius)
{
    return (SQUARE(x) + SQUARE(z)) <= SQUARE(radius);
}

BOOL ItemNearLara(PHD_VECTOR* pos, int radius)
{
    ANIM_FRAME* bounds;
    GAME_VECTOR target;
    target.x = pos->x - LaraItem->pos.xPos;
    target.y = pos->y - LaraItem->pos.yPos;
    target.z = pos->z - LaraItem->pos.zPos;
    if (!ItemCollide(target.y, ITEM_RADIUS_YMAX))
        return FALSE;
    if (!ItemCollide(target.x, radius) || !ItemCollide(target.z, radius))
        return FALSE;
    if (!ItemInRange(target.x, target.z, radius))
        return FALSE;

    bounds = (ANIM_FRAME*)GetBoundsAccurate(LaraItem);
    if (target.y >= bounds->minY && target.y <= bounds->maxY + LARA_RAD)
        return TRUE;
    
    return FALSE;
}

bool ItemNearTarget(PHD_3DPOS* src, ITEM_INFO* target, int radius)
{
    ANIM_FRAME* bounds;
    PHD_VECTOR pos;
    pos.x = src->xPos - target->pos.xPos;
    pos.y = src->yPos - target->pos.yPos;
    pos.z = src->zPos - target->pos.zPos;
    if (!ItemCollide(pos.y, ITEM_RADIUS_YMAX))
        return false;
    if (!ItemCollide(pos.x, radius) || !ItemCollide(pos.z, radius))
        return false;
    if (!ItemInRange(pos.x, pos.z, radius))
        return false;

    bounds = (ANIM_FRAME*)GetBoundsAccurate(target);
    if (pos.y >= bounds->minY && pos.y <= bounds->maxY)
        return true;

    return false;
}

#ifdef DLL_INJECT
void injector::f_game::inject_items()
{
    inject(0x00437050, ItemNearLara);
}
#endif
