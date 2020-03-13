#pragma once
/// ===========================================================///
///                           OTHER                            ///
/// ===========================================================///

#ifdef DLL_INJECT
#define InitialiseLaraLoad INITIALISE(0x00430EB0)
#define FlareItemControl CONTROL(0x0042FF90)
#define DrawFlareItem DRAW(0x0042F7B0)
#endif

/// ===========================================================///
///                     LARA CONSTANTS                         ///
/// ===========================================================///

#define LaraClampP(var, incValue, maxValue)\
    var += incValue;\
    if (var > maxValue) var = maxValue

#define LaraClampN(var, incValue, maxValue)\
    var -= incValue;\
    if (var < -maxValue) var = -maxValue


#define GUN_HAND_RIGHT 11
#define GUN_HAND_LEFT 13
#define INFINITE_AMMO -1

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
#define LARA_EXPOSURE_TIME ((30*10)*2) // max exposure time (* precision)
#define LARA_WADE_EXPOSURE 1   // wade exposure rate
#define LARA_SWIM_EXPOSURE 2   // swim exposure rate
#define LARA_HEAT_EXPOSURE 1   // replenish exposure rate
#define DIVE_COUNT 10
#define CAM_A_HANG 0
#define CAM_E_HANG ANGLE(10)
#define HANG_ANGLE ANGLE(35)
#define MAX_HEAD_ROTATION ANGLE(44) // Look Around Limits
#define MAX_HEAD_TILT ANGLE(30)
#define MIN_HEAD_TILT -ANGLE(35)
#define HEAD_TURN ANGLE(2)
#define LARA_DEF_ADD_EDGE ANGLE(5)
#define LARA_DUCK_DEFLECT ANGLE(2)
#define LARA_CRAWLB_RAD 250
#define VAULT_ANGLE ANGLE(30)
#define FASTFALL_SPEED 128
#define LARA_FASTFALL_SPEED (FASTFALL_SPEED+3)
#define FASTFALL_FRICTION(item) (item->speed * 95) / 100
#define DAMAGE_START 140
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

#define LARA_LEAN_UNDO (ONE_DEGREE)         // Leaning around Corners ..
#define LARA_LEAN_RATE ((ONE_DEGREE/2) + LARA_LEAN_UNDO)
#define LARA_LEAN_MAX ((10*ONE_DEGREE) + LARA_LEAN_UNDO)
#define LARA_TURN_UNDO (2*ONE_DEGREE)        // Large Turn UNDO limits LAG
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

#define LARA_FUNC(name) extern void name(ITEM_INFO* item, COLL_INFO* coll)

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
#pragma endregion

#pragma region lara1gun
enum WEAPON_ANIM
{
    W_AIM,
    W_DRAW,
    W_RECOIL,
    W_UNDRAW,
    W_UNAIM,
    W_RELOAD,
    W_UAIM,
    W_UUNAIM,
    W_URECOIL,
    W_SURF_UNDRAW
};

extern void draw_shotgun_meshes(int weapon_type);
extern void undraw_shotgun_meshes(int weapon_type);
extern void ready_shotgun(int weapon_type);
extern void draw_shotgun(int weapon_type);
extern void undraw_shotgun(int weapon_type);
extern void shotgun_handler(int weapon_type);
extern void animate_shotgun(int weapon_type);
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
#pragma endregion

#pragma region larafire
#define PISTOL_LOCK_YMIN -ANGLE(60)
#define PISTOL_LOCK_YMAX +ANGLE(60)
#define PISTOL_LOCK_XMIN -ANGLE(60)
#define PISTOL_LOCK_XMAX +ANGLE(60)
#define PISTOL_LARM_YMIN -ANGLE(170)
#define PISTOL_LARM_YMAX +ANGLE(60)
#define PISTOL_LARM_XMIN -ANGLE(80)
#define PISTOL_LARM_XMAX +ANGLE(80)
#define PISTOL_RARM_YMIN -ANGLE(60)
#define PISTOL_RARM_YMAX +ANGLE(170)
#define PISTOL_RARM_XMIN -ANGLE(80)
#define PISTOL_RARM_XMAX +ANGLE(80)
#define SHOTGUN_LOCK_YMIN -ANGLE(60)
#define SHOTGUN_LOCK_YMAX +ANGLE(60)
#define SHOTGUN_LOCK_XMIN -ANGLE(55)
#define SHOTGUN_LOCK_XMAX +ANGLE(55)
#define SHOTGUN_LARM_YMIN -ANGLE(80)
#define SHOTGUN_LARM_YMAX +ANGLE(80)
#define SHOTGUN_LARM_XMIN -ANGLE(65)
#define SHOTGUN_LARM_XMAX +ANGLE(65)
#define SHOTGUN_RARM_YMIN -ANGLE(80)
#define SHOTGUN_RARM_YMAX +ANGLE(80)
#define SHOTGUN_RARM_XMIN -ANGLE(65)
#define SHOTGUN_RARM_XMAX +ANGLE(65)
#define M16_LOCK_YMIN -ANGLE(60)
#define M16_LOCK_YMAX +ANGLE(60)
#define M16_LOCK_XMIN -ANGLE(55)
#define M16_LOCK_XMAX +ANGLE(55)
#define M16_LARM_YMIN -ANGLE(80)
#define M16_LARM_YMAX +ANGLE(80)
#define M16_LARM_XMIN -ANGLE(65)
#define M16_LARM_XMAX +ANGLE(65)
#define M16_RARM_YMIN -ANGLE(80)
#define M16_RARM_YMAX +ANGLE(80)
#define M16_RARM_XMIN -ANGLE(65)
#define M16_RARM_XMAX +ANGLE(65)
#define ROCKET_LOCK_YMIN -ANGLE(60)
#define ROCKET_LOCK_YMAX +ANGLE(60)
#define ROCKET_LOCK_XMIN -ANGLE(55)
#define ROCKET_LOCK_XMAX +ANGLE(55)
#define ROCKET_LARM_YMIN -ANGLE(80)
#define ROCKET_LARM_YMAX +ANGLE(80)
#define ROCKET_LARM_XMIN -ANGLE(65)
#define ROCKET_LARM_XMAX +ANGLE(65)
#define ROCKET_RARM_YMIN -ANGLE(80)
#define ROCKET_RARM_YMAX +ANGLE(80)
#define ROCKET_RARM_XMIN -ANGLE(65)
#define ROCKET_RARM_XMAX +ANGLE(65)
#define GRENADE_LOCK_YMIN -ANGLE(60)
#define GRENADE_LOCK_YMAX +ANGLE(60)
#define GRENADE_LOCK_XMIN -ANGLE(55)
#define GRENADE_LOCK_XMAX +ANGLE(55)
#define GRENADE_LARM_YMIN -ANGLE(80)
#define GRENADE_LARM_YMAX +ANGLE(80)
#define GRENADE_LARM_XMIN -ANGLE(65)
#define GRENADE_LARM_XMAX +ANGLE(65)
#define GRENADE_RARM_YMIN -ANGLE(80)
#define GRENADE_RARM_YMAX +ANGLE(80)
#define GRENADE_RARM_XMIN -ANGLE(65)
#define GRENADE_RARM_XMAX +ANGLE(65)
#define REVOLVER_LOCK_YMIN -ANGLE(60)
#define REVOLVER_LOCK_YMAX +ANGLE(60)
#define REVOLVER_LOCK_XMIN -ANGLE(60)
#define REVOLVER_LOCK_XMAX +ANGLE(60)
#define REVOLVER_LARM_YMIN -ANGLE(10)
#define REVOLVER_LARM_YMAX +ANGLE(10)
#define REVOLVER_LARM_XMIN -ANGLE(80)
#define REVOLVER_LARM_XMAX +ANGLE(80)
#define REVOLVER_RARM_YMIN 0
#define REVOLVER_RARM_YMAX 0
#define REVOLVER_RARM_XMIN 0
#define REVOLVER_RARM_XMAX 0
#define HARPOON_LOCK_YMIN -ANGLE(60)
#define HARPOON_LOCK_YMAX +ANGLE(60)
#define HARPOON_LOCK_XMIN -ANGLE(65)
#define HARPOON_LOCK_XMAX +ANGLE(65)
#define HARPOON_LARM_YMIN -ANGLE(20)
#define HARPOON_LARM_YMAX +ANGLE(20)
#define HARPOON_LARM_XMIN -ANGLE(75)
#define HARPOON_LARM_XMAX +ANGLE(75)
#define HARPOON_RARM_YMIN -ANGLE(80)
#define HARPOON_RARM_YMAX +ANGLE(80)
#define HARPOON_RARM_XMIN -ANGLE(75)
#define HARPOON_RARM_XMAX +ANGLE(75)
#define CROSSBOW_LOCK_YMIN -ANGLE(60)
#define CROSSBOW_LOCK_YMAX +ANGLE(60)
#define CROSSBOW_LOCK_XMIN -ANGLE(65)
#define CROSSBOW_LOCK_XMAX +ANGLE(65)
#define CROSSBOW_LARM_YMIN -ANGLE(20)
#define CROSSBOW_LARM_YMAX +ANGLE(20)
#define CROSSBOW_LARM_XMIN -ANGLE(75)
#define CROSSBOW_LARM_XMAX +ANGLE(75)
#define CROSSBOW_RARM_YMIN -ANGLE(80)
#define CROSSBOW_RARM_YMAX +ANGLE(80)
#define CROSSBOW_RARM_XMIN -ANGLE(75)
#define CROSSBOW_RARM_XMAX +ANGLE(75)
#define SKID_LOCK_YMIN -ANGLE(30)
#define SKID_LOCK_YMAX +ANGLE(30)
#define SKID_LOCK_XMIN -ANGLE(55)
#define SKID_LOCK_XMAX +ANGLE(55)
#define SKID_LARM_YMIN -ANGLE(30)
#define SKID_LARM_YMAX +ANGLE(30)
#define SKID_LARM_XMIN -ANGLE(55)
#define SKID_LARM_XMAX +ANGLE(55)
#define SKID_RARM_YMIN -ANGLE(30)
#define SKID_RARM_YMAX +ANGLE(30)
#define SKID_RARM_XMIN -ANGLE(55)
#define SKID_RARM_XMAX +ANGLE(55)

#define SHOTGUN_RANDOM_AMMO1 1820
#define SHOTGUN_RANDOM_AMMO2 5460
#define SHOTGUN_BULLET_MAX 6
#define SHOTGUN_SMOKE_MAX 7
#define SHOTGUN_SPARKS_MAX 12
#define SHOTGUN_GUNPOS_SMOKEDIST_MAX 1280
#define GRENADEGUN_SMOKE_MAX 5
#define GRENADE_TIMER 120

extern PISTOL_DEF pistols_table[4];
extern WEAPON_INFO weapons[LG_MAX_HOLD];

extern void TriggerShotgunSparks(int x, int y, int z, int xv, int yv, int zv);
extern int weapon_object(int weapon_type);
extern int weapon_meshes(int weapon_type);
extern void fire_shotgun(void);
extern void fire_grenade(void);
extern void fire_crossbow(PHD_3DPOS* haveDefinedPos);
extern void FireCrossbowBoltFromLaserSight(GAME_VECTOR* dest, GAME_VECTOR* src);
extern void InitialiseNewWeapons(void);
extern short* GetCurrentAmmo(int weapon_type);
extern void LaraTargetInfo(WEAPON_INFO* winfo);
extern void LaraGetNewTarget(WEAPON_INFO* winfo);
extern void AimWeapon(WEAPON_INFO* winfo, LARA_ARM* arm);
extern void find_target_point(ITEM_INFO* item, GAME_VECTOR* target);
extern BOOL FireWeapon(int weapon_type, ITEM_INFO* target, ITEM_INFO* src, short angles[2]);
extern int GetTargetOnLOS(GAME_VECTOR *dest, GAME_VECTOR *src, BOOL drawtarget, BOOL firing);
extern void HitTarget(ITEM_INFO* item, GAME_VECTOR* src, int damage, int flags);
extern void LaraGun(void);

#ifdef DLL_INJECT
#define CheckForHoldingState ((int(__cdecl*)(short state_current)) 0x0042DDC0)
#endif
#pragma endregion

#pragma region lara_flare
#define FL_HOLD_FT 1
#define FL_THROW_FT 32
#define FL_DRAW_FT 39
#define FL_IGNITE_FT 23
#define FL_2HOLD_FT 15
#define FL_HOLD_F 0
#define FL_THROW_F (FL_HOLD_F+FL_HOLD_FT)
#define FL_DRAW_F (FL_THROW_F+FL_THROW_FT)
#define FL_IGNITE_F (FL_DRAW_F+FL_DRAW_FT)
#define FL_2HOLD_F (FL_IGNITE_F+FL_IGNITE_FT)
#define FL_END_F (FL_2HOLD_F+FL_2HOLD_FT)
#define FL_DRAWGOTIT_F (13+FL_DRAW_F)
#define FL_PICKUP_F 58
#define FL_THROWRELEASE_F (20+FL_THROW_F)
#define FL_THROW_FRAMES 32

#ifdef DLL_INJECT
#define TorchControl ((void(__cdecl*)(void)) 0x0041F7C0)
#define DoFlareLight ((BOOL(__cdecl*)(PHD_3DPOS *pos, short age)) 0x0042F510)
#define DoFlareInHand ((void(__cdecl*)(short age)) 0x0042F6F0)
#define set_flare_arm ((void(__cdecl*)(short frame)) 0x0042FB00)
#define CreateFlare ((void(__cdecl*)(short objNumber, BOOL isThrowed)) 0x0042F880)
#define ready_flare ((void(__cdecl*)(void)) 0x0042FF50)
#define draw_flare_meshes ((void(__cdecl*)(void)) 0x0042FF10)
#define undraw_flare_meshes ((void(__cdecl*)(void)) 0x0042FF30)
#define draw_flare ((void(__cdecl*)(void)) 0x0042FB50)
#define undraw_flare ((void(__cdecl*)(void)) 0x0042FC60)
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
#define UseSpecialItem ((BOOL(__cdecl*)(ITEM_INFO* item)) 0x00424E90)
#endif
#pragma endregion

#pragma region lara_control
extern void LaraAboveWater(ITEM_INFO* item, COLL_INFO* coll);
extern void LaraSurface(ITEM_INFO* item, COLL_INFO* coll);
extern void LaraUnderWater(ITEM_INFO* item, COLL_INFO* coll);
extern void LaraWaterCurrent(COLL_INFO* coll);

#ifdef DLL_INJECT
#define legacy_LaraAboveWater ((void(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll)) 0x00420B10)
#define legacy_LaraSurface  ((void(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll)) 0x00431AA0)
#define legacy_LaraUnderWater ((void(__cdecl*)(ITEM_INFO* item, COLL_INFO* coll)) 0x004323A0)
#define legacy_LaraWaterCurrent ((void(__cdecl*)(COLL_INFO* coll)) 0x00432DA0)
#endif
#pragma endregion