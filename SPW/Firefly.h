#pragma once

#include "Settings.h"
#include "Collectable.h"

class Firefly : public Collectable
{
public:
    Firefly(Scene& scene);
    virtual ~Firefly();

    virtual void Start() override;
    virtual void Render() override;
    virtual void OnRespawn() override;
    virtual void Collect(GameBody* collector) override;

private:
    RE_Animator m_animator;

    enum class State
    {
        IDLE, SPINNING, DYING
    };
    State m_state;
};