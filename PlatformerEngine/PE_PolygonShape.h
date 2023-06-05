#pragma once

#include "PE_Settings.h"
#include "PE_Math.h"
#include "PE_Shape.h"

class PE_PolygonShape : public PE_Shape
{
public:
    PE_PolygonShape();
    PE_PolygonShape(const PE_Vec2 *vertices, int vertexCount);
    PE_PolygonShape(float xLower, float yLower, float xUpper, float yUpper);
    virtual ~PE_PolygonShape();

    void SetVertices(const PE_Vec2 *vertices, int vertexCount);
    void SetAsBox(const PE_AABB &box);
    void SetAsBox(float xLower, float yLower, float xUpper, float yUpper);

    virtual void GetAABB(const PE_Vec2 &transform, PE_AABB &aabb) const override;

    virtual bool RayCast(
        const PE_Vec2 &transform,
        const PE_RayCastInput &input,
        PE_RayCastOutput &output) const override;

    const PE_Vec2 *GetVertices() const;
    int GetVertexCount() const;

protected:
    friend class PE_Manifold;
    friend class PE_DistanceProxy;

    virtual PE_Shape *Clone(PE_Allocator &allocator) const override;

    PE_Vec2 m_vertices[PE_MAX_POLYGON_VERTICES];
    PE_Vec2 m_normals[PE_MAX_POLYGON_VERTICES];
    int m_vertexCount;
    PE_AABB m_aabb;
};

inline void PE_PolygonShape::GetAABB(const PE_Vec2 &transform, PE_AABB &aabb) const
{
    aabb = m_aabb;
    aabb.Translate(transform);
}

inline const PE_Vec2 *PE_PolygonShape::GetVertices() const
{
    return m_vertices;
}

inline int PE_PolygonShape::GetVertexCount() const
{
    return m_vertexCount;
}
