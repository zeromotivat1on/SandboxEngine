#include "sndpch.h"
#include "SandboxEngine/Math/Quat.h"

snd::mat3 snd::quat::Mat3() const
{
    mat3 mat;

    const f32 x2 = x + x;
    const f32 y2 = y + y;
    const f32 z2 = z + z;

    const f32 xx = x * x2;
    const f32 xy = x * y2;
    const f32 xz = x * z2;

    const f32 yy = y * y2;
    const f32 yz = y * z2;
    const f32 zz = z * z2;

    const f32 wx = w * x2;
    const f32 wy = w * y2;
    const f32 wz = w * z2;

    mat[0][0] = 1.0f - (yy + zz);
    mat[1][0] = xy - wz;
    mat[2][0] = xz + wy;

    mat[0][1] = xy + wz;
    mat[1][1] = 1.0f - (xx + zz);
    mat[2][1] = yz - wx;

    mat[0][2] = xz - wy;
    mat[1][2] = yz + wx;
    mat[2][2] = 1.0f - (xx + yy);

    return mat;
}

snd::mat4 snd::quat::Mat4() const
{
    return Mat3().Mat4();
}
