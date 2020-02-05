#include "framework.h"
#include "../oldobjects.h"
#include "3dsystem/3d_gen.h"
#include "game/box.h"
#include "game/control.h"
#include "game/effect2.h"
#include "game/people.h"
#include "utils/utils.h"

enum BAT_ANIM
{
    ABAT_START,
    ABAT_FLY,
    ABAT_ATK,
    ABAT_FALLING,
    ABAT_HIT_FLOOR,
    ABAT_IDLE,
};

enum BAT_STATE
{
    SBAT_START = 0x1,
    SBAT_FLY = 0x2,
    SBAT_ATK = 0x3,
    SBAT_FALLING = 0x4,
    SBAT_DEATH = 0x5,
    SBAT_IDLE = 0x6,
};

enum BAT_VALUE
{
    BAT_ANGLE = ANGLE(20),
    BAT_ATTACK_RANGE = SQUARE(STEP_L),
    BAT_DAMAGE = 2,
    BAT_VISIBILITY_DISTANCE = SQUARE(WALL_L * 5),
};

static BITE_INFO batBite = { 0, 16, 45, 4 };

void InitialiseBat(short itemNumber)
{
    ITEM_INFO* item;
    OBJECT_INFO* obj;

    InitialiseCreature(itemNumber);

    item = &items[itemNumber];
    obj = &objects[item->object_number];
    
    item->current_anim = obj->anim_index + ABAT_IDLE;
    item->current_frame = anims[item->current_anim].frame_base;
    item->state_current = SBAT_IDLE;
    item->state_next = SBAT_IDLE;
}

void BatControl(short itemNumber)
{
    if (!CreatureActive(itemNumber))
        return;

    ITEM_INFO* item, *enemy;
    OBJECT_INFO* obj;
    CREATURE_INFO* bat;
    AI_INFO info;
    short angle;

    item = &items[itemNumber];
    obj = &objects[item->object_number];
    if (item->data == NULL)
        return;
    bat = (CREATURE_INFO*)item->data;
    angle = 0;

    if (item->hit_points <= 0)
    {
        if (item->pos.y >= item->floor)
        {
            item->gravity_status = FALSE;
            item->pos.y = item->floor;
            item->state_next = SBAT_DEATH;
        }
        else
        {
            item->gravity_status = TRUE;
            item->current_anim = obj->anim_index + ABAT_FALLING;
            item->current_frame = anims[item->current_anim].frame_base;
            item->state_current = SBAT_FALLING;
            item->state_next = SBAT_FALLING;
            item->speed = 0;
        }
    }
    else
    {
        if (item->ai_bits)
            GetAITarget(bat);
        // search von_croy or lara and DESTROY IT !!
        else
            bat->enemy = FoundTarget(itemNumber, item, bat, VON_CROY); // return lara or voncroy !

        CreatureAIInfo(item, &info);
        GetCreatureMood(item, &info, TIMID);
        if (bat->flags)
            bat->mood = ESCAPE_MOOD;
        CreatureMood(item, &info, TIMID);
        angle = CreatureTurn(item, BAT_ANGLE);
        enemy = bat->enemy;

        switch (item->state_current)
        {
            case SBAT_IDLE:
                if (info.distance < BAT_VISIBILITY_DISTANCE || item->hit_status || bat->hurt_by_lara)
                    item->state_next = SBAT_START;
                break;
            case SBAT_FLY:
                if (info.distance < BAT_ATTACK_RANGE || !(GetRandomControl() & 0x3F))
                    bat->flags = 0;

                if (!bat->flags)
                {
                    if (item->touch_bits || (bat->enemy != lara_item && info.distance < BAT_ATTACK_RANGE && info.ahead))
                        item->state_next = SBAT_ATK;
                }
                break;
            case SBAT_ATK:
                if (!bat->flags)
                {
                    if (item->touch_bits || (bat->enemy != lara_item && info.distance < BAT_ATTACK_RANGE && info.ahead))
                    {
                        CreatureEffect(item, &batBite, DoBloodSplat);
                        if (bat->enemy == lara_item)
                        {
                            lara_item->hit_points -= BAT_DAMAGE;
                            lara_item->hit_status = TRUE;
                        }
                        bat->flags = 1;
                    }
                    else
                    {
                        item->state_next = SBAT_FLY;
                        bat->mood = BORED_MOOD;
                    }
                }
                else
                {
                    item->current_anim = obj->anim_index + ABAT_FLY;
                    item->current_frame = anims[item->current_anim].frame_base;
                    item->state_next = SBAT_FLY;
                    item->state_current = SBAT_FLY;
                }
                break;
        }
    }

    CreatureAnimation(itemNumber, angle, NO_TILT);
}