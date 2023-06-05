#pragma once

#include "PE_Shape.h"

#define PE_GJK_ITERS 20

/// A distance proxy is used by the GJK algorithm.
/// It encapsulates any shape.
class PE_DistanceProxy
{
public:
    PE_DistanceProxy() : radius(0.0f), count(0) {}

    float radius;
    int count;
    PE_Vec2 vertices[PE_MAX_POLYGON_VERTICES];
    // pas de m_buffer (chainShape)

    void Set(const PE_Shape &shape);
};

/// Used to warm start b2Distance.
/// Set count to zero on first call.
struct PE_SimplexCache
{
    float metric; ///< length or area
    int count;
    int indicesA[3];///< vertices on shape A   -> indexA
    int indicesB[3];///< vertices on shape B   -> indexB
    //float div; // NOUVEAU c2
};

/// Output for b2Distance.
struct PE_DistanceOutput
{
    PE_Vec2 pointA;        ///< closest point on shapeA
    PE_Vec2 pointB;        ///< closest point on shapeB
    int iterCount;    ///< number of GJK iterCount used
};

float PE_GJK(
    PE_DistanceProxy *proxyA, PE_Vec2 transformA,
    PE_DistanceProxy *proxyB, PE_Vec2 transformB,
    int useRadius, PE_SimplexCache *cache, PE_DistanceOutput *output); // cache et out peuvent être NULL

inline float PE_GetDistance(
    PE_Shape *shapeA, PE_Vec2 transformA,
    PE_Shape *shapeB, PE_Vec2 transformB,
    PE_DistanceOutput *output)
{
    PE_DistanceProxy proxyA, proxyB;
    proxyA.Set(*shapeA);
    proxyB.Set(*shapeB);

    return PE_GJK(&proxyA, transformA, &proxyB, transformB, true, NULL, output);
}
