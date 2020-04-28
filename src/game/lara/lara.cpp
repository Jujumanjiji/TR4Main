// TODO: THIS FILE ONLY EXIST FOR INJECT, DELETE IT WHEN TURNING TO EXE !
#include "framework.h"
#include "lara.h"

#ifdef DLL_INJECT
void injector::f_game::inject_lara()
{
    /// LARA ROUTINES:
    inject(0x00430250, LaraControl);
    inject(0x00420B10, LaraAboveWater);
    inject(0x00431AA0, LaraSurface);
    inject(0x004323A0, LaraUnderWater);
    inject(0x00432DA0, LaraWaterCurrent);
}
#endif
