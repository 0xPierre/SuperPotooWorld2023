#pragma once

#include "PE_Settings.h"

#include "PE_Allocator.h"
#include "PE_List.h"

class PE_Collider;
class PE_Collision;

class PE_ColliderProxy
{
public:
    /// @protected
    /// @brief Pointeur vers le collider associé au proxy.
    PE_Collider* m_collider;

    /// @protected
    /// @brief Indice du proxy dans la broad phase.
    int m_proxyId;

    /// @protected
    /// @brief Liste des collisions impliquant ce collider.
    PE_List m_collisionList;
};

PE_ColliderProxy *PE_ColliderProxy_New(PE_Collider *collider, PE_Allocator &allocator);
void PE_ColliderProxy_Free(PE_ColliderProxy *proxy, PE_Allocator &allocator);

/// @brief Indique si un proxy est reliée au gestionnaire de collisions.
/// @param proxy 
/// @return 
inline bool PE_ColliderProxy_IsActive(PE_ColliderProxy *proxy)
{
    return proxy->m_proxyId != PE_INVALID_ID;
}

inline void PE_ColliderProxy_Reset(PE_ColliderProxy *proxy)
{
    assert(PE_List_IsEmpty(&proxy->m_collisionList));
    assert(proxy->m_collider != NULL);

    PE_List_Init(&proxy->m_collisionList);
    proxy->m_proxyId = PE_INVALID_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// PE_CollisionIterator

typedef PE_ListIterator PE_CollisionIterator;

inline void PE_ColliderProxy_GetCollisionIterator(
    PE_ColliderProxy *proxy, PE_CollisionIterator *it)
{
    PE_List_GetIterator(&proxy->m_collisionList, it);
}

inline bool PE_CollisionIterator_MoveNext(PE_CollisionIterator *it)
{
    return PE_ListIterator_MoveNext(it);
}

inline PE_Collision *PE_CollisionIterator_Current(PE_CollisionIterator *it)
{
    return (PE_Collision *)PE_ListIterator_Current(it);
}
