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
    obj = &objects[FLOOR_4BLADE];
    if (obj->loaded)
    {
        obj->control = FourBladeControl;
        obj->collision = TrapCollision;
        obj->save_anim = true;
        obj->save_flags = true;
    }
}

void SetupTrap::roof_4blade()
{
    obj = &objects[ROOF_4BLADE];
    if (obj->loaded)
    {
        obj->control = FourBladeControl;
        obj->collision = TrapCollision;
        obj->save_anim = true;
        obj->save_flags = true;
    }
}

void SetupTrap::bird_blade()
{
    obj = &objects[BIRD_BLADE];
    if (obj->loaded)
    {
        obj->control = BirdBladeControl;
        obj->collision = TrapCollision;
        obj->save_anim = true;
        obj->save_flags = true;
    }
}

void SetupTrap::seth_blade()
{
    obj = &objects[SETH_BLADE];
    if (obj->loaded)
    {
        obj->initialise = InitialiseSethBlade;
        obj->control = SethBladeControl;
        obj->collision = TrapCollision;
        obj->save_anim = true;
        obj->save_flags = true;
    }
}

void SetupTrap::chain()
{
    obj = &objects[CHAIN];
    if (obj->loaded)
    {
        obj->control = ChainControl;
        obj->collision = TrapCollision;
        obj->save_anim = true;
        obj->save_flags = true;
    }
}

void SetupTrap::plough()
{
    obj = &objects[PLOUGH];
    if (obj->loaded)
    {
        obj->control = PloughControl;
        obj->collision = TrapCollision;
        obj->save_anim = true;
        obj->save_flags = true;
    }
}

void SetupTrap::spikeball()
{
    obj = &objects[SPIKEBALL];
    if (obj->loaded)
    {
        obj->control = SpikeBallControl;
        obj->collision = TrapCollision;
        obj->save_anim = true;
        obj->save_flags = true;
    }
}

void SetupTrap::hammer()
{
    obj = &objects[HAMMER];
    if (obj->loaded)
    {
        obj->control = HammerControl;
        obj->collision = TrapCollision;
        obj->save_anim = true;
        obj->save_flags = true;
    }
}

void SetupTrap::catwalk_blade()
{
    obj = &objects[CATWALK_BLADE];
    if (obj->loaded)
    {
        obj->control = CatwalkBladeControl;
        obj->collision = MovBlockCollision;
        obj->save_anim = true;
        obj->save_flags = true;
    }
}

void SetupTrap::moving_blade()
{
    obj = &objects[MOVING_BLADE];
    if (obj->loaded)
    {
        obj->control = MovingBladeControl;
        obj->collision = MovBlockCollision;
        obj->save_anim = true;
        obj->save_flags = true;
    }
}

void SetupTrap::plinth_blade()
{
    obj = &objects[PLINTH_BLADE];
    if (obj->loaded)
    {
        obj->control = PlinthBladeControl;
        obj->collision = MovBlockCollision;
        obj->save_anim = true;
        obj->save_flags = true;
    }
}

void SetupTrap::cog()
{
    obj = &objects[COG];
    if (obj->loaded)
    {
        obj->control = PuzzleControl;
        obj->collision = CogCollision;
        obj->save_anim = true;
        obj->save_flags = true;
    }
}

void SetupTrap::stargate()
{
    obj = &objects[STARGATE];
    if (obj->loaded)
    {
        obj->control = StargateControl;
        obj->collision = StargateCollision;
        obj->save_anim = true;
        obj->save_flags = true;
    }
}

void SetupTrap::rollingball()
{
    obj = &objects[ROLLINGBALL];
    if (obj->loaded)
    {
        obj->control = RollingBallControl;
        obj->collision = RollingBallCollision;
        obj->save_flags = true;
        obj->save_position = true;
    }
}

void SetupTrap::burning_floor()
{
    obj = &objects[BURNING_FLOOR];
    if (obj->loaded)
    {
        obj->initialise = InitialiseBurningFloor;
        obj->control = BurningFloorControl;
        obj->collision = BurningFloorCollision;
        obj->save_flags = true;
        obj->save_position = true;
    }
}

void SetupTrap::twoblock_platform()
{
    obj = &objects[TWOBLOCK_PLATFORM];
    if (obj->loaded)
    {
        obj->initialise = InitialiseTwoBlockPlatform;
        obj->control = TwoBlockPlatformControl;
        obj->floor = TwoBlockPlatformFloor;
        obj->ceiling = TwoBlockPlatformCeiling;
        obj->save_flags = true;
        obj->save_position = true;
    }
}

void SetupTrap::kill_all_triggers()
{
    obj = &objects[KILL_ALL_TRIGGERS];
    if (obj->loaded)
    {
        obj->control = KillAllTriggerControl;
        obj->draw_routine = NULL;
        obj->hit_points = 0;
        obj->save_flags = true;
    }
}

void SetupTrap::death_slide()
{
    obj = &objects[DEATH_SLIDE];
    if (obj->loaded)
    {
        obj->initialise = InitialiseDeathSlide;
        obj->control = DeathSlideControl;
        obj->collision = DeathSlideCollision;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_position = true;
    }
}

void SetupTrap::falling_block()
{
    obj = &objects[FALLING_BLOCK];
    if (obj->loaded)
    {
        obj->initialise = InitialiseShatter;
        obj->control = FallingBlockControl;
        obj->collision = FallingBlockCollision;
        obj->floor = FallingBlockFloor;
        obj->ceiling = FallingBlockCeiling;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_position = true;
    }

    obj = &objects[FALLING_BLOCK2];
    if (obj->loaded)
    {
        obj->initialise = InitialiseShatter;
        obj->control = FallingBlockControl;
        obj->collision = FallingBlockCollision;
        obj->floor = FallingBlockFloor;
        obj->ceiling = FallingBlockCeiling;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_position = true;
    }
}

void SetupTrap::falling_ceiling()
{
    obj = &objects[FALLING_CEILING];
    if (obj->loaded)
    {
        obj->control = FallingCeilingControl;
        obj->collision = FallingCeilingCollision;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_position = true;
    }
}

void SetupTrap::smashable_bike_floor()
{
    obj = &objects[SMASHABLE_BIKE_FLOOR];
    if (obj->loaded)
    {
        obj->initialise = InitialiseShatter;
        obj->control = SmashableBikeFloorControl;
        obj->collision = ObjectCollision;
        obj->floor = TwoBlockPlatformFloor;
        obj->ceiling = TwoBlockPlatformCeiling;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_position = true;
    }
}

void SetupTrap::smashable_bike_wall()
{
    obj = &objects[SMASHABLE_BIKE_WALL];
    if (obj->loaded)
    {
        obj->initialise = InitialiseShatter;
        obj->control = SmashableBikeWallControl;
        obj->collision = ObjectCollision;
        obj->save_flags = true;
    }
}

void SetupTrap::pushable_object()
{
    obj = &objects[PUSHABLE_OBJECT1];
    if (obj->loaded)
    {
        obj->initialise = InitialisePushableBlock;
        obj->control = PushableBlockControl;
        obj->collision = PushableBlockCollision;
        obj->save_flags = true;
        obj->save_position = true;
    }

    obj = &objects[PUSHABLE_OBJECT2];
    if (obj->loaded)
    {
        obj->initialise = InitialisePushableBlock;
        obj->control = PushableBlockControl;
        obj->collision = PushableBlockCollision;
        obj->save_flags = true;
        obj->save_position = true;
    }

    obj = &objects[PUSHABLE_OBJECT3];
    if (obj->loaded)
    {
        obj->initialise = InitialisePushableBlock;
        obj->control = PushableBlockControl;
        obj->collision = PushableBlockCollision;
        obj->save_flags = true;
        obj->save_position = true;
    }

    obj = &objects[PUSHABLE_OBJECT4];
    if (obj->loaded)
    {
        obj->initialise = InitialisePushableBlock;
        obj->control = PushableBlockControl;
        obj->collision = PushableBlockCollision;
        obj->save_flags = true;
        obj->save_position = true;
    }

    obj = &objects[PUSHABLE_OBJECT5];
    if (obj->loaded)
    {
        obj->initialise = InitialisePushableBlock;
        obj->control = PushableBlockControl;
        obj->collision = PushableBlockCollision;
        obj->save_flags = true;
        obj->save_position = true;
    }
}

void SetupTrap::darts()
{
    obj = &objects[DARTS];
    if (obj->loaded)
    {
        obj->control = DartControl;
        obj->draw_routine = DrawDart;
        obj->collision = ObjectCollision;
        obj->shadow_size = 128;
    }
}

void SetupTrap::planet_effect()
{
    obj = &objects[PLANET_EFFECT];
    if (obj->loaded)
    {
        obj->initialise = InitialisePlanetEffect;
        obj->control = PlanetEffectControl;
        obj->draw_routine = DrawPlanetEffect;
        obj->save_flags = true;
        obj->unknown = 1; // ???
    }
}

void SetupTrap::flame()
{
    obj = &objects[FLAME];
    if (obj->loaded)
    {
        obj->control = FlameControl;
        obj->draw_routine = NULL;
        obj->using_drawanimating_item = false;
    }
}

void SetupTrap::sas_drag_bloke()
{
    obj = &objects[SAS_DRAG_BLOKE];
    if (obj->loaded)
    {
        obj->collision = SasDragBlokeCollision;
        obj->control = PuzzleControl;
        obj->save_anim = true;
        obj->save_flags = true;
        obj->save_position = true;
    }
}

void SetupTrap::dart_emitter()
{
    obj = &objects[DART_EMITTER];
    if (obj->loaded)
    {
        obj->control = DartEmitterControl;
        obj->draw_routine = NULL;
        obj->save_flags = true;
    }
}

void SetupTrap::homing_dart_emitter()
{
    obj = &objects[HOMING_DART_EMITTER];
    if (obj->loaded)
    {
        obj->control = DartEmitterControl;
        obj->draw_routine = NULL;
        obj->save_flags = true;
    }
}

void SetupTrap::flame_emitter()
{
    obj = &objects[FLAME_EMITTER];
    if (obj->loaded)
    {
        obj->initialise = InitialiseFlameEmitter;
        obj->control = FlameEmitterControl;
        obj->collision = FlameCollision;
        obj->draw_routine = NULL;
        obj->save_flags = true;
    }

    obj = &objects[FLAME_EMITTER2];
    if (obj->loaded)
    {
        obj->initialise = InitialiseFlameEmitter2;
        obj->control = FlameEmitter2Control;
        obj->collision = FlameCollision;
        obj->draw_routine = NULL;
        obj->save_flags = true;
    }

    obj = &objects[FLAME_EMITTER3];
    if (obj->loaded)
    {
        obj->initialise = InitialiseFlameEmitter3;
        obj->control = FlameEmitter3Control;
        obj->collision = FlameCollision;
        obj->draw_routine = NULL;
        obj->save_flags = true;
    }
}

void SetupTrap::sprinkler()
{
    obj = &objects[SPRINKLER];
    if (obj->loaded)
    {
        obj->control = SprinklerControl;
        obj->collision = FlameCollision;
        obj->save_flags = true;
    }
}

void SetupTrap::rope()
{
    obj = &objects[ROPE];
    if (obj->loaded)
    {
        obj->initialise = InitialiseRope;
        obj->control = RopeControl;
        obj->collision = RopeCollision;
        obj->draw_routine = NULL;
        obj->save_flags = true;
    }
}

void SetupTrap::polerope()
{
    obj = &objects[POLEROPE];
    if (obj->loaded)
    {
        obj->collision = PoleRopeCollision;
        obj->save_flags = true;
    }
}

void SetupTrap::trigger_triggerer()
{
    obj = &objects[TRIGGER_TRIGGERER];
    if (obj->loaded)
    {
        obj->initialise = TriggerTriggerer;
        obj->control = TriggerTriggerer;
        obj->draw_routine = NULL;
        obj->save_flags = true;
    }
}

void SetupTrap::mine()
{
    obj = &objects[MINE];
    if (obj->loaded)
    {
        obj->initialise = InitialiseMine;
        obj->control = MineControl;
        obj->collision = MineCollision;
        obj->save_mesh = true;
        obj->using_drawanimating_item = true;
    }
}
