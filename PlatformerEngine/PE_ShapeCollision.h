#pragma once

#include "PE_Settings.h"
#include "PE_Shape.h"
#include "PE_PolygonShape.h"
#include "PE_CircleShape.h"
#include "PE_CapsuleShape.h"
#include "PE_Math.h"

/// @brief Structure stockant un contact entre deux formes.
class PE_Manifold
{
public:
    PE_Manifold();

    PE_Vec2 normal;
    float depth;
    bool overlap;

    void Collide(
        const PE_Shape &shapeA, PE_Vec2 transformA,
        const PE_Shape &shapeB, PE_Vec2 transformB
    );
    void Reverse();
    void Reset();

private:
    void CollidePolygons(
        const PE_PolygonShape &polygonA, PE_Vec2 transformA,
        const PE_PolygonShape &polygonB, PE_Vec2 transformB
    );
    void CollideCapsules(
        const PE_CapsuleShape &capsuleA, PE_Vec2 transformA,
        const PE_CapsuleShape &capsuleB, PE_Vec2 transformB
    );
    void CollideCircles(
        const PE_CircleShape &circleA, PE_Vec2 transformA,
        const PE_CircleShape &circleB, PE_Vec2 transformB
    );
    void CollidePolygonAndCircle(
        const PE_PolygonShape &polygonA, PE_Vec2 transformA,
        const PE_CircleShape &circleB, PE_Vec2 transformB
    );
    void CollideCapsuleAndCircle(
        const PE_CapsuleShape &capsuleA, PE_Vec2 transformA,
        const PE_CircleShape &circleB, PE_Vec2 transformB
    );
    void CollidePolygonAndCapsule(
        const PE_PolygonShape &polygonA, PE_Vec2 transformA,
        const PE_CapsuleShape &capsuleB, PE_Vec2 transformB
    );
    void CollideBoxes(
        const PE_PolygonShape &polygonA, PE_Vec2 transformA,
        const PE_PolygonShape &polygonB, PE_Vec2 transformB
    );
};

inline PE_Manifold::PE_Manifold() :
    overlap(false), depth(-1.0f), normal(PE_Vec2::up)
{
}

inline void PE_Manifold::Reverse()
{
    normal = -normal;
}

inline void PE_Manifold::Reset()
{
    overlap = false;
    depth = -1.0f;
}
