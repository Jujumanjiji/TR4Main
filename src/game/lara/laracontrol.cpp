#include "framework.h"
#include "lara.h"
#include "3d_gen.h"
#include "draw.h"
#include "camera.h"
#include "collide.h"
#include "control.h"
#include "items.h"
#include "sound.h"
#include "utils.h"

void(*lara_control_routines[NUM_LARA_STATES + 1])(ITEM_INFO* item, COLL_INFO* coll) = {
    lara_as_walk,
    lara_as_run,
    lara_as_stop,
    lara_as_forwardjump,
    dummy_func,
    lara_as_fastback,
    lara_as_turnr,
    lara_as_turnl,
    lara_as_death,
    lara_as_fastfall,
    lara_as_hang,
    lara_as_reach,
    lara_as_splat,
    lara_as_tread,
    dummy_func,
    lara_as_compress,
    lara_as_back,
    lara_as_swim,
    lara_as_glide,
    lara_as_null,
    lara_as_fastturn,
    lara_as_stepright,
    lara_as_stepleft,
    dummy_func,
    lara_as_slide,
    lara_as_backjump,
    lara_as_rightjump,
    lara_as_leftjump,
    lara_as_upjump,
    lara_as_fallback,
    lara_as_hangleft,
    lara_as_hangright,
    lara_as_slideback,
    lara_as_surftread,
    lara_as_surfswim,
    lara_as_dive,
    lara_as_pushblock,
    lara_as_pullblock,
    lara_as_ppready,
    lara_as_pickup,
    lara_as_switchon,
    lara_as_switchon,
    lara_as_usekey,
    lara_as_usepuzzle,
    lara_as_uwdeath,
    dummy_func,
    lara_as_special,
    lara_as_surfback,
    lara_as_surfleft,
    lara_as_surfright,
    dummy_func,
    dummy_func,
    lara_as_swandive,
    lara_as_fastdive,
    lara_as_null,               //; lara_as_gymnast
    lara_as_waterout,
    lara_as_climbstnc,
    lara_as_climbup,
    lara_as_climbleft,
    lara_as_climbend,
    lara_as_climbright,
    lara_as_climbdown,
    dummy_func,
    dummy_func,
    dummy_func,
    lara_as_wade,
    lara_as_waterroll,
    lara_as_pickupflare,
    dummy_func,
    dummy_func,
    lara_as_deathslide,
    lara_as_duck,
    lara_as_duck,
    lara_as_dash,
    lara_as_dashdive,
    lara_as_hang2,
    lara_as_monkeyswing,
    lara_as_monkeyl,
    lara_as_monkeyr,
    lara_as_monkey180,
    lara_as_all4s,
    lara_as_crawl,
    lara_as_hangturnl,
    lara_as_hangturnr,
    lara_as_all4turnl,
    lara_as_all4turnr,
    lara_as_crawlb,
    dummy_func,
    dummy_func,
    lara_as_controlled,
    lara_as_ropel,
    lara_as_roper,
    lara_as_controlled,
    lara_as_controlled,
    lara_as_controlled,
    lara_as_controlledl,
    lara_as_controlledl,
    lara_as_controlled,
    lara_as_pickup,
    lara_as_null,
    lara_as_null,
    lara_as_null,
    lara_as_poleleft,
    lara_as_poleright,
    lara_as_pulley,
    lara_as_duckl,
    lara_as_duckr,
    lara_as_extcornerl,
    lara_as_extcornerr,
    lara_as_intcornerl,
    lara_as_intcornerr,
    lara_as_rope,
    lara_as_climbrope,
    lara_as_climbroped,
    lara_as_rope,
    lara_as_rope,
    dummy_func,
    lara_as_controlled,
    NULL
};
void(*lara_collision_routines[NUM_LARA_STATES + 1])(ITEM_INFO* item, COLL_INFO* coll) = {
    lara_col_walk,
    lara_col_run,
    lara_col_stop,
    lara_col_forwardjump,
    lara_col_stop,
    lara_col_fastback,
    lara_col_turn, // right
    lara_col_turn, // left
    lara_col_death,
    lara_col_fastfall,
    lara_col_hang,
    lara_col_reach,
    lara_col_splat,
    lara_col_tread,
    lara_col_stop,
    lara_col_compress,
    lara_col_back,
    lara_col_swim,
    lara_col_glide,
    lara_default_col,
    lara_col_stop,
    lara_col_step, // right
    lara_col_step, // left
    lara_col_roll2,
    lara_col_slide,
    lara_col_backjump,
    lara_col_rightjump,
    lara_col_leftjump,
    lara_col_upjump,
    lara_col_fallback,
    lara_col_hangleft,
    lara_col_hangright,
    lara_col_slideback,
    lara_col_surftread,
    lara_col_surfswim,
    lara_col_dive,
    lara_default_col,
    lara_default_col,
    lara_default_col,
    lara_default_col,
    lara_default_col,
    lara_default_col,
    lara_default_col,
    lara_default_col,
    lara_col_uwdeath,
    lara_col_roll,
    dummy_func,
    lara_col_surfback,
    lara_col_surfleft,
    lara_col_surfright,
    dummy_func,
    dummy_func,
    lara_col_swandive,
    lara_col_fastdive,
    lara_default_col,
    lara_default_col,
    lara_col_climbstnc,
    lara_col_climbup,
    lara_col_climbleft,
    dummy_func,
    lara_col_climbright,
    lara_col_climbdown,
    dummy_func,
    dummy_func,
    dummy_func,
    lara_col_wade,
    lara_col_waterroll,
    lara_default_col,
    dummy_func,
    dummy_func,
    dummy_func,
    lara_col_duck,
    lara_col_duck,
    lara_col_dash,
    lara_col_dashdive,
    lara_col_hang2,
    lara_col_monkeyswing,
    lara_col_monkeyl,
    lara_col_monkeyr,
    lara_col_monkey180,
    lara_col_all4s,
    lara_col_crawl,
    lara_col_hangturn, // right
    lara_col_hangturn, // left
    lara_col_all4turn, // right
    lara_col_all4turn, // left
    lara_col_crawlb,
    dummy_func,
    lara_col_crawl2hang,
    lara_default_col,
    dummy_func,
    dummy_func,
    lara_default_col,
    dummy_func,
    dummy_func,
    lara_col_turnswitch,
    dummy_func,
    dummy_func,
    lara_default_col,
    lara_col_polestat,
    lara_col_poleup,
    lara_col_poledown,
    dummy_func,
    dummy_func,
    lara_default_col,
    lara_col_duck2,
    lara_col_duck2,
    lara_default_col,
    lara_default_col,
    lara_default_col,
    lara_default_col,
    lara_col_rope,
    dummy_func,
    dummy_func,
    lara_col_ropefwd,
    lara_col_ropefwd,
    dummy_func,
    dummy_func,
    NULL
};
void(*lara_camera_routines[NUM_LARA_STATES + 1])(ITEM_INFO* item, COLL_INFO* coll) = {

};

// Standard Lara Controller
void LaraAboveWater(ITEM_INFO* item, COLL_INFO* coll)
{
    coll->old.x = item->pos.x;
    coll->old.y = item->pos.y;
    coll->old.z = item->pos.z;
    coll->old_anim_state = item->state_current;
    coll->old_anim_number = item->current_anim;
    coll->old_frame_number = item->current_frame;
    coll->radius = LARA_RAD;
    coll->trigger = NULL;
    coll->lava_is_pit = FALSE;
    coll->slopes_are_pits = FALSE;
    coll->slopes_are_walls = FALSE;
    coll->enable_spaz = TRUE;
    coll->enable_baddie_push = TRUE;

    if (CHK_ANY(TrInput, IN_LOOK) && lara.look)
        LookLeftRight();
    else
        ResetLook();

    lara.look = TRUE;

    // TODO: vehicle control !!!!!
    if (lara.skidoo != NO_ITEM)
    {
        /*
        if ( dword_7FE28C[lara.skidoo].object_number == MOTORBIKE )
            MotorBikeControl(lara.skidoo);
        else
            JeepControl(lara.skidoo);
        */
        return;
    }

    if (item->state_current != STATE_LARA_DEATH && CHK_ANY(gf_level_flags, SLEV_TRAIN) && item->pos.y >= 0)
    {

    }

    lara_control_routines[item->state_current](item, coll);
    //lara_camera_routines[item->state_current](item, coll); // TODO: not started yet (camera routines) !

    if (item->pos.z_rot >= -LARA_LEAN_UNDO && item->pos.z_rot <= LARA_LEAN_UNDO)
        item->pos.z_rot = 0;
    else if (item->pos.z_rot < -LARA_LEAN_UNDO)
        item->pos.z_rot += LARA_LEAN_UNDO;
    else
        item->pos.z_rot -= LARA_LEAN_UNDO;

    if (lara.turn_rate >= -LARA_TURN_UNDO && lara.turn_rate <= LARA_TURN_UNDO)
        lara.turn_rate = 0;
    else if (lara.turn_rate < -LARA_TURN_UNDO)
        lara.turn_rate += LARA_TURN_UNDO;
    else
        lara.turn_rate -= LARA_TURN_UNDO;
    item->pos.y_rot += lara.turn_rate;

    AnimateLara(item);
    LaraBaddieCollision(item, coll);
    if (lara.skidoo == NO_ITEM)
        lara_collision_routines[item->state_current](item, coll);
    UpdateLaraRoom(item, -(LARA_HITE / 2));
    LaraGun();
    TestTriggers(coll->trigger, FALSE, NULL);
}