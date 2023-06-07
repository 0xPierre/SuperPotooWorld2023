#include "Brick.h"
#include "Scene.h"
#include "Camera.h"
#include "LevelScene.h"
#include "Graphics.h"

Brick::Brick(Scene &scene) :
    GameBody(scene, Layer::TERRAIN), m_animator()
{
    m_name = "Brick";
    RE_Atlas* atlas = scene.GetAssetManager().GetAtlas(AtlasID::TERRAIN);
    AssertNew(atlas);
    RE_AtlasPart* part = atlas->GetPart("Brick");
    AssertNew(part);
    RE_TexAnim* anim = new RE_TexAnim(m_animator, "Base", part);
    anim->SetCycleCount(0);
}

Brick::~Brick()
{
}

void Brick::Start()
{
    // Joue l'animation par défaut
    m_animator.PlayAnimation("Base");

    // Crée le corps
    PE_World& world = m_scene.GetWorld();
    PE_BodyDef bodyDef;
    bodyDef.type = PE_BodyType::STATIC;
    bodyDef.position = GetStartPosition();
    bodyDef.name = "Brick";
    PE_Body* body = world.CreateBody(bodyDef);
    SetBody(body);

    // Crée le colliderv 
    PE_PolygonShape box(0.f, 0.f, 1.f, 1.f);
    PE_ColliderDef colliderDef;
    colliderDef.filter.categoryBits = CATEGORY_TERRAIN | CATEGORY_TERRAIN_ENTITY;
    colliderDef.shape = &box;
    PE_Collider* collider = body->CreateCollider(colliderDef);
}

void Brick::Render()
{
    SDL_Renderer* renderer = m_scene.GetRenderer();
    Camera* camera = m_scene.GetActiveCamera();

    m_animator.Update(m_scene.GetTime());

    float scale = camera->GetWorldToViewScale();
    SDL_FRect rect = { 0 };
    rect.w = 1.0f * scale;
    rect.h = 1.0f * scale;
    camera->WorldToView(GetPosition(), rect.x, rect.y);
    m_animator.RenderCopyF(&rect, RE_Anchor::SOUTH_WEST);
}

void Brick::OnCollisionEnter(GameCollision &collision)
{
    // Disable end of the game in creative
    if (((LevelScene&)m_scene).IsCreative())
		return;

	// Check if the player is touching the brick
    if (collision.otherCollider->CheckCategory(CATEGORY_PLAYER))
    {
        float angle = PE_AngleDeg(collision.manifold.normal, PE_Vec2::up);
        if (angle < 70)
        {
            SetToRespawn(true);
            SetEnabled(false);
        }
    }
}

void Brick::OnRespawn()
{
    SetBodyEnabled(true);
    SetEnabled(true);
}

void Brick::DrawGizmos()
{
    SDL_Renderer* renderer = m_scene.GetRenderer();
    Graphics graphics(renderer, *m_scene.GetActiveCamera());
    PE_Vec2 position = GetPosition();
    PE_Vec2 velocity = GetVelocity();

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    graphics.DrawVector(0.5f * velocity, position);

    // Dessine en jaune les rayons pour la détection du sol
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    PE_Vec2 originL = position + PE_Vec2(-10.f, 0.f);
    PE_Vec2 originR = position + PE_Vec2(+10.f, 0.0f);
    graphics.DrawVector(0.1f * PE_Vec2::down, originL);
    graphics.DrawVector(0.1f * PE_Vec2::down, originR);
}
