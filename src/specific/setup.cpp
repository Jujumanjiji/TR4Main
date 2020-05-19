#include "framework.h"
#include "setup.h"
#include "setup_entity.h"
#include "setup_object.h"
#include "setup_trap.h"
#include "draw.h"
#include "effects.h"
#include "items.h"
#include "hair.h"
#include "pickup.h"
#include "rope.h"
#include "utils.h"
#include "oldobjects.h"

void InitialiseObjects(void)
{
    OBJECT_INFO* obj;

    for (int i = 0; i < NUMBER_OBJECTS; i++)
    {
        obj = &Objects[i];
        obj->initialise = NULL;
        obj->collision = NULL;
        obj->control = NULL;
        obj->floor = NULL;
        obj->ceiling = NULL;
        obj->drawRoutine = DrawAnimatingItem;
        obj->drawRoutineExtra = NULL;
        obj->hitPoints = DONT_TARGET;
        obj->pivotLength = 0;
        obj->radius = 10; // default value
        obj->shadowSize = 0;
        obj->intelligent = FALSE;
        obj->waterCreature = FALSE;
        obj->savePosition = FALSE;
        obj->saveAnim = FALSE;
        obj->saveFlags = FALSE;
        obj->saveMesh = FALSE;
        obj->saveHitpoints = FALSE;
        obj->usingDrawAnimatingItem = TRUE;
        obj->frameBase += *frames;
    }

    BaddyObjects();
    ObjectObjects();
    TrapObjects();
    InitialiseHair();
    InitialiseEffects();
    
    NumRPickups = 0;
    CurrentSequence = 0;
    SequenceResult[0][1][2] = 0;
    SequenceResult[0][2][1] = 1;
    SequenceResult[1][0][2] = 2;
    SequenceResult[1][2][0] = 3;
    SequenceResult[2][0][1] = 4;
    SequenceResult[2][1][0] = 5;
    SequenceUsed = 0;
    byte_7FD1DC = 0;
    byte_7F5700 = 0;
}

void BaddyObjects(void)
{
    setup_entity.lara_obj();
    setup_entity.motor_bike();
    setup_entity.jeep();
    setup_entity.skeleton();
    setup_entity.von_croy();
    setup_entity.guide();
    setup_entity.baddy_1();
    setup_entity.baddy_2();
    setup_entity.scorpion();
    setup_entity.small_scorpion();
    setup_entity.mummy();
    setup_entity.knight_templar();
    setup_entity.sphinx();
    setup_entity.setha();
    setup_entity.lara_double();
    setup_entity.horseman();
    setup_entity.hammerhead();
    setup_entity.crocodile();
    setup_entity.demigod1();
    setup_entity.demigod2();
    setup_entity.demigod3();
    setup_entity.mutant();
    setup_entity.troops();
    setup_entity.sas();
    setup_entity.harpy();
    setup_entity.wild_boar();
    setup_entity.dog();
    setup_entity.ahmet();
    setup_entity.baboon_normal();
    setup_entity.baboon_silent();
    setup_entity.baboon_inv();
    setup_entity.enemy_jeep();
    setup_entity.bat();
    setup_entity.big_beetle();
    setup_entity.sentry_gun();
    setup_entity.horse();
    setup_entity.sas_dying();
    setup_entity.sas_captain();
    setup_entity.jean_yves();
    setup_entity.game_piece1();
    setup_entity.game_piece2();
    setup_entity.game_piece3();
    setup_entity.enemy_piece();
    setup_entity.wheel_of_fortune();
    setup_entity.wraith1();
    setup_entity.wraith2();
    setup_entity.wraith3();
    setup_entity.wraith4();
    setup_entity.little_beetle();
    setup_entity.fish();
}

void ObjectObjects(void)
{
    setup_object.camera_target();
    setup_object.flare();
    setup_object.smash_object();
    setup_object.bridge();
    setup_object.switchs();
    setup_object.sequence_switch();
    setup_object.underwater_switch();
    setup_object.lever_switch();
    setup_object.jump_switch();
    setup_object.pulley();
    setup_object.turn_switch();
    setup_object.cog_switch();
    setup_object.crowbar_switch();
    setup_object.doors();
    setup_object.double_door();
    setup_object.sequence_door();
    setup_object.underwater_door();
    setup_object.pushpull_door();
    setup_object.floor_trapdoor();
    setup_object.ceiling_trapdoor();
    setup_object.trapdoor();
    setup_object.pickups();
    setup_object.inventory_item();
    setup_object.grenade();
    setup_object.crossbow_bolt();
    setup_object.key_hole();
    setup_object.puzzle_hole();
    setup_object.puzzle_done();
    setup_object.sarcophagus();
    setup_object.animatings();
    setup_object.statue_plinth();
    setup_object.element_puzzle();
    setup_object.expanding_platform();
    setup_object.obelisk();
    setup_object.squishy_block();
    setup_object.firerope();
    setup_object.mapper();
    setup_object.slicer_dicer();
    setup_object.scales();
    setup_object.sarcophagus_cut();
    setup_object.joby_spike();
    setup_object.teeth_spike();
    setup_object.horus_statue();
    setup_object.raising_block();
    setup_object.smoke_emitter();
    setup_object.red_green_blue_light();
    setup_object.blinking_light();
    setup_object.lens_flare();
    setup_object.amber_light();
    setup_object.bubble_effect();
    setup_object.waterfallmist();
    setup_object.waterfall();
    setup_object.lightning_conductor();
    setup_object.white_light();
    setup_object.body_part();
    setup_object.earthquake();
    setup_object.clockwork_beetle();
    setup_object.god_head();
}

void TrapObjects(void)
{
    setup_trap.floor_4blade();
    setup_trap.roof_4blade();
    setup_trap.bird_blade();
    setup_trap.seth_blade();
    setup_trap.chain();
    setup_trap.plough();
    setup_trap.spikeball();
    setup_trap.hammer();
    setup_trap.catwalk_blade();
    setup_trap.moving_blade();
    setup_trap.plinth_blade();
    setup_trap.cog();
    setup_trap.stargate();
    setup_trap.rollingball();
    setup_trap.burning_floor();
    setup_trap.twoblock_platform();
    setup_trap.kill_all_triggers();
    setup_trap.death_slide();
    setup_trap.falling_block();
    setup_trap.falling_ceiling();
    setup_trap.smashable_bike_floor();
    setup_trap.smashable_bike_wall();
    setup_trap.pushable_object();
    setup_trap.darts();
    setup_trap.planet_effect();
    setup_trap.flame();
    setup_trap.sas_drag_bloke();
    setup_trap.dart_emitter();
    setup_trap.homing_dart_emitter();
    setup_trap.flame_emitter();
    setup_trap.sprinkler();
    InitialiseRopeArray();
    setup_trap.rope();
    setup_trap.polerope();
    setup_trap.trigger_triggerer();
    setup_trap.mine();
}

void GetCarriedItems(void)
{
    ITEM_INFO* item, *pickup;
    int i;
    short pickup_number;

    // initialise carriedItems from all level items.
    // get all carried items by default in the same sector (512 XZmax)
    for (i = 0; i < level_items; i++)
    {
        item = &Items[i];
        if (Objects[item->objectNumber].intelligent)
        {
            item->carriedItem = NO_ITEM;
            pickup_number = rooms[item->roomNumber].item_number;
            do
            {
                pickup = &Items[pickup_number];
                if (abs(pickup->pos.xPos - item->pos.xPos) < 512 &&
                    abs(pickup->pos.yPos - item->pos.yPos) < 256 &&
                    abs(pickup->pos.zPos - item->pos.zPos) < 512 &&
                    Objects[pickup->objectNumber].collision == PickupCollision)
                {
                    pickup->carriedItem = item->carriedItem; // linked list for multiple pickup
                    item->carriedItem = pickup_number;
                    RemoveDrawnItem(pickup_number);
                    pickup->roomNumber = NO_ROOM;
                }

                pickup_number = pickup->nextItem;
            } while (pickup_number != NO_ITEM);
        }
    }
}

void GetAIPickups(void)
{
    ITEM_INFO* item;
    AIOBJECT* obj;
    int i, num;

    for (i = 0; i < level_items; i++)
    {
        item = &Items[i];
        if (Objects[item->objectNumber].intelligent)
        {
            item->aiBits = NULL;
            for (num = 0; num < nAIObjects; num++)
            {
                obj = &AIObjects[num];
                if (abs(obj->x - item->pos.xPos) < 512 &&
                    abs(obj->z - item->pos.zPos) < 512 &&
                    obj->room_number == item->roomNumber &&
                    obj->object_number <= AI_X2)
                {
                    item->aiBits = (((1 << obj->object_number) + 114) & 0x1F) << 9;
                    item->item_flags[3] = obj->trigger_flags;
                    if (obj->object_number != AI_GUARD)
                        obj->room_number = NO_ROOM;
                }
            }
        }
    }
}

#ifdef DLL_INJECT
void injector::f_specific::inject_setup()
{
    inject(0x0045C0D0, InitialiseObjects);
    inject(0x0045C1E0, BaddyObjects);
    inject(0x0045E1F0, ObjectObjects);
    inject(0x0045DC10, TrapObjects);
    //inject(0x0045EB40, GetCarriedItems);
    //inject(0x0045EC50, GetAIPickups);
}
#endif
