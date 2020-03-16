#include "framework.h"
#include "lara_render.h"
#include "3d_gen.h"
#include "3d_gen_a.h"
#include "draw.h"
#include "specific.h"
#include "utils.h"

LARA_RENDER lara_render;

void LARA_RENDER::assign_pprot(short pprot)
{
    this->pprot = pprot;
}

void LARA_RENDER::assign_matrixptr()
{
    if (this->pprot != 1)
        this->mptr = &lara_matrix_holsters;
    else
        this->mptr = &lara_matrix_normal;
}

void LARA_RENDER::create_item()
{
    this->item = lara_item;
}

void LARA_RENDER::create_object()
{
    this->obj = &objects[this->item->object_number];
}

void LARA_RENDER::create_bone()
{
    this->bone = &bones[this->obj->bone_index];
}

void LARA_RENDER::create_frame()
{
    this->frac = GetFrames(this->item, this->frmptr, &this->rate);
}

void LARA_RENDER::check_hitdirection()
{
    if (lara.hit_direction >= 0)
    {
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

        this->frame = &anims[spaz].frame_ptr[lara.hit_frame * (anims[spaz].interpolation >> 8)];
    }
    else
    {
        this->frame = this->frmptr[0];
    }
}

void LARA_RENDER::create_rotation()
{
    this->rotation1 = this->frame + 9;
    this->rotation2 = this->frmptr[1] + 9;
}

void LARA_RENDER::create_shadow()
{
    if (this->frac)
        lara_shadow_bbox = GetBoundsAccurate(this->item);
    else
        lara_shadow_bbox = this->frame;
}


void LARA_RENDER::assign_weapon()
{
    this->gun_type = LG_UNARMED;

    if (lara.gun_status == LHS_READY
    ||  lara.gun_status == LHS_SPECIAL
    ||  lara.gun_status == LHS_DRAW
    ||  lara.gun_status == LHS_UNDRAW)
    {
        this->gun_type = lara.gun_type;
    }
}

void LARA_RENDER::start_world()
{
    phd_PushMatrix();

    if (pprot != 0 && pprot != 2)
    {
        phd_mxptr[M03] = 0;
        phd_mxptr[M13] = 0;
        phd_mxptr[M23] = 0;
    }
    else
    {
        phd_TranslateAbs(item->pos.x, item->pos.y, item->pos.z);
    }

    phd_RotYXZ(item->pos.y_rot, item->pos.x_rot, item->pos.z_rot);
    if (pprot == 2)
    {
        PHD_VECTOR shift_matrix;
        shift_matrix.z = -0x4000;
        shift_matrix.y = -0x4000;
        shift_matrix.x = -0x4000;
        ShiftMatrixBasedPos(&shift_matrix);
    }
}

void LARA_RENDER::start_list()
{
    phd_PushMatrix();

    if (this->frac)
        InitInterpolate(this->frac, this->rate);
}

void LARA_RENDER::mesh_hips()
{
    if (this->frac)
    {
        phd_TranslateRel_ID(this->frame[6], this->frame[7], this->frame[8], (int)*(this->frmptr[1] + 6), (int)*(this->frmptr[1] + 7), (int)*(this->frmptr[1] + 8));
        gar_RotYXZsuperpack_I(&this->rotation1, &this->rotation2, 0);
        phd_PutMatrix_I(this->mptr->hips);
    }
    else
    {
        phd_TranslateRel(this->frame[6], this->frame[7], this->frame[8]);
        gar_RotYXZsuperpack(&this->rotation1, 0);
        phd_PutMatrix(this->mptr->hips);
    }
}

void LARA_RENDER::mesh_thight_l()
{
    if (frac)
    {
        phd_PushMatrix_I();
        phd_TranslateRel_I(this->bone[1], this->bone[2], this->bone[3]);
        gar_RotYXZsuperpack_I(&this->rotation1, &this->rotation2, 0);
        phd_PutMatrix_I(this->mptr->thigh_l);
    }
    else
    {
        phd_PushMatrix();
        phd_TranslateRel(this->bone[1], this->bone[2], this->bone[3]);
        gar_RotYXZsuperpack(&this->rotation1, 0);
        phd_PutMatrix(this->mptr->thigh_l);
    }
}

void LARA_RENDER::mesh_calf_l()
{
    if (frac)
    {
        phd_TranslateRel_I(this->bone[5], this->bone[6], this->bone[7]);
        gar_RotYXZsuperpack_I(&this->rotation1, &this->rotation2, 0);
        phd_PutMatrix_I(this->mptr->calf_l);
    }
    else
    {
        phd_TranslateRel(this->bone[5], this->bone[6], this->bone[7]);
        gar_RotYXZsuperpack(&this->rotation1, 0);
        phd_PutMatrix(this->mptr->calf_l);
    }
}

void LARA_RENDER::mesh_foot_l()
{
    if (frac)
    {
        phd_TranslateRel_I(this->bone[9], this->bone[10], this->bone[11]);
        gar_RotYXZsuperpack_I(&this->rotation1, &this->rotation2, 0);
        phd_PutMatrix_I(this->mptr->foot_l);
        phd_PopMatrix_I();
    }
    else
    {
        phd_TranslateRel(this->bone[9], this->bone[10], this->bone[11]);
        gar_RotYXZsuperpack(&this->rotation1, 0);
        phd_PutMatrix(this->mptr->foot_l);
        phd_PopMatrix();
    }
}

void LARA_RENDER::mesh_thight_r()
{
    if (frac)
    {
        phd_PushMatrix_I();
        phd_TranslateRel_I(this->bone[13], this->bone[14], this->bone[15]);
        gar_RotYXZsuperpack_I(&this->rotation1, &this->rotation2, 0);
        phd_PutMatrix_I(this->mptr->thigh_r);
    }
    else
    {
        phd_PushMatrix();
        phd_TranslateRel(this->bone[13], this->bone[14], this->bone[15]);
        gar_RotYXZsuperpack(&this->rotation1, 0);
        phd_PutMatrix(this->mptr->thigh_r);
    }
}

void LARA_RENDER::mesh_calf_r()
{
    if (frac)
    {
        phd_TranslateRel_I(this->bone[17], this->bone[18], this->bone[19]);
        gar_RotYXZsuperpack_I(&this->rotation1, &this->rotation2, 0);
        phd_PutMatrix_I(this->mptr->calf_r);
    }
    else
    {
        phd_TranslateRel(this->bone[17], this->bone[18], this->bone[19]);
        gar_RotYXZsuperpack(&this->rotation1, 0);
        phd_PutMatrix(this->mptr->calf_r);
    }
}

void LARA_RENDER::mesh_foot_r()
{
    if (frac)
    {
        phd_TranslateRel_I(this->bone[21], this->bone[22], this->bone[23]);
        gar_RotYXZsuperpack_I(&this->rotation1, &this->rotation2, 0);
        phd_PutMatrix_I(this->mptr->foot_r);
        phd_PopMatrix_I();
    }
    else
    {
        phd_TranslateRel(this->bone[21], this->bone[22], this->bone[23]);
        gar_RotYXZsuperpack(&this->rotation1, 0);
        phd_PutMatrix(this->mptr->foot_r);
        phd_PopMatrix();
    }
}

void LARA_RENDER::mesh_torso()
{
    if (frac)
    {
        phd_TranslateRel_I(bone[25], bone[26], bone[27]);
        gar_RotYXZsuperpack_I(&this->rotation1, &this->rotation2, 0);
        phd_RotYXZ_I(lara.torso_y_rot, lara.torso_x_rot, lara.torso_z_rot);
        phd_PutMatrix_I(this->mptr->torso);
    }
    else
    {
        phd_TranslateRel(bone[25], bone[26], bone[27]);
        gar_RotYXZsuperpack(&this->rotation1, 0);
        phd_RotYXZ(lara.torso_y_rot, lara.torso_x_rot, lara.torso_z_rot);
        phd_PutMatrix(this->mptr->torso);
    }
}

void LARA_RENDER::mesh_head()
{
    if (frac)
    {
        phd_PushMatrix_I();
        phd_TranslateRel_I(bone[53], bone[54], bone[55]);
        this->rotationw1 = this->rotation1;
        this->rotationw2 = this->rotation2;
        gar_RotYXZsuperpack_I(&this->rotationw1, &this->rotationw2, 6);
        phd_RotYXZ_I(lara.head_y_rot, lara.head_x_rot, lara.head_z_rot);
        phd_PutMatrix_I(this->mptr->head);
        phd_PopMatrix_I();
    }
    else
    {
        phd_PushMatrix();
        phd_TranslateRel(bone[53], bone[54], bone[55]);
        this->rotationw1 = this->rotation1;
        gar_RotYXZsuperpack(&this->rotationw1, 6);
        phd_RotYXZ(lara.head_y_rot, lara.head_x_rot, lara.head_z_rot);
        phd_PutMatrix(this->mptr->head);
        phd_PopMatrix();
    }
}

void LARA_RENDER::mesh_unarmed()
{
    if (this->gun_type == LG_UNARMED || this->gun_type == LG_FLARE || this->gun_type == LG_TORCH)
    {
        if (frac)
        {
            /// RIGHT_ARM
            phd_PushMatrix_I();
            phd_TranslateRel_I(bone[29], bone[30], bone[31]);
            gar_RotYXZsuperpack_I(&this->rotation1, &this->rotation2, 0);
            phd_PutMatrix_I(this->mptr->uarm_r);
            phd_TranslateRel_I(bone[33], bone[34], bone[35]);
            gar_RotYXZsuperpack_I(&this->rotation1, &this->rotation2, 0);
            phd_PutMatrix_I(this->mptr->larm_r);
            phd_TranslateRel_I(bone[37], bone[38], bone[39]);
            gar_RotYXZsuperpack_I(&this->rotation1, &this->rotation2, 0);
            phd_PutMatrix_I(this->mptr->hand_r);
            phd_PopMatrix_I();

            /// LEFT_ARM
            phd_PushMatrix_I();
            phd_TranslateRel_I(bone[41], bone[42], bone[43]);

            if (lara.flare_control_left)
            {
                this->rotation1 = this->rotation2 = lara.l_arm.frame_base + (lara.l_arm.frame_number - anims[lara.l_arm.anim_number].frame_base) * (anims[lara.l_arm.anim_number].interpolation >> 8) + 9;
                gar_RotYXZsuperpack_I(&this->rotation1, &this->rotation2, 11);
            }
            else
            {
                gar_RotYXZsuperpack_I(&this->rotation1, &this->rotation2, 0);
            }

            phd_PutMatrix_I(this->mptr->uarm_l);
            phd_TranslateRel_I(bone[45], bone[46], bone[47]);
            gar_RotYXZsuperpack_I(&this->rotation1, &this->rotation2, 0);
            phd_PutMatrix_I(this->mptr->larm_l);
            phd_TranslateRel_I(bone[49], bone[50], bone[51]);
            gar_RotYXZsuperpack_I(&this->rotation1, &this->rotation2, 0);
            phd_PutMatrix_I(this->mptr->hand_l);
            phd_PopMatrix_I();
        }
        else
        {
            /// RIGHT_ARM
            phd_PushMatrix();
            phd_TranslateRel(bone[29], bone[30], bone[31]);
            gar_RotYXZsuperpack(&this->rotation1, 0);
            phd_PutMatrix(this->mptr->uarm_r);
            phd_TranslateRel(bone[33], bone[34], bone[35]);
            gar_RotYXZsuperpack(&this->rotation1, 0);
            phd_PutMatrix(this->mptr->larm_r);
            phd_TranslateRel(bone[37], bone[38], bone[39]);
            gar_RotYXZsuperpack(&this->rotation1, 0);
            phd_PutMatrix(this->mptr->hand_r);
            phd_PopMatrix();

            /// LEFT_ARM
            phd_PushMatrix();
            phd_TranslateRel(bone[41], bone[42], bone[43]);

            if (lara.flare_control_left)
            {
                this->rotation1 = lara.l_arm.frame_base + (lara.l_arm.frame_number - anims[lara.l_arm.anim_number].frame_base) * (anims[lara.l_arm.anim_number].interpolation >> 8) + 9;
                gar_RotYXZsuperpack(&this->rotation1, 11);
            }
            else
            {
                gar_RotYXZsuperpack(&this->rotation1, 0);
            }

            phd_PutMatrix(this->mptr->uarm_l);
            phd_TranslateRel(bone[45], bone[46], bone[47]);
            gar_RotYXZsuperpack(&this->rotation1, 0);
            phd_PutMatrix(this->mptr->larm_l);
            phd_TranslateRel(bone[49], bone[50], bone[51]);
            gar_RotYXZsuperpack(&this->rotation1, 0);
            phd_PutMatrix(this->mptr->hand_l);
            phd_PopMatrix();
        }
    }
}

void LARA_RENDER::mesh_1gun()
{
    if (this->gun_type == LG_SHOTGUN
    ||  this->gun_type == LG_GRENADEGUN
    ||  this->gun_type == LG_CROSSBOW)
    {
        if (frac)
        {
            /// RIGHT_ARM
            phd_PushMatrix_I();
            phd_TranslateRel_I(bone[29], bone[30], bone[31]);
            this->rotation1 = this->rotation2 = lara.r_arm.frame_base + lara.r_arm.frame_number * (anims[lara.r_arm.anim_number].interpolation >> 8) + 9;
            gar_RotYXZsuperpack_I(&this->rotation1, &this->rotation2, 8);
            phd_PutMatrix_I(this->mptr->uarm_r);
            phd_TranslateRel_I(bone[33], bone[34], bone[35]);
            gar_RotYXZsuperpack_I(&this->rotation1, &this->rotation2, 0);
            phd_PutMatrix_I(this->mptr->larm_r);
            phd_TranslateRel_I(bone[37], bone[38], bone[39]);
            gar_RotYXZsuperpack_I(&this->rotation1, &this->rotation2, 0);
            phd_PutMatrix_I(this->mptr->hand_r);
            phd_PopMatrix_I();

            /// LEFT_ARM
            phd_PushMatrix_I();
            phd_TranslateRel_I(bone[41], bone[42], bone[43]);
            gar_RotYXZsuperpack_I(&this->rotation1, &this->rotation2, 0);
            phd_PutMatrix_I(this->mptr->uarm_l);
            phd_TranslateRel_I(bone[45], bone[46], bone[47]);
            gar_RotYXZsuperpack_I(&this->rotation1, &this->rotation2, 0);
            phd_PutMatrix_I(this->mptr->larm_l);
            phd_TranslateRel_I(bone[49], bone[50], bone[51]);
            gar_RotYXZsuperpack_I(&this->rotation1, &this->rotation2, 0);
            phd_PutMatrix_I(this->mptr->hand_l);
            phd_PopMatrix_I();
        }
        else
        {
            /// RIGHT_ARM
            phd_PushMatrix();
            phd_TranslateRel(bone[29], bone[30], bone[31]);
            this->rotation1 = lara.r_arm.frame_base + lara.r_arm.frame_number * (anims[lara.r_arm.anim_number].interpolation >> 8) + 9;
            gar_RotYXZsuperpack(&this->rotation1, 8);
            phd_PutMatrix(this->mptr->uarm_r);
            phd_TranslateRel(bone[33], bone[34], bone[35]);
            gar_RotYXZsuperpack(&this->rotation1, 0);
            phd_PutMatrix(this->mptr->larm_r);
            phd_TranslateRel(bone[37], bone[38], bone[39]);
            gar_RotYXZsuperpack(&this->rotation1, 0);
            phd_PutMatrix(this->mptr->hand_r);
            phd_PopMatrix();

            /// LEFT_ARM
            phd_PushMatrix();
            phd_TranslateRel(bone[41], bone[42], bone[43]);
            gar_RotYXZsuperpack(&this->rotation1, 0);
            phd_PutMatrix(this->mptr->uarm_l);
            phd_TranslateRel(bone[45], bone[46], bone[47]);
            gar_RotYXZsuperpack(&this->rotation1, 0);
            phd_PutMatrix(this->mptr->larm_l);
            phd_TranslateRel(bone[49], bone[50], bone[51]);
            gar_RotYXZsuperpack(&this->rotation1, 0);
            phd_PutMatrix(this->mptr->hand_l);
            phd_PopMatrix();
        }
    }
}

void LARA_RENDER::mesh_2gun()
{
    if (this->gun_type == LG_PISTOLS
    ||  this->gun_type == LG_UZIS)
    {
        if (frac)
        {
            /// RIGHT_ARM
            phd_PushMatrix_I();
            phd_TranslateRel_I(this->bone[29], this->bone[30], this->bone[31]);
            InterpolateArmMatrix();
            phd_RotYXZ(lara.r_arm.y_rot, lara.r_arm.x_rot, lara.r_arm.z_rot);
            this->rotation1 = lara.r_arm.frame_base + (lara.r_arm.frame_number - anims[lara.r_arm.anim_number].frame_base) * (anims[lara.r_arm.anim_number].interpolation >> 8) + 9;
            gar_RotYXZsuperpack(&this->rotation1, 8);
            phd_PutMatrix(this->mptr->uarm_r);
            phd_TranslateRel(this->bone[33], this->bone[34], this->bone[35]);
            gar_RotYXZsuperpack(&this->rotation1, 0);
            phd_PutMatrix(this->mptr->larm_r);
            phd_TranslateRel(this->bone[37], this->bone[38], this->bone[39]);
            gar_RotYXZsuperpack(&this->rotation1, 0);
            phd_PutMatrix(this->mptr->hand_r);
            phd_PopMatrix_I();

            /// LEFT_ARM
            phd_PushMatrix_I();
            phd_TranslateRel_I(bone[41], bone[42], bone[43]);
            InterpolateArmMatrix();
            phd_RotYXZ(lara.l_arm.y_rot, lara.l_arm.x_rot, lara.l_arm.z_rot);
            this->rotation1 = lara.l_arm.frame_base + (lara.l_arm.frame_number - anims[lara.l_arm.anim_number].frame_base) * (anims[lara.l_arm.anim_number].interpolation >> 8) + 9;
            gar_RotYXZsuperpack(&this->rotation1, 11);
            phd_PutMatrix(this->mptr->uarm_l);
            phd_TranslateRel(bone[45], bone[46], bone[47]);
            gar_RotYXZsuperpack(&this->rotation1, 0);
            phd_PutMatrix(this->mptr->larm_l);
            phd_TranslateRel(bone[49], bone[50], bone[51]);
            gar_RotYXZsuperpack(&this->rotation1, 0);
            phd_PutMatrix(this->mptr->hand_l);
            phd_PopMatrix_I();
        }
        else
        {
            /// RIGHT_ARM
            phd_PushMatrix();
            phd_TranslateRel(this->bone[29], this->bone[30], this->bone[31]);
            *(phd_mxptr + M00) = *(phd_mxptr + M00 - 24);
            *(phd_mxptr + M01) = *(phd_mxptr + M01 - 24);	  	// Restore Laras Main Rotations
            *(phd_mxptr + M02) = *(phd_mxptr + M02 - 24);       // Less any torso rotations
            *(phd_mxptr + M10) = *(phd_mxptr + M10 - 24);       // avoids Wobbly arms when
            *(phd_mxptr + M11) = *(phd_mxptr + M11 - 24);       // moving 'n' shooting
            *(phd_mxptr + M12) = *(phd_mxptr + M12 - 24);
            *(phd_mxptr + M20) = *(phd_mxptr + M20 - 24);
            *(phd_mxptr + M21) = *(phd_mxptr + M21 - 24);
            *(phd_mxptr + M22) = *(phd_mxptr + M22 - 24);
            phd_RotYXZ(lara.r_arm.y_rot, lara.r_arm.x_rot, lara.r_arm.z_rot);
            this->rotation1 = lara.r_arm.frame_base + (lara.r_arm.frame_number - anims[lara.r_arm.anim_number].frame_base) * (anims[lara.r_arm.anim_number].interpolation >> 8) + 9;
            gar_RotYXZsuperpack(&this->rotation1, 8);
            phd_PutMatrix(this->mptr->uarm_r);
            phd_TranslateRel(this->bone[33], this->bone[34], this->bone[35]);
            gar_RotYXZsuperpack(&this->rotation1, 0);
            phd_PutMatrix(this->mptr->larm_r);
            phd_TranslateRel(this->bone[37], this->bone[38], this->bone[39]);
            gar_RotYXZsuperpack(&this->rotation1, 0);
            phd_PutMatrix(this->mptr->hand_r);
            phd_PopMatrix();

            /// LEFT_ARM
            phd_PushMatrix();
            phd_TranslateRel(bone[41], bone[42], bone[43]);
            *(phd_mxptr + M00) = *(phd_mxptr + M00 - 24);
            *(phd_mxptr + M01) = *(phd_mxptr + M01 - 24);	  	// Restore Laras Main Rotations
            *(phd_mxptr + M02) = *(phd_mxptr + M02 - 24);       // Less any torso rotations
            *(phd_mxptr + M10) = *(phd_mxptr + M10 - 24);       // avoids Wobbly arms when
            *(phd_mxptr + M11) = *(phd_mxptr + M11 - 24);       // moving 'n' shooting
            *(phd_mxptr + M12) = *(phd_mxptr + M12 - 24);
            *(phd_mxptr + M20) = *(phd_mxptr + M20 - 24);
            *(phd_mxptr + M21) = *(phd_mxptr + M21 - 24);
            *(phd_mxptr + M22) = *(phd_mxptr + M22 - 24);
            phd_RotYXZ(lara.l_arm.y_rot, lara.l_arm.x_rot, lara.l_arm.z_rot);
            this->rotation1 = lara.l_arm.frame_base + (lara.l_arm.frame_number - anims[lara.l_arm.anim_number].frame_base) * (anims[lara.l_arm.anim_number].interpolation >> 8) + 9;
            gar_RotYXZsuperpack(&this->rotation1, 11);
            phd_PutMatrix(this->mptr->uarm_l);
            phd_TranslateRel(bone[45], bone[46], bone[47]);
            gar_RotYXZsuperpack(&this->rotation1, 0);
            phd_PutMatrix(this->mptr->larm_l);
            phd_TranslateRel(bone[49], bone[50], bone[51]);
            gar_RotYXZsuperpack(&this->rotation1, 0);
            phd_PutMatrix(this->mptr->hand_l);
            phd_PopMatrix();
        }
    }
}

void LARA_RENDER::mesh_revolver()
{
    if (this->gun_type == LG_REVOLVER)
    {
        if (frac)
        {
            /// RIGHT_ARM
            phd_PushMatrix_I();
            phd_TranslateRel_I(this->bone[29], this->bone[30], this->bone[31]);
            InterpolateArmMatrix();
            phd_RotYXZ_I(lara.torso_y_rot, lara.torso_x_rot, lara.torso_z_rot);
            this->rotation1 = lara.r_arm.frame_base + (lara.r_arm.frame_number - anims[lara.r_arm.anim_number].frame_base) * (anims[lara.r_arm.anim_number].interpolation >> 8) + 9;
            gar_RotYXZsuperpack(&this->rotation1, 8);
            phd_PutMatrix(this->mptr->uarm_r);
            phd_TranslateRel(this->bone[33], this->bone[34], this->bone[35]);
            gar_RotYXZsuperpack(&this->rotation1, 0);
            phd_PutMatrix(this->mptr->larm_r);
            phd_TranslateRel(this->bone[37], this->bone[38], this->bone[39]);
            gar_RotYXZsuperpack(&this->rotation1, 0);
            phd_PutMatrix(this->mptr->hand_r);
            phd_PopMatrix_I();

            /// LEFT_ARM
            phd_PushMatrix_I();
            phd_TranslateRel_I(bone[41], bone[42], bone[43]);
            InterpolateArmMatrix();
            phd_RotYXZ_I(lara.torso_y_rot, lara.torso_x_rot, lara.torso_z_rot);
            this->rotation1 = lara.l_arm.frame_base + (lara.l_arm.frame_number - anims[lara.l_arm.anim_number].frame_base) * (anims[lara.l_arm.anim_number].interpolation >> 8) + 9;
            gar_RotYXZsuperpack(&this->rotation1, 11);
            phd_PutMatrix(this->mptr->uarm_l);
            phd_TranslateRel(bone[45], bone[46], bone[47]);
            gar_RotYXZsuperpack(&this->rotation1, 0);
            phd_PutMatrix(this->mptr->larm_l);
            phd_TranslateRel(bone[49], bone[50], bone[51]);
            gar_RotYXZsuperpack(&this->rotation1, 0);
            phd_PutMatrix(this->mptr->hand_l);
            phd_PopMatrix_I();
        }
        else
        {
            /// RIGHT_ARM
            phd_PushMatrix();
            phd_TranslateRel(this->bone[29], this->bone[30], this->bone[31]);
            *(phd_mxptr + M00) = *(phd_mxptr + M00 - 24);
            *(phd_mxptr + M01) = *(phd_mxptr + M01 - 24);	  	// Restore Laras Main Rotations
            *(phd_mxptr + M02) = *(phd_mxptr + M02 - 24);       // Less any torso rotations
            *(phd_mxptr + M10) = *(phd_mxptr + M10 - 24);       // avoids Wobbly arms when
            *(phd_mxptr + M11) = *(phd_mxptr + M11 - 24);       // moving 'n' shooting
            *(phd_mxptr + M12) = *(phd_mxptr + M12 - 24);
            *(phd_mxptr + M20) = *(phd_mxptr + M20 - 24);
            *(phd_mxptr + M21) = *(phd_mxptr + M21 - 24);
            *(phd_mxptr + M22) = *(phd_mxptr + M22 - 24);
            phd_RotYXZ(lara.torso_y_rot, lara.torso_x_rot, lara.torso_z_rot);
            this->rotation1 = lara.r_arm.frame_base + (lara.r_arm.frame_number - anims[lara.r_arm.anim_number].frame_base) * (anims[lara.r_arm.anim_number].interpolation >> 8) + 9;
            gar_RotYXZsuperpack(&this->rotation1, 8);
            phd_PutMatrix(this->mptr->uarm_r);
            phd_TranslateRel(this->bone[33], this->bone[34], this->bone[35]);
            gar_RotYXZsuperpack(&this->rotation1, 0);
            phd_PutMatrix(this->mptr->larm_r);
            phd_TranslateRel(this->bone[37], this->bone[38], this->bone[39]);
            gar_RotYXZsuperpack(&this->rotation1, 0);
            phd_PutMatrix(this->mptr->hand_r);
            phd_PopMatrix();

            /// LEFT_ARM
            phd_PushMatrix();
            phd_TranslateRel(bone[41], bone[42], bone[43]);
            *(phd_mxptr + M00) = *(phd_mxptr + M00 - 24);
            *(phd_mxptr + M01) = *(phd_mxptr + M01 - 24);	  	// Restore Laras Main Rotations
            *(phd_mxptr + M02) = *(phd_mxptr + M02 - 24);       // Less any torso rotations
            *(phd_mxptr + M10) = *(phd_mxptr + M10 - 24);       // avoids Wobbly arms when
            *(phd_mxptr + M11) = *(phd_mxptr + M11 - 24);       // moving 'n' shooting
            *(phd_mxptr + M12) = *(phd_mxptr + M12 - 24);
            *(phd_mxptr + M20) = *(phd_mxptr + M20 - 24);
            *(phd_mxptr + M21) = *(phd_mxptr + M21 - 24);
            *(phd_mxptr + M22) = *(phd_mxptr + M22 - 24);
            phd_RotYXZ(lara.torso_y_rot, lara.torso_x_rot, lara.torso_z_rot);
            this->rotation1 = lara.l_arm.frame_base + (lara.l_arm.frame_number - anims[lara.l_arm.anim_number].frame_base) * (anims[lara.l_arm.anim_number].interpolation >> 8) + 9;
            gar_RotYXZsuperpack(&this->rotation1, 11);
            phd_PutMatrix(this->mptr->uarm_l);
            phd_TranslateRel(bone[45], bone[46], bone[47]);
            gar_RotYXZsuperpack(&this->rotation1, 0);
            phd_PutMatrix(this->mptr->larm_l);
            phd_TranslateRel(bone[49], bone[50], bone[51]);
            gar_RotYXZsuperpack(&this->rotation1, 0);
            phd_PutMatrix(this->mptr->hand_l);
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
