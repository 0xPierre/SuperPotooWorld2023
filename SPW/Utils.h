#pragma once

#include "Settings.h"

class DampedVec2 : public PE_Vec2
{
public:
    DampedVec2();

    void SetPosition(const PE_Vec2 &position);
    void SetPosition(float x, float y);
    void SetTarget(const PE_Vec2 &target);
    void SetTarget(float x, float y);
    void SetMaxSpeed(float maxSpeed);
    void SetSmoothTime(float smoothTime);

    void Update(const RE_Timer &m_time);

protected:
    PE_Vec2 m_target;
    float m_smoothTime;
    float m_maxSpeed;
    PE_Vec2 m_velocity;
};

inline void DampedVec2::SetPosition(const PE_Vec2 &position)
{
    SetPosition(position.x, position.y);
}

inline void DampedVec2::SetPosition(float xIn, float yIn)
{
    Set(xIn, yIn);
    m_target.Set(xIn, yIn);
    m_velocity.SetZero();
}

inline void DampedVec2::SetTarget(const PE_Vec2 & target)
{
    m_target = target;
}

inline void DampedVec2::SetTarget(float xIn, float yIn)
{
    m_target.Set(xIn, yIn);
}

inline void DampedVec2::SetMaxSpeed(float maxSpeed)
{
    m_maxSpeed = maxSpeed;
}

inline void DampedVec2::SetSmoothTime(float smoothTime)
{
    m_smoothTime = smoothTime;
}
