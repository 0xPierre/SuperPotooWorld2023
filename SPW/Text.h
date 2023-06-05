#pragma once

#include "Settings.h"
#include "UIObject.h"

class Text : public UIObject
{
public:
    Text(Scene &scene, const std::string &str, TTF_Font *font, SDL_Color color);
    virtual ~Text();

    void SetString(const std::string &str);
    void SetColor(SDL_Color color);
    void SetAnchor(RE_Anchor anchor);
    void Stretch();

    void GetNaturalSize(int &width, int &height);

    virtual void Render() override;
    virtual void Update() override;

protected:
    RE_Text m_text;
    RE_Anchor m_anchor;
    bool m_stretch;
};

inline void Text::SetString(const std::string &str)
{
    m_text.SetString(str);
}

inline void Text::SetColor(SDL_Color color)
{
    m_text.SetColor(color);
}

inline void Text::SetAnchor(RE_Anchor anchor)
{
    m_anchor = anchor;
    m_stretch = false;
}

inline void Text::Stretch()
{
    m_stretch = true;
}

inline void Text::GetNaturalSize(int &width, int &height)
{
    SDL_Texture *texture = m_text.GetTexture();
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
}
