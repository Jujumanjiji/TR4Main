#include "framework.h"
#include "setup_object.h"
#include "collide.h"
#include "door.h"
#include "items.h"
#include "lara.h"
#include "pickup.h"
#include "puzzle.h"
#include "switchs.h"
#include "oldobjects.h"

SetupObject setup_object;
void SetupObject::camera_target()
{
    obj = &Objects[CAMERA_TARGET];
    if (obj->loaded)
        obj->drawRoutine = NULL;
}

void SetupObject::flare()
{
    obj = &Objects[FLARE_ITEM];
    if (obj->loaded)
    {
        obj->control = FlareItemControl;
        obj->collision = PickupCollision;
        obj->drawRoutine = DrawFlareItem;
        obj->pivotLength = 256;
        obj->hitPoints = 256; // timer
        obj->saveFlags = true;
        obj->savePosition = true;
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
    obj = &Objects[BRIDGE_FLAT];
    if (obj->loaded)
    {
        obj->floor = BridgeFlatFloor;
        obj->ceiling = BridgeFlatCeiling;
    }

    obj = &Objects[BRIDGE_TILT1];
    if (obj->loaded)
    {
        obj->floor = BridgeTilt1Floor;
        obj->ceiling = BridgeTilt1Ceiling;
    }

    obj = &Objects[BRIDGE_TILT2];
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
    
    obj = &Objects[SWITCH_TYPE7];
    if (obj->loaded)
    {
        obj->control = PuzzleControl;
        obj->collision = NULL;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->explodableMeshBits = 1;
    }

    obj = &Objects[SWITCH_TYPE8];
    if (obj->loaded)
    {
        obj->control = PuzzleControl;
        obj->collision = NULL;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }
}

void SetupObject::sequence_switch()
{
    obj = &Objects[SEQUENCE_SWITCH1];
    if (obj->loaded)
    {
        obj->control = SequenceSwitchControl;
        obj->collision = SequenceSwitchCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }

    obj = &Objects[SEQUENCE_SWITCH2];
    if (obj->loaded)
    {
        obj->control = SequenceSwitchControl;
        obj->collision = SequenceSwitchCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }

    obj = &Objects[SEQUENCE_SWITCH3];
    if (obj->loaded)
    {
        obj->control = SequenceSwitchControl;
        obj->collision = SequenceSwitchCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }
}

void SetupObject::underwater_switch()
{
    obj = &Objects[UNDERWATER_SWITCH1];
    if (obj->loaded)
    {
        obj->control = SwitchControl;
        obj->collision = SwitchUnderwater1Collision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }

    obj = &Objects[UNDERWATER_SWITCH2];
    if (obj->loaded)
    {
        obj->control = SwitchControl;
        obj->collision = SwitchUnderwater2Collision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }
}

void SetupObject::lever_switch()
{
    obj = &Objects[LEVER_SWITCH];
    if (obj->loaded)
    {
        obj->control = SwitchControl;
        obj->collision = LeverSwitchCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }
}

void SetupObject::jump_switch()
{
    obj = &Objects[JUMP_SWITCH];
    if (obj->loaded)
    {
        obj->control = SwitchControl;
        obj->collision = JumpSwitchCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }
}

void SetupObject::pulley()
{
    obj = &Objects[PULLEY];
    if (obj->loaded)
    {
        obj->initialise = InitialisePulleySwitch;
        obj->control = SwitchControl;
        obj->collision = PulleySwitchCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }
}

void SetupObject::turn_switch()
{
    obj = &Objects[TURN_SWITCH];
    if (obj->loaded)
    {
        obj->control = SwitchControl;
        obj->collision = TurnSwitchCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }
}

void SetupObject::cog_switch()
{
    obj = &Objects[COG_SWITCH];
    if (obj->loaded)
    {
        obj->control = CogSwitchControl;
        obj->collision = CogSwitchCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }
}

void SetupObject::crowbar_switch()
{
    obj = &Objects[CROWBAR_SWITCH];
    if (obj->loaded)
    {
        obj->control = SwitchControl;
        obj->collision = CrowbarSwitchCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
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
    obj = &Objects[DOUBLE_DOORS];
    if (obj->loaded)
    {
        obj->initialise = InitialiseDoor;
        obj->control = DoubleDoorControl;
        obj->collision = DoubleDoorCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }
}

void SetupObject::sequence_door()
{
    obj = &Objects[SEQUENCE_DOOR1];
    if (obj->loaded)
    {
        obj->initialise = InitialiseDoor;
        obj->control = SequenceDoorControl;
        obj->collision = DoorCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }
}

void SetupObject::underwater_door()
{
    obj = &Objects[UNDERWATER_DOOR];
    if (obj->loaded)
    {
        obj->initialise = InitialiseDoor;
        obj->control = DoubleDoorControl;
        obj->collision = UnderwaterDoorCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }
}

void SetupObject::pushpull_door()
{
    obj = &Objects[PUSHPULL_DOOR1];
    if (obj->loaded)
    {
        obj->initialise = InitialiseDoor;
        obj->control = DoubleDoorControl;
        obj->collision = PushPullDoorCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }

    obj = &Objects[PUSHPULL_DOOR2];
    if (obj->loaded)
    {
        obj->initialise = InitialiseDoor;
        obj->control = DoubleDoorControl;
        obj->collision = PushPullDoorCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }

    obj = &Objects[KICK_DOOR1];
    if (obj->loaded)
    {
        obj->initialise = InitialiseDoor;
        obj->control = DoubleDoorControl;
        obj->collision = PushPullDoorCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }

    obj = &Objects[KICK_DOOR2];
    if (obj->loaded)
    {
        obj->initialise = InitialiseDoor;
        obj->control = DoubleDoorControl;
        obj->collision = PushPullDoorCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }
}

void SetupObject::floor_trapdoor()
{
    obj = &Objects[FLOOR_TRAPDOOR1];
    if (obj->loaded)
    {
        obj->initialise = InitialiseFloorCeilingTrapDoor;
        obj->control = FloorCeilingTrapDoorControl;
        obj->collision = FloorTrapDoorCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }

    obj = &Objects[FLOOR_TRAPDOOR2];
    if (obj->loaded)
    {
        obj->initialise = InitialiseFloorCeilingTrapDoor;
        obj->control = FloorCeilingTrapDoorControl;
        obj->collision = FloorTrapDoorCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }
}

void SetupObject::ceiling_trapdoor()
{
    obj = &Objects[CEILING_TRAPDOOR1];
    if (obj->loaded)
    {
        obj->initialise = InitialiseFloorCeilingTrapDoor;
        obj->control = FloorCeilingTrapDoorControl;
        obj->collision = CeilingTrapDoorCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }

    obj = &Objects[CEILING_TRAPDOOR2];
    if (obj->loaded)
    {
        obj->initialise = InitialiseFloorCeilingTrapDoor;
        obj->control = FloorCeilingTrapDoorControl;
        obj->collision = CeilingTrapDoorCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }
}

void SetupObject::trapdoor()
{
    obj = &Objects[TRAPDOOR1];
    if (obj->loaded)
    {
        obj->initialise = InitialiseFloorCeilingTrapDoor;
        obj->control = FloorCeilingTrapDoorControl;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }

    obj = &Objects[TRAPDOOR2];
    if (obj->loaded)
    {
        obj->initialise = InitialiseFloorCeilingTrapDoor;
        obj->control = FloorCeilingTrapDoorControl;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }

    obj = &Objects[TRAPDOOR3];
    if (obj->loaded)
    {
        obj->initialise = InitialiseFloorCeilingTrapDoor;
        obj->control = FloorCeilingTrapDoorControl;
        obj->saveAnim = true;
        obj->saveFlags = true;
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
    obj = &Objects[GRENADE];
    if (obj->loaded)
    {
        obj->initialise = NULL;
        obj->collision = NULL;
        obj->control = GrenadeControl;
        obj->drawRoutine = DrawProjectile;
        obj->saveFlags = true;
        obj->savePosition = true;
    }
}

void SetupObject::crossbow_bolt()
{
    obj = &Objects[CROSSBOW_BOLT];
    if (obj->loaded)
    {
        obj->initialise = NULL;
        obj->collision = NULL;
        obj->control = CrossbowBoltControl;
        obj->drawRoutine = DrawProjectile;
        obj->saveFlags = true;
        obj->savePosition = true;
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
    obj = &Objects[SARCOPHAGUS];
    if (obj->loaded)
    {
        obj->control = PuzzleControl;
        obj->collision = SarcophagusCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
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
    obj = &Objects[STATUE_PLINTH];
    if (obj->loaded)
    {
        obj->initialise = InitialiseStatuePlinth;
        obj->collision = StatuePlinthCollision;
        obj->unknown = 1;
        obj->saveFlags = true;
    }
}

void SetupObject::element_puzzle()
{
    obj = &Objects[ELEMENT_PUZZLE];
    if (obj->loaded)
    {
        obj->initialise = InitialiseElementPuzzle;
        obj->control = ElementPuzzleControl;
        obj->collision = ElementPuzzleCollision;
        obj->unknown = 1;
        obj->saveFlags = true;
    }
}

void SetupObject::expanding_platform()
{
    obj = &Objects[EXPANDING_PLATFORM];
    if (obj->loaded)
    {
        obj->initialise = InitialiseRaisingBlock;
        obj->control = RaisingBlockControl;
        obj->drawRoutine = DrawScaledObjects;
        obj->saveFlags = true;
    }
}

void SetupObject::obelisk()
{
    obj = &Objects[OBELISK];
    if (obj->loaded)
    {
        obj->initialise = InitialiseObelisk;
        obj->control = ObeliskControl;
        obj->collision = ObjectCollision;
        obj->saveFlags = true;
        obj->savePosition = true;
    }
}

void SetupObject::squishy_block()
{
    obj = &Objects[SQUISHY_BLOCK1];
    if (obj->loaded)
    {
        obj->control = SquishyBlock1Control;
        obj->collision = ObjectCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }

    obj = &Objects[SQUISHY_BLOCK2];
    if (obj->loaded)
    {
        obj->control = SquishyBlock2Control;
        obj->collision = SquishyBlock2Collision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }
}

void SetupObject::firerope()
{
    obj = &Objects[FIREROPE];
    if (obj->loaded)
    {
        obj->control = FireRopeControl;
        obj->collision = FireRopeCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }
}

void SetupObject::mapper()
{
    obj = &Objects[MAPPER];
    if (obj->loaded)
    {
        obj->initialise = InitialiseMapper;
        obj->control = MapperControl;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }
}

void SetupObject::slicer_dicer()
{
    obj = &Objects[SLICER_DICER];
    if (obj->loaded)
    {
        obj->initialise = InitialiseSlicerDicer;
        obj->control = SlicerDicerControl;
        obj->collision = MovBlockCollision;
        obj->saveFlags = true;
    }
}

void SetupObject::scales()
{
    obj = &Objects[SCALES];
    if (obj->loaded)
    {
        obj->control = ScalesControl;
        obj->collision = ScalesCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }
}

void SetupObject::sarcophagus_cut()
{
    obj = &Objects[SARCOPHAGUS_CUT];
    if (obj->loaded)
        obj->unknown = 1;
}

void SetupObject::joby_spike()
{
    obj = &Objects[JOBY_SPIKES];
    if (obj->loaded)
    {
        obj->initialise = InitialiseJobySpikes;
        obj->control = JobySpikesControl;
        obj->drawRoutine = DrawScaledObjects;
        obj->saveFlags = true;
    }
}

void SetupObject::teeth_spike()
{
    obj = &Objects[TEETH_SPIKES];
    if (obj->loaded)
    {
        obj->initialise = InitialiseTeethSpike;
        obj->control = TeethSpikeControl;
        obj->drawRoutine = DrawScaledObjects;
        obj->saveFlags = true;
    }
}

void SetupObject::horus_statue()
{
    obj = &Objects[HORUS_STATUE];
    if (obj->loaded)
    {
        obj->collision = ObjectCollision;
        obj->unknown = 1;
    }
}

void SetupObject::raising_block()
{
    obj = &Objects[RAISING_BLOCK1];
    if (obj->loaded)
    {
        obj->initialise = InitialiseRaisingBlock;
        obj->control = RaisingBlockControl;
        obj->collision = NULL;
        obj->drawRoutine = DrawScaledObjects;
        obj->saveFlags = true;
    }

    obj = &Objects[RAISING_BLOCK2];
    if (obj->loaded)
    {
        obj->initialise = InitialiseRaisingBlock;
        obj->control = RaisingBlockControl;
        obj->collision = NULL;
        obj->drawRoutine = DrawScaledObjects;
        obj->saveFlags = true;
    }
}

void SetupObject::smoke_emitter()
{
    obj = &Objects[SMOKE_EMITTER_WHITE];
    if (obj->loaded)
    {
        obj->initialise = InitialiseSmokeEmitter;
        obj->control = SmokeEmitterControl;
        obj->drawRoutine = NULL;
        obj->saveFlags = true;
    }

    obj = &Objects[SMOKE_EMITTER_BLACK];
    if (obj->loaded)
    {
        obj->initialise = InitialiseSmokeEmitter;
        obj->control = SmokeEmitterControl;
        obj->drawRoutine = NULL;
        obj->saveFlags = true;
    }

    obj = &Objects[STEAM_EMITTER];
    if (obj->loaded)
    {
        obj->initialise = InitialiseSmokeEmitter;
        obj->control = SmokeEmitterControl;
        obj->drawRoutine = NULL;
        obj->saveFlags = true;
    }
}

void SetupObject::red_green_blue_light()
{
    obj = &Objects[RED_LIGHT];
    if (obj->loaded)
    {
        obj->control = LightControl;
        obj->drawRoutine = NULL;
        obj->saveFlags = true;
    }

    obj = &Objects[GREEN_LIGHT];
    if (obj->loaded)
    {
        obj->control = LightControl;
        obj->drawRoutine = NULL;
        obj->saveFlags = true;
    }

    obj = &Objects[BLUE_LIGHT];
    if (obj->loaded)
    {
        obj->control = LightControl;
        obj->drawRoutine = NULL;
        obj->saveFlags = true;
    }
}

void SetupObject::blinking_light()
{
    obj = &Objects[BLINKING_LIGHT];
    if (obj->loaded)
    {
        obj->control = BlinkingLightControl;
        obj->saveFlags = true;
    }
}

void SetupObject::lens_flare()
{
    obj = &Objects[LENS_FLARE];
    if (obj->loaded)
        obj->drawRoutine = DrawLensFlare;
}

void SetupObject::amber_light()
{
    obj = &Objects[AMBER_LIGHT];
    if (obj->loaded)
    {
        obj->control = AmberLightControl;
        obj->drawRoutine = NULL;
        obj->saveFlags = true;
    }
}

void SetupObject::bubble_effect()
{
    obj = &Objects[BUBBLES];
    obj->loaded = true;
    obj->control = BubblesControl;
    obj->drawRoutine = (LPDRAW)true;
    obj->nMeshes = 0;
}

void SetupObject::waterfallmist()
{
    obj = &Objects[WATERFALLMIST];
    if (obj->loaded)
    {
        obj->control = WaterfallMistControl;
        obj->drawRoutine = NULL;
    }
}

void SetupObject::waterfall()
{
    obj = &Objects[WATERFALL1];
    if (obj->loaded)
    {
        obj->control = WaterfallControl;
        obj->saveFlags = true;
    }

    obj = &Objects[WATERFALL2];
    if (obj->loaded)
    {
        obj->control = WaterfallControl;
        obj->saveFlags = true;
    }

    obj = &Objects[WATERFALL3];
    if (obj->loaded)
    {
        obj->control = WaterfallControl;
        obj->saveFlags = true;
    }
}

void SetupObject::lightning_conductor()
{
    obj = &Objects[LIGHTNING_CONDUCTOR];
    if (obj->loaded)
    {
        obj->initialise = InitialiseLightningConductor;
        obj->control = LightningConductorControl;
        obj->drawRoutine = NULL;
        obj->saveFlags = true;
    }
}

void SetupObject::white_light()
{
    obj = &Objects[WHITE_LIGHT];
    if (obj->loaded)
    {
        obj->control = WhiteLightControl;
        obj->drawRoutine = NULL;
        obj->saveFlags = true;
    }
}

void SetupObject::body_part()
{
    obj = &Objects[BODY_PART];
    obj->loaded = true;
    obj->nMeshes = 0;
    obj->control = BodyPartControl;
    obj->drawRoutine = (LPDRAW)TRUE;
}

void SetupObject::earthquake()
{
    obj = &Objects[EARTHQUAKE];
    if (obj->loaded)
    {
        obj->control = EarthquakeControl;
        obj->drawRoutine = NULL;
        obj->saveFlags = true;
    }
}

void SetupObject::clockwork_beetle()
{
    obj = &Objects[CLOCKWORK_BEETLE];
    if (obj->loaded)
    {
        obj->initialise = NULL;
        obj->control = ClockworkBeetleControl;
    }
}

void SetupObject::god_head()
{
    obj = &Objects[GOD_HEAD];
    if (obj->loaded)
    {
        obj->control = GodHeadControl;
        obj->drawRoutine = DrawGodHead;
        obj->saveFlags = true;
    }
}
