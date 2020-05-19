#include "framework.h"
#include "setup_entity.h"
#include "collide.h"
#include "box.h"
#include "lara.h"
#include "gameflow.h"
#include "utils.h"
#include "oldobjects.h"

SetupEntity setup_entity;
void SetupEntity::lara_obj()
{
    obj = &Objects[LARA];
    if (obj->loaded)
    {
        obj->initialise = InitialiseLaraLoad;
        obj->shadowSize = 160;
        obj->hitPoints = initHealth(LARA);
        obj->hitEffect = HIT_BLOOD;
        obj->drawRoutine = NULL; // custom render elsewhere !
        obj->usingDrawAnimatingItem = false;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->saveHitpoints = true;
        obj->savePosition = true;
    }
    else
    {
        MessageBox(NULL, "[BaddyObjects] LARA object is not loaded ! please include lara in your project !", "Tomb Raider 4", BOX_DEFFLAG(MB_ICONERROR));
    }
}

void SetupEntity::motor_bike()
{
    obj = &Objects[MOTORBIKE];
    if (obj->loaded)
    {
        obj->initialise = InitialiseMotorBike;
        obj->control = NULL;
        obj->collision = MotorBikeCollision;
        obj->drawRoutineExtra = DrawMotorBikeEffect;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->saveHitpoints = true;
        obj->savePosition = true;
        
        bones[obj->boneIndex + 4]  |= ROT_X;
        bones[obj->boneIndex + 12] |= ROT_X;
        bones[obj->boneIndex + 28] |= ROT_X;
    }
}

void SetupEntity::jeep()
{
    obj = &Objects[JEEP];
    if (obj->loaded)
    {
        obj->initialise = InitialiseJeep;
        obj->control = NULL;
        obj->collision = JeepCollision;
        obj->drawRoutineExtra = DrawJeepEffect;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->saveHitpoints = true;
        obj->savePosition = true;
        
        bones[obj->boneIndex + 32] |= ROT_X;
        bones[obj->boneIndex + 36] |= ROT_X;
        bones[obj->boneIndex + 44] |= ROT_X;
        bones[obj->boneIndex + 48] |= ROT_X;
    }
}

void SetupEntity::skeleton()
{
    obj = &Objects[SKELETON];
    if (obj->loaded)
    {
        obj->initialise = InitialiseSkeleton;
        obj->control = SkeletonControl;
        obj->collision = CreatureCollision;
        obj->hitPoints = initHealth(SKELETON);
        obj->shadowSize = 128;
        obj->pivotLength = 50;
        obj->radius = 128;
        obj->mipDistance = 5120;
        obj->intelligent = true;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->saveHitpoints = true;
        obj->savePosition = true;
        obj->undead = true;
        obj->hitEffect = HIT_SMOKE;
        obj->explodableMeshBits = 2560;
    }
}

void SetupEntity::von_croy()
{
    obj = &Objects[VON_CROY];
    if (obj->loaded)
    {
        obj->initialise = InitialiseVonCroy;
        if (LevelNow == 1) // Level1: Anghor Wat
            obj->control = VonCroyControlTutorial;
        else
            obj->control = VonCroyControlNormal;
        obj->collision = CreatureCollision;
        obj->pivotLength = 0;
        obj->hitPoints = initHealth(VON_CROY);
        obj->shadowSize = 128;
        obj->radius = 128;
        obj->intelligent = true;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->saveHitpoints = true;
        obj->savePosition = true;
        obj->mipDistance = 5120;
        obj->explodableMeshBits = 0x200000;
        obj->hitEffect = HIT_BLOOD;
        
        bones[obj->boneIndex + 24] |= ROT_Y | ROT_X;
        bones[obj->boneIndex + 80] |= ROT_Y | ROT_X;
        meshes[obj->meshIndex + 15] = meshes[Objects[MESHSWAP1].meshIndex + 14];
        meshes[obj->meshIndex + 31] = meshes[Objects[MESHSWAP1].meshIndex + 30];
        meshes[obj->meshIndex + 37] = meshes[Objects[MESHSWAP1].meshIndex + 36];
    }

    obj = &Objects[VON_CROY_MIP];
    if (obj->loaded)
    {
        meshes[obj->meshIndex + 15] = meshes[Objects[MESHSWAP1].meshIndex + 14];
        meshes[obj->meshIndex + 31] = meshes[Objects[MESHSWAP1].meshIndex + 30];
        meshes[obj->meshIndex + 37] = meshes[Objects[MESHSWAP1].meshIndex + 36];
    }
}

void SetupEntity::guide()
{
    obj = &Objects[GUIDE];
    if (obj->loaded)
    {
        obj->initialise = InitialiseGuide;
        obj->control = GuideControl;
        obj->collision = CreatureCollision;
        obj->shadowSize = 128;
        obj->hitPoints = initHealth(GUIDE);
        obj->pivotLength = 0;
        obj->radius = 128;
        obj->mipDistance = 5120;
        obj->intelligent = true;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->saveHitpoints = true;
        obj->saveMesh = true;
        obj->savePosition = true;
        obj->hitEffect = HIT_SMOKE;
        
        bones[obj->boneIndex + 24] |= ROT_Y | ROT_X;
        bones[obj->boneIndex + 80] |= ROT_Y | ROT_X;
        meshes[obj->meshIndex + 31] = meshes[Objects[MESHSWAP2].meshIndex + 30];
        meshes[obj->meshIndex + 37] = meshes[Objects[MESHSWAP2].meshIndex + 36];
        meshes[obj->meshIndex + 43] = meshes[Objects[MESHSWAP2].meshIndex + 42];
    }

    obj = &Objects[GUIDE_MIP];
    if (obj->loaded)
    {
        meshes[obj->meshIndex + 31] = meshes[Objects[MESHSWAP2].meshIndex + 30];
        meshes[obj->meshIndex + 37] = meshes[Objects[MESHSWAP2].meshIndex + 36];
        meshes[obj->meshIndex + 43] = meshes[Objects[MESHSWAP2].meshIndex + 42];
    }
}

void SetupEntity::baddy_1()
{
    obj = &Objects[BADDY_1];
    if (obj->loaded)
    {
        obj->bitOffset = 1;
        obj->initialise = InitialiseBaddy;
        obj->control = BaddyControl;
        //obj->draw_routine = DrawBaddy; // custom render to switch between uzi & sword.
        obj->collision = CreatureCollision;
        obj->shadowSize = 128;
        obj->hitPoints = initHealth(BADDY_1);
        obj->pivotLength = 50;
        obj->radius = 102;
        obj->mipDistance = 5120;
        obj->intelligent = true;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->saveHitpoints = true;
        obj->savePosition = true;
        obj->usingDrawAnimatingItem = false;
        obj->hitEffect = HIT_BLOOD;
        
        bones[obj->boneIndex + 7 * 4] |= ROT_Y | ROT_X; // TORSO
        bones[obj->boneIndex + 22 * 4] |= ROT_Y | ROT_X; // HEAD

        if (Objects[BADDY_2].loaded)
            obj->animIndex = Objects[BADDY_2].animIndex;
    }
}

void SetupEntity::baddy_2()
{
    obj = &Objects[BADDY_2];
    if (obj->loaded)
    {
        obj->bitOffset = 1;
        obj->initialise = InitialiseBaddy;
        obj->control = BaddyControl;
        obj->collision = CreatureCollision;
        obj->shadowSize = 128;
        obj->hitPoints = initHealth(BADDY_2);
        obj->pivotLength = 50;
        obj->radius = 102;
        obj->mipDistance = 5120;
        obj->intelligent = true;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->saveHitpoints = true;
        obj->savePosition = true;
        obj->hitEffect = HIT_BLOOD;
        
        bones[obj->boneIndex + 7 * 4] |= ROT_Y | ROT_X; // TORSO
        bones[obj->boneIndex + 22 * 4] |= ROT_Y | ROT_X; // HEAD
    }

    obj = &Objects[BADDY_2_MIP];
    if (obj->loaded)
    {
        meshes[obj->meshIndex + 4 * 2] = meshes[Objects[MESHSWAP2].meshIndex + 4 * 2]; // enable swap ?
        meshes[obj->meshIndex + 7 * 2] = meshes[Objects[MESHSWAP2].meshIndex + 7 * 2]; //
    }
}

void SetupEntity::scorpion()
{
    obj = &Objects[SCORPION];
    if (obj->loaded)
    {
        obj->initialise = InitialiseScorpion;
        obj->control = ScorpionControl;
        obj->collision = CreatureCollision;
        obj->shadowSize = 128;
        obj->hitPoints = initHealth(SCORPION);
        obj->pivotLength = 50;
        obj->radius = 512;
        obj->mipDistance = 5120;
        obj->intelligent = true;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->saveHitpoints = true;
        obj->savePosition = true;
        obj->hitEffect = HIT_BLOOD;
    }
}

void SetupEntity::small_scorpion()
{
    obj = &Objects[SMALL_SCORPION];
    if (obj->loaded)
    {
        obj->initialise = InitialiseSmallScorpion;
        obj->control = SmallScorpionControl;
        obj->collision = CreatureCollision;
        obj->shadowSize = 128;
        obj->hitPoints = initHealth(SMALL_SCORPION);
        obj->pivotLength = 20;
        obj->radius = 128;
        obj->intelligent = true;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->saveHitpoints = true;
        obj->savePosition = true;
        obj->hitEffect = HIT_BLOOD;
    }
}

void SetupEntity::mummy()
{
    obj = &Objects[MUMMY];
    if (obj->loaded)
    {
        obj->initialise = InitialiseMummy;
        obj->control = MummyControl;
        obj->collision = CreatureCollision;
        obj->shadowSize = 128;
        obj->hitPoints = initHealth(MUMMY);
        obj->radius = 170;
        obj->mipDistance = 5120;
        obj->intelligent = true;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->saveHitpoints = true;
        obj->savePosition = true;
        obj->hitEffect = HIT_SMOKE;
        obj->undead = true;
        
        bones[obj->boneIndex + 28] |= ROT_Y | ROT_X;
        bones[obj->boneIndex + 72] |= ROT_Y;
    }
}

void SetupEntity::knight_templar()
{
    obj = &Objects[KNIGHTS_TEMPLAR];
    if (obj->loaded)
    {
        obj->initialise = InitialiseKnightTemplar;
        obj->control = KnightTemplarControl;
        obj->collision = CreatureCollision;
        obj->shadowSize = 128;
        obj->hitPoints = initHealth(KNIGHTS_TEMPLAR);
        obj->pivotLength = 0;
        obj->radius = 128;
        obj->mipDistance = 5120;
        obj->intelligent = true;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->saveHitpoints = true;
        obj->savePosition = true;
        obj->undead = true;
        
        bones[obj->boneIndex + 24] |= ROT_Y | ROT_X;
        bones[obj->boneIndex + 56] |= ROT_Y;
    }
}

void SetupEntity::sphinx()
{
    obj = &Objects[SPHINX];
    if (obj->loaded)
    {
        obj->initialise = InitialiseSphinx;
        obj->control = SphinxControl;
        obj->collision = CreatureCollision;
        obj->shadowSize = 128;
        obj->hitPoints = initHealth(SPHINX);
        obj->pivotLength = 500;
        obj->radius = 512;
        obj->mipDistance = 5120;
        obj->hitEffect = HIT_FRAGMENT;
        obj->intelligent = true;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->saveHitpoints = true;
        obj->savePosition = true;
        obj->undead = true;
    }
}

void SetupEntity::setha()
{
    obj = &Objects[SETHA];
    if (obj->loaded)
    {
        obj->initialise = InitialiseSetha;
        obj->control = SethaControl;
        obj->collision = CreatureCollision;
        obj->shadowSize = 128;
        obj->hitPoints = initHealth(SETHA);
        obj->pivotLength = 50;
        obj->radius = 341;
        obj->intelligent = true;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->saveHitpoints = true;
        obj->saveMesh = true;
        obj->savePosition = true;
        obj->mipDistance = 5120;
    }
}

void SetupEntity::lara_double()
{
    obj = &Objects[LARA_DOUBLE];
    if (obj->loaded)
    {
        obj->initialise = InitialiseCreature;
        obj->control = LaraDoubleControl;
        obj->collision = CreatureCollision;
        obj->shadowSize = 128;
        obj->hitPoints = initHealth(LARA_DOUBLE);
        obj->pivotLength = 50;
        obj->radius = 128;
        obj->intelligent = true;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->saveHitpoints = true;
        obj->savePosition = true;
        obj->undead = true;
        obj->hitEffect = HIT_FRAGMENT;
    }
}

void SetupEntity::horseman()
{
    obj = &Objects[HORSEMAN];
    if (obj->loaded)
    {
        obj->initialise = InitialiseHorseMan;
        obj->control = HorseManControl;
        obj->collision = CreatureCollision;
        obj->shadowSize = 128;
        obj->hitPoints = initHealth(HORSEMAN);
        obj->pivotLength = 50;
        obj->radius = 409;
        obj->mipDistance = 5120;
        obj->intelligent = true;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->saveHitpoints = true;
        obj->saveMesh = true;
        obj->savePosition = true;
        obj->undead = true;
        obj->hitEffect = HIT_FRAGMENT;
    }
}

void SetupEntity::hammerhead()
{
    obj = &Objects[HAMMERHEAD];
    if (obj->loaded)
    {
        obj->initialise = InitialiseHammerhead;
        obj->control = HammerheadControl;
        obj->collision = CreatureCollision;
        obj->shadowSize = 128;
        obj->hitPoints = initHealth(HAMMERHEAD);
        obj->pivotLength = 300;
        obj->radius = 341;
        obj->mipDistance = 5120;
        obj->intelligent = true;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->saveHitpoints = true;
        obj->savePosition = true;
        obj->waterCreature = true;
        obj->hitEffect = HIT_BLOOD;
        
        bones[obj->boneIndex + 0] |= ROT_Y;
        bones[obj->boneIndex + 4] |= ROT_Y;
        bones[obj->boneIndex + 8] |= ROT_Y;
        bones[obj->boneIndex + 36] |= ROT_Y;
    }
}

void SetupEntity::crocodile()
{
    obj = &Objects[CROCODILE];
    if (obj->loaded)
    {
        obj->initialise = InitialiseCrocodile;
        obj->control = CrocodileControl;
        obj->collision = CreatureCollision;
        obj->shadowSize = 128;
        obj->hitPoints = initHealth(CROCODILE);
        obj->pivotLength = 300;
        obj->radius = 402;
        obj->mipDistance = 5120;
        obj->intelligent = true;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->saveHitpoints = true;
        obj->savePosition = true;
        obj->waterCreature = true;
        obj->hitEffect = HIT_BLOOD;
        
        bones[obj->boneIndex + 0] |= ROT_Y;
        bones[obj->boneIndex + 28] |= ROT_Y;
        bones[obj->boneIndex + 36] |= ROT_Y;
        bones[obj->boneIndex + 40] |= ROT_Y;
    }
}

void SetupEntity::demigod1()
{
    obj = &Objects[DEMIGOD1];
    if (obj->loaded)
    {
        obj->initialise = InitialiseDemigod;
        obj->control = DemigodControl;
        obj->collision = CreatureCollision;
        obj->shadowSize = 128;
        obj->hitPoints = initHealth(DEMIGOD1);
        obj->pivotLength = 0;
        obj->radius = 341;
        obj->mipDistance = 5120;
        obj->intelligent = true;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->saveHitpoints = true;
        obj->savePosition = true;
        //obj->non_lot = true;
        obj->hitEffect = HIT_BLOOD;
        
        bones[obj->boneIndex + 32] |= ROT_ALL;
        bones[obj->boneIndex + 80] |= ROT_Y;
    }
}

void SetupEntity::demigod2()
{
    obj = &Objects[DEMIGOD2];
    if (obj->loaded)
    {
        obj->initialise = InitialiseDemigod;
        obj->control = DemigodControl;
        obj->collision = CreatureCollision;
        obj->shadowSize = 128;
        obj->hitPoints = initHealth(DEMIGOD2);
        obj->pivotLength = 50;
        obj->radius = 341;
        obj->mipDistance = 5120;
        obj->intelligent = true;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->saveHitpoints = true;
        obj->savePosition = true;
        //obj->non_lot = true;
        obj->hitEffect = HIT_BLOOD;
        
        bones[obj->boneIndex + 32] |= ROT_ALL;
        bones[obj->boneIndex + 80] |= ROT_Y;
    }
}

void SetupEntity::demigod3()
{
    obj = &Objects[DEMIGOD3];
    if (obj->loaded)
    {
        obj->initialise = InitialiseDemigod;
        obj->control = DemigodControl;
        obj->collision = CreatureCollision;
        obj->shadowSize = 128;
        obj->hitPoints = initHealth(DEMIGOD3);
        obj->pivotLength = 50;
        obj->radius = 341;
        obj->mipDistance = 5120;
        obj->intelligent = true;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->saveHitpoints = true;
        obj->savePosition = true;
        //obj->non_lot = true;
        obj->hitEffect = HIT_BLOOD;
        
        bones[obj->boneIndex + 32] |= ROT_ALL;
        bones[obj->boneIndex + 80] |= ROT_Y;
    }
}

void SetupEntity::mutant()
{
    obj = &Objects[MUTANT];
    if (obj->loaded)
    {
        obj->initialise = InitialiseMutant;
        obj->control = MutantControl;
        obj->collision = CreatureCollision;
        obj->shadowSize = 128;
        obj->hitPoints = initHealth(MUTANT);
        obj->pivotLength = 50;
        obj->radius = 128;
        obj->intelligent = true;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->saveMesh = true;
        obj->savePosition = true;
        obj->undead = true;
        obj->hitEffect = HIT_SMOKE;
        
        bones[obj->boneIndex + 24] |= ROT_Y | ROT_X;
        bones[obj->boneIndex + 28] |= ROT_Y | ROT_X;
    }
}

void SetupEntity::troops()
{
    obj = &Objects[TROOPS];
    if (obj->loaded)
    {
        obj->bitOffset = 0;
        obj->initialise = InitialiseTroops;
        obj->control = TroopsControl;
        obj->collision = CreatureCollision;
        obj->shadowSize = 128;
        obj->hitPoints = initHealth(TROOPS);
        obj->pivotLength = 50;
        obj->radius = 102;
        obj->mipDistance = 5120;
        obj->intelligent = true;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->saveHitpoints = true;
        obj->savePosition = true;
        obj->hitEffect = HIT_BLOOD;
        
        bones[obj->boneIndex + 0] |= ROT_Y | ROT_X;
        bones[obj->boneIndex + 28] |= ROT_Y | ROT_X;
    }
}

void SetupEntity::sas()
{
    obj = &Objects[SAS];
    if (obj->loaded)
    {
        obj->bitOffset = 0;
        obj->initialise = InitialiseSAS;
        obj->control = SASControl;
        obj->collision = CreatureCollision;
        obj->shadowSize = 128;
        obj->hitPoints = initHealth(SAS);
        obj->pivotLength = 50;
        obj->radius = 102;
        obj->mipDistance = 5120;
        obj->intelligent = true;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->saveHitpoints = true;
        obj->savePosition = true;
        obj->hitEffect = HIT_BLOOD;
        
        bones[obj->boneIndex + 0] |= ROT_Y | ROT_X;
        bones[obj->boneIndex + 28] |= ROT_Y | ROT_X;
    }
}

void SetupEntity::harpy()
{
    obj = &Objects[HARPY];
    if (obj->loaded)
    {
        obj->initialise = InitialiseHarpy;
        obj->control = HarpyControl;
        obj->collision = CreatureCollision;
        obj->shadowSize = 128;
        obj->hitPoints = initHealth(HARPY);
        obj->pivotLength = 50;
        obj->radius = 409;
        obj->mipDistance = 5120;
        obj->intelligent = true;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->saveHitpoints = true;
        obj->savePosition = true;
        obj->hitEffect = HIT_BLOOD;
    }
}

void SetupEntity::wild_boar()
{
    obj = &Objects[WILD_BOAR];
    if (obj->loaded)
    {
        obj->initialise = InitialiseWildBoar;
        obj->control = WildBoarControl;
        obj->collision = CreatureCollision;
        obj->shadowSize = 128;
        obj->hitPoints = initHealth(WILD_BOAR);
        obj->pivotLength = 50;
        obj->radius = 102;
        obj->intelligent = true;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->saveHitpoints = true;
        obj->savePosition = true;
        obj->hitEffect = HIT_BLOOD;
        
        bones[obj->boneIndex + 48] |= ROT_Z | ROT_Y;
        bones[obj->boneIndex + 52] |= ROT_Z | ROT_Y;
    }
}

void SetupEntity::dog()
{
    obj = &Objects[DOG];
    if (obj->loaded)
    {
        obj->initialise = InitialiseDog;
        obj->control = DogControl;
        obj->collision = CreatureCollision;
        obj->shadowSize = 128;
        obj->hitPoints = initHealth(DOG);
        obj->pivotLength = 300;
        obj->radius = 341;
        obj->mipDistance = 5120;
        obj->intelligent = true;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->saveHitpoints = true;
        obj->savePosition = true;
        obj->hitEffect = HIT_BLOOD;
        
        bones[obj->boneIndex + 0] |= ROT_Y;
        bones[obj->boneIndex + 8] |= ROT_Y | ROT_X;
    }
}

void SetupEntity::ahmet()
{
    obj = &Objects[AHMET];
    if (obj->loaded)
    {
        obj->initialise = InitialiseAhmet;
        obj->control = AhmetControl;
        obj->collision = CreatureCollision;
        obj->shadowSize = 128;
        obj->hitPoints = initHealth(AHMET);
        obj->pivotLength = 300;
        obj->radius = 341;
        obj->mipDistance = 5120;
        obj->intelligent = true;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->saveHitpoints = true;
        obj->savePosition = true;
        obj->hitEffect = HIT_BLOOD;
        
        bones[obj->boneIndex + 36] |= ROT_Y;
    }
}

void SetupEntity::baboon_normal()
{
    obj = &Objects[BABOON_NORMAL];
    if (obj->loaded)
    {
        obj->initialise = InitialiseBaboon;
        obj->control = BaboonControl;
        obj->collision = CreatureCollision;
        obj->shadowSize = 128;
        obj->hitPoints = initHealth(BABOON_NORMAL);
        obj->pivotLength = 200;
        obj->radius = 256;
        obj->intelligent = true;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->saveHitpoints = true;
        obj->savePosition = true;
        obj->hitEffect = HIT_BLOOD;
    }
}

void SetupEntity::baboon_silent()
{
    obj = &Objects[BABOON_SILENT];
    if (obj->loaded)
    {
        obj->initialise = InitialiseBaboon;
        obj->control = BaboonControl;
        obj->collision = CreatureCollision;
        obj->shadowSize = 128;
        obj->hitPoints = initHealth(BABOON_SILENT);
        obj->pivotLength = 200;
        obj->radius = 256;
        obj->intelligent = true;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->saveHitpoints = true;
        obj->savePosition = true;
        obj->hitEffect = HIT_BLOOD;

        if (Objects[BABOON_NORMAL].loaded)
            Objects[BABOON_SILENT].animIndex = Objects[BABOON_NORMAL].animIndex;
    }
}

void SetupEntity::baboon_inv()
{
    obj = &Objects[BABOON_INV];
    if (obj->loaded)
    {
        obj->initialise = InitialiseBaboon;
        obj->control = BaboonControl;
        obj->collision = CreatureCollision;
        obj->shadowSize = 128;
        obj->hitPoints = initHealth(BABOON_INV);
        obj->pivotLength = 200;
        obj->radius = 256;
        obj->intelligent = true;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->saveHitpoints = true;
        obj->savePosition = true;
        obj->hitEffect = HIT_BLOOD;

        if (Objects[BABOON_NORMAL].loaded)
            Objects[BABOON_SILENT].animIndex = Objects[BABOON_NORMAL].animIndex;
    }
}

// can disappear like nothing !!
void SetupEntity::enemy_jeep()
{
    obj = &Objects[ENEMY_JEEP];
    if (obj->loaded)
    {
        // the train level consider it as not a entity !
        if (CHK_ANY(gfLevelFlags, SLEV_TRAIN))
        {
            obj->initialise = InitialiseEnemyJeepTrain;
            obj->control = EnemyJeepTrainControl;
            obj->collision = CreatureCollision;
            obj->shadowSize = 128; // not exist in TR4 Train level by default !!
            obj->saveAnim = true;
            obj->saveFlags = true;
            obj->saveHitpoints = true;
            obj->savePosition = true;
            obj->hitEffect = HIT_FRAGMENT;
        }
        else
        {
            obj->initialise = InitialiseEnemyJeep;
            obj->control = EnemyJeepControl;
            obj->collision = CreatureCollision;
            obj->shadowSize = 128;
            obj->hitPoints = initHealth(ENEMY_JEEP);
            obj->pivotLength = 500;
            obj->radius = 512;
            obj->intelligent = true;
            obj->saveAnim = true;
            obj->saveFlags = true;
            obj->saveHitpoints = true;
            obj->saveMesh = true;
            obj->savePosition = true;
            obj->hitEffect = HIT_FRAGMENT;
            
            bones[obj->boneIndex + 32] |= ROT_X;
            bones[obj->boneIndex + 36] |= ROT_X;
            bones[obj->boneIndex + 44] |= ROT_X;
            bones[obj->boneIndex + 48] |= ROT_X;
        }
    }
}

void SetupEntity::bat()
{
    obj = &Objects[BAT];
    if (obj->loaded)
    {
        obj->initialise = InitialiseBat;
        obj->control = BatControl;
        obj->collision = CreatureCollision;
        obj->shadowSize = 128;
        obj->hitPoints = initHealth(BAT);
        obj->pivotLength = 10;
        obj->radius = 102;
        obj->intelligent = true;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->saveHitpoints = true;
        obj->savePosition = true;
        obj->hitEffect = HIT_BLOOD;
    }
}

void SetupEntity::big_beetle()
{
    obj = &Objects[BIG_BEETLE];
    if (obj->loaded)
    {
        obj->initialise = InitialiseBigBeetle;
        obj->control = BigBeetleControl;
        obj->collision = CreatureCollision;
        obj->shadowSize = 128;
        obj->hitPoints = initHealth(BIG_BEETLE);
        obj->pivotLength = 50;
        obj->radius = 204;
        obj->mipDistance = 5120;
        obj->intelligent = true;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->saveHitpoints = true;
        obj->savePosition = true;
        obj->hitEffect = HIT_BLOOD;
    }
}

void SetupEntity::sentry_gun()
{
    obj = &Objects[SENTRY_GUN];
    if (obj->loaded)
    {
        obj->initialise = InitialiseSentryGun;
        obj->control = SentryGunControl;
        obj->collision = CreatureCollision;
        obj->shadowSize = 128;
        obj->hitPoints = initHealth(SENTRY_GUN);
        obj->pivotLength = 50;
        obj->radius = 204;
        obj->explodableMeshBits = 64;
        obj->intelligent = true;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->saveHitpoints = true;
        obj->saveMesh = true;
        obj->savePosition = true;
        obj->undead = true; // only killable by grenadegun or crossbow
        obj->hitEffect = HIT_FRAGMENT;
        
        bones[obj->boneIndex + 0] |= ROT_Y;
        bones[obj->boneIndex + 4] |= ROT_X;
        bones[obj->boneIndex + 8] |= ROT_Z;
        bones[obj->boneIndex + 12] |= ROT_Z;
    }
}

void SetupEntity::horse()
{
    obj = &Objects[HORSE];
    if (obj->loaded)
    {
        obj->initialise = InitialiseHorse;
        obj->collision = ObjectCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }
}

void SetupEntity::sas_dying()
{
    obj = &Objects[SAS_DYING];
    if (obj->loaded)
    {
        obj->initialise = InitialiseSasDying;
        obj->control = SasDyingControl;
        obj->collision = ObjectCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->savePosition = true;
    }
}

void SetupEntity::sas_captain()
{
    obj = &Objects[SAS_CAPTAIN];
    if (obj->loaded)
    {
        obj->initialise = InitialiseSasDying;
        obj->control = SasDyingControl;
        obj->collision = ObjectCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->savePosition = true;
    }
}

void SetupEntity::jean_yves()
{
    obj = &Objects[JEAN_YVES];
    if (obj->loaded)
    {
        obj->initialise = InitialiseJeanYves;
        obj->control = JeanYvesControl;
        obj->collision = ObjectCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->savePosition = true;
        obj->mipDistance = 10240;
    }
}

void SetupEntity::game_piece1()
{
    obj = &Objects[GAME_PIECE1];
    if (obj->loaded)
    {
        obj->initialise = InitialiseGamePiece;
        obj->control = GamePieceControl;
        obj->collision = ObjectCollision;
        obj->saveHitpoints = true;
        obj->saveFlags = true;
        obj->savePosition = true;
    }
}

void SetupEntity::game_piece2()
{
    obj = &Objects[GAME_PIECE2];
    if (obj->loaded)
    {
        obj->initialise = InitialiseGamePiece;
        obj->control = GamePieceControl;
        obj->collision = ObjectCollision;
        obj->saveHitpoints = true;
        obj->saveFlags = true;
        obj->savePosition = true;
    }
}

void SetupEntity::game_piece3()
{
    obj = &Objects[GAME_PIECE3];
    if (obj->loaded)
    {
        obj->initialise = InitialiseGamePiece;
        obj->control = GamePieceControl;
        obj->collision = ObjectCollision;
        obj->saveHitpoints = true;
        obj->saveFlags = true;
        obj->savePosition = true;
    }
}

void SetupEntity::enemy_piece()
{
    obj = &Objects[ENEMY_PIECE];
    if (obj->loaded)
    {
        obj->collision = ObjectCollision;
        obj->saveFlags = true;
        obj->savePosition = true;
    }
}

void SetupEntity::wheel_of_fortune()
{
    obj = &Objects[WHEEL_OF_FORTUNE];
    if (obj->loaded)
    {
        obj->initialise = InitialiseWheelOfFortune;
        obj->control = WheelOfFortuneControl;
        obj->collision = WheelOfFortuneCollision;
        obj->hitPoints = 1;
        obj->saveAnim = true;
        obj->saveFlags = true;
        
        bones[obj->boneIndex + 0] |= ROT_Z;
        bones[obj->boneIndex + 4] |= ROT_Z;
        bones[obj->boneIndex + 8] |= ROT_Z;
        bones[obj->boneIndex + 12] |= ROT_Z;
    }
}

void SetupEntity::wraith1()
{
    obj = &Objects[WRAITH1];
    if (obj->loaded)
    {
        obj->initialise = InitialiseWraith;
        obj->control = WraithControl;
        obj->drawRoutine = DrawWraith;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->saveHitpoints = true;
        obj->savePosition = true;
    }
}

void SetupEntity::wraith2()
{
    obj = &Objects[WRAITH2];
    if (obj->loaded)
    {
        obj->initialise = InitialiseWraith;
        obj->control = WraithControl;
        obj->drawRoutine = DrawWraith;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->saveHitpoints = true;
        obj->savePosition = true;
    }
}

void SetupEntity::wraith3()
{
    obj = &Objects[WRAITH3];
    if (obj->loaded)
    {
        obj->initialise = InitialiseWraith;
        obj->control = WraithControl;
        obj->drawRoutine = DrawWraith;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->saveHitpoints = true;
        obj->savePosition = true;
    }
}

void SetupEntity::wraith4()
{
    obj = &Objects[WRAITH4];
    if (obj->loaded)
    {
        obj->initialise = InitialiseWraith;
        obj->control = WraithControl;
        obj->drawRoutine = DrawWraith;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->saveHitpoints = true;
        obj->savePosition = true;
    }
}

void SetupEntity::little_beetle()
{
    obj = &Objects[LITTLE_BEETLE];
    if (obj->loaded)
    {
        obj->initialise = InitialiseLittleBeetle;
        obj->control = LittleBeetleControl;
        obj->drawRoutine = NULL;
    }
}

void SetupEntity::fish()
{
    obj = &Objects[FISH];
    if (obj->loaded)
    {
        obj->initialise = InitialiseFish;
        obj->control = FishControl;
        obj->drawRoutine = NULL;
        obj->saveFlags = true;
    }
}
