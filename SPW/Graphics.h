#pragma once

#include "Settings.h"
#include "Camera.h"

class Graphics
{
public:
    Graphics(SDL_Renderer *renderer, const Camera &camera);

    void DrawPoint(const PE_Vec2 &point);
    void DrawLine(const PE_Vec2 &pointA, const PE_Vec2 &pointB);
    void DrawVector(const PE_Vec2 &vector, const PE_Vec2 &transform);
    void DrawShape(const PE_Shape &shape, const PE_Vec2 &transform);
    void DrawBody(PE_Body *body, float alpha);
    void DrawCollider(PE_Collider *collider, float alpha);
    void DrawAABB(const PE_AABB &aabb, const PE_Vec2 &transform);

private:
    const Camera &m_camera;
    SDL_Renderer *m_renderer;

    void DrawCircle(const PE_CircleShape &circle, const PE_Vec2 &transform);
    void DrawPolygon(const PE_PolygonShape &polygon, const PE_Vec2 &transform);
    void DrawCapsule(const PE_CapsuleShape &capsule, const PE_Vec2 &transform);
};
