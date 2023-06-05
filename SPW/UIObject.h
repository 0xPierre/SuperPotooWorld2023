#pragma once

#include "Settings.h"
#include "GameObject.h"

class UIRect
{
public:
    UIRect();
    UIRect(Vec2 anchorMin, Vec2 anchorMax, Vec2 offsetMin, Vec2 offsetMax);

    Vec2 anchorMin; // Position relative
    Vec2 anchorMax; // Position relative
    Vec2 offsetMin; // Position absolue
    Vec2 offsetMax; // Position absolue
};

class UIBorders
{
public:
    int left, right, top, bottom;

    UIBorders(int left, int right, int top, int bottom) :
        left(left), right(right), top(top), bottom(bottom)
    {}
};

class UIObject : public GameObject
{
public:
    UIObject(Scene &scene);
    virtual ~UIObject();

    void SetLocalRect(UIRect &rect);
    UIRect &GetLocalRect();
    SDL_FRect GetCanvasRect() const;
    bool Contains(const SDL_FPoint &point) const;

    virtual void DrawGizmos() override;

protected:
    void SetGizmosColor(SDL_Color color);
    void SetGizmosFillOpacity(int alpha);

    UIRect m_rect;
    SDL_Color m_gizmosColor;
    int m_gizmosAlpha;
};

inline void UIObject::SetLocalRect(UIRect &rect)
{
    m_rect = rect;
}

inline UIRect &UIObject::GetLocalRect()
{
    return m_rect;
}

inline void UIObject::SetGizmosColor(SDL_Color color)
{
    m_gizmosColor = color;
}

inline void UIObject::SetGizmosFillOpacity(int alpha)
{
    m_gizmosAlpha = alpha;
}