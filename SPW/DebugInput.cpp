#include "DebugInput.h"

DebugInput::DebugInput() :
    InputGroup(), nextStepDown(false), nextStepPressed(false), quitStepPressed(false),
    infoPressed(false), gizmosPressed(false), bodyPressed(false),
    trackPressed(false), camPressed(false), idPressed(false)
{
}

void DebugInput::OnPreEventProcess()
{
    bodyPressed = false;
    camPressed = false;
    gizmosPressed = false;
    infoPressed = false;
    quitStepPressed = false;
    nextStepPressed = false;
    trackPressed = false;
    idPressed = false;
}

void DebugInput::OnEventProcess(SDL_Event evt)
{
    SDL_Scancode scanCode;
    switch (evt.type)
    {
    case SDL_KEYDOWN:
        scanCode = evt.key.keysym.scancode;

        if (evt.key.repeat)
            break;

        switch (scanCode)
        {
        case SDL_SCANCODE_Q:
            quitStepPressed = true;
            break;

        case SDL_SCANCODE_RCTRL:
            infoPressed = true;
            break;

        case SDL_SCANCODE_TAB:
            // Nouveau pas
            nextStepPressed = true;
            nextStepDown = true;
            break;

        case SDL_SCANCODE_G:
            // Gizmos
            gizmosPressed = true;
            break;

        case SDL_SCANCODE_P:
            // Body
            bodyPressed = true;
            break;

        case SDL_SCANCODE_C:
            // Caméra
            camPressed = true;
            break;

        case SDL_SCANCODE_T:
            // Camera traking
            trackPressed = true;
            break;

        case SDL_SCANCODE_I:
            // Affichage des ID
            idPressed = true;
            break;

        default:
            break;
        }
        break;

    case SDL_KEYUP:
        scanCode = evt.key.keysym.scancode;

        switch (scanCode)
        {
        case SDL_SCANCODE_TAB:
            // Nouveau pas
            nextStepDown = false;
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }
}

void DebugInput::Reset()
{
    bodyPressed = false;
    camPressed = false;
    gizmosPressed = false;
    infoPressed = false;
    quitStepPressed = false;
    nextStepDown = false;
    nextStepPressed = false;
    quitStepPressed = false;
    trackPressed = false;
    idPressed = false;
}
