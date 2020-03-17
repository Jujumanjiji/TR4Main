#include "framework.h"
#include "inventory.h"

void construct_inventory_2D(void)
{
}


#ifdef DLL_INJECT
void injector::inject_inventory()
{
    this->inject(ADDRESS_STRUCT(0x0043B9B0, construct_inventory_2D));
}
#endif