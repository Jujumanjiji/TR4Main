#include "framework.h"
#include "people.h"
#include "control.h"
#include "draw.h"

constexpr auto TARGET_VISIBILITY_MAXRANGE = SQUARE(SECTOR(30));

BOOL TargetVisible(ITEM_INFO* item, AI_INFO* info)
{
    CREATURE_INFO* creature = GetCreatureInfo(item);
    ITEM_INFO* enemy;
    ANIM_FRAME* bounds;
    GAME_VECTOR src, target;
    short angle;

    if (creature->enemy == nullptr)
        return FALSE;
    enemy = creature->enemy;
    if (enemy->hitPoints <= 0)
        return FALSE;
    if (enemy->data == nullptr)
        return FALSE;

    angle = info->angle - creature->jointRotation[2];
    if (angle <= -0x4000 || angle >= 0x4000 || info->distance >= TARGET_VISIBILITY_MAXRANGE)
        return FALSE;
    bounds = (ANIM_FRAME*)GetBestFrame(enemy);

    src.x = item->pos.xPos;
    src.y = item->pos.yPos - CLICK(3);
    src.z = item->pos.zPos;
    src.roomNumber = item->roomNumber;

    target.x = enemy->pos.xPos;
    target.y = enemy->pos.yPos + ((bounds->maxY + (3 * bounds->minY)) >> 2);
    target.z = enemy->pos.zPos;
    target.roomNumber = enemy->roomNumber;

    return LOS(&src, &target);
}

BOOL TargetVisibleCustom(ITEM_INFO* item, AI_INFO* info, int height)
{
    CREATURE_INFO* creature = GetCreatureInfo(item);
    ITEM_INFO* enemy;
    ANIM_FRAME* bounds;
    GAME_VECTOR src, target;
    short angle;

    if (creature->enemy == nullptr)
        return FALSE;
    enemy = creature->enemy;
    if (enemy->hitPoints <= 0)
        return FALSE;
    if (enemy->data == nullptr)
        return FALSE;

    angle = info->angle - creature->jointRotation[2];
    if (angle <= -0x4000 || angle >= 0x4000 || info->distance >= TARGET_VISIBILITY_MAXRANGE)
        return FALSE;
    bounds = (ANIM_FRAME*)GetBestFrame(enemy);
    src.x = item->pos.xPos;
    src.y = item->pos.yPos - height;
    src.z = item->pos.zPos;
    src.roomNumber = item->roomNumber;
    target.x = enemy->pos.xPos;
    target.y = enemy->pos.yPos + ((bounds->maxY + (3 * bounds->minY)) >> 2);
    target.z = enemy->pos.zPos;
    target.roomNumber = enemy->roomNumber;
    return LOS(&src, &target);
}

#ifdef DLL_INJECT
void injector::f_game::inject_people()
{
    inject(0x0040AEB0, TargetVisible);
}
#endif
