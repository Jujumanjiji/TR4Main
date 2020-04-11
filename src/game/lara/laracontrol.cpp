#include "framework.h"
#include "lara.h"
#include "3d_gen.h"
#include "draw.h"
#include "camera.h"
#include "collide.h"
#include "control.h"
#include "effects.h"
#include "gameflow.h"
#include "items.h"
#include "sound.h"
#include "utils.h"
#include "oldobjects.h"

static short cheat_hit_points = 0;
static bool cheatEnabled = false;
static bool haveStuff = false;

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

static void LaraCheatGetStuff(void)
{
    lara.flare_count = INFINITE_AMMO;
    lara.small_medipack_count = INFINITE_AMMO;
    lara.large_medipack_count = INFINITE_AMMO;
    if (objects[CROWBAR_ITEM].loaded)
        lara.crowbar = TRUE;
    lara.lasersight = TRUE;
    lara.uzi_type_carried = (CR_PRESENT | CR_AMMO1);
    lara.shotgun_type_carried = (CR_PRESENT | CR_AMMO1);
    lara.revolver_type_carried = (CR_PRESENT | CR_AMMO1);
    lara.uzi_ammo_count = INFINITE_AMMO;
    lara.revolver_ammo_count = INFINITE_AMMO;
    lara.shotgun_ammo1_count = INFINITE_AMMO;
}

static void LaraCheatyBits(void)
{
#ifdef DEBUG_CHEAT
    gameflow.cheat_enabled = TRUE;
#endif

    if (gameflow.cheat_enabled)
    {
        if (CHK_EXI(TrInput, IN_LOOK) && !haveStuff)
        {
            LaraCheatGetStuff();
            lara_item->hit_points = LARA_HITPOINTS;
            haveStuff = true;
        }

        if (CHK_EXI(TrInput, IN_SPRINT) && !cheatEnabled)
        {
            cheatEnabled = true;
            lara_item->pos.y -= STEP_L;
            if (lara.water_status != LWS_CHEAT)
            {
                lara.water_status = LWS_CHEAT;
                lara_item->anim_number = ANIMATION_LARA_UNDERWATER_SWIM_SOLID;
                lara_item->frame_number = anims[lara_item->anim_number].frame_base;
                lara_item->state_current = STATE_LARA_UNDERWATER_FORWARD;
                lara_item->state_next = STATE_LARA_UNDERWATER_FORWARD;
                lara_item->gravity_status = FALSE;
                lara_item->fallspeed = 30;
                lara.air = LARA_AIR;
                lara.death_count = 0;
                lara.torso_y_rot = 0;
                lara.torso_x_rot = 0;
                lara.head_y_rot = 0;
                lara.head_x_rot = 0;
                cheat_hit_points = lara_item->hit_points;
            }
        }
    }
}

#ifdef FEATURE_WADESPLASH_WHENEXIT
static void Feature_WadeSplash_Exit(ITEM_INFO* item)
{
    int wh = GetWaterHeight(item->pos.x, item->pos.y, item->pos.z, item->room_number);
    splash_setup.x = item->pos.x;
    splash_setup.y = wh;
    splash_setup.z = item->pos.z;
    splash_setup.inner_rad = 16;
    splash_setup.inner_size = 12;
    splash_setup.inner_radvel = 160;
    splash_setup.inner_yvel = -(item->fallspeed / 3) * 72;
    splash_setup.inner_y = 24;
    splash_setup.middle_rad = 24;
    splash_setup.middle_size = 224;
    splash_setup.middle_radvel = -(item->fallspeed / 3) * 36;
    splash_setup.middle_yvel = 32;
    splash_setup.middle_y = 32;
    splash_setup.outer_rad = 272;
    SetupSplash(&splash_setup);
    splash_count = 16;
}
#define WadeSplash_Exit Feature_WadeSplash_Exit
#else
#define WadeSplash_Exit
#endif

void LaraCheat(ITEM_INFO *item, COLL_INFO *coll)
{
    lara_item->hit_points = LARA_HITPOINTS;
    LaraUnderWater(item, coll);

    if (CHK_EXI(TrInput, IN_WALK) && CHK_NOP(TrInput, IN_LOOK))
    {
        cheatEnabled = false;
        lara.water_status = LWS_ABOVEWATER;
        item->anim_number = ANIMATION_LARA_STAY_SOLID;
        item->frame_number = anims[item->anim_number].frame_base;
        item->pos.x_rot = 0;
        item->pos.z_rot = 0;
        lara.torso_y_rot = 0;
        lara.torso_x_rot = 0;
        lara.head_y_rot = 0;
        lara.head_x_rot = 0;
        lara.gun_status = 0;
        lara.mesh_effects = NULL;
        lara_item->hit_points = cheat_hit_points;
        LaraInitialiseMeshes();
    }
}

static void LaraWadeControl(ITEM_INFO* item, int hfw)
{
    camera.target_elevation = -ANGLE(22);

    if (hfw < WADE_DEPTH)
    {
        lara.water_status = LWS_ABOVEWATER;
        if (item->state_current == STATE_LARA_WADE_FORWARD)
            item->state_next = STATE_LARA_RUN_FORWARD;
    }
    else if (hfw > SWIM_DEPTH)
    {
        lara.water_status = LWS_SURFACE;
        item->pos.y = item->pos.y + 1 - hfw;

        switch (item->state_current)
        {
        case STATE_LARA_WALK_BACK:
            item->anim_number = ANIMATION_LARA_ONWATER_IDLE_TO_SWIM_BACK;
            item->frame_number = anims[item->anim_number].frame_base;
            item->state_current = STATE_LARA_ONWATER_BACK;
            item->state_next = STATE_LARA_ONWATER_BACK;
            break;
        case STATE_LARA_WALK_RIGHT:
            item->anim_number = ANIMATION_LARA_ONWATER_SWIM_RIGHT;
            item->frame_number = anims[item->anim_number].frame_base;
            item->state_current = STATE_LARA_ONWATER_RIGHT;
            item->state_next = STATE_LARA_ONWATER_RIGHT;
            break;
        case STATE_LARA_WALK_LEFT:
            item->anim_number = ANIMATION_LARA_ONWATER_SWIM_LEFT;
            item->frame_number = anims[item->anim_number].frame_base;
            item->state_current = STATE_LARA_ONWATER_LEFT;
            item->state_next = STATE_LARA_ONWATER_LEFT;
            break;
        case STATE_LARA_WALK_FORWARD:
        default: // need default for some reason...
            item->anim_number = ANIMATION_LARA_ONWATER_SWIM_FORWARD;
            item->frame_number = anims[item->anim_number].frame_base;
            item->state_current = STATE_LARA_ONWATER_FORWARD;
            item->state_next = STATE_LARA_ONWATER_FORWARD;
            break;
        }

        item->gravity_status = FALSE;
        item->fallspeed = 0;
        lara.dive_count = 0;
        lara_item->pos.z_rot = 0;
        item->pos.x_rot = 0;
        lara.torso_y_rot = 0;
        lara.torso_x_rot = 0;
        lara.head_y_rot = 0;
        lara.head_x_rot = 0;
        UpdateLaraRoom(item, 0);
    }
}

void LaraControl(void)
{
    ITEM_INFO* item = lara_item;
    PHD_VECTOR last_pos;

    if (lara.is_moving)
    {
        lara.move_count++;
        if (lara.move_count > 90)
        {
            lara.is_moving = FALSE;
            lara.gun_status = LHS_ARMLESS;
        }
    }

    LaraCheatyBits();
    if (!bDisableLaraControl)
        lara.locate.pad = -128;

    last_pos.x = item->pos.x;
    last_pos.y = item->pos.y;
    last_pos.z = item->pos.z;

    if (lara.gun_status == LHS_HANDBUSY
    &&  item->state_current == STATE_LARA_IDLE
    &&  item->state_next == STATE_LARA_IDLE
    &&  item->anim_number == ANIMATION_LARA_STAY_IDLE
    && !item->gravity_status)
    {
        lara.gun_status = LHS_ARMLESS;
    }

    if (item->state_current != STATE_LARA_SPRINT && lara.dash_timer < LARA_DASH_TIME)
        lara.dash_timer++;
    lara.is_ducked = FALSE;

    bool is_water = RWATER(item->room_number);
    int wd, wh, hfw;
    wd = GetWaterDepth(item->pos.x, item->pos.y, item->pos.z, item->room_number);
    wh = GetWaterHeight(item->pos.x, item->pos.y, item->pos.z, item->room_number);

    if (wh == -NO_HEIGHT)
        hfw = -NO_HEIGHT;
    else
        hfw = item->pos.y - wh;
    lara.water_surface_dist = -hfw;

    if (lara.skidoo == NO_ITEM)
        WadeSplash(item, wh, wd);

    if (lara.skidoo == NO_ITEM)
    {
        switch (lara.water_status)
        {
            case LWS_ABOVEWATER:
                if (hfw != -NO_HEIGHT && hfw >= WADE_DEPTH)
                {
                    if (wd <= (SWIM_DEPTH - STEP_L))
                    {
                        if (hfw > WADE_DEPTH)
                        {
                            lara.water_status = LWS_WADE;
                            if (!item->gravity_status)
                                item->state_next = STATE_LARA_IDLE;
                        }
                    }
                    else if (is_water)
                    {
                        lara.air = LARA_AIR;
                        lara.water_status = LWS_UNDERWATER;
                        item->gravity_status = FALSE;
                        item->pos.y += 100;

                        UpdateLaraRoom(item, 0);
                        StopSoundEffect(SFX_LARA_FALL);

                        if (item->state_current == STATE_LARA_SWANDIVE_BEGIN)
                        {
                            item->pos.x_rot = -ANGLE(45);
                            item->state_next = ANIMATION_LARA_FREE_FALL_MIDDLE;
                            AnimateLara(item);
                            item->fallspeed *= 2;
                        }
                        else if (item->state_current == STATE_LARA_SWANDIVE_END)
                        {
                            item->pos.x_rot = -ANGLE(85);
                            item->state_next = STATE_LARA_UNDERWATER_DIVING;
                            AnimateLara(item);
                            item->fallspeed *= 2;
                        }
                        else
                        {
                            item->pos.x_rot = -ANGLE(45);
                            item->anim_number = ANIMATION_LARA_FREE_FALL_TO_UNDERWATER;
                            item->frame_number = anims[item->anim_number].frame_base;
                            item->state_current = STATE_LARA_UNDERWATER_DIVING;
                            item->state_next = STATE_LARA_UNDERWATER_FORWARD;
                            item->fallspeed = (item->fallspeed * 3) / 2;
                        }

                        lara.torso_y_rot = 0;
                        lara.torso_x_rot = 0;
                        lara.head_y_rot = 0;
                        lara.head_x_rot = 0;
                        Splash(item);
                    }

                    LaraWadeControl(item, hfw);
                }
                break;
            case LWS_UNDERWATER:
                if (!is_water)
                {
                    if (wd != -NO_HEIGHT && ABS(hfw) < STEP_L)
                    {
                        WadeSplash_Exit(item);

                        lara.water_status = LWS_SURFACE;
                        item->pos.y = item->pos.y + 1 - hfw;
                        item->anim_number = ANIMATION_LARA_UNDERWATER_TO_ONWATER;
                        item->frame_number = anims[item->anim_number].frame_base;
                        item->state_current = STATE_LARA_ONWATER_STOP;
                        item->state_next = STATE_LARA_ONWATER_STOP;
                        item->fallspeed = 0;

                        lara.dive_count = 11;
                        item->pos.x_rot = 0;
                        lara_item->pos.z_rot = 0;
                        lara.torso_y_rot = 0;
                        lara.torso_x_rot = 0;
                        lara.head_y_rot = 0;
                        lara.head_x_rot = 0;
                        UpdateLaraRoom(item, -(LARA_HITE / 2));
                        SoundEffect(SFX_LARA_BREATH, &lara_item->pos, SFX_ALWAYS);
                    }
                    else
                    {
                        lara.water_status = LWS_ABOVEWATER;
                        item->anim_number = ANIMATION_LARA_FREE_FALL_FORWARD;
                        item->frame_number = anims[item->anim_number].frame_base;
                        item->state_current = STATE_LARA_JUMP_FORWARD;
                        item->state_next = STATE_LARA_JUMP_FORWARD;
                        item->speed = item->fallspeed / 4;
                        item->fallspeed = 0;
                        item->gravity_status = TRUE;

                        item->pos.x_rot = 0;
                        lara_item->pos.z_rot = 0;
                        lara.torso_y_rot = 0;
                        lara.torso_x_rot = 0;
                        lara.head_y_rot = 0;
                        lara.head_x_rot = 0;
                    }
                }
                break;
            case LWS_SURFACE:
                if (!is_water)
                {
                    if (hfw > WADE_DEPTH)
                    {
                        lara.water_status = LWS_WADE;
                        item->anim_number = ANIMATION_LARA_STAY_IDLE;
                        item->frame_number = anims[item->anim_number].frame_base;
                        item->state_current = STATE_LARA_IDLE;
                        item->state_next = STATE_LARA_WADE_FORWARD;
                        AnimateItem(item);
                    }
                    else
                    {
                        lara.water_status = LWS_ABOVEWATER;
                        item->anim_number = ANIMATION_LARA_FREE_FALL_FORWARD;
                        item->frame_number = anims[item->anim_number].frame_base;
                        item->state_current = STATE_LARA_JUMP_FORWARD;
                        item->state_next = STATE_LARA_JUMP_FORWARD;
                        item->speed = item->fallspeed / 4;
                        item->gravity_status = TRUE;
                    }

                    item->fallspeed = 0;
                    item->pos.x_rot = 0;
                    lara_item->pos.z_rot = 0;
                    lara.torso_y_rot = 0;
                    lara.torso_x_rot = 0;
                    lara.head_y_rot = 0;
                    lara.head_x_rot = 0;
                }
                break;
            case LWS_WADE:
                LaraWadeControl(item, hfw);
                break;
            case LWS_CHEAT:
                if (gameflow.cheat_enabled)
                    LaraCheat(item, lara_coll);
                break;
        }
    }

    // TODO: S_SetReverbTypes() NOTE: it's a dummy_func() in TR4 but it's because of IDAPro !
    
    if (item->hit_points <= 0)
    {
        item->hit_points = 0;

        if (!lara.death_count)
            S_CDStop();

        lara.death_count++;
        if (CHK_EXI(item->flags, IFLAG_ONESHOT))
        {
            lara.death_count++;
            return;
        }
    }

    switch (lara.water_status)
    {
        case LWS_ABOVEWATER:
        case LWS_WADE:
            if (lara.skidoo == NO_ITEM)
                lara.air = 1800;
            LaraAboveWater(item, lara_coll);
            break;
        case LWS_UNDERWATER:
            if (item->hit_points >= 0)
            {
                lara.air--;
                if (lara.air < 0)
                {
                    lara.air = -1;
                    item->hit_points -= 5;
                }
            }

            LaraUnderWater(item, lara_coll);
            break;
        case LWS_SURFACE:
            if (item->hit_points >= 0)
            {
                lara.air += 10;
                if (lara.air > LARA_AIR)
                    lara.air = LARA_AIR;
            }

            LaraSurface(item, lara_coll);
            break;
#ifdef DEBUG_CHEAT // speed up the flycheat (2x with this)
        case LWS_CHEAT:
            if (gameflow.cheat_enabled)
                LaraCheat(item, lara_coll);
            break;
#endif
    }

    savegame_level.distance += phd_sqrt(
        SQUARE(item->pos.z - last_pos.z) +
        SQUARE(item->pos.y - last_pos.y) +
        SQUARE(item->pos.x - last_pos.x)
    );
}

// Standard Lara Controller
void LaraAboveWater(ITEM_INFO* item, COLL_INFO* coll)
{
    coll->old.x = item->pos.x;
    coll->old.y = item->pos.y;
    coll->old.z = item->pos.z;
    coll->old_anim_state = item->state_current;
    coll->old_anim_number = item->anim_number;
    coll->old_frame_number = item->frame_number;
    coll->radius = LARA_RAD;
    coll->trigger = NULL;
    coll->lava_is_pit = FALSE;
    coll->slopes_are_pits = FALSE;
    coll->slopes_are_walls = FALSE;
    coll->enable_spaz = TRUE;
    coll->enable_baddie_push = TRUE;

    if (CHK_EXI(TrInput, IN_LOOK) && lara.look)
        LookLeftRight();
    else
        ResetLook();
    lara.look = TRUE;

    if (lara.skidoo != NO_ITEM)
    {
        ITEM_INFO* item = &items[lara.skidoo];
        switch (item->object_number)
        {
            case MOTORBIKE:
                if (MotorBikeControl(lara.skidoo))
                    return;
                break;
            case JEEP:
                if (JeepControl(lara.skidoo))
                    return;
                break;
        }
        return;
    }

    if (item->state_current != STATE_LARA_DEATH && CHK_EXI(gfLevelFlags, SLEV_TRAIN) && item->pos.y >= 0)
    {
        if (item->pos.z <= 51900 || item->pos.z >= 53554)
        {
            camera.mike_pos.y = -WALL_L;
            camera.mike_pos.z = item->pos.z > 51900 ? 55296 : 50176;
        }
        else
        {
            camera.mike_pos.y = -STEPUP_HEIGHT;
            camera.mike_pos.z = 52736;
        }

        camera.mike_pos.x = item->pos.x;
        camera.mike_pos_room = item->room_number;
        force_fixed_camera = true;
        SetAnimationForItemAS(item, ANIMATION_LARA_TRAIN_DEATH, STATE_LARA_DEATH);
        item->pos.y_rot = -0x4000;
        item->speed = 0;
        item->fallspeed = 0;
        item->gravity_status = FALSE;
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

void LaraSurface(ITEM_INFO* item, COLL_INFO* coll)
{
    camera.target_elevation = -ANGLE(22);
    coll->old.x = item->pos.x;
    coll->old.y = item->pos.y;
    coll->old.z = item->pos.z;
    coll->bad_pos = NO_HEIGHT;
    coll->bad_neg = -(STEP_L / 2);
    coll->bad_ceiling = LARA_RAD;
    coll->radius = SURF_RADIUS;
    coll->trigger = NULL;
    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = FALSE;
    coll->lava_is_pit = FALSE;
    coll->slopes_are_pits = FALSE;
    coll->slopes_are_walls = FALSE;

    if (CHK_EXI(TrInput, IN_LOOK) && lara.look)
        LookLeftRight();
    else
        ResetLook();
    lara.look = TRUE;

    lara_control_routines[item->state_current](item, coll);
    //lara_camera_routines[item->state_current](item, coll); // not implemented yet

    if (item->pos.z_rot >= -(2 * LARA_LEAN_UNDO) && item->pos.z_rot <= (2 * LARA_LEAN_UNDO))
        item->pos.z_rot = 0;
    else if (item->pos.z_rot < 0)
        item->pos.z_rot += (2 * LARA_LEAN_UNDO);
    else
        item->pos.z_rot -= (2 * LARA_LEAN_UNDO);

    if (lara.current_active && lara.water_status != LWS_CHEAT)
        LaraWaterCurrent(coll);

    AnimateLara(item);

    item->pos.x += (item->fallspeed * SIN(lara.move_angle)) >> (W2V_SHIFT + 2);
    item->pos.z += (item->fallspeed * COS(lara.move_angle)) >> (W2V_SHIFT + 2);

    LaraBaddieCollision(item, coll);
    if (lara.skidoo == NO_ITEM)
        lara_collision_routines[item->state_current](item, coll);
    UpdateLaraRoom(item, LARA_RAD);
    LaraGun();
    TestTriggers(coll->trigger, FALSE, NULL);
}

void LaraUnderWater(ITEM_INFO* item, COLL_INFO* coll)
{
    coll->old.x = item->pos.x;
    coll->old.y = item->pos.y;
    coll->old.z = item->pos.z;
    coll->bad_pos = NO_HEIGHT;
    coll->bad_neg = -UW_HITE;
    coll->bad_ceiling = UW_HITE;
    coll->radius = UW_RADIUS;
    coll->trigger = NULL;
    coll->enable_spaz = FALSE;
    coll->enable_baddie_push = FALSE;
    coll->lava_is_pit = FALSE;
    coll->slopes_are_pits = FALSE;
    coll->slopes_are_walls = FALSE;

    if (CHK_EXI(TrInput, IN_LOOK) && lara.look)
        LookLeftRight();
    else
        ResetLook();
    lara.look = TRUE;

    lara_control_routines[item->state_current](item, coll);
    //lara_camera_routines[item->state_current](item, coll); // not implemented yet

    if (item->pos.z_rot >= -(2 * LARA_LEAN_UNDO) && item->pos.z_rot <= (2 * LARA_LEAN_UNDO))
        item->pos.z_rot = 0;
    else if (item->pos.z_rot < 0)
        item->pos.z_rot += (2 * LARA_LEAN_UNDO);
    else
        item->pos.z_rot -= (2 * LARA_LEAN_UNDO);

    if (item->pos.x_rot < -ANGLE(85))
        item->pos.x_rot = -ANGLE(85);
    else if (item->pos.x_rot > ANGLE(85))
        item->pos.x_rot = ANGLE(85);

    if (item->pos.z_rot < -LARA_LEAN_MAX_UW)
        item->pos.z_rot = -LARA_LEAN_MAX_UW;
    else if (item->pos.z_rot > LARA_LEAN_MAX_UW)
        item->pos.z_rot = LARA_LEAN_MAX_UW;

    if (lara.turn_rate >= -LARA_TURN_UNDO && lara.turn_rate <= LARA_TURN_UNDO)
        lara.turn_rate = 0;
    else if (lara.turn_rate < -LARA_TURN_UNDO)
        lara.turn_rate += LARA_TURN_UNDO;
    else
        lara.turn_rate -= LARA_TURN_UNDO;
    item->pos.y_rot += lara.turn_rate;

    if (lara.current_active && lara.water_status != LWS_CHEAT)
        LaraWaterCurrent(coll);

    AnimateLara(item);
    item->pos.y -= (  SIN(item->pos.x_rot) * item->fallspeed  >> (W2V_SHIFT + 2));
    item->pos.x += (((SIN(item->pos.y_rot) * item->fallspeed) >> (W2V_SHIFT + 2)) * COS(item->pos.x_rot)) >> W2V_SHIFT;
    item->pos.z += (((COS(item->pos.y_rot) * item->fallspeed) >> (W2V_SHIFT + 2)) * COS(item->pos.x_rot)) >> W2V_SHIFT;

    if (lara.water_status != LWS_CHEAT)
        LaraBaddieCollision(item, coll);
    if (lara.skidoo == NO_ITEM)
        lara_collision_routines[item->state_current](item, coll);
    UpdateLaraRoom(item, 0);
    LaraGun();
    TestTriggers(coll->trigger, FALSE, NULL);
}

// sink mode
void LaraWaterCurrent(COLL_INFO* coll)
{
    ITEM_INFO *item;
    PHD_VECTOR target;
    long angle, dx, dz, speed, sinkval;
    long shifter, absvel;

    item = lara_item;
    if (!lara.current_active)
    {
        absvel = abs(lara.current_xvel);
        if (absvel > 16)
            shifter = 4;
        else if (absvel > 8)
            shifter = 3;
        else
            shifter = 2;
        lara.current_xvel -= lara.current_xvel >> shifter;
        if (absvel < 4)
            lara.current_xvel = 0;

        absvel = abs(lara.current_zvel);
        if (absvel > 16)
            shifter = 4;
        else if (absvel > 8)
            shifter = 3;
        else
            shifter = 2;
        lara.current_zvel -= lara.current_zvel >> shifter;
        if (absvel < 4)
            lara.current_zvel = 0;

        if (lara.current_xvel == 0 && lara.current_zvel == 0)
            return;
    }
    else
    {
        sinkval = lara.current_active - 1;
        target.x = camera.fixed[sinkval].x;
        target.y = camera.fixed[sinkval].y;
        target.z = camera.fixed[sinkval].z;
        speed    = camera.fixed[sinkval].data;
        angle = ((mGetAngle(target.x, target.z, item->pos.x, item->pos.z) - 0x4000) >> 4) & 4095;
        dx = speed * rcossin_tbl[2 * angle] >> 2;
        dz = speed * rcossin_tbl[2 * angle + 1] >> 2;
        lara.current_xvel += (short)(dx - lara.current_xvel) >> 4;
        lara.current_zvel += (short)(dz - lara.current_zvel) >> 4;
        item->pos.y += (target.y - item->pos.y) >> 4;
    }

    item->pos.x += (lara.current_xvel >> 8);
    item->pos.z += (lara.current_zvel >> 8);
    lara.current_active = 0;

    coll->facing = phd_atan(item->pos.z - coll->old.z, item->pos.x - coll->old.x);
    GetCollisionInfo(coll, item->pos.x, item->pos.y + (UW_HITE / 2), item->pos.z, item->room_number, UW_HITE);

    switch (coll->coll_type)
    {
        case COLL_FRONT:
            if (item->pos.x_rot > ANGLE(35))
                item->pos.x_rot += UW_WALLDEFLECT;
            else if (item->pos.x_rot < -ANGLE(35))
                item->pos.x_rot -= UW_WALLDEFLECT;
            else
                item->fallspeed = 0;
            break;
        case COLL_TOP:
            item->pos.x_rot -= UW_WALLDEFLECT;
            break;
        case COLL_TOPFRONT:
            item->fallspeed = 0;
            break;
        case COLL_LEFT:
            item->pos.y_rot += ANGLE(5);
            break;
        case COLL_RIGHT:
            item->pos.y_rot -= ANGLE(5);
            break;
    }

    if (coll->mid_floor < 0 && coll->mid_floor != -NO_HEIGHT)
        item->pos.y += coll->mid_floor;

    ShiftItem(item, coll);
    coll->old.x = item->pos.x;
    coll->old.y = item->pos.y;
    coll->old.z = item->pos.z;
}