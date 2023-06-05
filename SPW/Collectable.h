#pragma once

#include "Settings.h"
#include "GameBody.h"

class Collectable : public GameBody
{
public:
    Collectable(Scene &scene, Layer layer);
    virtual ~Collectable();

    virtual void Collect(GameBody *collector);
};