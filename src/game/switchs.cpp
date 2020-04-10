#include "framework.h"
#include "switchs.h"

void found_item_with_detector(short object_number)
{
    switch (object_number)
    {
        case PUZZLE_ITEM1:
        case PUZZLE_ITEM2:
        case PUZZLE_ITEM3:
        case PUZZLE_ITEM4:
        case PUZZLE_ITEM5:
        case PUZZLE_ITEM6:
        case PUZZLE_ITEM7:
        case PUZZLE_ITEM8:
        case PUZZLE_ITEM9:
        case PUZZLE_ITEM10:
        case PUZZLE_ITEM11:
        case PUZZLE_ITEM12:
            lara.puzzleitems[object_number - PUZZLE_ITEM1]--;
            break;
        case PUZZLE_ITEM1_COMBO1:
        case PUZZLE_ITEM1_COMBO2:
        case PUZZLE_ITEM2_COMBO1:
        case PUZZLE_ITEM2_COMBO2:
        case PUZZLE_ITEM3_COMBO1:
        case PUZZLE_ITEM3_COMBO2:
        case PUZZLE_ITEM4_COMBO1:
        case PUZZLE_ITEM4_COMBO2:
        case PUZZLE_ITEM5_COMBO1:
        case PUZZLE_ITEM5_COMBO2:
        case PUZZLE_ITEM6_COMBO1:
        case PUZZLE_ITEM6_COMBO2:
        case PUZZLE_ITEM8_COMBO1:
        case PUZZLE_ITEM8_COMBO2:
            lara.puzzleitemscombo &= ~(1 << (object_number + 69));
            break;
        case KEY_ITEM1:
        case KEY_ITEM2:
        case KEY_ITEM3:
        case KEY_ITEM4:
        case KEY_ITEM5:
        case KEY_ITEM6:
        case KEY_ITEM7:
        case KEY_ITEM8:
        case KEY_ITEM9:
        case KEY_ITEM10:
        case KEY_ITEM11:
        case KEY_ITEM12:
            lara.keyitems &= ~(1 << (object_number + 53));
            break;
        case KEY_ITEM1_COMBO1:
        case KEY_ITEM1_COMBO2:
        case KEY_ITEM2_COMBO1:
        case KEY_ITEM2_COMBO2:
        case KEY_ITEM3_COMBO1:
        case KEY_ITEM3_COMBO2:
        case KEY_ITEM4_COMBO1:
        case KEY_ITEM4_COMBO2:
        case KEY_ITEM5_COMBO1:
        case KEY_ITEM5_COMBO2:
        case KEY_ITEM6_COMBO1:
        case KEY_ITEM6_COMBO2:
        case KEY_ITEM7_COMBO1:
        case KEY_ITEM7_COMBO2:
        case KEY_ITEM8_COMBO1:
        case KEY_ITEM8_COMBO2:
            lara.keyitemscombo &= ~(1 << (object_number + 41));
            break;
        case PICKUP_ITEM1:
        case PICKUP_ITEM2:
        case PICKUP_ITEM3:
        case PICKUP_ITEM4:
            lara.pickupitems &= ~(1 << (object_number + 25));
            break;
        case PICKUP_ITEM1_COMBO1:
        case PICKUP_ITEM1_COMBO2:
        case PICKUP_ITEM2_COMBO1:
        case PICKUP_ITEM2_COMBO2:
        case PICKUP_ITEM3_COMBO1:
        case PICKUP_ITEM3_COMBO2:
        case PICKUP_ITEM4_COMBO1:
        case PICKUP_ITEM4_COMBO2:
            lara.pickupitemscombo &= ~(1 << (object_number + 21));
            break;
        case QUEST_ITEM1:
        case QUEST_ITEM2:
        case QUEST_ITEM3:
        case QUEST_ITEM4:
        case QUEST_ITEM5:
        case QUEST_ITEM6:
            lara.questitems &= ~(1 << (object_number + 4));
            break;
    }
}

#ifdef DLL_INJECT
void injector::inject_switchs()
{
    this->inject(0x0043F050, found_item_with_detector);
}
#endif