#include "framework.h"
#include "lot.h"
#include "specific/init.h"

int baddie_count = 0;

void InitialiseLOT(BOOL allocateBaddie)
{
    CREATURE_INFO* cinfo;

    if (allocateBaddie)
        baddie_slots = (CREATURE_INFO*)game_malloc(NUM_SLOTS * sizeof(CREATURE_INFO));

    cinfo = baddie_slots;
    for (int i = 0; i < NUM_SLOTS; i++, cinfo++)
    {
        cinfo->item_number = NO_ITEM;
        if (allocateBaddie)
            cinfo->LOT.node = (BOX_NODE*)game_malloc(number_boxes * sizeof(BOX_NODE));
    }

    baddie_count = 0;
}

void DisableBaddieAI(short itemNumber)
{
    ITEM_INFO* item;
    CREATURE_INFO* creature;

    item = &items[itemNumber];
    if (item->data == NULL)
        return;
    creature = (CREATURE_INFO*)item->data;

    if (creature)
    {
        creature->item_number = NO_ITEM;
        --baddie_count;
    }

    item->data = NULL;
}

BOOL EnableBaddieAI(short itemNumber, BOOL always)
{
    ITEM_INFO* item;
    CREATURE_INFO* creature;
    int x, y, z, worstdist, worstslot;

    item = &items[itemNumber];
    // Return now if Baddie already got AI
    if (item->data)
        return TRUE;

    if (baddie_count < NUM_SLOTS)
    {
        creature = baddie_slots;
        for (int slot = 0; slot < NUM_SLOTS; slot++, creature++)
        {
            if (creature->item_number == NO_ITEM)
            {
                InitialiseSlot(itemNumber, slot);
                return TRUE;
            }
        }
    }

    if (!always)
    {
        item = &items[itemNumber];
        x = (item->pos.x - camera.pos.x) >> 8;
        y = (item->pos.y - camera.pos.y) >> 8;
        z = (item->pos.z - camera.pos.z) >> 8;
        worstdist = SQUARE(x) + SQUARE(y) + SQUARE(z);
    }
    else
    {
        worstdist = 0;
    }

    worstslot = NO_ITEM;
    creature = baddie_slots;
    for (int slot = 0; slot < NUM_SLOTS; slot++, creature++)
    {
        item = &items[creature->item_number];
        x = (item->pos.x - camera.pos.x) >> 8;
        y = (item->pos.y - camera.pos.y) >> 8;
        z = (item->pos.z - camera.pos.z) >> 8;
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
        short worstnumber = baddie_slots[worstslot].item_number;
        item = &items[worstnumber];
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

    creature = &baddie_slots[slots];
    item = &items[itemNumber];

    // Default settings for creature
    creature->item_number = itemNumber;
    creature->mood = BORED_MOOD;
    creature->joint_rotation[0] = 0;
    creature->joint_rotation[1] = 0;
    creature->joint_rotation[2] = 0;
    creature->joint_rotation[3] = 0;
    creature->maximum_turn = ANGLE(1);
    creature->flags = 0;
    creature->enemy = NULL;
    creature->alerted = FALSE;
    creature->head_left = FALSE;
    creature->head_right = FALSE;
    creature->reached_goal = FALSE;
    creature->hurt_by_lara = FALSE;
    creature->patrol2 = FALSE;
    creature->jump_ahead = FALSE;
    creature->monkey_ahead = FALSE;

    // Default settings for LOT
    creature->LOT.step = STEP_L;
    creature->LOT.drop = -STEP_L;
    creature->LOT.block_mask = BLOCKED;
    creature->LOT.fly = NO_FLY;
    creature->LOT.can_jump = FALSE;
    creature->LOT.can_monkey = FALSE;
    creature->LOT.is_amphibious = FALSE;
    creature->LOT.is_jumping = FALSE;
    creature->LOT.is_monkeying = FALSE;
    creature->LOT.zone = ZONE_BASIC;

    // Assign default setting to item and the future one if creature is modified.
    item->data = creature;

    switch (item->object_number)
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
    if (itemNumber != lara.item_number)
        CreateZone(item);
    ++baddie_count;
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
    r = &rooms[item->room_number];
    item->box_number = XZ_GET_SECTOR(r, item->pos.x, item->pos.z).box & BOX_NUMBER;

    if (creature->LOT.fly == NO_FLY)
    {
        zone = ground_zone[creature->LOT.zone][FALSE];
        flip = ground_zone[creature->LOT.zone][TRUE];
        zone_number = zone[item->box_number];
        flip_number = flip[item->box_number];

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
