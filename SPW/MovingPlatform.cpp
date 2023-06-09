#include "MovingPlatform.h"
#include "Scene.h"
#include "Camera.h"
#include "Heart.h"
#include "LevelScene.h"
#include "StaticMap.h"

MovingPlatform::MovingPlatform(Scene& scene) :
    GameBody(scene, Layer::TERRAIN_FOREGROUND), m_animator(), m_hit(false), m_state(State::MOVING_RIGHT)
{
    m_name = "MovingPlatform";
    RE_Atlas* atlas = scene.GetAssetManager().GetAtlas(AtlasID::TERRAIN);
    AssertNew(atlas);

    RE_AtlasPart* part = atlas->GetPart("MovingPlatform");
    AssertNew(part);
    RE_TexAnim* baseAnim = new RE_TexAnim(
        m_animator, "Base", part
    );
    baseAnim->SetCycleCount(0);
}

void MovingPlatform::Start()
{
    // Joue l'animation par défaut
    m_animator.PlayAnimation("Base");

    // Crée le corps
    PE_World& world = m_scene.GetWorld();
    PE_BodyDef bodyDef;
    bodyDef.type = PE_BodyType::DYNAMIC;
    bodyDef.position = GetStartPosition();
    bodyDef.name = "MovingPlatform";
    PE_Body* body = world.CreateBody(bodyDef);
    SetBody(body);
    body->SetGravityScale(0);

    // Crée le collider
    PE_PolygonShape box(0.f, 0.2f, 4.f, .5f);
    PE_ColliderDef colliderDef;
    colliderDef.friction = 1.0f;
    colliderDef.filter.categoryBits = CATEGORY_TERRAIN_ENTITY | CATEGORY_TERRAIN;
    colliderDef.filter.maskBits = CATEGORY_TERRAIN | CATEGORY_STOP_MOVING_PLATFORM | CATEGORY_PLAYER;
    colliderDef.shape = &box;
    PE_Collider* collider = body->CreateCollider(colliderDef);

    SetToRespawn(true);
}

void MovingPlatform::Render()
{
    SDL_Renderer* renderer = m_scene.GetRenderer();
    Camera* camera = m_scene.GetActiveCamera();

    m_animator.Update(m_scene.GetTime());

    float scale = camera->GetWorldToViewScale();
    SDL_FRect rect = { 0 };
    rect.w = 4.0f * scale;
    rect.h = 0.50f * scale;
    camera->WorldToView(GetPosition(), rect.x, rect.y);
    m_animator.RenderCopyF(&rect, RE_Anchor::SOUTH_WEST);
}

void MovingPlatform::FixedUpdate()
{
    PE_Body* body = GetBody();
    PE_Vec2 position = body->GetPosition();
    PE_Vec2 velocity = body->GetLocalVelocity();


    if (m_state == State::MOVING_RIGHT)
    {
        body->SetVelocity(PE_Vec2(2.f, 0.f));
    }
    else if (m_state == State::MOVING_UP)
    {
        body->SetVelocity(PE_Vec2(0.f, 2.f));
    }
    else if (m_state == State::MOVING_DOWN)
    {
        body->SetVelocity(PE_Vec2(0.f, -2.f));
    }
    else
    {
        body->SetVelocity(PE_Vec2(-2.f, 0.f));
    }
}

void MovingPlatform::OnRespawn()
{
    m_animator.PlayAnimation("Base");
    m_state = State::MOVING_RIGHT;

    PE_Body* body = GetBody();
    body->SetPosition(GetStartPosition());
    SetToRespawn(true);
}

void MovingPlatform::OnCollisionEnter(GameCollision& collision)
{
    PE_Body* body = GetBody();
    PE_Vec2 position = body->GetPosition();
    PE_Vec2 velocity = body->GetLocalVelocity();
    LevelScene* levelscene = dynamic_cast<LevelScene*>(&m_scene);

    if (collision.otherCollider->CheckCategory(CATEGORY_STOP_MOVING_PLATFORM | CATEGORY_TERRAIN))
    {
        float angle = PE_AngleDeg(collision.manifold.normal, PE_Vec2::up);
        bool collideInCorner = angle == 90;

        if (collideInCorner)
        {
            if (m_state == State::MOVING_RIGHT)
                m_state = State::MOVING_LEFT;
            else
                m_state = State::MOVING_RIGHT;
        }
    }
}
