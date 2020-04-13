#include "framework.h"
#include "draw.h"
#include "3d_gen.h"
#include "3d_gen_a.h"
#include "control.h"
#include "delstuff.h"
#include "deltapak.h"
#include "health.h"
#include "output.h"
#include "specific.h"
#include "utils.h"

int DrawPhaseGame(void)
{
    CalcLaraMatrices(0);
    phd_PushUnitMatrix();
    CalcLaraMatrices(1);
    phd_PopMatrix();
    if (GnPlayingCutseq)
        frigup_lara();
    SetLaraUnderwaterNodes();
    DrawRooms(camera.pos.room_number);
    DrawGameInfo(1);
    S_OutputPolyList();
    camera.number_frames = S_DumpScreen();
    S_AnimateTextures(camera.number_frames);
    return camera.number_frames;
}

void InitInterpolate(int frac, int rate)
{
    IM_rate = rate;
    IM_frac = frac;
    IM_ptr = IM_stack;
    memcpy(IM_stack, phd_mxptr, sizeof(PHD_MATRIX));
}

void DrawEffect(short fx_number)
{
    PHD_MATRIX* mptr;
    FX_INFO* fx;
    OBJECT_INFO* obj;

    fx = &effects[fx_number];
    obj = &objects[fx->object_number];
    if (obj->loaded && obj->draw_routine)
    {
        phd_PushMatrix();
        phd_TranslateAbs(fx->pos.x, fx->pos.y, fx->pos.z);
        mptr = phd_mxptr;
        if (mptr->m23 > phd_znear && mptr->m23 < phd_zfar)
        {
            phd_RotYXZ(fx->pos.y_rot, fx->pos.x_rot, fx->pos.z_rot);
            if (obj->nmeshes > 0)
                phd_PutPolygons(meshes[obj->mesh_index], NO_CLIP);
            else
                phd_PutPolygons(meshes[fx->frame_number], NO_CLIP);
        }
        phd_PopMatrix();
    }
}

void DrawAnimatingItem(ITEM_INFO* item)
{
    OBJECT_INFO* obj;
    BONE_STRUCT* bone;
    DWORD flags;
    int frac, rate, clip;
    int mesh_bits;
    short** mesh;
    short* frmptr[2];
    short* pprot1, *pprot2;
    short* extra_rotation;

    frac = GetFrames(item, frmptr, &rate);
    obj = &objects[item->object_number];
    if (obj->shadow_size)
        S_PrintShadow(obj->shadow_size, frmptr[0], item);
    phd_PushMatrix(); // world
    phd_TranslateAbs(item->pos.x, item->pos.y, item->pos.z);
    phd_RotYXZ(item->pos.y_rot, item->pos.x_rot, item->pos.z_rot);

    // get the mip instead !
    if (obj->loaded && obj->object_mip && (phd_mxptr->m23 >> 16) > obj->object_mip)
        obj++;

    if (item->object_number < ENEMY_JEEP || item->object_number > SETHA_MIP)
    {
        DrawMovingItem(item, frmptr[0]);
    }
    else
    {
        phd_left = 0;
        phd_right = phd_winwidth;
        phd_top = 0;
        phd_bottom = phd_winheight;
    }

    clip = S_GetObjectBounds(frmptr[0]);
    if (clip) // clipped ?
    {
        CalculateObjectLighting(item, frmptr[0]);
        extra_rotation = (short*)item->data;
        if (!extra_rotation)
            extra_rotation = dummy_rotation;
        bone = (BONE_STRUCT*)&bones[obj->bone_index];
        mesh = &meshes[obj->mesh_index];
        mesh_bits = 1;

        if (frac)
        {
            // INTERPOLATE
            InitInterpolate(frac, rate);
            phd_TranslateRel_ID((int)*(frmptr[0] + 6), (int)*(frmptr[0] + 7), (int)*(frmptr[0] + 8), (int)*(frmptr[1] + 6), (int)*(frmptr[1] + 7), (int)*(frmptr[1] + 8));
            pprot1 = frmptr[0] + 9;
            pprot2 = frmptr[1] + 9;
            gar_RotYXZsuperpack_I(&pprot1, &pprot2, 0);

            if (mesh_bits & item->mesh_bits)
                phd_PutPolygons_I(mesh[1], clip);
            else
                phd_PutPolygons_I(mesh[0], clip);

            mesh += 2;
            for (int i = 0; i < (obj->nmeshes - 1); i++, mesh += 2)
            {
                flags = bone->flags;
                if (flags & BT_POP)
                    phd_PopMatrix_I();
                if (flags & BT_PUSH)
                    phd_PushMatrix_I();

                phd_TranslateRel_I(bone->x, bone->y, bone->z);
                gar_RotYXZsuperpack_I(&pprot1, &pprot2, 0);

                if (flags & ROT_ALL)
                {
                    if (flags & ROT_Y)
                        phd_RotY_I(*(extra_rotation++));
                    if (flags & ROT_X)
                        phd_RotX_I(*(extra_rotation++));
                    if (flags & ROT_Z)
                        phd_RotZ_I(*(extra_rotation++));
                }

                mesh_bits <<= 1;
                if (mesh_bits & item->mesh_bits)
                    phd_PutPolygons_I(mesh[1], clip);
                else
                    phd_PutPolygons_I(mesh[0], clip);

                if (item->fired_weapon && i == (EnemyOffset[obj->bit_offset].mesh - 1))
                {
                    BITE_INFO* offset = &EnemyOffset[obj->bit_offset];
                    phd_PushMatrix_I();
                    phd_TranslateRel_I(offset->x, offset->y, offset->z);
                    phd_RotYXZ_I(0, -16380, (GetRandomControl() << 14) + (GetRandomControl() >> 2) - 4096);
                    InterpolateMatrix();
                    phd_PutPolygons(meshes[objects[GUN_FLASH].mesh_index], clip);
                    phd_PopMatrix_I();
                    item->fired_weapon--;
                }

                bone++;
            }
        }
        else
        {
            // NORMAL
            phd_TranslateRel((int)*(frmptr[0] + 6), (int)*(frmptr[0] + 7), (int)*(frmptr[0] + 8));
            pprot1 = frmptr[0] + 9;
            gar_RotYXZsuperpack(&pprot1, 0);

            if (mesh_bits & item->mesh_bits)
                phd_PutPolygons(mesh[1], clip);
            else
                phd_PutPolygons(mesh[0], clip);

            mesh += 2;
            for (int i = 0; i < (obj->nmeshes - 1); i++, mesh += 2)
            {
                flags = bone->flags;
                if (flags & BT_POP)
                    phd_PopMatrix();
                if (flags & BT_PUSH)
                    phd_PushMatrix();

                phd_TranslateRel(bone->x, bone->y, bone->z);
                gar_RotYXZsuperpack(&pprot1, 0);

                if (flags & ROT_ALL)
                {
                    if (flags & ROT_Y)
                        phd_RotY(*(extra_rotation++));
                    if (flags & ROT_X)
                        phd_RotX(*(extra_rotation++));
                    if (flags & ROT_Z)
                        phd_RotZ(*(extra_rotation++));
                }
                
                mesh_bits <<= 1;
                if (mesh_bits & item->mesh_bits)
                    phd_PutPolygons(mesh[1], clip);
                else
                    phd_PutPolygons(mesh[0], clip);

                if (item->fired_weapon && i == (EnemyOffset[obj->bit_offset].mesh - 1))
                {
                    BITE_INFO* offset = &EnemyOffset[obj->bit_offset];
                    phd_PushMatrix();
                    phd_TranslateRel(offset->x, offset->y, offset->z);
                    phd_RotX(-16380);
                    phd_PutPolygons(meshes[objects[GUN_FLASH].mesh_index], clip);
                    phd_PopMatrix();
                    item->fired_weapon--;
                }

                bone++;
            }
        }
    }

    phd_left = 0;
    phd_top = 0;
    phd_right = phd_winwidth;
    phd_bottom = phd_winheight;
    phd_PopMatrix(); // !world
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

/*
#define current_item_light VAR_U_(0x005B74B0, ITEM_INFO*)
#define sub_478570 ((void(__cdecl*)(ITEM_INFO* item)) 0x00478570)
#define sub_478750 ((void(__cdecl*)(ITEM_INFO* item)) 0x00478750)

void CalculateObjectLighting(ITEM_INFO* item, short* frame)
{
    PHD_MATRIX* mptr;
    int x, y, z;

    if (item->shade >= 0)
    {
        S_CalculateStaticMeshLight(item->pos.x, item->pos.y, item->pos.z, item->shade & 0x7FFF, &rooms[item->room_number]);
    }
    else
    {
        phd_PushUnitMatrix();
        mptr = phd_mxptr;
        mptr->m03 = 0;
        mptr->m13 = 0;
        mptr->m23 = 0;
        phd_RotYXZ(item->pos.y_rot, item->pos.x_rot, item->pos.z_rot);
        phd_TranslateRel((frame[0] + frame[1]) >> 1, (frame[2] + frame[3]) >> 1, (frame[4] + frame[5]) >> 1);
        x = item->pos.x + (mptr->m03 >> W2V_SHIFT);
        y = item->pos.y + (mptr->m13 >> W2V_SHIFT);
        z = item->pos.z + (mptr->m23 >> W2V_SHIFT);
        phd_PopMatrix();
        
        current_item_light = item;
        item->light_data[5476] = x;
        item->light_data[5480] = y;
        item->light_data[5484] = z;

        //sub_478570(item); // POSITION UPDATER ?
        //sub_478750(item);
    }
}
*/

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

static short interpolate_bounds[6];
short* GetBoundsAccurate(ITEM_INFO* item)
{
    short* bounds;
    int frac, rate;
    short *frmptr[2];

    frac = GetFrames(item, frmptr, &rate);
    if (frac == 0)
    {
        return frmptr[0];
    }
    else
    {
        bounds = interpolate_bounds;
       
        for (int i = 0; i < 6; i++, bounds++, frmptr[0]++, frmptr[1]++)
            *(bounds) = *(frmptr[0]) + ((*(frmptr[1]) - *(frmptr[0])) * frac) / rate;

        return interpolate_bounds;
    }
}

short* GetBestFrame(ITEM_INFO* item)
{
    int frac, rate;
    short *frmptr[2];

    frac = GetFrames(item, frmptr, &rate);
    if (frac <= (rate >> 1))
        return frmptr[0];
    else
        return frmptr[1];
}



#ifdef DLL_INJECT
void injector::inject_draw()
{
    this->inject(0x0044EBA0, DrawPhaseGame);
    //this->inject(0x0044EC10, DrawRooms);
    //this->inject(0x0044F2D0, PrintRooms);
    //this->inject(0x0044F330, PrintObjects);
    //this->inject(0x0044F5D0, GetRoomBounds);
    //this->inject(0x0044F790, SetRoomBounds);
    this->inject(0x0044FB10, DrawEffect);
    //this->inject(0x0044FC00, DrawMovingItem);
    this->inject(0x0044FF60, DrawAnimatingItem);
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
    this->inject(0x00450E60, GetBoundsAccurate);
    this->inject(0x00450EE0, GetBestFrame);
}
#endif