#include "LevelEnd.h"
#include "Camera.h"
#include "LevelScene.h"

LevelEnd::LevelEnd(Scene &scene) :
    GameBody(scene, Layer::TERRAIN_BACKGROUND), m_animator()
{
    m_name = "LevelEnd";

    // Animation "Base"
    RE_Atlas *atlas = scene.GetAssetManager().GetAtlas(AtlasID::TERRAIN);
    AssertNew(atlas);
    RE_AtlasPart *part = atlas->GetPart("LevelEnd");
    AssertNew(part);
    RE_TexAnim *anim = new RE_TexAnim(m_animator, "Base", part);
    anim->SetCycleCount(0);
}

void LevelEnd::Start()
{
    // Joue l'animation par défaut
    m_animator.PlayAnimation("Base");

    // Crée le corps
    PE_World &world = m_scene.GetWorld();
    PE_BodyDef bodyDef;
    bodyDef.type = PE_BodyType::STATIC;
    bodyDef.position = GetStartPosition() + PE_Vec2(0.5f, 0.0f);
    bodyDef.name = "LevelEnd";
    PE_Body *body = world.CreateBody(bodyDef);
    SetBody(body);

    // Crée le collider
    PE_PolygonShape box(-0.4f, 0.0f, 0.4f, 1.9f);
    PE_ColliderDef colliderDef;
    colliderDef.filter.categoryBits = CATEGORY_TERRAIN;
    colliderDef.shape = &box;
    colliderDef.isTrigger = true;
    PE_Collider *collider = body->CreateCollider(colliderDef);
}

void LevelEnd::Render()
{
    SDL_Renderer *renderer = m_scene.GetRenderer();
    Camera *camera = m_scene.GetActiveCamera();

    m_animator.Update(m_scene.GetTime());

    float scale = camera->GetWorldToViewScale();
    SDL_FRect rect = { 0 };
    rect.w = 1.0f * scale;
    rect.h = 2.0f * scale;
    camera->WorldToView(GetPosition(), rect.x, rect.y);
    m_animator.RenderCopyF(&rect, RE_Anchor::SOUTH);
}

void LevelEnd::OnCollisionEnter(GameCollision &collision)
{
    if (collision.otherCollider->CheckCategory(CATEGORY_PLAYER))
    {
        m_scene.Quit();
    }
}
