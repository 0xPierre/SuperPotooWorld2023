#include "GameBody.h"
#include "Scene.h"
#include "Graphics.h"

GameBody::GameBody(Scene &scene, Layer layer) :
    GameObject(scene, layer), m_body(nullptr), m_startPos(PE_Vec2::zero),
    m_textID(nullptr)
{
    m_name = "GameBody";

    SDL_Color debugColor = { 0, 200, 255, 255 };
    m_debugColor = debugColor;

    SDL_Color sleepColor = { 150, 150, 150, 255 };
    m_sleepColor = sleepColor;
}

GameBody::~GameBody()
{
    DeleteBody();
    if (m_textID) delete m_textID;
}

void GameBody::OnCollisionEnter(GameCollision &collision)
{
}

void GameBody::OnCollisionStay(GameCollision &collision)
{
}

void GameBody::OnCollisionExit(GameCollision &collision)
{
}

GameBody *GameBody::GetFromBody(PE_Body *body)
{
    return body->GetUserData().gameBody;
}

void GameBody::SetBody(PE_Body *body)
{
    assert(body);
    if (m_body)
    {
        m_body->GetUserData().gameBody = nullptr;
    }
    m_body = body;
    m_body->GetUserData().gameBody = this;
}

void GameBody::DeleteBody()
{
    if (m_body == nullptr)
    {
        return;
    }
    m_scene.GetWorld().RemoveBody(m_body);
    m_body = nullptr;
}

PE_Vec2 GameBody::GetPosition() const
{
    if (m_body)
    {
        return m_body->GetInterpolation(m_scene.GetAlpha());
    }
    else
    {
        assert(false);
        return PE_Vec2::zero;
    }
}

PE_Vec2 GameBody::GetVelocity() const
{
    if (m_body)
    {
        return m_body->GetVelocity();
    }
    else
    {
        assert(false);
        return PE_Vec2::zero;
    }
}

void GameBody::DrawBody(PE_Body *body)
{
    SDL_Renderer *renderer = m_scene.GetRenderer();
    SDL_Color color = body->IsAwake() ? m_debugColor : m_sleepColor;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    Graphics graphics(m_scene.GetRenderer(), *m_scene.GetActiveCamera());
    graphics.DrawBody(body, m_scene.GetAlpha());
}

void GameBody::DrawCollider(PE_Collider *collider)
{
    SDL_Renderer *renderer = m_scene.GetRenderer();
    SDL_Color color = collider->GetBody()->IsAwake() ? m_debugColor : m_sleepColor;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    Graphics graphics(m_scene.GetRenderer(), *m_scene.GetActiveCamera());
    graphics.DrawCollider(collider, m_scene.GetAlpha());
}

void GameBody::DrawID()
{
    if (m_textID == nullptr)
    {
        AssetManager &assets = m_scene.GetAssetManager();
        TTF_Font *font = assets.GetFont(FontID::SMALL);
        SDL_Color color = assets.GetColor(ColorID::WHITE);
        m_textID = new RE_Text(m_scene.GetRenderer(), font, std::to_string(GetID()), color);
    }

    SDL_Renderer *renderer = m_scene.GetRenderer();
    Camera *camera = m_scene.GetActiveCamera();
    SDL_Texture *texture = m_textID->GetTexture();

    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    SDL_FRect dstRect = { 0 };
    camera->WorldToView(GetPosition(), dstRect.x, dstRect.y);
    dstRect.w = (float)w;
    dstRect.h = (float)h;

    RE_RenderCopyF(renderer, texture, NULL, &dstRect, RE_Anchor::CENTER);
}
