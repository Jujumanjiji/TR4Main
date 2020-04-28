#include "framework.h"
#include "sphere.h"
#include "3d_gen.h"
#include "3d_gen_a.h"

static PHD_MATRIX* LaraMatrixMesh(int joint)
{
    switch (joint)
    {
        default:
        case JHIPS:
            return &lara_matrix_normal.hips;
        case JTHIGH_L:
            return &lara_matrix_normal.thigh_l;
        case JCALF_L:
            return &lara_matrix_normal.calf_l;
        case JFOOT_L:
            return &lara_matrix_normal.foot_l;
        case JTHIGH_R:
            return &lara_matrix_normal.thigh_r;
        case JCALF_R:
            return &lara_matrix_normal.calf_r;
        case JFOOT_R:
            return &lara_matrix_normal.foot_r;
        case JTORSO:
            return &lara_matrix_normal.torso;
        case JHEAD:
            return &lara_matrix_normal.head;
        case JUARM_R:
            return &lara_matrix_normal.uarm_r;
        case JLARM_R:
            return &lara_matrix_normal.larm_r;
        case JHAND_R:
            return &lara_matrix_normal.hand_r;
        case JUARM_L:
            return &lara_matrix_normal.uarm_l;
        case JLARM_L:
            return &lara_matrix_normal.larm_l;
        case JHAND_L:
            return &lara_matrix_normal.hand_l;
    }
}

static void TransposeMatrix(PHD_MATRIX* matrix)
{
    PHD_MATRIX* mptr;
    mptr = phd_mxptr;
    mptr->m00 = matrix->m00;
    mptr->m01 = matrix->m01;
    mptr->m02 = matrix->m02;
    mptr->m03 = matrix->m03;
    mptr->m10 = matrix->m10;
    mptr->m11 = matrix->m11;
    mptr->m12 = matrix->m12;
    mptr->m13 = matrix->m13;
    mptr->m20 = matrix->m20;
    mptr->m21 = matrix->m21;
    mptr->m22 = matrix->m22;
    mptr->m23 = matrix->m23;
}

void GetLaraJointAbsPosition(PHD_VECTOR* pos, int joint)
{
    PHD_MATRIX* mptr;

    phd_PushMatrix();
    TransposeMatrix(LaraMatrixMesh(joint));
    phd_TranslateRel(pos->x, pos->y, pos->z);
    mptr = phd_mxptr;
    pos->x = mptr->m03 >> W2V_SHIFT;
    pos->y = mptr->m13 >> W2V_SHIFT;
    pos->z = mptr->m23 >> W2V_SHIFT;
    pos->x += lara_item->pos.x;
    pos->y += lara_item->pos.y;
    pos->z += lara_item->pos.z;
    phd_PopMatrix();
}

#ifdef DLL_INJECT
void injector::f_game::inject_sphere()
{
    inject(0x0041D9A0, GetLaraJointAbsPosition);
}
#endif
