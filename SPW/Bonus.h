#pragma once

#include "Settings.h"
#include "GameBody.h"

class Bonus : public GameBody
{
public:
    Bonus(Scene &scene);

    virtual void Start() override;
    virtual void Render() override;
    virtual void OnRespawn() override;
    virtual void OnCollisionEnter(GameCollision &collision) override;

private:
    RE_Animator m_animator;
    bool m_hit;
};