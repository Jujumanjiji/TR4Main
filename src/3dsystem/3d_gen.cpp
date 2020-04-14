#include "framework.h"
#include "3d_gen.h"
#include "w2v_setup.h"

void AlterFOV(short newFov)
{
    CurrentFOV = newFov;
    phd_persp = phd_winwidth / 2 * COS(newFov / 2) / SIN(newFov / 2);
    f_persp = float(phd_persp);
    f_oneopersp = one / f_persp;
    f_perspoznear = f_persp / f_znear;
    f_opersp = f_persp;
    f_zspersp = Z_ONE  / f_opersp;
    f_oneoznear = f_opersp / f_zsnear;
    LfAspectCorrection = W2V_ASPECT_CORRECTION / (float(phd_winwidth) / float(phd_winheight));
}

void phd_GenerateW2V(PHD_3DPOS* viewpos)
{
    w2v.SetupMatrix_Ptr();
    w2v.SetupMatrix_CosAndSin(viewpos);
    w2v.Matrix00();
    w2v.Matrix01();
    w2v.Matrix02();
    w2v.Matrix10();
    w2v.Matrix11();
    w2v.Matrix12();
    w2v.Matrix20();
    w2v.Matrix21();
    w2v.Matrix22();
    w2v.SetupPoisonEffect();
    w2v.Matrix03();
    w2v.Matrix13();
    w2v.Matrix23();
    w2v.AspectCorrection(M10);
    w2v.AspectCorrection(M11);
    w2v.AspectCorrection(M12);
    SetupDXW2V(&dx_mxw2v, &w2v_matrix);
}

void phd_LookAt(int xsrc, int ysrc, int zsrc, int xtar, int ytar, int ztar, short roll)
{
    PHD_3DPOS viewpos;
    short angles[2];
    short angx, angy;
    int dx, dy, dz;

    phd_GetVectorAngles(xtar - xsrc, ytar - ysrc, ztar - zsrc, angles);
    viewpos.x = xsrc;
    viewpos.y = ysrc;
    viewpos.z = zsrc;
    viewpos.x_rot = angles[1];
    viewpos.y_rot = angles[0];
    viewpos.z_rot = roll;

    dx = SQUARE(xsrc - xtar);
    dy = ysrc - ytar;
    dz = SQUARE(zsrc - ztar);
    
    angx = (short)mGetAngle(0, 0, (DWORD)sqrt(dx + dz), (DWORD)dy);
    angy = (short)mGetAngle(zsrc, xsrc, ztar, xtar);
    cam_rot.x = angx >> 4;
    cam_rot.y = angy >> 4;
    cam_rot.z = 0;
    cam_pos.x = xsrc;
    cam_pos.y = ysrc;
    cam_pos.z = zsrc;
    phd_GenerateW2V(&viewpos);
    SetupDXMatrixTransformState();
}

int phd_atan(int x, int y)
{
    if (x == 0 && y == 0)
        return 0;

    int result = 0;
    int swapBuf;
    char flags = 0;

    if (x < 0)
    {
        flags |= 4;
        x = -x;
    }

    if (y < 0)
    {
        flags |= 2;
        y = -y;
    }

    if (y > x)
    {
        flags |= 1;
        SWAP(x, y, swapBuf);
    }

    for (; (short)y != y; x >>= 1)
        result >>= 1;

    if (x == 0)
        x = 1;

    result = atanBase[flags] + atanTable[(y << 11) / x];
    return abs(result);
}

int phd_sqrt(int x)
{
    int result;
    int base = 0x40000000;
    int based_result;

    result = 0;
    for (; base != 0; base >>= 2)
    {
        for (; base != 0; base >>= 2)
        {
            based_result = base + result;
            result >>= 1;

            if (based_result > x)
                break;

            x -= based_result;
            result |= base;
        }
    }

    return result;
}

BOOL phd_TranslateRel(int x, int y, int z)
{
    PHD_MATRIX* mptr = phd_mxptr;
    mptr->m03 += ((mptr->m00 * x) + (mptr->m01 * y) + (mptr->m02 * z));
    mptr->m13 += ((mptr->m10 * x) + (mptr->m11 * y) + (mptr->m12 * z));
    mptr->m23 += ((mptr->m20 * x) + (mptr->m21 * y) + (mptr->m22 * z));

    bool relX = ABS(mptr->m03) > phd_zfar;
    bool relY = ABS(mptr->m13) > phd_zfar;
    bool relZ = ABS(mptr->m23) > phd_zfar;

    if (relX || relY || relZ)
        return TRUE;
    else
        return FALSE;
}

void phd_TranslateAbs(int x, int y, int z)
{
    PHD_MATRIX* mptr;

    x -= w2v_matrix.m03;
    y -= w2v_matrix.m13;
    z -= w2v_matrix.m23;

    mptr = phd_mxptr;
    mptr->m03 = ((mptr->m00 * x) + (mptr->m01 * y) + (mptr->m02 * z));
    mptr->m13 = ((mptr->m10 * x) + (mptr->m11 * y) + (mptr->m12 * z));
    mptr->m23 = ((mptr->m20 * x) + (mptr->m21 * y) + (mptr->m22 * z));
}

template<typename T>
static void UnpackRotation(T ang, int& matrix1, int& matrix2, int shift, bool invert)
{
    int r0, r1;
    int cos, sin;

    if (ang)
    {
        sin = SIN(ang);
        cos = COS(ang);

        if (invert)
        {
            r0 = ((matrix1 * cos) - (matrix2 * sin));
            r1 = ((matrix2 * cos) + (matrix1 * sin));
        }
        else
        {
            r0 = ((matrix1 * cos) + (matrix2 * sin));
            r1 = ((matrix2 * cos) - (matrix1 * sin));
        }

        matrix1 = r0 >> shift;
        matrix2 = r1 >> shift;
    }
}

void phd_RotX(short rx)
{
    PHD_MATRIX* mptr = phd_mxptr;
    UnpackRotation(rx, mptr->m01, mptr->m02, W2V_SHIFT, false);
    UnpackRotation(rx, mptr->m11, mptr->m12, W2V_SHIFT, false);
    UnpackRotation(rx, mptr->m21, mptr->m22, W2V_SHIFT, false);
}

void phd_RotY(short ry)
{
    PHD_MATRIX* mptr = phd_mxptr;
    UnpackRotation(ry, mptr->m00, mptr->m02, W2V_SHIFT, true);
    UnpackRotation(ry, mptr->m10, mptr->m12, W2V_SHIFT, true);
    UnpackRotation(ry, mptr->m20, mptr->m22, W2V_SHIFT, true);
}

void phd_RotZ(short rz)
{
    PHD_MATRIX* mptr = phd_mxptr;
    UnpackRotation(rz, mptr->m00, mptr->m01, W2V_SHIFT, false);
    UnpackRotation(rz, mptr->m10, mptr->m11, W2V_SHIFT, false);
    UnpackRotation(rz, mptr->m20, mptr->m21, W2V_SHIFT, false);
}

void phd_RotYXZpack(int angs)
{
    PHD_MATRIX* mptr = phd_mxptr;
    int ang = (angs >> 4) & 0xFFC0;     // EXTRACT Y
    UnpackRotation(ang, mptr->m00, mptr->m02, W2V_SHIFT, true);
    UnpackRotation(ang, mptr->m10, mptr->m12, W2V_SHIFT, true);
    UnpackRotation(ang, mptr->m20, mptr->m22, W2V_SHIFT, true);

    ang = (angs >> 14) & 0xFFC0;        // EXTRACT X
    UnpackRotation(ang, mptr->m01, mptr->m02, W2V_SHIFT, false);
    UnpackRotation(ang, mptr->m11, mptr->m12, W2V_SHIFT, false);
    UnpackRotation(ang, mptr->m21, mptr->m22, W2V_SHIFT, false);

    ang = (angs & 0x3FF) << 6;          // EXTRACT Z
    UnpackRotation(ang, mptr->m00, mptr->m01, W2V_SHIFT, false);
    UnpackRotation(ang, mptr->m10, mptr->m11, W2V_SHIFT, false);
    UnpackRotation(ang, mptr->m20, mptr->m21, W2V_SHIFT, false);
}

void phd_RotYXZ(short rx, short ry, short rz)
{
    PHD_MATRIX* mptr = phd_mxptr;
    // EXTRACT Y
    UnpackRotation(rx, mptr->m00, mptr->m02, W2V_SHIFT, true);
    UnpackRotation(rx, mptr->m10, mptr->m12, W2V_SHIFT, true);
    UnpackRotation(rx, mptr->m20, mptr->m22, W2V_SHIFT, true);
    // EXTRACT X
    UnpackRotation(ry, mptr->m01, mptr->m02, W2V_SHIFT, false);
    UnpackRotation(ry, mptr->m11, mptr->m12, W2V_SHIFT, false);
    UnpackRotation(ry, mptr->m21, mptr->m22, W2V_SHIFT, false);
    // EXTRACT Z
    UnpackRotation(rz, mptr->m00, mptr->m01, W2V_SHIFT, false);
    UnpackRotation(rz, mptr->m10, mptr->m11, W2V_SHIFT, false);
    UnpackRotation(rz, mptr->m20, mptr->m21, W2V_SHIFT, false);
}

void phd_GetVectorAngles(int x, int y, int z, short* dest)
{
    *(dest + 0) = phd_atan(z, x);
    
    while ((short)x != x || (short)y != y || (short)z != z)
    {
        x >>= 2;            //
        y >>= 2;            // Scale to World Size
        z >>= 2;            //
    }

    short pitch = phd_atan(phd_sqrt(SQUARE(z) + SQUARE(x)), y);
    if ((y > 0 && pitch > 0) || (y < 0 && pitch < 0))
        pitch = -pitch;
    *(dest + 1) = pitch;
}

void ScaleCurrentMatrix(PHD_VECTOR* scale)
{
    PHD_MATRIX* mptr = phd_mxptr;
    mptr->m00 = mptr->m00 * scale->x >> W2V_SHIFT;
    mptr->m01 = mptr->m01 * scale->y >> W2V_SHIFT;
    mptr->m02 = mptr->m02 * scale->z >> W2V_SHIFT;

    mptr->m10 = scale->x * mptr->m10 >> W2V_SHIFT;
    mptr->m11 = mptr->m11 * scale->y >> W2V_SHIFT;
    mptr->m12 = mptr->m12 * scale->z >> W2V_SHIFT;
    
    mptr->m20 = scale->x * mptr->m20 >> W2V_SHIFT;
    mptr->m21 = scale->y * mptr->m21 >> W2V_SHIFT;
    mptr->m22 = scale->z * mptr->m22 >> W2V_SHIFT;
}

static void SetupZRange(void)
{
    float fb = (f_zsfar * f_zsnear * ZRANGE) / (f_zsnear - f_zsfar);
    f_a = MINZR - fb / f_zsnear;
    f_b = -fb;
    f_boo = -fb / Z_ONE;
}

static void SetupZNear(int nNear)
{
    phd_znear = nNear;
    f_znear = float(phd_znear);
    f_zsnear = float(phd_znear >> W2V_SHIFT);
    f_oneoznear = f_opersp / f_zsnear;
    f_perspoznear = f_persp / f_znear;
}

static void SetupZFar(int nFar)
{
    phd_zfar = nFar;
    f_zfar = float(phd_zfar);
    f_zsfar = float(phd_zfar >> W2V_SHIFT);
    f_oneozfar = Z_ONE / f_zsnear;
}

void SetupZ(int nNear, int nFar)
{
    SetupZNear(nNear);
    SetupZFar(nFar);
    SetupZRange();
}

void phd_InitWindow(int x, int y, int width, int height, int nearz, int farz, short view_angle)
{
    phd_winxmin = x;
    phd_winymin = y;
    phd_winxmax = width - 1;
    phd_winymax = height - 1;
    phd_winwidth = width;
    phd_winheight = height;
    phd_centerx = width / 2;
    phd_centery = height / 2;
    phd_znear = (nearz << W2V_SHIFT);
    phd_zfar = (farz << W2V_SHIFT);
    f_centerx = float(phd_centerx);
    f_centery = float(phd_centery);
    f_winxmin = float(phd_winxmin);
    f_winymin = float(phd_winymin);
    f_winxmax = float(phd_winxmax + 1);
    f_winymax = float(phd_winymax + 1);

    AlterFOV(ANGLE(view_angle));
    SetupZ(phd_znear, phd_zfar);
    
    phd_right = phd_winxmax;
    phd_bottom = phd_winymax;
    phd_top = y;
    phd_left = x;
    phd_mxptr = matrix_stack;
}

void SetupDXMatrixTransformState(void)
{
    InitD3DMatrix(&dx_mxworld);
    InitD3DMatrix(&dx_mxprojection);
    dx_mxprojection._22 = -1.0;

    DX_TRY(App.lpD3DDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, &dx_mxworld));
    DX_TRY(App.lpD3DDevice->SetTransform(D3DTRANSFORMSTATE_PROJECTION, &dx_mxprojection));
}

void SetD3DViewMatrix(void)
{
    PHD_MATRIX* mptr = phd_mxptr;
    InitD3DMatrix(&dx_mxview);
    dx_mxview._11 = float(mptr->m00) * W2V_D3DVALUE;
    dx_mxview._21 = float(mptr->m01) * W2V_D3DVALUE;
    dx_mxview._31 = float(mptr->m02) * W2V_D3DVALUE;
    dx_mxview._12 = float(mptr->m10) * W2V_D3DVALUE;
    dx_mxview._22 = float(mptr->m11) * W2V_D3DVALUE;
    dx_mxview._32 = float(mptr->m12) * W2V_D3DVALUE;
    dx_mxview._13 = float(mptr->m20) * W2V_D3DVALUE;
    dx_mxview._23 = float(mptr->m21) * W2V_D3DVALUE;
    dx_mxview._33 = float(mptr->m22) * W2V_D3DVALUE;
    dx_mxview._41 = float(mptr->m03 >> W2V_SHIFT);
    dx_mxview._42 = float(mptr->m13 >> W2V_SHIFT);
    dx_mxview._43 = float(mptr->m23 >> W2V_SHIFT);
    DX_TRY(App.lpD3DDevice->SetTransform(D3DTRANSFORMSTATE_VIEW, &dx_mxview));
}

void InitD3DMatrix(D3DMATRIX *mptr)
{
    mptr->_11 = 1.0f;
    mptr->_12 = 0.0f;
    mptr->_13 = 0.0f;
    mptr->_14 = 0.0f;
    mptr->_21 = 0.0f;
    mptr->_22 = 1.0f;
    mptr->_23 = 0.0f;
    mptr->_24 = 0.0f;
    mptr->_31 = 0.0f;
    mptr->_32 = 0.0f;
    mptr->_33 = 1.0f;
    mptr->_34 = 0.0f;
    mptr->_41 = 0.0f;
    mptr->_42 = 0.0f;
    mptr->_43 = 0.0f;
    mptr->_44 = 1.0f;
}

void SetupDXW2V(D3DMATRIX* dest, PHD_MATRIX* pptr)
{
    InitD3DMatrix(dest);
    dest->_11 = float(pptr->m00) * W2V_D3DVALUE;
    dest->_21 = float(pptr->m01) * W2V_D3DVALUE;
    dest->_31 = float(pptr->m02) * W2V_D3DVALUE;
    dest->_12 = float(pptr->m10) * W2V_D3DVALUE;
    dest->_22 = float(pptr->m11) * W2V_D3DVALUE;
    dest->_32 = float(pptr->m12) * W2V_D3DVALUE;
    dest->_13 = float(pptr->m20) * W2V_D3DVALUE;
    dest->_23 = float(pptr->m21) * W2V_D3DVALUE;
    dest->_33 = float(pptr->m22) * W2V_D3DVALUE;
    dest->_41 = float(pptr->m03 >> W2V_SHIFT);
    dest->_42 = float(pptr->m13 >> W2V_SHIFT);
    dest->_43 = float(pptr->m23 >> W2V_SHIFT);
}

#ifdef DLL_INJECT
void injector::inject_3d_gen()
{
    this->inject(0x0048F9D0, AlterFOV);
    this->inject(0x0048FDC0, phd_GenerateW2V);
    this->inject(0x00490110, phd_LookAt);
    this->inject(0x00490350, phd_TranslateRel);
    this->inject(0x00490A10, phd_TranslateAbs);
    this->inject(0x00490400, phd_RotX);
    this->inject(0x004904B0, phd_RotY);
    this->inject(0x00490560, phd_RotZ);
    this->inject(0x00490610, phd_RotYXZpack);
    this->inject(0x00490820, phd_RotYXZ);
    this->inject(0x00490A90, phd_GetVectorAngles);
    //this->inject(0x0047DA60, phd_PutPolygons); // corrupted in IDAPro
    this->inject(0x00490210, phd_atan);
    this->inject(0x00490280, phd_sqrt);
    this->inject(0x0048FA90, SetupZ);
    this->inject(0x0048FB60, ScaleCurrentMatrix);
    this->inject(0x0048FC10, phd_InitWindow);
    //this->inject(0x0048FD40, mGetAngle);
    this->inject(0x00490CF0, SetupDXMatrixTransformState);
    this->inject(0x00490B30, SetD3DViewMatrix);
    this->inject(0x00490DD0, InitD3DMatrix);
    this->inject(0x00490C30, SetupDXW2V);
}
#endif