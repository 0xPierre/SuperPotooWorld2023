#pragma once

#include "Settings.h"
#include "GameBody.h"

class Enemy : public GameBody
{
public:
    Enemy(Scene &scene);
    virtual ~Enemy();

    virtual void Damage(GameBody *damager);
};
