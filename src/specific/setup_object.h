#pragma once

class SetupObject
{
public:
	void camera_target();
	void flare();
	void smash_object();
	void bridge();
	void switchs();
	void sequence_switch();
	void underwater_switch();
	void lever_switch();
	void jump_switch();
	void pulley();
	void turn_switch();
	void cog_switch();
	void crowbar_switch();
	void doors();
	void double_door();
	void sequence_door();
	void underwater_door();
	void pushpull_door();
	void floor_trapdoor();
	void ceiling_trapdoor();
	void trapdoor();
	void pickups();
	void inventory_item();
	void grenade();
	void crossbow_bolt();
	void key_hole();
	void puzzle_hole();
	void puzzle_done();
	void sarcophagus();
	void animatings();
	void statue_plinth();
	void element_puzzle();
	void expanding_platform();
	void obelisk();
	void squishy_block();
	void firerope();
	void mapper();
	void slicer_dicer();
	void scales();
	void sarcophagus_cut();
	void joby_spike();
	void teeth_spike();
	void horus_statue();
	void raising_block();
	void smoke_emitter();
	void red_green_blue_light();
	void blinking_light();
	void lens_flare();
	void amber_light();
	void bubble_effect();
	void waterfallmist();
	void waterfall();
	void lightning_conductor();
	void white_light();
	void body_part();
	void earthquake();
	void clockwork_beetle();
	void god_head();

private:
	OBJECT_INFO* obj;
};

extern SetupObject setup_object;