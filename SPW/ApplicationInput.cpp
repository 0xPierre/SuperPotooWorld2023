#include "ApplicationInput.h"

ApplicationInput::ApplicationInput() :
    InputGroup(), pausePressed(false), quitPressed(false), printPressed(false)
{
}

void ApplicationInput::OnPreEventProcess()
{
    quitPressed = false;
    pausePressed = false;
    printPressed = false;
}

void ApplicationInput::OnEventProcess(SDL_Event evt)
{
    SDL_Scancode scanCode;
    switch (evt.type)
    {
    case SDL_QUIT:
        quitPressed = true;
        break;

    case SDL_KEYDOWN:
        scanCode = evt.key.keysym.scancode;

        if (evt.key.repeat)
            break;

        switch (scanCode)
        {
        case SDL_SCANCODE_ESCAPE:
            // Touche pour quiter le jeu
            quitPressed = true;
            break;

        case SDL_SCANCODE_RETURN:
            pausePressed = true;
            break;

        case SDL_SCANCODE_BACKSPACE:
            printPressed = true;
            break;

        default:
            break;
        }
        break;


    case SDL_JOYBUTTONDOWN:
        switch (evt.jbutton.button)
        {
        case 9:
            pausePressed = true;
            break;
        default:
            break;
        }
        break;

    default:
        break;
    }
}

void ApplicationInput::Reset()
{
    quitPressed = false;
    pausePressed = false;
    printPressed = false;
}
