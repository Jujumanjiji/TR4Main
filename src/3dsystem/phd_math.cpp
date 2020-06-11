#include "framework.h"
#include "phd_math.h"

short ANGLE(short angle)
{
    return angle * ONE_DEGREE;
}

float to_degrees(short angle)
{
    return (unsigned short)angle * 360.0f / 65536.0f;
}

float to_radians(short angle)
{
    return angle * 360.0f / 65536.0f * RADIAN;
}

long mGetAngle(long srcX, long srcZ, long targetX, long targetZ)
{
    long angleX;
    long angleZ;
    long swapBuf;
    long result;
    char flags;

    result = 0;
    angleX = targetX - srcX;
    angleZ = targetZ - srcZ;

    if (targetX != srcX || targetZ != srcZ)
    {
        flags = 0;
        if (angleX < 0)
        {
            flags |= 4;
            angleX = srcX - targetX;
        }

        if (angleZ < 0)
        {
            flags |= 2;
            angleZ = srcZ - targetZ;
        }

        if (angleZ > angleX)
        {
            flags |= 1;
            SWAP(angleX, angleZ, swapBuf);
        }

        for (; (short)angleZ != angleZ; angleX >>= 1)
            angleZ >>= 1;

        if (angleX == 0)
            angleX = 1;
        result = atanBase[flags] + atanTable[(angleZ << 11) / angleX];
    }

    if (result == 0)
        return 0;

    return abs(result) & 0xFFFF;
}

int phd_atan(int x, int y)
{
    if ((x == 0) && (y == 0))
        return 0;

    int octant = 0;

    if (x < 0)
    {
        octant += 4;
        x = -x;
    }

    if (y < 0)
    {
        octant += 2;
        y = -y;
    }

    if (y > x)
    {
        octant++;
        int n = x;
        x = y;
        y = n;
    }

    while ((short)y != y)
    {
        y >>= 1;
        x >>= 1;
    }

    if (!x)
        x = 1;

    int n = atanTable[(y << 11) / x] + atanBase[octant];
    short angle;

    if (n < 0)
        angle = -n;
    else
        angle = n;

    return angle;
}

int phd_sqrt(int x)
{
    return int(sqrt(x));
}

int phd_sin(short angle)
{
    return int((sin(to_radians(angle)) * 16384.0f));
}

int phd_cos(short angle)
{
    return int((cos(to_radians(angle)) * 16384.0f));
}
