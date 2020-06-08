#include "framework.h"
#include "mutant.h"
#include "box.h"

enum MUTANT_STATE
{
    MUTANT_EMPTY,
    MUTANT_APPEAR,
    MUTANT_IDLE,
    MUTANT_SHOOT,
    MUTANT_LOCUST1,
    MUTANT_LOCUST2,
};

constexpr auto MUTANT_ANIM_APPEAR = 0;

void InitialiseMutant(short itemNumber)
{
    ITEM_INFO* item;
    InitialiseCreature(itemNumber);

    item = &Items[itemNumber];
    item->animNumber = Objects[item->objectNumber].animIndex + MUTANT_ANIM_APPEAR;
    item->frameNumber = Anims[item->animNumber].frameBase;
    item->currentAnimState = MUTANT_APPEAR;
    item->goalAnimState = MUTANT_APPEAR;
}

#ifdef DLL_INJECT
void injector::f_game::inject_mutant()
{
    inject(0x00403ED0, InitialiseMutant);
}
#endif
