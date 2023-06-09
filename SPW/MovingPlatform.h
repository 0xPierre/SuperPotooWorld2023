#pragma once

#include "Settings.h"
#include "GameBody.h"

class MovingPlatform : public GameBody
{
public:
    MovingPlatform(Scene& scene);

    virtual void Start() override;
    virtual void Render() override;
    virtual void OnRespawn() override;
    virtual void OnCollisionEnter(GameCollision& collision) override;
    void FixedUpdate() override;

private:
    RE_Animator m_animator;
    bool m_hit;
    enum class State
    {
        MOVING_LEFT, MOVING_RIGHT, MOVING_UP, MOVING_DOWN,
    };
    State m_state;
};