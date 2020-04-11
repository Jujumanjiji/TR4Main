#include "framework.h"
#include "sphere.h"
#include "3d_gen.h"
#include "3d_gen_a.h"

static int* LaraMatrixMesh(int joint)
{
    switch (joint)
    {
        default:
        case HIPS:
            return lara_matrix_normal.hips;
        case THIGH_L:
            return lara_matrix_normal.thigh_l;
        case CALF_L:
            return lara_matrix_normal.calf_l;
        case FOOT_L:
            return lara_matrix_normal.foot_l;
        case THIGH_R:
            return lara_matrix_normal.thigh_r;
        case CALF_R:
            return lara_matrix_normal.calf_r;
        case FOOT_R:
            return lara_matrix_normal.foot_r;
        case TORSO:
            return lara_matrix_normal.torso;
        case UARM_R:
            return lara_matrix_normal.uarm_r;
        case LARM_R:
            return lara_matrix_normal.larm_r;
        case HAND_R:
            return lara_matrix_normal.hand_r;
        case UARM_L:
            return lara_matrix_normal.uarm_l;
        case LARM_L:
            return lara_matrix_normal.larm_l;
        case HAND_L:
            return lara_matrix_normal.hand_l;
        case HEAD:
            return lara_matrix_normal.head;
    }
}

static void TransposeMatrix(int* matrix)
{
    *(phd_mxptr + M00) = *(matrix + M00);
    *(phd_mxptr + M01) = *(matrix + M01);
    *(phd_mxptr + M02) = *(matrix + M02);
    *(phd_mxptr + M03) = *(matrix + M03);
    *(phd_mxptr + M10) = *(matrix + M10);
    *(phd_mxptr + M11) = *(matrix + M11);
    *(phd_mxptr + M12) = *(matrix + M12);
    *(phd_mxptr + M13) = *(matrix + M13);
    *(phd_mxptr + M20) = *(matrix + M20);
    *(phd_mxptr + M21) = *(matrix + M21);
    *(phd_mxptr + M22) = *(matrix + M22);
    *(phd_mxptr + M23) = *(matrix + M23);
}

void GetLaraHandAbsPosition(PHD_VECTOR* pos, int joint)
{
    phd_PushMatrix();
    TransposeMatrix(LaraMatrixMesh(joint));
    phd_TranslateRel(pos->x, pos->y, pos->z);
    pos->x = *(phd_mxptr + M03) >> W2V_SHIFT;
    pos->y = *(phd_mxptr + M13) >> W2V_SHIFT;
    pos->z = *(phd_mxptr + M23) >> W2V_SHIFT;
    pos->x += lara_item->pos.x;
    pos->y += lara_item->pos.y;
    pos->z += lara_item->pos.z;
    phd_PopMatrix();
}

#ifdef DLL_INJECT
void injector::inject_sphere()
{
    this->inject(0x0041D9A0, GetLaraHandAbsPosition);
}
#endif
