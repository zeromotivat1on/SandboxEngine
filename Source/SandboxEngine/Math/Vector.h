#pragma once

namespace snd
{
    struct mat3;
    struct euler;
    
    struct vec2
    {
        f32             x;
        f32             y;

                        vec2();
        explicit        vec2(f32 a);
        explicit        vec2(f32 a, f32 b);

        f32			    operator[](i32 index) const;
        f32&			operator[](i32 index);
        vec2			operator-() const;
        vec2			operator+(const vec2& a) const;
        vec2			operator-(const vec2& a) const;
        f32			    operator*(const vec2& a) const;
        vec2			operator*(f32 a) const;
        vec2			operator/(f32 a) const;
        vec2&		    operator+=(const vec2& a);
        vec2&		    operator-=(const vec2& a);
        vec2&		    operator/=(const vec2& a);
        vec2&		    operator/=(f32 a);
        vec2&		    operator*=(f32 a);
        
        bool			operator==(const vec2& a) const;
        bool			operator!=(const vec2& a) const;

        friend vec2	    operator*(f32 a, vec2 b);

        vec2& 			Set(f32 a, f32 b);
        vec2&			Zero();
        
        bool			Equal(const vec2& a) const;
        bool			Equal(const vec2& a, f32 epsilon) const;

        f32			    Length() const;
        f32			    LengthSqr() const;
        f32             Dot(const vec2& a) const;
        vec2&           Normalize();            // normalize vector
        vec2&		    Truncate(f32 length);   // cap vector length
        
        i32				Dimension() const;

        f32*			Ptr();
        const f32*	    Ptr() const;
        const char*	    String(i8 precision = 2) const;

        vec2&			Clamp(const vec2& min, const vec2& max);
        vec2&			Snap();                 // snap to closest integer value
        vec2&			Lerp(const vec2& v1, const vec2& v2, f32 alpha);
    };
    
    SND_INLINE vec2::vec2()
        : x(0.0f), y(0.0f)
    {
    }

    SND_INLINE vec2::vec2(f32 a)
        : x(a), y(a)
    {
    }
    
    SND_INLINE vec2::vec2(f32 a, f32 b)
        : x(a), y(b)
    {
    }

    SND_INLINE f32 vec2::operator[](i32 index) const
    {
        return (&x)[index];
    }

    SND_INLINE f32& vec2::operator[](i32 index)
    {
        return (&x)[index];
    }

    SND_INLINE vec2 vec2::operator-() const
    {
        return vec2(-x, -y);
    }

    SND_INLINE vec2 vec2::operator+(const vec2& a) const
    {
        return vec2(x + a.x, y + a.y);
    }

    SND_INLINE vec2 vec2::operator-(const vec2& a) const
    {
        return vec2(x - a.x, y - a.y);
    }
    
    SND_INLINE f32 vec2::operator*(const vec2& a) const
    {
        return Dot(a);
    }

    SND_INLINE vec2 vec2::operator*(f32 a) const
    {
        return vec2(x * a, y * a);
    }

    SND_INLINE vec2 vec2::operator/(f32 a) const
    {
        const f32 inv = 1.0f / a;
        return vec2(x * inv, y * inv);
    }

    SND_INLINE vec2& vec2::operator+=(const vec2& a)
    {
        x += a.x;
        y += a.y;
        return *this;
    }

    SND_INLINE vec2& vec2::operator-=(const vec2& a)
    {
        x -= a.x;
        y -= a.y;
        return *this;
    }

    SND_INLINE vec2& vec2::operator/=(const vec2& a)
    {
        x /= a.x;
        y /= a.y;
        return *this;
    }

    SND_INLINE vec2& vec2::operator/=(f32 a)
    {
        const f32 inv = 1.0f / a;
        x *= inv;
        y *= inv;
        return *this;
    }

    SND_INLINE vec2& vec2::operator*=(f32 a)
    {
        x *= a;
        y *= a;
        return *this;
    }

    SND_INLINE bool vec2::operator==(const vec2& a) const
    {
        return Equal(a);
    }

    SND_INLINE bool vec2::operator!=(const vec2& a) const
    {
        return !(*this == a);
    }

    SND_INLINE vec2 operator*(f32 a, vec2 b)
    {
	    return vec2(b.x * a, b.y * a);
    }

    SND_INLINE vec2& vec2::Set(f32 a, f32 b)
    {
        x = a;
        y = b;
        return *this;
    }

    SND_INLINE vec2& vec2::Zero()
    {
        x = y = 0.0f;
        return *this;
    }

    SND_INLINE bool vec2::Equal(const vec2& a) const
    {
        return x == a.x && y == a.y;
    }

    SND_INLINE bool vec2::Equal(const vec2& a, f32 epsilon) const
    {
        return math::Absf(x - a.x) <= epsilon &&
               math::Absf(y - a.y) <= epsilon;
    }

    SND_INLINE f32 vec2::Length() const
    {
        return math::Sqrt(x * x + y * y);
    }

    SND_INLINE f32 vec2::LengthSqr() const
    {
        return x * x + y * y;
    }

    SND_INLINE f32 vec2::Dot(const vec2& a) const
    {
        return x * a.x + y * a.y;
    }
    
    SND_INLINE vec2& vec2::Normalize()
    {
        const f32 lengthInv = math::InvSqrt(x * x + y * y);

        x *= lengthInv;
        y *= lengthInv;
        
        return *this;
    }

    SND_INLINE vec2& vec2::Truncate(f32 length)
    {
        if (length == 0.0f)
        {
            Zero();
            return *this;
        }
        
        const f32 lengthSquare = x * x + y * y;
        if (lengthSquare > length * length)
        {
            const f32 scaleFactor = length * math::InvSqrt(lengthSquare);
            x *= scaleFactor;
            y *= scaleFactor;
        }
        
        return *this;
    }

    SND_INLINE i32 vec2::Dimension() const
    {
        return 2;
    }

    SND_INLINE f32* vec2::Ptr()
    {
        return &x;
    }

    SND_INLINE const f32* vec2::Ptr() const
    {
        return &x;
    }

    SND_INLINE const char* vec2::String(i8 precision) const
    {
    }

    SND_INLINE vec2& vec2::Clamp(const vec2& min, const vec2& max)
    {
        x = std::clamp(x, min.x, max.x);
        y = std::clamp(y, min.y, max.y);
        return *this;
    }

    SND_INLINE vec2& vec2::Snap()
    {
        x = static_cast<f32>(static_cast<i32>(x));
        y = static_cast<f32>(static_cast<i32>(y));
        return *this;
    }

    SND_INLINE vec2& vec2::Lerp(const vec2& v1, const vec2& v2, f32 alpha)
    {
        if (alpha <= 0.0f)
        {
            *this = v1;
        }
        else if (alpha >= 1.0f)
        {
            *this = v2;
        }
        else
        {
            *this = v1 + alpha * (v2 - v1);
        }
        
        return *this;
    }

    struct vec3
    {
        f32             x;
        f32             y;
        f32             z;

                        vec3();
        explicit        vec3(f32 a);
        explicit        vec3(f32 a, f32 b, f32 c);

        f32			    operator[](i32 index) const;
        f32&			operator[](i32 index);
        vec3			operator-() const;
        vec3			operator+(const vec3& a) const;
        vec3			operator-(const vec3& a) const;
        f32			    operator*(const vec3& a) const;
        vec3			operator*(f32 a) const;
        vec3			operator/(f32 a) const;
        vec3&		    operator+=(const vec3& a);
        vec3&		    operator-=(const vec3& a);
        vec3&		    operator/=(const vec3& a);
        vec3&		    operator/=(f32 a);
        vec3&		    operator*=(f32 a);
        
        bool			operator==(const vec3& a) const;
        bool			operator!=(const vec3& a) const;

        friend vec3	    operator*(f32 a, vec3 b);

        vec3& 			Set(f32 a, f32 b, f32 c);
        vec3&			Zero();
        
        bool			Equal(const vec3& a) const;
        bool			Equal(const vec3& a, f32 epsilon) const;

        f32			    Length() const;
        f32			    LengthSqr() const;
        f32             Dot(const vec3& a) const;
        vec3            Cross(const vec3& a) const;
        vec3&           Normalize();            // normalize vector
        vec3&		    Truncate(f32 length);   // cap vector length
        
        i32				Dimension() const;

        f32			    Yaw() const;
        f32			    Pitch() const;
        euler		    EulerAngles() const;
        const vec2&	    Vec2() const;
        vec2&		    Vec2();
        
        f32*			Ptr();
        const f32*	    Ptr() const;
        const char*	    String(i32 precision = 2) const;

        vec3&			Clamp(const vec3& min, const vec3& max);
        vec3&			Snap();                 // snap to closest integer value
        vec3&			Lerp(const vec3& v1, const vec3& v2, f32 alpha);
    };

    SND_INLINE vec3::vec3()
        : x(0.0f), y(0.0f), z(0.0f)
    {
    }

    SND_INLINE vec3::vec3(f32 a)
        : x(a), y(a), z(a)
    {
    }

    SND_INLINE vec3::vec3(f32 a, f32 b, f32 c)
        : x(a), y(b), z(c)
    {
    }

    SND_INLINE f32 vec3::operator[](i32 index) const
    {
        return (&x)[index];
    }

    SND_INLINE f32& vec3::operator[](i32 index)
    {
        return (&x)[index];
    }

    SND_INLINE vec3 vec3::operator-() const
    {
        return vec3(-x, -y, -z);
    }

    SND_INLINE vec3 vec3::operator+(const vec3& a) const
    {
        return vec3(x + a.x, y + a.y, z + a.z);
    }

    SND_INLINE vec3 vec3::operator-(const vec3& a) const
    {
        return vec3(x - a.x, y - a.y, z - a.z);
    }

    SND_INLINE f32 vec3::operator*(const vec3& a) const
    {
        return Dot(a);
    }

    SND_INLINE vec3 vec3::operator*(f32 a) const
    {
        return vec3(x * a, y * a, z * a);
    }

    SND_INLINE vec3 vec3::operator/(f32 a) const
    {
        const f32 inv = 1.0f / a;
        return vec3(x * inv, y * inv, z * inv);
    }

    SND_INLINE vec3& vec3::operator+=(const vec3& a)
    {
        x += a.x;
        y += a.y;
        z += a.z;
        return *this;
    }

    SND_INLINE vec3& vec3::operator-=(const vec3& a)
    {
        x -= a.x;
        y -= a.y;
        z -= a.z;
        return *this;
    }

    SND_INLINE vec3& vec3::operator/=(const vec3& a)
    {
        x /= a.x;
        y /= a.y;
        z /= a.z;
        return *this;
    }

    SND_INLINE vec3& vec3::operator/=(f32 a)
    {
        const f32 inv = 1.0f / a;
        x *= inv;
        y *= inv;
        z *= inv;
        return *this;
    }

    SND_INLINE vec3& vec3::operator*=(f32 a)
    {
        x *= a;
        y *= a;
        z *= a;
        return *this;
    }

    SND_INLINE bool vec3::operator==(const vec3& a) const
    {
        return Equal(a);
    }

    SND_INLINE bool vec3::operator!=(const vec3& a) const
    {
        return !(*this == a);
    }

    SND_INLINE vec3 operator*(f32 a, vec3 b)
    {
        return vec3(b.x * a, b.y * a, b.z * a);
    }

    SND_INLINE vec3& vec3::Set(f32 a, f32 b, f32 c)
    {
        x = a;
        y = b;
        z = c;
        return *this;
    }

    SND_INLINE vec3& vec3::Zero()
    {
        x = y = z = 0.0f;
        return *this;
    }

    SND_INLINE bool vec3::Equal(const vec3& a) const
    {
        return (x == a.x) && (y == a.y) && (z == a.z);
    }

    SND_INLINE bool vec3::Equal(const vec3& a, f32 epsilon) const
    {
        return math::Absf(x - a.x) <= epsilon &&
               math::Absf(y - a.y) <= epsilon &&
               math::Absf(z - a.z) <= epsilon;
    }

    SND_INLINE f32 vec3::Length() const
    {
        return math::Sqrt(x * x + y * y + z * z);
    }

    SND_INLINE f32 vec3::LengthSqr() const
    {
        return x * x + y * y + z * z;
    }

    SND_INLINE f32 vec3::Dot(const vec3& a) const
    {
        return x * a.x + y * a.y + z * a.z;
    }

    SND_INLINE vec3 vec3::Cross(const vec3& a) const
    {
        return vec3(y * a.z - z * a.y, z * a.x - x * a.z, x * a.y - y * a.x);
    }

    SND_INLINE vec3& vec3::Normalize()
    {
        const f32 lengthInv = math::InvSqrt(x * x + y * y + z * z);

        x *= lengthInv;
        y *= lengthInv;
        z *= lengthInv;
        
        return *this;
    }

    SND_INLINE vec3& vec3::Truncate(f32 length)
    {
        if (length == 0.0f)
        {
            Zero();
            return *this;
        }
        
        const f32 lengthSquare = x * x + y * y + z * z;
        if (lengthSquare > length * length)
        {
            const f32 scaleFactor = length * math::InvSqrt(lengthSquare);
            x *= scaleFactor;
            y *= scaleFactor;
            z *= scaleFactor;
        }
        
        return *this;
    }

    SND_INLINE i32 vec3::Dimension() const
    {
        return 3;
    }

    SND_INLINE f32 vec3::Yaw() const
    {
    }

    SND_INLINE f32 vec3::Pitch() const
    {
    }

    SND_INLINE euler vec3::EulerAngles() const
    {
    }

    SND_INLINE const vec2& vec3::Vec2() const
    {
	    return *reinterpret_cast<const vec2*>(this);
    }

    SND_INLINE vec2& vec3::Vec2()
    {
        return *reinterpret_cast<vec2*>(this);
    }

    SND_INLINE f32* vec3::Ptr()
    {
        return &x;
    }

    SND_INLINE const f32* vec3::Ptr() const
    {
        return &x;
    }

    SND_INLINE const char* vec3::String(i32 precision) const
    {
    }

    SND_INLINE vec3& vec3::Clamp(const vec3& min, const vec3& max)
    {
        x = std::clamp(x, min.x, max.x);
        y = std::clamp(y, min.y, max.y);
        z = std::clamp(z, min.z, max.z);
        return *this;
    }

    SND_INLINE vec3& vec3::Snap()
    {
        x = static_cast<f32>(static_cast<i32>(x));
        y = static_cast<f32>(static_cast<i32>(y));
        z = static_cast<f32>(static_cast<i32>(z));
        return *this;
    }

    SND_INLINE vec3& vec3::Lerp(const vec3& v1, const vec3& v2, f32 alpha)
    {
        if (alpha <= 0.0f)
        {
            *this = v1;
        }
        else if (alpha >= 1.0f)
        {
            *this = v2;
        }
        else
        {
            *this = v1 + alpha * (v2 - v1);
        }

        return *this;
    }

    struct vec4
    {
        f32             x;
        f32             y;
        f32             z;
        f32             w;

                        vec4();
        explicit        vec4(f32 a);
        explicit        vec4(f32 a, f32 b, f32 c, f32 d);

        f32			    operator[](i32 index) const;
        f32&			operator[](i32 index);
        vec4			operator-() const;
        vec4			operator+(const vec4& a) const;
        vec4			operator-(const vec4& a) const;
        f32			    operator*(const vec4& a) const;
        vec4			operator*(f32 a) const;
        vec4			operator/(f32 a) const;
        vec4&		    operator+=(const vec4& a);
        vec4&		    operator-=(const vec4& a);
        vec4&		    operator/=(const vec4& a);
        vec4&		    operator/=(f32 a);
        vec4&		    operator*=(f32 a);
        
        bool			operator==(const vec4& a) const;
        bool			operator!=(const vec4& a) const;

        friend vec4	    operator*(f32 a, vec4 b);

        vec4& 			Set(f32 a, f32 b, f32 c, f32 d);
        vec4&			Zero();
        
        bool			Equal(const vec4& a) const;
        bool			Equal(const vec4& a, f32 epsilon) const;

        f32			    Length() const;
        f32			    LengthSqr() const;
        f32             Dot(const vec4& a) const;
        vec4&           Normalize();            // normalize vector
        
        i32				Dimension() const;

        const vec2&	    Vec2() const;
        vec2&		    Vec2();
        const vec3&	    Vec3() const;
        vec3&		    Vec3();
        
        f32*			Ptr();
        const f32*	    Ptr() const;
        const char*	    String(i32 precision = 2) const;

        vec4&			Lerp(const vec4& v1, const vec4& v2, f32 alpha);
    };

    SND_INLINE vec4::vec4()
        : x(0.0f), y(0.0f), z(0.0f), w(0.0f)
    {
    }

    SND_INLINE vec4::vec4(f32 a)
        : x(a), y(a), z(a), w(a)
    {
        
    }

    SND_INLINE vec4::vec4(f32 a, f32 b, f32 c, f32 d)
        : x(a), y(b), z(c), w(d)
    {
    }

    SND_INLINE f32 vec4::operator[](i32 index) const
    {
        return (&x)[index];
    }

    SND_INLINE f32& vec4::operator[](i32 index)
    {
        return (&x)[index];
    }

    SND_INLINE vec4 vec4::operator-() const
    {
        return vec4(-x, -y, -z, -w);
    }

    SND_INLINE vec4 vec4::operator+(const vec4& a) const
    {
        return vec4(x + a.x, y + a.y, z + a.z, w + a.w);
    }

    SND_INLINE vec4 vec4::operator-(const vec4& a) const
    {
        return vec4(x - a.x, y - a.y, z - a.z, w - a.w);
    }

    SND_INLINE f32 vec4::operator*(const vec4& a) const
    {
        return Dot(a);
    }

    SND_INLINE vec4 vec4::operator*(f32 a) const
    {
        return vec4(x * a, y * a, z * a, w * a);
    }

    SND_INLINE vec4 vec4::operator/(f32 a) const
    {
        const f32 inv = 1.0f / a;
        return vec4(x * inv, y * inv, z * inv, w * inv);
    }

    SND_INLINE vec4& vec4::operator+=(const vec4& a)
    {
        x += a.x;
        y += a.y;
        z += a.z;
        w += a.w;
        return *this;
    }

    SND_INLINE vec4& vec4::operator-=(const vec4& a)
    {
        x -= a.x;
        y -= a.y;
        z -= a.z;
        w -= a.w;
        return *this;
    }

    SND_INLINE vec4& vec4::operator/=(const vec4& a)
    {
        x /= a.x;
        y /= a.y;
        z /= a.z;
        w /= a.w;
        return *this;
    }

    SND_INLINE vec4& vec4::operator/=(f32 a)
    {
        const f32 inv = 1.0f / a;
        x *= inv;
        y *= inv;
        z *= inv;
        w *= inv;
        return *this;
    }

    SND_INLINE vec4& vec4::operator*=(f32 a)
    {
        x *= a;
        y *= a;
        z *= a;
        w *= a;
        return *this;
    }

    SND_INLINE bool vec4::operator==(const vec4& a) const
    {
        return Equal(a);
    }

    SND_INLINE bool vec4::operator!=(const vec4& a) const
    {
        return !(*this == a);
    }

    SND_INLINE vec4 operator*(f32 a, vec4 b)
    {
        return vec4(b.x * a, b.y * a, b.z * a, b.w * a);
    }

    SND_INLINE vec4& vec4::Set(f32 a, f32 b, f32 c, f32 d)
    {
        x = a;
        y = b;
        z = c;
        w = d;
        return *this;
    }

    SND_INLINE vec4& vec4::Zero()
    {
        x = y = z = w = 0.0f;
        return *this;
    }

    SND_INLINE bool vec4::Equal(const vec4& a) const
    {
        return (x == a.x) && (y == a.y) && (z == a.z) && (w == a.w);
    }

    SND_INLINE bool vec4::Equal(const vec4& a, f32 epsilon) const
    {
        return math::Absf(x - a.x) <= epsilon &&
               math::Absf(y - a.y) <= epsilon &&
               math::Absf(z - a.z) <= epsilon &&
               math::Absf(w - a.w) <= epsilon;
    }

    SND_INLINE f32 vec4::Length() const
    {
        return math::Sqrt(x * x + y * y + z * z + w * w);
    }

    SND_INLINE f32 vec4::LengthSqr() const
    {
        return x * x + y * y + z * z + w * w;
    }

    SND_INLINE f32 vec4::Dot(const vec4& a) const
    {
        return x * a.x + y * a.y + z * a.z + w * a.w;
    }

    SND_INLINE vec4& vec4::Normalize()
    {
        const f32 lengthInv = math::InvSqrt(x * x + y * y + z * z + w * w);

        x *= lengthInv;
        y *= lengthInv;
        z *= lengthInv;
        w *= lengthInv;
        
        return *this;
    }

    SND_INLINE i32 vec4::Dimension() const
    {
        return 4;
    }

    SND_INLINE const vec2& vec4::Vec2() const
    {
	    return *reinterpret_cast<const vec2*>(this);
    }

    SND_INLINE vec2& vec4::Vec2()
    {
        return *reinterpret_cast<vec2*>(this);
    }

    SND_INLINE const vec3& vec4::Vec3() const
    {
        return *reinterpret_cast<const vec3*>(this);
    }

    SND_INLINE vec3& vec4::Vec3()
    {
        return *reinterpret_cast<vec3*>(this);
    }

    SND_INLINE f32* vec4::Ptr()
    {
        return &x;
    }

    SND_INLINE const f32* vec4::Ptr() const
    {
        return &x;
    }

    SND_INLINE const char* vec4::String(i32 precision) const
    {
    }

    SND_INLINE vec4& vec4::Lerp(const vec4& v1, const vec4& v2, f32 alpha)
    {
        if (alpha <= 0.0f)
        {
            *this = v1;
        }
        else if (alpha >= 1.0f)
        {
            *this = v2;
        }
        else
        {
            *this = v1 + alpha * (v2 - v1);
        }

        return *this;
    }
}
