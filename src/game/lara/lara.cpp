// TODO: THIS FILE ONLY EXIST FOR INJECT, DELETE IT WHEN TURNING TO EXE !
#include "framework.h"
#include "lara.h"

#ifdef DLL_INJECT
void injector::inject_lara()
{
    /// LARA ROUTINES:
    this->inject(0x00430250, LaraControl);
    this->inject(0x00420B10, LaraAboveWater);
    this->inject(0x00431AA0, LaraSurface);
    this->inject(0x004323A0, LaraUnderWater);
    this->inject(0x00432DA0, LaraWaterCurrent);
}
#endif