#pragma once

#include "PE_Settings.h"
#include "PE_Math.h"
#include "PE_Shape.h"

class PE_CapsuleShape : public PE_Shape
{
public:
    PE_CapsuleShape(const PE_Vec2 &vertexA, const PE_Vec2 &vertexB, float radius);
    virtual ~PE_CapsuleShape();

    virtual void GetAABB(const PE_Vec2 &transform, PE_AABB &aabb) const override;

    virtual bool RayCast(
        const PE_Vec2 &transform,
        const PE_RayCastInput &input,
        PE_RayCastOutput &output) const override;

    const PE_Vec2 *GetVertices() const;
    float GetRadius() const;

protected:
    friend class PE_Manifold;
    friend class PE_DistanceProxy;

    virtual PE_Shape *Clone(PE_Allocator &allocator) const override;

    PE_Vec2 m_vertices[2];
    PE_Vec2 m_normals[2];
    float m_radius;
    PE_AABB m_aabb;
};

inline void PE_CapsuleShape::GetAABB(const PE_Vec2 &transform, PE_AABB &aabb) const
{
    aabb = m_aabb;
    aabb.Translate(transform);
}

inline const PE_Vec2 *PE_CapsuleShape::GetVertices() const
{
    return m_vertices;
}

inline float PE_CapsuleShape::GetRadius() const
{
    return m_radius;
}
