#pragma once

#include "PE_Allocator.h"
#include "PE_List.h"
#include "PE_Vector.h"
#include "PE_CollisionContainer.h"
#include "PE_Collider.h"

#include <stdlib.h>

class PE_World;
struct PE_BroadPhase;
class PE_CollisionPair;
class PE_Collision;

class PE_CollisionManager
{
public:
    PE_CollisionManager(PE_World *world);
    ~PE_CollisionManager();

    /// @protected
    /// @brief Pointer to the world allocator.
    PE_Allocator *m_allocator;

    /// @protected
    /// @brief Pointer to the parent world.
    PE_World *m_world;

    /// @protected
    /// @brief The broadPhase of the collision detection algorithm.
    PE_BroadPhase *m_broadPhase;

    PE_CollisionContainer *m_container;


    void FindNewCollisions();
    void Collide();

    void CreateProxy(PE_AABB *aabb, PE_ColliderProxy *proxy);
    void RemoveProxy(PE_ColliderProxy *proxy);
    void SynchronizeProxy(PE_ColliderProxy *proxy, PE_Vec2 displacement);
};
