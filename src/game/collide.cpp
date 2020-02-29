#include "framework.h"
#include "collide.h"

void ShiftItem(ITEM_INFO* item, COLL_INFO* coll)
{
    item->pos.x += coll->shift.x;
    item->pos.y += coll->shift.y;
    item->pos.z += coll->shift.z;
    coll->shift.x = 0;
    coll->shift.y = 0;
    coll->shift.z = 0;
}

#ifdef DLL_INJECT
void injector::inject_collide()
{
    this->inject(ADDRESS_STRUCT(0x00446F70, ShiftItem));
}
#endif