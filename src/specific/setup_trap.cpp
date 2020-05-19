#include "framework.h"
#include "setup_trap.h"
#include "collide.h"
#include "traps.h"
#include "puzzle.h"
#include "rope.h"
#include "oldobjects.h"

SetupTrap setup_trap;
void SetupTrap::floor_4blade()
{
    obj = &Objects[FLOOR_4BLADE];
    if (obj->loaded)
    {
        obj->control = FourBladeControl;
        obj->collision = TrapCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }
}

void SetupTrap::roof_4blade()
{
    obj = &Objects[ROOF_4BLADE];
    if (obj->loaded)
    {
        obj->control = FourBladeControl;
        obj->collision = TrapCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }
}

void SetupTrap::bird_blade()
{
    obj = &Objects[BIRD_BLADE];
    if (obj->loaded)
    {
        obj->control = BirdBladeControl;
        obj->collision = TrapCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }
}

void SetupTrap::seth_blade()
{
    obj = &Objects[SETH_BLADE];
    if (obj->loaded)
    {
        obj->initialise = InitialiseSethBlade;
        obj->control = SethBladeControl;
        obj->collision = TrapCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }
}

void SetupTrap::chain()
{
    obj = &Objects[CHAIN];
    if (obj->loaded)
    {
        obj->control = ChainControl;
        obj->collision = TrapCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }
}

void SetupTrap::plough()
{
    obj = &Objects[PLOUGH];
    if (obj->loaded)
    {
        obj->control = PloughControl;
        obj->collision = TrapCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }
}

void SetupTrap::spikeball()
{
    obj = &Objects[SPIKEBALL];
    if (obj->loaded)
    {
        obj->control = SpikeBallControl;
        obj->collision = TrapCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }
}

void SetupTrap::hammer()
{
    obj = &Objects[HAMMER];
    if (obj->loaded)
    {
        obj->control = HammerControl;
        obj->collision = TrapCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }
}

void SetupTrap::catwalk_blade()
{
    obj = &Objects[CATWALK_BLADE];
    if (obj->loaded)
    {
        obj->control = CatwalkBladeControl;
        obj->collision = MovBlockCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }
}

void SetupTrap::moving_blade()
{
    obj = &Objects[MOVING_BLADE];
    if (obj->loaded)
    {
        obj->control = MovingBladeControl;
        obj->collision = MovBlockCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }
}

void SetupTrap::plinth_blade()
{
    obj = &Objects[PLINTH_BLADE];
    if (obj->loaded)
    {
        obj->control = PlinthBladeControl;
        obj->collision = MovBlockCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }
}

void SetupTrap::cog()
{
    obj = &Objects[COG];
    if (obj->loaded)
    {
        obj->control = PuzzleControl;
        obj->collision = CogCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }
}

void SetupTrap::stargate()
{
    obj = &Objects[STARGATE];
    if (obj->loaded)
    {
        obj->control = StargateControl;
        obj->collision = StargateCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
    }
}

void SetupTrap::rollingball()
{
    obj = &Objects[ROLLINGBALL];
    if (obj->loaded)
    {
        obj->control = RollingBallControl;
        obj->collision = RollingBallCollision;
        obj->saveFlags = true;
        obj->savePosition = true;
    }
}

void SetupTrap::burning_floor()
{
    obj = &Objects[BURNING_FLOOR];
    if (obj->loaded)
    {
        obj->initialise = InitialiseBurningFloor;
        obj->control = BurningFloorControl;
        obj->collision = BurningFloorCollision;
        obj->saveFlags = true;
        obj->savePosition = true;
    }
}

void SetupTrap::twoblock_platform()
{
    obj = &Objects[TWOBLOCK_PLATFORM];
    if (obj->loaded)
    {
        obj->initialise = InitialiseTwoBlockPlatform;
        obj->control = TwoBlockPlatformControl;
        obj->floor = TwoBlockPlatformFloor;
        obj->ceiling = TwoBlockPlatformCeiling;
        obj->saveFlags = true;
        obj->savePosition = true;
    }
}

void SetupTrap::kill_all_triggers()
{
    obj = &Objects[KILL_ALL_TRIGGERS];
    if (obj->loaded)
    {
        obj->control = KillAllTriggerControl;
        obj->drawRoutine = NULL;
        obj->hitPoints = 0;
        obj->saveFlags = true;
    }
}

void SetupTrap::death_slide()
{
    obj = &Objects[DEATH_SLIDE];
    if (obj->loaded)
    {
        obj->initialise = InitialiseDeathSlide;
        obj->control = DeathSlideControl;
        obj->collision = DeathSlideCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->savePosition = true;
    }
}

void SetupTrap::falling_block()
{
    obj = &Objects[FALLING_BLOCK];
    if (obj->loaded)
    {
        obj->initialise = InitialiseShatter;
        obj->control = FallingBlockControl;
        obj->collision = FallingBlockCollision;
        obj->floor = FallingBlockFloor;
        obj->ceiling = FallingBlockCeiling;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->savePosition = true;
    }

    obj = &Objects[FALLING_BLOCK2];
    if (obj->loaded)
    {
        obj->initialise = InitialiseShatter;
        obj->control = FallingBlockControl;
        obj->collision = FallingBlockCollision;
        obj->floor = FallingBlockFloor;
        obj->ceiling = FallingBlockCeiling;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->savePosition = true;
    }
}

void SetupTrap::falling_ceiling()
{
    obj = &Objects[FALLING_CEILING];
    if (obj->loaded)
    {
        obj->control = FallingCeilingControl;
        obj->collision = FallingCeilingCollision;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->savePosition = true;
    }
}

void SetupTrap::smashable_bike_floor()
{
    obj = &Objects[SMASHABLE_BIKE_FLOOR];
    if (obj->loaded)
    {
        obj->initialise = InitialiseShatter;
        obj->control = SmashableBikeFloorControl;
        obj->collision = ObjectCollision;
        obj->floor = TwoBlockPlatformFloor;
        obj->ceiling = TwoBlockPlatformCeiling;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->savePosition = true;
    }
}

void SetupTrap::smashable_bike_wall()
{
    obj = &Objects[SMASHABLE_BIKE_WALL];
    if (obj->loaded)
    {
        obj->initialise = InitialiseShatter;
        obj->control = SmashableBikeWallControl;
        obj->collision = ObjectCollision;
        obj->saveFlags = true;
    }
}

void SetupTrap::pushable_object()
{
    obj = &Objects[PUSHABLE_OBJECT1];
    if (obj->loaded)
    {
        obj->initialise = InitialisePushableBlock;
        obj->control = PushableBlockControl;
        obj->collision = PushableBlockCollision;
        obj->saveFlags = true;
        obj->savePosition = true;
    }

    obj = &Objects[PUSHABLE_OBJECT2];
    if (obj->loaded)
    {
        obj->initialise = InitialisePushableBlock;
        obj->control = PushableBlockControl;
        obj->collision = PushableBlockCollision;
        obj->saveFlags = true;
        obj->savePosition = true;
    }

    obj = &Objects[PUSHABLE_OBJECT3];
    if (obj->loaded)
    {
        obj->initialise = InitialisePushableBlock;
        obj->control = PushableBlockControl;
        obj->collision = PushableBlockCollision;
        obj->saveFlags = true;
        obj->savePosition = true;
    }

    obj = &Objects[PUSHABLE_OBJECT4];
    if (obj->loaded)
    {
        obj->initialise = InitialisePushableBlock;
        obj->control = PushableBlockControl;
        obj->collision = PushableBlockCollision;
        obj->saveFlags = true;
        obj->savePosition = true;
    }

    obj = &Objects[PUSHABLE_OBJECT5];
    if (obj->loaded)
    {
        obj->initialise = InitialisePushableBlock;
        obj->control = PushableBlockControl;
        obj->collision = PushableBlockCollision;
        obj->saveFlags = true;
        obj->savePosition = true;
    }
}

void SetupTrap::darts()
{
    obj = &Objects[DARTS];
    if (obj->loaded)
    {
        obj->control = DartControl;
        obj->drawRoutine = DrawDart;
        obj->collision = ObjectCollision;
        obj->shadowSize = 128;
    }
}

void SetupTrap::planet_effect()
{
    obj = &Objects[PLANET_EFFECT];
    if (obj->loaded)
    {
        obj->initialise = InitialisePlanetEffect;
        obj->control = PlanetEffectControl;
        obj->drawRoutine = DrawPlanetEffect;
        obj->saveFlags = true;
        obj->unknown = 1; // ???
    }
}

void SetupTrap::flame()
{
    obj = &Objects[FLAME];
    if (obj->loaded)
    {
        obj->control = FlameControl;
        obj->drawRoutine = NULL;
        obj->usingDrawAnimatingItem = false;
    }
}

void SetupTrap::sas_drag_bloke()
{
    obj = &Objects[SAS_DRAG_BLOKE];
    if (obj->loaded)
    {
        obj->collision = SasDragBlokeCollision;
        obj->control = PuzzleControl;
        obj->saveAnim = true;
        obj->saveFlags = true;
        obj->savePosition = true;
    }
}

void SetupTrap::dart_emitter()
{
    obj = &Objects[DART_EMITTER];
    if (obj->loaded)
    {
        obj->control = DartEmitterControl;
        obj->drawRoutine = NULL;
        obj->saveFlags = true;
    }
}

void SetupTrap::homing_dart_emitter()
{
    obj = &Objects[HOMING_DART_EMITTER];
    if (obj->loaded)
    {
        obj->control = DartEmitterControl;
        obj->drawRoutine = NULL;
        obj->saveFlags = true;
    }
}

void SetupTrap::flame_emitter()
{
    obj = &Objects[FLAME_EMITTER];
    if (obj->loaded)
    {
        obj->initialise = InitialiseFlameEmitter;
        obj->control = FlameEmitterControl;
        obj->collision = FlameCollision;
        obj->drawRoutine = NULL;
        obj->saveFlags = true;
    }

    obj = &Objects[FLAME_EMITTER2];
    if (obj->loaded)
    {
        obj->initialise = InitialiseFlameEmitter2;
        obj->control = FlameEmitter2Control;
        obj->collision = FlameCollision;
        obj->drawRoutine = NULL;
        obj->saveFlags = true;
    }

    obj = &Objects[FLAME_EMITTER3];
    if (obj->loaded)
    {
        obj->initialise = InitialiseFlameEmitter3;
        obj->control = FlameEmitter3Control;
        obj->collision = FlameCollision;
        obj->drawRoutine = NULL;
        obj->saveFlags = true;
    }
}

void SetupTrap::sprinkler()
{
    obj = &Objects[SPRINKLER];
    if (obj->loaded)
    {
        obj->control = SprinklerControl;
        obj->collision = FlameCollision;
        obj->saveFlags = true;
    }
}

void SetupTrap::rope()
{
    obj = &Objects[ROPE];
    if (obj->loaded)
    {
        obj->initialise = InitialiseRope;
        obj->control = RopeControl;
        obj->collision = RopeCollision;
        obj->drawRoutine = NULL;
        obj->saveFlags = true;
    }
}

void SetupTrap::polerope()
{
    obj = &Objects[POLEROPE];
    if (obj->loaded)
    {
        obj->collision = PoleRopeCollision;
        obj->saveFlags = true;
    }
}

void SetupTrap::trigger_triggerer()
{
    obj = &Objects[TRIGGER_TRIGGERER];
    if (obj->loaded)
    {
        obj->initialise = TriggerTriggerer;
        obj->control = TriggerTriggerer;
        obj->drawRoutine = NULL;
        obj->saveFlags = true;
    }
}

void SetupTrap::mine()
{
    obj = &Objects[MINE];
    if (obj->loaded)
    {
        obj->initialise = InitialiseMine;
        obj->control = MineControl;
        obj->collision = MineCollision;
        obj->saveMesh = true;
        obj->usingDrawAnimatingItem = true;
    }
}
