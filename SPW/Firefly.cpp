#include "Firefly.h"
#include <chrono>
#include "Scene.h"
#include "Camera.h"
#include "LevelScene.h"
#include "stdlib.h"

Firefly::Firefly(Scene& scene) :
    Collectable(scene, Layer::COLLECTABLE), m_animator(), m_state(State::IDLE)
{
    m_name = "Firefly";
    m_randomPos = rand();
    
    RE_Atlas* atlas = scene.GetAssetManager().GetAtlas(AtlasID::COLLECTABLE);
    AssertNew(atlas);
    // Animation "Idle"
    RE_AtlasPart* part = atlas->GetPart("Firefly");
    AssertNew(part);
    RE_TexAnim* flyingAnim = new RE_TexAnim(m_animator, "Idle", part);
    flyingAnim->SetCycleCount(-1);
    flyingAnim->SetCycleTime(0.3f);
    // Couleur des colliders en debug
    m_debugColor.r = 255;
    m_debugColor.g = 0;
    m_debugColor.b = 255;
}

Firefly::~Firefly()
{
}

void Firefly::Start()
{
    // Joue l'animation par défaut
    m_animator.PlayAnimation("Idle");

    // Crée le corps
    PE_World& world = m_scene.GetWorld();
    PE_BodyDef bodyDef;
    bodyDef.type = PE_BodyType::STATIC;
    bodyDef.position = GetStartPosition() + PE_Vec2(0.5f, 0.5f);
    bodyDef.name = "Firefly";
    bodyDef.damping.SetZero();
    PE_Body* body = world.CreateBody(bodyDef);
    SetBody(body);

    // Crée le collider
    PE_ColliderDef colliderDef;
    PE_CircleShape circle(PE_Vec2::zero, 0.25f);
    colliderDef.isTrigger = true;
    colliderDef.filter.categoryBits = CATEGORY_COLLECTABLE;
    colliderDef.shape = &circle;
    PE_Collider* collider = body->CreateCollider(colliderDef);
}

void Firefly::Render()
{
    SDL_Renderer* renderer = m_scene.GetRenderer();
    Camera* camera = m_scene.GetActiveCamera();
    const clock_t currentTime = clock();
    const double milliseconds = (double)currentTime / (CLOCKS_PER_SEC / 1000);

    m_animator.Update(m_scene.GetTime());

    float scale = camera->GetWorldToViewScale();
    SDL_FRect rect = { 0 };
    rect.h = 1.0f * scale;
    rect.w = 1.0f * scale;
    camera->WorldToView(GetPosition(), rect.x, rect.y);
    if (m_state == State::IDLE)
    {
        rect.x += sin((milliseconds + m_randomPos) / 400) * 10;
        rect.y += sin((milliseconds + m_randomPos) / 400) * 15;
        m_dyingTimeMemory = milliseconds;
    }
    else if (m_state == State::DYING)
    {
        rect.x -= (milliseconds - m_dyingTimeMemory) / 2;
        rect.y -= (milliseconds - m_dyingTimeMemory) / 3;
        rect.w -= (milliseconds - m_dyingTimeMemory) / 50;
        rect.h -= (milliseconds - m_dyingTimeMemory) / 50;
        m_animator.MultAlpha(1 - (milliseconds - m_dyingTimeMemory) / 1000);

        if (milliseconds - m_dyingTimeMemory > 4000)
        {
            SetEnabled(false);
        }
    }
    m_animator.RenderCopyF(&rect, RE_Anchor::CENTER);
}

void Firefly::OnRespawn()
{
    m_state = State::IDLE;

    SetBodyEnabled(true);
    SetEnabled(true);

    m_animator.StopAnimations();
    m_animator.PlayAnimation("Idle");
}

void Firefly::Collect(GameBody * collector)
{
    LevelScene* levelScene = (LevelScene *)&m_scene;

    if (levelScene->IsCreative())
        return;
    if (m_state == State::DYING)
    {
        return;
    }

    Player* player = dynamic_cast<Player*>(collector);
    if (player == nullptr)
    {
        assert(false);
        return;
    }

    SetToRespawn(true);

    m_state = State::DYING;
    player->AddFirefly(1);
}

void Firefly::CreativeRemove()
{
    SetToRespawn(true);
	SetEnabled(false);
}