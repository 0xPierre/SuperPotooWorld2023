#pragma once

#include "RE_Settings.h"

class RE_Text
{
public:

    RE_Text(SDL_Renderer *renderer, TTF_Font *font, const std::string &str, SDL_Color color);
    RE_Text(RE_Text const&) = delete;
    RE_Text& operator=(RE_Text const&) = delete;
    ~RE_Text();

    void SetString(const std::string &str);
    void SetColor(SDL_Color color);
    const std::string &GetString();
    SDL_Texture *GetTexture();

protected:

    void RefreshTexture();

    std::string m_str;
    TTF_Font *m_font;
    SDL_Color m_color;
    SDL_Texture *m_texture;
    SDL_Renderer *m_renderer;
};

inline const std::string &RE_Text::GetString()
{
    return m_str;
}

inline SDL_Texture *RE_Text::GetTexture()
{
    return m_texture;
}
