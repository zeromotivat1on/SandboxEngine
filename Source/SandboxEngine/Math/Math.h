#pragma once

namespace snd::math
{
    inline constexpr f32 PI = 3.14159265358979323846f;
    
    i32     Sign(f32 n);
    i32     Abs (i32 n);
    f32     Absf(f32 n);
    f32     Sqrt(f32 n);
    f32     InvSqrt(f32 n);

    f32     Rad(f32 d);
    f32     Deg(f32 r);
    
    f32     Cos(f32 r);
    f32     Sin(f32 r);
    f32     Tan(f32 r);
    
    // Math implementation
    
    SND_INLINE i32 Sign(f32 n)
    {
        return (n > 0) - (n < 0);
    }

    SND_INLINE i32 Abs(i32 n)
    {
        const i32 sign = n >> 31;
        return (n ^ sign) - sign;
    }

    SND_INLINE f32 Absf(f32 n)
    {
        i32 tmp = *reinterpret_cast<i32*>(&n);
        tmp &= 0x7FFFFFFF; // clear sign bit
        return *reinterpret_cast<f32*>(&tmp);
    }

    SND_INLINE f32 Sqrt(f32 n)
    {
        return sqrtf(n);
    }

    SND_INLINE f32 InvSqrt(f32 n)
    {
        return 1 / sqrtf(n);
    }

    SND_INLINE f32 Rad(f32 d)
    {
        return d * (PI / 180.0f);
    }

    SND_INLINE f32 Deg(f32 r)
    {
        return r * (180.0f / PI);
    }

    SND_INLINE f32 Cos(f32 r)
    {
        return cosf(r);
    }

    SND_INLINE f32 Sin(f32 r)
    {
        return sinf(r);
    }

    SND_INLINE f32 Tan(f32 r)
    {
        return tanf(r);
    }
}
