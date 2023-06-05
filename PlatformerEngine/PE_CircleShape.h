#pragma once

#include "PE_Settings.h"
#include "PE_Math.h"
#include "PE_Shape.h"

class PE_CircleShape : public PE_Shape
{
public:
    PE_CircleShape(const PE_Vec2 &center, float radius);
    virtual ~PE_CircleShape();

    virtual void GetAABB(const PE_Vec2 &transform, PE_AABB &aabb) const override;

    virtual bool RayCast(
        const PE_Vec2 &transform,
        const PE_RayCastInput &input,
        PE_RayCastOutput &output) const override;

    PE_Vec2 GetCenter() const;
    float GetRadius() const;

protected:
    friend class PE_Manifold;
    friend class PE_DistanceProxy;

    virtual PE_Shape *Clone(PE_Allocator &allocator) const override;

    PE_Vec2 m_center;
    float m_radius;
};

inline void PE_CircleShape::GetAABB(const PE_Vec2 &transform, PE_AABB &aabb) const
{
    PE_Vec2 center = m_center + transform;
    aabb.Set(center, center);
    aabb.Extend(m_radius + m_skin);
}

inline PE_Vec2 PE_CircleShape::GetCenter() const
{
    return m_center;
}

inline float PE_CircleShape::GetRadius() const
{
    return m_radius;
}
