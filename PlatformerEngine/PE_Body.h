#pragma once

#include "PE_Settings.h"
#include "PE_UserData.h"

#include "PE_Collider.h"
#include "PE_CollisionPair.h"
#include "PE_List.h"
#include "PE_Math.h"
#include "PE_Solver.h"
#include "PE_UserData.h"

class PE_World;
class PE_Body;
struct PE_SolverBody;

/// @brief Enumération donnant les types possibles pour un corps.
enum class PE_BodyType : int
{
    /// @brief Un corps statique est conçu pour ne jamais se déplacer sous simulation,
    /// même en cas de collision. Un corps statique ne peut entrer en collision qu'avec un
    /// corps dynamique.
    STATIC = 0,

    /// @brief Un corps cinématique est conçu pour se déplacer sous simulation,
    /// mais seulement sous le contrôle explicite de l'utilisateur.
    /// Il n'est pas soumis à la gravité et ne peut entrer en collision qu'avec un
    /// corps dynamique.
    KINEMATIC,

    /// @brief Un corps dynamique est conçu pour se déplacer sous simulation.
    /// Il est soumis à la gravité et peut potentiellement enter en collision avec n'importe
    /// quel autre corps.
    DYNAMIC
};

/// @brief Classe regroupant les informations nécessaires à la création d'un corps.
/// Une même définition peut servir à créer plusieurs corps.
class PE_BodyDef
{
public:
    PE_BodyDef();

    /// @brief Le type du corps.
    PE_BodyType type;

    /// @brief La position du corps exprimée dans le référentiel monde.
    PE_Vec2 position;

    /// @brief La vitesse linéaire de l'origine du corps exprimée dans le référentiel monde.
    PE_Vec2 velocity;

    /// @brief L'amortissement linéaire du corps.
    /// Ce paramètre permet de réduire la vitesse linéaire horizontale et/ou verticale du corps.
    PE_Vec2 damping;

    /// @brief L'échelle de gravité du corps.
    float gravityScale;

    /// @brief La masse du corps.
    float mass;

    /// @brief Le nom du corps.
    const char *name;

    /// @brief Les données de l'utilisateur associées avec le corps.
    PE_BodyUserData userData;
};

// Différence entre SIMULATED et AWAKE
// not SIMULATED : aucune participation au moteur physique (les colliders ne sont pas dans la broadphase)
// not AWAKE : Pas de mise à jour de la position (plus d'intégration ni de modification par le solver)
//             Les collisions sont possibles.

/// @brief Classe représentant un corps indéformable dans le moteur physique.
class PE_Body
{
public:
    PE_Vec2 GetPosition() const;
    PE_Vec2 GetInterpolation(float alpha) const;
    PE_BodyType GetType() const;

    PE_Collider *CreateCollider(PE_ColliderDef &def);
    void RemoveCollider(PE_Collider *collider);

    void SetPosition(const PE_Vec2 &position);
    void SetVelocity(const PE_Vec2 &velocity);

    PE_Vec2 GetVelocity() const;
    PE_Vec2 GetLocalVelocity() const;
    PE_Vec2 GetExternVelocity() const;

    void ApplyImpulse(const PE_Vec2 &impulse);
    PE_Vec2 GetNetForce() const;
    PE_Vec2 GetNetSolverForce() const;

    bool IsEnabled() const;
    void SetEnabled(bool enabled);

    bool IsAwake() const;
    void SetAwake(bool awake);

    void SetGravityScale(float gravityScale);
    float GetGravityScale() const;

    PE_World *GetWorld();

    void SetDamping(const PE_Vec2 &damping);
    PE_Vec2 GetDamping() const;

    void ClearForces();

    PE_Body *GetParent() const;
    void SetParent(PE_Body *parent);

    PE_BodyUserData &GetUserData();
    void ApplyForce(PE_Vec2 force);

private:
    friend class PE_World;
    friend class PE_Collider;
    friend class PE_Solver;
    friend class PE_CollisionPair;
    friend class PE_CollisionManager;

    void GetColliderIterator(PE_ListIterator &it);

    void SynchronizeTransform();
    void SynchronizeColliders();
    void FinalizeStep();
    void Integrate(float timeStep);
    void IntegratePosition(float timeStep);
    void Advance(float alpha);
    const PE_Sweep &GetSweep() const;
    bool ShouldCollide(const PE_Body *other) const;
    PE_BodySolverData &GetSolverData();

    void ApplySolverForce(PE_Vec2 force);
    void ApplySolverImpulse(PE_Vec2 impulse);

    PE_Body(const PE_BodyDef &bodyDef, PE_World &world);
    ~PE_Body();

    void AddChild(PE_Body *child);
    void RemoveChild(PE_Body *child);

    enum Flag : int
    {
        AWAKE        = 0x01,
        ENABLED      = 0x02,
        INTEGRATED   = 0x04,
    };

    /// @brief Flags du corps.
    int m_flags;

    void AddFlags(int flags);
    void SubFlags(int flags);
    bool TestFlag(Flag flag) const;

    /// @brief Liste des colliders attachés à ce corps.
    PE_List m_colliderList;

    const char *m_name;

    /// @brief Type du corps.
    PE_BodyType m_type;

    /// @brief Position du corps exprimée dans le référentiel monde.
    PE_Vec2 m_position;

    PE_Vec2 m_lastPos;
    
    /// @brief Vitesse linéaire de l'origine du corps exprimée dans le référentiel monde.
    PE_Vec2 m_localVelocity;

    PE_Vec2 m_externVelocity;

    /// @brief Somme des forces appliquées au corps, exprimée dans le référentiel monde.
    PE_Vec2 m_forces;

    PE_Vec2 m_solverForces;

    PE_Vec2 m_solverImpulse;

    PE_Vec2 m_damping;

    /// @brief Echelle de gravité du corps.
    float m_gravityScale;

    float m_mass;
    float m_invMass;

    /// @brief Pointeur vers le monde parent de ce corps.
    PE_World *m_world;

    /// @brief Décrit le mouvement du corps pendant un pas d'intégration.
    PE_Sweep m_sweep;


    PE_Body *m_parent;
    PE_Body **m_children;
    int m_childCount;
    int m_childCapacity;

    /// @brief Données du solver associées à ce corps.
    PE_BodySolverData m_solverData;

    PE_BodyUserData m_userData;
};

inline PE_Vec2 PE_Body::GetPosition() const
{
    return m_position;
}

inline PE_BodyType PE_Body::GetType() const
{
    return m_type;
}

inline PE_BodyUserData &PE_Body::GetUserData()
{
    return m_userData;
}

inline void PE_Body::ApplyForce(PE_Vec2 force)
{
    if (m_type != PE_BodyType::DYNAMIC)
    {
        return;
    }
    SetAwake(true);
    m_forces += force;
}

inline void PE_Body::GetColliderIterator(PE_ListIterator &it)
{
    PE_List_GetIterator(&m_colliderList, &it);
}

inline void PE_Body::SynchronizeTransform()
{
    m_position = m_sweep.position1;
}

inline PE_Vec2 PE_Body::GetLocalVelocity() const
{
    return m_localVelocity;
}

inline PE_Vec2 PE_Body::GetExternVelocity() const
{
    return m_externVelocity;
}

inline void PE_Body::ApplyImpulse(const PE_Vec2 &impulse)
{
    if (m_type != PE_BodyType::DYNAMIC)
    {
        return;
    }
    SetAwake(true);
    m_localVelocity += m_invMass * impulse;
}

inline PE_Vec2 PE_Body::GetNetForce() const
{
    return m_forces;
}

inline PE_Vec2 PE_Body::GetNetSolverForce() const
{
    return m_solverForces;
}

inline bool PE_Body::IsEnabled() const
{
    return TestFlag(PE_Body::Flag::ENABLED);
}

inline bool PE_Body::IsAwake() const
{
    return TestFlag(PE_Body::Flag::AWAKE);
}

inline void PE_Body::SetGravityScale(float gravityScale)
{
    m_gravityScale = gravityScale;
}

inline float PE_Body::GetGravityScale() const
{
    return m_gravityScale;
}

inline PE_World *PE_Body::GetWorld()
{
    return m_world;
}

inline void PE_Body::SetDamping(const PE_Vec2 &damping)
{
    m_damping = damping;
}

inline PE_Vec2 PE_Body::GetDamping() const
{
    return m_damping;
}

inline void PE_Body::ClearForces()
{
    m_forces.SetZero();
}

inline PE_Body *PE_Body::GetParent() const
{
    return m_parent;
}

inline void PE_Body::AddFlags(int flags)
{
    m_flags |= flags;
}

inline void PE_Body::SubFlags(int flags)
{
    m_flags &= ~flags;
}

inline bool PE_Body::TestFlag(Flag flag) const
{
    return (m_flags & flag) == flag;
}

inline PE_Vec2 PE_Body::GetInterpolation(float alpha) const
{
    return ((1.0f - alpha) * m_lastPos) + (alpha * m_position);
}

inline void PE_Body::Advance(float alpha)
{
    m_sweep.Advance(alpha);
    SynchronizeTransform();
}

inline const PE_Sweep &PE_Body::GetSweep() const
{
    return m_sweep;
}

inline bool PE_Body::ShouldCollide(const PE_Body *other) const
{
    return (IsAwake() || other->IsAwake())
        && (m_type == PE_BodyType::DYNAMIC || other->m_type == PE_BodyType::DYNAMIC);
}

inline PE_BodySolverData &PE_Body::GetSolverData()
{
    return m_solverData;
}

inline void PE_Body::ApplySolverForce(PE_Vec2 force)
{
    if (m_type != PE_BodyType::DYNAMIC)
    {
        return;
    }
    m_solverForces += force;
}

inline void PE_Body::ApplySolverImpulse(PE_Vec2 impulse)
{
    if (m_type != PE_BodyType::DYNAMIC)
    {
        return;
    }
    m_localVelocity += impulse;
    m_solverImpulse += impulse;
}
