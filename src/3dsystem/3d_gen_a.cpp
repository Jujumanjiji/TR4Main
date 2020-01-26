#include "framework.h"
#include "3d_gen_a.h"

void phd_PushMatrix(void)
{
    int *backup = phd_mxptr;
    memcpy(phd_mxptr + 12, backup, sizeof(PHD_MATRIX));
    phd_mxptr = backup + 12;
}

void phd_PushUnitMatrix(void)
{
    // increase matrix by 1
    phd_mxptr += 12;

    // set the unit matrix mode
    phd_mxptr[M22] = 0x4000;
    phd_mxptr[M11] = 0x4000;
    phd_mxptr[M00] = 0x4000;
    phd_mxptr[M03] = 0;
    phd_mxptr[M02] = 0;
    phd_mxptr[M01] = 0;
    phd_mxptr[M13] = 0;
    phd_mxptr[M12] = 0;
    phd_mxptr[M10] = 0;
    phd_mxptr[M23] = 0;
    phd_mxptr[M21] = 0;
    phd_mxptr[M20] = 0;
}

void phd_PopMatrix(void)
{
    phd_mxptr -= 12;
}

void injector::inject_3d_gen_a()
{
    this->inject(0x004902B0, phd_PushMatrix);
    this->inject(0x00490310, phd_PushUnitMatrix);
    this->inject(NULL,       phd_PopMatrix); // original are define !
}