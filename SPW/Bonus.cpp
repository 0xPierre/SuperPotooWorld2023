#include "Bonus.h"
#include "Scene.h"
#include "Camera.h"
#include "Heart.h"
#include "LevelScene.h"

Bonus::Bonus(Scene &scene) :
    GameBody(scene, Layer::TERRAIN_FOREGROUND), m_animator(), m_hit(false), m_state(State::FULL)
{
    m_name = "Bonus";
    RE_Atlas* atlas = scene.GetAssetManager().GetAtlas(AtlasID::TERRAIN);
    AssertNew(atlas);
    
    // Animation "Full"
    RE_AtlasPart* part = atlas->GetPart("BonusFull");
    AssertNew(part);
    RE_TexAnim* fullAnim = new RE_TexAnim(
        m_animator, "Full", part
    );
    fullAnim->SetCycleCount(-1);
    fullAnim->SetCycleTime(0.5f);

    // Animation "Full"
    part = atlas->GetPart("BonusEmpty");
    AssertNew(part);
    RE_TexAnim* emptyAnim = new RE_TexAnim(
        m_animator, "Empty", part
    );
    emptyAnim->SetCycleCount(0);
}

void Bonus::Start()
{
    // Joue l'animation par défaut
    m_animator.PlayAnimation("Full");

    // Crée le corps
    PE_World& world = m_scene.GetWorld();
    PE_BodyDef bodyDef;
    bodyDef.type = PE_BodyType::STATIC;
    bodyDef.position = GetStartPosition();
    bodyDef.name = "Bonus";
    PE_Body* body = world.CreateBody(bodyDef);
    SetBody(body);

    // Crée le collider
    PE_PolygonShape box(0.f, 0.f, 1.f, 1.f);
    PE_ColliderDef colliderDef;
    colliderDef.filter.categoryBits = CATEGORY_TERRAIN | CATEGORY_TERRAIN_ENTITY;
    colliderDef.shape = &box;
    PE_Collider* collider = body->CreateCollider(colliderDef);
}

void Bonus::Render()
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

void Bonus::OnRespawn()
{
    m_animator.PlayAnimation("Full");
    m_state = State::FULL;
}

void Bonus::OnCollisionEnter(GameCollision &collision)
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
            if (m_state == State::FULL)
            {
                m_state = State::EMPTY;
                SetToRespawn(true);
                m_animator.PlayAnimation("Empty");
                Heart* heart = new Heart(m_scene);
                PE_Vec2 pos = GetPosition();
                pos.x += 0.7f;
                pos.y += 1.f;

                heart->SetStartPosition(pos);
            }
        }
    }
}
