#pragma once

#include "Settings.h"
#include "Enemy.h"

class Nut : public Enemy
{
public:
    Nut(Scene &scene);
    virtual ~Nut();

    virtual void Start() override;
    virtual void FixedUpdate() override;
    virtual void Render() override;
    virtual void OnRespawn() override;

    virtual void Damage(GameBody *damager) override;

    virtual void OnCollisionStay(GameCollision &collision) override;

private:
    RE_Animator m_animator;

    enum class State
    {
        IDLE, SPINNING, DYING
    };
    State m_state;
};