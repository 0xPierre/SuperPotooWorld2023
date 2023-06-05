#pragma once

#include "PE_Collider.h"
#include "PE_List.h"
#include "PE_Math.h"
#include "PE_Shape.h"
#include "PE_ShapeCollision.h"
#include "PE_ShapeDistance.h"
#include "PE_Solver.h"
#include "PE_World.h"
#include "PE_Callback.h"

class PE_CollisionPair;

/// @ingroup PE_Collision
/// @brief Structure contenant les informations sur une collision entre deux colliders.
class PE_Collision
{
public:
    /// @protected
    /// @brief Noeud contenant un pointeur vers cette collision dans la listes des collisions du collider parent.
    /// Ce membre appartient au collider parent.

    /// @protected
    /// @brief Pointeur vers le collider principal impliqué dans la collision.
    PE_Collider *m_collider;

    /// @protected
    /// @brief Pointeur vers le second collider impliqué dans la collision.
    PE_Collider *m_otherCollider;

    /// @protected
    /// @brief Pointeur vers la paire de collisions.
    /// Il s'agit de la structure contenant cette collision dans le détecteur de collisions.
    PE_CollisionPair *m_cPair;
};

class PE_CollisionPair
{
public:
    PE_Collider *GetColliderA();
    PE_Collider *GetColliderB();
    PE_Manifold GetManifold() const;

    bool IsEnabled() const;
    void SetEnabled(bool enabled);

    void ResolveUpA();
    void ResolveUpB();
    void SetFriction(bool hasFriction);

private:
    friend class PE_CollisionContainer;
    friend class PE_CollisionManager;
    friend class PE_Solver;

    PE_CollisionPair(PE_ColliderProxy *proxyA, PE_ColliderProxy *proxyB);
    ~PE_CollisionPair();

    void Update(PE_CollisionListener *listener);
    void ComputeTOI();
    float MixFrictions(float frictionA, float frictionB) const;

    PE_ListNode m_proxyNodeA;

    /// @brief Collision for the first collider.
    PE_Collision m_collisionA;

    PE_ListNode m_proxyNodeB;

    /// @brief Collision for the second collider.
    PE_Collision m_collisionB;

    PE_SimplexCache m_cache;

    /// @protected
    /// @brief The time of impact between the two colliders.
    float m_timeOfImpact;

    float m_friction;

    /// @protected
    /// @brief Le manifold de la collision.
    PE_Manifold m_manifold;

    /// @protected
    /// @brief Pointer to the first collider in collision.
    PE_Collider *m_colliderA;

    /// @protected
    /// @brief Pointer to the second collider in collision.
    PE_Collider *m_colliderB;

    enum Flag : int
    {
        ///@brief Flag indiquant si les deux corps d'une collision sont en contact.
        /// Il sert uniquement au solver.
        TOUCHING    = 0x01,

        ///@brief Flag indiquant si la collision représente un contact persistant.
        /// Il sert uniquement pour la gestion des callbacks.
        STAY        = 0x02,

        ///@brief Flag indiquant si le moment de l'impact de la collision est à jour.
        TOI         = 0x04,

        ///@brief Flag indiquant que la collision est activée.
        /// Une collision peut être désactivée automatiquement lors d'une collision one-way
        /// ou manuellement par l'utilisateur dans un callback.
        ENABLED     = 0x08,
    };
    int m_flags;

    void AddFlags(int flags);
    void SubFlags(int flags);
    bool TestFlag(Flag flag) const;

    /// @protected
    /// @brief A pointer to the solver data.
    PE_CPairSolverData m_solverData;
};

inline PE_Collider *PE_CollisionPair::GetColliderA()
{
    return m_colliderA;
}

inline PE_Collider *PE_CollisionPair::GetColliderB()
{
    return m_colliderB;
}

inline PE_Manifold PE_CollisionPair::GetManifold() const
{
    return m_manifold;
}

inline bool PE_CollisionPair::IsEnabled() const
{
    return TestFlag(PE_CollisionPair::Flag::ENABLED);
}

inline void PE_CollisionPair::SetEnabled(bool enabled)
{
    if (enabled)
    {
        AddFlags(PE_CollisionPair::Flag::ENABLED);
    }
    else
    {
        SubFlags(PE_CollisionPair::Flag::ENABLED);
    }
}

inline void PE_CollisionPair::ResolveUpA()
{
    m_solverData.SetCollisionAUp();
}

inline void PE_CollisionPair::ResolveUpB()
{
    m_solverData.SetCollisionBUp();
}

inline void PE_CollisionPair::SetFriction(bool hasFriction)
{
    m_solverData.SetFriction(hasFriction);
}

inline void PE_CollisionPair::AddFlags(int flags)
{
    m_flags |= flags;
}

inline void PE_CollisionPair::SubFlags(int flags)
{
    m_flags &= ~flags;
}

inline bool PE_CollisionPair::TestFlag(Flag flag) const
{
    return (m_flags & flag) == flag;
}
