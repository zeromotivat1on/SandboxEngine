#pragma once

namespace snd
{
	struct vec3;
	struct mat3;
	struct mat4;

	struct quat
	{
		f32				x;
		f32				y;
		f32				z;
		f32				w;

						quat();
		explicit		quat(f32 a);
		explicit		quat(f32 a, f32 b, f32 c, f32 d);
		
		f32				operator[](i32 index) const;
		f32& 			operator[](i32 index);
		quat			operator-() const;
		quat			operator+(const quat& a) const;
		quat			operator-(const quat& a) const;
		quat			operator*(const quat& a) const;
		vec3			operator*(const vec3& a) const;
		quat			operator*(f32 a) const;
		quat& 			operator+=(const quat& a);
		quat& 			operator-=(const quat& a);
		quat& 			operator*=(const quat& a);
		quat& 			operator*=(f32 a);

		bool			operator==(	const quat& a) const;
		bool			operator!=(	const quat& a) const;
		
		friend quat		operator*(const f32 a, const quat& b);
		friend vec3		operator*(const vec3& a, const quat& b);

		quat& 			Set(f32 a, f32 b, f32 c, f32 d);

		bool			Equal(const quat& a) const;
		bool			Equal(const quat& a, const f32 epsilon) const;
		
		quat			Inverse() const;
		f32				Length() const;
		quat& 			Normalize();

		f32				W() const;	// calculate quaternion w-component

		mat3			Mat3() const;
		mat4			Mat4() const;

		i32				Dimension() const;

		const f32*		Ptr() const;
		f32*			Ptr();
		const char*		String(i32 precision = 2) const;
	};

	SND_INLINE quat::quat()
		: x(0.0f), y(0.0f), z(0.0f), w(0.0f)
	{
	}

	SND_INLINE quat::quat(f32 a)
		: x(a), y(a), z(a), w(1.0f)
	{
	}
	
	SND_INLINE quat::quat(f32 a, f32 b, f32 c, f32 d)
		: x(a), y(b), z(c), w(d)
	{
	}
	
	SND_INLINE f32 quat::operator[](i32 index) const
	{
		return (&x)[index];
	}

	SND_INLINE f32& quat::operator[](i32 index)
	{
		return (&x)[index];
	}

	SND_INLINE quat quat::operator-() const
	{
		return quat(-x, -y, -z, -w);
	}

	SND_INLINE quat quat::operator+(const quat& a) const
	{
		return quat(x + a.x, y + a.y, z + a.z, w + a.w);
	}

	SND_INLINE quat quat::operator-(const quat& a) const
	{
		return quat(x - a.x, y - a.y, z - a.z, w - a.w);
	}

	SND_INLINE quat quat::operator*(const quat& a) const
	{
		return quat
		(
			w * a.x + x * a.w + y * a.z - z * a.y,
			w * a.y + y * a.w + z * a.x - x * a.z,
			w * a.z + z * a.w + x * a.y - y * a.x,
			w * a.w - x * a.x - y * a.y - z * a.z
		);
	}

	SND_INLINE vec3 quat::operator*(const vec3& a) const
	{
		const f32 xxzz = x * x - z * z;
		const f32 wwyy = w * w - y * y;

		const f32 xw2 = x * w * 2.0f;
		const f32 xy2 = x * y * 2.0f;
		const f32 xz2 = x * z * 2.0f;
		const f32 yw2 = y * w * 2.0f;
		const f32 yz2 = y * z * 2.0f;
		const f32 zw2 = z * w * 2.0f;

		return vec3
		(
			(xxzz + wwyy) * a.x + (xy2 + zw2) * a.y						+ (xz2 - yw2) * a.z,
			(xy2 - zw2) * a.x   + (y * y + w * w - x * x - z * z) * a.y + (yz2 + xw2) * a.z,
			(xz2 + yw2) * a.x   + (yz2 - xw2) * a.y						+ (wwyy - xxzz) * a.z
		);
	}

	SND_INLINE quat quat::operator*(f32 a) const
	{
		return quat(x * a, y * a, z * a, w * a);
	}
	
	SND_INLINE quat& quat::operator+=(const quat& a)
	{
		x += a.x;
		y += a.y;
		z += a.z;
		w += a.w;
		return *this;
	}

	SND_INLINE quat& quat::operator-=(const quat& a)
	{
		x -= a.x;
		y -= a.y;
		z -= a.z;
		w -= a.w;
		return *this;
	}

	SND_INLINE quat& quat::operator*=(const quat& a)
	{
		*this = *this * a;
		return *this;
	}

	SND_INLINE quat& quat::operator*=(f32 a) {
		x *= a;
		y *= a;
		z *= a;
		w *= a;
		return *this;
	}

	SND_INLINE bool quat::operator==(const quat& a) const
	{
		return Equal(a);
	}

	SND_INLINE bool quat::operator!=(const quat& a) const
	{
		return !(*this == a);
	}
	
	SND_INLINE quat operator*(const f32 a, const quat& b)
	{
		return b * a;
	}

	SND_INLINE vec3 operator*(const vec3& a, const quat& b)
	{
		return b * a;
	}

	SND_INLINE quat& quat::Set(f32 a, f32 b, f32 c, f32 d)
	{
		x = a;
		y = b;
		z = c;
		w = d;
		return *this;
	}

	SND_INLINE bool quat::Equal(const quat& a) const
	{
		return (x == a.x) && (y == a.y) && (z == a.z) && (w == a.w);
	}

	SND_INLINE bool quat::Equal(const quat& a, const f32 epsilon) const
	{
		return math::Absf(x - a.x) <= epsilon &&
			   math::Absf(y - a.y) <= epsilon &&
			   math::Absf(z - a.z) <= epsilon &&
			   math::Absf(w - a.w) <= epsilon;
	}

	SND_INLINE quat quat::Inverse() const
	{
		return quat(-x, -y, -z, w);
	}

	SND_INLINE f32 quat::Length() const
	{
		return math::Sqrt(x * x + y * y + z * z + w * w);
	}

	SND_INLINE quat& quat::Normalize()
	{
		const f32 length = Length();
		if (length != 0.0f)
		{
			const f32 lengthInv = 1 / length;
			x *= lengthInv;
			y *= lengthInv;
			z *= lengthInv;
			w *= lengthInv;
		}
		return *this;
	}

	SND_INLINE f32 quat::W() const
	{
		return math::Sqrt(math::Absf(1.0f - (x * x + y * y + z * z)));
	}

	SND_INLINE i32 quat::Dimension() const
	{
		return 4;
	}

	SND_INLINE const f32 *quat::Ptr() const
	{
		return &x;
	}

	SND_INLINE f32 *quat::Ptr()
	{
		return &x;
	}

	SND_INLINE const char* quat::String(i32 precision) const
	{
	}
}
