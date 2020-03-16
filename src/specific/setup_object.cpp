#include "framework.h"
#include "setup_object.h"
#include "collide.h"
#include "door.h"
#include "items.h"
#include "lara.h"
#include "pickup.h"
#include "puzzle.h"
#include "oldobjects.h"
#include "switchs.h"

SetupObject setup_object;

void SetupObject::camera_target()
{
    obj = &objects[CAMERA_TARGET];
    if (obj->loaded)
        obj->draw_routine = NULL;
}

void SetupObject::flare()
{
    obj = &objects[FLARE_ITEM];
    if (obj->loaded)
    {
        obj->control = FlareItemControl;
        obj->collision = PickupCollision;
        obj->draw_routine = DrawFlareItem;
        obj->pivot_length = 256;
        obj->hit_points = 256; // timer
        obj->save_flags = true;
        obj->save_position = true;
    }
}

void SetupObject::smash_object()
{
    INIT_SMASH_OBJECT(SMASH_OBJECT1);
    INIT_SMASH_OBJECT(SMASH_OBJECT2);
    INIT_SMASH_OBJECT(SMASH_OBJECT3);
    INIT_SMASH_OBJECT(SMASH_OBJECT4);
    INIT_SMASH_OBJECT(SMASH_OBJECT5);
    INIT_SMASH_OBJECT(SMASH_OBJECT6);
    INIT_SMASH_OBJECT(SMASH_OBJECT7);
    INIT_SMASH_OBJECT(SMASH_OBJECT8);
}

void SetupObject::bridge()
{
    obj = &objects[BRIDGE_FLAT];
    if (obj->loaded)
    {
        obj->floor = BridgeFlatFloor;
        obj->ceiling = BridgeFlatCeiling;
    }

    obj = &objects[BRIDGE_TILT1];
    if (obj->loaded)
    {
        obj->floor = BridgeTilt1Floor;
        obj->ceiling = BridgeTilt1Ceiling;
    }

    obj = &objects[BRIDGE_TILT2];
    if (obj->loaded)
    {
        obj->floor = BridgeTilt2Floor;
        obj->ceiling = BridgeTilt2Ceiling;
    }
}

void SetupObject::switchs()
{
    INIT_SWITCH(SWITCH_TYPE1);
    INIT_SWITCH(SWITCH_TYPE2);
    INIT_SWITCH(SWITCH_TYPE3);
    INIT_SWITCH(SWITCH_TYPE4);
    INIT_SWITCH(SWITCH_TYPE5);
    INIT_SWITCH(SWITCH_TYPE6);
    
    obj = &objects[SWITCH_TYPE7];
    if (obj->loaded)
    {
        obj->control = PuzzleControl;
        obj->collision = NULL;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->explodable_meshbits = 1;
        //obj->unknown = TRUE;
    }

    obj = &objects[SWITCH_TYPE8];
    if (obj->loaded)
    {
        obj->control = PuzzleControl;
        obj->collision = NULL;
        obj->save_anim = true;
        obj->save_flags = true;
    }
}

void SetupObject::sequence_switch()
{
    obj = &objects[SEQUENCE_SWITCH1];
    if (obj->loaded)
    {
        obj->control = SequenceSwitchControl;
        obj->collision = SequenceSwitchCollision;
        obj->save_anim = true;
        obj->save_flags = true;
    }

    obj = &objects[SEQUENCE_SWITCH2];
    if (obj->loaded)
    {
        obj->control = SequenceSwitchControl;
        obj->collision = SequenceSwitchCollision;
        obj->save_anim = true;
        obj->save_flags = true;
    }

    obj = &objects[SEQUENCE_SWITCH3];
    if (obj->loaded)
    {
        obj->control = SequenceSwitchControl;
        obj->collision = SequenceSwitchCollision;
        obj->save_anim = true;
        obj->save_flags = true;
    }
}

void SetupObject::underwater_switch()
{
    obj = &objects[UNDERWATER_SWITCH1];
    if (obj->loaded)
    {
        obj->control = SwitchControl;
        obj->collision = SwitchUnderwater1Collision;
        obj->save_anim = true;
        obj->save_flags = true;
    }

    obj = &objects[UNDERWATER_SWITCH2];
    if (obj->loaded)
    {
        obj->control = SwitchControl;
        obj->collision = SwitchUnderwater2Collision;
        obj->save_anim = true;
        obj->save_flags = true;
    }
}

void SetupObject::lever_switch()
{
    obj = &objects[LEVER_SWITCH];
    if (obj->loaded)
    {
        obj->control = SwitchControl;
        obj->collision = LeverSwitchCollision;
        obj->save_anim = true;
        obj->save_flags = true;
    }
}

void SetupObject::jump_switch()
{
    obj = &objects[JUMP_SWITCH];
    if (obj->loaded)
    {
        obj->control = SwitchControl;
        obj->collision = JumpSwitchCollision;
        obj->save_anim = true;
        obj->save_flags = true;
    }
}

void SetupObject::pulley()
{
    obj = &objects[PULLEY];
    if (obj->loaded)
    {
        obj->initialise = InitialisePulleySwitch;
        obj->control = SwitchControl;
        obj->collision = PulleySwitchCollision;
        obj->save_anim = true;
        obj->save_flags = true;
    }
}

void SetupObject::turn_switch()
{
    obj = &objects[TURN_SWITCH];
    if (obj->loaded)
    {
        obj->control = SwitchControl;
        obj->collision = TurnSwitchCollision;
        obj->save_anim = true;
        obj->save_flags = true;
    }
}

void SetupObject::cog_switch()
{
    obj = &objects[COG_SWITCH];
    if (obj->loaded)
    {
        obj->control = CogSwitchControl;
        obj->collision = CogSwitchCollision;
        obj->save_anim = true;
        obj->save_flags = true;
    }
}

void SetupObject::crowbar_switch()
{
    obj = &objects[CROWBAR_SWITCH];
    if (obj->loaded)
    {
        obj->control = SwitchControl;
        obj->collision = CrowbarSwitchCollision;
        obj->save_anim = true;
        obj->save_flags = true;
    }
}

void SetupObject::doors()
{
    INIT_DOOR(DOOR_TYPE1);
    INIT_DOOR(DOOR_TYPE2);
    INIT_DOOR(DOOR_TYPE3);
    INIT_DOOR(DOOR_TYPE4);
    INIT_DOOR(DOOR_TYPE5);
    INIT_DOOR(DOOR_TYPE6);
    INIT_DOOR(DOOR_TYPE7);
    INIT_DOOR(DOOR_TYPE8);
}

void SetupObject::double_door()
{
    obj = &objects[DOUBLE_DOORS];
    if (obj->loaded)
    {
        obj->initialise = InitialiseDoor;
        obj->control = DoubleDoorControl;
        obj->collision = DoubleDoorCollision;
        obj->save_anim = true;
        obj->save_flags = true;
    }
}

void SetupObject::sequence_door()
{
    obj = &objects[SEQUENCE_DOOR1];
    if (obj->loaded)
    {
        obj->initialise = InitialiseDoor;
        obj->control = SequenceDoorControl;
        obj->collision = DoorCollision;
        obj->save_anim = true;
        obj->save_flags = true;
    }
}

void SetupObject::underwater_door()
{
    obj = &objects[UNDERWATER_DOOR];
    if (obj->loaded)
    {
        obj->initialise = InitialiseDoor;
        obj->control = DoubleDoorControl;
        obj->collision = UnderwaterDoorCollision;
        obj->save_anim = true;
        obj->save_flags = true;
    }
}

void SetupObject::pushpull_door()
{
    obj = &objects[PUSHPULL_DOOR1];
    if (obj->loaded)
    {
        obj->initialise = InitialiseDoor;
        obj->control = DoubleDoorControl;
        obj->collision = PushPullDoorCollision;
        obj->save_anim = true;
        obj->save_flags = true;
    }

    obj = &objects[PUSHPULL_DOOR2];
    if (obj->loaded)
    {
        obj->initialise = InitialiseDoor;
        obj->control = DoubleDoorControl;
        obj->collision = PushPullDoorCollision;
        obj->save_anim = true;
        obj->save_flags = true;
    }

    obj = &objects[KICK_DOOR1];
    if (obj->loaded)
    {
        obj->initialise = InitialiseDoor;
        obj->control = DoubleDoorControl;
        obj->collision = PushPullDoorCollision;
        obj->save_anim = true;
        obj->save_flags = true;
    }

    obj = &objects[KICK_DOOR2];
    if (obj->loaded)
    {
        obj->initialise = InitialiseDoor;
        obj->control = DoubleDoorControl;
        obj->collision = PushPullDoorCollision;
        obj->save_anim = true;
        obj->save_flags = true;
    }
}

void SetupObject::floor_trapdoor()
{
    obj = &objects[FLOOR_TRAPDOOR1];
    if (obj->loaded)
    {
        obj->initialise = InitialiseFloorCeilingTrapDoor;
        obj->control = FloorCeilingTrapDoorControl;
        obj->collision = FloorTrapDoorCollision;
        obj->save_anim = true;
        obj->save_flags = true;
    }

    obj = &objects[FLOOR_TRAPDOOR2];
    if (obj->loaded)
    {
        obj->initialise = InitialiseFloorCeilingTrapDoor;
        obj->control = FloorCeilingTrapDoorControl;
        obj->collision = FloorTrapDoorCollision;
        obj->save_anim = true;
        obj->save_flags = true;
    }
}

void SetupObject::ceiling_trapdoor()
{
    obj = &objects[CEILING_TRAPDOOR1];
    if (obj->loaded)
    {
        obj->initialise = InitialiseFloorCeilingTrapDoor;
        obj->control = FloorCeilingTrapDoorControl;
        obj->collision = CeilingTrapDoorCollision;
        obj->save_anim = true;
        obj->save_flags = true;
    }

    obj = &objects[CEILING_TRAPDOOR2];
    if (obj->loaded)
    {
        obj->initialise = InitialiseFloorCeilingTrapDoor;
        obj->control = FloorCeilingTrapDoorControl;
        obj->collision = CeilingTrapDoorCollision;
        obj->save_anim = true;
        obj->save_flags = true;
    }
}

void SetupObject::trapdoor()
{
    obj = &objects[TRAPDOOR1];
    if (obj->loaded)
    {
        obj->initialise = InitialiseFloorCeilingTrapDoor;
        obj->control = FloorCeilingTrapDoorControl;
        obj->save_anim = true;
        obj->save_flags = true;
    }

    obj = &objects[TRAPDOOR2];
    if (obj->loaded)
    {
        obj->initialise = InitialiseFloorCeilingTrapDoor;
        obj->control = FloorCeilingTrapDoorControl;
        obj->save_anim = true;
        obj->save_flags = true;
    }

    obj = &objects[TRAPDOOR3];
    if (obj->loaded)
    {
        obj->initialise = InitialiseFloorCeilingTrapDoor;
        obj->control = FloorCeilingTrapDoorControl;
        obj->save_anim = true;
        obj->save_flags = true;
    }
}

void SetupObject::pickups()
{
    INIT_PICKUP(PUZZLE_ITEM1);
    INIT_PICKUP(PUZZLE_ITEM2);
    INIT_PICKUP(PUZZLE_ITEM3);
    INIT_PICKUP(PUZZLE_ITEM4);
    INIT_PICKUP(PUZZLE_ITEM5);
    INIT_PICKUP(PUZZLE_ITEM6);
    INIT_PICKUP(PUZZLE_ITEM7);
    INIT_PICKUP(PUZZLE_ITEM8);
    INIT_PICKUP(PUZZLE_ITEM9);
    INIT_PICKUP(PUZZLE_ITEM10);
    INIT_PICKUP(PUZZLE_ITEM11);
    INIT_PICKUP(PUZZLE_ITEM12);
    INIT_PICKUP(PUZZLE_ITEM1_COMBO1);
    INIT_PICKUP(PUZZLE_ITEM1_COMBO2);
    INIT_PICKUP(PUZZLE_ITEM2_COMBO1);
    INIT_PICKUP(PUZZLE_ITEM2_COMBO2);
    INIT_PICKUP(PUZZLE_ITEM3_COMBO1);
    INIT_PICKUP(PUZZLE_ITEM3_COMBO2);
    INIT_PICKUP(PUZZLE_ITEM4_COMBO1);
    INIT_PICKUP(PUZZLE_ITEM4_COMBO2);
    INIT_PICKUP(PUZZLE_ITEM5_COMBO1);
    INIT_PICKUP(PUZZLE_ITEM5_COMBO2);
    INIT_PICKUP(PUZZLE_ITEM6_COMBO1);
    INIT_PICKUP(PUZZLE_ITEM6_COMBO2);
    INIT_PICKUP(PUZZLE_ITEM7_COMBO1);
    INIT_PICKUP(PUZZLE_ITEM7_COMBO2);
    INIT_PICKUP(PUZZLE_ITEM8_COMBO1);
    INIT_PICKUP(PUZZLE_ITEM8_COMBO2);
    INIT_PICKUP(KEY_ITEM1);
    INIT_PICKUP(KEY_ITEM2);
    INIT_PICKUP(KEY_ITEM3);
    INIT_PICKUP(KEY_ITEM4);
    INIT_PICKUP(KEY_ITEM5);
    INIT_PICKUP(KEY_ITEM6);
    INIT_PICKUP(KEY_ITEM7);
    INIT_PICKUP(KEY_ITEM8);
    INIT_PICKUP(KEY_ITEM9);
    INIT_PICKUP(KEY_ITEM10);
    INIT_PICKUP(KEY_ITEM11);
    INIT_PICKUP(KEY_ITEM12);
    INIT_PICKUP(KEY_ITEM1_COMBO1);
    INIT_PICKUP(KEY_ITEM1_COMBO2);
    INIT_PICKUP(KEY_ITEM2_COMBO1);
    INIT_PICKUP(KEY_ITEM2_COMBO2);
    INIT_PICKUP(KEY_ITEM3_COMBO1);
    INIT_PICKUP(KEY_ITEM3_COMBO2);
    INIT_PICKUP(KEY_ITEM4_COMBO1);
    INIT_PICKUP(KEY_ITEM4_COMBO2);
    INIT_PICKUP(KEY_ITEM5_COMBO1);
    INIT_PICKUP(KEY_ITEM5_COMBO2);
    INIT_PICKUP(KEY_ITEM6_COMBO1);
    INIT_PICKUP(KEY_ITEM6_COMBO2);
    INIT_PICKUP(KEY_ITEM7_COMBO1);
    INIT_PICKUP(KEY_ITEM7_COMBO2);
    INIT_PICKUP(KEY_ITEM8_COMBO1);
    INIT_PICKUP(KEY_ITEM8_COMBO2);
    INIT_PICKUP(PICKUP_ITEM1);
    INIT_PICKUP(PICKUP_ITEM2);
    INIT_PICKUP(PICKUP_ITEM3);
    INIT_PICKUP(PICKUP_ITEM4);
    INIT_PICKUP(PICKUP_ITEM1_COMBO1);
    INIT_PICKUP(PICKUP_ITEM1_COMBO2);
    INIT_PICKUP(PICKUP_ITEM2_COMBO1);
    INIT_PICKUP(PICKUP_ITEM2_COMBO2);
    INIT_PICKUP(PICKUP_ITEM3_COMBO1);
    INIT_PICKUP(PICKUP_ITEM3_COMBO2);
    INIT_PICKUP(PICKUP_ITEM4_COMBO1);
    INIT_PICKUP(PICKUP_ITEM4_COMBO2);
    INIT_PICKUP(EXAMINE1);
    INIT_PICKUP(EXAMINE2);
    INIT_PICKUP(EXAMINE3);
    INIT_PICKUP(CROWBAR_ITEM);
    INIT_PICKUP(BURNING_TORCH_ITEM);
    INIT_PICKUP(CLOCKWORK_BEETLE);
    INIT_PICKUP(CLOCKWORK_BEETLE_COMBO1);
    INIT_PICKUP(CLOCKWORK_BEETLE_COMBO2);
    INIT_PICKUP(MINE_DETECTOR);
    INIT_PICKUP(QUEST_ITEM1);
    INIT_PICKUP(QUEST_ITEM2);
    INIT_PICKUP(QUEST_ITEM3);
    INIT_PICKUP(QUEST_ITEM4);
    INIT_PICKUP(QUEST_ITEM5);
    INIT_PICKUP(QUEST_ITEM6);
    INIT_PICKUP(MAP);
    INIT_PICKUP(SECRET_MAP);
    INIT_PICKUP(WATERSKIN1_EMPTY);
    INIT_PICKUP(WATERSKIN2_EMPTY);
}

void SetupObject::inventory_item()
{
    INIT_PICKUP(PISTOLS_ITEM);
    INIT_PICKUP(PISTOLS_AMMO_ITEM);
    INIT_PICKUP(UZI_ITEM);
    INIT_PICKUP(UZI_AMMO_ITEM);
    INIT_PICKUP(SHOTGUN_ITEM);
    INIT_PICKUP(SHOTGUN_AMMO1_ITEM);
    INIT_PICKUP(SHOTGUN_AMMO2_ITEM);
    INIT_PICKUP(CROSSBOW_ITEM);
    INIT_PICKUP(CROSSBOW_AMMO1_ITEM);
    INIT_PICKUP(CROSSBOW_AMMO2_ITEM);
    INIT_PICKUP(CROSSBOW_AMMO3_ITEM);
    INIT_PICKUP(GRENADE_GUN_ITEM);
    INIT_PICKUP(GRENADE_GUN_AMMO1_ITEM);
    INIT_PICKUP(GRENADE_GUN_AMMO2_ITEM);
    INIT_PICKUP(GRENADE_GUN_AMMO3_ITEM);
    INIT_PICKUP(REVOLVER_ITEM);
    INIT_PICKUP(REVOLVER_AMMO_ITEM);
    INIT_PICKUP(BIGMEDI_ITEM);
    INIT_PICKUP(SMALLMEDI_ITEM);
    INIT_PICKUP(LASERSIGHT_ITEM);
    INIT_PICKUP(BINOCULARS_ITEM);
    INIT_PICKUP(FLARE_INV_ITEM);
    INIT_PICKUP(DIARY_ITEM);
    INIT_PICKUP(COMPASS_ITEM);
    INIT_PICKUP(MEMCARD_LOAD_INV_ITEM);
    INIT_PICKUP(MEMCARD_SAVE_INV_ITEM);
    INIT_PICKUP(PC_LOAD_INV_ITEM);
    INIT_PICKUP(PC_SAVE_INV_ITEM);
}

void SetupObject::grenade()
{
    obj = &objects[GRENADE];
    if (obj->loaded)
    {
        obj->initialise = NULL;
        obj->collision = NULL;
        obj->control = GrenadeControl;
        obj->draw_routine = DrawProjectile;
        obj->save_flags = true;
        obj->save_position = true;
    }
}

void SetupObject::crossbow_bolt()
{
    obj = &objects[CROSSBOW_BOLT];
    if (obj->loaded)
    {
        obj->initialise = NULL;
        obj->collision = NULL;
        obj->control = CrossbowBoltControl;
        obj->draw_routine = DrawProjectile;
        obj->save_flags = true;
        obj->save_position = true;
    }
}

void SetupObject::key_hole()
{
    INIT_KEY_HOLE(KEY_HOLE1);
    INIT_KEY_HOLE(KEY_HOLE2);
    INIT_KEY_HOLE(KEY_HOLE3);
    INIT_KEY_HOLE(KEY_HOLE4);
    INIT_KEY_HOLE(KEY_HOLE5);
    INIT_KEY_HOLE(KEY_HOLE6);
    INIT_KEY_HOLE(KEY_HOLE7);
    INIT_KEY_HOLE(KEY_HOLE8);
    INIT_KEY_HOLE(KEY_HOLE9);
    INIT_KEY_HOLE(KEY_HOLE10);
    INIT_KEY_HOLE(KEY_HOLE11);
    INIT_KEY_HOLE(KEY_HOLE12);
}

void SetupObject::puzzle_hole()
{
    INIT_PUZZLE_HOLE(PUZZLE_HOLE1);
    INIT_PUZZLE_HOLE(PUZZLE_HOLE2);
    INIT_PUZZLE_HOLE(PUZZLE_HOLE3);
    INIT_PUZZLE_HOLE(PUZZLE_HOLE4);
    INIT_PUZZLE_HOLE(PUZZLE_HOLE5);
    INIT_PUZZLE_HOLE(PUZZLE_HOLE6);
    INIT_PUZZLE_HOLE(PUZZLE_HOLE7);
    INIT_PUZZLE_HOLE(PUZZLE_HOLE8);
    INIT_PUZZLE_HOLE(PUZZLE_HOLE9);
    INIT_PUZZLE_HOLE(PUZZLE_HOLE10);
    INIT_PUZZLE_HOLE(PUZZLE_HOLE11);
    INIT_PUZZLE_HOLE(PUZZLE_HOLE12);
}

void SetupObject::puzzle_done()
{
    INIT_PUZZLE_DONE(PUZZLE_DONE1);
    INIT_PUZZLE_DONE(PUZZLE_DONE2);
    INIT_PUZZLE_DONE(PUZZLE_DONE3);
    INIT_PUZZLE_DONE(PUZZLE_DONE4);
    INIT_PUZZLE_DONE(PUZZLE_DONE5);
    INIT_PUZZLE_DONE(PUZZLE_DONE6);
    INIT_PUZZLE_DONE(PUZZLE_DONE7);
    INIT_PUZZLE_DONE(PUZZLE_DONE8);
    INIT_PUZZLE_DONE(PUZZLE_DONE9);
    INIT_PUZZLE_DONE(PUZZLE_DONE10);
    INIT_PUZZLE_DONE(PUZZLE_DONE11);
    INIT_PUZZLE_DONE(PUZZLE_DONE12);
}

void SetupObject::sarcophagus()
{
    obj = &objects[SARCOPHAGUS];
    if (obj->loaded)
    {
        obj->control = PuzzleControl;
        obj->collision = SarcophagusCollision;
        obj->save_anim = true;
        obj->save_flags = true;
    }
}

void SetupObject::animatings()
{
    INIT_ANIMATING(ANIMATING1, true);
    INIT_ANIMATING(ANIMATING2, true);
    INIT_ANIMATING(ANIMATING3, true);
    INIT_ANIMATING(ANIMATING4, true);
    INIT_ANIMATING(ANIMATING5, true);
    INIT_ANIMATING(ANIMATING6, true);
    INIT_ANIMATING(ANIMATING7, true);
    INIT_ANIMATING(ANIMATING8, true);
    INIT_ANIMATING(ANIMATING9, true);
    INIT_ANIMATING(ANIMATING10, true);
    INIT_ANIMATING(ANIMATING11, true);
    INIT_ANIMATING(ANIMATING12, true);
    INIT_ANIMATING(ANIMATING13, false);
    INIT_ANIMATING(ANIMATING14, false);
    INIT_ANIMATING(ANIMATING15, false);
    INIT_ANIMATING(ANIMATING16, false);

    // MIP is set like normal one
    INIT_ANIMATING(ANIMATING1_MIP, true);
    INIT_ANIMATING(ANIMATING2_MIP, true);
    INIT_ANIMATING(ANIMATING3_MIP, true);
    INIT_ANIMATING(ANIMATING4_MIP, true);
    INIT_ANIMATING(ANIMATING5_MIP, true);
    INIT_ANIMATING(ANIMATING6_MIP, true);
    INIT_ANIMATING(ANIMATING7_MIP, true);
    INIT_ANIMATING(ANIMATING8_MIP, true);
    INIT_ANIMATING(ANIMATING9_MIP, true);
    INIT_ANIMATING(ANIMATING10_MIP, true);
    INIT_ANIMATING(ANIMATING11_MIP, true);
    INIT_ANIMATING(ANIMATING12_MIP, true);
    INIT_ANIMATING(ANIMATING13_MIP, false);
    INIT_ANIMATING(ANIMATING14_MIP, false);
    INIT_ANIMATING(ANIMATING15_MIP, false);
    INIT_ANIMATING(ANIMATING16_MIP, false);
}

void SetupObject::statue_plinth()
{
    obj = &objects[STATUE_PLINTH];
    if (obj->loaded)
    {
        obj->initialise = InitialiseStatuePlinth;
        obj->collision = StatuePlinthCollision;
        obj->unknown = 1;
        obj->save_flags = true;
    }
}

void SetupObject::element_puzzle()
{
    obj = &objects[ELEMENT_PUZZLE];
    if (obj->loaded)
    {
        obj->initialise = InitialiseElementPuzzle;
        obj->control = ElementPuzzleControl;
        obj->collision = ElementPuzzleCollision;
        obj->unknown = 1;
        obj->save_flags = true;
    }
}

void SetupObject::expanding_platform()
{
    obj = &objects[EXPANDING_PLATFORM];
    if (obj->loaded)
    {
        obj->initialise = InitialiseRaisingBlock;
        obj->control = RaisingBlockControl;
        obj->draw_routine = DrawScaledObjects;
        obj->save_flags = true;
    }
}

void SetupObject::obelisk()
{
    obj = &objects[OBELISK];
    if (obj->loaded)
    {
        obj->initialise = InitialiseObelisk;
        obj->control = ObeliskControl;
        obj->collision = ObjectCollision;
        obj->save_flags = true;
        obj->save_position = true;
    }
}

void SetupObject::squishy_block()
{
    obj = &objects[SQUISHY_BLOCK1];
    if (obj->loaded)
    {
        obj->control = SquishyBlock1Control;
        obj->collision = ObjectCollision;
        obj->save_anim = true;
        obj->save_flags = true;
    }

    obj = &objects[SQUISHY_BLOCK2];
    if (obj->loaded)
    {
        obj->control = SquishyBlock2Control;
        obj->collision = SquishyBlock2Collision;
        obj->save_anim = true;
        obj->save_flags = true;
    }
}

void SetupObject::firerope()
{
    obj = &objects[FIREROPE];
    if (obj->loaded)
    {
        obj->control = FireRopeControl;
        obj->collision = FireRopeCollision;
        obj->save_anim = true;
        obj->save_flags = true;
    }
}

void SetupObject::mapper()
{
    obj = &objects[MAPPER];
    if (obj->loaded)
    {
        obj->initialise = InitialiseMapper;
        obj->control = MapperControl;
        obj->save_anim = true;
        obj->save_flags = true;
    }
}

void SetupObject::slicer_dicer()
{
    obj = &objects[SLICER_DICER];
    if (obj->loaded)
    {
        obj->initialise = InitialiseSlicerDicer;
        obj->control = SlicerDicerControl;
        obj->collision = MovBlockCollision;
        obj->save_flags = true;
    }
}

void SetupObject::scales()
{
    obj = &objects[SCALES];
    if (obj->loaded)
    {
        obj->control = ScalesControl;
        obj->collision = ScalesCollision;
        obj->save_anim = true;
        obj->save_flags = true;
    }
}

void SetupObject::sarcophagus_cut()
{
    obj = &objects[SARCOPHAGUS_CUT];
    if (obj->loaded)
        obj->unknown = 1;
}

void SetupObject::joby_spike()
{
    obj = &objects[JOBY_SPIKES];
    if (obj->loaded)
    {
        obj->initialise = InitialiseJobySpikes;
        obj->control = JobySpikesControl;
        obj->draw_routine = DrawScaledObjects;
        obj->save_flags = true;
    }
}

void SetupObject::teeth_spike()
{
    obj = &objects[TEETH_SPIKES];
    if (obj->loaded)
    {
        obj->initialise = InitialiseTeethSpike;
        obj->control = TeethSpikeControl;
        obj->draw_routine = DrawScaledObjects;
        obj->save_flags = true;
    }
}

void SetupObject::horus_statue()
{
    obj = &objects[HORUS_STATUE];
    if (obj->loaded)
    {
        obj->collision = ObjectCollision;
        obj->unknown = 1;
    }
}

void SetupObject::raising_block()
{
    obj = &objects[RAISING_BLOCK1];
    if (obj->loaded)
    {
        obj->initialise = InitialiseRaisingBlock;
        obj->control = RaisingBlockControl;
        obj->collision = NULL;
        obj->draw_routine = DrawScaledObjects;
        obj->save_flags = true;
    }

    obj = &objects[RAISING_BLOCK2];
    if (obj->loaded)
    {
        obj->initialise = InitialiseRaisingBlock;
        obj->control = RaisingBlockControl;
        obj->collision = NULL;
        obj->draw_routine = DrawScaledObjects;
        obj->save_flags = true;
    }
}

void SetupObject::smoke_emitter()
{
    obj = &objects[SMOKE_EMITTER_WHITE];
    if (obj->loaded)
    {
        obj->initialise = InitialiseSmokeEmitter;
        obj->control = SmokeEmitterControl;
        obj->draw_routine = NULL;
        obj->save_flags = true;
    }

    obj = &objects[SMOKE_EMITTER_BLACK];
    if (obj->loaded)
    {
        obj->initialise = InitialiseSmokeEmitter;
        obj->control = SmokeEmitterControl;
        obj->draw_routine = NULL;
        obj->save_flags = true;
    }

    obj = &objects[STEAM_EMITTER];
    if (obj->loaded)
    {
        obj->initialise = InitialiseSmokeEmitter;
        obj->control = SmokeEmitterControl;
        obj->draw_routine = NULL;
        obj->save_flags = true;
    }
}

void SetupObject::red_green_blue_light()
{
    obj = &objects[RED_LIGHT];
    if (obj->loaded)
    {
        obj->control = LightControl;
        obj->draw_routine = NULL;
        obj->save_flags = true;
    }

    obj = &objects[GREEN_LIGHT];
    if (obj->loaded)
    {
        obj->control = LightControl;
        obj->draw_routine = NULL;
        obj->save_flags = true;
    }

    obj = &objects[BLUE_LIGHT];
    if (obj->loaded)
    {
        obj->control = LightControl;
        obj->draw_routine = NULL;
        obj->save_flags = true;
    }
}

void SetupObject::blinking_light()
{
    obj = &objects[BLINKING_LIGHT];
    if (obj->loaded)
    {
        obj->control = BlinkingLightControl;
        obj->save_flags = true;
    }
}

void SetupObject::lens_flare()
{
    obj = &objects[LENS_FLARE];
    if (obj->loaded)
        obj->draw_routine = DrawLensFlare;
}

void SetupObject::amber_light()
{
    obj = &objects[AMBER_LIGHT];
    if (obj->loaded)
    {
        obj->control = AmberLightControl;
        obj->draw_routine = NULL;
        obj->save_flags = true;
    }
}

void SetupObject::bubble_effect()
{
    obj = &objects[BUBBLES];
    obj->loaded = true;
    obj->control = BubblesControl;
    obj->draw_routine = (LPDRAW)true;
    obj->nmeshes = 0;
}

void SetupObject::waterfallmist()
{
    obj = &objects[WATERFALLMIST];
    if (obj->loaded)
    {
        obj->control = WaterfallMistControl;
        obj->draw_routine = NULL;
    }
}

void SetupObject::waterfall()
{
    obj = &objects[WATERFALL1];
    if (obj->loaded)
    {
        obj->control = WaterfallControl;
        obj->save_flags = true;
    }

    obj = &objects[WATERFALL2];
    if (obj->loaded)
    {
        obj->control = WaterfallControl;
        obj->save_flags = true;
    }

    obj = &objects[WATERFALL3];
    if (obj->loaded)
    {
        obj->control = WaterfallControl;
        obj->save_flags = true;
    }
}

void SetupObject::lightning_conductor()
{
    obj = &objects[LIGHTNING_CONDUCTOR];
    if (obj->loaded)
    {
        obj->initialise = InitialiseLightningConductor;
        obj->control = LightningConductorControl;
        obj->draw_routine = NULL;
        obj->save_flags = true;
    }
}

void SetupObject::white_light()
{
    obj = &objects[WHITE_LIGHT];
    if (obj->loaded)
    {
        obj->control = WhiteLightControl;
        obj->draw_routine = NULL;
        obj->save_flags = true;
    }
}

void SetupObject::body_part()
{
    obj = &objects[BODY_PART];
    obj->loaded = true;
    obj->nmeshes = 0;
    obj->control = BodyPartControl;
    obj->draw_routine = (LPDRAW)TRUE;
}

void SetupObject::earthquake()
{
    obj = &objects[EARTHQUAKE];
    if (obj->loaded)
    {
        obj->control = EarthquakeControl;
        obj->draw_routine = NULL;
        obj->save_flags = true;
    }
}

void SetupObject::clockwork_beetle()
{
    obj = &objects[CLOCKWORK_BEETLE];
    if (obj->loaded)
    {
        obj->initialise = NULL;
        obj->control = ClockworkBeetleControl;
    }
}

void SetupObject::god_head()
{
    obj = &objects[GOD_HEAD];
    if (obj->loaded)
    {
        obj->control = GodHeadControl;
        obj->draw_routine = DrawGodHead;
        obj->save_flags = true;
    }
}
