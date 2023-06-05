#pragma once

#include "PE_List.h"
#include "PE_Allocator.h"
#include "PE_Callback.h"

class PE_World;
class PE_CollisionPair;
class PE_Collision;
class PE_Collider;

class PE_CollisionContainer
{
public:
    PE_CollisionContainer(PE_Allocator *allocator);
    ~PE_CollisionContainer();

    PE_CollisionPair *CreateCPair(PE_Collider *colliderA, PE_Collider *colliderB);
    void RemoveCPair(PE_CollisionPair *cPair, PE_CollisionListener *listener);

    // PRIVATE

    /// @protected
    /// @brief Pointer to the world allocator.
    PE_Allocator *m_allocator;

    /// @protected
    /// @brief Pointer to the parent world.
    PE_World *m_world;

    /// @protected
    /// @brief The list of every current collision pairs.
    PE_List m_list;

private:
    friend class PE_Solver;
    friend class PE_CollisionManager;

    PE_CollisionPair *findCPair(PE_Collider *colliderA, PE_Collider *colliderB);

    void GetCollisionPairIterator(PE_ListIterator &it);
};

inline void PE_CollisionContainer::GetCollisionPairIterator(PE_ListIterator &it)
{
    PE_List_GetIterator(&m_list, &it);
}
