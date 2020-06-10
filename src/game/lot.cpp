#include "framework.h"
#include "lot.h"
#include "init.h"

int BaddieCount = 0;

void InitialiseLOT(BOOL allocateBaddie)
{
    CREATURE_INFO* cinfo;

    if (allocateBaddie)
        BaddieSlots = (CREATURE_INFO*)game_malloc(NUM_SLOTS * sizeof(CREATURE_INFO));

    cinfo = BaddieSlots;
    for (int i = 0; i < NUM_SLOTS; i++, cinfo++)
    {
        cinfo->itemNum = NO_ITEM;
        if (allocateBaddie)
            cinfo->LOT.node = (BOX_NODE*)game_malloc(number_boxes * sizeof(BOX_NODE));
    }

    BaddieCount = 0;
}

void DisableBaddieAI(short itemNumber)
{
    ITEM_INFO* item;
    CREATURE_INFO* creature;

    item = &Items[itemNumber];
    if (item->data == NULL)
        return;
    creature = (CREATURE_INFO*)item->data;

    if (creature)
    {
        creature->itemNum = NO_ITEM;
        --BaddieCount;
    }

    item->data = NULL;
}

BOOL EnableBaddieAI(short itemNumber, BOOL always)
{
    ITEM_INFO* item;
    CREATURE_INFO* creature;
    int x, y, z, worstdist, worstslot;

    item = &Items[itemNumber];
    // Return now if Baddie already got AI
    if (item->data)
        return TRUE;

    if (BaddieCount < NUM_SLOTS)
    {
        creature = BaddieSlots;
        for (int slot = 0; slot < NUM_SLOTS; slot++, creature++)
        {
            if (creature->itemNum == NO_ITEM)
            {
                InitialiseSlot(itemNumber, slot);
                return TRUE;
            }
        }
    }

    if (!always)
    {
        item = &Items[itemNumber];
        x = (item->pos.xPos - camera.pos.x) >> 8;
        y = (item->pos.yPos - camera.pos.y) >> 8;
        z = (item->pos.zPos - camera.pos.z) >> 8;
        worstdist = SQUARE(x) + SQUARE(y) + SQUARE(z);
    }
    else
    {
        worstdist = 0;
    }

    worstslot = NO_ITEM;
    creature = BaddieSlots;
    for (int slot = 0; slot < NUM_SLOTS; slot++, creature++)
    {
        item = &Items[creature->itemNum];
        x = (item->pos.xPos - camera.pos.x) >> 8;
        y = (item->pos.yPos - camera.pos.y) >> 8;
        z = (item->pos.zPos - camera.pos.z) >> 8;
        int dist = SQUARE(x) + SQUARE(y) + SQUARE(z);
        if (dist > worstdist)
        {
            worstdist = dist;
            worstslot = slot;
        }
    }
    
    // Convert Baddie on worst slot to INVISIBLE and take over slot
    if (worstslot >= 0)
    {
        short worstnumber = BaddieSlots[worstslot].itemNum;
        item = &Items[worstnumber];
        item->status = FITEM_INVISIBLE;
        DisableBaddieAI(worstnumber);
        InitialiseSlot(itemNumber, worstslot);
        return TRUE;
    }

    return FALSE;
}

void InitialiseSlot(short itemNumber, int slots)
{
    CREATURE_INFO* creature;
    ITEM_INFO* item;

    creature = &BaddieSlots[slots];
    item = &Items[itemNumber];

    // Default settings for creature
    creature->itemNum = itemNumber;
    creature->mood = BORED_MOOD;
    creature->jointRotation[0] = 0;
    creature->jointRotation[1] = 0;
    creature->jointRotation[2] = 0;
    creature->jointRotation[3] = 0;
    creature->maximumTurn = ANGLE(1);
    creature->flags = 0;
    creature->enemy = NULL;
    creature->alerted = FALSE;
    creature->headLeft = FALSE;
    creature->headRight = FALSE;
    creature->reachedGoal = FALSE;
    creature->hurtByLara = FALSE;
    creature->patrol2 = FALSE;
    creature->jumpAhead = FALSE;
    creature->monkeyAhead = FALSE;

    // Default settings for LOT
    creature->LOT.step = STEP_L;
    creature->LOT.drop = -STEP_L;
    creature->LOT.block_mask = BLOCKED;
    creature->LOT.fly = NO_FLYING;
    creature->LOT.can_jump = FALSE;
    creature->LOT.can_monkey = FALSE;
    creature->LOT.is_amphibious = FALSE;
    creature->LOT.is_jumping = FALSE;
    creature->LOT.is_monkeying = FALSE;
    creature->LOT.zone = ZONE_BASIC;

    // Assign default setting to item and the future one if creature is modified.
    item->data = creature;

    switch (item->objectNumber)
    {
        case ENEMY_JEEP:
        case VON_CROY:
        case BADDY_1:
        case BADDY_2:
            creature->LOT.step = SECTOR(1);
            creature->LOT.drop = -SECTOR(1);
            creature->LOT.can_jump = TRUE;
            creature->LOT.can_monkey = TRUE;
            creature->LOT.zone = ZONE_HUMAN;
            break;
        case SKELETON:
        case SETHA:
            creature->LOT.step = CLICK(1);
            creature->LOT.drop = -CLICK(2);
            creature->LOT.can_jump = TRUE;
            creature->LOT.can_monkey = FALSE;
            creature->LOT.zone = ZONE_SKELLY;
            break;
        case GUIDE:
        case MUMMY:
        case SPHINX:
        case HORSEMAN:
        case SCORPION:
        case TROOPS:
        case KNIGHTS_TEMPLAR:
        case MUTANT:
        case WILD_BOAR:
        case DEMIGOD1:
        case DEMIGOD2:
        case DEMIGOD3:
        case DOG:
            creature->LOT.step = CLICK(1);
            creature->LOT.drop = -CLICK(2);
            creature->LOT.zone = ZONE_BASIC;
            break;
        case HAMMERHEAD:
            creature->LOT.step = SECTOR(20);
            creature->LOT.drop = -SECTOR(20);
            creature->LOT.fly = CREATURE_FLOAT_SPEED;
            creature->LOT.is_amphibious = FALSE;
            creature->LOT.zone = ZONE_WATER;
            break;
        case CROCODILE:
            creature->LOT.step = SECTOR(20);
            creature->LOT.drop = -SECTOR(20);
            creature->LOT.fly = CREATURE_FLOAT_SPEED;
            creature->LOT.is_amphibious = TRUE;
            creature->LOT.zone = ZONE_WATER;
            break;
        case HARPY:
        case BIG_BEETLE:
            creature->LOT.step = SECTOR(20);
            creature->LOT.drop = -SECTOR(20);
            creature->LOT.fly = 32;
            creature->LOT.zone = ZONE_FLYER;
            break;
        case BAT:
            creature->LOT.step = SECTOR(20);
            creature->LOT.drop = -SECTOR(20);
            creature->LOT.fly = 16;
            creature->LOT.zone = ZONE_FLYER;
            break;
    }

    ClearLOT(&creature->LOT);
    if (itemNumber != Lara.itemNumber)
        CreateZone(item);
    ++BaddieCount;
}

void CreateZone(ITEM_INFO* item)
{
    CREATURE_INFO* creature;
    BOX_NODE* node;
    ROOM_INFO* r;
    int i;
    short *zone, zone_number;
    short *flip, flip_number;

    creature = (CREATURE_INFO*)item->data;
    // Basic information about baddie
    r = &Rooms[item->roomNumber];
    item->boxNumber = XZ_GET_SECTOR(r, item->pos.xPos, item->pos.zPos).box & BOX_NUMBER;

    if (creature->LOT.fly == NO_FLYING)
    {
        zone = ground_zone[creature->LOT.zone][FALSE];
        flip = ground_zone[creature->LOT.zone][TRUE];
        zone_number = zone[item->boxNumber];
        flip_number = flip[item->boxNumber];

        for (i = 0, creature->LOT.zone_count = 0, node = creature->LOT.node; i < number_boxes; i++, zone++, flip++)
        {
            if (*zone == zone_number || *flip == flip_number)
            {
                node->box_number = i;
                node++;
                creature->LOT.zone_count++;
            }
        }
    }
    else
    {
        for (i = 0, creature->LOT.zone_count = 0, node = creature->LOT.node; i < number_boxes; i++)
        {
            node->box_number = i;
            node++;
            creature->LOT.zone_count++;
        }
    }
}

void ClearLOT(LOT_INFO* LOT)
{
    BOX_NODE* node;
    int i;

    LOT->tail = NO_BOX;
    LOT->head = NO_BOX;
    LOT->search_number = 0;
    LOT->target_box = NO_BOX;
    LOT->required_box = NO_BOX;

    for (i = 0, node = LOT->node; i < number_boxes; i++, node++)
    {
        node->exit_box = NO_BOX;
        node->next_expansion = NO_BOX;
        node->search_number = 0;
    }
}

#ifdef DLL_INJECT
void injector::f_game::inject_lot()
{
    inject(0x00455200, InitialiseLOT);
    inject(0x00455270, DisableBaddieAI);
    inject(0x004552B0, EnableBaddieAI);
    inject(0x00455470, InitialiseSlot);
    inject(0x004556A0, CreateZone);
    inject(0x004557B0, ClearLOT);
}
#endif
