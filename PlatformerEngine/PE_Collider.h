#pragma once

#include "PE_Settings.h"
#include "PE_Shape.h"
#include "PE_Math.h"
#include "PE_List.h"
#include "PE_Allocator.h"
#include "PE_ColliderProxy.h"
#include "PE_Solver.h"
#include "PE_UserData.h"

class PE_World;
class PE_Body;
class PE_Collision;
class PE_CollisionManager;
class PE_Collider;

/// @brief Classe permettant de filtrer les collisions.
class PE_Filter
{
public:
    PE_Filter();

    /// @brief Les bits représentant les catégories d'un collider.
    /// Normalement, on ne définit qu'un seul bit à un.
    int categoryBits;

    /// @brief Les bits servant de masque pour les collisions.
    /// Ce membre définit les catégories avec lesquelles le collider peut entrer en collision.
    int maskBits;

    bool ShouldCollide(const PE_Filter &other) const;
};

inline bool PE_Filter::ShouldCollide(const PE_Filter &other) const
{
    return (categoryBits & other.maskBits) && (other.categoryBits & maskBits);
}

/// @brief Classe regroupant les informations nécessaires à la création d'un collider.
/// Une même définition peut servir à créer plusieurs colliders.
class PE_ColliderDef
{
public:
    PE_ColliderDef();

    void SetDefault();

    /// @brief Forme du collider.
    PE_Shape *shape;

    /// @brief Indique si le collider est un déclencheur.
    bool isTrigger;
    
    /// @brief Indique si le collider a un comportement "one-way".
    /// Autrement dit, s'il autorise uniquement les collisions dans une seule direction.
    bool isOneWay;

    float friction;

    /// @brief Filtre à appliquer au collider pendant la détection des collisions.
    PE_Filter filter;
    
    PE_Vec2 up;

    PE_ColliderUserData userData;
};

/// @brief Classe représentant un collider dans le moteur physique.
/// Un collider peut être affecté à un corps pour la détection de collisions.
/// Si le collider est un détecteur (trigger), il n'applique aucune force aux autres colliders/corps.
class PE_Collider
{
public:
    void SetTrigger(bool isTrigger);
    bool IsTrigger() const;
    bool IsOneWay() const;

    PE_Body *GetBody();
    const PE_Shape &GetShape() const;
    PE_Vec2 GetUp() const;
    PE_AABB GetAABB() const;

    bool CheckCategory(int maskBits) const;

    PE_ColliderUserData &GetUserData();

private:
    friend class PE_Body;
    friend class PE_CollisionContainer;
    friend class PE_CollisionManager;
    friend class PE_CollisionPair;
    friend class PE_Solver;

    /// @brief Proxy utilisé pour connecter le collider avec le gestionnaire de collisions.
    PE_ColliderProxy *m_proxy;

    enum Flag : int
    {
        TRIGGER = 0x01,
        ONE_WAY = 0x02,
    };

    /// @protected
    /// @brief Flags du collider.
    int m_flags;

    /// @protected
    /// @brief Pointeur vers le corps parent.
    PE_Body *m_body;

    /// @protected
    /// @brief Forme du collider.
    PE_Shape *m_shape;

    float m_friction;

    PE_Vec2 m_up;

    /// @protected
    /// @brief Filtre à appliquer au collider pendant la détection des collisions.
    PE_Filter m_filter;

    /// @protected
    /// @brief Boîte englobante du collider pour le balayage courant.
    /// Cette AABB est mise à jour par le corps parent et est utilisée dans la broad phase.
    PE_AABB m_aabb;

    PE_ColliderUserData m_userData;

    PE_Collider(PE_ColliderDef &def, PE_Body &body);
    ~PE_Collider();

    bool ShouldCollide(const PE_Collider *other);
    void CreateProxy(PE_CollisionManager *manager, const PE_Vec2 &transform);
    void RemoveProxy(PE_CollisionManager *manager);
    PE_ColliderProxy *GetProxy();

    void AddFlags(int flags);
    void SubFlags(int flags);

    void Synchronize(
        PE_CollisionManager *manager,
        const PE_Vec2 &transform0, const PE_Vec2 &transform1);
};

inline PE_ColliderProxy *PE_Collider::GetProxy()
{
    return m_proxy;
}

inline void PE_Collider::AddFlags(int flags)
{
    m_flags |= flags;
}

inline void PE_Collider::SubFlags(int flags)
{
    m_flags &= ~flags;
}

inline void PE_Collider::SetTrigger(bool isTrigger)
{
    if (isTrigger)
        AddFlags(PE_Collider::Flag::TRIGGER);
    else
        SubFlags(PE_Collider::Flag::TRIGGER);
}

inline bool PE_Collider::IsTrigger() const
{
    int flag = PE_Collider::Flag::TRIGGER;
    return (m_flags & flag) == flag;
}

inline bool PE_Collider::IsOneWay() const
{
    int flag = PE_Collider::Flag::ONE_WAY;
    return (m_flags & flag) == flag;
}

inline PE_Body *PE_Collider::GetBody()
{
    return m_body;
}

inline const PE_Shape &PE_Collider::GetShape() const
{
    return *m_shape;
}

inline PE_Vec2 PE_Collider::GetUp() const
{
    return m_up;
}

inline PE_AABB PE_Collider::GetAABB() const
{
    return m_aabb;
}

inline bool PE_Collider::ShouldCollide(const PE_Collider *other)
{
    return m_filter.ShouldCollide(other->m_filter);
}

inline bool PE_Collider::CheckCategory(int maskBits) const
{
    return (m_filter.categoryBits & maskBits) != 0;
}
