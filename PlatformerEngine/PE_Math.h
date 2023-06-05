#pragma once

#include "PE_Settings.h"

/// @ingroup PE_Math
/// @brief Structure représentant un vecteur 2D dont les composantes sont de type float.
class PE_Vec2
{
public:
    /// @brief Abscisse du vecteur.
    float x;

    /// @brief Ordonnée du vecteur.
    float y;

    PE_Vec2();
    PE_Vec2(float x, float y);
    PE_Vec2(const PE_Vec2 &v);

    static const PE_Vec2 up;
    static const PE_Vec2 down;
    static const PE_Vec2 left;
    static const PE_Vec2 right;
    static const PE_Vec2 zero;
    static const PE_Vec2 one;

    void Set(float x, float y);
    void SetZero();
    PE_Vec2 Abs() const;

    float Length() const;
    float LengthSquared() const;

    PE_Vec2 Perp() const;
    PE_Vec2 PerpNeg() const;
    PE_Vec2 PerpDir(const PE_Vec2 &direction) const;

    PE_Vec2 &Normalize(); 

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

    PE_Vec2 operator - () const
    {
        return PE_Vec2(-x, -y);
    }

    /// Add a vector to this vector.
    void operator += (const PE_Vec2& v)
    {
        x += v.x; y += v.y;
    }

    /// Subtract a vector from this vector.
    void operator -= (const PE_Vec2& v)
    {
        x -= v.x; y -= v.y;
    }

    /// Multiply this vector by a scalar.
    void operator *= (float s)
    {
        x *= s; y *= s;
    }
};

/// Add two vectors component-wise.
inline PE_Vec2 operator + (const PE_Vec2& a, const PE_Vec2& b)
{
    return PE_Vec2(a.x + b.x, a.y + b.y);
}

/// Subtract two vectors component-wise.
inline PE_Vec2 operator - (const PE_Vec2& a, const PE_Vec2& b)
{
    return PE_Vec2(a.x - b.x, a.y - b.y);
}

inline PE_Vec2 operator * (float s, const PE_Vec2& a)
{
    return PE_Vec2(s * a.x, s * a.y);
}

inline bool operator == (const PE_Vec2& a, const PE_Vec2& b)
{
    return a.x == b.x && a.y == b.y;
}

inline bool operator != (const PE_Vec2& a, const PE_Vec2& b)
{
    return a.x != b.x || a.y != b.y;
}

inline float PE_Dot(const PE_Vec2 &a, const PE_Vec2 &b)
{
    return a.x * b.x + a.y * b.y;
}

inline float PE_Det(const PE_Vec2& a, const PE_Vec2& b)
{
    return a.x * b.y - a.y * b.x;
}

inline float PE_Distance(const PE_Vec2& a, const PE_Vec2& b)
{
    PE_Vec2 c = a - b;
    return c.Length();
}

inline float PE_DistanceSquared(const PE_Vec2& a, const PE_Vec2& b)
{
    PE_Vec2 c = a - b;
    return PE_Dot(c, c);
}

inline PE_Vec2::PE_Vec2() : x(0.0f), y(0.0f) {}
inline PE_Vec2::PE_Vec2(float x, float y) : x(x), y(y) {}
inline PE_Vec2::PE_Vec2(const PE_Vec2 &v) : x(v.x), y(v.y) {}

inline void PE_Vec2::Set(float vx, float vy)
{
    this->x = vx;
    this->y = vy;
}

inline void PE_Vec2::SetZero()
{
    x = 0.0f;
    y = 0.0f;
}

inline PE_Vec2 PE_Vec2::Abs() const
{
    return PE_Vec2(fabsf(x), fabsf(y));
}

inline float PE_Vec2::Length() const
{
    return sqrtf(LengthSquared());
}

inline float PE_Vec2::LengthSquared() const
{
    return x * x + y * y;
}

inline PE_Vec2 PE_Vec2::Perp() const
{
    return PE_Vec2(-y, x);
}

inline PE_Vec2 PE_Vec2::PerpNeg() const
{
    return PE_Vec2(y, -x);
}

inline PE_Vec2 PE_Vec2::PerpDir(const PE_Vec2 &direction) const
{
    PE_Vec2 perp = Perp();
    return (PE_Dot(perp, direction) >= 0.0f) ? perp : -perp;
}

inline PE_Vec2 &PE_Vec2::Normalize()
{
    float norm = Length();
    if (norm >= 0.0f)
    {
        float invNorm = 1.0f / Length();
        x *= invNorm;
        y *= invNorm;
    }
    return *this;
}

template <typename T>
inline T PE_Min(T a, T b)
{
    return a < b ? a : b;
}

inline PE_Vec2 PE_Min(const PE_Vec2& a, const PE_Vec2& b)
{
    return PE_Vec2(PE_Min(a.x, b.x), PE_Min(a.y, b.y));
}

template <typename T>
inline T PE_Max(T a, T b)
{
    return a > b ? a : b;
}

inline PE_Vec2 PE_Max(const PE_Vec2& a, const PE_Vec2& b)
{
    return PE_Vec2(PE_Max(a.x, b.x), PE_Max(a.y, b.y));
}

template <typename T>
inline T PE_Clamp(T value, T a, T b)
{
    return PE_Max(a, PE_Min(value, b));
}

float PE_SignedAngleDeg(PE_Vec2 from, PE_Vec2 to);
float PE_SignedAngleRad(PE_Vec2 from, PE_Vec2 to);
float PE_AngleDeg(PE_Vec2 from, PE_Vec2 to);
float PE_AngleRad(PE_Vec2 from, PE_Vec2 to);

template <typename T>
inline T PE_Lerp(T from, T to, float t)
{
    t = PE_Clamp(t, 0.0f, 1.0f);
    return (1.0f - t) * from + t * to;
}

template <typename T>
inline T PE_LerpUnclamped(T from, T to, float t)
{
    return (1.0f - t) * from + t * to;
}

struct PE_Mat2
{
    PE_Vec2 cols[2];
};

inline PE_Vec2 PE_Mat2_MulMV(PE_Mat2 mat, PE_Vec2 v)
{
    PE_Vec2 res;
    res.x = mat.cols[0].x * v.x + mat.cols[1].x * v.y;
    res.y = mat.cols[0].y * v.x + mat.cols[1].y * v.y;
    return res;
}

inline PE_Vec2 PE_Mat2_MulMVT(PE_Mat2 mat, PE_Vec2 v)
{
    PE_Vec2 res;
    res.x = mat.cols[0].x * v.x + mat.cols[0].y * v.y;
    res.y = mat.cols[1].x * v.x + mat.cols[1].y * v.y;
    return res;
}

/// @ingroup PE_Math
/// @brief Structure représentant une boîte englobante (rectangle) dont les côtés sont alignés sur les axes.
/// AABB signifie "Aligned Axes Bounding Box".
class PE_AABB
{
public:
    PE_AABB();
    PE_AABB(const PE_AABB &aabb);
    PE_AABB(const PE_Vec2 &lower, const PE_Vec2 &upper);
    PE_AABB(float xLower, float yLower, float xUpper, float yUpper);
    ~PE_AABB();

    /// @brief Coordonnées du coin inférieur gauche de la boîte.
    PE_Vec2 lower;

    /// @brief Coordonnées du coin supérieur droit de la boîte.
    PE_Vec2 upper;

    void Set(const PE_Vec2 &lower, const PE_Vec2 &upper);
    void Set(float xLower, float yLower, float xUpper, float yUpper);
    void Translate(const PE_Vec2 &transform);

    bool Overlap(const PE_AABB &aabb) const;
    bool Contains(const PE_AABB &aabb) const;
    bool Contains(const PE_Vec2 &point) const;

    /// Combine an AABB into this one.
    void Combine(const PE_AABB &aabb);

    /// Combine two AABBs into this one.
    void Combine(const PE_AABB &aabb1, const PE_AABB &aabb2);
    void Extend(float value);
    float GetPerimeter() const;

    float GetWidth() const;
    float GetHeight() const;
    PE_Vec2 GetCenter() const;
    void Scale(const PE_Vec2 &center, float scale);
};

inline PE_AABB::PE_AABB() :
    lower(), upper()
{
}

inline PE_AABB::PE_AABB(const PE_AABB &aabb) :
    lower(aabb.lower), upper(aabb.upper)
{
}

inline PE_AABB::PE_AABB(const PE_Vec2 &vLower, const PE_Vec2 &vUpper) :
    lower(vLower), upper(vUpper)
{
}

inline PE_AABB::PE_AABB(float xLower, float yLower, float xUpper, float yUpper) :
    lower(xLower, yLower), upper(xUpper, yUpper)
{
}

inline PE_AABB::~PE_AABB()
{
}

inline void PE_AABB::Set(const PE_Vec2 &vLower, const PE_Vec2 &vUpper)
{
    lower = vLower;
    upper = vUpper;
}

inline void PE_AABB::Set(float xLower, float yLower, float xUpper, float yUpper)
{
    lower.Set(xLower, yLower);
    upper.Set(xUpper, yUpper);
}

inline void PE_AABB::Translate(const PE_Vec2 &transform)
{
    lower += transform;
    upper += transform;
}

inline void PE_AABB::Combine(const PE_AABB &aabb)
{
    lower = PE_Min(lower, aabb.lower);
    upper = PE_Max(upper, aabb.upper);
}

inline void PE_AABB::Combine(const PE_AABB &aabb1, const PE_AABB &aabb2)
{
    lower = PE_Min(aabb1.lower, aabb2.lower);
    upper = PE_Max(aabb1.upper, aabb2.upper);
}

inline void PE_AABB::Extend(float value)
{
    lower.x -= value; lower.y -= value;
    upper.x += value; upper.y += value;
}

inline float PE_AABB::GetWidth() const
{
    return upper.x - lower.x;
}

inline float PE_AABB::GetHeight() const
{
    return upper.y - lower.y;
}

inline PE_Vec2 PE_AABB::GetCenter() const
{
    return 0.5f * (lower + upper);
}

inline void PE_AABB::Scale(const PE_Vec2 &center, float scale)
{
    lower = center + (scale * (lower - center));
    upper = center + (scale * (upper - center));
}

float PE_Distance(const PE_AABB &a, const PE_AABB &b);

/// @brief Structure décrivant le mouvement d'un point pendant la mise à jour du monde.
class PE_Sweep
{
public:
    /// @brief Position au ratio alpha0
    PE_Vec2 position0;

    /// @brief Position finale.
    PE_Vec2 position1;

    /// @brief Ratio entre la position avant et après la mise à jour du monde.
    float alpha0;

    PE_Vec2 GetTransform(float beta) const;
    void Advance(float alpha);

    static float Synchronize(PE_Sweep &sweepA, PE_Sweep &sweepB);
};

inline PE_Vec2 PE_Sweep::GetTransform(float beta) const
{
    return ((1.0f - beta) * position0) + (beta * position1);
}

inline void PE_Sweep::Advance(float alpha)
{
    float beta = (alpha - alpha0) / (1.0f - alpha0);
    position0 = GetTransform(beta);
    alpha0 = alpha;
}

inline float PE_Sweep::Synchronize(PE_Sweep &sweepA, PE_Sweep &sweepB)
{
    float alpha = sweepA.alpha0;
    if (sweepA.alpha0 < sweepB.alpha0)
    {
        alpha = sweepB.alpha0;
        sweepA.Advance(alpha);
    }
    else if (sweepB.alpha0 < sweepA.alpha0)
    {
        alpha = sweepA.alpha0;
        sweepB.Advance(alpha);
    }
    return alpha;
}

bool PE_LineIntersect(
    PE_Vec2 p1, PE_Vec2 vec1,
    PE_Vec2 p2, PE_Vec2 vec2, PE_Vec2 *intersection);
