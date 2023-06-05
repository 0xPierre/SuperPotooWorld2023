#include "InputManager.h"

InputManager::InputManager() :
    m_application(), m_mouse(), m_controls(), m_debug()
{
}

InputManager::~InputManager()
{
}

void InputManager::ProcessEvents()
{
    std::array<InputGroup *, 4> groups = {
        &m_application, &m_mouse, &m_controls, &m_debug
    };

    for (InputGroup *group : groups)
    {
        if (group->IsEnabled())
        {
            group->OnPreEventProcess();
        }
    }

    SDL_Event evt;
    while (SDL_PollEvent(&evt))
    {
        for (InputGroup *group : groups)
        {
            if (group->IsEnabled())
            {
                group->OnEventProcess(evt);
            }
        }
    }
}
