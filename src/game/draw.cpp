#include "framework.h"
#include "draw.h"

int GetFrames(ITEM_INFO* item, short* frame[2], int* rate)
{
    ANIM_STRUCT *anim;
    int frm;
    int first, second;
    int frame_size;
    int interp, rat;

    frm = item->current_frame;
    anim = &anims[item->current_anim];
    frame[0] = anim->frame_ptr;
    frame[1] = anim->frame_ptr;
    rat = *rate = LOBYTE(anim->interpolation);
    frame_size = anim->interpolation >> 8;
    frm -= anim->frame_base;
    first = frm / rat;
    interp = frm % rat;
    frame[0] += first * frame_size;				  // Get Frame pointers
    frame[1] = frame[0] + frame_size;               // and store away
    if (interp == 0)
        return(0);
    second = first * rat + rat;
    if (second > anim->frame_end)                       // Clamp KeyFrame to End if need be
        *rate = anim->frame_end - (second - rat);
    return(interp);
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
    //this->inject(0x00450AB0, InterpolateArmMatrix);
    //this->inject(0x00450BB0, CalculateObjectLighting);
    //this->inject(0x00450CB0, CalculateObjectLightingLara);
    this->inject(ADDRESS_STRUCT(0x00450DC0, GetFrames));
    //this->inject(ADDRESS_STRUCT(0x00450E60, GetBoundsAccurate));
    //this->inject(0x00450EE0, GetBestFrame);
}
#endif
