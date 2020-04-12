#include "framework.h"
#include "draw.h"
#include "3d_gen.h"
#include "3d_gen_a.h"
#include "output.h"
#include "specific.h"
#include "utils.h"

int GetFrames(ITEM_INFO* item, short* frame[], int* rate)
{
    ANIM_STRUCT *anim;
    int frm;
    int first = 0, second = 0;
    int frame_size;
    int interl;
    int interp, rat;

    frm = item->frame_number;
    anim = &anims[item->anim_number];
    frame[0] = frame[1] = anim->frame_ptr;
    interl = LOBYTE(anim->interpolation);
    rat = interl;
    *rate = interl;
    frame_size = anim->interpolation >> 8;
    frm -= anim->frame_base;
    first = frm / rat;
    interp = frm % rat;
    frame[0] += first * frame_size;                 // Get Frame pointers
    frame[1] = frame[0] + frame_size;               // and store away
    if (interp == 0)
        return 0;
    second = first * rat + rat;
    if (second > anim->frame_end)                   // Clamp KeyFrame to End if need be
        *rate = anim->frame_end - (second - rat);
    return interp;
}

void InitInterpolate(int frac, int rate)
{
    IM_rate = rate;
    IM_frac = frac;
    IM_ptr = IM_stack;
    memcpy(IM_stack, phd_mxptr, sizeof(PHD_MATRIX));
}

void phd_PopMatrix_I(void)
{
    phd_PopMatrix();
    IM_ptr--;
}

void phd_PushMatrix_I(void)
{
    phd_PushMatrix();
    memcpy(&IM_ptr[1], &IM_ptr[0], sizeof(PHD_MATRIX));
    IM_ptr++;
}

void phd_RotY_I(short ang)
{
    PHD_MATRIX* backup;
    phd_RotY(ang);
    backup = phd_mxptr;
    phd_mxptr = IM_ptr;
    phd_RotY(ang);
    phd_mxptr = backup;
}

void phd_RotX_I(short ang)
{
    PHD_MATRIX* backup;
    phd_RotX(ang);
    backup = phd_mxptr;
    phd_mxptr = IM_ptr;
    phd_RotX(ang);
    phd_mxptr = backup;
}

void phd_RotZ_I(short ang)
{
    PHD_MATRIX* backup;
    phd_RotZ(ang);
    backup = phd_mxptr;
    phd_mxptr = IM_ptr;
    phd_RotZ(ang);
    phd_mxptr = backup;
}

void phd_TranslateRel_I(int x, int y, int z)
{
    PHD_MATRIX* backup;
    phd_TranslateRel(x, y, z);
    backup = phd_mxptr;
    phd_mxptr = IM_ptr;
    phd_TranslateRel(x, y, z);
    phd_mxptr = backup;
}

void phd_TranslateRel_ID(int x1, int y1, int z1, int x2, int y2, int z2)
{
    PHD_MATRIX *backup;
    phd_TranslateRel(x1, y1, z1);
    backup = phd_mxptr;
    phd_mxptr = IM_ptr;
    phd_TranslateRel(x2, y2, z2);
    phd_mxptr = backup;
}

void phd_RotYXZ_I(short y, short x, short z)
{
    PHD_MATRIX *backup;
    phd_RotYXZ(y, x, z);
    backup = phd_mxptr;
    phd_mxptr = IM_ptr;
    phd_RotYXZ(y, x, z);
    phd_mxptr = backup;
}

void gar_RotYXZsuperpack_I(short** pprot1, short** pprot2, int skip)
{
    PHD_MATRIX *backup;
    gar_RotYXZsuperpack(pprot1, skip);
    backup = phd_mxptr;
    phd_mxptr = IM_ptr;
    gar_RotYXZsuperpack(pprot2, skip);
    phd_mxptr = backup;
}

void gar_RotYXZsuperpack(short** pprot, int skip)
{
    uint16 *prot;
    uint16 rot;
    uint8 flag;

    while (skip)
    {
        flag = *((uint8*)*pprot + 1);
        if (flag & 192)
            (*pprot) += 1;
        else
            (*pprot) += 2;
        skip--;
    }

    prot = (uint16*)*pprot;
    switch (*prot >> PACKED_SHIFT)
    {
        case GAR_YXZPACK:
            phd_RotYXZpack((*(prot + 0) << 16) + *(prot + 1));
            *pprot += 2;
            return;
        case GAR_Y:
            phd_RotY(16 * short(*(prot + 0)));
            break;
        case GAR_X:
            phd_RotX(16 * short(*(prot + 0)));
            break;
        case GAR_Z: default:
            phd_RotZ(16 * short(*(prot + 0)));
            break;
    }

    ++*pprot;
}

void phd_PutPolygons_I(short* ptr, int clip)
{
    phd_PushMatrix();
    InterpolateMatrix();
    phd_PutPolygons(ptr, clip);
    phd_PopMatrix();
}

void InterpolateMatrix(void)
{
    PHD_MATRIX *mptr, *iptr;
    int frac, rate;

    mptr = phd_mxptr;
    iptr = IM_ptr;
    frac = IM_frac;
    rate = IM_rate;

    if (rate == 2 || (frac == 2 && rate == 4))                      // If interpolating Odd Frames
    {                                                               // then do simple case
        mptr->m00 = (iptr->m00 + mptr->m00) >> 1;
        mptr->m01 = (iptr->m01 + mptr->m01) >> 1;
        mptr->m02 = (iptr->m02 + mptr->m02) >> 1;
        mptr->m03 = (iptr->m03 + mptr->m03) >> 1;

        mptr->m10 = (iptr->m10 + mptr->m10) >> 1;
        mptr->m11 = (iptr->m11 + mptr->m11) >> 1;
        mptr->m12 = (iptr->m12 + mptr->m12) >> 1;
        mptr->m13 = (iptr->m13 + mptr->m13) >> 1;

        mptr->m20 = (iptr->m20 + mptr->m20) >> 1;
        mptr->m21 = (iptr->m21 + mptr->m21) >> 1;
        mptr->m22 = (iptr->m22 + mptr->m22) >> 1;
        mptr->m23 = (iptr->m23 + mptr->m23) >> 1;
    }
    else if (frac == 1)
    {
        mptr->m00 += (iptr->m00 - mptr->m00) >> 2;
        mptr->m01 += (iptr->m01 - mptr->m01) >> 2;
        mptr->m02 += (iptr->m02 - mptr->m02) >> 2;
        mptr->m03 += (iptr->m03 - mptr->m03) >> 2;

        mptr->m10 += (iptr->m10 - mptr->m10) >> 2;
        mptr->m11 += (iptr->m11 - mptr->m11) >> 2;
        mptr->m12 += (iptr->m12 - mptr->m12) >> 2;
        mptr->m13 += (iptr->m13 - mptr->m13) >> 2;

        mptr->m20 += (iptr->m20 - mptr->m20) >> 2;
        mptr->m21 += (iptr->m21 - mptr->m21) >> 2;
        mptr->m22 += (iptr->m22 - mptr->m22) >> 2;
        mptr->m23 += (iptr->m23 - mptr->m23) >> 2;
    }
    else
    {
        mptr->m00 = iptr->m00 - ((iptr->m00 - mptr->m00) >> 2);
        mptr->m01 = iptr->m01 - ((iptr->m01 - mptr->m01) >> 2);
        mptr->m02 = iptr->m02 - ((iptr->m02 - mptr->m02) >> 2);
        mptr->m03 = iptr->m03 - ((iptr->m03 - mptr->m03) >> 2);

        mptr->m10 = iptr->m10 - ((iptr->m10 - mptr->m10) >> 2);
        mptr->m11 = iptr->m11 - ((iptr->m11 - mptr->m11) >> 2);
        mptr->m12 = iptr->m12 - ((iptr->m12 - mptr->m12) >> 2);
        mptr->m13 = iptr->m13 - ((iptr->m13 - mptr->m13) >> 2);

        mptr->m20 = iptr->m20 - ((iptr->m20 - mptr->m20) >> 2);
        mptr->m21 = iptr->m21 - ((iptr->m21 - mptr->m21) >> 2);
        mptr->m22 = iptr->m22 - ((iptr->m22 - mptr->m22) >> 2);
        mptr->m23 = iptr->m23 - ((iptr->m23 - mptr->m23) >> 2);
    }
}

void InterpolateArmMatrix(void)
{
    PHD_MATRIX *mptr, *iptr;
    int frac, rate;

    iptr = IM_ptr;
    frac = IM_frac;
    rate = IM_rate;

    // NOTE: the old way not work with PHD_MATRIX struct...
    // -24 is replaced by [-2] with PHD_MATRIX :)
    mptr = phd_mxptr;
    mptr->m00 = mptr[-2].m00;
    mptr->m01 = mptr[-2].m01;
    mptr->m02 = mptr[-2].m02;
    mptr->m10 = mptr[-2].m10;
    mptr->m11 = mptr[-2].m11;
    mptr->m12 = mptr[-2].m12;
    mptr->m20 = mptr[-2].m20;
    mptr->m21 = mptr[-2].m21;
    mptr->m22 = mptr[-2].m22;

    if (rate == 2 || (frac == 2 && rate == 4))                                           // If interpolating Odd Frames
    {                                                                                    // then do simple case
        mptr->m03 = (iptr->m03 + mptr->m03) >> 1;
        mptr->m13 = (iptr->m13 + mptr->m13) >> 1;
        mptr->m23 = (iptr->m23 + mptr->m23) >> 1;
    }
    else if (frac == 1)
    {
        mptr->m03 += (iptr->m03 - mptr->m03) >> 2;
        mptr->m13 += (iptr->m13 - mptr->m13) >> 2;
        mptr->m23 += (iptr->m23 - mptr->m23) >> 2;
    }
    else
    {
        mptr->m03 = iptr->m03 - ((iptr->m03 - mptr->m03) >> 2);
        mptr->m13 = iptr->m13 - ((iptr->m13 - mptr->m13) >> 2);
        mptr->m23 = iptr->m23 - ((iptr->m23 - mptr->m23) >> 2);
    }
}

#ifdef DLL_INJECT
void injector::inject_draw()
{
    //this->inject(0x0044EBA0, DrawPhaseGame);
    //this->inject(0x0044EC10, DrawRooms);
    //this->inject(0x0044F2D0, PrintRooms);
    //this->inject(0x0044F330, PrintObjects);
    //this->inject(0x0044F5D0, GetRoomBounds);
    //this->inject(0x0044F790, SetRoomBounds);
    //this->inject(0x0044FB10, DrawEffect);
    //this->inject(0x0044FC00, DrawMovingItem);
    //this->inject(0x0044FF60, DrawAnimatingItem);
    //this->inject(0x0041D140, DrawLara);
    //this->inject(0x00455800, DrawLara_Mirror);
    this->inject(0x00450520, InitInterpolate);
    this->inject(0x00450560, phd_PopMatrix_I);
    this->inject(0x00450590, phd_PushMatrix_I);
    this->inject(0x004505C0, phd_RotY_I);
    this->inject(0x004505F0, phd_RotX_I);
    this->inject(0x00450620, phd_RotZ_I);
    this->inject(0x00450650, phd_TranslateRel_I);
    this->inject(0x00450690, phd_TranslateRel_ID);
    this->inject(0x004506E0, phd_RotYXZ_I);
    this->inject(0x00450720, gar_RotYXZsuperpack_I);
    this->inject(0x00450760, gar_RotYXZsuperpack);
    this->inject(0x00450810, phd_PutPolygons_I);
    this->inject(0x00450840, InterpolateMatrix);
    this->inject(0x00450AB0, InterpolateArmMatrix);
    //this->inject(0x00450BB0, CalculateObjectLighting);
    //this->inject(0x00450CB0, CalculateObjectLightingLara);
    this->inject(0x00450DC0, GetFrames);
    //this->inject(0x00450E60, GetBoundsAccurate);
    //this->inject(0x00450EE0, GetBestFrame);
}
#endif