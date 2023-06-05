#pragma once

#include "Settings.h"
#include "InputGroup.h"

class MouseInput : public InputGroup
{
public:
    MouseInput();

    virtual void OnPreEventProcess() override;
    virtual void OnEventProcess(SDL_Event evt) override;
    virtual void Reset() override;

    // Position de la souris
    PE_Vec2 viewPos;
    PE_Vec2 lastViewPos;

    // Clic gauche
    int  leftClickCount;
    bool leftDown;
    bool leftReleased;

    // Clic droit
    int  rightClickCount;
    bool rightDown;
    bool rightReleased;

    // Roulette
    bool middleClick;
    int  wheel;
};
