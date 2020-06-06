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
#include "motorbike.h"

static short cheat_hit_points = 0;
static bool cheatEnabled = false;
static bool haveStuff = false;

void(*lara_control_routines[NUM_LARA_STATES_TR4 + 1])(ITEM_INFO* item, COLL_INFO* coll) = {
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
void(*lara_collision_routines[NUM_LARA_STATES_TR4 + 1])(ITEM_INFO* item, COLL_INFO* coll) = {
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
void(*lara_camera_routines[NUM_LARA_STATES_TR4 + 1])(ITEM_INFO* item, COLL_INFO* coll) = {

};

static void LaraCheatGetStuff(void)
{
    Lara.flare_count = INFINITE_AMMO;
    Lara.small_medipack_count = INFINITE_AMMO;
    Lara.large_medipack_count = INFINITE_AMMO;
    if (Objects[CROWBAR_ITEM].loaded)
        Lara.crowbar = TRUE;
    Lara.lasersight = TRUE;
    Lara.uzi_type_carried = (CR_PRESENT | CR_AMMO1);
    Lara.shotgun_type_carried = (CR_PRESENT | CR_AMMO1);
    Lara.revolver_type_carried = (CR_PRESENT | CR_AMMO1);
    Lara.uzi_ammo_count = INFINITE_AMMO;
    Lara.revolver_ammo_count = INFINITE_AMMO;
    Lara.shotgun_ammo1_count = INFINITE_AMMO;
}

static void LaraCheatyBits(void)
{
    if (gameflow->cheat_enabled)
    {
        if (CHK_EXI(TrInput, IN_LOOK) && !haveStuff)
        {
            LaraCheatGetStuff();
            LaraItem->hitPoints = LARA_HITPOINTS;
            haveStuff = true;
        }

        if (CHK_EXI(TrInput, IN_SPRINT) && !cheatEnabled)
        {
            cheatEnabled = true;
            LaraItem->pos.yPos -= STEP_L;
            if (Lara.waterStatus != LWS_CHEAT)
            {
                Lara.waterStatus = LWS_CHEAT;
                LaraItem->animNumber = ANIMATION_LARA_UNDERWATER_SWIM_SOLID;
                LaraItem->frameNumber = Anims[LaraItem->animNumber].frameBase;
                LaraItem->currentAnimState = STATE_LARA_UNDERWATER_FORWARD;
                LaraItem->goalAnimState = STATE_LARA_UNDERWATER_FORWARD;
                LaraItem->gravityStatus = FALSE;
                LaraItem->fallspeed = 30;
                Lara.air = LARA_AIR;
                Lara.deathCount = 0;
                Lara.torsoYrot = 0;
                Lara.torsoXrot = 0;
                Lara.headYrot = 0;
                Lara.headXrot = 0;
                cheat_hit_points = LaraItem->hitPoints;
            }
        }
    }
}

#ifdef FEATURE_WADESPLASH_WHENEXIT
static void Feature_WadeSplash_Exit(ITEM_INFO* item)
{
    int wh = GetWaterHeight(item->pos.xPos, item->pos.yPos, item->pos.zPos, item->roomNumber);
    splash_setup.x = item->pos.xPos;
    splash_setup.y = wh;
    splash_setup.z = item->pos.zPos;
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
    LaraItem->hitPoints = LARA_HITPOINTS;
    LaraUnderWater(item, coll);

    if (CHK_EXI(TrInput, IN_WALK) && CHK_NOP(TrInput, IN_LOOK))
    {
        cheatEnabled = false;
        Lara.waterStatus = LWS_ABOVEWATER;
        item->animNumber = ANIMATION_LARA_STAY_SOLID;
        item->frameNumber = Anims[item->animNumber].frameBase;
        item->pos.xRot = 0;
        item->pos.zRot = 0;
        Lara.torsoYrot = 0;
        Lara.torsoXrot = 0;
        Lara.headYrot = 0;
        Lara.headXrot = 0;
        Lara.gunStatus = 0;
        Lara.meshEffects = NULL;
        LaraItem->hitPoints = cheat_hit_points;
        LaraInitialiseMeshes();
    }
}

static void LaraWadeControl(ITEM_INFO* item, int hfw)
{
    camera.target_elevation = -ANGLE(22);

    if (hfw < WADE_DEPTH)
    {
        Lara.waterStatus = LWS_ABOVEWATER;
        if (item->currentAnimState == STATE_LARA_WADE_FORWARD)
            item->goalAnimState = STATE_LARA_RUN_FORWARD;
    }
    else if (hfw > SWIM_DEPTH)
    {
        Lara.waterStatus = LWS_SURFACE;
        item->pos.yPos = item->pos.yPos + 1 - hfw;

        switch (item->currentAnimState)
        {
        case STATE_LARA_WALK_BACK:
            item->animNumber = ANIMATION_LARA_ONWATER_IDLE_TO_SWIM_BACK;
            item->frameNumber = Anims[item->animNumber].frameBase;
            item->currentAnimState = STATE_LARA_ONWATER_BACK;
            item->goalAnimState = STATE_LARA_ONWATER_BACK;
            break;
        case STATE_LARA_WALK_RIGHT:
            item->animNumber = ANIMATION_LARA_ONWATER_SWIM_RIGHT;
            item->frameNumber = Anims[item->animNumber].frameBase;
            item->currentAnimState = STATE_LARA_ONWATER_RIGHT;
            item->goalAnimState = STATE_LARA_ONWATER_RIGHT;
            break;
        case STATE_LARA_WALK_LEFT:
            item->animNumber = ANIMATION_LARA_ONWATER_SWIM_LEFT;
            item->frameNumber = Anims[item->animNumber].frameBase;
            item->currentAnimState = STATE_LARA_ONWATER_LEFT;
            item->goalAnimState = STATE_LARA_ONWATER_LEFT;
            break;
        case STATE_LARA_WALK_FORWARD:
        default: // need default for some reason...
            item->animNumber = ANIMATION_LARA_ONWATER_SWIM_FORWARD;
            item->frameNumber = Anims[item->animNumber].frameBase;
            item->currentAnimState = STATE_LARA_ONWATER_FORWARD;
            item->goalAnimState = STATE_LARA_ONWATER_FORWARD;
            break;
        }

        item->gravityStatus = FALSE;
        item->fallspeed = 0;
        Lara.diveCount = 0;
        LaraItem->pos.zRot = 0;
        item->pos.xRot = 0;
        Lara.torsoYrot = 0;
        Lara.torsoXrot = 0;
        Lara.headYrot = 0;
        Lara.headXrot = 0;
        UpdateLaraRoom(item, 0);
    }
}

void LaraControl(void)
{
    ITEM_INFO* item = LaraItem;
    PHD_VECTOR last_pos;

    if (Lara.isMoving)
    {
        Lara.moveCount++;
        if (Lara.moveCount > 90)
        {
            Lara.isMoving = FALSE;
            Lara.gunStatus = LHS_ARMLESS;
        }
    }

    LaraCheatyBits();
    if (!bDisableLaraControl)
        Lara.locationPad = -128;

    last_pos.x = item->pos.xPos;
    last_pos.y = item->pos.yPos;
    last_pos.z = item->pos.zPos;

    if (Lara.gunStatus == LHS_HANDBUSY
    &&  item->currentAnimState == STATE_LARA_IDLE
    &&  item->goalAnimState == STATE_LARA_IDLE
    &&  item->animNumber == ANIMATION_LARA_STAY_IDLE
    && !item->gravityStatus)
    {
        Lara.gunStatus = LHS_ARMLESS;
    }

    if (item->currentAnimState != STATE_LARA_SPRINT && Lara.dashTimer < LARA_DASH_TIME)
        Lara.dashTimer++;
    Lara.isDucked = FALSE;

    bool is_water = RWATER(item->roomNumber);
    int wd, wh, hfw;
    wd = GetWaterDepth(item->pos.xPos, item->pos.yPos, item->pos.zPos, item->roomNumber);
    wh = GetWaterHeight(item->pos.xPos, item->pos.yPos, item->pos.zPos, item->roomNumber);

    if (wh == -NO_HEIGHT)
        hfw = -NO_HEIGHT;
    else
        hfw = item->pos.yPos - wh;
    Lara.waterSurfaceDist = -hfw;

    if (Lara.skidoo == NO_ITEM)
        WadeSplash(item, wh, wd);

    if (Lara.skidoo == NO_ITEM)
    {
        switch (Lara.waterStatus)
        {
            case LWS_ABOVEWATER:
                if (hfw != -NO_HEIGHT && hfw >= WADE_DEPTH)
                {
                    if (wd <= (SWIM_DEPTH - STEP_L))
                    {
                        if (hfw > WADE_DEPTH)
                        {
                            Lara.waterStatus = LWS_WADE;
                            if (!item->gravityStatus)
                                item->goalAnimState = STATE_LARA_IDLE;
                        }
                    }
                    else if (is_water)
                    {
                        Lara.air = LARA_AIR;
                        Lara.waterStatus = LWS_UNDERWATER;
                        item->gravityStatus = FALSE;
                        item->pos.yPos += 100;

                        UpdateLaraRoom(item, 0);
                        StopSoundEffect(SFX_LARA_FALL);

                        if (item->currentAnimState == STATE_LARA_SWANDIVE_BEGIN)
                        {
                            item->pos.xRot = -ANGLE(45);
                            item->goalAnimState = ANIMATION_LARA_FREE_FALL_MIDDLE;
                            AnimateLara(item);
                            item->fallspeed *= 2;
                        }
                        else if (item->currentAnimState == STATE_LARA_SWANDIVE_END)
                        {
                            item->pos.xRot = -ANGLE(85);
                            item->goalAnimState = STATE_LARA_UNDERWATER_DIVING;
                            AnimateLara(item);
                            item->fallspeed *= 2;
                        }
                        else
                        {
                            item->pos.xRot = -ANGLE(45);
                            item->animNumber = ANIMATION_LARA_FREE_FALL_TO_UNDERWATER;
                            item->frameNumber = Anims[item->animNumber].frameBase;
                            item->currentAnimState = STATE_LARA_UNDERWATER_DIVING;
                            item->goalAnimState = STATE_LARA_UNDERWATER_FORWARD;
                            item->fallspeed = (item->fallspeed * 3) / 2;
                        }

                        Lara.torsoYrot = 0;
                        Lara.torsoXrot = 0;
                        Lara.headYrot = 0;
                        Lara.headXrot = 0;
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

                        Lara.waterStatus = LWS_SURFACE;
                        item->pos.yPos = item->pos.yPos + 1 - hfw;
                        item->animNumber = ANIMATION_LARA_UNDERWATER_TO_ONWATER;
                        item->frameNumber = Anims[item->animNumber].frameBase;
                        item->currentAnimState = STATE_LARA_ONWATER_STOP;
                        item->goalAnimState = STATE_LARA_ONWATER_STOP;
                        item->fallspeed = 0;

                        Lara.diveCount = 11;
                        item->pos.xRot = 0;
                        LaraItem->pos.zRot = 0;
                        Lara.torsoYrot = 0;
                        Lara.torsoXrot = 0;
                        Lara.headYrot = 0;
                        Lara.headXrot = 0;
                        UpdateLaraRoom(item, -(LARA_HITE / 2));
                        SoundEffect(SFX_LARA_BREATH, &LaraItem->pos, SFX_ALWAYS);
                    }
                    else
                    {
                        Lara.waterStatus = LWS_ABOVEWATER;
                        item->animNumber = ANIMATION_LARA_FREE_FALL_FORWARD;
                        item->frameNumber = Anims[item->animNumber].frameBase;
                        item->currentAnimState = STATE_LARA_JUMP_FORWARD;
                        item->goalAnimState = STATE_LARA_JUMP_FORWARD;
                        item->speed = item->fallspeed / 4;
                        item->fallspeed = 0;
                        item->gravityStatus = TRUE;

                        item->pos.xRot = 0;
                        LaraItem->pos.zRot = 0;
                        Lara.torsoYrot = 0;
                        Lara.torsoXrot = 0;
                        Lara.headYrot = 0;
                        Lara.headXrot = 0;
                    }
                }
                break;
            case LWS_SURFACE:
                if (!is_water)
                {
                    if (hfw > WADE_DEPTH)
                    {
                        Lara.waterStatus = LWS_WADE;
                        item->animNumber = ANIMATION_LARA_STAY_IDLE;
                        item->frameNumber = Anims[item->animNumber].frameBase;
                        item->currentAnimState = STATE_LARA_IDLE;
                        item->goalAnimState = STATE_LARA_WADE_FORWARD;
                        AnimateItem(item);
                    }
                    else
                    {
                        Lara.waterStatus = LWS_ABOVEWATER;
                        item->animNumber = ANIMATION_LARA_FREE_FALL_FORWARD;
                        item->frameNumber = Anims[item->animNumber].frameBase;
                        item->currentAnimState = STATE_LARA_JUMP_FORWARD;
                        item->goalAnimState = STATE_LARA_JUMP_FORWARD;
                        item->speed = item->fallspeed / 4;
                        item->gravityStatus = TRUE;
                    }

                    item->fallspeed = 0;
                    item->pos.xRot = 0;
                    LaraItem->pos.zRot = 0;
                    Lara.torsoYrot = 0;
                    Lara.torsoXrot = 0;
                    Lara.headYrot = 0;
                    Lara.headXrot = 0;
                }
                break;
            case LWS_WADE:
                LaraWadeControl(item, hfw);
                break;
            case LWS_CHEAT:
                if (gameflow->cheat_enabled)
                    LaraCheat(item, lara_coll);
                break;
        }
    }

    // TODO: S_SetReverbTypes() NOTE: it's a dummy_func() in TR4 but it's because of IDAPro !
    
    if (item->hitPoints <= 0)
    {
        item->hitPoints = 0;

        if (!Lara.deathCount)
            S_CDStop();

        Lara.deathCount++;
        if (CHK_EXI(item->flags, IFLAG_ONESHOT))
        {
            Lara.deathCount++;
            return;
        }
    }

    switch (Lara.waterStatus)
    {
        case LWS_ABOVEWATER:
        case LWS_WADE:
            if (Lara.skidoo == NO_ITEM)
                Lara.air = 1800;
            LaraAboveWater(item, lara_coll);
            break;
        case LWS_UNDERWATER:
            if (item->hitPoints >= 0)
            {
                Lara.air--;
                if (Lara.air < 0)
                {
                    Lara.air = -1;
                    item->hitPoints -= 5;
                }
            }

            LaraUnderWater(item, lara_coll);
            break;
        case LWS_SURFACE:
            if (item->hitPoints >= 0)
            {
                Lara.air += 10;
                if (Lara.air > LARA_AIR)
                    Lara.air = LARA_AIR;
            }

            LaraSurface(item, lara_coll);
            break;
#ifdef DEBUG_CHEAT // speed up the flycheat (2x with this)
        case LWS_CHEAT:
            if (gameflow->cheat_enabled)
                LaraCheat(item, lara_coll);
            break;
#endif
    }

    savegame_level.distance += phd_sqrt(
        SQUARE(item->pos.zPos - last_pos.z) +
        SQUARE(item->pos.yPos - last_pos.y) +
        SQUARE(item->pos.xPos - last_pos.x)
    );
}

// Standard Lara Controller
void LaraAboveWater(ITEM_INFO* item, COLL_INFO* coll)
{
    coll->old.x = item->pos.xPos;
    coll->old.y = item->pos.yPos;
    coll->old.z = item->pos.zPos;
    coll->oldAnimState = item->currentAnimState;
    coll->oldAnimNumber = item->animNumber;
    coll->oldFrameNumber = item->frameNumber;
    coll->radius = LARA_RAD;
    coll->trigger = NULL;
    coll->lavaIsPit = FALSE;
    coll->slopesArePits = FALSE;
    coll->slopesAreWalls = FALSE;
    coll->enableSpaz = TRUE;
    coll->enableBaddiePush = TRUE;

    if (CHK_EXI(TrInput, IN_LOOK) && Lara.look)
        LookLeftRight();
    else
        ResetLook();
    Lara.look = TRUE;

    if (Lara.skidoo != NO_ITEM)
    {
        ITEM_INFO* item = &Items[Lara.skidoo];
        switch (item->objectNumber)
        {
            case MOTORBIKE:
                if (MotorBikeControl())
                    return;
                break;
            case JEEP:
                if (JeepControl(Lara.skidoo))
                    return;
                break;
        }
        return;
    }

    if (item->currentAnimState != STATE_LARA_DEATH && CHK_EXI(gfLevelFlags, SLEV_TRAIN) && item->pos.yPos >= 0)
    {
        if (item->pos.zPos <= 51900 || item->pos.zPos >= 53554)
        {
            camera.mike_pos.y = -WALL_L;
            camera.mike_pos.z = item->pos.zPos > 51900 ? 55296 : 50176;
        }
        else
        {
            camera.mike_pos.y = -STEPUP_HEIGHT;
            camera.mike_pos.z = 52736;
        }

        camera.mike_pos.x = item->pos.xPos;
        camera.mike_pos_room = item->roomNumber;
        force_fixed_camera = true;
        SetAnimationForItemAS(item, ANIMATION_LARA_TRAIN_DEATH, STATE_LARA_DEATH);
        item->pos.yRot = -0x4000;
        item->speed = 0;
        item->fallspeed = 0;
        item->gravityStatus = FALSE;
    }

    lara_control_routines[item->currentAnimState](item, coll);
    //lara_camera_routines[item->state_current](item, coll); // TODO: not started yet (camera routines) !

    if (item->pos.zRot >= -LARA_LEAN_UNDO && item->pos.zRot <= LARA_LEAN_UNDO)
        item->pos.zRot = 0;
    else if (item->pos.zRot < -LARA_LEAN_UNDO)
        item->pos.zRot += LARA_LEAN_UNDO;
    else
        item->pos.zRot -= LARA_LEAN_UNDO;

    if (Lara.turnRate >= -LARA_TURN_UNDO && Lara.turnRate <= LARA_TURN_UNDO)
        Lara.turnRate = 0;
    else if (Lara.turnRate < -LARA_TURN_UNDO)
        Lara.turnRate += LARA_TURN_UNDO;
    else
        Lara.turnRate -= LARA_TURN_UNDO;
    item->pos.yRot += Lara.turnRate;

    AnimateLara(item);
    LaraBaddieCollision(item, coll);
    if (Lara.skidoo == NO_ITEM)
        lara_collision_routines[item->currentAnimState](item, coll);
    UpdateLaraRoom(item, -(LARA_HITE / 2));
    LaraGun();
    TestTriggers(coll->trigger, FALSE, NULL);
}

void LaraSurface(ITEM_INFO* item, COLL_INFO* coll)
{
    camera.target_elevation = -ANGLE(22);
    coll->old.x = item->pos.xPos;
    coll->old.y = item->pos.yPos;
    coll->old.z = item->pos.zPos;
    coll->badPos = NO_HEIGHT;
    coll->badNeg = -(STEP_L / 2);
    coll->badCeiling = LARA_RAD;
    coll->radius = SURF_RADIUS;
    coll->trigger = NULL;
    coll->enableSpaz = FALSE;
    coll->enableBaddiePush = FALSE;
    coll->lavaIsPit = FALSE;
    coll->slopesArePits = FALSE;
    coll->slopesAreWalls = FALSE;

    if (CHK_EXI(TrInput, IN_LOOK) && Lara.look)
        LookLeftRight();
    else
        ResetLook();
    Lara.look = TRUE;

    lara_control_routines[item->currentAnimState](item, coll);
    //lara_camera_routines[item->state_current](item, coll); // not implemented yet

    if (item->pos.zRot >= -(2 * LARA_LEAN_UNDO) && item->pos.zRot <= (2 * LARA_LEAN_UNDO))
        item->pos.zRot = 0;
    else if (item->pos.zRot < 0)
        item->pos.zRot += (2 * LARA_LEAN_UNDO);
    else
        item->pos.zRot -= (2 * LARA_LEAN_UNDO);

    if (Lara.currentActive && Lara.waterStatus != LWS_CHEAT)
        LaraWaterCurrent(coll);

    AnimateLara(item);

    item->pos.xPos += (item->fallspeed * SIN(Lara.moveAngle)) >> (W2V_SHIFT + 2);
    item->pos.zPos += (item->fallspeed * COS(Lara.moveAngle)) >> (W2V_SHIFT + 2);

    LaraBaddieCollision(item, coll);
    if (Lara.skidoo == NO_ITEM)
        lara_collision_routines[item->currentAnimState](item, coll);
    UpdateLaraRoom(item, LARA_RAD);
    LaraGun();
    TestTriggers(coll->trigger, FALSE, NULL);
}

void LaraUnderWater(ITEM_INFO* item, COLL_INFO* coll)
{
    coll->old.x = item->pos.xPos;
    coll->old.y = item->pos.yPos;
    coll->old.z = item->pos.zPos;
    coll->badPos = NO_HEIGHT;
    coll->badNeg = -UW_HITE;
    coll->badCeiling = UW_HITE;
    coll->radius = UW_RADIUS;
    coll->trigger = NULL;
    coll->enableSpaz = FALSE;
    coll->enableBaddiePush = FALSE;
    coll->lavaIsPit = FALSE;
    coll->slopesArePits = FALSE;
    coll->slopesAreWalls = FALSE;

    if (CHK_EXI(TrInput, IN_LOOK) && Lara.look)
        LookLeftRight();
    else
        ResetLook();
    Lara.look = TRUE;

    lara_control_routines[item->currentAnimState](item, coll);
    //lara_camera_routines[item->state_current](item, coll); // not implemented yet

    if (item->pos.zRot >= -(2 * LARA_LEAN_UNDO) && item->pos.zRot <= (2 * LARA_LEAN_UNDO))
        item->pos.zRot = 0;
    else if (item->pos.zRot < 0)
        item->pos.zRot += (2 * LARA_LEAN_UNDO);
    else
        item->pos.zRot -= (2 * LARA_LEAN_UNDO);

    if (item->pos.xRot < -ANGLE(85))
        item->pos.xRot = -ANGLE(85);
    else if (item->pos.xRot > ANGLE(85))
        item->pos.xRot = ANGLE(85);

    if (item->pos.zRot < -LARA_LEAN_MAX_UW)
        item->pos.zRot = -LARA_LEAN_MAX_UW;
    else if (item->pos.zRot > LARA_LEAN_MAX_UW)
        item->pos.zRot = LARA_LEAN_MAX_UW;

    if (Lara.turnRate >= -LARA_TURN_UNDO && Lara.turnRate <= LARA_TURN_UNDO)
        Lara.turnRate = 0;
    else if (Lara.turnRate < -LARA_TURN_UNDO)
        Lara.turnRate += LARA_TURN_UNDO;
    else
        Lara.turnRate -= LARA_TURN_UNDO;
    item->pos.yRot += Lara.turnRate;

    if (Lara.currentActive && Lara.waterStatus != LWS_CHEAT)
        LaraWaterCurrent(coll);

    AnimateLara(item);
    item->pos.yPos -= (  SIN(item->pos.xRot) * item->fallspeed  >> (W2V_SHIFT + 2));
    item->pos.xPos += (((SIN(item->pos.yRot) * item->fallspeed) >> (W2V_SHIFT + 2)) * COS(item->pos.xRot)) >> W2V_SHIFT;
    item->pos.zPos += (((COS(item->pos.yRot) * item->fallspeed) >> (W2V_SHIFT + 2)) * COS(item->pos.xRot)) >> W2V_SHIFT;

    if (Lara.waterStatus != LWS_CHEAT)
        LaraBaddieCollision(item, coll);
    if (Lara.skidoo == NO_ITEM)
        lara_collision_routines[item->currentAnimState](item, coll);
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

    item = LaraItem;
    if (!Lara.currentActive)
    {
        absvel = abs(Lara.currentXvel);
        if (absvel > 16)
            shifter = 4;
        else if (absvel > 8)
            shifter = 3;
        else
            shifter = 2;
        Lara.currentXvel -= Lara.currentXvel >> shifter;
        if (absvel < 4)
            Lara.currentXvel = 0;

        absvel = abs(Lara.currentZvel);
        if (absvel > 16)
            shifter = 4;
        else if (absvel > 8)
            shifter = 3;
        else
            shifter = 2;
        Lara.currentZvel -= Lara.currentZvel >> shifter;
        if (absvel < 4)
            Lara.currentZvel = 0;

        if (Lara.currentXvel == 0 && Lara.currentZvel == 0)
            return;
    }
    else
    {
        sinkval = Lara.currentActive - 1;
        target.x = camera.fixed[sinkval].x;
        target.y = camera.fixed[sinkval].y;
        target.z = camera.fixed[sinkval].z;
        speed    = camera.fixed[sinkval].data;
        angle = ((mGetAngle(target.x, target.z, item->pos.xPos, item->pos.zPos) - 0x4000) >> 4) & 4095;
        dx = speed * rcossin_tbl[2 * angle] >> 2;
        dz = speed * rcossin_tbl[2 * angle + 1] >> 2;
        Lara.currentXvel += (short)(dx - Lara.currentXvel) >> 4;
        Lara.currentZvel += (short)(dz - Lara.currentZvel) >> 4;
        item->pos.yPos += (target.y - item->pos.yPos) >> 4;
    }

    item->pos.xPos += (Lara.currentXvel >> 8);
    item->pos.zPos += (Lara.currentZvel >> 8);
    Lara.currentActive = 0;

    coll->facing = phd_atan(item->pos.zPos - coll->old.z, item->pos.xPos - coll->old.x);
    GetCollisionInfo(coll, item->pos.xPos, item->pos.yPos + (UW_HITE / 2), item->pos.zPos, item->roomNumber, UW_HITE);

    switch (coll->collType)
    {
        case COLL_FRONT:
            if (item->pos.xRot > ANGLE(35))
                item->pos.xRot += UW_WALLDEFLECT;
            else if (item->pos.xRot < -ANGLE(35))
                item->pos.xRot -= UW_WALLDEFLECT;
            else
                item->fallspeed = 0;
            break;
        case COLL_TOP:
            item->pos.xRot -= UW_WALLDEFLECT;
            break;
        case COLL_TOPFRONT:
            item->fallspeed = 0;
            break;
        case COLL_LEFT:
            item->pos.yRot += ANGLE(5);
            break;
        case COLL_RIGHT:
            item->pos.yRot -= ANGLE(5);
            break;
    }

    if (coll->middle.floor < 0 && coll->middle.floor != -NO_HEIGHT)
        item->pos.yPos += coll->middle.floor;

    ShiftItem(item, coll);
    coll->old.x = item->pos.xPos;
    coll->old.y = item->pos.yPos;
    coll->old.z = item->pos.zPos;
}