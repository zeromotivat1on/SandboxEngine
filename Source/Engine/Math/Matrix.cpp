#include "sndpch.h"
#include "Engine/Math/Matrix.h"

// math

snd::mat4 snd::math::LookAt(const vec3& eye, const vec3& at, const vec3& up)
{
	const vec3 f = (at - eye).Normalize();
	const vec3 r = f.Cross(up).Normalize();
	const vec3 u = r.Cross(f);

	mat4 result;

	result[0][0] = r.x;
	result[1][0] = r.y;
	result[2][0] = r.z;
	result[3][0] = -r.Dot(eye);

	result[0][1] = u.x;
	result[1][1] = u.y;
	result[2][1] = u.z;
	result[3][1] = -u.Dot(eye);

	result[0][2] = -f.x;
	result[1][2] = -f.y;
	result[2][2] = -f.z;
	result[3][2] = f.Dot(eye);

	result[0][3] = 0.0f;
	result[1][3] = 0.0f;
	result[2][3] = 0.0f;
	result[3][3] = 1.0f;

	return result;
}

snd::mat4 snd::math::Perspective(f32 rfovy, f32 aspect, f32 n, f32 f)
{
	const f32 tanHalfFovY = Tan(rfovy * 0.5f);

	mat4 result;
	result[0][0] = 1.0f / (aspect * tanHalfFovY);	// x-axis scaling
	result[1][1] = 1.0f / tanHalfFovY;				// y-axis scaling
	result[2][2] = -(f + n) / (f - n);				// z-axis scaling
	result[2][3] = -1.0f;							// rh perspective divide
	result[3][2] = -(2.0f * f * n) / (f - n);		// z-axis translation

	return result;
}

snd::mat4 snd::math::Orthographic(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f)
{
	mat4 result;
	result[0][0] =  2.0f / (r - l);
	result[1][1] =  2.0f / (t - b);
	result[2][2] = -2.0f / (f - n);
	result[3][0] = -(r + l) / (r - l);
	result[3][1] = -(t + b) / (t - b);
	result[3][2] = -(f + n) / (f - n);
	result[3][3] =  1.0f;
	return result;
}

// mat4

f32 snd::mat4::Determinant() const
{
	// determinant_size_rows_cols

	// 2x2 sub-determinants
	const f32 det2_01_01 = mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
	const f32 det2_01_02 = mat[0][0] * mat[1][2] - mat[0][2] * mat[1][0];
	const f32 det2_01_03 = mat[0][0] * mat[1][3] - mat[0][3] * mat[1][0];
	const f32 det2_01_12 = mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1];
	const f32 det2_01_13 = mat[0][1] * mat[1][3] - mat[0][3] * mat[1][1];
	const f32 det2_01_23 = mat[0][2] * mat[1][3] - mat[0][3] * mat[1][2];

	// 3x3 sub-determinants
	const f32 det3_201_012 = mat[2][0] * det2_01_12 - mat[2][1] * det2_01_02 + mat[2][2] * det2_01_01;
	const f32 det3_201_013 = mat[2][0] * det2_01_13 - mat[2][1] * det2_01_03 + mat[2][3] * det2_01_01;
	const f32 det3_201_023 = mat[2][0] * det2_01_23 - mat[2][2] * det2_01_03 + mat[2][3] * det2_01_02;
	const f32 det3_201_123 = mat[2][1] * det2_01_23 - mat[2][2] * det2_01_13 + mat[2][3] * det2_01_12;

	return -det3_201_123 * mat[3][0] + det3_201_023 * mat[3][1] - det3_201_013 * mat[3][2] + det3_201_012 * mat[3][3];
}

bool snd::mat4::InverseSelf()
{
	// determinant_size_rows_cols

	// 2x2 sub-determinants required to calculate 4x4 determinant
	const f32 det2_01_01 = mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
	const f32 det2_01_02 = mat[0][0] * mat[1][2] - mat[0][2] * mat[1][0];
	const f32 det2_01_03 = mat[0][0] * mat[1][3] - mat[0][3] * mat[1][0];
	const f32 det2_01_12 = mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1];
	const f32 det2_01_13 = mat[0][1] * mat[1][3] - mat[0][3] * mat[1][1];
	const f32 det2_01_23 = mat[0][2] * mat[1][3] - mat[0][3] * mat[1][2];

	// 3x3 sub-determinants required to calculate 4x4 determinant
	const f32 det3_201_012 = mat[2][0] * det2_01_12 - mat[2][1] * det2_01_02 + mat[2][2] * det2_01_01;
	const f32 det3_201_013 = mat[2][0] * det2_01_13 - mat[2][1] * det2_01_03 + mat[2][3] * det2_01_01;
	const f32 det3_201_023 = mat[2][0] * det2_01_23 - mat[2][2] * det2_01_03 + mat[2][3] * det2_01_02;
	const f32 det3_201_123 = mat[2][1] * det2_01_23 - mat[2][2] * det2_01_13 + mat[2][3] * det2_01_12;

	const f32 det = -det3_201_123 * mat[3][0] + det3_201_023 * mat[3][1] - det3_201_013 * mat[3][2] + det3_201_012 * mat[3][3];

	if (math::Absf(det) < math::g_MatrixInvEpsilon)
	{
		return false;
	}

	const f32 invDet = 1.0f / det;

	// Remaining 2x2 sub-determinants
	const f32 det2_03_01 = mat[0][0] * mat[3][1] - mat[0][1] * mat[3][0];
	const f32 det2_03_02 = mat[0][0] * mat[3][2] - mat[0][2] * mat[3][0];
	const f32 det2_03_03 = mat[0][0] * mat[3][3] - mat[0][3] * mat[3][0];
	const f32 det2_03_12 = mat[0][1] * mat[3][2] - mat[0][2] * mat[3][1];
	const f32 det2_03_13 = mat[0][1] * mat[3][3] - mat[0][3] * mat[3][1];
	const f32 det2_03_23 = mat[0][2] * mat[3][3] - mat[0][3] * mat[3][2];

	const f32 det2_13_01 = mat[1][0] * mat[3][1] - mat[1][1] * mat[3][0];
	const f32 det2_13_02 = mat[1][0] * mat[3][2] - mat[1][2] * mat[3][0];
	const f32 det2_13_03 = mat[1][0] * mat[3][3] - mat[1][3] * mat[3][0];
	const f32 det2_13_12 = mat[1][1] * mat[3][2] - mat[1][2] * mat[3][1];
	const f32 det2_13_13 = mat[1][1] * mat[3][3] - mat[1][3] * mat[3][1];
	const f32 det2_13_23 = mat[1][2] * mat[3][3] - mat[1][3] * mat[3][2];

	// Remaining 3x3 sub-determinants
	const f32 det3_203_012 = mat[2][0] * det2_03_12 - mat[2][1] * det2_03_02 + mat[2][2] * det2_03_01;
	const f32 det3_203_013 = mat[2][0] * det2_03_13 - mat[2][1] * det2_03_03 + mat[2][3] * det2_03_01;
	const f32 det3_203_023 = mat[2][0] * det2_03_23 - mat[2][2] * det2_03_03 + mat[2][3] * det2_03_02;
	const f32 det3_203_123 = mat[2][1] * det2_03_23 - mat[2][2] * det2_03_13 + mat[2][3] * det2_03_12;

	const f32 det3_213_012 = mat[2][0] * det2_13_12 - mat[2][1] * det2_13_02 + mat[2][2] * det2_13_01;
	const f32 det3_213_013 = mat[2][0] * det2_13_13 - mat[2][1] * det2_13_03 + mat[2][3] * det2_13_01;
	const f32 det3_213_023 = mat[2][0] * det2_13_23 - mat[2][2] * det2_13_03 + mat[2][3] * det2_13_02;
	const f32 det3_213_123 = mat[2][1] * det2_13_23 - mat[2][2] * det2_13_13 + mat[2][3] * det2_13_12;

	const f32 det3_301_012 = mat[3][0] * det2_01_12 - mat[3][1] * det2_01_02 + mat[3][2] * det2_01_01;
	const f32 det3_301_013 = mat[3][0] * det2_01_13 - mat[3][1] * det2_01_03 + mat[3][3] * det2_01_01;
	const f32 det3_301_023 = mat[3][0] * det2_01_23 - mat[3][2] * det2_01_03 + mat[3][3] * det2_01_02;
	const f32 det3_301_123 = mat[3][1] * det2_01_23 - mat[3][2] * det2_01_13 + mat[3][3] * det2_01_12;

	mat[0][0] =	-det3_213_123 * invDet;
	mat[1][0] = +det3_213_023 * invDet;
	mat[2][0] = -det3_213_013 * invDet;
	mat[3][0] = +det3_213_012 * invDet;

	mat[0][1] = +det3_203_123 * invDet;
	mat[1][1] = -det3_203_023 * invDet;
	mat[2][1] = +det3_203_013 * invDet;
	mat[3][1] = -det3_203_012 * invDet;

	mat[0][2] = +det3_301_123 * invDet;
	mat[1][2] = -det3_301_023 * invDet;
	mat[2][2] = +det3_301_013 * invDet;
	mat[3][2] = -det3_301_012 * invDet;

	mat[0][3] = -det3_201_123 * invDet;
	mat[1][3] = +det3_201_023 * invDet;
	mat[2][3] = -det3_201_013 * invDet;
	mat[3][3] = +det3_201_012 * invDet;

	return true;
}

snd::mat4& snd::mat4::Rotate(const mat4& rotation)
{
	*this = *this * rotation;
	return *this;
}

snd::mat4& snd::mat4::Rotate(const quat& rotation)
{
    *this = *this * rotation.Mat4();
    return *this;
}
