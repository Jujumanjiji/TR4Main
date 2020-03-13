// TODO: THIS FILE ONLY EXIST FOR INJECT, DELETE IT WHEN TURNING TO EXE !
#include "framework.h"
#include "lara.h"

#ifdef DLL_INJECT
void injector::inject_lara()
{
    /// LARA ROUTINES:
    this->inject(ADDRESS_STRUCT(0x00420B10, LaraAboveWater));
    this->inject(ADDRESS_STRUCT(0x00431AA0, LaraSurface));
    this->inject(ADDRESS_STRUCT(0x004323A0, LaraUnderWater));
    this->inject(ADDRESS_STRUCT(0x00432DA0, LaraWaterCurrent));

    /// CONTROL ROUTINES:
    // no more need to inject these one since i recreated these array from the function above !
    /*
    this->inject(legacy_lara_as_walk);
    this->inject(legacy_lara_as_run);
    this->inject(legacy_lara_as_stop);
    this->inject(legacy_lara_as_forwardjump);
    this->inject(legacy_lara_as_fastback);
    this->inject(legacy_lara_as_turnr);
    this->inject(legacy_lara_as_turnl);
    this->inject(legacy_lara_as_death);
    this->inject(legacy_lara_as_fastfall);
    this->inject(legacy_lara_as_hang);
    this->inject(legacy_lara_as_reach);
    this->inject(legacy_lara_as_splat);
    this->inject(legacy_lara_as_tread);
    this->inject(legacy_lara_as_compress);
    this->inject(legacy_lara_as_back);
    this->inject(legacy_lara_as_swim);
    this->inject(legacy_lara_as_glide);
    this->inject(legacy_lara_as_null);
    this->inject(legacy_lara_as_fastturn);
    this->inject(legacy_lara_as_stepright);
    this->inject(legacy_lara_as_stepleft);
    this->inject(legacy_lara_as_slide);
    this->inject(legacy_lara_as_backjump);
    this->inject(legacy_lara_as_rightjump);
    this->inject(legacy_lara_as_leftjump);
    this->inject(legacy_lara_as_upjump);
    this->inject(legacy_lara_as_fallback);
    this->inject(legacy_lara_as_hangleft);
    this->inject(legacy_lara_as_hangright);
    this->inject(legacy_lara_as_slideback);
    this->inject(legacy_lara_as_surftread);
    this->inject(legacy_lara_as_surfswim);
    this->inject(legacy_lara_as_dive);
    this->inject(legacy_lara_as_pushblock);
    this->inject(legacy_lara_as_pullblock);
    this->inject(legacy_lara_as_ppready);
    this->inject(legacy_lara_as_pickup);
    this->inject(legacy_lara_as_switchon);
    this->inject(legacy_lara_as_usekey);
    this->inject(legacy_lara_as_usepuzzle);
    this->inject(legacy_lara_as_surfback);
    this->inject(legacy_lara_as_surfleft);
    this->inject(legacy_lara_as_surfright);
    this->inject(legacy_lara_as_swandive);
    this->inject(legacy_lara_as_fastdive);
    this->inject(legacy_lara_as_waterout);
    this->inject(legacy_lara_as_climbstnc);
    this->inject(legacy_lara_as_climbup);
    this->inject(legacy_lara_as_climbleft);
    this->inject(legacy_lara_as_climbend);
    this->inject(legacy_lara_as_climbright);
    this->inject(legacy_lara_as_climbdown);
    this->inject(legacy_lara_as_wade);
    this->inject(legacy_lara_as_waterroll);
    this->inject(legacy_lara_as_pickupflare);
    this->inject(legacy_lara_as_deathslide);
    this->inject(legacy_lara_as_duck);
    this->inject(legacy_lara_as_dash);
    this->inject(legacy_lara_as_dashdive);
    this->inject(legacy_lara_as_hang2);
    this->inject(legacy_lara_as_monkeyswing);
    this->inject(legacy_lara_as_monkeyl);
    this->inject(legacy_lara_as_monkeyr);
    this->inject(legacy_lara_as_monkey180);
    this->inject(legacy_lara_as_all4s);
    this->inject(legacy_lara_as_crawl);
    this->inject(legacy_lara_as_hangturnl);
    this->inject(legacy_lara_as_hangturnr);
    this->inject(legacy_lara_as_all4turnl);
    this->inject(legacy_lara_as_all4turnr);
    this->inject(legacy_lara_as_crawlb);
    this->inject(legacy_lara_as_controlled);
    this->inject(legacy_lara_as_ropel);
    this->inject(legacy_lara_as_roper);
    this->inject(legacy_lara_as_controlledl);
    this->inject(legacy_lara_as_poleleft);
    this->inject(legacy_lara_as_poleright);
    this->inject(legacy_lara_as_pulley);
    this->inject(legacy_lara_as_duckl);
    this->inject(legacy_lara_as_duckr);
    this->inject(legacy_lara_as_extcornerl);
    this->inject(legacy_lara_as_extcornerr);
    this->inject(legacy_lara_as_intcornerl);
    this->inject(legacy_lara_as_intcornerr);
    this->inject(legacy_lara_as_rope);
    this->inject(legacy_lara_as_climbrope);
    this->inject(legacy_lara_as_climbroped);

    /// COLLISION ROUTINES:
    this->inject(legacy_lara_default_col);
    this->inject(legacy_lara_col_walk);
    this->inject(legacy_lara_col_run);
    this->inject(legacy_lara_col_stop);
    this->inject(legacy_lara_col_forwardjump);
    this->inject(legacy_lara_col_fastback);
    this->inject(legacy_lara_col_turn);
    this->inject(legacy_lara_col_death);
    this->inject(legacy_lara_col_fastfall);
    this->inject(legacy_lara_col_hang);
    this->inject(legacy_lara_col_reach);
    this->inject(legacy_lara_col_splat);
    this->inject(legacy_lara_col_tread);
    this->inject(legacy_lara_col_compress);
    this->inject(legacy_lara_col_back);
    this->inject(legacy_lara_col_swim);
    this->inject(legacy_lara_col_glide);
    this->inject(legacy_lara_col_step);
    this->inject(legacy_lara_col_roll); 
    this->inject(legacy_lara_col_slide);
    this->inject(legacy_lara_col_backjump);
    this->inject(legacy_lara_col_rightjump);
    this->inject(legacy_lara_col_leftjump);
    this->inject(legacy_lara_col_upjump);
    this->inject(legacy_lara_col_fallback);
    this->inject(legacy_lara_col_hangleft);
    this->inject(legacy_lara_col_hangright);
    this->inject(legacy_lara_col_slideback);
    this->inject(legacy_lara_col_surftread);
    this->inject(legacy_lara_col_surfswim);
    this->inject(legacy_lara_col_dive);
    this->inject(legacy_lara_col_uwdeath);
    this->inject(legacy_lara_col_roll2);
    this->inject(legacy_lara_col_surfback);
    this->inject(legacy_lara_col_surfleft);
    this->inject(legacy_lara_col_surfright);
    this->inject(legacy_lara_col_swandive);
    this->inject(legacy_lara_col_fastdive);
    this->inject(legacy_lara_col_climbstnc);
    this->inject(legacy_lara_col_climbup);
    this->inject(legacy_lara_col_climbleft);
    this->inject(legacy_lara_col_climbright);
    this->inject(legacy_lara_col_climbdown);
    this->inject(legacy_lara_col_wade);
    this->inject(legacy_lara_col_waterroll);
    this->inject(legacy_lara_col_duck);
    this->inject(legacy_lara_col_dash);
    this->inject(legacy_lara_col_dashdive);
    this->inject(legacy_lara_col_hang2);
    this->inject(legacy_lara_col_monkeyswing);
    this->inject(legacy_lara_col_monkeyl);
    this->inject(legacy_lara_col_monkeyr);
    this->inject(legacy_lara_col_monkey180); // TODO: recreate it, it's not used because it's use lara_col_monkeyswing but no IN_ROLL is set !
    this->inject(legacy_lara_col_all4s);
    this->inject(legacy_lara_col_crawl);
    this->inject(legacy_lara_col_hangturn);
    this->inject(legacy_lara_col_all4turn);
    this->inject(legacy_lara_col_crawlb);
    this->inject(legacy_lara_col_crawl2hang);
    this->inject(legacy_lara_col_turnswitch);
    this->inject(legacy_lara_col_polestat);
    this->inject(legacy_lara_col_poleup);
    this->inject(legacy_lara_col_poledown);
    this->inject(legacy_lara_col_duck2);
    this->inject(legacy_lara_col_rope);
    this->inject(legacy_lara_col_ropefwd);
    */
}
#endif