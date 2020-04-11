#include "framework.h"
#include "3d_gen_a.h"

void phd_PushMatrix(void)
{
    memcpy(&phd_mxptr[1], &phd_mxptr[0], sizeof(PHD_MATRIX));
    phd_mxptr++;
}

void phd_PushUnitMatrix(void)
{
    // increase matrix by 1
    phd_mxptr++;

    // set the unit matrix mode
    phd_mxptr->m00 = 0x4000;
    phd_mxptr->m01 = 0;
    phd_mxptr->m02 = 0;
    phd_mxptr->m03 = 0;

    phd_mxptr->m10 = 0;
    phd_mxptr->m11 = 0x4000;
    phd_mxptr->m12 = 0;
    phd_mxptr->m13 = 0;

    phd_mxptr->m20 = 0;
    phd_mxptr->m21 = 0;
    phd_mxptr->m22 = 0x4000;
    phd_mxptr->m23 = 0;
}

void phd_PopMatrix(void)
{
    phd_mxptr--;
}

#ifdef DLL_INJECT
void injector::inject_3d_gen_a()
{
    this->inject(0x004902B0, phd_PushMatrix);
    this->inject(0x00490310, phd_PushUnitMatrix);
    this->inject(NULL,       phd_PopMatrix); // original are define !
}
#endif