#pragma once

#include "PE_Settings.h"
#include "PE_Math.h"
#include "PE_Allocator.h"

/// Ray-cast input data. The ray extends from p1 to p1 + maxFraction * (p2 - p1).
struct PE_RayCastInput
{
    PE_Vec2 p1, p2;
    float maxFraction;
};

/// Ray-cast output data. The ray hits at p1 + fraction * (p2 - p1), where p1 and p2
/// come from b2RayCastInput.
struct PE_RayCastOutput
{
    PE_Vec2 normal;
    float fraction;
};

/// @ingroup PE_Shape
/// @brief Structure représentant la forme d'un collider
class PE_Shape
{
public:
    virtual ~PE_Shape();

    enum class Type : int
    {
        CIRCLE, POLYGON, POLYGON_BOX, CAPSULE
    };

    virtual void GetAABB(const PE_Vec2 &transform, PE_AABB &aabb) const = 0;

    virtual bool RayCast(
        const PE_Vec2 &transform,
        const PE_RayCastInput &input,
        PE_RayCastOutput &output) const = 0;

    float GetSkin() const;
    PE_Shape::Type GetType() const;

protected:
    friend class PE_Collider;

    PE_Shape(PE_Shape::Type type);
    virtual PE_Shape *Clone(PE_Allocator &allocator) const = 0;

    Type m_type;
    float m_skin;
};

inline PE_Shape::~PE_Shape()
{
}

inline float PE_Shape::GetSkin() const
{
    return m_skin;
}

inline PE_Shape::Type PE_Shape::GetType() const
{
    return m_type;
}

inline PE_Shape::PE_Shape(PE_Shape::Type type) :
    m_skin(PE_SKIN_RADIUS), m_type(type)
{
}


