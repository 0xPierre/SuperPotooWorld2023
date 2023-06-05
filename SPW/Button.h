#pragma once

#include "Settings.h"
#include "UIObject.h"
#include "Text.h"

class ButtonListener;

class Button : public UIObject
{
public:
    Button(Scene &scene, RE_AtlasPart *atlasPart);
    virtual ~Button();

    enum class State : int
    {
        UP = 0, HOVER, DOWN, DISABLED
    };

    void SetListener(ButtonListener *listener);
    void SetButtonEnabled(bool enabled);
    bool IsButtonEnabled() const;
    void SetBorders(UIBorders *borders);

    void SetText(Text *text, State state);

    virtual void Update() override;
    virtual void Render() override;

protected:
    ButtonListener *m_listener;
    RE_AtlasPart *m_atlasPart;

    State m_currState;
    State m_prevState;

    UIBorders *m_borders;
    std::array<Text *, 4> m_texts;
};

class ButtonListener
{
public:
    ButtonListener();
    virtual ~ButtonListener();

    virtual void OnPress();
    virtual void OnStateChange(Button::State state);
};

inline void Button::SetListener(ButtonListener * listener)
{
    m_listener = listener;
}

inline bool Button::IsButtonEnabled() const
{
    return m_currState != Button::State::DISABLED;
}

inline void Button::SetBorders(UIBorders *borders)
{
    if (m_borders) delete m_borders;
    m_borders = borders;
}
