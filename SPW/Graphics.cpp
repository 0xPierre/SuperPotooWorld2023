#include "Graphics.h"

#define CIRCLE_SEG_COUNT 64

Graphics::Graphics(SDL_Renderer *renderer, const Camera &camera) :
    m_renderer(renderer), m_camera(camera)
{
}

void Graphics::DrawPoint(const PE_Vec2 &point)
{
    float x, y;
    m_camera.WorldToView(point, x, y);

    int w = 3;
    SDL_RenderDrawLine(
        m_renderer, (int)roundf(x) - w, (int)roundf(y) - w,
        (int)roundf(x) + w, (int)roundf(y) + w);
    SDL_RenderDrawLine(
        m_renderer, (int)roundf(x) + w, (int)roundf(y) - w,
        (int)roundf(x) - w, (int)roundf(y) + w);
}

void Graphics::DrawLine(const PE_Vec2 &pointA, const PE_Vec2 &pointB)
{
    float xA, yA, xB, yB;
    m_camera.WorldToView(pointA, xA, yA);
    m_camera.WorldToView(pointB, xB, yB);

    SDL_RenderDrawLine(
        m_renderer, (int)roundf(xA), (int)roundf(yA),
        (int)roundf(xB), (int)roundf(yB));
}

void Graphics::DrawVector(const PE_Vec2 &vector, const PE_Vec2 &transform)
{
    DrawLine(transform, transform + vector);
}

void Graphics::DrawShape(const PE_Shape &shape, const PE_Vec2 &transform)
{
    switch (shape.GetType())
    {
    case PE_Shape::Type::POLYGON:
    case PE_Shape::Type::POLYGON_BOX:
        DrawPolygon(dynamic_cast<const PE_PolygonShape &>(shape), transform);
        break;
    case PE_Shape::Type::CIRCLE:
        DrawCircle(dynamic_cast<const PE_CircleShape &>(shape), transform);
        break;
    case PE_Shape::Type::CAPSULE:
        DrawCapsule(dynamic_cast<const PE_CapsuleShape &>(shape), transform);
        break;
    default:
        break;
    }
}

void Graphics::DrawBody(PE_Body *body, float alpha)
{
    PE_Vec2 transform = body->GetInterpolation(alpha);
    DrawPoint(transform);

    PE_Body *parent = body->GetParent();
    if (parent)
    {
        PE_Vec2 parentTransform = parent->GetInterpolation(alpha);
        DrawLine(transform, parentTransform);
    }
}

void Graphics::DrawCollider(PE_Collider *collider, float alpha)
{
    PE_Vec2 transform = collider->GetBody()->GetInterpolation(alpha);
    DrawShape(collider->GetShape(), transform);
}

void Graphics::DrawAABB(const PE_AABB &aabb, const PE_Vec2 &transform)
{
    float xA, yA, xB, yB;
    m_camera.WorldToView(aabb.lower + transform, xA, yA);
    m_camera.WorldToView(aabb.upper + transform, xB, yB);

    SDL_FRect rect;
    rect.x = xA;
    rect.y = yA;
    rect.w = fabsf(xB - xA);
    rect.h = fabsf(yB - yA);

    SDL_RenderDrawRectF(m_renderer, &rect);
}

void Graphics::DrawCircle(const PE_CircleShape &circle, const PE_Vec2 &transform)
{
    PE_Vec2 center = circle.GetCenter() + transform;
    PE_Vec2 right = center + circle.GetRadius() * PE_Vec2::right;

    float x, y, xRight, yRight;
    m_camera.WorldToView(center, x, y);
    m_camera.WorldToView(right, xRight, yRight);

    int xCenter = (int)roundf(x);
    int yCenter = (int)roundf(y);
    int radius  = (int)roundf(xRight - x);

    int offsetx = 0;
    int offsety = radius;
    int d = radius - 1;
    int status = 0;

    while (offsety >= offsetx) {
        status += SDL_RenderDrawPoint(m_renderer, xCenter + offsetx, yCenter + offsety);
        status += SDL_RenderDrawPoint(m_renderer, xCenter + offsety, yCenter + offsetx);
        status += SDL_RenderDrawPoint(m_renderer, xCenter - offsetx, yCenter + offsety);
        status += SDL_RenderDrawPoint(m_renderer, xCenter - offsety, yCenter + offsetx);
        status += SDL_RenderDrawPoint(m_renderer, xCenter + offsetx, yCenter - offsety);
        status += SDL_RenderDrawPoint(m_renderer, xCenter + offsety, yCenter - offsetx);
        status += SDL_RenderDrawPoint(m_renderer, xCenter - offsetx, yCenter - offsety);
        status += SDL_RenderDrawPoint(m_renderer, xCenter - offsety, yCenter - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }
}

void Graphics::DrawPolygon(const PE_PolygonShape &polygon, const PE_Vec2 &transform)
{
    int count = polygon.GetVertexCount();
    const PE_Vec2 *vertices = polygon.GetVertices();
    float xVertices[PE_MAX_POLYGON_VERTICES] = { 0 };
    float yVertices[PE_MAX_POLYGON_VERTICES] = { 0 };

    assert(2 <= count && count < PE_MAX_POLYGON_VERTICES);

    for (int i = 0; i < count; i++)
    {
        PE_Vec2 vertex = vertices[i] + transform;
        m_camera.WorldToView(vertex, xVertices[i], yVertices[i]);
    }
    for (int i = 0; i < count - 1; i++)
    {
        SDL_RenderDrawLine(
            m_renderer, (int)roundf(xVertices[i]), (int)roundf(yVertices[i]),
            (int)roundf(xVertices[i + 1]), (int)roundf(yVertices[i + 1]));
    }
    SDL_RenderDrawLine(
        m_renderer, (int)roundf(xVertices[0]), (int)roundf(yVertices[0]),
        (int)roundf(xVertices[count - 1]), (int)roundf(yVertices[count - 1]));
}

void Graphics::DrawCapsule(const PE_CapsuleShape &capsule, const PE_Vec2 &transform)
{
    PE_Vec2 vertexA = capsule.GetVertices()[0] + transform;
    PE_Vec2 vertexB = capsule.GetVertices()[1] + transform;
    PE_Vec2 right = vertexA + capsule.GetRadius() * PE_Vec2::right;
    PE_Vec2 skewA = vertexB - vertexA;
    skewA *= capsule.GetRadius() / skewA.Length();
    skewA = skewA.Perp() + vertexA;

    float xA, yA, xB, yB, x0, y0, x1, y1, xS, yS;
    m_camera.WorldToView(vertexA, xA, yA);
    m_camera.WorldToView(vertexB, xB, yB);
    m_camera.WorldToView(right, x0, y0);
    m_camera.WorldToView(skewA, xS, yS);

    float radius = x0 - xA;
    float inc = 2.0f * 3.14159265f / (float)CIRCLE_SEG_COUNT;
    float theta = inc;

    x0 -= xA;
    y0 -= yA;
    for (int i = 0; i < CIRCLE_SEG_COUNT; ++i, theta += inc)
    {
        x1 = radius * cosf(theta);
        y1 = radius * sinf(theta);

        SDL_RenderDrawLine(
            m_renderer,
            (int)roundf(xA + x0), (int)roundf(yA + y0),
            (int)roundf(xA + x1), (int)roundf(yA + y1));
        SDL_RenderDrawLine(
            m_renderer,
            (int)(xB + x0), (int)(yB + y0),
            (int)(xB + x1), (int)(yB + y1));

        x0 = x1;
        y0 = y1;
    }

    xS -= xA;
    yS -= yA;
    SDL_RenderDrawLine(
        m_renderer,
        (int)roundf(xA + xS), (int)roundf(yA + yS),
        (int)roundf(xB + xS), (int)roundf(yB + yS));
    SDL_RenderDrawLine(
        m_renderer,
        (int)roundf(xA - xS), (int)roundf(yA - yS),
        (int)roundf(xB - xS), (int)roundf(yB - yS));
}
