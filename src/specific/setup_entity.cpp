#include "framework.h"
#include "setup_entity.h"
#include "game/collide.h"
#include "game/box.h"
#include "game/laramisc.h"
#include "game/oldobjects.h"
#include "specific/json/reader.h"
#include "utils/utils.h"

SetupEntity setup_entity;
void SetupEntity::lara_obj()
{
    obj = &objects[LARA];
    if (obj->loaded)
    {
        obj->initialise = InitialiseLaraLoad;
        obj->shadow_size = 160;
        obj->hit_points = initHealth(LARA);
        obj->hit_effect = HIT_BLOOD;
        obj->draw_routine = NULL; // custom render elsewhere !
        obj->using_drawanimating_item = false;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_hitpoints = true;
        obj->save_position = true;
    }
    else
    {
        MessageBox(NULL, "[BaddyObjects] LARA object is not loaded ! please include lara in your project !", "Tomb Raider 4", BOX_DEFFLAG(MB_ICONERROR));
    }
}

void SetupEntity::motor_bike()
{
    obj = &objects[MOTORBIKE];
    if (obj->loaded)
    {
        obj->initialise = InitialiseMotorBike;
        obj->control = MotorBikeControl;
        obj->collision = MotorBikeCollision;
        obj->draw_routine_extra = MotorBikeEffect;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_hitpoints = true;
        obj->save_position = true;
        bones[obj->bone_index + 4] |= ROT_X;
        bones[obj->bone_index + 12] |= ROT_X;
        bones[obj->bone_index + 28] |= ROT_X;
    }
}

void SetupEntity::jeep()
{
    obj = &objects[JEEP];
    if (obj->loaded)
    {
        obj->initialise = InitialiseJeep;
        obj->control = JeepControl;
        obj->collision = JeepCollision;
        obj->draw_routine_extra = DrawJeepEffect;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_hitpoints = true;
        obj->save_position = true;
        bones[obj->bone_index + 32] |= ROT_X;
        bones[obj->bone_index + 36] |= ROT_X;
        bones[obj->bone_index + 44] |= ROT_X;
        bones[obj->bone_index + 48] |= ROT_X;
    }
}

void SetupEntity::skeleton()
{
    obj = &objects[SKELETON];
    if (obj->loaded)
    {
        obj->initialise = InitialiseSkeleton;
        obj->control = SkeletonControl;
        obj->collision = CreatureCollision;
        obj->hit_points = initHealth(SKELETON);
        obj->shadow_size = 128;
        obj->pivot_length = 50;
        obj->radius = 128;
        obj->object_mip = 5120;
        obj->intelligent = true;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_hitpoints = true;
        obj->save_position = true;
        obj->undead = true;
        obj->hit_effect = HIT_SMOKE;
        obj->explodable_meshbits = 2560;
    }
}

void SetupEntity::von_croy()
{
    obj = &objects[VON_CROY];
    if (obj->loaded)
    {
        obj->initialise = InitialiseVonCroy;
        if (LevelNow == 1) // Level1: Anghor Wat
            obj->control = VonCroyControlTutorial;
        else
            obj->control = VonCroyControlNormal;
        obj->collision = CreatureCollision;
        obj->pivot_length = 0;
        obj->hit_points = initHealth(VON_CROY);
        obj->shadow_size = 128;
        obj->radius = 128;
        obj->intelligent = true;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_hitpoints = true;
        obj->save_position = true;
        obj->object_mip = 5120;
        obj->explodable_meshbits = 0x200000;
        obj->hit_effect = HIT_BLOOD;
        bones[obj->bone_index + 24] |= ROT_Y | ROT_X;
        bones[obj->bone_index + 80] |= ROT_Y | ROT_X;
        meshes[obj->mesh_index + 15] = meshes[objects[MESHSWAP1].mesh_index + 14];
        meshes[obj->mesh_index + 31] = meshes[objects[MESHSWAP1].mesh_index + 30];
        meshes[obj->mesh_index + 37] = meshes[objects[MESHSWAP1].mesh_index + 36];
    }

    obj = &objects[VON_CROY_MIP];
    if (obj->loaded)
    {
        meshes[obj->mesh_index + 15] = meshes[objects[MESHSWAP1].mesh_index + 14];
        meshes[obj->mesh_index + 31] = meshes[objects[MESHSWAP1].mesh_index + 30];
        meshes[obj->mesh_index + 37] = meshes[objects[MESHSWAP1].mesh_index + 36];
    }
}

void SetupEntity::guide()
{
    obj = &objects[GUIDE];
    if (obj->loaded)
    {
        obj->initialise = InitialiseGuide;
        obj->control = GuideControl;
        obj->collision = CreatureCollision;
        obj->shadow_size = 128;
        obj->hit_points = initHealth(GUIDE);
        obj->pivot_length = 0;
        obj->radius = 128;
        obj->object_mip = 5120;
        obj->intelligent = true;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_hitpoints = true;
        obj->save_mesh = true;
        obj->save_position = true;
        obj->hit_effect = HIT_SMOKE;
        bones[obj->bone_index + 24] |= ROT_Y | ROT_X;
        bones[obj->bone_index + 80] |= ROT_Y | ROT_X;
        meshes[obj->mesh_index + 31] = meshes[objects[MESHSWAP2].mesh_index + 30];
        meshes[obj->mesh_index + 37] = meshes[objects[MESHSWAP2].mesh_index + 36];
        meshes[obj->mesh_index + 43] = meshes[objects[MESHSWAP2].mesh_index + 42];
    }

    obj = &objects[GUIDE_MIP];
    if (obj->loaded)
    {
        meshes[obj->mesh_index + 31] = meshes[objects[MESHSWAP2].mesh_index + 30];
        meshes[obj->mesh_index + 37] = meshes[objects[MESHSWAP2].mesh_index + 36];
        meshes[obj->mesh_index + 43] = meshes[objects[MESHSWAP2].mesh_index + 42];
    }
}

void SetupEntity::baddy_1()
{
    obj = &objects[BADDY_1];
    if (obj->loaded)
    {
        obj->bit_offset = 1;
        obj->initialise = InitialiseBaddy1;
        obj->control = Baddy1Control;
        obj->draw_routine = DrawBaddy1; // custom render to switch between uzi & sword.
        obj->collision = CreatureCollision;
        obj->shadow_size = 128;
        obj->hit_points = initHealth(BADDY_1);
        obj->pivot_length = 50;
        obj->radius = 102;
        obj->object_mip = 5120;
        obj->intelligent = true;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_hitpoints = true;
        obj->save_position = true;
        obj->using_drawanimating_item = false;
        obj->hit_effect = HIT_BLOOD;
        bones[obj->bone_index + 7 * 4]  |= ROT_Y | ROT_X; // TORSO
        bones[obj->bone_index + 22 * 4] |= ROT_Y | ROT_X; // HEAD

        if (objects[BADDY_2].loaded)
            obj->anim_index = objects[BADDY_2].anim_index;
    }

    obj = &objects[BADDY_1_MIP];
    if (obj->loaded)
    {
        meshes[obj->mesh_index + 4 * 2] = meshes[objects[MESHSWAP3].mesh_index + 4 * 2]; // enable swap ?
        meshes[obj->mesh_index + 7 * 2] = meshes[objects[MESHSWAP3].mesh_index + 7 * 2]; //
    }
}

void SetupEntity::baddy_2()
{
    obj = &objects[BADDY_2];
    if (obj->loaded)
    {
        obj->bit_offset = 1;
        obj->initialise = InitialiseBaddy;
        obj->control = BaddyControl;
        obj->collision = CreatureCollision;
        obj->shadow_size = 128;
        obj->hit_points = initHealth(BADDY_2);
        obj->pivot_length = 50;
        obj->radius = 102;
        obj->object_mip = 5120;
        obj->intelligent = true;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_hitpoints = true;
        obj->save_position = true;
        obj->hit_effect = HIT_BLOOD;
        bones[obj->bone_index + 28] |= ROT_Y | ROT_X;
        bones[obj->bone_index + 88] |= ROT_Y | ROT_X;

        // TODO: swap not working correctly !
        meshes[obj->mesh_index + 4 * 2] = meshes[objects[MESHSWAP2].mesh_index + 4 * 2]; // enable swap ?
        meshes[obj->mesh_index + 7 * 2] = meshes[objects[MESHSWAP2].mesh_index + 7 * 2]; //
    }

    obj = &objects[BADDY_2_MIP];
    if (obj->loaded)
    {
        meshes[obj->mesh_index + 4 * 2] = meshes[objects[MESHSWAP2].mesh_index + 4 * 2]; // enable swap ?
        meshes[obj->mesh_index + 7 * 2] = meshes[objects[MESHSWAP2].mesh_index + 7 * 2]; //
    }
}

void SetupEntity::scorpion()
{
    obj = &objects[SCORPION];
    if (obj->loaded)
    {
        obj->initialise = InitialiseScorpion;
        obj->control = ScorpionControl;
        obj->collision = CreatureCollision;
        obj->shadow_size = 128;
        obj->hit_points = initHealth(SCORPION);
        obj->pivot_length = 50;
        obj->radius = 512;
        obj->object_mip = 5120;
        obj->intelligent = true;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_hitpoints = true;
        obj->save_position = true;
        obj->hit_effect = HIT_BLOOD;
    }
}

void SetupEntity::small_scorpion()
{
    obj = &objects[SMALL_SCORPION];
    if (obj->loaded)
    {
        obj->initialise = InitialiseSmallScorpion;
        obj->control = SmallScorpionControl;
        obj->collision = CreatureCollision;
        obj->shadow_size = 128;
        obj->hit_points = initHealth(SMALL_SCORPION);
        obj->pivot_length = 20;
        obj->radius = 128;
        obj->intelligent = true;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_hitpoints = true;
        obj->save_position = true;
        obj->hit_effect = HIT_BLOOD;
    }
}

void SetupEntity::mummy()
{
    obj = &objects[MUMMY];
    if (obj->loaded)
    {
        obj->initialise = InitialiseMummy;
        obj->control = MummyControl;
        obj->collision = CreatureCollision;
        obj->shadow_size = 128;
        obj->hit_points = initHealth(MUMMY);
        obj->radius = 170;
        obj->object_mip = 5120;
        obj->intelligent = true;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_hitpoints = true;
        obj->save_position = true;
        obj->hit_effect = HIT_SMOKE;
        obj->undead = true;
        bones[obj->bone_index + 28] |= ROT_Y | ROT_X;
        bones[obj->bone_index + 72] |= ROT_Y;
    }
}

void SetupEntity::knight_templar()
{
    obj = &objects[KNIGHTS_TEMPLAR];
    if (obj->loaded)
    {
        obj->initialise = InitialiseKnightTemplar;
        obj->control = KnightTemplarControl;
        obj->collision = CreatureCollision;
        obj->shadow_size = 128;
        obj->hit_points = initHealth(KNIGHTS_TEMPLAR);
        obj->pivot_length = 0;
        obj->radius = 128;
        obj->object_mip = 5120;
        obj->intelligent = true;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_hitpoints = true;
        obj->save_position = true;
        obj->undead = true;
        bones[obj->bone_index + 24] |= ROT_Y | ROT_X;
        bones[obj->bone_index + 56] |= ROT_Y;
    }
}

void SetupEntity::sphinx()
{
    obj = &objects[SPHINX];
    if (obj->loaded)
    {
        obj->initialise = InitialiseSphinx;
        obj->control = SphinxControl;
        obj->collision = CreatureCollision;
        obj->shadow_size = 128;
        obj->hit_points = initHealth(SPHINX);
        obj->pivot_length = 500;
        obj->radius = 512;
        obj->object_mip = 5120;
        obj->hit_effect = HIT_FRAGMENT;
        obj->intelligent = true;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_hitpoints = true;
        obj->save_position = true;
        obj->undead = true;
    }
}

void SetupEntity::setha()
{
    obj = &objects[SETHA];
    if (obj->loaded)
    {
        obj->initialise = InitialiseSetha;
        obj->control = SethaControl;
        obj->collision = CreatureCollision;
        obj->shadow_size = 128;
        obj->hit_points = initHealth(SETHA);
        obj->pivot_length = 50;
        obj->radius = 341;
        obj->intelligent = true;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_hitpoints = true;
        obj->save_mesh = true;
        obj->save_position = true;
        obj->object_mip = 5120;
    }
}

void SetupEntity::lara_double()
{
    obj = &objects[LARA_DOUBLE];
    if (obj->loaded)
    {
        obj->initialise = InitialiseCreature;
        obj->control = LaraDoubleControl;
        obj->collision = CreatureCollision;
        obj->shadow_size = 128;
        obj->hit_points = initHealth(LARA_DOUBLE);
        obj->pivot_length = 50;
        obj->radius = 128;
        obj->intelligent = true;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_hitpoints = true;
        obj->save_position = true;
        obj->undead = true;
        obj->hit_effect = HIT_FRAGMENT;
    }
}

void SetupEntity::horseman()
{
    obj = &objects[HORSEMAN];
    if (obj->loaded)
    {
        obj->initialise = InitialiseHorseMan;
        obj->control = HorseManControl;
        obj->collision = CreatureCollision;
        obj->shadow_size = 128;
        obj->hit_points = initHealth(HORSEMAN);
        obj->pivot_length = 50;
        obj->radius = 409;
        obj->object_mip = 5120;
        obj->intelligent = true;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_hitpoints = true;
        obj->save_mesh = true;
        obj->save_position = true;
        obj->undead = true;
        obj->hit_effect = HIT_FRAGMENT;
    }
}

void SetupEntity::hammerhead()
{
    obj = &objects[HAMMERHEAD];
    if (obj->loaded)
    {
        obj->initialise = InitialiseHammerhead;
        obj->control = HammerheadControl;
        obj->collision = CreatureCollision;
        obj->shadow_size = 128;
        obj->hit_points = initHealth(HAMMERHEAD);
        obj->pivot_length = 300;
        obj->radius = 341;
        obj->object_mip = 5120;
        obj->intelligent = true;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_hitpoints = true;
        obj->save_position = true;
        obj->water_creature = true;
        obj->hit_effect = HIT_BLOOD;
        bones[obj->bone_index + 0] |= ROT_Y;
        bones[obj->bone_index + 4] |= ROT_Y;
        bones[obj->bone_index + 8] |= ROT_Y;
        bones[obj->bone_index + 36] |= ROT_Y;
    }
}

void SetupEntity::crocodile()
{
    obj = &objects[CROCODILE];
    if (obj->loaded)
    {
        obj->initialise = InitialiseCrocodile;
        obj->control = CrocodileControl;
        obj->collision = CreatureCollision;
        obj->shadow_size = 128;
        obj->hit_points = initHealth(CROCODILE);
        obj->pivot_length = 300;
        obj->radius = 402;
        obj->object_mip = 5120;
        obj->intelligent = true;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_hitpoints = true;
        obj->save_position = true;
        obj->water_creature = true;
        obj->hit_effect = HIT_BLOOD;
        bones[obj->bone_index + 0] |= ROT_Y;
        bones[obj->bone_index + 28] |= ROT_Y;
        bones[obj->bone_index + 36] |= ROT_Y;
        bones[obj->bone_index + 40] |= ROT_Y;
    }
}

void SetupEntity::demigod1()
{
    obj = &objects[DEMIGOD1];
    if (obj->loaded)
    {
        obj->initialise = InitialiseDemigod;
        obj->control = DemigodControl;
        obj->collision = CreatureCollision;
        obj->shadow_size = 128;
        obj->hit_points = initHealth(DEMIGOD1);
        obj->pivot_length = 0;
        obj->radius = 341;
        obj->object_mip = 5120;
        obj->intelligent = true;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_hitpoints = true;
        obj->save_position = true;
        //obj->non_lot = true;
        obj->hit_effect = HIT_BLOOD;
        bones[obj->bone_index + 32] |= ROT_ALL;
        bones[obj->bone_index + 80] |= ROT_Y;
    }
}

void SetupEntity::demigod2()
{
    obj = &objects[DEMIGOD2];
    if (obj->loaded)
    {
        obj->initialise = InitialiseDemigod;
        obj->control = DemigodControl;
        obj->collision = CreatureCollision;
        obj->shadow_size = 128;
        obj->hit_points = initHealth(DEMIGOD2);
        obj->pivot_length = 50;
        obj->radius = 341;
        obj->object_mip = 5120;
        obj->intelligent = true;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_hitpoints = true;
        obj->save_position = true;
        //obj->non_lot = true;
        obj->hit_effect = HIT_BLOOD;
        bones[obj->bone_index + 32] |= ROT_ALL;
        bones[obj->bone_index + 80] |= ROT_Y;
    }
}

void SetupEntity::demigod3()
{
    obj = &objects[DEMIGOD3];
    if (obj->loaded)
    {
        obj->initialise = InitialiseDemigod;
        obj->control = DemigodControl;
        obj->collision = CreatureCollision;
        obj->shadow_size = 128;
        obj->hit_points = initHealth(DEMIGOD3);
        obj->pivot_length = 50;
        obj->radius = 341;
        obj->object_mip = 5120;
        obj->intelligent = true;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_hitpoints = true;
        obj->save_position = true;
        //obj->non_lot = true;
        obj->hit_effect = HIT_BLOOD;
        bones[obj->bone_index + 32] |= ROT_ALL;
        bones[obj->bone_index + 80] |= ROT_Y;
    }
}

void SetupEntity::mutant()
{
    obj = &objects[MUTANT];
    if (obj->loaded)
    {
        obj->initialise = InitialiseMutant;
        obj->control = MutantControl;
        obj->collision = CreatureCollision;
        obj->shadow_size = 128;
        obj->hit_points = initHealth(MUTANT);
        obj->pivot_length = 50;
        obj->radius = 128;
        obj->intelligent = true;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_mesh = true;
        obj->save_position = true;
        obj->undead = true;
        obj->hit_effect = HIT_SMOKE;
        bones[obj->bone_index + 24] |= ROT_Y | ROT_X;
        bones[obj->bone_index + 28] |= ROT_Y | ROT_X;
    }
}

void SetupEntity::troops()
{
    obj = &objects[TROOPS];
    if (obj->loaded)
    {
        obj->bit_offset = 0;
        obj->initialise = InitialiseTroops;
        obj->control = TroopsControl;
        obj->collision = CreatureCollision;
        obj->shadow_size = 128;
        obj->hit_points = initHealth(TROOPS);
        obj->pivot_length = 50;
        obj->radius = 102;
        obj->object_mip = 5120;
        obj->intelligent = true;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_hitpoints = true;
        obj->save_position = true;
        obj->hit_effect = HIT_BLOOD;
        bones[obj->bone_index + 0] |= ROT_Y | ROT_X;
        bones[obj->bone_index + 28] |= ROT_Y | ROT_X;
    }
}

void SetupEntity::sas()
{
    obj = &objects[SAS];
    if (obj->loaded)
    {
        obj->bit_offset = 0;
        obj->initialise = InitialiseSAS;
        obj->control = SASControl;
        obj->collision = CreatureCollision;
        obj->shadow_size = 128;
        obj->hit_points = initHealth(SAS);
        obj->pivot_length = 50;
        obj->radius = 102;
        obj->object_mip = 5120;
        obj->intelligent = true;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_hitpoints = true;
        obj->save_position = true;
        obj->hit_effect = HIT_BLOOD;
        bones[obj->bone_index + 0] |= ROT_Y | ROT_X;
        bones[obj->bone_index + 28] |= ROT_Y | ROT_X;
    }
}

void SetupEntity::harpy()
{
    obj = &objects[HARPY];
    if (obj->loaded)
    {
        obj->initialise = InitialiseHarpy;
        obj->control = HarpyControl;
        obj->collision = CreatureCollision;
        obj->shadow_size = 128;
        obj->hit_points = initHealth(HARPY);
        obj->pivot_length = 50;
        obj->radius = 409;
        obj->object_mip = 5120;
        obj->intelligent = true;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_hitpoints = true;
        obj->save_position = true;
        obj->hit_effect = HIT_BLOOD;
    }
}

void SetupEntity::wild_boar()
{
    obj = &objects[WILD_BOAR];
    if (obj->loaded)
    {
        obj->initialise = InitialiseWildBoar;
        obj->control = WildBoarControl;
        obj->collision = CreatureCollision;
        obj->shadow_size = 128;
        obj->hit_points = initHealth(WILD_BOAR);
        obj->pivot_length = 50;
        obj->radius = 102;
        obj->intelligent = true;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_hitpoints = true;
        obj->save_position = true;
        obj->hit_effect = HIT_BLOOD;
        bones[obj->bone_index + 48] |= ROT_Z | ROT_Y;
        bones[obj->bone_index + 52] |= ROT_Z | ROT_Y;
    }
}

void SetupEntity::dog()
{
    obj = &objects[DOG];
    if (obj->loaded)
    {
        obj->initialise = InitialiseDog;
        obj->control = DogControl;
        obj->collision = CreatureCollision;
        obj->shadow_size = 128;
        obj->hit_points = initHealth(DOG);
        obj->pivot_length = 300;
        obj->radius = 341;
        obj->object_mip = 5120;
        obj->intelligent = true;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_hitpoints = true;
        obj->save_position = true;
        obj->hit_effect = HIT_BLOOD;
        bones[obj->bone_index + 0] |= ROT_Y;
        bones[obj->bone_index + 8] |= ROT_Y | ROT_X;
    }
}

void SetupEntity::ahmet()
{
    obj = &objects[AHMET];
    if (obj->loaded)
    {
        obj->initialise = InitialiseAhmet;
        obj->control = AhmetControl;
        obj->collision = CreatureCollision;
        obj->shadow_size = 128;
        obj->hit_points = initHealth(AHMET);
        obj->pivot_length = 300;
        obj->radius = 341;
        obj->object_mip = 5120;
        obj->intelligent = true;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_hitpoints = true;
        obj->save_position = true;
        obj->hit_effect = HIT_BLOOD;
        bones[obj->bone_index + 36] |= ROT_Y;
    }
}

void SetupEntity::baboon_normal()
{
    obj = &objects[BABOON_NORMAL];
    if (obj->loaded)
    {
        obj->initialise = InitialiseBaboon;
        obj->control = BaboonControl;
        obj->collision = CreatureCollision;
        obj->shadow_size = 128;
        obj->hit_points = initHealth(BABOON_NORMAL);
        obj->pivot_length = 200;
        obj->radius = 256;
        obj->intelligent = true;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_hitpoints = true;
        obj->save_position = true;
        obj->hit_effect = HIT_BLOOD;
    }
}

void SetupEntity::baboon_silent()
{
    obj = &objects[BABOON_SILENT];
    if (obj->loaded)
    {
        obj->initialise = InitialiseBaboon;
        obj->control = BaboonControl;
        obj->collision = CreatureCollision;
        obj->shadow_size = 128;
        obj->hit_points = initHealth(BABOON_SILENT);
        obj->pivot_length = 200;
        obj->radius = 256;
        obj->intelligent = true;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_hitpoints = true;
        obj->save_position = true;
        obj->hit_effect = HIT_BLOOD;

        if (objects[BABOON_NORMAL].loaded)
            objects[BABOON_SILENT].anim_index = objects[BABOON_NORMAL].anim_index;
    }
}

void SetupEntity::baboon_inv()
{
    obj = &objects[BABOON_INV];
    if (obj->loaded)
    {
        obj->initialise = InitialiseBaboon;
        obj->control = BaboonControl;
        obj->collision = CreatureCollision;
        obj->shadow_size = 128;
        obj->hit_points = initHealth(BABOON_INV);
        obj->pivot_length = 200;
        obj->radius = 256;
        obj->intelligent = true;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_hitpoints = true;
        obj->save_position = true;
        obj->hit_effect = HIT_BLOOD;

        if (objects[BABOON_NORMAL].loaded)
            objects[BABOON_SILENT].anim_index = objects[BABOON_NORMAL].anim_index;
    }
}

// can disappear like nothing !!
void SetupEntity::enemy_jeep()
{
    obj = &objects[ENEMY_JEEP];
    if (obj->loaded)
    {
        // the train level consider it as not a entity !
        if (gf_level_flags & SLEV_TRAIN)
        {
            obj->initialise = InitialiseEnemyJeepTrain;
            obj->control = EnemyJeepTrainControl;
            obj->collision = CreatureCollision;
            obj->shadow_size = 128; // not exist in TR4 Train level by default !!
            obj->save_anim = true;
            obj->save_flags = true;
            obj->save_hitpoints = true;
            obj->save_position = true;
            obj->hit_effect = HIT_FRAGMENT;
        }
        else
        {
            obj->initialise = InitialiseEnemyJeep;
            obj->control = EnemyJeepControl;
            obj->collision = CreatureCollision;
            obj->shadow_size = 128;
            obj->hit_points = initHealth(ENEMY_JEEP);
            obj->pivot_length = 500;
            obj->radius = 512;
            obj->intelligent = true;
            obj->save_anim = true;
            obj->save_flags = true;
            obj->save_hitpoints = true;
            obj->save_mesh = true;
            obj->save_position = true;
            obj->hit_effect = HIT_FRAGMENT;
            bones[obj->bone_index + 32] |= ROT_X;
            bones[obj->bone_index + 36] |= ROT_X;
            bones[obj->bone_index + 44] |= ROT_X;
            bones[obj->bone_index + 48] |= ROT_X;
        }
    }
}

void SetupEntity::bat()
{
    obj = &objects[BAT];
    if (obj->loaded)
    {
        obj->initialise = InitialiseBat;
        obj->control = BatControl;
        obj->collision = CreatureCollision;
        obj->shadow_size = 128;
        obj->hit_points = initHealth(BAT);
        obj->pivot_length = 10;
        obj->radius = 102;
        obj->intelligent = true;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_hitpoints = true;
        obj->save_position = true;
        obj->hit_effect = HIT_BLOOD;
    }
}

void SetupEntity::big_beetle()
{
    obj = &objects[BIG_BEETLE];
    if (obj->loaded)
    {
        obj->initialise = InitialiseBigBeetle;
        obj->control = BigBeetleControl;
        obj->collision = CreatureCollision;
        obj->shadow_size = 128;
        obj->hit_points = initHealth(BIG_BEETLE);
        obj->pivot_length = 50;
        obj->radius = 204;
        obj->object_mip = 5120;
        obj->intelligent = true;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_hitpoints = true;
        obj->save_position = true;
        obj->hit_effect = HIT_BLOOD;
    }
}

void SetupEntity::sentry_gun()
{
    obj = &objects[SENTRY_GUN];
    if (obj->loaded)
    {
        obj->initialise = InitialiseSentryGun;
        obj->control = SentryGunControl;
        obj->collision = CreatureCollision;
        obj->shadow_size = 128;
        obj->hit_points = initHealth(SENTRY_GUN);
        obj->pivot_length = 50;
        obj->radius = 204;
        obj->explodable_meshbits = 64;
        obj->intelligent = true;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_hitpoints = true;
        obj->save_mesh = true;
        obj->save_position = true;
        obj->undead = true; // only killable by grenadegun or crossbow
        obj->hit_effect = HIT_FRAGMENT;
        bones[obj->bone_index + 0] |= ROT_Y;
        bones[obj->bone_index + 4] |= ROT_X;
        bones[obj->bone_index + 8] |= ROT_Z;
        bones[obj->bone_index + 12] |= ROT_Z;
    }
}

void SetupEntity::horse()
{
    obj = &objects[HORSE];
    if (obj->loaded)
    {
        obj->initialise = InitialiseHorse;
        obj->collision = ObjectCollision;
        obj->save_anim = true;
        obj->save_flags = true;
    }
}

void SetupEntity::sas_dying()
{
    obj = &objects[SAS_DYING];
    if (obj->loaded)
    {
        obj->initialise = InitialiseSasDying;
        obj->control = SasDyingControl;
        obj->collision = ObjectCollision;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_position = true;
    }
}

void SetupEntity::sas_captain()
{
    obj = &objects[SAS_CAPTAIN];
    if (obj->loaded)
    {
        obj->initialise = InitialiseSasDying;
        obj->control = SasDyingControl;
        obj->collision = ObjectCollision;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_position = true;
    }
}

void SetupEntity::jean_yves()
{
    obj = &objects[JEAN_YVES];
    if (obj->loaded)
    {
        obj->initialise = InitialiseJeanYves;
        obj->control = JeanYvesControl;
        obj->collision = ObjectCollision;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_position = true;
        obj->object_mip = 10240;
    }
}

void SetupEntity::game_piece1()
{
    obj = &objects[GAME_PIECE1];
    if (obj->loaded)
    {
        obj->initialise = InitialiseGamePiece;
        obj->control = GamePieceControl;
        obj->collision = ObjectCollision;
        obj->save_hitpoints = true;
        obj->save_flags = true;
        obj->save_position = true;
    }
}

void SetupEntity::game_piece2()
{
    obj = &objects[GAME_PIECE2];
    if (obj->loaded)
    {
        obj->initialise = InitialiseGamePiece;
        obj->control = GamePieceControl;
        obj->collision = ObjectCollision;
        obj->save_hitpoints = true;
        obj->save_flags = true;
        obj->save_position = true;
    }
}

void SetupEntity::game_piece3()
{
    obj = &objects[GAME_PIECE3];
    if (obj->loaded)
    {
        obj->initialise = InitialiseGamePiece;
        obj->control = GamePieceControl;
        obj->collision = ObjectCollision;
        obj->save_hitpoints = true;
        obj->save_flags = true;
        obj->save_position = true;
    }
}

void SetupEntity::enemy_piece()
{
    obj = &objects[ENEMY_PIECE];
    if (obj->loaded)
    {
        obj->collision = ObjectCollision;
        obj->save_flags = true;
        obj->save_position = true;
    }
}

void SetupEntity::wheel_of_fortune()
{
    obj = &objects[WHEEL_OF_FORTUNE];
    if (obj->loaded)
    {
        obj->initialise = InitialiseWheelOfFortune;
        obj->control = WheelOfFortuneControl;
        obj->collision = WheelOfFortuneCollision;
        obj->hit_points = 1;
        obj->save_anim = true;
        obj->save_flags = true;
        bones[obj->bone_index + 0] |= ROT_Z;
        bones[obj->bone_index + 4] |= ROT_Z;
        bones[obj->bone_index + 8] |= ROT_Z;
        bones[obj->bone_index + 12] |= ROT_Z;
    }
}

void SetupEntity::wraith1()
{
    obj = &objects[WRAITH1];
    if (obj->loaded)
    {
        obj->initialise = InitialiseWraith;
        obj->control = WraithControl;
        obj->draw_routine = DrawWraith;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_hitpoints = true;
        obj->save_position = true;
    }
}

void SetupEntity::wraith2()
{
    obj = &objects[WRAITH2];
    if (obj->loaded)
    {
        obj->initialise = InitialiseWraith;
        obj->control = WraithControl;
        obj->draw_routine = DrawWraith;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_hitpoints = true;
        obj->save_position = true;
    }
}

void SetupEntity::wraith3()
{
    obj = &objects[WRAITH3];
    if (obj->loaded)
    {
        obj->initialise = InitialiseWraith;
        obj->control = WraithControl;
        obj->draw_routine = DrawWraith;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_hitpoints = true;
        obj->save_position = true;
    }
}

void SetupEntity::wraith4()
{
    obj = &objects[WRAITH4];
    if (obj->loaded)
    {
        obj->initialise = InitialiseWraith;
        obj->control = WraithControl;
        obj->draw_routine = DrawWraith;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_hitpoints = true;
        obj->save_position = true;
    }
}

void SetupEntity::little_beetle()
{
    obj = &objects[LITTLE_BEETLE];
    if (obj->loaded)
    {
        obj->initialise = InitialiseLittleBeetle;
        obj->control = LittleBeetleControl;
        obj->draw_routine = NULL;
    }
}

void SetupEntity::fish()
{
    obj = &objects[FISH];
    if (obj->loaded)
    {
        obj->initialise = InitialiseFish;
        obj->control = FishControl;
        obj->draw_routine = NULL;
        obj->save_flags = true;
    }
}

void injector::inject_entity()
{
    this->inject(0x00402D90, InitialiseCrocodile);
    this->inject(0x00402E30, CrocodileControl);
}