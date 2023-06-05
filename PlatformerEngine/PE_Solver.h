#pragma once

#include "PE_Settings.h"
#include "PE_Allocator.h"
#include "PE_List.h"
#include "PE_Vector.h"
#include "PE_Math.h"

class PE_World;
class PE_Body;
class PE_CollisionPair;
class PE_CollisionManager;
class PE_Solver;

///////////////////////////////////////////////////////////////////////////////////////////////////
//  PE_SolverBody

struct PE_BodySolverData
{
public:
    PE_BodySolverData() { Reset(); }
    void Reset();

private:
    friend class PE_Solver;

    int m_iterCount;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//  PE_CPairSolverData

/// @brief A SolverCollisionPair contains the solver data associated with a collision pair.
/// Each collision pair contains this object.
class PE_CPairSolverData
{
public:
    PE_CPairSolverData() { Reset(); }

    void Reset();
    void SetCollisionAUp();
    void SetCollisionBUp();
    void SetFriction(bool hasFriction);

private:
    friend class PE_Solver;

    enum Flag : int
    {
        COLLISION_A_UP = 1 << 0,
        COLLISION_B_UP = 1 << 1,
        FRICTION       = 1 << 2,
        FORCES_APPLIED = 1 << 3,
    };
    int m_flags;

    void AddFlags(int flags);
    void SubFlags(int flags);
    bool TestFlag(int flag) const;

    /// @brief Nombre d'itérations du solver sur cette pair pendant la mise à jour actuelle.
    int m_iterCount;
};

inline void PE_CPairSolverData::Reset()
{
    m_iterCount = 0;
    m_flags = FRICTION;
}

inline void PE_CPairSolverData::SetCollisionAUp()
{
    AddFlags(COLLISION_A_UP);
}

inline void PE_CPairSolverData::SetCollisionBUp()
{
    AddFlags(COLLISION_B_UP);
}

inline void PE_CPairSolverData::SetFriction(bool hasFriction)
{
    if (hasFriction)
        AddFlags(FRICTION);
    else
        SubFlags(FRICTION);
}

inline void PE_CPairSolverData::AddFlags(int flags)
{
    m_flags |= flags;
}

inline void PE_CPairSolverData::SubFlags(int flags)
{
    m_flags &= ~flags;
}

inline bool PE_CPairSolverData::TestFlag(int flag) const
{
    return (m_flags & flag) == flag;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//  PE_Solver

/// @brief The solver is used to solved the collisions computed by the collision manager.
class PE_Solver
{
public:
    PE_Solver(PE_World *world);
    ~PE_Solver();

    void Solve();
    void SolveTOI();
    void Reset();

private:
    void SolveCollisionPair(PE_CollisionPair *cPair);
    void SolveBodySweep(PE_Body *body, PE_Vec2 normal, float depth, bool resolveUp);
    void ApplyFriction(
        PE_Body *body, PE_Vec2 velocity, PE_Vec2 normal,
        PE_Vec2 gravity, PE_Vec2 up, float friction, float timeStep
    );

    /// @brief Pointer to the world allocator.
    PE_Allocator *m_allocator;

    /// @brief Pour la liste des PE_Body
    PE_World *m_world;

    /// @brief Pour la liste de collisions;
    PE_CollisionManager *m_manager;
};
