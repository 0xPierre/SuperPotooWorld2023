#pragma once

#include "Settings.h"
#include "InputGroup.h"
#include "ApplicationInput.h"
#include "MouseInput.h"
#include "ControlsInput.h"
#include "DebugInput.h"

class InputManager
{
public:
    InputManager();
    InputManager(InputManager const&) = delete;
    InputManager& operator=(InputManager const&) = delete;
    ~InputManager();

    ApplicationInput &GetApplication();
    MouseInput &GetMouse();
    ControlsInput &GetControls();
    DebugInput &GetDebug();

    void ProcessEvents();

protected:
    ApplicationInput m_application;
    MouseInput m_mouse;
    ControlsInput m_controls;
    DebugInput m_debug;
};

inline ApplicationInput &InputManager::GetApplication()
{
    return m_application;
}

inline MouseInput &InputManager::GetMouse()
{
    return m_mouse;
}

inline ControlsInput &InputManager::GetControls()
{
    return m_controls;
}

inline DebugInput &InputManager::GetDebug()
{
    return m_debug;
}
