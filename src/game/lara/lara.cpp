#include "framework.h"
#include "lara.h"
#include "3d_gen.h"
#include "camera.h"
#include "sound.h"
#include "utils.h"

void injector::inject_lara()
{
    /// CONTROL ROUTINES:
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
    this->inject(legacy_lara_as_climbing);
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
}