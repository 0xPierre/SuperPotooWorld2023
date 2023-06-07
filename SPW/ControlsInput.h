#pragma once

#include "Settings.h"
#include "InputGroup.h"

class ControlsInput : public InputGroup
{
public:
    ControlsInput();

    virtual void OnPreEventProcess() override;
    virtual void OnEventProcess(SDL_Event evt) override;
    virtual void Reset() override;

    float hAxis;
    bool jumpDown;
    bool jumpPressed;
    bool longJump;
    long double jumpTimeMemory;
    bool goDownDown;
    int terrainSelected;
    int groundSelected;
    double milliseconds;
};
