#include "Text.h"
#include "Scene.h"

Text::Text(Scene &scene, const std::string &str, TTF_Font *font, SDL_Color color) :
    UIObject(scene), m_text(scene.GetRenderer(), font, str, color),
    m_anchor(RE_Anchor::CENTER), m_stretch(false)
{
    m_name = "Text";

    SDL_Renderer *renderer = scene.GetRenderer();
    m_rect.anchorMax.Set(1.0f, 1.0f);

    SDL_Color gColor = { 0, 200, 255, 255 };
    SetGizmosColor(gColor);
    SetGizmosFillOpacity(64);
}

Text::~Text()
{
}

void Text::Render()
{
    SDL_Renderer *renderer = m_scene.GetRenderer();
    SDL_FRect dstRect = GetCanvasRect();
    SDL_Texture *texture = m_text.GetTexture();

    if (m_stretch)
    {
        SDL_RenderCopyF(renderer, texture, NULL, &dstRect);
    }
    else
    {
        switch (m_anchor)
        {
        case RE_Anchor::NORTH_WEST:
            break;
        case RE_Anchor::NORTH:
            dstRect.x += 0.5f * dstRect.w;
            break;
        case RE_Anchor::NORTH_EAST:
            dstRect.x += dstRect.w;
            break;
        case RE_Anchor::WEST:
            dstRect.y += 0.5f * dstRect.h;
            break;
        case RE_Anchor::CENTER:
            dstRect.x += 0.5f * dstRect.w;
            dstRect.y += 0.5f * dstRect.h;
            break;
        case RE_Anchor::EAST:
            dstRect.x += dstRect.w;
            dstRect.y += 0.5f * dstRect.h;
            break;
        case RE_Anchor::SOUTH_WEST:
            dstRect.y += dstRect.h;
            break;
        case RE_Anchor::SOUTH:
            dstRect.x += 0.5f * dstRect.w;
            dstRect.y += dstRect.h;
            break;
        case RE_Anchor::SOUTH_EAST:
            dstRect.x += dstRect.w;
            dstRect.y += dstRect.h;
            break;
        default:
            break;
        }

        int w, h;
        GetNaturalSize(w, h);
        dstRect.h = (float)h;
        dstRect.w = (float)w;

        RE_RenderCopyF(renderer, texture, NULL, &dstRect, m_anchor);
    }
}

void Text::Update()
{
    SetVisible(true);
}
