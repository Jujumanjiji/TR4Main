#pragma once
/// ===========================================================///
///                           OTHER                            ///
/// ===========================================================///

#ifdef DLL_INJECT
#define InitialiseLaraLoad INITIALISE(0x00430EB0)
#define FlareItemControl CONTROL(0x0042FF90)
#define DrawFlareItem DRAW(0x0042F7B0)
#endif

#define LARA_FUNC(name) extern void name(ITEM_INFO* item, COLL_INFO* coll)

/// ===========================================================///
///                     LARA CONSTANTS                         ///
/// ===========================================================///

#define LaraClampP(var, incValue, maxValue)\
    var += incValue;\
    if (var > maxValue) var = maxValue

#define LaraClampN(var, incValue, maxValue)\
    var -= incValue;\
    if (var < -maxValue) var = -maxValue

#define LARA_AIR (2*30*30)
#define LARA_HITPOINTS 1000
#define LARA_DASH_TIME (30*4)
#define GRAVITY 6
#define SWAMP_GRAVITY 2
#define LARA_RAD 100
#define LARA_HITE 762
#define LARA_JUMPUP_HITE 870
#define LARA_SHOULDER_HITE 640
#define LARA_DUCK_FRONT STEP_L
#define LARA_DUCK_HEIGHT 400
#define LARA_HANG_HEIGHT 600
#define LARA_CRAWL_FRONT 300
#define LARA_CRAWL_BACK -300
#define UW_RADIUS 300
#define UW_HITE 400
#define CLIMB_WIDTH 120
#define CLIMB_WIDTHR 120
#define CLIMB_WIDTHL 120
#define DEATH_SPEED 100
#define DEATH_ACC 5
#define LARA_EXPOSURE_TIME ((30*10)*2)  // max exposure time (* precision)
#define LARA_WADE_EXPOSURE 1            // wade exposure rate
#define LARA_SWIM_EXPOSURE 2            // swim exposure rate
#define LARA_HEAT_EXPOSURE 1            // replenish exposure rate
#define DIVE_COUNT 10
#define CAM_A_HANG 0
#define CAM_E_HANG ANGLE(10)
#define HANG_ANGLE ANGLE(35)
#define MAX_HEAD_ROTATION ANGLE(44)		// Look Around Limits
#define MAX_HEAD_TILT ANGLE(30)
#define MIN_HEAD_TILT -ANGLE(35)
#define HEAD_TURN ANGLE(2)
#define LARA_DEF_ADD_EDGE ANGLE(5)
#define LARA_DUCK_DEFLECT ANGLE(2)
#define LARA_CRAWLB_RAD 250
#define VAULT_ANGLE	ANGLE(30)
#define FASTFALL_SPEED 128
#define LARA_FASTFALL_SPEED	(FASTFALL_SPEED+3)
#define FASTFALL_FRICTION(item) (item->speed * 95) / 100
#define DAMAGE_START  140
#define DAMAGE_LENGTH 14
#define SLOPE_DIF 60
#define BAD_JUMP_CEILING ((STEP_L*3)/4)
#define UW_FRICTION 8
#define UW_MAXSPEED (50*4)
#define WATER_FRICTION 6
#define SURF_FRICTION 4
#define SURFMOVE_FRICTION (SURF_FRICTION * 2)
#define SURF_MAXSPEED (15*4)
#define SURF_RADIUS 100
#define SURF_HITE 700
#define UW_WALLDEFLECT ANGLE(2)
#define NO_BAD_POS NO_HEIGHT
#define MAX_HEAD_ROTATION_SURF ANGLE(50)
#define MAX_HEAD_TILT_SURF ANGLE(40)
#define MIN_HEAD_TILT_SURF -ANGLE(40)
#define HEAD_TURN_SURF ANGLE(3)
#define CLIMB_SHIFT 70
#define CAM_E_CLIMBS -ANGLE(20)
#define CAM_A_CLIMBL -ANGLE(30)
#define CAM_E_CLIMBL -ANGLE(15)
#define CAM_A_CLIMBR ANGLE(30)
#define CAM_E_CLIMBR -ANGLE(15)
#define CAM_E_CLIMBU ANGLE(30)
#define CAM_E_CLIMBD -ANGLE(45)
#define CLIMB_HITE (STEP_L*2)
#define CLIMB_STANCE 700
#define CLIMB_HANG 900
#define LARA_IS_CLIMBING TRUE
#define LARA_IS_NOT_CLIMBING FALSE
#define HANG_ANGLE ANGLE(35)
#define OUTER 1
#define INNER -1
#define VAULT_CLICK(var, click, checkType, valueType) (var checkType (-(STEP_L * click) valueType (STEP_L / 2)))

/// later:
#define SLIDE_SLOPE3 ANGLE(11)
#define SLIDE_SLOPE4 ANGLE(15)
#define SLIDE_SLOPE3_INV ANGLE(15)
#define SLIDE_SLOPE4_INV ANGLE(20)
/// !later

#define LARA_LEAN_UNDO (ONE_DEGREE)                                 // Leaning around Corners ..
#define LARA_LEAN_RATE ((ONE_DEGREE/2) + LARA_LEAN_UNDO)
#define LARA_LEAN_MAX ((10*ONE_DEGREE) + LARA_LEAN_UNDO)
#define LARA_TURN_UNDO (2*ONE_DEGREE)                               // Large Turn UNDO limits LAG
#define LARA_TURN_RATE ((ONE_DEGREE/4) + LARA_TURN_UNDO)
#define LARA_JUMP_TURN ((ONE_DEGREE*1) + LARA_TURN_UNDO)
#define LARA_SLOW_TURN ((ONE_DEGREE*2) + LARA_TURN_UNDO)
#define LARA_MED_TURN ((ONE_DEGREE*4) + LARA_TURN_UNDO)
#define LARA_FAST_TURN ((ONE_DEGREE*6) + LARA_TURN_UNDO)
#define LARA_DASH_LEAN_RATE ((ONE_DEGREE/2) + LARA_LEAN_UNDO)
#define LARA_DASH_LEAN_MAX ((ONE_DEGREE*15) + LARA_LEAN_UNDO)
#define LARA_DASH_TURN_RATE ((ONE_DEGREE/4) + LARA_TURN_UNDO)
#define LARA_DASH_TURN_MAX ((ONE_DEGREE*2) + LARA_TURN_UNDO)
#define LARA_MONKEY_TURN_RATE ((ONE_DEGREE/4) + LARA_TURN_UNDO)
#define LARA_MONKEY_TURN_MAX ((ONE_DEGREE*2) + LARA_TURN_UNDO)
#define LARA_LEAN_MAX_UW (LARA_LEAN_MAX*2)

#define LARA_SPRINT_TO_ROLL_FRAME 2
#define LARA_STAND_TO_ROLL_FRAME 2
#define LARA_HANG_IDLE_FRAME 12
#define LARA_HANG_IDLE_FRAME_AFTERLEFTRIGHT 21

#define LARA_CLIMB_LNK1 29
#define LARA_CLIMB_LNK2 58

/// ==============================================================

#pragma region control
/// Lara Control Routines:
LARA_FUNC(lara_as_walk);
LARA_FUNC(lara_as_run);
LARA_FUNC(lara_as_stop);
LARA_FUNC(lara_as_forwardjump);
LARA_FUNC(lara_as_fastback);
LARA_FUNC(lara_as_turnr);
LARA_FUNC(lara_as_turnl);
LARA_FUNC(lara_as_death);
LARA_FUNC(lara_as_fastfall);
LARA_FUNC(lara_as_hang);
LARA_FUNC(lara_as_reach);
LARA_FUNC(lara_as_splat);
LARA_FUNC(lara_as_tread);
LARA_FUNC(lara_as_compress);
LARA_FUNC(lara_as_back);
LARA_FUNC(lara_as_swim);
LARA_FUNC(lara_as_glide);
LARA_FUNC(lara_as_null);
LARA_FUNC(lara_as_fastturn);
LARA_FUNC(lara_as_stepright);
LARA_FUNC(lara_as_stepleft);
LARA_FUNC(lara_as_slide);
LARA_FUNC(lara_as_backjump);
LARA_FUNC(lara_as_rightjump);
LARA_FUNC(lara_as_leftjump);
LARA_FUNC(lara_as_upjump);
LARA_FUNC(lara_as_fallback);
LARA_FUNC(lara_as_hangleft);
LARA_FUNC(lara_as_hangright);
LARA_FUNC(lara_as_slideback);
LARA_FUNC(lara_as_surftread);
LARA_FUNC(lara_as_surfswim);
LARA_FUNC(lara_as_dive);
LARA_FUNC(lara_as_pushblock);
LARA_FUNC(lara_as_pullblock);
LARA_FUNC(lara_as_ppready);
LARA_FUNC(lara_as_pickup);
LARA_FUNC(lara_as_switchon);
LARA_FUNC(lara_as_switchoff);
LARA_FUNC(lara_as_usekey);
LARA_FUNC(lara_as_usepuzzle);
LARA_FUNC(lara_as_uwdeath);
LARA_FUNC(lara_as_special);
LARA_FUNC(lara_as_surfback);
LARA_FUNC(lara_as_surfleft);
LARA_FUNC(lara_as_surfright);
LARA_FUNC(lara_as_swandive);
LARA_FUNC(lara_as_fastdive);
LARA_FUNC(lara_as_waterout);
LARA_FUNC(lara_as_climbstnc);
LARA_FUNC(lara_as_climbup);
LARA_FUNC(lara_as_climbleft);
LARA_FUNC(lara_as_climbend);
LARA_FUNC(lara_as_climbright);
LARA_FUNC(lara_as_climbdown);
LARA_FUNC(lara_as_wade);
LARA_FUNC(lara_as_waterroll);
LARA_FUNC(lara_as_pickupflare);
LARA_FUNC(lara_as_deathslide);
LARA_FUNC(lara_as_duck);
LARA_FUNC(lara_as_dash);
LARA_FUNC(lara_as_dashdive);
LARA_FUNC(lara_as_hang2);
LARA_FUNC(lara_as_monkeyswing);
LARA_FUNC(lara_as_monkeyl);
LARA_FUNC(lara_as_monkeyr);
LARA_FUNC(lara_as_monkey180);
LARA_FUNC(lara_as_all4s);
LARA_FUNC(lara_as_crawl);
LARA_FUNC(lara_as_hangturnl);
LARA_FUNC(lara_as_hangturnr);
LARA_FUNC(lara_as_all4turnl);
LARA_FUNC(lara_as_all4turnr);
LARA_FUNC(lara_as_crawlb);
LARA_FUNC(lara_as_controlled);
LARA_FUNC(lara_as_ropel);
LARA_FUNC(lara_as_roper);
LARA_FUNC(lara_as_controlledl);
LARA_FUNC(lara_as_poleleft);
LARA_FUNC(lara_as_poleright);
LARA_FUNC(lara_as_pulley);
LARA_FUNC(lara_as_duckl);
LARA_FUNC(lara_as_duckr);
LARA_FUNC(lara_as_extcornerl);
LARA_FUNC(lara_as_extcornerr);
LARA_FUNC(lara_as_intcornerl);
LARA_FUNC(lara_as_intcornerr);
LARA_FUNC(lara_as_rope);
LARA_FUNC(lara_as_climbrope);
LARA_FUNC(lara_as_climbroped);

#ifdef DLL_INJECT
#define legacy_lara_as_walk                    ADDRESS_STRUCT(0x00425AC0, lara_as_walk)
#define legacy_lara_as_run                     ADDRESS_STRUCT(0x00425B60, lara_as_run)
#define legacy_lara_as_stop                    ADDRESS_STRUCT(0x00424A60, lara_as_stop)
#define legacy_lara_as_forwardjump             ADDRESS_STRUCT(0x00425480, lara_as_forwardjump)
#define legacy_lara_as_fastback                ADDRESS_STRUCT(0x00425D10, lara_as_fastback)
#define legacy_lara_as_turnr                   ADDRESS_STRUCT(0x00425D70, lara_as_turnr)
#define legacy_lara_as_turnl                   ADDRESS_STRUCT(0x00425E20, lara_as_turnl)
#define legacy_lara_as_death                   ADDRESS_STRUCT(0x00425EC0, lara_as_death)
#define legacy_lara_as_fastfall                ADDRESS_STRUCT(0x00425F10, lara_as_fastfall)
#define legacy_lara_as_hang                    ADDRESS_STRUCT(0x00425F50, lara_as_hang)
#define legacy_lara_as_reach                   ADDRESS_STRUCT(0x00425630, lara_as_reach)
#define legacy_lara_as_splat                   ADDRESS_STRUCT(0x00426EB0, lara_as_splat)
#define legacy_lara_as_tread                   ADDRESS_STRUCT(0x004327A0, lara_as_tread)
#define legacy_lara_as_compress                ADDRESS_STRUCT(0x00426EC0, lara_as_compress)
#define legacy_lara_as_back                    ADDRESS_STRUCT(0x00426FD0, lara_as_back)
#define legacy_lara_as_swim                    ADDRESS_STRUCT(0x00432620, lara_as_swim)
#define legacy_lara_as_glide                   ADDRESS_STRUCT(0x00432720, lara_as_glide)
#define legacy_lara_as_null                    ADDRESS_STRUCT(0x00427060, lara_as_null)
#define legacy_lara_as_fastturn                ADDRESS_STRUCT(0x00427070, lara_as_fastturn)
#define legacy_lara_as_stepright               ADDRESS_STRUCT(0x004270C0, lara_as_stepright)
#define legacy_lara_as_stepleft                ADDRESS_STRUCT(0x00427150, lara_as_stepleft)
#define legacy_lara_as_slide                   ADDRESS_STRUCT(0x004271E0, lara_as_slide)
#define legacy_lara_as_backjump                ADDRESS_STRUCT(0x00427210, lara_as_backjump)
#define legacy_lara_as_rightjump               ADDRESS_STRUCT(0x00427260, lara_as_rightjump)
#define legacy_lara_as_leftjump                ADDRESS_STRUCT(0x004272A0, lara_as_leftjump)
#define legacy_lara_as_upjump                  ADDRESS_STRUCT(0x00425110, lara_as_upjump)
#define legacy_lara_as_fallback                ADDRESS_STRUCT(0x004272E0, lara_as_fallback)
#define legacy_lara_as_hangleft                ADDRESS_STRUCT(0x00426B50, lara_as_hangleft)
#define legacy_lara_as_hangright               ADDRESS_STRUCT(0x00426BD0, lara_as_hangright)
#define legacy_lara_as_slideback               ADDRESS_STRUCT(0x00427310, lara_as_slideback)
#define legacy_lara_as_surftread               ADDRESS_STRUCT(0x00431DA0, lara_as_surftread)
#define legacy_lara_as_surfswim                ADDRESS_STRUCT(0x00431C10, lara_as_surfswim)
#define legacy_lara_as_dive                    ADDRESS_STRUCT(0x00432830, lara_as_dive)
#define legacy_lara_as_pushblock               ADDRESS_STRUCT(0x00427330, lara_as_pushblock)
#define legacy_lara_as_pullblock               ADDRESS_STRUCT(0x00427370, lara_as_pullblock)
#define legacy_lara_as_ppready                 ADDRESS_STRUCT(0x004273B0, lara_as_ppready)
#define legacy_lara_as_pickup                  ADDRESS_STRUCT(0x004273E0, lara_as_pickup)
#define legacy_lara_as_switchon                ADDRESS_STRUCT(0x00427480, lara_as_switchon)
#define legacy_lara_as_usekey                  ADDRESS_STRUCT(0x004274C0, lara_as_usekey)
#define legacy_lara_as_usepuzzle               ADDRESS_STRUCT(0x00427500, lara_as_usepuzzle)
#define legacy_lara_as_uwdeath                 ADDRESS_STRUCT(0x00432850, lara_as_uwdeath)
#define legacy_lara_as_special                 ADDRESS_STRUCT(0x00427580, lara_as_special)
#define legacy_lara_as_surfback                ADDRESS_STRUCT(0x00431C80, lara_as_surfback)
#define legacy_lara_as_surfleft                ADDRESS_STRUCT(0x00431CE0, lara_as_surfleft)
#define legacy_lara_as_surfright               ADDRESS_STRUCT(0x00431D40, lara_as_surfright)
#define legacy_lara_as_swandive                ADDRESS_STRUCT(0x004275A0, lara_as_swandive)
#define legacy_lara_as_fastdive                ADDRESS_STRUCT(0x004275E0, lara_as_fastdive)
#define legacy_lara_as_waterout                ADDRESS_STRUCT(0x00427640, lara_as_waterout)
#define legacy_lara_as_climbstnc               ADDRESS_STRUCT(0x0042C6C0, lara_as_climbstnc)
#define legacy_lara_as_climbup                 ADDRESS_STRUCT(0x0042D470, lara_as_climbup)
#define legacy_lara_as_climbleft               ADDRESS_STRUCT(0x0042CC80, lara_as_climbleft)
#define legacy_lara_as_climbend                ADDRESS_STRUCT(0x0042D7A0, lara_as_climbend)
#define legacy_lara_as_climbright              ADDRESS_STRUCT(0x0042D3D0, lara_as_climbright)
#define legacy_lara_as_climbdown               ADDRESS_STRUCT(0x0042D5E0, lara_as_climbdown)
#define legacy_lara_as_wade                    ADDRESS_STRUCT(0x00427660, lara_as_wade)
#define legacy_lara_as_waterroll               ADDRESS_STRUCT(0x004328B0, lara_as_waterroll)
#define legacy_lara_as_pickupflare             ADDRESS_STRUCT(0x00427420, lara_as_pickupflare)
#define legacy_lara_as_deathslide              ADDRESS_STRUCT(0x00427730, lara_as_deathslide)
#define legacy_lara_as_duck                    ADDRESS_STRUCT(0x00421040, lara_as_duck)
#define legacy_lara_as_dash                    ADDRESS_STRUCT(0x004220B0, lara_as_dash)
#define legacy_lara_as_dashdive                ADDRESS_STRUCT(0x00422A90, lara_as_dashdive)
#define legacy_lara_as_hang2                   ADDRESS_STRUCT(0x00422DE0, lara_as_hang2)
#define legacy_lara_as_monkeyswing             ADDRESS_STRUCT(0x00423880, lara_as_monkeyswing)
#define legacy_lara_as_monkeyl                 ADDRESS_STRUCT(0x00423A60, lara_as_monkeyl)
#define legacy_lara_as_monkeyr                 ADDRESS_STRUCT(0x00423B30, lara_as_monkeyr)
#define legacy_lara_as_monkey180               ADDRESS_STRUCT(0x00423D20, lara_as_monkey180)
#define legacy_lara_as_all4s                   ADDRESS_STRUCT(0x00421210, lara_as_all4s)
#define legacy_lara_as_crawl                   ADDRESS_STRUCT(0x004216A0, lara_as_crawl)
#define legacy_lara_as_hangturnl               ADDRESS_STRUCT(0x00423C00, lara_as_hangturnl)
#define legacy_lara_as_hangturnr               ADDRESS_STRUCT(0x00423CE0, lara_as_hangturnr)
#define legacy_lara_as_all4turnl               ADDRESS_STRUCT(0x00421900, lara_as_all4turnl)
#define legacy_lara_as_all4turnr               ADDRESS_STRUCT(0x004219B0, lara_as_all4turnr)
#define legacy_lara_as_crawlb                  ADDRESS_STRUCT(0x00421A10, lara_as_crawlb)
#define legacy_lara_as_controlled              ADDRESS_STRUCT(0x004277D0, lara_as_controlled)
#define legacy_lara_as_ropel                   ADDRESS_STRUCT(0x00424620, lara_as_ropel)
#define legacy_lara_as_roper                   ADDRESS_STRUCT(0x00424650, lara_as_roper)
#define legacy_lara_as_controlledl             ADDRESS_STRUCT(0x004278B0, lara_as_controlledl)
#define legacy_lara_as_poleleft                ADDRESS_STRUCT(0x00423EE0, lara_as_poleleft)
#define legacy_lara_as_poleright               ADDRESS_STRUCT(0x00423F20, lara_as_poleright)
#define legacy_lara_as_pulley                  ADDRESS_STRUCT(0x00427940, lara_as_pulley)
#define legacy_lara_as_duckl                   ADDRESS_STRUCT(0x00420D50, lara_as_duckl)
#define legacy_lara_as_duckr                   ADDRESS_STRUCT(0x00420D80, lara_as_duckr)
#define legacy_lara_as_extcornerl              ADDRESS_STRUCT(0x00426D70, lara_as_extcornerl)
#define legacy_lara_as_extcornerr              ADDRESS_STRUCT(0x00426DC0, lara_as_extcornerr)
#define legacy_lara_as_intcornerl              ADDRESS_STRUCT(0x00426E10, lara_as_intcornerl)
#define legacy_lara_as_intcornerr              ADDRESS_STRUCT(0x00426E60, lara_as_intcornerr)
#define legacy_lara_as_rope                    ADDRESS_STRUCT(0x004241A0, lara_as_rope)
#define legacy_lara_as_climbrope               ADDRESS_STRUCT(0x004248E0, lara_as_climbrope)
#define legacy_lara_as_climbroped              ADDRESS_STRUCT(0x00424950, lara_as_climbroped)
#endif
#pragma endregion

#pragma region collision
/// Lara Collision Routines:
LARA_FUNC(lara_default_col);
LARA_FUNC(lara_col_walk);
LARA_FUNC(lara_col_run);
LARA_FUNC(lara_col_stop);
LARA_FUNC(lara_col_forwardjump);
LARA_FUNC(lara_col_fastback);
LARA_FUNC(lara_col_turn);
LARA_FUNC(lara_col_death);
LARA_FUNC(lara_col_fastfall);
LARA_FUNC(lara_col_hang);
LARA_FUNC(lara_col_reach);
LARA_FUNC(lara_col_splat);
LARA_FUNC(lara_col_tread);
LARA_FUNC(lara_col_compress);
LARA_FUNC(lara_col_back);
LARA_FUNC(lara_col_swim);
LARA_FUNC(lara_col_glide);
LARA_FUNC(lara_col_step);
LARA_FUNC(lara_col_roll);
LARA_FUNC(lara_col_slide);
LARA_FUNC(lara_col_backjump);
LARA_FUNC(lara_col_rightjump);
LARA_FUNC(lara_col_leftjump);
LARA_FUNC(lara_col_upjump);
LARA_FUNC(lara_col_fallback);
LARA_FUNC(lara_col_hangleft);
LARA_FUNC(lara_col_hangright);
LARA_FUNC(lara_col_slideback);
LARA_FUNC(lara_col_surftread);
LARA_FUNC(lara_col_surfswim);
LARA_FUNC(lara_col_dive);
LARA_FUNC(lara_col_uwdeath);
LARA_FUNC(lara_col_roll2);
LARA_FUNC(lara_col_surfback);
LARA_FUNC(lara_col_surfleft);
LARA_FUNC(lara_col_surfright);
LARA_FUNC(lara_col_swandive);
LARA_FUNC(lara_col_fastdive);
LARA_FUNC(lara_col_climbstnc);
LARA_FUNC(lara_col_climbup);
LARA_FUNC(lara_col_climbleft);
LARA_FUNC(lara_col_climbright);
LARA_FUNC(lara_col_climbdown);
LARA_FUNC(lara_col_wade);
LARA_FUNC(lara_col_waterroll);
LARA_FUNC(lara_col_duck);
LARA_FUNC(lara_col_dash);
LARA_FUNC(lara_col_dashdive);
LARA_FUNC(lara_col_hang2);
LARA_FUNC(lara_col_monkeyswing);
LARA_FUNC(lara_col_monkeyl);
LARA_FUNC(lara_col_monkeyr);
LARA_FUNC(lara_col_monkey180);
LARA_FUNC(lara_col_all4s);
LARA_FUNC(lara_col_crawl);
LARA_FUNC(lara_col_hangturn);
LARA_FUNC(lara_col_all4turn);
LARA_FUNC(lara_col_crawlb);
LARA_FUNC(lara_col_crawl2hang);
LARA_FUNC(lara_col_turnswitch);
LARA_FUNC(lara_col_polestat);
LARA_FUNC(lara_col_poleup);
LARA_FUNC(lara_col_poledown);
LARA_FUNC(lara_col_duck2);
LARA_FUNC(lara_col_rope);
LARA_FUNC(lara_col_ropefwd);

#ifdef DLL_INJECT
#define legacy_lara_default_col                ADDRESS_STRUCT(0x00428AD0, lara_default_col)
#define legacy_lara_col_walk                   ADDRESS_STRUCT(0x00427A40, lara_col_walk)
#define legacy_lara_col_run                    ADDRESS_STRUCT(0x00427C30, lara_col_run)
#define legacy_lara_col_stop                   ADDRESS_STRUCT(0x00425070, lara_col_stop)
#define legacy_lara_col_forwardjump            ADDRESS_STRUCT(0x00425550, lara_col_forwardjump)
#define legacy_lara_col_fastback               ADDRESS_STRUCT(0x00427E00, lara_col_fastback)
#define legacy_lara_col_turn                   ADDRESS_STRUCT(0x00427EF0, lara_col_turn)
#define legacy_lara_col_death                  ADDRESS_STRUCT(0x00427FD0, lara_col_death)
#define legacy_lara_col_fastfall               ADDRESS_STRUCT(0x00428040, lara_col_fastfall)
#define legacy_lara_col_hang                   ADDRESS_STRUCT(0x00425FA0, lara_col_hang)
#define legacy_lara_col_reach                  ADDRESS_STRUCT(0x00425650, lara_col_reach)
#define legacy_lara_col_splat                  ADDRESS_STRUCT(0x004280F0, lara_col_splat)
#define legacy_lara_col_tread                  ADDRESS_STRUCT(0x00432B30, lara_col_tread)
#define legacy_lara_col_compress               ADDRESS_STRUCT(0x00428160, lara_col_compress)
#define legacy_lara_col_back                   ADDRESS_STRUCT(0x00428210, lara_col_back)
#define legacy_lara_col_swim                   ADDRESS_STRUCT(0x004328C0, lara_col_swim)
#define legacy_lara_col_glide                  ADDRESS_STRUCT(0x00432B10, lara_col_glide)
#define legacy_lara_col_step                   ADDRESS_STRUCT(0x00428350, lara_col_step)
#define legacy_lara_col_roll                   ADDRESS_STRUCT(0x004286D0, lara_col_roll)
#define legacy_lara_col_slide                  ADDRESS_STRUCT(0x00428450, lara_col_slide)
#define legacy_lara_col_backjump               ADDRESS_STRUCT(0x00428580, lara_col_backjump)
#define legacy_lara_col_rightjump              ADDRESS_STRUCT(0x004285B0, lara_col_rightjump)
#define legacy_lara_col_leftjump               ADDRESS_STRUCT(0x004285E0, lara_col_leftjump)
#define legacy_lara_col_upjump                 ADDRESS_STRUCT(0x00425130, lara_col_upjump)
#define legacy_lara_col_fallback               ADDRESS_STRUCT(0x00428610, lara_col_fallback)
#define legacy_lara_col_hangleft               ADDRESS_STRUCT(0x00426B90, lara_col_hangleft)
#define legacy_lara_col_hangright              ADDRESS_STRUCT(0x00426C10, lara_col_hangright)
#define legacy_lara_col_slideback              ADDRESS_STRUCT(0x004286A0, lara_col_slideback)
#define legacy_lara_col_surftread              ADDRESS_STRUCT(0x00432340, lara_col_surftread)
#define legacy_lara_col_surfswim               ADDRESS_STRUCT(0x00431E60, lara_col_surfswim)
#define legacy_lara_col_dive                   ADDRESS_STRUCT(0x00432B50, lara_col_dive)
#define legacy_lara_col_uwdeath                ADDRESS_STRUCT(0x00432B70, lara_col_uwdeath)
#define legacy_lara_col_roll2                  ADDRESS_STRUCT(0x00428770, lara_col_roll2)
#define legacy_lara_col_surfback               ADDRESS_STRUCT(0x004322B0, lara_col_surfback)
#define legacy_lara_col_surfleft               ADDRESS_STRUCT(0x004322E0, lara_col_surfleft)
#define legacy_lara_col_surfright              ADDRESS_STRUCT(0x00432310, lara_col_surfright)
#define legacy_lara_col_swandive               ADDRESS_STRUCT(0x00428850, lara_col_swandive)
#define legacy_lara_col_fastdive               ADDRESS_STRUCT(0x004288D0, lara_col_fastdive)
#define legacy_lara_col_climbstnc              ADDRESS_STRUCT(0x0042C770, lara_col_climbstnc)
#define legacy_lara_col_climbup                ADDRESS_STRUCT(0x0042D490, lara_col_climbup)
#define legacy_lara_col_climbleft              ADDRESS_STRUCT(0x0042CCC0, lara_col_climbleft)
#define legacy_lara_col_climbright             ADDRESS_STRUCT(0x0042D410, lara_col_climbright)
#define legacy_lara_col_climbdown              ADDRESS_STRUCT(0x0042D600, lara_col_climbdown)
#define legacy_lara_col_wade                   ADDRESS_STRUCT(0x00428960, lara_col_wade)
#define legacy_lara_col_waterroll              ADDRESS_STRUCT(0x00432BD0, lara_col_waterroll)
#define legacy_lara_col_duck                   ADDRESS_STRUCT(0x004210F0, lara_col_duck)
#define legacy_lara_col_dash                   ADDRESS_STRUCT(0x00422200, lara_col_dash)
#define legacy_lara_col_dashdive               ADDRESS_STRUCT(0x00422AC0, lara_col_dashdive)
#define legacy_lara_col_hang2                  ADDRESS_STRUCT(0x00422EB0, lara_col_hang2)
#define legacy_lara_col_monkeyswing            ADDRESS_STRUCT(0x00423930, lara_col_monkeyswing)
#define legacy_lara_col_monkeyl                ADDRESS_STRUCT(0x00423AC0, lara_col_monkeyl)
#define legacy_lara_col_monkeyr                ADDRESS_STRUCT(0x00423B90, lara_col_monkeyr)
#define legacy_lara_col_monkey180              ADDRESS_STRUCT(0x00423D40, lara_col_monkey180)
#define legacy_lara_col_all4s                  ADDRESS_STRUCT(0x00421280, lara_col_all4s)
#define legacy_lara_col_crawl                  ADDRESS_STRUCT(0x00421770, lara_col_crawl)
#define legacy_lara_col_hangturn               ADDRESS_STRUCT(0x00423C40, lara_col_hangturn)
#define legacy_lara_col_all4turn               ADDRESS_STRUCT(0x00421960, lara_col_all4turn)
#define legacy_lara_col_crawlb                 ADDRESS_STRUCT(0x00421AD0, lara_col_crawlb)
#define legacy_lara_col_crawl2hang             ADDRESS_STRUCT(0x00421BF0, lara_col_crawl2hang)
#define legacy_lara_col_turnswitch             ADDRESS_STRUCT(0x004278D0, lara_col_turnswitch)
#define legacy_lara_col_polestat               ADDRESS_STRUCT(0x00423D60, lara_col_polestat)
#define legacy_lara_col_poleup                 ADDRESS_STRUCT(0x00423F60, lara_col_poleup)
#define legacy_lara_col_poledown               ADDRESS_STRUCT(0x00423FF0, lara_col_poledown)
#define legacy_lara_col_duck2                  ADDRESS_STRUCT(0x00420DB0, lara_col_duck2)
#define legacy_lara_col_rope                   ADDRESS_STRUCT(0x00424270, lara_col_rope)
#define legacy_lara_col_ropefwd                ADDRESS_STRUCT(0x00424680, lara_col_ropefwd)
#endif
#pragma endregion

#pragma region lara1gun
enum LARA_SHOTGUN_STATE
{
    LSS_LAND_AIM,
    LSS_LAND_DRAW,
    LSS_LAND_RECOIL,
    LSS_LAND_UNDRAW,
    LSS_LAND_UNAIM,
    LSS_LAND_RELOAD,
    LSS_WATER_AIM,
    LSS_WATER_UNAIM,
    LSS_WATER_FIRE,
    LSS_WATER_UNDRAW,
    LSS_WATER_WADE_UNDRAW,
};

extern void draw_shotgun_meshes(int weapon_type);
extern void undraw_shotgun_meshes(int weapon_type);
extern void ready_shotgun(int weapon_type);
extern void draw_shotgun(int weapon_type);
extern void undraw_shotgun(int weapon_type);
extern void shotgun_handler(int weapon_type);
extern void animate_shotgun(int weapon_type);

#ifdef DLL_INJECT
#define legacy_draw_shotgun_meshes        ADDRESS_STRUCT(0x00428E40, draw_shotgun_meshes)
#define legacy_undraw_shotgun_meshes      ADDRESS_STRUCT(0x00428E70, undraw_shotgun_meshes)
#define legacy_ready_shotgun              ADDRESS_STRUCT(0x00428EA0, ready_shotgun)
#define legacy_draw_shotgun               ADDRESS_STRUCT(0x0042AE50, draw_shotgun)
#define legacy_undraw_shotgun             ADDRESS_STRUCT(0x0042AFE0, undraw_shotgun)
#define legacy_shotgun_handler            ADDRESS_STRUCT(0x00428F10, shotgun_handler)
#define legacy_animate_shotgun            ADDRESS_STRUCT(0x0042B100, animate_shotgun)
#endif

#pragma endregion

#pragma region lara2gun
extern int weapon_holsters(int weapon_type);
extern void set_arm_info(LARA_ARM* arm, short frame);
extern void draw_pistol_meshes(int weapon_type);
extern void undraw_pistols_left(int weapon_type);
extern void undraw_pistols_right(int weapon_type);
extern void draw_pistols(int weapon_type);
extern void undraw_pistols(int weapon_type);
extern void ready_pistols(int weapon_type);
extern void pistol_handler(int weapon_type);
extern void animate_pistols(int weapon_type);

#ifdef DLL_INJECT
#define legacy_set_arm_info             ADDRESS_STRUCT(0x0042B7C0, set_arm_info)
#define legacy_draw_pistol_meshes       ADDRESS_STRUCT(0x0042BB20, draw_pistol_meshes)
#define legacy_undraw_pistols_left      ADDRESS_STRUCT(0x0042BB70, undraw_pistols_left)
#define legacy_undraw_pistols_right     ADDRESS_STRUCT(0x0042BBB0, undraw_pistols_right)
#define legacy_draw_pistols             ADDRESS_STRUCT(0x0042B720, draw_pistols)
#define legacy_undraw_pistols           ADDRESS_STRUCT(0x0042B840, undraw_pistols)
#define legacy_ready_pistols            ADDRESS_STRUCT(0x0042BAB0, ready_pistols)
#define legacy_pistol_handler           ADDRESS_STRUCT(0x0042BBF0, pistol_handler)
#define legacy_animate_pistols          ADDRESS_STRUCT(0x0042BE40, animate_pistols)
#endif
#pragma endregion

#pragma region larafire

#ifdef DLL_INJECT
#define weapon_object ((int(__cdecl*)(int weapon_type)) 0x0042EA70)
#define weapon_meshes ((int(__cdecl*)(int weapon_type)) 0x0042EAC0)
#define fire_shotgun ((void(__cdecl*)(void)) 0x00429260)
#define fire_grenade ((void(__cdecl*)(void)) 0x00429480)
#define fire_crossbow ((void(__cdecl*)(PHD_3DPOS* haveDefinedPos)) 0x0042A270)
#define InitialiseNewWeapons ((void(__cdecl*)(void)) 0x0042DDF0)
#define LaraTargetInfo ((void(__cdecl*)(WEAPON_INFO* winfo)) 0x0042DF30)
#define LaraGetNewTarget ((void(__cdecl*)(WEAPON_INFO* winfo)) 0x0042E0D0)
#define AimWeapon ((void(__cdecl*)(WEAPON_INFO* winfo, LARA_ARM* arm)) 0x0042E560)
#define FireWeapon ((BOOL(__cdecl*)(int weapon_type, ITEM_INFO* target, ITEM_INFO* laraitem, short angles[2])) 0x0042E630)
#define LaraGun ((void(__cdecl*)(void)) 0x0042D840)
#endif
#pragma endregion

#pragma region lara_utils
extern short LaraFloorFront(ITEM_INFO* item, short angle, int distance);
extern short LaraCeilingFront(ITEM_INFO* item, short angle, int distance, int height);
extern BOOL LaraHitCeiling(ITEM_INFO* item, COLL_INFO* coll);
extern BOOL LaraHangTest(ITEM_INFO* item, COLL_INFO* coll);
extern BOOL LaraDeflectEdge(ITEM_INFO* item, COLL_INFO* coll);
extern BOOL LaraDeflectEdgeDuck(ITEM_INFO* item, COLL_INFO* coll);
extern void LaraDeflectEdgeJump(ITEM_INFO* item, COLL_INFO* coll);
extern BOOL LaraFallen(ITEM_INFO* item, COLL_INFO* coll);
extern int IsValidHangPos(ITEM_INFO* item, COLL_INFO* coll);
extern int LaraHangRightCornerTest(ITEM_INFO* item, COLL_INFO* coll);
extern int LaraHangLeftCornerTest(ITEM_INFO* item, COLL_INFO* coll);
extern int LaraClimbLeftCornerTest(ITEM_INFO* item, COLL_INFO* coll);
extern int LaraClimbRightCornerTest(ITEM_INFO* item, COLL_INFO* coll);
extern void LaraSlideEdgeJump(ITEM_INFO* item, COLL_INFO* coll);
extern BOOL LaraTestClimbStance(ITEM_INFO* item, COLL_INFO* coll);
extern int LaraTestEdgeCatch(ITEM_INFO* item, COLL_INFO* coll, int* edge);
extern BOOL LaraTestHangOnClimbWall(ITEM_INFO* item, COLL_INFO* coll);
extern void LaraSlideSlope(ITEM_INFO* item, COLL_INFO* coll);
extern void LaraCollideJump(ITEM_INFO* item, COLL_INFO* coll);
extern BOOL LaraCheckForLetGo(ITEM_INFO* item, COLL_INFO* coll);
extern int LaraTestClimbUpPos(ITEM_INFO* item, int front, int right, int* shift, int* ledge);
extern int LaraTestClimbPos(ITEM_INFO* item, int front, int right, int origin, int height, int* shift);
extern int LaraTestClimb(int x, int y, int z, int xfront, int zfront, int item_height, short item_room, int* shift);
extern void LaraDoClimbLeftRight(ITEM_INFO* item, COLL_INFO* coll, int result, int shift);
extern BOOL LaraTestWaterClimbOut(ITEM_INFO* item, COLL_INFO* coll);
extern BOOL LaraTestWaterStepOut(ITEM_INFO* item, COLL_INFO* coll);
extern void LaraTestWaterDepth(ITEM_INFO* item, COLL_INFO* coll);
extern bool LaraLandedBad(ITEM_INFO* item);
extern void LaraCollideStop(ITEM_INFO* item, COLL_INFO* coll);
extern void LaraSwimCollision(ITEM_INFO* item, COLL_INFO* coll);
extern void LaraSurfaceCollision(ITEM_INFO* item, COLL_INFO* coll);
///extern BOOL UseSpecialItem(ITEM_INFO* item);
extern BOOL TestLaraSlide(ITEM_INFO* item, COLL_INFO* coll);
extern BOOL TestLaraVault(ITEM_INFO* item, COLL_INFO* coll);
extern BOOL TestMonkeyLeft(ITEM_INFO* item, COLL_INFO* coll);
extern BOOL TestMonkeyRight(ITEM_INFO* item, COLL_INFO* coll);
extern BOOL TestHangSwingIn(ITEM_INFO* item, short angle);
extern BOOL TestWall(ITEM_INFO* item, int front, int right, int height);
extern void SwimTurn(ITEM_INFO* item);
extern void MonkeySwingFall(ITEM_INFO* item);
extern void MonkeySwingSnap(ITEM_INFO* item, COLL_INFO* coll);
extern void FallFromRope(ITEM_INFO* item);
extern void UpdateRopeSwing(ITEM_INFO* item);
extern void ApplyVelocityToRope(int node, short angle, short n);
extern void SetPendulumVelocity(int x, int y, int z);
extern void JumpOffRope(ITEM_INFO* item);
extern void SetCornerAnim(ITEM_INFO* item, COLL_INFO* coll, short angle, short flip);
extern BOOL CanLaraHangSideways(ITEM_INFO* item, COLL_INFO* coll, short angle);
extern short GetDirOctant(int angle);
extern short GetClimbTrigger(int x, int y, int z, short roomNumber);
extern void SnapLaraToEdgeOfBlock(ITEM_INFO* item, COLL_INFO* coll, short angle);
extern void GetLaraCollisionInfo(ITEM_INFO* item, COLL_INFO* coll);
extern void AnimateLara(ITEM_INFO* item);

extern BOOL LaraTestHangJump(ITEM_INFO* item, COLL_INFO* coll);
extern BOOL LaraTestHangJumpUp(ITEM_INFO* item, COLL_INFO* coll);

#ifdef DLL_INJECT
///#define legacy_LaraFloorFront ((short(__cdecl*)(ITEM_INFO* item, short angle, int distance)) 0x00421620)
///#define legacy_LaraCeilingFront ((short(__cdecl*)(ITEM_INFO* item, short angle, int distance, int height)) 0x00420A80)
///#define legacy_LaraHitCeiling ((BOOL(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll)) 0x00422390)
///#define LaraHangTest ((BOOL(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll)) 0x004230E0)
///#define legacy_LaraDeflectEdge ((BOOL(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll)) 0x00422400)
///#define legacy_LaraDeflectEdgeDuck ((BOOL(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll)) 0x00421880)
///#define legacy_LaraDeflectEdgeJump ((void(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll)) 0x00422C50)
///#define legacy_LaraFallen ((BOOL(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll)) 0x00420FE0)
///#define legacy_IsValidHangPos ((int(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll)) 0x00426600)
///#define legacy_LaraHangRightCornerTest ((int(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll)) 0x00426230)
///#define legacy_LaraHangLeftCornerTest ((int(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll)) 0x004266E0)
///#define legacy_LaraClimbLeftCornerTest ((BOOL(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll)) 0x0042CEE0)
///#define legacy_LaraClimbRightCornerTest ((BOOL(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll)) 0x0042D160)
///#define legacy_LaraSlideEdgeJump ((void(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll)) 0x004259C0)
///#define legacy_LaraTestClimbStance ((BOOL(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll)) 0x00422810)
///#define legacy_LaraTestEdgeCatch ((int(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll, int* edge)) 0x00421DE0)
///#define legacy_LaraTestHangOnClimbWall ((BOOL(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll)) 0x00421E90)
///#define legacy_LaraSlideSlope ((void(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll)) 0x00428470)
///#define legacy_LaraCollideJump ((void(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll)) 0x00428B20)
///#define legacy_LaraCheckForLetGo ((BOOL(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll)) 0x0042C980)
///#define legacy_LaraTestClimbUpPos ((int(__cdecl*)(ITEM_INFO* item, int front, int right, int* shift, int* ledge)) 0x0042CA60)
///#define legacy_LaraTestClimbPos ((int(__cdecl*)(ITEM_INFO* item, int front, int right, int origin, int height, int* shift)) 0x0042C3C0)
///#define legacy_LaraTestClimb ((int(__cdecl*)(int x, int y, int z, int xfront, int zfront, int item_height, short item_room, int* shift)) 0x0042C470)
///#define legacy_LaraDoClimbLeftRight ((void(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll, int result, int shift)) 0x0042CD20)
///#define legacy_LaraTestWaterClimbOut ((BOOL(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll)) 0x00431E90)
///#define legacy_LaraTestWaterStepOut ((BOOL(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll)) 0x004321B0)
///#define legacy_LaraTestWaterDepth ((void(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll)) 0x00432A30)
///#define legacy_LaraLandedBad ((bool(__cdecl*)(ITEM_INFO* item)) 0x00422BE0)
///#define legacy_LaraCollideStop ((void(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll)) 0x004229D0)
///#define legacy_LaraSwimCollision ((void(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll)) 0x004328E0)
///#define legacy_LaraSurfaceCollision ((void(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll)) 0x004320B0)
#define UseSpecialItem ((BOOL(__cdecl*)(ITEM_INFO* item)) 0x00424E90)
///#define legacy_TestLaraSlide ((BOOL(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll)) 0x00420EA0)
///#define legacy_TestLaraVault ((BOOL(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll)) 0x00422480)
///#define legacy_TestMonkeyLeft ((BOOL(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll)) 0x004236B0)
///#define legacy_TestMonkeyRight ((BOOL(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll)) 0x004237B0)
///#define legacy_TestHangSwingIn ((BOOL(__cdecl*)(ITEM_INFO* item, short angle)) 0x00421FF0)
///#define legacy_TestWall ((BOOL(__cdecl*)(ITEM_INFO* item, int front, int right, int height)) 0x004228D0)
///#define legacy_SwimTurn ((void(__cdecl*)(ITEM_INFO* item)) 0x00432690)
///#define legacy_MonkeySwingFall ((void(__cdecl*)(ITEM_INFO* item)) 0x00422E50)
///#define legacy_MonkeySwingSnap ((void(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll)) 0x00423660)
///#define legacy_FallFromRope ((void(__cdecl*)(ITEM_INFO* item)) 0x004241D0)
///#define legacy_UpdateRopeSwing ((void(__cdecl*)(ITEM_INFO* item)) 0x00424320)
///#define legacy_ApplyVelocityToRope ((void(__cdecl*)(int node, short angle, short n)) 0x00424150)
///#define legacy_SetPendulumVelocity ((void(__cdecl*)(int x, int y, int z)) 0x004597D0)
///#define legacy_JumpOffRope ((void(__cdecl*)(ITEM_INFO* item)) 0x00424820)
///#define legacy_SetCornerAnim ((void(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll, short angle, short flip)) 0x00426C50)
///#define legacy_CanLaraHangSideways ((BOOL(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll, short angle)) 0x00426AB0)
///#define legacy_GetDirOctant ((short(__cdecl*)(int angle)) 0x00423780)
///#define legacy_GetClimbTrigger ((short(__cdecl*)(int x, int y, int z, short roomNumber)) 0x0042D7D0)
///#define legacy_SnapLaraToEdgeOfBlock ((void(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll, short angle)) 0x004235B0)
///#define legacy_GetLaraCollisionInfo ((void(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll)) 0x00422350)
///#define legacy_AnimateLara ((void(__cdecl*)(ITEM_INFO* item)) 0x00430B60)
#endif
#pragma endregion

#pragma region lara_control
extern void LaraAboveWater(ITEM_INFO* item, COLL_INFO* coll);
extern void LaraSurface(ITEM_INFO* item, COLL_INFO* coll);
extern void LaraUnderWater(ITEM_INFO* item, COLL_INFO* coll);
extern void LaraWaterCurrent(COLL_INFO* coll);

#ifdef DLL_INJECT
#define legacy_LaraAboveWater   ((void(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll)) 0x00420B10)
#define legacy_LaraSurface      ((void(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll)) 0x00431AA0)
#define legacy_LaraUnderWater   ((void(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll)) 0x004323A0)
#define legacy_LaraWaterCurrent ((void(__cdecl*)(COLL_INFO* coll)) 0x00432DA0)
#endif
#pragma endregion