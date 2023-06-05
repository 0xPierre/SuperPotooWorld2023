#pragma once
#include "Settings.h"
#include "UIObject.h"

class Image : public UIObject
{
public:
    Image(Scene &scene, RE_AtlasPart *atlasPart, int index);

    void SetBorders(UIBorders *borders);

    void GetNaturalSize(int &width, int &height) const;
    float GetNaturalRatio() const;

    virtual void Render() override;
    virtual void Update() override;

private:
    SDL_Texture *m_texture;
    SDL_Rect m_srcRect;
    UIBorders *m_borders;
    int m_index;
};

inline void Image::SetBorders(UIBorders *borders)
{
    if (m_borders) delete m_borders;
    m_borders = borders;
}

inline void Image::GetNaturalSize(int &width, int &height) const
{
    width = m_srcRect.w;
    height = m_srcRect.h;
}

inline float Image::GetNaturalRatio() const
{
    return (float)m_srcRect.w / (float)m_srcRect.h;
}

inline void Image::Render()
{
    SDL_Renderer *renderer = m_scene.GetRenderer();
    if (m_texture)
    {
        SDL_FRect dstRect = GetCanvasRect();
        if (m_borders)
        {
            RE_RenderCopyBordersF(
                renderer, m_texture, &m_srcRect, &dstRect, RE_Anchor::NORTH_WEST,
                m_borders->left, m_borders->right, m_borders->top, m_borders->bottom
            );
        }
        else
        {
            SDL_RenderCopyF(renderer, m_texture, &m_srcRect, &dstRect);
        }
    }
}

inline void Image::Update()
{
    SetVisible(true);
}
