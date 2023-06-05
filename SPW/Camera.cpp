#include "Camera.h"

float Camera::GetWorldToViewScale() const
{
    return (float)m_width / m_worldView.GetWidth();
}

void Camera::WorldToView(PE_Vec2 position, float &x, float &y) const
{
    float w = m_worldView.GetWidth();
    float h = m_worldView.GetHeight();
    float xScale = (float)m_width  / w;
    float yScale = (float)m_height / h;
    x = (position.x - m_worldView.lower.x) * xScale;
    y = (position.y - m_worldView.lower.y) * yScale;
    y = (float)m_height - y;
}

void Camera::ViewToWorld(float x, float y, PE_Vec2 &position) const
{
    y = m_height - y;
    float ratioX = x / (float)m_width;
    float ratioY = y / (float)m_height;
    float w = m_worldView.GetWidth();
    float h = m_worldView.GetHeight();
    position.x = m_worldView.lower.x + ratioX * w;
    position.y = m_worldView.lower.y + ratioY * h;
}

Camera::Camera(Scene &scene) :
    GameObject(scene, Layer::FOREGROUND),
    m_width(0), m_height(0)
{
    m_name = "Camera";

    // Dimensions de la vue dans le référentiel monde
    // La hauteur est calculée pour un ratio 16/9
    float worldW = 24.0f;
    float worldH = 9.0f / 16.0f * worldW;

    //SetToRespawn(true);

    SDL_Renderer *renderer = m_scene.GetRenderer();
    SDL_RenderGetLogicalSize(renderer, &m_width, &m_height);

    m_worldView.Set(0.0f, 0.0f, worldW, worldH);
    m_worldBounds.Set(0.0f, 0.0f, 1000000.0f, worldH);
}

Camera::~Camera()
{
}
