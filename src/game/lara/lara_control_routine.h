#pragma once

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
#define legacy_lara_as_duck                    LARA_ROUTINESNEW(0x00421040, lara_as_duck)
#define legacy_lara_as_dash                    LARA_ROUTINESNEW(0x004220B0, lara_as_dash)
#define legacy_lara_as_dashdive                LARA_ROUTINESNEW(0x00422A90, lara_as_dashdive)
#define legacy_lara_as_hang2                   LARA_ROUTINESNEW(0x00422DE0, lara_as_hang2)
#define legacy_lara_as_monkeyswing             LARA_ROUTINESNEW(0x00423880, lara_as_monkeyswing)
#define legacy_lara_as_monkeyl                 LARA_ROUTINESNEW(0x00423A60, lara_as_monkeyl)
#define legacy_lara_as_monkeyr                 LARA_ROUTINESNEW(0x00423B30, lara_as_monkeyr)
#define legacy_lara_as_monkey180               LARA_ROUTINESNEW(0x00423D20, lara_as_monkey180)
#define legacy_lara_as_all4s                   LARA_ROUTINESNEW(0x00421210, lara_as_all4s)
#define legacy_lara_as_crawl                   LARA_ROUTINESNEW(0x004216A0, lara_as_crawl)
#define legacy_lara_as_hangturnl               LARA_ROUTINESNEW(0x00423C00, lara_as_hangturnl)
#define legacy_lara_as_hangturnr               LARA_ROUTINESNEW(0x00423CE0, lara_as_hangturnr)
#define legacy_lara_as_all4turnl               LARA_ROUTINESNEW(0x00421900, lara_as_all4turnl)
#define legacy_lara_as_all4turnr               LARA_ROUTINESNEW(0x004219B0, lara_as_all4turnr)
#define legacy_lara_as_crawlb                  LARA_ROUTINESNEW(0x00421A10, lara_as_crawlb)
#define legacy_lara_as_controlled              LARA_ROUTINESNEW(0x004277D0, lara_as_controlled)
#define legacy_lara_as_ropel                   LARA_ROUTINESNEW(0x00424620, lara_as_ropel)
#define legacy_lara_as_roper                   LARA_ROUTINESNEW(0x00424650, lara_as_roper)
#define legacy_lara_as_controlledl             LARA_ROUTINESNEW(0x004278B0, lara_as_controlledl)
#define legacy_lara_as_poleleft                LARA_ROUTINESNEW(0x00423EE0, lara_as_poleleft)
#define legacy_lara_as_poleright               LARA_ROUTINESNEW(0x00423F20, lara_as_poleright)
#define legacy_lara_as_pulley                  LARA_ROUTINESNEW(0x00427940, lara_as_pulley)
#define legacy_lara_as_duckl                   LARA_ROUTINESNEW(0x00420D50, lara_as_duckl)
#define legacy_lara_as_duckr                   LARA_ROUTINESNEW(0x00420D80, lara_as_duckr)
#define legacy_lara_as_extcornerl              LARA_ROUTINESNEW(0x00426D70, lara_as_extcornerl)
#define legacy_lara_as_extcornerr              LARA_ROUTINESNEW(0x00426DC0, lara_as_extcornerr)
#define legacy_lara_as_intcornerl              LARA_ROUTINESNEW(0x00426E10, lara_as_intcornerl)
#define legacy_lara_as_intcornerr              LARA_ROUTINESNEW(0x00426E60, lara_as_intcornerr)
#define legacy_lara_as_rope                    LARA_ROUTINESNEW(0x004241A0, lara_as_rope)
#define legacy_lara_as_climbrope               LARA_ROUTINESNEW(0x004248E0, lara_as_climbrope)
#define legacy_lara_as_climbroped              LARA_ROUTINESNEW(0x00424950, lara_as_climbroped)