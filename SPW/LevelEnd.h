#pragma once

#include "Settings.h"
#include "GameBody.h"

class LevelEnd : public GameBody
{
public:
    LevelEnd(Scene &scene);

    virtual void Render() override;
    virtual void Start() override;
    virtual void OnCollisionEnter(GameCollision &collision) override;

private:
    RE_Animator m_animator;
};