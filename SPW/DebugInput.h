#pragma once

#include "Settings.h"
#include "InputGroup.h"

class DebugInput : public InputGroup
{
public:
    DebugInput();

    virtual void OnPreEventProcess() override;
    virtual void OnEventProcess(SDL_Event evt) override;
    virtual void Reset() override;

    // Mode pas-à-pas
    bool nextStepDown;
    bool nextStepPressed;
    bool quitStepPressed;

    // Informations
    bool infoPressed;
    bool gizmosPressed;
    bool bodyPressed;
    bool idPressed;

    // Changement de caméra
    bool trackPressed;
    bool camPressed;
};
