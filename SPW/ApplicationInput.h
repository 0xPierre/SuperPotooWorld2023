#pragma once

#include "Settings.h"
#include "InputGroup.h"

class ApplicationInput : public InputGroup
{
public:
    ApplicationInput();

    virtual void OnPreEventProcess() override;
    virtual void OnEventProcess(SDL_Event evt) override;
    virtual void Reset() override;

    bool quitPressed;
    bool pausePressed;
    bool printPressed;
};
