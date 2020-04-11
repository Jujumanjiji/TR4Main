#include "framework.h"
#include "sphere.h"
#include "3d_gen.h"
#include "3d_gen_a.h"

static int* LaraMatrixMesh(int joint)
{
    switch (joint)
    {
        default:
        case JHIPS:
            return lara_matrix_normal.hips;
        case JTHIGH_L:
            return lara_matrix_normal.thigh_l;
        case JCALF_L:
            return lara_matrix_normal.calf_l;
        case JFOOT_L:
            return lara_matrix_normal.foot_l;
        case JTHIGH_R:
            return lara_matrix_normal.thigh_r;
        case JCALF_R:
            return lara_matrix_normal.calf_r;
        case JFOOT_R:
            return lara_matrix_normal.foot_r;
        case JTORSO:
            return lara_matrix_normal.torso;
        case JHEAD:
            return lara_matrix_normal.head;
        case JUARM_R:
            return lara_matrix_normal.uarm_r;
        case JLARM_R:
            return lara_matrix_normal.larm_r;
        case JHAND_R:
            return lara_matrix_normal.hand_r;
        case JUARM_L:
            return lara_matrix_normal.uarm_l;
        case JLARM_L:
            return lara_matrix_normal.larm_l;
        case JHAND_L:
            return lara_matrix_normal.hand_l;
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

void GetLaraJointAbsPosition(PHD_VECTOR* pos, int joint)
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
    this->inject(0x0041D9A0, GetLaraJointAbsPosition);
}
#endif
