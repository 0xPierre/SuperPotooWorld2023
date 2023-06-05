#include "UIObject.h"
#include "Scene.h"
#include "GameObject.h"

UIRect::UIRect() :
    anchorMin(Vec2(0.0f, 0.0f)), anchorMax(Vec2(1.0f, 1.0f)),
    offsetMin(Vec2(0.0f, 0.0f)), offsetMax(Vec2(0.0f, 0.0f))
{
}

UIRect::UIRect(Vec2 anchorMin, Vec2 anchorMax, Vec2 offsetMin, Vec2 offsetMax) :
    anchorMin(anchorMin), anchorMax(anchorMax), offsetMin(offsetMin), offsetMax(offsetMax)
{};

UIObject::UIObject(Scene &scene) :
    GameObject(scene, Layer::UI), m_gizmosAlpha(32), m_rect()
{
    m_name = "UIObject";
    m_rect.anchorMax.Set(1.0f, 1.0f);

    SDL_Color color = { 255, 200, 0, 255 };
    m_gizmosColor = color;
}

UIObject::~UIObject()
{
}


SDL_FRect UIObject::GetCanvasRect() const
{
    UIObject *uiParent = dynamic_cast<UIObject *>(GetParent());

    if (uiParent)
    {
        SDL_FRect pRect = uiParent->GetCanvasRect();
        float lowerX = pRect.x + pRect.w * m_rect.anchorMin.x + m_rect.offsetMin.x;
        float lowerY = pRect.y + pRect.h * m_rect.anchorMin.y + m_rect.offsetMin.y;
        float upperX = pRect.x + pRect.w * m_rect.anchorMax.x + m_rect.offsetMax.x;
        float upperY = pRect.y + pRect.h * m_rect.anchorMax.y + m_rect.offsetMax.y;

        SDL_FRect res = {
            lowerX,
            lowerY,
            fabsf(upperX - lowerX),
            fabsf(upperY - lowerY)
        };
        return res;
    }
    else
    {
        SDL_FRect res = {
            m_rect.offsetMin.x,
            m_rect.offsetMin.y,
            fabsf(m_rect.offsetMax.x - m_rect.offsetMin.x),
            fabsf(m_rect.offsetMax.y - m_rect.offsetMin.y)
        };
        return res;
    }
}

bool UIObject::Contains(const SDL_FPoint &point) const
{
    SDL_FRect rect = GetCanvasRect();
    return (point.x >= rect.x) && (point.x <= rect.x + rect.w)
        && (point.y >= rect.y) && (point.y <= rect.y + rect.h);
}

void UIObject::DrawGizmos()
{
    SDL_Renderer *renderer = m_scene.GetRenderer();
    SDL_FRect rect = GetCanvasRect();

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    if (m_gizmosAlpha > 0)
    {
        SDL_SetRenderDrawColor(
            renderer, m_gizmosColor.r, m_gizmosColor.g, m_gizmosColor.b, m_gizmosAlpha
        );
        SDL_RenderFillRectF(renderer, &rect);
    }
}
