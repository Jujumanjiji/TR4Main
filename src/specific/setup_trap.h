#pragma once

class SetupTrap
{
public:
    void floor_4blade();
    void roof_4blade();
    void bird_blade();
    void seth_blade();
    void chain();
    void plough();
    void spikeball();
    void hammer();
    void catwalk_blade();
    void moving_blade();
    void plinth_blade();
    void cog();
    void stargate();
    void rollingball();
    void burning_floor();
    void twoblock_platform();
    void kill_all_triggers();
    void death_slide();
    void falling_block();
    void falling_ceiling();
    void smashable_bike_floor();
    void smashable_bike_wall();
    void pushable_object();
    void darts();
    void planet_effect();
    void flame();
    void sas_drag_bloke();
    void dart_emitter();
    void homing_dart_emitter();
    void flame_emitter();
    void sprinkler();
    void rope();
    void polerope();
    void trigger_triggerer();
    void mine();

private:
    OBJECT_INFO* obj;
};

extern SetupTrap setup_trap;
