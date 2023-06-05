#pragma once

#include "PE_Settings.h"

#include "PE_Allocator.h"
#include "PE_BroadPhase.h"
#include "PE_CollisionManager.h"
#include "PE_List.h"
#include "PE_Math.h"
#include "PE_Callback.h"

class PE_Body;
class PE_BodyDef;
class PE_Solver;

/// @ingroup PE_World
/// @class PE_World_s
/// @brief Le monde gère et crée tous les corps physiques.
class PE_World
{
public:
    PE_World(PE_Vec2 gravity, float timeStep);
    PE_World(PE_World const &) = delete;
    PE_World &operator=(PE_World const &) = delete;
    ~PE_World();

    PE_Body *CreateBody(PE_BodyDef &def);
    void RemoveBody(PE_Body *body);

    int GetBodyCount();

    void SetGravity(const PE_Vec2 &gravity);
    PE_Vec2 GetGravity() const;

    float GetTimeStep() const;
    PE_Vec2 GetUp() const;

    void FixedUpdate();

    bool IsLocked() const;

    void SetCollisionListener(PE_CollisionListener *listener);

    // Query
    void QueryAABB(PE_QueryCallback &callback, const PE_AABB &aabb);
    void RayCast(PE_RayCastCallback &callback, PE_Vec2 origin, PE_Vec2 direction, float distance);

private:
    friend class PE_Body;
    friend class PE_Collider;
    friend class PE_CollisionManager;
    friend class PE_Solver;

    void GetBodyIterator(PE_ListIterator &it);

    /// @brief Allocateur principal utilisé pour toutes les allocations mémoire dans le monde.
    PE_Allocator m_allocator;

    /// @brief Solver utilisé pour résoudre toutes les collisions dans ce monde.
    PE_Solver *m_solver;

    /// @brief Détecteur de collisions.
    PE_CollisionManager *m_manager;

    /// @brief Liste des corps du monde.
    PE_List m_bodyList;

    /// @brief booléen permettant de bloquer l'accès à certaines fonctions lors de callback.
    bool m_locked;

    /// @brief Pas de temps utilisé pour chaque mise à jour du monde.
    /// Le pas de temps est fixe et varie typiquement entre 1.0f/50.0f et 1.0f/60.0f.
    float m_timeStep;

    /// @brief Vecteur de gravité global du monde.
    /// Note that the gravity of a body can be scaled.
    PE_Vec2 m_gravity;

    PE_Vec2 m_up;

    PE_CollisionListener *m_collisionListener;
};


inline int PE_World::GetBodyCount()
{
    return PE_List_GetNodeCount(&m_bodyList);
}

inline void PE_World::SetGravity(const PE_Vec2 &gravity)
{
    m_gravity = gravity;
    float norm = gravity.Length();
    if (norm > 0.0f)
    {
        m_up = (-1.0f / norm) * gravity;
    }
    else
    {
        m_up = PE_Vec2::up;
    }
}

inline PE_Vec2 PE_World::GetGravity() const
{
    return m_gravity;
}

inline float PE_World::GetTimeStep() const
{
    return m_timeStep;
}

inline PE_Vec2 PE_World::GetUp() const
{
    return m_up;
}

inline bool PE_World::IsLocked() const
{
    return m_locked;
}

inline void PE_World::SetCollisionListener(PE_CollisionListener *listener)
{
    m_collisionListener = listener;
}

inline void PE_World::GetBodyIterator(PE_ListIterator &it)
{
    PE_List_GetIterator(&m_bodyList, &it);
}
