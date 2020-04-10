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

    frm = item->current_frame;
    anim = &anims[item->current_anim];
    frame[0] = frame[1] = anim->frame_ptr;
    interl = LOBYTE(anim->interpolation);
    rat = interl;
    *rate = interl;
    frame_size = anim->interpolation >> 8;
    frm -= anim->frame_base;
    first = frm / rat;
    interp = frm % rat;
    frame[0] += first * frame_size;				  // Get Frame pointers
    frame[1] = frame[0] + frame_size;               // and store away
    if (interp == 0)
        return 0;
    second = first * rat + rat;
    if (second > anim->frame_end)                       // Clamp KeyFrame to End if need be
        *rate = anim->frame_end - (second - rat);
    return interp;
}

void InterpolateArmMatrix(void)
{
    int *mptr, *iptr;
    int frac, rate;

    mptr = phd_mxptr;
    iptr = IM_ptr;
    frac = IM_frac;
    rate = IM_rate;

    *(mptr + M00) = *(mptr + M00 - 24);
    *(mptr + M01) = *(mptr + M01 - 24);
    *(mptr + M02) = *(mptr + M02 - 24);
    *(mptr + M10) = *(mptr + M10 - 24);
    *(mptr + M11) = *(mptr + M11 - 24);
    *(mptr + M12) = *(mptr + M12 - 24);
    *(mptr + M20) = *(mptr + M20 - 24);
    *(mptr + M21) = *(mptr + M21 - 24);
    *(mptr + M22) = *(mptr + M22 - 24);

    if (rate == 2 || (frac == 2 && rate == 4))                                           // If interpolating Odd Frames
    {                                                                                    // then do simple case
        *(mptr + M03) = (*(iptr + M03) + *(mptr + M03)) >> 1;
        *(mptr + M13) = (*(iptr + M13) + *(mptr + M13)) >> 1;
        *(mptr + M23) = (*(iptr + M23) + *(mptr + M23)) >> 1;
    }
    else if (frac == 1)
    {
        *(mptr + M03) += (*(iptr + M03) - *(mptr + M03)) >> 2;
        *(mptr + M13) += (*(iptr + M13) - *(mptr + M13)) >> 2;
        *(mptr + M23) += (*(iptr + M23) - *(mptr + M23)) >> 2;
    }
    else
    {
        *(mptr + M03) = *(iptr + M03) - ((*(iptr + M03) - *(mptr + M03)) >> 2);
        *(mptr + M13) = *(iptr + M13) - ((*(iptr + M13) - *(mptr + M13)) >> 2);
        *(mptr + M23) = *(iptr + M23) - ((*(iptr + M23) - *(mptr + M23)) >> 2);
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
    //this->inject(ADDRESS_STRUCT(0x0041D140, DrawLara));
    //this->inject(0x00455800, DrawLara_Mirror);
    //this->inject(0x00450520, InitInterpolate);
    //this->inject(0x00450560, phd_PopMatrix_I);
    //this->inject(0x00450590, phd_PushMatrix_I);
    //this->inject(0x004505C0, phd_RotY_I);
    //this->inject(0x004505F0, phd_RotX_I);
    //this->inject(0x00450620, phd_RotZ_I);
    //this->inject(0x00450650, phd_TranslateRel_I);
    //this->inject(0x00450690, phd_TranslateRel_ID);
    //this->inject(0x004506E0, phd_RotYXZ_I);
    //this->inject(0x00450720, gar_RotYXZsuperpack_I);
    //this->inject(0x00450760, gar_RotYXZsuperpack);
    //this->inject(0x00450810, phd_PutPolygons_I);
    //this->inject(0x00450840, InterpolateMatrix);
    this->inject(0x00450AB0, InterpolateArmMatrix);
    //this->inject(0x00450BB0, CalculateObjectLighting);
    //this->inject(0x00450CB0, CalculateObjectLightingLara);
    this->inject(0x00450DC0, GetFrames);
    //this->inject(0x00450E60, GetBoundsAccurate);
    //this->inject(0x00450EE0, GetBestFrame);
}
#endif