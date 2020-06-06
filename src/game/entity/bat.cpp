#include "framework.h"
#include "bat.h"
#include "box.h"
#include "control.h"
#include "effect2.h"

enum BAT_STATE
{
    BAT_EMPTY,
    BAT_START,
    BAT_FLY,
    BAT_ATK,
    BAT_FALLING,
    BAT_DEATH,
    BAT_IDLE
};

enum BAT_ANIM
{
    ANIM_BAT_START,
    ANIM_BAT_FLY,
    ANIM_BAT_ATK,
    ANIM_BAT_FALLING,
    ANIM_BAT_HIT_FLOOR,
    ANIM_BAT_IDLE
};

#define BAT_ANGLE ANGLE(20)
constexpr auto BAT_ATTACK_RANGE = SQUARE(CLICK(1)); // 65536
constexpr auto BAT_TARGETING_RANGE = SQUARE(SECTOR(5)); // 0x1900000
constexpr auto BAT_TARGET_YPOS = SQUARE(CLICK(2) / 17);
constexpr auto BAT_DAMAGE = 2;

static BITE_INFO batBite = { 0, 16, 45, 4 };

void InitialiseBat(short itemNumber)
{
    ITEM_INFO* item = &Items[itemNumber];
    InitialiseCreature(itemNumber);
    item->animNumber = Objects[item->objectNumber].animIndex + ANIM_BAT_IDLE;
    item->frameNumber = Anims[item->animNumber].frameBase;
    item->goalAnimState = BAT_IDLE;
    item->currentAnimState = BAT_IDLE;
}

void BatControl(short itemNumber)
{
    if (!CreatureActive(itemNumber))
        return;

    ITEM_INFO* item, *target;
    CREATURE_INFO* bat, *slots;
    AI_INFO info;
    int distance, bestdistance;
    short angle;

    item = &Items[itemNumber];
    bat = GetCreatureInfo(item);
    angle = 0;
    
    if (item->hitPoints <= 0)
    {
        if (item->pos.yPos >= item->floor)
        {
            item->goalAnimState = BAT_DEATH;
            item->pos.yPos = item->floor;
            item->gravityStatus = FALSE;
        }
        else
        {
            item->animNumber = Objects[item->objectNumber].animIndex + ANIM_BAT_FALLING;
            item->frameNumber = Anims[item->animNumber].frameBase;
            item->goalAnimState = BAT_FALLING;
            item->currentAnimState = BAT_FALLING;
            item->speed = 0;
        }
    }
    else
    {
        if (item->aiBits & ALL_AIOBJ)
        {
            GetAITarget(bat);
        }
        else
        {
            // check if voncroy are in range !
            bestdistance = MAXINT;
            bat->enemy = LaraItem;

            slots = &BaddieSlots[0];
            for (int i = 0; i < NUM_SLOTS; i++, slots++)
            {
                target = &Items[slots->itemNum];
                if (target->objectNumber == VON_CROY && target->status != FITEM_INVISIBLE)
                {
                    int x, z;
                    x = target->pos.xPos - item->pos.xPos;
                    z = target->pos.zPos - item->pos.zPos;
                    distance = SQUARE(x) + SQUARE(z);
                    if (distance < bestdistance)
                    {
                        bat->enemy = target;
                        bestdistance = distance;
                    }
                }
            }
        }

        CreatureAIInfo(item, &info);
        GetCreatureMood(item, &info, TIMID);
        if (bat->flags)
            bat->mood = ESCAPE_MOOD;
        CreatureMood(item, &info, TIMID);
        angle = CreatureTurn(item, BAT_ANGLE);

        switch (item->currentAnimState)
        {
        case BAT_IDLE:
            if (info.distance < BAT_TARGETING_RANGE || item->hitStatus || bat->hurtByLara)
                item->goalAnimState = BAT_START;
            break;
        case BAT_FLY:
            if (info.distance < BAT_ATTACK_RANGE || !(GetRandomControl() & 0x3F))
                bat->flags = 0;

            if (!bat->flags)
            {
                if (item->touchBits || bat->enemy != LaraItem)
                {
                    if (info.distance < BAT_ATTACK_RANGE
                    &&  info.ahead && abs(item->pos.yPos - bat->enemy->pos.yPos) < BAT_TARGET_YPOS)
                    {
                        item->goalAnimState = BAT_ATK;
                    }
                }
            }
            break;
        case BAT_ATK:
            if (!bat->flags && (item->touchBits || bat->enemy != LaraItem) && info.distance < BAT_ATTACK_RANGE && info.ahead && abs(item->pos.yPos - bat->enemy->pos.yPos) < BAT_TARGET_YPOS)
            {
                CreatureEffect(item, &batBite, DoBloodSplat);
                if (bat->enemy == LaraItem)
                {
                    LaraItem->hitPoints -= BAT_DAMAGE;
                    LaraItem->hitStatus = TRUE;
                }
                bat->flags = 1;
            }
            else
            {
                item->goalAnimState = BAT_FLY;
                bat->mood = BORED_MOOD;
            }
            break;
        }
    }

    CreatureAnimation(itemNumber, angle, 0);
}

#ifdef DLL_INJECT
void injector::f_game::inject_bat()
{
    inject(0x00402A10, InitialiseBat);
    inject(0x00402A70, BatControl);
}
#endif
