#pragma once

#include "RE_Settings.h"

/// @ingroup RE_Utils
/// @brief Structure représentant un vecteur 2D dont les composantes sont de type float.
class Vec2
{
public:
    /// @brief Abscisse du vecteur.
    float x;

    /// @brief Ordonnée du vecteur.
    float y;

    Vec2() : x(0), y(0) {}
    Vec2(float x, float y) : x(x), y(y) {}
    Vec2(const Vec2 &v) : x(v.x), y(v.y) {}

    void Set(float xIn, float yIn) { x = xIn; y = yIn; }


    /// Read from and indexed element.
    float operator () (int32_t i) const
    {
        assert(0 <= i && i < 2);
        return (&x)[i];
    }

    /// Write to an indexed element.
    float& operator () (int32_t i)
    {
        assert(0 <= i && i < 2);
        return (&x)[i];
    }

    Vec2 operator - () const
    {
        return Vec2(-x, -y);
    }

    /// Add a vector to this vector.
    void operator += (const Vec2& v)
    {
        x += v.x; y += v.y;
    }

    /// Subtract a vector from this vector.
    void operator -= (const Vec2& v)
    {
        x -= v.x; y -= v.y;
    }

    /// Multiply this vector by a scalar.
    void operator *= (float s)
    {
        x *= s; y *= s;
    }

    static Vec2 Lerp(Vec2 from, Vec2 to, float t)
    {
        t = fmaxf(0.0f, fminf(t, 1.0f));
        from *= 1.0f - t;
        to *= t;
        from += to;
        return from;
    }
};

/// Add two vectors component-wise.
inline Vec2 operator + (const Vec2& a, const Vec2& b)
{
    return Vec2(a.x + b.x, a.y + b.y);
}

/// Subtract two vectors component-wise.
inline Vec2 operator - (const Vec2& a, const Vec2& b)
{
    return Vec2(a.x - b.x, a.y - b.y);
}

inline Vec2 operator * (float s, const Vec2& a)
{
    return Vec2(s * a.x, s * a.y);
}

inline bool operator == (const Vec2& a, const Vec2& b)
{
    return a.x == b.x && a.y == b.y;
}

inline bool operator != (const Vec2& a, const Vec2& b)
{
    return a.x != b.x || a.y != b.y;
}
