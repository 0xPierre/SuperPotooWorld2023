#pragma once

#include "Settings.h"
#include "GameBody.h"

class Checkpoint : public GameBody
{
public:
    Checkpoint(Scene &scene);

    virtual void Start() override;
    virtual void Render() override;
    virtual void OnCollisionEnter(GameCollision &collision) override;

private:
    RE_Animator m_animator;
    bool m_isActive;
};