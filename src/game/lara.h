#pragma once
#include "lara_constants.h"

///#define NEW_ROUTINES

#define InitialiseLaraLoad INITIALISE(0x00430EB0)
#define FlareItemControl CONTROL(0x0042FF90)
#define DrawFlareItem DRAW(0x0042F7B0)

#define LARA_ROUTINES_ARGS ITEM_INFO* item, COLL_INFO* coll
#define LARA_FUNC(name) extern void name(LARA_ROUTINES_ARGS)

#define LARA_ROUTINESNEW(address, newAddr) _FUNCV(address, (LARA_ROUTINES_ARGS), newAddr)
#ifdef NEW_ROUTINES
#define LARA_ROUTINES(address) __FUNCV(address, LARA_ROUTINES_ARGS)
#else
#define LARA_ROUTINES(address) ((void(__cdecl*)(LARA_ROUTINES_ARGS)) address)
#endif

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
LARA_FUNC(lara_as_climbing);
LARA_FUNC(lara_as_climbleft);
LARA_FUNC(lara_as_climbend);
LARA_FUNC(lara_as_climbright);
LARA_FUNC(lara_as_climbdown);
LARA_FUNC(lara_as_wade);
LARA_FUNC(lara_as_waterroll);
LARA_FUNC(lara_as_pickupflare);
LARA_FUNC(lara_as_deathslide);

#define legacy_lara_as_walk                    LARA_ROUTINESNEW(0x00425AC0, lara_as_walk)
#define legacy_lara_as_run                     LARA_ROUTINESNEW(0x00425B60, lara_as_run)
#define legacy_lara_as_stop                    LARA_ROUTINESNEW(0x00424A60, lara_as_stop)
#define legacy_lara_as_forwardjump             LARA_ROUTINESNEW(0x00425480, lara_as_forwardjump)
#define legacy_lara_as_fastback                LARA_ROUTINESNEW(0x00425D10, lara_as_fastback)
#define legacy_lara_as_turnr                   LARA_ROUTINESNEW(0x00425D70, lara_as_turnr)
#define legacy_lara_as_turnl                   LARA_ROUTINESNEW(0x00425E20, lara_as_turnl)
#define legacy_lara_as_death                   LARA_ROUTINESNEW(0x00425EC0, lara_as_death)
#define legacy_lara_as_fastfall                LARA_ROUTINESNEW(0x00425F10, lara_as_fastfall)
#define legacy_lara_as_hang                    LARA_ROUTINESNEW(0x00425F50, lara_as_hang)
#define legacy_lara_as_reach                   LARA_ROUTINESNEW(0x00425630, lara_as_reach)
#define legacy_lara_as_splat                   LARA_ROUTINESNEW(0x00426EB0, lara_as_splat)
#define legacy_lara_as_tread                   LARA_ROUTINESNEW(0x004327A0, lara_as_tread)
#define legacy_lara_as_compress                LARA_ROUTINESNEW(0x00426EC0, lara_as_compress)
#define legacy_lara_as_back                    LARA_ROUTINESNEW(0x00426FD0, lara_as_back)
#define legacy_lara_as_swim                    LARA_ROUTINESNEW(0x00432620, lara_as_swim)
#define legacy_lara_as_glide                   LARA_ROUTINESNEW(0x00432720, lara_as_glide)
#define legacy_lara_as_null                    LARA_ROUTINESNEW(0x00427060, lara_as_null)
#define legacy_lara_as_fastturn                LARA_ROUTINESNEW(0x00427070, lara_as_fastturn)
#define legacy_lara_as_stepright               LARA_ROUTINESNEW(0x004270C0, lara_as_stepright)
#define legacy_lara_as_stepleft                LARA_ROUTINESNEW(0x00427150, lara_as_stepleft)
#define legacy_lara_as_slide                   LARA_ROUTINESNEW(0x004271E0, lara_as_slide)
#define legacy_lara_as_backjump                LARA_ROUTINESNEW(0x00427210, lara_as_backjump)
#define legacy_lara_as_rightjump               LARA_ROUTINESNEW(0x00427260, lara_as_rightjump)
#define legacy_lara_as_leftjump                LARA_ROUTINESNEW(0x004272A0, lara_as_leftjump)
#define legacy_lara_as_upjump                  LARA_ROUTINESNEW(0x00425110, lara_as_upjump)
#define legacy_lara_as_fallback                LARA_ROUTINESNEW(0x004272E0, lara_as_fallback)
#define legacy_lara_as_hangleft                LARA_ROUTINESNEW(0x00426B50, lara_as_hangleft)
#define legacy_lara_as_hangright               LARA_ROUTINESNEW(0x00426BD0, lara_as_hangright)
#define legacy_lara_as_slideback               LARA_ROUTINESNEW(0x00427310, lara_as_slideback)
#define legacy_lara_as_surftread               LARA_ROUTINESNEW(0x00431DA0, lara_as_surftread)
#define legacy_lara_as_surfswim                LARA_ROUTINESNEW(0x00431C10, lara_as_surfswim)
#define legacy_lara_as_dive                    LARA_ROUTINESNEW(0x00432830, lara_as_dive)
#define legacy_lara_as_pushblock               LARA_ROUTINESNEW(0x00427330, lara_as_pushblock)
#define legacy_lara_as_pullblock               LARA_ROUTINESNEW(0x00427370, lara_as_pullblock)
#define legacy_lara_as_ppready                 LARA_ROUTINESNEW(0x004273B0, lara_as_ppready)
#define legacy_lara_as_pickup                  LARA_ROUTINESNEW(0x004273E0, lara_as_pickup)
#define legacy_lara_as_switchon                LARA_ROUTINESNEW(0x00427480, lara_as_switchon)
#define legacy_lara_as_usekey                  LARA_ROUTINESNEW(0x004274C0, lara_as_usekey)
#define legacy_lara_as_usepuzzle               LARA_ROUTINESNEW(0x00427500, lara_as_usepuzzle)
#define legacy_lara_as_uwdeath                 LARA_ROUTINESNEW(0x00432850, lara_as_uwdeath)
#define legacy_lara_as_special                 LARA_ROUTINESNEW(0x00427580, lara_as_special)
#define legacy_lara_as_surfback                LARA_ROUTINESNEW(0x00431C80, lara_as_surfback)
#define legacy_lara_as_surfleft                LARA_ROUTINESNEW(0x00431CE0, lara_as_surfleft)
#define legacy_lara_as_surfright               LARA_ROUTINESNEW(0x00431D40, lara_as_surfright)
#define legacy_lara_as_swandive                LARA_ROUTINESNEW(0x004275A0, lara_as_swandive)
#define legacy_lara_as_fastdive                LARA_ROUTINESNEW(0x004275E0, lara_as_fastdive)
#define legacy_lara_as_waterout                LARA_ROUTINESNEW(0x00427640, lara_as_waterout)
#define legacy_lara_as_climbstnc               LARA_ROUTINESNEW(0x0042C6C0, lara_as_climbstnc)
#define legacy_lara_as_climbing                LARA_ROUTINESNEW(0x0042D470, lara_as_climbing)
#define legacy_lara_as_climbleft               LARA_ROUTINESNEW(0x0042CC80, lara_as_climbleft)
#define legacy_lara_as_climbend                LARA_ROUTINESNEW(0x0042D7A0, lara_as_climbend)
#define legacy_lara_as_climbright              LARA_ROUTINESNEW(0x0042D3D0, lara_as_climbright)
#define legacy_lara_as_climbdown               LARA_ROUTINESNEW(0x0042D5E0, lara_as_climbdown)
#define legacy_lara_as_wade                    LARA_ROUTINESNEW(0x00427660, lara_as_wade)
#define legacy_lara_as_waterroll               LARA_ROUTINESNEW(0x004328B0, lara_as_waterroll)
#define legacy_lara_as_pickupflare             LARA_ROUTINESNEW(0x00427420, lara_as_pickupflare)
#define legacy_lara_as_deathslide              LARA_ROUTINESNEW(0x00427730, lara_as_deathslide)
#define legacy_lara_as_duck                    LARA_ROUTINES(0x00421040)
#define legacy_lara_as_dash                    LARA_ROUTINES(0x004220B0)
#define legacy_lara_as_dashdive                LARA_ROUTINES(0x00422A90)
#define legacy_lara_as_hang2                   LARA_ROUTINES(0x00422DE0)
#define legacy_lara_as_monkeyswing             LARA_ROUTINES(0x00423880)
#define legacy_lara_as_monkeyl                 LARA_ROUTINES(0x00423A60)
#define legacy_lara_as_monkeyr                 LARA_ROUTINES(0x00423B30)
#define legacy_lara_as_monkey180               LARA_ROUTINES(0x00423D20)
#define legacy_lara_as_all4s                   LARA_ROUTINES(0x00421210)
#define legacy_lara_as_crawl                   LARA_ROUTINES(0x004216A0)
#define legacy_lara_as_hangturnl               LARA_ROUTINES(0x00423C00)
#define legacy_lara_as_hangturnr               LARA_ROUTINES(0x00423CE0)
#define legacy_lara_as_all4turnl               LARA_ROUTINES(0x00421900)
#define legacy_lara_as_all4turnr               LARA_ROUTINES(0x004219B0)
#define legacy_lara_as_crawlb                  LARA_ROUTINES(0x00421A10)
#define legacy_lara_as_controlled              LARA_ROUTINES(0x004277D0)
#define legacy_lara_as_ropel                   LARA_ROUTINES(0x00424620)
#define legacy_lara_as_roper                   LARA_ROUTINES(0x00424650)
#define legacy_lara_as_controlledl             LARA_ROUTINES(0x004278B0)
#define legacy_lara_as_poleleft                LARA_ROUTINES(0x00423EE0)
#define legacy_lara_as_poleright               LARA_ROUTINES(0x00423F20)
#define legacy_lara_as_pulley                  LARA_ROUTINES(0x00427940)
#define legacy_lara_as_duckl                   LARA_ROUTINES(0x00420D50)
#define legacy_lara_as_duckr                   LARA_ROUTINES(0x00420D80)
#define legacy_lara_as_extcornerl              LARA_ROUTINES(0x00426D70)
#define legacy_lara_as_extcornerr              LARA_ROUTINES(0x00426DC0)
#define legacy_lara_as_intcornerl              LARA_ROUTINES(0x00426E10)
#define legacy_lara_as_intcornerr              LARA_ROUTINES(0x00426E60)
#define legacy_lara_as_rope                    LARA_ROUTINES(0x004241A0)
#define legacy_lara_as_climbrope               LARA_ROUTINES(0x004248E0)
#define legacy_lara_as_climbroped              LARA_ROUTINES(0x00424950)

/// Lara Collision Routines:


#define legacy_lara_default_col                LARA_ROUTINES(0x00428AD0)
#define legacy_lara_col_walk                   LARA_ROUTINES(0x00427A40)
#define legacy_lara_col_run                    LARA_ROUTINES(0x00427C30)
#define legacy_lara_col_stop                   LARA_ROUTINES(0x00425070)
#define legacy_lara_col_forwardjump            LARA_ROUTINES(0x00425550)
#define legacy_lara_col_fastback               LARA_ROUTINES(0x00427E00)
#define legacy_lara_col_turn                   LARA_ROUTINES(0x00427EF0)
#define legacy_lara_col_death                  LARA_ROUTINES(0x00427FD0)
#define legacy_lara_col_fastfall               LARA_ROUTINES(0x00428040)
#define legacy_lara_col_hang                   LARA_ROUTINES(0x00425FA0)
#define legacy_lara_col_reach                  LARA_ROUTINES(0x00425650)
#define legacy_lara_col_splat                  LARA_ROUTINES(0x004280F0)
#define legacy_lara_col_tread                  LARA_ROUTINES(0x00432B30)
#define legacy_lara_col_compress               LARA_ROUTINES(0x00428160)
#define legacy_lara_col_back                   LARA_ROUTINES(0x00428210)
#define legacy_lara_col_swim                   LARA_ROUTINES(0x004328C0)
#define legacy_lara_col_glide                  LARA_ROUTINES(0x00432B10)
#define legacy_lara_col_stepright              LARA_ROUTINES(0x00428350)
#define legacy_lara_col_roll2                  LARA_ROUTINES(0x00428770)
#define legacy_lara_col_slide                  LARA_ROUTINES(0x00428450)
#define legacy_lara_col_backjump               LARA_ROUTINES(0x00428580)
#define legacy_lara_col_rightjump              LARA_ROUTINES(0x004285B0)
#define legacy_lara_col_leftjump               LARA_ROUTINES(0x004285E0)
#define legacy_lara_col_upjump                 LARA_ROUTINES(0x00425130)
#define legacy_lara_col_fallback               LARA_ROUTINES(0x00428610)
#define legacy_lara_col_hangleft               LARA_ROUTINES(0x00426B90)
#define legacy_lara_col_hangright              LARA_ROUTINES(0x00426C10)
#define legacy_lara_col_slideback              LARA_ROUTINES(0x004286A0)
#define legacy_lara_col_surftread              LARA_ROUTINES(0x00432340)
#define legacy_lara_col_surfswim               LARA_ROUTINES(0x00431E60)
#define legacy_lara_col_dive                   LARA_ROUTINES(0x00432B50)
#define legacy_lara_col_uwdeath                LARA_ROUTINES(0x00432B70)
#define legacy_lara_col_roll                   LARA_ROUTINES(0x004286D0)
#define legacy_lara_col_surfback               LARA_ROUTINES(0x004322B0)
#define legacy_lara_col_surfleft               LARA_ROUTINES(0x004322E0)
#define legacy_lara_col_surfright              LARA_ROUTINES(0x00432310)
#define legacy_lara_col_swandive               LARA_ROUTINES(0x00428850)
#define legacy_lara_col_fastdive               LARA_ROUTINES(0x004288D0)
#define legacy_lara_col_climbstnc              LARA_ROUTINES(0x0042C770)
#define legacy_lara_col_climbing               LARA_ROUTINES(0x0042D490)
#define legacy_lara_col_climbleft              LARA_ROUTINES(0x0042CCC0)
#define legacy_lara_col_climbright             LARA_ROUTINES(0x0042D410)
#define legacy_lara_col_climbdown              LARA_ROUTINES(0x0042D600)
#define legacy_lara_col_wade                   LARA_ROUTINES(0x00428960)
#define legacy_lara_col_waterroll              LARA_ROUTINES(0x00432BD0)
#define legacy_lara_col_duck                   LARA_ROUTINES(0x004210F0)
#define legacy_lara_col_dash                   LARA_ROUTINES(0x00422200)
#define legacy_lara_col_dashdive               LARA_ROUTINES(0x00422AC0)
#define legacy_lara_col_hang2                  LARA_ROUTINES(0x00422EB0)
#define legacy_lara_col_monkeyswing            LARA_ROUTINES(0x00423930)
#define legacy_lara_col_monkeyl                LARA_ROUTINES(0x00423AC0)
#define legacy_lara_col_monkeyr                LARA_ROUTINES(0x00423B90)
#define legacy_lara_col_monkey180              LARA_ROUTINES(0x00423D40)
#define legacy_lara_col_all4s                  LARA_ROUTINES(0x00421280)
#define legacy_lara_col_crawl                  LARA_ROUTINES(0x00421770)
#define legacy_lara_col_hangturn               LARA_ROUTINES(0x00423C40)
#define legacy_lara_col_all4turn               LARA_ROUTINES(0x00421960)
#define legacy_lara_col_crawlb                 LARA_ROUTINES(0x00421AD0)
#define legacy_lara_col_crawl2hang             LARA_ROUTINES(0x00421BF0)
#define legacy_lara_col_turnswitch             LARA_ROUTINES(0x004278D0)
#define legacy_lara_col_polestat               LARA_ROUTINES(0x00423D60)
#define legacy_lara_col_poleup                 LARA_ROUTINES(0x00423F60)
#define legacy_lara_col_poledown               LARA_ROUTINES(0x00423FF0)
#define legacy_lara_col_ropefwd                LARA_ROUTINES(0x00420DB0)
#define legacy_lara_col_rope1                  LARA_ROUTINES(0x00424270)
#define legacy_lara_col_rope2                  LARA_ROUTINES(0x00424680)