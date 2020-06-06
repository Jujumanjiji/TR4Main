#include "framework.h"
#include "lara_render.h"
#include "3d_gen.h"
#include "3d_gen_a.h"
#include "draw.h"
#include "specific.h"
#include "utils.h"

LARA_RENDER lara_render;
void LARA_RENDER::enable_cutscene(bool value)
{
    this->cutsceneMode = value;
}

void LARA_RENDER::assign_pprot(short pprot)
{
    this->pprot = pprot;
}

void LARA_RENDER::assign_matrixptr()
{
    if (pprot != 1)
        mptr = &lara_matrix_holsters;
    else
        mptr = &lara_matrix_normal;
}

void LARA_RENDER::create_item()
{
    item = LaraItem;
}

void LARA_RENDER::create_object()
{
    obj = &Objects[item->objectNumber];
}

void LARA_RENDER::create_bone(BONE_STRUCT* boneIfCutscene)
{
    if (cutsceneMode)
        bone = boneIfCutscene;
    else
        bone = (BONE_STRUCT*)&Bones[obj->boneIndex];
}

void LARA_RENDER::create_frame()
{
    if (!cutsceneMode)
        frac = GetFrames(item, frmptr, &rate);
    else
        frac = NULL;
}

void LARA_RENDER::check_hitdirection(short* frameCutscene)
{
    if (cutsceneMode)
    {
        frame = frameCutscene;
    }
    else
    {
        if (Lara.hitDirection >= 0)
        {
            short spaz;
            switch (Lara.hitDirection)
            {
                case NORTH:
                    spaz = Lara.isDucked ? ANIMATION_LARA_CROUCH_SMASH_BACKWARD : ANIMATION_LARA_AH_FORWARD;
                    break;
                case SOUTH:
                    spaz = Lara.isDucked ? ANIMATION_LARA_CROUCH_SMASH_FORWARD : ANIMATION_LARA_AH_BACKWARD;
                    break;
                case EAST:
                    spaz = Lara.isDucked ? ANIMATION_LARA_CROUCH_SMASH_RIGHT : ANIMATION_LARA_AH_LEFT;
                    break;
                case WEST:
                    spaz = Lara.isDucked ? ANIMATION_LARA_CROUCH_SMASH_LEFT : ANIMATION_LARA_AH_RIGHT;
                    break;
            }
            frame = &Anims[spaz].framePtr[Lara.hitFrame * (Anims[spaz].interpolation >> 8)];
        }
        else
        {
            frame = frmptr[0];
        }
    }
}

void LARA_RENDER::create_rotation(short* frameCutscene)
{
    if (cutsceneMode)
    {
        rotation1 = frameCutscene + 9;
    }
    else
    {
        rotation1 = frame + 9;
        rotation2 = frmptr[1] + 9;
    }
}

void LARA_RENDER::create_shadow(short* frameCutscene)
{
    if (cutsceneMode)
        lara_shadow_bbox = frameCutscene;
    else if (frac)
        lara_shadow_bbox = GetBoundsAccurate(item);
    else
        lara_shadow_bbox = frame;
}

void LARA_RENDER::assign_weapon()
{
    gun_type = LG_UNARMED;

    if (Lara.gunStatus == LHS_READY
    ||  Lara.gunStatus == LHS_SPECIAL
    ||  Lara.gunStatus == LHS_DRAW
    ||  Lara.gunStatus == LHS_UNDRAW)
    {
        gun_type = Lara.gunType;
    }
}

void LARA_RENDER::start_world()
{
    phd_PushMatrix();

    if (pprot != 0 && pprot != 2)
    {
        phd_mxptr->m03 = 0;
        phd_mxptr->m13 = 0;
        phd_mxptr->m23 = 0;
    }
    else
    {
        phd_TranslateAbs(item->pos.xPos, item->pos.yPos, item->pos.zPos);
    }

    phd_RotYXZ(item->pos.yRot, item->pos.xRot, item->pos.zRot);
    if (pprot == 2)
    {
        PHD_VECTOR shift_matrix;
        shift_matrix.z = -0x4000;
        shift_matrix.y = -0x4000;
        shift_matrix.x = -0x4000;
        ScaleCurrentMatrix(&shift_matrix);
    }
}

void LARA_RENDER::start_list()
{
    phd_PushMatrix();

    if (frac)
        InitInterpolate(frac, rate);
}

void LARA_RENDER::mesh_hips(short* frameCutscene)
{
    if (cutsceneMode)
    {
        phd_TranslateRel(frameCutscene[6], frameCutscene[7], frameCutscene[8]);
        gar_RotYXZsuperpack(&rotation1, 0);
        phd_PutMatrix(&mptr->hips);
    }
    else if (frac)
    {
        phd_TranslateRel_ID((int)*(frame + 6), (int)*(frame + 7), (int)*(frame + 8), (int)*(frmptr[1] + 6), (int)*(frmptr[1] + 7), (int)*(frmptr[1] + 8));
        gar_RotYXZsuperpack_I(&rotation1, &rotation2, 0);
        phd_PutMatrix_I(&mptr->hips);
    }
    else
    {
        phd_TranslateRel((int)*(frame + 6), (int)*(frame + 7), (int)*(frame + 8));
        gar_RotYXZsuperpack(&rotation1, 0);
        phd_PutMatrix(&mptr->hips);
    }
}

void LARA_RENDER::mesh_thight_l()
{
    if (frac)
    {
        phd_PushMatrix_I();
        phd_TranslateRel_I(bone[MTB(THIGH_L)].x, bone[MTB(THIGH_L)].y, bone[MTB(THIGH_L)].z);
        gar_RotYXZsuperpack_I(&rotation1, &rotation2, 0);
        phd_PutMatrix_I(&mptr->thigh_l);
    }
    else
    {
        phd_PushMatrix();
        phd_TranslateRel(bone[MTB(THIGH_L)].x, bone[MTB(THIGH_L)].y, bone[MTB(THIGH_L)].z);
        gar_RotYXZsuperpack(&rotation1, 0);
        phd_PutMatrix(&mptr->thigh_l);
    }
}

void LARA_RENDER::mesh_calf_l()
{
    if (frac)
    {
        phd_TranslateRel_I(bone[MTB(CALF_L)].x, bone[MTB(CALF_L)].y, bone[MTB(CALF_L)].z);
        gar_RotYXZsuperpack_I(&rotation1, &rotation2, 0);
        phd_PutMatrix_I(&mptr->calf_l);
    }
    else
    {
        phd_TranslateRel(bone[MTB(CALF_L)].x, bone[MTB(CALF_L)].y, bone[MTB(CALF_L)].z);
        gar_RotYXZsuperpack(&rotation1, 0);
        phd_PutMatrix(&mptr->calf_l);
    }
}

void LARA_RENDER::mesh_foot_l()
{
    if (frac)
    {
        phd_TranslateRel_I(bone[MTB(FOOT_L)].x, bone[MTB(FOOT_L)].y, bone[MTB(FOOT_L)].z);
        gar_RotYXZsuperpack_I(&rotation1, &rotation2, 0);
        phd_PutMatrix_I(&mptr->foot_l);
        phd_PopMatrix_I();
    }
    else
    {
        phd_TranslateRel(bone[MTB(FOOT_L)].x, bone[MTB(FOOT_L)].y, bone[MTB(FOOT_L)].z);
        gar_RotYXZsuperpack(&rotation1, 0);
        phd_PutMatrix(&mptr->foot_l);
        phd_PopMatrix();
    }
}

void LARA_RENDER::mesh_thight_r()
{
    if (frac)
    {
        phd_PushMatrix_I();
        phd_TranslateRel_I(bone[MTB(THIGH_R)].x, bone[MTB(THIGH_R)].y, bone[MTB(THIGH_R)].z);
        gar_RotYXZsuperpack_I(&rotation1, &rotation2, 0);
        phd_PutMatrix_I(&mptr->thigh_r);
    }
    else
    {
        phd_PushMatrix();
        phd_TranslateRel(bone[MTB(THIGH_R)].x, bone[MTB(THIGH_R)].y, bone[MTB(THIGH_R)].z);
        gar_RotYXZsuperpack(&rotation1, 0);
        phd_PutMatrix(&mptr->thigh_r);
    }
}

void LARA_RENDER::mesh_calf_r()
{
    if (frac)
    {
        phd_TranslateRel_I(bone[MTB(CALF_R)].x, bone[MTB(CALF_R)].y, bone[MTB(CALF_R)].z);
        gar_RotYXZsuperpack_I(&rotation1, &rotation2, 0);
        phd_PutMatrix_I(&mptr->calf_r);
    }
    else
    {
        phd_TranslateRel(bone[MTB(CALF_R)].x, bone[MTB(CALF_R)].y, bone[MTB(CALF_R)].z);
        gar_RotYXZsuperpack(&rotation1, 0);
        phd_PutMatrix(&mptr->calf_r);
    }
}

void LARA_RENDER::mesh_foot_r()
{
    if (frac)
    {
        phd_TranslateRel_I(bone[MTB(FOOT_R)].x, bone[MTB(FOOT_R)].y, bone[MTB(FOOT_R)].z);
        gar_RotYXZsuperpack_I(&rotation1, &rotation2, 0);
        phd_PutMatrix_I(&mptr->foot_r);
        phd_PopMatrix_I();
    }
    else
    {
        phd_TranslateRel(bone[MTB(FOOT_R)].x, bone[MTB(FOOT_R)].y, bone[MTB(FOOT_R)].z);
        gar_RotYXZsuperpack(&rotation1, 0);
        phd_PutMatrix(&mptr->foot_r);
        phd_PopMatrix();
    }
}

void LARA_RENDER::mesh_torso()
{
    if (frac)
    {
        phd_TranslateRel_I(bone[MTB(TORSO)].x, bone[MTB(TORSO)].y, bone[MTB(TORSO)].z);
        gar_RotYXZsuperpack_I(&rotation1, &rotation2, 0);
        phd_RotYXZ_I(Lara.torsoYrot, Lara.torsoXrot, Lara.torsoZrot);
        phd_PutMatrix_I(&mptr->torso);
    }
    else
    {
        phd_TranslateRel(bone[MTB(TORSO)].x, bone[MTB(TORSO)].y, bone[MTB(TORSO)].z);
        gar_RotYXZsuperpack(&rotation1, 0);
        phd_RotYXZ(Lara.torsoYrot, Lara.torsoXrot, Lara.torsoZrot);
        phd_PutMatrix(&mptr->torso);
    }
}

void LARA_RENDER::mesh_head()
{
    if (frac)
    {
        phd_PushMatrix_I();
        phd_TranslateRel_I(bone[MTB(HEAD)].x, bone[MTB(HEAD)].y, bone[MTB(HEAD)].z);
        rotationw1 = rotation1;
        rotationw2 = rotation2;
        gar_RotYXZsuperpack_I(&rotationw1, &rotationw2, 6);
        phd_RotYXZ_I(Lara.headYrot, Lara.headXrot, Lara.headZrot);
        phd_PutMatrix_I(&mptr->head);
        phd_PopMatrix_I();
    }
    else
    {
        phd_PushMatrix();
        phd_TranslateRel(bone[MTB(HEAD)].x, bone[MTB(HEAD)].y, bone[MTB(HEAD)].z);
        rotationw1 = rotation1;
        gar_RotYXZsuperpack(&rotationw1, 6);
        phd_RotYXZ(Lara.headYrot, Lara.headXrot, Lara.headZrot);
        phd_PutMatrix(&mptr->head);
        phd_PopMatrix();
    }
}

void LARA_RENDER::mesh_unarmed()
{
    if (gun_type == LG_UNARMED || gun_type == LG_FLARE || gun_type == LG_TORCH)
    {
        if (frac)
        {
            /// RIGHT_ARM
            phd_PushMatrix_I();
            phd_TranslateRel_I(bone[MTB(UARM_R)].x, bone[MTB(UARM_R)].y, bone[MTB(UARM_R)].z);
            gar_RotYXZsuperpack_I(&rotation1, &rotation2, 0);
            phd_PutMatrix_I(&mptr->uarm_r);
            phd_TranslateRel_I(bone[MTB(LARM_R)].x, bone[MTB(LARM_R)].y, bone[MTB(LARM_R)].z);
            gar_RotYXZsuperpack_I(&rotation1, &rotation2, 0);
            phd_PutMatrix_I(&mptr->larm_r);
            phd_TranslateRel_I(bone[MTB(HAND_R)].x, bone[MTB(HAND_R)].y, bone[MTB(HAND_R)].z);
            gar_RotYXZsuperpack_I(&rotation1, &rotation2, 0);
            phd_PutMatrix_I(&mptr->hand_r);
            phd_PopMatrix_I();

            /// LEFT_ARM
            phd_PushMatrix_I();
            phd_TranslateRel_I(bone[MTB(UARM_L)].x, bone[MTB(UARM_L)].y, bone[MTB(UARM_L)].z);

            if (Lara.flareControlLeft)
            {
                rotation1 = rotation2 = Lara.leftArm.frameBase + (Lara.leftArm.frameNumber - Anims[Lara.leftArm.animNumber].frameBase) * (Anims[Lara.leftArm.animNumber].interpolation >> 8) + 9;
                gar_RotYXZsuperpack_I(&rotation1, &rotation2, 11);
            }
            else
            {
                gar_RotYXZsuperpack_I(&rotation1, &rotation2, 0);
            }

            phd_PutMatrix_I(&mptr->uarm_l);
            phd_TranslateRel_I(bone[MTB(LARM_L)].x, bone[MTB(LARM_L)].y, bone[MTB(LARM_L)].z);
            gar_RotYXZsuperpack_I(&rotation1, &rotation2, 0);
            phd_PutMatrix_I(&mptr->larm_l);
            phd_TranslateRel_I(bone[MTB(HAND_L)].x, bone[MTB(HAND_L)].y, bone[MTB(HAND_L)].z);
            gar_RotYXZsuperpack_I(&rotation1, &rotation2, 0);
            phd_PutMatrix_I(&mptr->hand_l);
            phd_PopMatrix_I();
        }
        else
        {
            /// RIGHT_ARM
            phd_PushMatrix();
            phd_TranslateRel(bone[MTB(UARM_R)].x, bone[MTB(UARM_R)].y, bone[MTB(UARM_R)].z);
            gar_RotYXZsuperpack(&rotation1, 0);
            phd_PutMatrix(&mptr->uarm_r);
            phd_TranslateRel(bone[MTB(LARM_R)].x, bone[MTB(LARM_R)].y, bone[MTB(LARM_R)].z);
            gar_RotYXZsuperpack(&rotation1, 0);
            phd_PutMatrix(&mptr->larm_r);
            phd_TranslateRel(bone[MTB(HAND_R)].x, bone[MTB(HAND_R)].y, bone[MTB(HAND_R)].z);
            gar_RotYXZsuperpack(&rotation1, 0);
            phd_PutMatrix(&mptr->hand_r);
            phd_PopMatrix();

            /// LEFT_ARM
            phd_PushMatrix();
            phd_TranslateRel(bone[MTB(UARM_L)].x, bone[MTB(UARM_L)].y, bone[MTB(UARM_L)].z);

            if (Lara.flareControlLeft)
            {
                rotation1 = Lara.leftArm.frameBase + (Lara.leftArm.frameNumber - Anims[Lara.leftArm.animNumber].frameBase) * (Anims[Lara.leftArm.animNumber].interpolation >> 8) + 9;
                gar_RotYXZsuperpack(&rotation1, 11);
            }
            else
            {
                gar_RotYXZsuperpack(&rotation1, 0);
            }

            phd_PutMatrix(&mptr->uarm_l);
            phd_TranslateRel(bone[MTB(LARM_L)].x, bone[MTB(LARM_L)].y, bone[MTB(LARM_L)].z);
            gar_RotYXZsuperpack(&rotation1, 0);
            phd_PutMatrix(&mptr->larm_l);
            phd_TranslateRel(bone[MTB(HAND_L)].x, bone[MTB(HAND_L)].y, bone[MTB(HAND_L)].z);
            gar_RotYXZsuperpack(&rotation1, 0);
            phd_PutMatrix(&mptr->hand_l);
            phd_PopMatrix();
        }
    }
}

void LARA_RENDER::mesh_1gun()
{
    if (gun_type == LG_SHOTGUN
    ||  gun_type == LG_GRENADEGUN
    ||  gun_type == LG_CROSSBOW)
    {
        if (frac)
        {
            /// RIGHT_ARM
            phd_PushMatrix_I();
            phd_TranslateRel_I(bone[MTB(UARM_R)].x, bone[MTB(UARM_R)].y, bone[MTB(UARM_R)].z);
            rotation1 = rotation2 = Lara.rightArm.frameBase + Lara.rightArm.frameNumber * (Anims[Lara.rightArm.animNumber].interpolation >> 8) + 9;
            gar_RotYXZsuperpack_I(&rotation1, &rotation2, 8);
            phd_PutMatrix_I(&mptr->uarm_r);
            phd_TranslateRel_I(bone[MTB(LARM_R)].x, bone[MTB(LARM_R)].y, bone[MTB(LARM_R)].z);
            gar_RotYXZsuperpack_I(&rotation1, &rotation2, 0);
            phd_PutMatrix_I(&mptr->larm_r);
            phd_TranslateRel_I(bone[MTB(HAND_R)].x, bone[MTB(HAND_R)].y, bone[MTB(HAND_R)].z);
            gar_RotYXZsuperpack_I(&rotation1, &rotation2, 0);
            phd_PutMatrix_I(&mptr->hand_r);
            phd_PopMatrix_I();

            /// LEFT_ARM
            phd_PushMatrix_I();
            phd_TranslateRel_I(bone[MTB(UARM_L)].x, bone[MTB(UARM_L)].y, bone[MTB(UARM_L)].z);
            gar_RotYXZsuperpack_I(&rotation1, &rotation2, 0);
            phd_PutMatrix_I(&mptr->uarm_l);
            phd_TranslateRel_I(bone[MTB(LARM_L)].x, bone[MTB(LARM_L)].y, bone[MTB(LARM_L)].z);
            gar_RotYXZsuperpack_I(&rotation1, &rotation2, 0);
            phd_PutMatrix_I(&mptr->larm_l);
            phd_TranslateRel_I(bone[MTB(HAND_L)].x, bone[MTB(HAND_L)].y, bone[MTB(HAND_L)].z);
            gar_RotYXZsuperpack_I(&rotation1, &rotation2, 0);
            phd_PutMatrix_I(&mptr->hand_l);
            phd_PopMatrix_I();
        }
        else
        {
            /// RIGHT_ARM
            phd_PushMatrix();
            phd_TranslateRel(bone[MTB(UARM_R)].x, bone[MTB(UARM_R)].y, bone[MTB(UARM_R)].z);
            rotation1 = Lara.rightArm.frameBase + Lara.rightArm.frameNumber * (Anims[Lara.rightArm.animNumber].interpolation >> 8) + 9;
            gar_RotYXZsuperpack(&rotation1, 8);
            phd_PutMatrix(&mptr->uarm_r);
            phd_TranslateRel(bone[MTB(LARM_R)].x, bone[MTB(LARM_R)].y, bone[MTB(LARM_R)].z);
            gar_RotYXZsuperpack(&rotation1, 0);
            phd_PutMatrix(&mptr->larm_r);
            phd_TranslateRel(bone[MTB(HAND_R)].x, bone[MTB(HAND_R)].y, bone[MTB(HAND_R)].z);
            gar_RotYXZsuperpack(&rotation1, 0);
            phd_PutMatrix(&mptr->hand_r);
            phd_PopMatrix();

            /// LEFT_ARM
            phd_PushMatrix();
            phd_TranslateRel(bone[MTB(UARM_L)].x, bone[MTB(UARM_L)].y, bone[MTB(UARM_L)].z);
            gar_RotYXZsuperpack(&rotation1, 0);
            phd_PutMatrix(&mptr->uarm_l);
            phd_TranslateRel(bone[MTB(LARM_L)].x, bone[MTB(LARM_L)].y, bone[MTB(LARM_L)].z);
            gar_RotYXZsuperpack(&rotation1, 0);
            phd_PutMatrix(&mptr->larm_l);
            phd_TranslateRel(bone[MTB(HAND_L)].x, bone[MTB(HAND_L)].y, bone[MTB(HAND_L)].z);
            gar_RotYXZsuperpack(&rotation1, 0);
            phd_PutMatrix(&mptr->hand_l);
            phd_PopMatrix();
        }
    }
}

void LARA_RENDER::mesh_2gun()
{
    if (gun_type == LG_PISTOLS
    ||  gun_type == LG_UZIS)
    {
        if (frac)
        {
            /// RIGHT_ARM
            phd_PushMatrix_I();
            phd_TranslateRel_I(bone[MTB(UARM_R)].x, bone[MTB(UARM_R)].y, bone[MTB(UARM_R)].z);
            InterpolateArmMatrix();
            phd_RotYXZ(Lara.rightArm.yRot, Lara.rightArm.xRot, Lara.rightArm.zRot);
            rotation1 = Lara.rightArm.frameBase + (Lara.rightArm.frameNumber - Anims[Lara.rightArm.animNumber].frameBase) * (Anims[Lara.rightArm.animNumber].interpolation >> 8) + 9;
            gar_RotYXZsuperpack(&rotation1, 8);
            phd_PutMatrix(&mptr->uarm_r);
            phd_TranslateRel(bone[MTB(LARM_R)].x, bone[MTB(LARM_R)].y, bone[MTB(LARM_R)].z);
            gar_RotYXZsuperpack(&rotation1, 0);
            phd_PutMatrix(&mptr->larm_r);
            phd_TranslateRel(bone[MTB(HAND_R)].x, bone[MTB(HAND_R)].y, bone[MTB(HAND_R)].z);
            gar_RotYXZsuperpack(&rotation1, 0);
            phd_PutMatrix(&mptr->hand_r);
            phd_PopMatrix_I();

            /// LEFT_ARM
            phd_PushMatrix_I();
            phd_TranslateRel_I(bone[MTB(UARM_L)].x, bone[MTB(UARM_L)].y, bone[MTB(UARM_L)].z);
            InterpolateArmMatrix();
            phd_RotYXZ(Lara.leftArm.yRot, Lara.leftArm.xRot, Lara.leftArm.zRot);
            rotation1 = Lara.leftArm.frameBase + (Lara.leftArm.frameNumber - Anims[Lara.leftArm.animNumber].frameBase) * (Anims[Lara.leftArm.animNumber].interpolation >> 8) + 9;
            gar_RotYXZsuperpack(&rotation1, 11);
            phd_PutMatrix(&mptr->uarm_l);
            phd_TranslateRel(bone[MTB(LARM_L)].x, bone[MTB(LARM_L)].y, bone[MTB(LARM_L)].z);
            gar_RotYXZsuperpack(&rotation1, 0);
            phd_PutMatrix(&mptr->larm_l);
            phd_TranslateRel(bone[MTB(HAND_L)].x, bone[MTB(HAND_L)].y, bone[MTB(HAND_L)].z);
            gar_RotYXZsuperpack(&rotation1, 0);
            phd_PutMatrix(&mptr->hand_l);
            phd_PopMatrix_I();
        }
        else
        {
            /// RIGHT_ARM
            phd_PushMatrix();
            phd_TranslateRel(bone[MTB(UARM_R)].x, bone[MTB(UARM_R)].y, bone[MTB(UARM_R)].z);
            InterpolateArm();
            phd_RotYXZ(Lara.rightArm.yRot, Lara.rightArm.xRot, Lara.rightArm.zRot);
            rotation1 = Lara.rightArm.frameBase + (Lara.rightArm.frameNumber - Anims[Lara.rightArm.animNumber].frameBase) * (Anims[Lara.rightArm.animNumber].interpolation >> 8) + 9;
            gar_RotYXZsuperpack(&rotation1, 8);
            phd_PutMatrix(&mptr->uarm_r);
            phd_TranslateRel(bone[MTB(LARM_R)].x, bone[MTB(LARM_R)].y, bone[MTB(LARM_R)].z);
            gar_RotYXZsuperpack(&rotation1, 0);
            phd_PutMatrix(&mptr->larm_r);
            phd_TranslateRel(bone[MTB(HAND_R)].x, bone[MTB(HAND_R)].y, bone[MTB(HAND_R)].z);
            gar_RotYXZsuperpack(&rotation1, 0);
            phd_PutMatrix(&mptr->hand_r);
            phd_PopMatrix();

            /// LEFT_ARM
            phd_PushMatrix();
            phd_TranslateRel(bone[MTB(UARM_L)].x, bone[MTB(UARM_L)].y, bone[MTB(UARM_L)].z);
            InterpolateArm();
            phd_RotYXZ(Lara.leftArm.yRot, Lara.leftArm.xRot, Lara.leftArm.zRot);
            rotation1 = Lara.leftArm.frameBase + (Lara.leftArm.frameNumber - Anims[Lara.leftArm.animNumber].frameBase) * (Anims[Lara.leftArm.animNumber].interpolation >> 8) + 9;
            gar_RotYXZsuperpack(&rotation1, 11);
            phd_PutMatrix(&mptr->uarm_l);
            phd_TranslateRel(bone[MTB(LARM_L)].x, bone[MTB(LARM_L)].y, bone[MTB(LARM_L)].z);
            gar_RotYXZsuperpack(&rotation1, 0);
            phd_PutMatrix(&mptr->larm_l);
            phd_TranslateRel(bone[MTB(HAND_L)].x, bone[MTB(HAND_L)].y, bone[MTB(HAND_L)].z);
            gar_RotYXZsuperpack(&rotation1, 0);
            phd_PutMatrix(&mptr->hand_l);
            phd_PopMatrix();
        }
    }
}

void LARA_RENDER::mesh_revolver()
{
    if (gun_type == LG_REVOLVER)
    {
        if (frac)
        {
            /// RIGHT_ARM
            phd_PushMatrix_I();
            phd_TranslateRel_I(bone[MTB(UARM_R)].x, bone[MTB(UARM_R)].y, bone[MTB(UARM_R)].z);
            InterpolateArmMatrix();
            phd_RotYXZ_I(Lara.torsoYrot, Lara.torsoXrot, Lara.torsoZrot);
            rotation1 = Lara.rightArm.frameBase + (Lara.rightArm.frameNumber - Anims[Lara.rightArm.animNumber].frameBase) * (Anims[Lara.rightArm.animNumber].interpolation >> 8) + 9;
            gar_RotYXZsuperpack(&rotation1, 8);
            phd_PutMatrix(&mptr->uarm_r);
            phd_TranslateRel(bone[MTB(LARM_R)].x, bone[MTB(LARM_R)].y, bone[MTB(LARM_R)].z);
            gar_RotYXZsuperpack(&rotation1, 0);
            phd_PutMatrix(&mptr->larm_r);
            phd_TranslateRel(bone[MTB(HAND_R)].x, bone[MTB(HAND_R)].y, bone[MTB(HAND_R)].z);
            gar_RotYXZsuperpack(&rotation1, 0);
            phd_PutMatrix(&mptr->hand_r);
            phd_PopMatrix_I();

            /// LEFT_ARM
            phd_PushMatrix_I();
            phd_TranslateRel_I(bone[MTB(UARM_L)].x, bone[MTB(UARM_L)].y, bone[MTB(UARM_L)].z);
            InterpolateArmMatrix();
            phd_RotYXZ_I(Lara.torsoYrot, Lara.torsoXrot, Lara.torsoZrot);
            rotation1 = Lara.leftArm.frameBase + (Lara.leftArm.frameNumber - Anims[Lara.leftArm.animNumber].frameBase) * (Anims[Lara.leftArm.animNumber].interpolation >> 8) + 9;
            gar_RotYXZsuperpack(&rotation1, 11);
            phd_PutMatrix(&mptr->uarm_l);
            phd_TranslateRel(bone[MTB(LARM_L)].x, bone[MTB(LARM_L)].y, bone[MTB(LARM_L)].z);
            gar_RotYXZsuperpack(&rotation1, 0);
            phd_PutMatrix(&mptr->larm_l);
            phd_TranslateRel(bone[MTB(HAND_L)].x, bone[MTB(HAND_L)].y, bone[MTB(HAND_L)].z);
            gar_RotYXZsuperpack(&rotation1, 0);
            phd_PutMatrix(&mptr->hand_l);
            phd_PopMatrix_I();
        }
        else
        {
            /// RIGHT_ARM
            phd_PushMatrix();
            phd_TranslateRel(bone[MTB(UARM_R)].x, bone[MTB(UARM_R)].y, bone[MTB(UARM_R)].z);
            InterpolateArm();
            phd_RotYXZ(Lara.torsoYrot, Lara.torsoXrot, Lara.torsoZrot);
            rotation1 = Lara.rightArm.frameBase + (Lara.rightArm.frameNumber - Anims[Lara.rightArm.animNumber].frameBase) * (Anims[Lara.rightArm.animNumber].interpolation >> 8) + 9;
            gar_RotYXZsuperpack(&rotation1, 8);
            phd_PutMatrix(&mptr->uarm_r);
            phd_TranslateRel(bone[MTB(LARM_R)].x, bone[MTB(LARM_R)].y, bone[MTB(LARM_R)].z);
            gar_RotYXZsuperpack(&rotation1, 0);
            phd_PutMatrix(&mptr->larm_r);
            phd_TranslateRel(bone[MTB(HAND_R)].x, bone[MTB(HAND_R)].y, bone[MTB(HAND_R)].z);
            gar_RotYXZsuperpack(&rotation1, 0);
            phd_PutMatrix(&mptr->hand_r);
            phd_PopMatrix();

            /// LEFT_ARM
            phd_PushMatrix();
            phd_TranslateRel(bone[MTB(UARM_L)].x, bone[MTB(UARM_L)].y, bone[MTB(UARM_L)].z);
            InterpolateArm();
            phd_RotYXZ(Lara.torsoYrot, Lara.torsoXrot, Lara.torsoZrot);
            rotation1 = Lara.leftArm.frameBase + (Lara.leftArm.frameNumber - Anims[Lara.leftArm.animNumber].frameBase) * (Anims[Lara.leftArm.animNumber].interpolation >> 8) + 9;
            gar_RotYXZsuperpack(&rotation1, 11);
            phd_PutMatrix(&mptr->uarm_l);
            phd_TranslateRel(bone[MTB(LARM_L)].x, bone[MTB(LARM_L)].y, bone[MTB(LARM_L)].z);
            gar_RotYXZsuperpack(&rotation1, 0);
            phd_PutMatrix(&mptr->larm_l);
            phd_TranslateRel(bone[MTB(HAND_L)].x, bone[MTB(HAND_L)].y, bone[MTB(HAND_L)].z);
            gar_RotYXZsuperpack(&rotation1, 0);
            phd_PutMatrix(&mptr->hand_l);
            phd_PopMatrix();
        }
    }
}

void LARA_RENDER::end_list()
{
    phd_PopMatrix();
}

void LARA_RENDER::end_world()
{
    phd_PopMatrix();
}
