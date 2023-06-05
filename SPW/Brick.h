#pragma once

#include "Settings.h"
#include "GameBody.h"

class Brick : public GameBody
{
public:
    Brick(Scene &scene);
    virtual ~Brick();
    
    virtual void Start() override;
    virtual void Render() override;
    virtual void OnCollisionEnter(GameCollision &collision) override;
    virtual void OnRespawn() override;

private:
    RE_Animator m_animator;
};