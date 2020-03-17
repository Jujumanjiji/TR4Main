#include "framework.h"
#include "draw.h"
#include "3d_gen.h"
#include "3d_gen_a.h"
#include "output.h"
#include "specific.h"
#include "utils.h"

/*
void DrawLara(ITEM_INFO* item, BOOL lightDisabled)
{
    OBJECT_INFO* obj;
    int *bone;
    short *rotation1, *rotation2;
    short *rotationw1, *rotationw2;
    short *rotationWP1, *rotationWP2;
    short *frame[2];
    short weapon;
    int savemptr[12];
    int clip, frac, rate;
    int left;
    int right;
    int top;
    int bottom;

    right = phd_right;
    top = phd_top;
    left = phd_left;
    bottom = phd_bottom;
    phd_top = 0;
    phd_left = 0;
    phd_bottom = phd_winymax;
    phd_right = phd_winxmax;

    if (CHK_ANY(TrInput, IN_LOOK))
    {

    }
    else
    {

    }

    frac = GetFrames(item, frame, &rate);
    obj = &objects[item->object_number];
    if (lara.hit_direction >= 0)
    {
        ANIM_STRUCT* anim;
        short spaz;

        switch (lara.hit_direction)
        {
        case NORTH:
            if (lara.is_ducked)
                spaz = ANIMATION_LARA_CROUCH_SMASH_BACKWARD;
            else
                spaz = ANIMATION_LARA_AH_FORWARD;
            break;
        case SOUTH:
            if (lara.is_ducked)
                spaz = ANIMATION_LARA_CROUCH_SMASH_FORWARD;
            else
                spaz = ANIMATION_LARA_AH_BACKWARD;
            break;
        case EAST:
            if (lara.is_ducked)
                spaz = ANIMATION_LARA_CROUCH_SMASH_RIGHT;
            else
                spaz = ANIMATION_LARA_AH_LEFT;
            break;
        case WEST:
            if (lara.is_ducked)
                spaz = ANIMATION_LARA_CROUCH_SMASH_LEFT;
            else
                spaz = ANIMATION_LARA_AH_RIGHT;
            break;
        }

        anim = &anims[spaz];
        frame[0] = &anim->frame_ptr[lara.hit_frame * (anim->interpolation >> 8)];
    }

    short* bounds = GetBoundsAccurate(item);
    if (lara.skidoo == NO_ITEM)
        S_PrintShadow(obj->shadow_size, bounds, item);

    phd_PushMatrix(); // world
    phd_TranslateAbs(item->pos.x, item->pos.y, item->pos.z);
    phd_RotYXZ(item->pos.y_rot, item->pos.x_rot, item->pos.z_rot);
    clip = S_GetObjectBounds(frame[0]);

    phd_PushMatrix(); // list
    if (!lightDisabled)
        CalculateObjectLightingLara();

    bone = &bones[objects[item->object_number].bone_index];
    rotation1 = (short*)(frame[0] + 9);
    rotation2 = (short*)(frame[1] + 9);

    /// HIPS
    if (frac)
        InitInterpolate(frac, rate);
    phd_SwapTranslateRel(frac, frame[0], frame[1]);
    phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
    phd_SwapPutPolygons(frac, clip, &lara.mesh.hips);

    /// LEFT LEG
    phd_SwapPushMatrix(frac);
    phd_SwapTranslateRel(frac, BONE(bone, MESH_TO_BONE(THIGH_L)));
    phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
    phd_SwapPutPolygons(frac, clip, &lara.mesh.thigh_l);

    phd_SwapTranslateRel(frac, BONE(bone, MESH_TO_BONE(CALF_L)));
    phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
    phd_SwapPutPolygons(frac, clip, &lara.mesh.calf_l);

    phd_SwapTranslateRel(frac, BONE(bone, MESH_TO_BONE(FOOT_L)));
    phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
    phd_SwapPutPolygons(frac, clip, &lara.mesh.foot_l);
    phd_SwapPopMatrix(frac);

    /// RIGHT LEG
    phd_SwapPushMatrix(frac);
    phd_SwapTranslateRel(frac, BONE(bone, MESH_TO_BONE(THIGH_R)));
    phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
    phd_SwapPutPolygons(frac, clip, &lara.mesh.thigh_r);

    phd_SwapTranslateRel(frac, BONE(bone, MESH_TO_BONE(CALF_R)));
    phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
    phd_SwapPutPolygons(frac, clip, &lara.mesh.calf_r);

    phd_SwapTranslateRel(frac, BONE(bone, MESH_TO_BONE(FOOT_R)));
    phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
    phd_SwapPutPolygons(frac, clip, &lara.mesh.foot_r);
    phd_SwapPopMatrix(frac);

    /// TORSO
    phd_SwapTranslateRel(frac, BONE(bone, MESH_TO_BONE(TORSO)));
    phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
    phd_SwapRotYXZ(frac, lara.torso_y_rot, lara.torso_x_rot, lara.torso_z_rot);
    phd_SwapPutPolygons(frac, clip, &lara.mesh.torso);

    /// HEAD
    phd_SwapPushMatrix(frac);
    phd_SwapTranslateRel(frac, BONE(bone, MESH_TO_BONE(HEAD)));
    rotationw1 = rotation1;
    rotationw2 = rotation2;
    phd_SwapGarYXZsuperpack(frac, &rotationw1, &rotationw2, 6);
    phd_SwapRotYXZ(frac, lara.head_y_rot, lara.head_x_rot, lara.head_z_rot);
    phd_SwapPutPolygons(frac, clip, &lara.mesh.head);
    //DrawHair();
    phd_SwapPopMatrix(frac);

    /// WEAPON
    weapon = LG_UNARMED;
    if (lara.gun_status == LHS_READY       	// if Guns are Away dont use Animations...
    ||  lara.gun_status == LHS_SPECIAL
    ||  lara.gun_status == LHS_DRAW
    ||  lara.gun_status == LHS_UNDRAW)
    {
        weapon = lara.gun_type;
    }

    switch (weapon)
    {
        case LG_UNARMED:
        case LG_FLARE:
        case LG_TORCH:
            /// RIGHT ARM
            phd_SwapPushMatrix(frac);
            phd_SwapTranslateRel(frac, BONE(bone, MESH_TO_BONE(UARM_R)));
            phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
            if (frac)
                InterpolateMatrix();
            phd_SwapPutPolygons(frac, clip, &lara.mesh.uarm_r);
            phd_SwapTranslateRel(frac, BONE(bone, MESH_TO_BONE(LARM_R)));
            phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
            if (frac)
                InterpolateMatrix();
            phd_SwapPutPolygons(frac, clip, &lara.mesh.larm_r);
            phd_SwapTranslateRel(frac, BONE(bone, MESH_TO_BONE(HAND_R)));
            phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
            if (frac)
                InterpolateMatrix();
            phd_SwapPutPolygons(frac, clip, &lara.mesh.hand_r);
            phd_SwapPopMatrix(frac);

            /// LEFT ARM
            phd_SwapPushMatrix(frac);
            phd_SwapTranslateRel(frac, BONE(bone, MESH_TO_BONE(UARM_L)));
            
            if (lara.flare_control_left)
            {
                rotation1 = rotation2 = lara.l_arm.frame_base + (lara.l_arm.frame_curr - anims[lara.l_arm.anim_curr].frame_base) * (anims[lara.l_arm.anim_curr].interpolation >> 8) + 9;
                phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2, 11);
            }
            else
            {
                phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2, 0);
            }
            if (frac)
                InterpolateMatrix();

            phd_SwapPutPolygons(frac, clip, &lara.mesh.uarm_l);
            phd_SwapTranslateRel(frac, BONE(bone, MESH_TO_BONE(LARM_L)));
            phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
            if (frac)
                InterpolateMatrix();
            phd_SwapPutPolygons(frac, clip, &lara.mesh.larm_l);
            phd_SwapTranslateRel(frac, BONE(bone, MESH_TO_BONE(HAND_L)));
            phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
            if (frac)
                InterpolateMatrix();
            phd_SwapPutPolygons(frac, clip, &lara.mesh.hand_l);
            phd_SwapPopMatrix(frac);
            break;

        case LG_PISTOLS:
        case LG_UZIS:
            /// RIGHT ARM
            phd_SwapPushMatrix(frac);
            phd_SwapTranslateRel(frac, BONE(bone, MESH_TO_BONE(UARM_R)));
            
            if (frac)
            {
                InterpolateArmMatrix();
            }
            else
            {
                phd_mxptr[0] = *(phd_mxptr - 24);
                phd_mxptr[1] = *(phd_mxptr - 23);
                phd_mxptr[2] = *(phd_mxptr - 22);
                phd_mxptr[4] = *(phd_mxptr - 20);
                phd_mxptr[5] = *(phd_mxptr - 19);
                phd_mxptr[6] = *(phd_mxptr - 18);
                phd_mxptr[8] = *(phd_mxptr - 16);
                phd_mxptr[9] = *(phd_mxptr - 15);
                phd_mxptr[10] = *(phd_mxptr - 14);
            }

            phd_SwapRotYXZ(frac, lara.r_arm.y_rot, lara.r_arm.x_rot, lara.r_arm.z_rot);
            rotation1 = rotation2 = lara.r_arm.frame_base + (lara.r_arm.frame_curr - anims[lara.r_arm.anim_curr].frame_base) * (anims[lara.r_arm.anim_curr].interpolation >> 8) + 9;
            phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2, UARM_R);
            phd_SwapPutPolygons(frac, clip, &lara.mesh.uarm_r);
            phd_SwapTranslateRel(frac, BONE(bone, MESH_TO_BONE(LARM_R)));
            phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
            phd_SwapPutPolygons(frac, clip, &lara.mesh.larm_r);
            phd_SwapTranslateRel(frac, BONE(bone, MESH_TO_BONE(HAND_R)));
            phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
            phd_SwapPutPolygons(frac, clip, &lara.mesh.hand_r);
            phd_SwapPopMatrix(frac);

            /// LEFT ARM
            phd_SwapPushMatrix(frac);
            phd_SwapTranslateRel(frac, BONE(bone, MESH_TO_BONE(UARM_L)));

            if (frac)
            {
                InterpolateArmMatrix();
            }
            else
            {
                phd_mxptr[0] = *(phd_mxptr - 24);
                phd_mxptr[1] = *(phd_mxptr - 23);
                phd_mxptr[2] = *(phd_mxptr - 22);
                phd_mxptr[4] = *(phd_mxptr - 20);
                phd_mxptr[5] = *(phd_mxptr - 19);
                phd_mxptr[6] = *(phd_mxptr - 18);
                phd_mxptr[8] = *(phd_mxptr - 16);
                phd_mxptr[9] = *(phd_mxptr - 15);
                phd_mxptr[10] = *(phd_mxptr - 14);
            }

            phd_SwapRotYXZ(frac, lara.l_arm.y_rot, lara.l_arm.x_rot, lara.l_arm.z_rot);
            rotation1 = rotation2 = lara.l_arm.frame_base + (lara.l_arm.frame_curr - anims[lara.l_arm.anim_curr].frame_base) * (anims[lara.l_arm.anim_curr].interpolation >> 8) + 9;
            phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2, UARM_L);
            phd_SwapPutPolygons(frac, clip, &lara.mesh.uarm_l);
            phd_SwapTranslateRel(frac, BONE(bone, MESH_TO_BONE(LARM_L)));
            phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
            phd_SwapPutPolygons(frac, clip, &lara.mesh.larm_l);
            phd_SwapTranslateRel(frac, BONE(bone, MESH_TO_BONE(HAND_L)));
            phd_SwapGarYXZsuperpack(frac, &rotation1, &rotation2);
            phd_SwapPutPolygons(frac, clip, &lara.mesh.hand_l);
            phd_SwapPopMatrix(frac);
            break;

        case LG_REVOLVER:

            break;

        case LG_SHOTGUN:
        case LG_GRENADEGUN:
        case LG_CROSSBOW:

            break;
    }

    phd_PopMatrix(); // !list
    phd_PopMatrix(); // !world

    phd_left = left;
    phd_right = right;
    phd_top = top;
    phd_bottom = bottom;
    //item_color = 0xFF000000;
}

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
}*/

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
    //this->inject(0x00450DC0, GetFrames);
    //this->inject(0x00450E60, GetBoundsAccurate);
    //this->inject(0x00450EE0, GetBestFrame);
}
#endif