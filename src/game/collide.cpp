#include "framework.h"
#include "collide.h"
#include "control.h"
#include "items.h"

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

#ifdef DLL_INJECT
void injector::inject_collide()
{
    this->inject(0x00446F70, ShiftItem);
    this->inject(0x00446FB0, UpdateLaraRoom);
}
#endif