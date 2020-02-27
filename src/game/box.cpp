#include "framework.h"
#include "box.h"
#include "3d_gen.h"
#include "control.h"
#include "draw.h"
#include "items.h"
#include "lot.h"
#include "sphere.h"
#include "utils.h"

#define ATTACK_RANGE SQUARE(WALL_L * 3)
#define BORED_DIST (WALL_L * 6)
#define STALK_DIST (WALL_L * 3)
#define ESCAPE_DIST (WALL_L * 5)
#define ESCAPE_CHANCE 0x800
#define RECOVER_CHANCE 0x100
#define MAX_X_ROT ANGLE(20)
#define GUNFIRE_ALERT_RADIUS 8000

/*
// Default setup of creature structure
void InitialiseCreature(short itemNumber)
{
    ITEM_INFO* item;

    item = &items[itemNumber];
    item->data = NULL;
    item->collidable = TRUE;
}

// If creature hasn't got a slot allocated, then try to activate it
BOOL CreatureActive(short itemNumber)
{
    ITEM_INFO* item;

    item = &items[itemNumber];
    if (CHK_ANY(item->flags, IFLAG_KILLED_ITEM))
        return FALSE;

    if (item->status == FITEM_INVISIBLE)
    {
        if (!EnableBaddieAI(itemNumber, FALSE))
            return FALSE;
        item->status = FITEM_ACTIVE;
    }

    return TRUE;
}

// Expands LOT list by up to 'expansion' nodes, allowing for restarts caused the
// target moving. Returns 1 if more expansion is needed, 0 otherwise
int UpdateLOT(LOT_INFO* LOT, int expansion)
{
    BOX_NODE* expand;

    // If target has passed into new box, add this to head of expansion list and update
    // search number (only 65536 searches possible, so things will go wrong if
    // player passes through this many boxes during the course of a level)
    if (LOT->required_box != NO_BOX && LOT->required_box != LOT->target_box)
    {
        LOT->target_box = LOT->required_box;

        expand = &LOT->node[LOT->required_box];
        if (expand->next_expansion == NO_BOX && LOT->tail != LOT->required_box)
        {
            expand->next_expansion = LOT->head;

            // list may be empty
            if (LOT->head == NO_BOX)
                LOT->tail = LOT->target_box;
            LOT->head = LOT->target_box;
        }
        expand->search_number = ++LOT->search_number;
        expand->exit_box = NO_BOX;
    }

    // expand LOT
    return SearchLOT(LOT, expansion);
}

int SearchLOT(LOT_INFO* LOT, int expansion)
{
    if (expansion <= 0)
        return TRUE;

    short* zone;
    short search_zone;
    BOX_NODE* node, *expand;
    BOX_INFO* box;
    int index, done, change, box_number;

    zone = ground_zone[LOT->zone][flip_status];
    search_zone = zone[LOT->head];

    for (int i = 0; i < expansion; i++)
    {
        if (LOT->head == NO_BOX)
        {
            LOT->tail = NO_BOX;
            return FALSE;
        }

        node = &LOT->node[LOT->head];
        box = &boxes[LOT->head];

        // expand node
        index = box->overlap_index & OVERLAP_INDEX;
        done = FALSE;
        do
        {
            box_number = overlaps[index++];
            if (box_number & BOX_END_BIT)
                done = TRUE;

            box_number &= BOX_NUMBER;
            if (LOT->fly == NO_FLY && search_zone != zone[box_number])
                continue;

            change = boxes[box_number].height - box->height;
            if (change > LOT->step || change < LOT->drop)
                continue;

            expand = &LOT->node[box_number];
            if ((node->search_number & SEARCH_NUMBER) < (expand->search_number & SEARCH_NUMBER))
                continue;

            if (node->search_number & BLOCKED_SEARCH)
            {
                if ((node->search_number & SEARCH_NUMBER) == (expand->search_number & SEARCH_NUMBER))
                    continue;

                expand->search_number = node->search_number;
            }
            else
            {
                if ((node->search_number & SEARCH_NUMBER) == (expand->search_number & SEARCH_NUMBER) && !(expand->search_number & BLOCKED_SEARCH))
                    continue;

                if (boxes[box_number].overlap_index & LOT->block_mask)
                {
                    expand->search_number = (node->search_number | BLOCKED_SEARCH);
                }
                else
                {
                    expand->search_number = node->search_number;
                    expand->exit_box = LOT->head;
                }
            }

            if (expand->next_expansion == NO_BOX && box_number != LOT->tail)
            {
                LOT->node[LOT->tail].next_expansion = box_number;
                LOT->tail = box_number;
            }
        } while (!done);

        LOT->head = node->next_expansion;
        node->next_expansion = NO_BOX;
    }

    return TRUE;
}

// Put together all the information needed for a baddie to be bad
void CreatureAIInfo(ITEM_INFO* item, AI_INFO* info)
{
    if (item->data == NULL)
        return;

    CREATURE_INFO* creature;
    ITEM_INFO* enemy;
    OBJECT_INFO* obj;
    ROOM_INFO* r;
    short* zone, angle = 0, laraState;
    int x, y, z;

    creature = (CREATURE_INFO*)item->data;
    obj = &objects[item->object_number];

    enemy = creature->enemy;
    if (!enemy)
    {
        enemy = lara_item;
        creature->enemy = lara_item;
    }

    // if the item is poisoned, deal damage !
    if (item->poisoned)
    {
        if (!obj->undead && obj->hit_points != DONT_TARGET && !(wibble & 0x3F))
        {
            if (item->hit_points > 1)
                item->hit_points -= BADDIE_POISON_DAMAGE;
        }
    }

    zone = ground_zone[creature->LOT.zone][flip_status];

    // baddie
    r = &rooms[item->room_number];
    item->box_number = XZ_GET_SECTOR(r, item->pos.x, item->pos.z).box & BOX_NUMBER;
    info->zone_number = zone[item->box_number];
    
    // enemy
    r = &rooms[enemy->room_number];
    enemy->box_number = XZ_GET_SECTOR(r, enemy->pos.x, enemy->pos.z).box & BOX_NUMBER;
    info->enemy_zone = zone[enemy->box_number];

    // check block mask
    if (!obj->non_lot)
    {
        if (boxes[enemy->box_number].overlap_index & creature->LOT.block_mask)
            info->enemy_zone |= BLOCKED;
        else if (creature->LOT.node[item->box_number].search_number == (creature->LOT.search_number | BLOCKED_SEARCH))
            info->enemy_zone |= BLOCKED;
    }
    
    z = (enemy->pos.z + (enemy->speed * PREDICTIVE_SCALE_FACTOR * COS(enemy->pos.y_rot) >> W2V_SHIFT)) - (item->pos.z + (obj->pivot_length * COS(item->pos.y_rot) >> W2V_SHIFT));
    x = (enemy->pos.x + (enemy->speed * PREDICTIVE_SCALE_FACTOR * SIN(enemy->pos.y_rot) >> W2V_SHIFT)) - (item->pos.x + (obj->pivot_length * SIN(item->pos.y_rot) >> W2V_SHIFT));
    y = item->pos.y - enemy->pos.y;
    angle = phd_atan(z, x);

    if (z > 32000 || z < -32000 || x > 32000 || x < -32000)
    {
        info->distance = 0x7FFFFFFF;
    }
    else
    {
        if (creature->enemy)
            info->distance = SQUARE(z) + SQUARE(x);
        else
            info->distance = 0x7FFFFFFF;
    }

    info->angle = angle - item->pos.y_rot;
    info->enemy_facing = 0x8000 + angle - enemy->pos.y_rot;
    
    x = abs(x);
    z = abs(z);

    if (enemy == lara_item)
    {
        laraState = lara_item->state_current;
        if (laraState == 71 &&
            laraState == 72 &&
            laraState == 80 &&
            laraState == 81 &&
            laraState == 84 &&
            laraState == 85)
        {
            y -= STEPUP_HEIGHT;
        }
    }

    if (x > z)
        info->x_angle = phd_atan(x + (z >> 1), y);
    else
        info->x_angle = phd_atan(z + (x >> 1), y);

    info->ahead = (info->angle > -ANGLE(90) && info->angle < ANGLE(90));
    // TODO: change STEP_L*2 to a think like item->attack_distance
    info->bite = (info->ahead && enemy->hit_points > 0 && abs(enemy->pos.y - item->pos.y) <= STEP_L*2);
}

// Set target to random position in box
void TargetBox(LOT_INFO* LOT, short box_number)
{
    BOX_INFO* box;

    box_number &= BOX_NUMBER;
    box = &boxes[box_number];
    LOT->target.z = (box->left << WALL_SHIFT) + GetRandomControl() * ((box->right - box->left - 1) >> (15 - WALL_SHIFT)) + WALL_L / 2;
    LOT->target.x = (box->top << WALL_SHIFT) + GetRandomControl() * ((box->bottom - box->top - 1) >> (15 - WALL_SHIFT)) + WALL_L / 2;
    LOT->required_box = box_number;

    if (LOT->fly == NO_FLY)
        LOT->target.y = box->height;
    else
        LOT->target.y = box->height - STEPUP_HEIGHT;
}

// Valid box cannot be in opposite quad to baddie
BOOL EscapeBox(ITEM_INFO *item, ITEM_INFO *enemy, short box_number)
{
    BOX_INFO* box;
    int x, z;

    box = &boxes[box_number];
    x = enemy->pos.x - ((box->top + box->bottom) << (WALL_SHIFT - 1));
    z = enemy->pos.z - ((box->left + box->right) << (WALL_SHIFT - 1));

    // Is box far enough from enemy to be escape type?
    if (x > -ESCAPE_DIST && x < ESCAPE_DIST && z > -ESCAPE_DIST && z < ESCAPE_DIST)
        return FALSE;

    // Don't go to opposite quad to one we're in (i.e. via enemy)
    if ((z > 0) ^ (item->pos.z > enemy->pos.z) &&
        (x > 0) ^ (item->pos.x > enemy->pos.x))
        return FALSE;

    return TRUE;
}

// Valid box is one in same zone (not blocked), but one that item is not currently in
BOOL ValidBox(ITEM_INFO* item, short zone_number, short box_number)
{
    CREATURE_INFO* creature;
    BOX_INFO* box;
    short* zone;
    int x, z;

    creature = (CREATURE_INFO*)item->data;
    zone = ground_zone[creature->LOT.zone][flip_status];

    // Zone may be BLOCKED during LOT search
    if (creature->LOT.fly == NO_FLY && zone[box_number] != zone_number)
        return FALSE;

    box = &boxes[box_number];
    if (creature->LOT.block_mask & box->overlap_index)
        return FALSE;

    x = item->pos.x;
    z = item->pos.z;

    if (z > (box->left << WALL_SHIFT) && z < (box->right  << WALL_SHIFT) &&
        x > (box->top  << WALL_SHIFT) && x < (box->bottom << WALL_SHIFT))
        return FALSE;

    return TRUE;
}

// Use creatures position and status to decide its mood, and thus its target
void GetCreatureMood(ITEM_INFO* item, AI_INFO* info, int violent)
{
    if (item->data == NULL)
        return;

    CREATURE_INFO* creature;
    LOT_INFO* LOT;
    ITEM_INFO* enemy;
    MOOD_TYPE old_mood;

    creature = (CREATURE_INFO*)item->data;
    LOT = &creature->LOT;
    enemy = creature->enemy;

    // Clear required box if BLOCKED
    if (LOT->node[item->box_number].search_number == (LOT->search_number | BLOCKED_SEARCH))
        LOT->required_box = NO_BOX;

    if (creature->mood != ATTACK_MOOD && LOT->required_box != NO_BOX)
    {
        // If within box target, clear required box and become BORED
        if (!ValidBox(item, info->zone_number, LOT->target_box))
        {
            // Creatures will run away until you enter their zone
            if (SameZone(creature, creature->enemy))
                creature->mood = BORED_MOOD;
            LOT->required_box = NO_BOX;
        }
    }

    // First decide mood
    if (!enemy)
        creature->mood = BORED_MOOD;
    else if (enemy->hit_points <= 0 && enemy == lara_item)
        creature->mood = BORED_MOOD;
    
    // Remember current mood to see if any mood swing occurs
    old_mood = creature->mood;

    // VIOLENT creature mood. Basically ATTACK if enemy in same zone, STALK if not, and
    // only run away if she shoots while cannot ATTACK
    if (violent)
    {
        switch (old_mood)
        {
            case BORED_MOOD:
            case STALK_MOOD:
                // If enemy leaves zone, become BORED
                if (SameZone(creature, creature->enemy))
                    creature->mood = ATTACK_MOOD;
                else if (item->hit_status)
                    creature->mood = ESCAPE_MOOD;
                break;
            case ATTACK_MOOD:
                // If same zone, ATTACK. If not in same zone and shot at, ESCAPE
                if (!SameZone(creature, creature->enemy))
                    creature->mood = BORED_MOOD;
                break;
            case ESCAPE_MOOD:
                // If enemy is in same zone ATTACK
                if (SameZone(creature, creature->enemy))
                    creature->mood = ATTACK_MOOD;
                break;
        }
    }
    // TIMID creature mood. Basically, run away if shot at, and STALK to attack position
    else
    {
        switch (old_mood)
        {
            case BORED_MOOD:
            case STALK_MOOD:
                // If shot by enemy, ESCAPE. Else if same zone as enemy and close enough, ATTACK
                if (creature->alerted && !SameZone(creature, creature->enemy))
                {
                    if (info->distance > STALK_DIST)
                        creature->mood = STALK_MOOD;
                    else
                        creature->mood = BORED_MOOD;
                }
                else if (SameZone(creature, creature->enemy))
                {
                    if (info->distance < ATTACK_RANGE || old_mood == STALK_MOOD && LOT->required_box == NO_BOX)
                        creature->mood = ATTACK_MOOD;
                    else
                        creature->mood = STALK_MOOD;
                }
                break;
            case ATTACK_MOOD:
                // If enemy shoots ESCAPE. If enemy leaves zone, become BORED
                if (item->hit_status && (GetRandomControl() < ESCAPE_CHANCE || !SameZone(creature, creature->enemy)))
                    creature->mood = STALK_MOOD;
                else if (!SameZone(creature, creature->enemy) && info->distance > BORED_DIST)
                    creature->mood = BORED_MOOD;
                break;
            case ESCAPE_MOOD:
                // May recover in ESCAPE_MOOD target box selection if can find suitable STALK
                if (SameZone(creature, creature->enemy) && GetRandomControl() < RECOVER_CHANCE)
                    creature->mood = STALK_MOOD;
                break;
        }
    }

    // Any change of mood results in required box being undefined
    if (old_mood != creature->mood)
    {
        // Change from ATTACK mood means that need to retarget away from Lara's old position
        // (which may be unreachable if right near the edge of a block)
        if (old_mood == ATTACK_MOOD)
            TargetBox(LOT, LOT->target_box);

        LOT->required_box = NO_BOX;
    }
}

// Use creatures position and status to decide its mood, and thus its target
void CreatureMood(ITEM_INFO* item, AI_INFO* info, int violent)
{
    if (item->data == NULL)
        return;

    CREATURE_INFO* creature;
    LOT_INFO* LOT;
    ITEM_INFO* enemy;
    short box_number;
    short* bounds;

    creature = (CREATURE_INFO*)item->data;
    LOT = &creature->LOT;
    enemy = creature->enemy;

    // Selected mood, so now select target (which may change mood anyway)
    switch (creature->mood)
    {
    case BORED_MOOD:
        // Get a random box and see if same zone; if so, only except it if suitable
        // for STALK mode, or need a new target
        box_number = LOT->node[creature->LOT.zone_count * GetRandomControl() >> 15].box_number;
        if (ValidBox(item, info->zone_number, box_number))
        {
            if (StalkBox(item, enemy, box_number) && enemy != NULL && enemy->hit_points > 0)
            {
                TargetBox(LOT, box_number);
                creature->mood = BORED_MOOD;
            }
            else if (LOT->required_box == NO_BOX)
            {
                TargetBox(LOT, box_number);
            }
        }
        break;
    case ATTACK_MOOD:
        // Enemy position is target
        LOT->target.x = enemy->pos.x;
        LOT->target.y = enemy->pos.y;
        LOT->target.z = enemy->pos.z;
        LOT->required_box = enemy->box_number;

        // Target for flying creatures
        if (LOT->fly == NO_FLY && lara.water_status == LARA_ABOVEWATER)
        {
            bounds = GetBestFrame(enemy);
            LOT->target.y += bounds[2];
        }
        break;
    case ESCAPE_MOOD:
        // If waiting for a valid box, and this is it, use it
        box_number = LOT->node[creature->LOT.zone_count * GetRandomControl() >> 15].box_number;
        if (ValidBox(item, info->zone_number, box_number) && LOT->required_box == NO_BOX)
        {
            if (EscapeBox(item, enemy, box_number))
            {
                TargetBox(LOT, box_number);
            }
            else if (SameZone(creature, enemy) && StalkBox(item, enemy, box_number) && !violent)
            {
                // If TIMID and running away while enemy in same zone, then switch to STALK when get happy
                TargetBox(LOT, box_number);
                creature->mood = STALK_MOOD;
            }
        }
        break;
    case STALK_MOOD:
        // Only get a new box if need one or current stalk box is no longer really valid.
        // In former case, change to BORED if find box not suitable for stalking.
        if (LOT->required_box == NO_BOX || !StalkBox(item, enemy, LOT->required_box))
        {
            box_number = LOT->node[creature->LOT.zone_count * GetRandomControl() >> 15].box_number;
            if (ValidBox(item, info->zone_number, box_number))
            {
                if (StalkBox(item, enemy, box_number))
                {
                    TargetBox(LOT, box_number);
                }
                else if (LOT->required_box == NO_BOX)
                {
                    TargetBox(LOT, box_number);
                    if (!SameZone(creature, enemy))
                        creature->mood = BORED_MOOD;
                }
            }
        }
        break;
    }

    if (LOT->required_box == NO_BOX)
        TargetBox(LOT, item->box_number);

    // Got target based on mood, now calculate intermediate target to get there
    TargetType = CalculateTarget(&creature->target, item, LOT);

    creature->jump_ahead = false;
    creature->monkey_ahead = false;

    short exit_box = LOT->node[item->box_number].exit_box;
    if (exit_box != NO_BOX)
    {
        short overlapIndex = boxes[exit_box].overlap_index & OVERLAP_INDEX;
        short next_box = 0;
        
        do
        {
            overlapIndex++;
            next_box = overlaps[overlapIndex];
        } while (next_box != NO_BOX && ((next_box & BOX_END_BIT) == FALSE) && ((next_box & BOX_NUMBER) != exit_box));

        if ((next_box & BOX_NUMBER) == exit_box)
        {
            if (next_box & BOX_JUMP)
                creature->jump_ahead = true;
            if (next_box & BOX_MONKEY)
                creature->monkey_ahead = true;
        }
    }
}

// Valid box for stalking is one close to enemy and not in its forward quadrant.
// If baddie is in forward quadrant, can only go to one of enemy's side quadrants
// LATEST: Got rid of quad restriction as many baddies are shooters now
BOOL StalkBox(ITEM_INFO* item, ITEM_INFO* enemy, short box_number)
{
    if (enemy == NULL)
        return FALSE;

    BOX_INFO *box;
    int x, z;
    int x_box, z_box;
    int box_quad, enemy_quad, baddie_quad;

    box = &boxes[box_number];
    x = enemy->pos.x - ((box->top + box->bottom) << 9);
    z = enemy->pos.z - ((box->right + box->left) << 9);
    x_box = (box->bottom - box->top + 3) << WALL_SHIFT;
    z_box = (box->right - box->left + 3) << WALL_SHIFT;

    if (x > x_box || x < -x_box || z > z_box || z < -z_box)
        return FALSE;

    enemy_quad = (enemy->pos.y_rot >> W2V_SHIFT) + 2;

    if (z > 0)
        box_quad = (x > 0) ? 2 : 1;
    else
        box_quad = (x > 0) ? 3 : 0;

    if (enemy_quad == box_quad)
        return FALSE;

    if (item->pos.z > enemy->pos.z)
        baddie_quad = (item->pos.x > enemy->pos.x) ? 2 : 1;
    else
        baddie_quad = (item->pos.x > enemy->pos.x) ? 3 : 0;

    if (enemy_quad == baddie_quad && abs(enemy_quad - box_quad) == 2)
        return FALSE;

    return TRUE;
}

// Check out all other baddies in room for overlapping bounds
// return angle of turn to avoid baddy rather than 1/0
int CreatureCreature(short itemNumber)
{
    ITEM_INFO* item;
    ROOM_INFO* r;
    int x, y, z;
    int distance, xdistance, zdistance;
    short radius, y_rot;
    short link;
    int angle;

    item = &items[itemNumber];
    x = item->pos.x;
    y = item->pos.y;
    z = item->pos.z;
    y_rot = item->pos.y_rot;
    radius = objects[item->object_number].radius;

    r = &rooms[item->room_number];
    link = r->item_number;

    do
    {
        item = &items[link];

        // End the moment find this item (sort of priority)
        if (link == itemNumber)
            return 0;

        // Don't stop just cuz Lara in the way
        if (item != lara_item && item->status == FITEM_ACTIVE && item->hit_points > 0)
        {
            xdistance = abs(item->pos.x - x);
            zdistance = abs(item->pos.z - z);
            distance = (xdistance > zdistance) ? xdistance + (zdistance >> 1) : zdistance + (xdistance >> 1);

            if (distance < radius + objects[item->object_number].radius)
            {
                angle = phd_atan(item->pos.z - z, item->pos.x - x) - y_rot;
                return angle;
            }
        }

        link = item->next_item;
    }
    while (link != NO_ITEM);

    return 0;
}

// Check out whether floor position provides an obstruction by using boxes
BOOL BadFloor(int x, int y, int z, int box_height, int next_height, short roomNumber, LOT_INFO* LOT)
{
    FLOOR_INFO* floor;
    int height;

    // Get adjacent box
    floor = GetFloor(x, y, z, &roomNumber);
    if (floor->box == NO_BOX)
        return TRUE;

    if (LOT->is_jumping)
        return FALSE;

    if (boxes[floor->box].overlap_index & LOT->block_mask)
        return TRUE;

    height = boxes[floor->box].height;
    if ((box_height - height) > LOT->step || (box_height - height) < LOT->drop)
        return TRUE;

    // Don't allow creature to jump down a height lower than its next LOT box
    // (else it will fall off every ledge rather than keep to the path that leads where it wants to go)
    if ((box_height - height) < -LOT->step && height > next_height)
        return TRUE;

    if ((LOT->fly != NO_FLY) && y > (height + LOT->fly))
        return TRUE;

    return FALSE;
}

// When the creature is defeated, drop item, disable collision, cant be targeted
// and it's possible to explode it !
void CreatureDie(short itemNumber, BOOL explode)
{
    ITEM_INFO* item;
    OBJECT_INFO* obj;

    item = &items[itemNumber];
    item->hit_points = DONT_TARGET;
    item->collidable = FALSE;

    if (explode)
    {
        obj = &objects[item->object_number];
        if (obj->hit_effect == HIT_BLOOD)
            ExplodingDeath(itemNumber, NO_ITEM, 258); // enable blood sprite in body mesh
        else
            ExplodingDeath(itemNumber, NO_ITEM, 256); // no blood when exploding
        KillItem(itemNumber);
    }
    else
    {
        RemoveActiveItem(itemNumber);
    }
    
    DisableBaddieAI(itemNumber);
    item->flags |= (IFLAG_KILLED_ITEM | IFLAG_INVISIBLE);

    // drop any carried items
    SpawnPickup(item);
}

/*
int CreatureAnimation(short itemNumber, short angle, short tilt)
{
    ITEM_INFO* item = &items[itemNumber];
    if (item->data == NULL)
        return 0;

    CREATURE_INFO* creature;
    LOT_INFO* LOT;
    PHD_VECTOR old;
    FLOOR_INFO* floor;
    short* zone, roomNumber, next_box;
    short* bounds;
    short radius;
    int dy;
    int biff_angle;
    int height, next_height, box_height, x, y, z;
    int pos_x, pos_z;
    int shift_x, shift_z;
    int ceiling;

    creature = (CREATURE_INFO*)item->data;
    LOT = &creature->LOT;

    // store creature current position
    old.x = item->pos.x;
    old.y = item->pos.y;
    old.z = item->pos.z;

    // need old box height and zone
    box_height = boxes[item->box_number].height;
    zone = ground_zone[creature->LOT.zone][flip_status];

    // for non-swimming baddies, need to adjust room to one by feet for sfx
    if (!objects[item->object_number].water_creature)
    {
        roomNumber = item->room_number;
        GetFloor(item->pos.x, item->pos.y, item->pos.z, &roomNumber);
        if (roomNumber != item->room_number)
            ItemNewRoom(itemNumber, roomNumber);
    }

    AnimateItem(item);

    if (item->status == FITEM_DEACTIVATED)
    {
        CreatureDie(itemNumber, FALSE);
        return 0;
    }

    bounds = GetBoundsAccurate(item);
    y = item->pos.y + bounds[2];

    // do centre point test for collision against wall
    roomNumber = item->room_number;
    GetFloor(old.x, old.y, old.z, &roomNumber);
    floor = GetFloor(item->pos.x, item->pos.y, item->pos.z, &roomNumber);
    height = boxes[floor->box & BOX_NUMBER].height;
    next_height = 0;

    // this is one of the places where real LOT is used in CreatureAnimation
    // Need to find an alternative for non-LOT creatures
    if (!objects[item->object_number].non_lot)
    {
        next_box = LOT->node[floor->box].exit_box;
    }
    else
    {
        floor = GetFloor(item->pos.x, item->pos.y, item->pos.z, &roomNumber);
        height = boxes[floor->box].height;
        next_box = floor->box;
    }

    if (next_box != NO_BOX)
        next_height = boxes[next_box].height;
    else
        next_height = height;

    // First clamp positions that are illegal back to legal (unusual)
    if (floor->box == NO_BOX || !LOT->is_jumping && (LOT->fly == NO_FLY && zone[item->box_number] != zone[floor->box]) ||
        box_height - height > LOT->step || box_height - height < LOT->drop)
    {
        pos_x = item->pos.x >> WALL_SHIFT;
        pos_z = item->pos.z >> WALL_SHIFT;
        shift_x = old.x >> WALL_SHIFT;
        shift_z = old.z >> WALL_SHIFT;

        if (pos_x < shift_x)
            item->pos.x = old.x & (~(WALL_L - 1));
        else if (pos_x > shift_x)
            item->pos.x = old.x | (WALL_L - 1);

        if (pos_x < shift_x)
            item->pos.z = old.z & (~(WALL_L - 1));
        else if (pos_x > shift_z)
            item->pos.z = old.z | (WALL_L - 1);

        floor = GetFloor(item->pos.x, y, item->pos.z, &roomNumber);
        height = boxes[floor->box].height;

        // this is one of the places where real LOT is used in CreatureAnimation
        // Need to find an alternative for non-LOT creatures
        if (!objects[item->object_number].non_lot)
        {
            next_box = LOT->node[floor->box].exit_box;
        }
        else
        {
            floor = GetFloor(item->pos.x, item->pos.y, item->pos.z, &roomNumber);
            height = boxes[floor->box].height;
            next_box = floor->box;
        }

        if (next_box != NO_BOX)
            next_height = boxes[next_box].height;
        else
            next_height = height;
    }

    x = item->pos.x;
    z = item->pos.z;
    pos_x = x & (WALL_L - 1);
    pos_z = z & (WALL_L - 1);
    radius = objects[item->object_number].radius;
    shift_x = 0;
    shift_z = 0;

    // Check all the possible grid collision points (including diagonals) and get
    // suitable shifts to move away from bad places
    if (pos_z < radius)
    {
        // Test z first
        if (BadFloor(x, y, z - radius, height, next_height, roomNumber, LOT))
        {
            shift_z = radius - pos_z;
        }

        if (pos_x < radius)
        {
            // Test x second: if that fails, and no z shift, then (and only then) test the diagonal
            if (BadFloor(x - radius, y, z, height, next_height, roomNumber, LOT))
            {
                shift_x = radius - pos_x;
            }
            else if (!shift_z && BadFloor(x - radius, y, z - radius, height, next_height, roomNumber, LOT))
            {
                // diagonal: choose which shift based on direction of travel (both shift lead to sticking)
                if (item->pos.y_rot > -ANGLE(135) && item->pos.y_rot < ANGLE(45))
                    shift_z = radius - pos_z;
                else
                    shift_x = radius - pos_x;
            }
        }
        else if (pos_x > WALL_L - radius)
        {
            if (BadFloor(x + radius, y, z, height, next_height, roomNumber, LOT))
            {
                shift_x = WALL_L - radius - pos_x;
            }
            else if (!shift_z && BadFloor(x + radius, y, z - radius, height, next_height, roomNumber, LOT))
            {
                if (item->pos.y_rot > -ANGLE(45) && item->pos.y_rot < ANGLE(135))
                    shift_z = radius - pos_z;
                else
                    shift_x = WALL_L - radius - pos_x;
            }
        }
    }
    else if (pos_z > WALL_L - radius)
    {
        if (BadFloor(x, y, z + radius, height, next_height, roomNumber, LOT))
        {
            shift_z = WALL_L - radius - pos_z;
        }

        if (pos_x < radius)
        {
            if (BadFloor(x - radius, y, z, height, next_height, roomNumber, LOT))
            {
                shift_x = radius - pos_x;
            }
            else if (!shift_z && BadFloor(x - radius, y, z + radius, height, next_height, roomNumber, LOT))
            {
                if (item->pos.y_rot > -ANGLE(45) && item->pos.y_rot < ANGLE(135))
                    shift_x = radius - pos_x;
                else
                    shift_z = WALL_L - radius - pos_z;
            }
        }
        else if (pos_x > WALL_L - radius)
        {
            if (BadFloor(x + radius, y, z, height, next_height, roomNumber, LOT))
            {
                shift_x = WALL_L - radius - pos_x;
            }
            else if (!shift_z && BadFloor(x + radius, y, z + radius, height, next_height, roomNumber, LOT))
            {
                if (item->pos.y_rot > -ANGLE(135) && item->pos.y_rot < ANGLE(45))
                    shift_x = WALL_L - radius - pos_x;
                else
                    shift_z = WALL_L - radius - pos_z;
            }
        }
    }
    else if (pos_x < radius)
    {
        if (BadFloor(x - radius, y, z, height, next_height, roomNumber, LOT))
            shift_x = radius - pos_x;
    }
    else if (pos_x > WALL_L - radius)
    {
        if (BadFloor(x + radius, y, z, height, next_height, roomNumber, LOT))
            shift_x = WALL_L - radius - pos_x;
    }

    // update information
    item->pos.x += shift_x;
    item->pos.z += shift_z;

    // double angle of turn if hit a wall (+ get correct room number)
    if (shift_x || shift_z)
    {
        floor = GetFloor(item->pos.x, y, item->pos.z, &roomNumber);
        item->pos.y_rot += angle;
        if (tilt)
            CreatureTilt(item, (short)(tilt * 2));
    }

    // check for biff against another baddie
    if (item->speed && item->hit_points > 0)
        biff_angle = CreatureCreature(itemNumber);
    else
        biff_angle = 0;
    
    if (biff_angle)
    {
        if (abs(biff_angle) < BIFF_AVOID_TURN)
            item->pos.y_rot -= biff_angle;
        else if (biff_angle > 0)
            item->pos.y_rot -= BIFF_AVOID_TURN;
        else
            item->pos.y_rot += BIFF_AVOID_TURN;
        return TRUE;
    }

    // do vertical dimension for creature with a flying ability
    if (LOT->fly != NO_FLY)
    {
        dy = creature->target.y - item->pos.y;
        if (dy > LOT->fly)
            dy = LOT->fly;
        else if (dy < -LOT->fly)
            dy = -LOT->fly;

        // vertical collision avoidance
        height = GetHeight(floor, item->pos.x, y, item->pos.z);
        if (item->pos.y + dy > height)
        {
            if (item->pos.y > height)
            {
                item->pos.x = old.x;
                item->pos.z = old.z;
                dy = -LOT->fly;
            }
            else
            {
                dy = 0;
                item->pos.y = height;
            }
        }
        else if (!objects[item->object_number].water_creature)
        {
            floor = GetFloor(item->pos.x, y + STEP_L, item->pos.z, &roomNumber);
            if (rooms[roomNumber].flags & ROOM_WATER)
                dy = -LOT->fly;
        }
        else
        {
            // avoid bashing your head
            ceiling = GetCeiling(floor, item->pos.x, y, item->pos.z);

            if (item->pos.y + bounds[2] + dy < ceiling)
            {
                if (item->pos.y + bounds[2] < ceiling)
                {
                    item->pos.x = old.x;
                    item->pos.z = old.z;
                    dy = LOT->fly;
                }
                else
                {
                    dy = 0;
                }
            }
        }

        item->pos.y += dy;
        floor = GetFloor(item->pos.x, y, item->pos.z, &roomNumber);
        item->floor = GetHeight(floor, item->pos.x, y, item->pos.z);

        // give the baddie some x rotation
        angle = (item->speed) ? phd_atan(item->speed, -dy) : 0;
        if (angle < -MAX_X_ROT)
            angle = -MAX_X_ROT;
        else if (angle > MAX_X_ROT)
            angle = MAX_X_ROT;

        if (angle < item->pos.x_rot - ANGLE(1))
            item->pos.x_rot -= ANGLE(1);
        else if (angle > item->pos.x_rot + ANGLE(1))
            item->pos.x_rot += ANGLE(1);
        else
            item->pos.x_rot = angle;
    }
    else if (LOT->is_jumping)
    {
        floor = GetFloor(item->pos.x, item->pos.y, item->pos.z, &roomNumber);
        item->floor = GetHeight(floor, item->pos.x, item->pos.y, item->pos.z);

        if (LOT->is_monkeying)
        {
            ceiling = GetCeiling(floor, item->pos.x, y, item->pos.z);
            item->pos.y = ceiling - bounds[2];
        }
        else
        {
            if (item->pos.y > height + STEP_L)
            {
                item->pos.x = old.x;
                item->pos.y = old.y;
                item->pos.z = old.z;
            }
            else
            {
                item->pos.y = height;
            }
        }
    }
    else
    {
        // stop baddies walking through crawl spaces
        floor = GetFloor(item->pos.x, y, item->pos.z, &roomNumber);
        ceiling = GetCeiling(floor, item->pos.x, y, item->pos.z);

        if (item->pos.y + bounds[2] < ceiling)
        {
            item->pos.x = old.x;
            item->pos.y = old.y;
            item->pos.z = old.z;
        }

        floor = GetFloor(item->pos.x, item->pos.y, item->pos.z, &roomNumber);
        item->floor = GetHeight(floor, item->pos.x, item->pos.y, item->pos.z);

        // slightly smoother baddie drop movement
        if (item->pos.y > item->floor)
            item->pos.y = item->floor;
        else if (item->floor - item->pos.y > STEP_L / 4)
            item->pos.y += STEP_L / 4;
        else if (item->pos.y < item->floor)
            item->pos.y = item->floor;

        item->pos.x_rot = 0;
    }

    // get room slightly above feet in case in shallow water, else baddie drawn blue tinted
    roomNumber = item->room_number;
    if (!objects[item->object_number].water_creature)
    {
        GetFloor(item->pos.x, item->pos.y - (STEP_L * 2), item->pos.z, &roomNumber);
        // if this room is water filled, then baddie has just drowned
        if (rooms[roomNumber].flags & ROOM_WATER)
            item->hit_points = 0;
    }

    if (item->room_number != roomNumber)
        ItemNewRoom(itemNumber, roomNumber);

    return TRUE;
}

// Calculate turning circle required to reach target and if this is less than twice
// the maximum turn rate then turn quickly
short CreatureTurn(ITEM_INFO* item, short maximum_turn)
{
    if (item->data == NULL || maximum_turn == 0)
        return 0;

    CREATURE_INFO* creature;
    int x, z;
    int range;
    short angle;

    creature = (CREATURE_INFO*)item->data;
    x = creature->target.x - item->pos.x;
    z = creature->target.z - item->pos.z;
    angle = phd_atan(z, x) - item->pos.y_rot;
    range = (item->speed << W2V_SHIFT) / maximum_turn;

    if ((angle > 0x4000 || angle < -0x4000) && SQUARE(x) + SQUARE(z) < SQUARE(range))
        maximum_turn >>= 1;

    CustABS(angle, maximum_turn);

    item->pos.y_rot += angle;
    return angle;
}

void CreatureTilt(ITEM_INFO* item, short angle)
{
    short ang = (angle << 2) - item->pos.z_rot;

    CustABS(ang, MAX_TILT);

    item->pos.z_rot += ang;
}

void CreatureJoint(ITEM_INFO* item, short joint, short required)
{
    if (item->data == NULL)
        return;

    CREATURE_INFO* creature = (CREATURE_INFO*)item->data;
    short changes;

    // limit rate of change
    changes = required - creature->joint_rotation[joint];
    CustABS(changes, MAX_JOINT_CHANGE);

    creature->joint_rotation[joint] += changes;

    CustABS(creature->joint_rotation[joint], JOINT_ARC);
}

// Make dead underwater creatures float to the surface
void CreatureFloat(short itemNumber)
{
    ITEM_INFO* item;
    FLOOR_INFO* floor;
    int water_level;
    short roomNumber;

    item = &items[itemNumber];
    item->hit_points = DONT_TARGET;
    item->pos.x_rot = 0;

    water_level = GetWaterHeight(item->pos.x, item->pos.y, item->pos.z, item->room_number);
    if (item->pos.y > water_level)
        item->pos.y -= CREATURE_FLOAT_SPEED;
    else if (item->pos.y < water_level)
        item->pos.y = water_level;

    AnimateItem(item);

    roomNumber = item->room_number;
    floor = GetFloor(item->pos.x, item->pos.y, item->pos.z, &roomNumber);
    item->floor = GetHeight(floor, item->pos.x, item->pos.y, item->pos.z);
    if (roomNumber != item->room_number)
        ItemNewRoom(itemNumber, roomNumber);

    if (item->pos.y <= water_level && item->current_frame == anims[item->current_anim].frame_base)
    {
        item->collidable = FALSE;
        item->status = FITEM_DEACTIVATED;
        DisableBaddieAI(itemNumber);
        RemoveActiveItem(itemNumber);
        item->after_death = TRUE;
    }
}

// Keep creature underwater at minimum 'depth' and remove x_rot whilst they swim at that depth
void CreatureUnderwater(ITEM_INFO* item, int depth)
{
    FLOOR_INFO* floor;
    int height;
    int water_level;
    short roomNumber;

    water_level = GetWaterHeight(item->pos.x, item->pos.y, item->pos.z, item->room_number);
    if (item->pos.y < water_level + depth)
    {
        roomNumber = item->room_number;
        floor = GetFloor(item->pos.x, item->pos.y, item->pos.z, &roomNumber);
        height = GetHeight(floor, item->pos.x, item->pos.y, item->pos.z);

        item->pos.y = water_level + depth;
        if (water_level + depth > height)
            item->pos.y = height;

        if (item->pos.x_rot > ANGLE(2))
            item->pos.x_rot -= ANGLE(2);
        else if (item->pos.x_rot > 0)
            item->pos.x_rot = 0;
    }
}

short CreatureEffect(ITEM_INFO* item, BITE_INFO* bite, lpEffect effect)
{
    PHD_VECTOR pos;
    pos.x = bite->x;
    pos.y = bite->y;
    pos.z = bite->z;
    GetJointAbsPosition(item, &pos, bite->mesh);
    return effect(pos.x, pos.y, pos.z, item->speed, item->pos.y_rot, item->room_number);
}

short CreatureEffectAlternate(ITEM_INFO* item, BITE_INFO* bite, short speed, short y_rot, lpEffect effect)
{
    PHD_VECTOR pos;
    pos.x = bite->x;
    pos.y = bite->y;
    pos.z = bite->z;
    GetJointAbsPosition(item, &pos, bite->mesh);
    return effect(pos.x, pos.y, pos.z, speed, y_rot, item->room_number);
}

// Allows creatures to vault up onto high steps.
// Need to pass the max height allowable, where 2=half block, 3=3/4 block, 4=full block,
// and it will return the height that is vaulted (or 0)
int CreatureVault(short itemNumber, short angle, int vault, int shift)
{
    ITEM_INFO* item;
    int y, xx, yy, x_floor, y_floor;
    int vault_result = 0;
    short roomNumber;

    item = &items[itemNumber];
    // Remember current position of creature in case it needs to vault up
    xx = item->pos.z >> WALL_SHIFT;
    yy = item->pos.x >> WALL_SHIFT;
    y = item->pos.y;
    roomNumber = item->room_number;

    CreatureAnimation(itemNumber, angle, 0);

    // Check for double block vault situation (1.5 blocks or more height change)
    if (item->floor > y + STEP(9))
        vault_result = -4;
    else if (item->floor > y + STEP(7) && item->object_number != VON_CROY)
        vault_result = -3;
    else if (item->floor > y + STEP(5) && item->object_number != VON_CROY)
        vault_result = -2;
    else if (item->pos.y > y - STEP(3))
        return 0;
    else if (item->pos.y > y - STEP(5))
        vault_result = 2;
    else if (item->pos.y > y - STEP(7))
        vault_result = 3;
    else if (item->pos.y > y - STEP(9))
        vault_result = 4;

    // Jump creature to correct position
    x_floor = item->pos.z >> WALL_SHIFT;
    y_floor = item->pos.x >> WALL_SHIFT;
    if (xx == x_floor)
    {
        if (yy == y_floor)
            return 0;

        if (yy < y_floor)
        {
            item->pos.x = (y_floor << WALL_SHIFT) - shift;
            item->pos.y_rot = 0x4000;
        }
        else
        {
            item->pos.x = (yy << WALL_SHIFT) + shift;
            item->pos.y_rot = -0x4000;
        }
    }
    else if (yy == y_floor)
    {
        if (xx < x_floor)
        {
            item->pos.z = (x_floor << WALL_SHIFT) - shift;
            item->pos.y_rot = 0;
        }
        else
        {
            item->pos.z = (xx << WALL_SHIFT) + shift;
            item->pos.y_rot = -0x8000;
        }
    }
    // else a diagonal collision - never seems to happen...

    item->pos.y = y;
    item->floor = y;
    if (roomNumber != item->room_number)
        ItemNewRoom(itemNumber, roomNumber);

    return vault_result;
}

void CreatureKill(ITEM_INFO* item, int kill_anim, int kill_state, int kill_lara_anim)
{
    item->current_anim = objects[item->object_number].anim_index + kill_anim;
    item->current_frame = anims[item->current_anim].frame_base;
    item->state_current = kill_state;
    
    lara_item->current_anim = kill_lara_anim;
    lara_item->current_frame = anims[lara_item->current_anim].frame_base;
    lara_item->state_current = STATE_LARA_DEATH;
    lara_item->state_next = STATE_LARA_DEATH;
    lara_item->pos.x = item->pos.x;
    lara_item->pos.y = item->pos.y;
    lara_item->pos.z = item->pos.z;
    lara_item->pos.y_rot = item->pos.y_rot;
    lara_item->pos.x_rot = item->pos.x_rot;
    lara_item->pos.z_rot = item->pos.z_rot;
    lara_item->fallspeed = 0;
    lara_item->gravity_status = FALSE;
    lara_item->speed = 0;

    if (item->room_number != lara_item->room_number)
        ItemNewRoom(lara.item_number, item->room_number);

    AnimateItem(lara_item);

    lara.gun_status = 1;
    lara.gun_type = LG_UNARMED;
    lara.hit_direction = -1;
    lara.air = -1;

    camera.src.room_number = lara_item->room_number;
    camera.mike_pos.x = item->pos.x + (SIN(item->pos.y_rot) << WALL_SHIFT >> W2V_SHIFT);
    camera.mike_pos.y = item->pos.y - WALL_L;
    camera.mike_pos.z = item->pos.z + (COS(item->pos.y_rot) << WALL_SHIFT >> W2V_SHIFT);
    camera.mike_pos_room = item->room_number;
    UseForcedFixedCamera = TRUE;
}

void AlertAllGuards(short itemNumber)
{
    ITEM_INFO* item, *target;
    CREATURE_INFO* cinfo;

    item = &items[itemNumber];
    cinfo = baddie_slots;

    for (int slot = 0; slot < NUM_SLOTS; slot++, cinfo++)
    {
        if (cinfo->item_number != NO_ITEM)
        {
            target = &items[cinfo->item_number];
            if (item->object_number == target->object_number && target->status == FITEM_ACTIVE)
                cinfo->alerted = TRUE;
        }
    }
}

void AlertNearbyGuards(ITEM_INFO* item)
{
    ITEM_INFO *target;
    CREATURE_INFO* cinfo;
    int x, y, z, range;

    cinfo = baddie_slots;
    for (int slot = 0; slot < NUM_SLOTS; slot++, cinfo++)
    {
        if (cinfo->item_number != NO_ITEM)
        {
            target = &items[cinfo->item_number];
            if (target->room_number == item->room_number)
            {
                cinfo->alerted = TRUE;
                continue;
            }

            x = (target->pos.x - item->pos.x) >> 6;
            y = (target->pos.y - item->pos.y) >> 6;
            z = (target->pos.z - item->pos.z) >> 6;
            range = SQUARE(x) + SQUARE(y) + SQUARE(z);

            if (range < GUNFIRE_ALERT_RADIUS)
                cinfo->alerted = TRUE;
        }
    }
}

short AIGuard(CREATURE_INFO* creature)
{
    ITEM_INFO* item;
    int random;
    
    item = &items[creature->item_number];
    if (CHK_ANY(item->ai_bits, MODIFY))
        return 0;
    
    random = GetRandomControl();
    if (random < 0x100)
    {
        creature->head_left = TRUE;
        creature->head_right = TRUE;
    }
    else if (random < 0x180)
    {
        creature->head_left = TRUE;
        creature->head_right = FALSE;
    }
    else if (random < 0x200)
    {
        creature->head_left = FALSE;
        creature->head_right = TRUE;
    }

    if (creature->head_left && creature->head_right)
        return 0;
    else if (creature->head_left)
        return -ANGLE(90);
    else if (creature->head_right)
        return ANGLE(90);

    return 0;
}

void GetAITarget(CREATURE_INFO* creature)
{
    ITEM_INFO* item, *enemy;
    FLOOR_INFO* floor;
    char ai_bits;
    short enemy_object;
    
    enemy = creature->enemy;
    if (enemy)
        enemy_object = enemy->object_number;
    else
        enemy_object = NO_ITEM;

    item = &items[creature->item_number];
    ai_bits = item->ai_bits;

    if (CHK_ANY(ai_bits, GUARD))
    {
        creature->enemy = lara_item;
        if (creature->alerted)
        {
            item->ai_bits &= ~(GUARD);
            if (CHK_ANY(ai_bits, AMBUSH))
                item->ai_bits |= MODIFY;
        }
    }
    else if (CHK_ANY(ai_bits, AMBUSH))
    {
        //if (item->object_number == MONKEY)
        //    return;
        if (CHK_NOP(ai_bits, MODIFY) && !creature->hurt_by_lara)
        {
            creature->enemy = lara_item;
        }
        else if (enemy_object != AI_AMBUSH)
        {
            FindAITargetObject(creature, AI_AMBUSH);
        }
        else if (CHECK_REACHED_GOAL(item, enemy, REACHED_GOAL_RADIUS))
        {
            floor = GetFloor(enemy->pos.x, enemy->pos.y, enemy->pos.z, &enemy->room_number);
            GetHeight(floor, enemy->pos.x, enemy->pos.y, enemy->pos.z);
            TestTriggers(trigger_index, TRUE, 0);

            creature->reached_goal = TRUE; // attack now !
            creature->enemy = lara_item;
            item->ai_bits &= ~(AMBUSH);
            item->ai_bits &= ~(MODIFY);
            item->ai_bits |= GUARD;
            creature->alerted = FALSE;
        }
    }
    else if (CHK_ANY(ai_bits, PATROL1))
    {
        if (creature->alerted || creature->hurt_by_lara)
        {
            item->ai_bits &= ~(PATROL1);
            if (CHK_ANY(ai_bits, AMBUSH))
                item->ai_bits |= MODIFY;
        }
        else if (!creature->patrol2 && enemy_object != AI_PATROL1)
        {
            FindAITargetObject(creature, AI_PATROL1);
        }
        else if (creature->patrol2 && enemy_object != AI_PATROL2)
        {
            FindAITargetObject(creature, AI_PATROL2);
        }
        else if (CHECK_REACHED_GOAL(item, enemy, REACHED_GOAL_RADIUS))
        {
            floor = GetFloor(enemy->pos.x, enemy->pos.y, enemy->pos.z, &enemy->room_number);
            GetHeight(floor, enemy->pos.x, enemy->pos.y, enemy->pos.z);
            TestTriggers(trigger_index, TRUE, 0);

            creature->patrol2 &= ~creature->patrol2;
        }
    }
    else if (CHK_ANY(ai_bits, FOLLOW))
    {
        if (creature->hurt_by_lara)
        {
            creature->enemy = lara_item;
            creature->alerted = TRUE;
            item->ai_bits &= ~(FOLLOW);
        }
        else if (item->hit_status)
        {
            item->ai_bits &= ~(FOLLOW);
        }
        else if (enemy_object != AI_FOLLOW)
        {
            FindAITargetObject(creature, AI_FOLLOW);
        }
        else if (CHECK_REACHED_GOAL(item, enemy, REACHED_GOAL_RADIUS))
        {
            creature->reached_goal = TRUE;
            item->ai_bits &= ~(FOLLOW);
        }
    }
    // TODO: implement monkey search item ! (probably AI_SEARCH (ocb object_number needed ?) ?)
    // else if (item->object_number == MONKEY && item->carried_item == NO_ITEM)
}

void FindAITargetObject(CREATURE_INFO* creature, short objectNumber)
{
    ITEM_INFO* item, *ai_target;
    ROOM_INFO* r;
    AIOBJECT* ai;
    short* zone;
    
    item = &items[creature->item_number];
    ai = AIObjects;

    for (int i = 0; i < nAIObjects; i++, ai++)
    {
        if (ai->object_number == objectNumber && ai->trigger_flags == item->reserved_4 && ai->room_number != NO_ROOM)
        {
            zone = ground_zone[creature->LOT.zone][flip_status];

            r = &rooms[item->room_number];
            item->box_number = XZ_GET_SECTOR(r, item->pos.x, item->pos.z).box & BOX_NUMBER;

            r = &rooms[ai->room_number];
            ai->box_number = XZ_GET_SECTOR(r, ai->x, ai->z).box & BOX_NUMBER;

            if (zone[item->box_number] == zone[ai->box_number])
                break;
        }
    }

    ai_target = &creature->ai_target;
    ai_target->object_number = ai->object_number;
    ai_target->room_number = ai->room_number;
    ai_target->pos.x = ai->x;
    ai_target->pos.y = ai->y;
    ai_target->pos.z = ai->z;
    ai_target->pos.y_rot = ai->y_rot;
    ai_target->flags = ai->flags;
    ai_target->ocb_bits = ai->trigger_flags;
    ai_target->box_number = ai->box_number;
    if (!ai_target->collidable)
    {
        creature->ai_target.pos.x += SINN(creature->ai_target.pos.y_rot) << WALL_SHIFT >> W2V_SHIFT;
        creature->ai_target.pos.z += COSN(creature->ai_target.pos.y_rot) << WALL_SHIFT >> W2V_SHIFT;
    }
    creature->enemy = ai_target;
}

// The creature is in the same zone as the targeted item ?
BOOL SameZone(CREATURE_INFO *creature, ITEM_INFO *target_item)
{
    ITEM_INFO* item;
    ROOM_INFO* r;
    short* zone;
    
    item = &items[creature->item_number];
    zone = ground_zone[creature->LOT.zone][flip_status];
    
    // Basic information about creature
    r = &rooms[item->room_number];
    item->box_number = XZ_GET_SECTOR(r, item->pos.x, item->pos.z).box & BOX_NUMBER;

    // Basic information about target
    r = &rooms[target_item->room_number];
    target_item->box_number = XZ_GET_SECTOR(r, target_item->pos.x, target_item->pos.z).box & BOX_NUMBER;

    // return true if the src and target are in the same zone !
    return (zone[item->box_number] == zone[target_item->box_number]);
}
*/

void injector::inject_box()
{
    /*
    this->inject(0x0043FB30, InitialiseCreature);
    this->inject(0x0043FB70, CreatureActive);
    this->inject(0x0043FBE0, CreatureAIInfo);
    this->inject(0x0043FF70, UpdateLOT);
    this->inject(0x0043FFF0, SearchLOT);
    this->inject(0x004401F0, TargetBox);
    this->inject(0x00440290, EscapeBox);
    this->inject(0x00440340, ValidBox);
    this->inject(0x004403E0, GetCreatureMood);
    this->inject(0x00440620, CreatureMood);
    this->inject(0x00440940, StalkBox);
    //this->inject(0x00440A40, CalculateTarget);   // the most difficult
    this->inject(0x00440E90, CreatureCreature);
    this->inject(0x00440FD0, BadFloor);
    this->inject(0x00441080, CreatureDie);
    //this->inject(0x00441230, CreatureAnimation); // the most difficult
    this->inject(0x00441C60, CreatureTurn);
    this->inject(0x00441EE0, CreatureTilt);
    this->inject(0x00441F20, CreatureJoint);
    this->inject(0x00441F80, CreatureFloat);
    this->inject(0x00442080, CreatureUnderwater);
    this->inject(0x00442110, CreatureEffect);
    this->inject(0x00442170, CreatureEffectAlternate);
    this->inject(0x004421D0, CreatureVault);
    this->inject(0x00442380, CreatureKill);
    this->inject(0x00442550, AlertAllGuards);
    this->inject(0x004425D0, AlertNearbyGuards);
    this->inject(0x00442670, AIGuard);
    this->inject(0x00442720, GetAITarget);
    this->inject(0x00442A60, FindAITargetObject);
    this->inject(0x00442C40, SameZone);*/
}