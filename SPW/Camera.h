#pragma once

#include "Settings.h"
#include "GameObject.h"

class Camera : public GameObject
{
public:
    Camera(Scene &scene);
    virtual ~Camera();

    PE_AABB GetWorldView() const;
    void SetWorldView(const PE_AABB &worldView);
    void TranslateWorldView(const PE_Vec2 &displacement);

    PE_AABB GetWorldBounds() const;
    void SetWorldBounds(const PE_AABB &bounds);

    int GetWidth() const;
    int GetHeight() const;

    float GetWorldToViewScale() const;

    void WorldToView(PE_Vec2 position, float &x, float &y) const;
    void ViewToWorld(float x, float y, PE_Vec2 &position) const;

protected:
    /// @brief Rectangle représentant la vue de la caméra.
    /// Ces coordonnées sont dans le référentiel monde.
    PE_AABB m_worldView;

    PE_AABB m_worldBounds;

    /// @brief Largeur en pixels de la caméra.
    int m_width;

    /// @brief Hauteur en pixels de la caméra.
    int m_height;
};

inline PE_AABB Camera::GetWorldView() const
{
    return m_worldView;
}

inline void Camera::SetWorldView(const PE_AABB &worldView)
{
    m_worldView = worldView;
}

inline void Camera::TranslateWorldView(const PE_Vec2 &displacement)
{
    m_worldView.Translate(displacement);
}

inline PE_AABB Camera::GetWorldBounds() const
{
    return m_worldBounds;
}

inline void Camera::SetWorldBounds(const PE_AABB &bounds)
{
    m_worldBounds = bounds;
}

inline int Camera::GetWidth() const
{
    return m_width;
}

inline int Camera::GetHeight() const
{
    return m_height;
}
