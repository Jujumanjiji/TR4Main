#include "framework.h"
#include "game/oldobjects.h"
#include "3dsystem/3d_gen.h"
#include "game/box.h"
#include "game/control.h"
#include "game/effect2.h"
#include "game/people.h"
#include "utils/utils.h"

enum CROCODILE_ANIM
{
    /// Land
    ACROC_IDLE,
    ACROC_START_WALKING,
    ACROC_RUNNING,
    ACROC_LEFT_WALK_TO_IDLE,
    ACROC_RIGHT_WALK_TO_IDLE,
    ACROC_WALK,
    ACROC_WALK_START,
    ACROC_HIT_BY_SHOTGUN,
    ACROC_HIT_BY_SHOTGUN_WALK,
    ACROC_HIT_BY_SHOTGUN_TO_WALK,
    ACROC_ATK,
    ACROC_LAND_DEATH,
    /// Water
    ACROC_WATER_SWIM,
    ACROC_WATER_START_BITE,
    ACROC_WATER_BITE,
    ACROC_WATER_BITE_TO_SWIM,
    ACROC_WATER_DEATH,
    /// Transition
    ACROC_SWIM_MODE,
    ACROC_LAND_MODE,
};

enum CROCODILE_STATE
{
    /// Land
    CROC_RESET = 0,
    CROC_IDLE = 1,
    CROC_RUN = 2,
    CROC_WALK = 3,  // transition from water to walk !
    CROC_HIT = 4,
    CROC_ATK = 5,
    CROC_EMPTY1 = 6,
    CROC_DIE = 7,
    /// Water
    WCROC_SWIM = 8, // transition from walk to water !
    WCROC_ATK = 9,
    WCROC_DIE = 10,
};

enum CROCODILE_VALUE
{
    CROC_DAMAGE = 120,
    CROC_TOUCHBIT1 = (STEP_L*2),
    CROC_TOUCHBIT2 = (STEP_L*3),
    CROC_VISIBILITY_DISTANCE = SQUARE(WALL_L * 5),
    CROC_DETECT_RANGE = SQUARE(STEP_L * 6),
    CROC_ATTACK_RANGE = SQUARE(STEP_L * 2),
    CROC_RUN_DISTANCE = SQUARE(WALL_L),
    CROC_ANGLE = ANGLE(3),
};

static BITE_INFO crocBite = { 0, -156, 500, 9 };

void SwitchCrocodileLOT(CREATURE_INFO* creature, ZONE_TYPE newZone)
{
    switch (newZone)
    {
        case ZONE_WATER:
            if (creature->LOT.fly != 16) // to stop updating if it's already 16
            {
                creature->LOT.step = (WALL_L * 20);
                creature->LOT.drop = -(WALL_L * 20);
                creature->LOT.fly = 16;
            }
            break;
        case ZONE_BASIC:
            if (creature->LOT.fly != 0) // to stop updating if it's already 0
            {
                creature->LOT.step = STEP_L;
                creature->LOT.drop = -STEP_L;
                creature->LOT.fly = 0;
            }
            break;
    }
}

void CalcCrocodilePosToFloor(ITEM_INFO* item)
{
    FLOOR_INFO* floor, *floor2;
    int x, y, z;
    int x2, y2, z2;
    int height, height2;
    int at;
    short roomNumber = 0, roomNumber2 = 0;
    short rotX;

    x = item->pos.x + ((SIN(item->pos.y_rot) << WALL_SHIFT) >> W2V_SHIFT);
    y = item->pos.y;
    z = item->pos.z + ((COS(item->pos.y_rot) << WALL_SHIFT) >> W2V_SHIFT);
    roomNumber = item->room_number;
    floor = GetFloor(x, y, z, &roomNumber);
    height = GetHeight(floor, x, y, z);
    if (abs(item->pos.y - height) > 512)
        height = y;
    x2 = item->pos.x + ((SIN(item->pos.y_rot) << WALL_SHIFT) >> W2V_SHIFT);
    y2 = item->pos.y;
    z2 = item->pos.z + ((COS(item->pos.y_rot) << WALL_SHIFT) >> W2V_SHIFT);
    roomNumber2 = item->room_number;
    floor2 = GetFloor(x2, y2, z2, &roomNumber2);
    height2 = GetHeight(floor2, x2, y2, z2);
    if (abs(item->pos.y - height2) > 512)
        height2 = y2;
    at = phd_atan(2048, height2 - height);

    rotX = item->pos.x_rot;
    if (abs(at) >= 256)
    {
        if (at < rotX)
            item->pos.x_rot -= 256;
        else if (at > rotX)
            item->pos.x_rot += 256;
    }
    else if (item->state_current < WCROC_SWIM)
    {
        item->pos.x_rot = at;
    }
}

void SwapCrocodileType(ITEM_INFO* item, CREATURE_INFO* creature)
{
    short roomNumber = item->room_number;
    FLOOR_INFO* floor = GetFloor(item->pos.x, item->pos.y, item->pos.z, &roomNumber);

    if (RWATER(roomNumber))
    {
        creature->LOT.fly = 16;
        if (item->state_current == CROC_RUN)
        {
            item->state_required = CROC_WALK;
            item->state_next = CROC_WALK;
        }
        else if (item->state_current == CROC_WALK)
        {
            item->state_required = WCROC_SWIM;
            item->state_next = WCROC_SWIM;
        }
        
        SwitchCrocodileLOT(creature, ZONE_WATER);
        CreatureUnderwater(item, STEP_L);
    }
    else if (RNOT_WATER(roomNumber))
    {
        creature->LOT.fly = 0;
        if (item->state_current == WCROC_SWIM)
        {
            item->state_next = CROC_WALK;
        }
        else if (item->current_anim != objects[item->object_number].anim_index + ACROC_LAND_MODE)
        {
            SwitchCrocodileLOT(creature, ZONE_BASIC);
            CreatureUnderwater(item, NULL);
        }
    }
}

void InitialiseCrocodile(short itemNumber)
{
    ITEM_INFO* item;
    OBJECT_INFO* obj;

    item = &items[itemNumber];
    obj = &objects[item->object_number];

    InitialiseCreature(itemNumber);

    // crocodile is in water ?
    if (RWATER(item->room_number))
    {
        item->current_anim = obj->anim_index + ACROC_WATER_SWIM;
        item->current_frame = anims[item->current_anim].frame_base;
        item->state_next = WCROC_SWIM;
        item->state_current = WCROC_SWIM;
    }
    // crocodile is on land ?
    else
    {
        item->current_anim = obj->anim_index + ACROC_IDLE;
        item->current_frame = anims[item->current_anim].frame_base;
        item->state_next = CROC_IDLE;
        item->state_current = CROC_IDLE;
    }
}

void CrocodileControl(short itemNumber)
{
    // check if the creature is enabled
    if (!CreatureActive(itemNumber))
        return;

    ITEM_INFO* item;
    CREATURE_INFO* croc;
    OBJECT_INFO* obj;
    AI_INFO info;
    short guard;
    short angle, head_y;

    item = &items[itemNumber];
    if (item->data == NULL) // crash proof
        return;
    croc = (CREATURE_INFO*)item->data;
    obj = &objects[item->object_number];
    guard = 0;
    angle = 0;
    head_y = 0;

    if (item->hit_points <= 0)
    {
        if (item->state_current != CROC_DIE && item->state_current != WCROC_DIE)
        {
            if (RWATER(item->room_number))
            {
                item->current_anim = obj->anim_index + ACROC_WATER_DEATH;
                item->current_frame = anims[item->current_anim].frame_base;
                item->state_current = WCROC_DIE;
                item->state_next = WCROC_DIE;

            }
            else
            {
                item->current_anim = obj->anim_index + ACROC_LAND_DEATH;
                item->current_frame = anims[item->current_anim].frame_base;
                item->state_current = CROC_DIE;
                item->state_next = CROC_DIE;
            }
        }

        if (RWATER(item->room_number))
        {
            CreatureFloat(itemNumber);
            return;
        }
    }
    else
    {
        if (item->ai_bits)
            GetAITarget(croc);
        else if (croc->hurt_by_lara)
            croc->enemy = lara_item;

        CreatureAIInfo(item, &info);
        GetCreatureMood(item, &info, VIOLENT);
        CreatureMood(item, &info, VIOLENT);
        angle = CreatureTurn(item, croc->maximum_turn);

        if (CHK_NOP(item->ai_bits, GUARD) && info.ahead) // no guard ? then look at lara !
            head_y = info.angle;

        if (item->hit_status || info.distance < CROC_DETECT_RANGE || (TargetVisible(item, &info) && info.distance < CROC_VISIBILITY_DISTANCE))
        {
            if (!croc->alerted)
                croc->alerted = true;
            AlertAllGuards(itemNumber);
        }

        guard = 4 * angle;
        switch (item->state_current)
        {
            case CROC_IDLE:
                croc->maximum_turn = 0;

                if (item->ai_bits & GUARD)
                {
                    item->state_next = CROC_IDLE;
                    item->item_flags[0] = item->item_flags[1] + guard;

                    if (!(GetRandomControl() & 0x1F))
                    {
                        if (GetRandomControl() & 1)
                            item->item_flags[1] = 0;
                        else
                            item->item_flags[1] = (GetRandomControl() & 1) != 0 ? 12 : -12;
                    }

                    if (item->item_flags[0] <= WALL_L)
                    {
                        if (item->item_flags[0] < -WALL_L)
                            item->item_flags[0] = -WALL_L;
                    }
                    else
                    {
                        item->item_flags[0] = WALL_L;
                    }
                }
                else if (info.bite && info.distance < CROC_ATTACK_RANGE)
                {
                    item->state_next = CROC_ATK;
                }
                else
                {
                    if (!info.ahead || info.distance >= CROC_RUN_DISTANCE)
                        item->state_next = CROC_RUN;
                    else
                        item->state_next = CROC_WALK;
                }
                break;
            case CROC_WALK:
                croc->maximum_turn = CROC_ANGLE;
                croc->LOT.step = CLICK(1);
                croc->LOT.drop = -CLICK(1);

                if (item->state_required)
                    item->state_next = item->state_required;
                else if (info.bite && info.distance < CROC_ATTACK_RANGE)
                    item->state_next = CROC_IDLE;
                else if (!info.bite || info.distance > CROC_RUN_DISTANCE)
                    item->state_next = CROC_RUN;
                break;
            case CROC_RUN:
                croc->maximum_turn = CROC_ANGLE;

                if (RWATER(item->room_number))
                {
                    item->state_required = CROC_WALK;
                    item->state_next = CROC_WALK;
                }
                else if (info.zone_number != info.enemy_zone)
                {
                    croc->mood = ESCAPE_MOOD;
                }
                else if (item->state_required)
                {
                    item->state_next = item->state_required;
                }
                else
                {
                    if (info.bite && info.distance < CROC_ATTACK_RANGE)
                        item->state_next = CROC_IDLE;
                    else if (info.ahead && info.distance < CROC_RUN_DISTANCE)
                        item->state_next = CROC_WALK;
                }
                break;
            case CROC_HIT: // not in the current code !
                croc->maximum_turn = 0;

                break;
            case CROC_ATK:
                croc->maximum_turn = 0;

                if (item->current_frame == anims[item->current_anim].frame_base)
                    item->state_required = CROC_RESET;

                if (info.bite && CHK_ANY(item->touch_bits, (CROC_TOUCHBIT1 | CROC_TOUCHBIT2)))
                {
                    if (!item->state_required)
                    {
                        CreatureEffectAlternate(item, &crocBite, 10, -1, DoBloodSplat);
                        lara_item->hit_points -= CROC_DAMAGE;
                        lara_item->hit_status = TRUE;
                        item->state_required = CROC_IDLE;
                    }
                }
                else
                {
                    item->state_next = CROC_IDLE;
                }
                break;
            case WCROC_SWIM:
                croc->maximum_turn = CROC_ANGLE;
                croc->LOT.step = SECTOR(20);
                croc->LOT.drop = -SECTOR(20);

                if (item->state_required)
                    item->state_next = item->state_required;
                else if (info.bite && item->touch_bits & (CROC_TOUCHBIT1 | CROC_TOUCHBIT2))
                    item->state_next = WCROC_ATK;
                break;
            case WCROC_ATK:
                croc->maximum_turn = 0;

                if (item->current_frame == anims[item->current_anim].frame_base)
                    item->state_required = CROC_RESET;

                if (info.bite && CHK_ANY(item->touch_bits, (CROC_TOUCHBIT1 | CROC_TOUCHBIT2)))
                {
                    if (!item->state_required)
                    {
                        CreatureEffectAlternate(item, &crocBite, 10, -1, DoBloodSplat);
                        lara_item->hit_points -= CROC_DAMAGE;
                        lara_item->hit_status = TRUE;
                        item->state_required = WCROC_SWIM;
                    }
                }
                else
                {
                    item->state_next = WCROC_SWIM;
                }
                break;
        }
    }

    CreatureTilt(item, NO_TILT);

    if (CHK_NOP(item->ai_bits, GUARD) && info.ahead)
    {
        CreatureJoint(item, 0, head_y);
    }
    else
    {
        CreatureJoint(item, 0, guard);
        CreatureJoint(item, 1, guard);
        CreatureJoint(item, 2, -guard);
        CreatureJoint(item, 3, -guard);
    }

    //CalcCrocodilePosToFloor(item);
    SwapCrocodileType(item, croc);

    if (RWATER(item->room_number))
        CreatureUnderwater(item, STEP_L);

    CreatureAnimation(itemNumber, angle, NO_TILT);
}