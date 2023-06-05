#pragma once

#include "Settings.h"
#include "Collectable.h"

class Heart : public Collectable
{
public:
    Heart(Scene &scene);

    virtual void Start() override;
    virtual void Render() override;
    virtual void OnRespawn() override;
    virtual void Update() override;

    virtual void OnCollisionEnter(GameCollision &collision) override;
    virtual void Collect(GameBody *collector) override;
};