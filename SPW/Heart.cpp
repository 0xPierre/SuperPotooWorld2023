#include "Heart.h"
#include "Scene.h"
#include "Camera.h"
#include "Player.h"
#include "Camera.h"
#include "LevelScene.h"
#include "Graphics.h"

Heart::Heart(Scene &scene) :
    Collectable(scene, Layer::COLLECTABLE), m_animator()
{
    m_name = "Heart";

    RE_Atlas* atlas = scene.GetAssetManager().GetAtlas(AtlasID::COLLECTABLE);
    AssertNew(atlas);
 
    RE_AtlasPart* part = atlas->GetPart("Heart");
    AssertNew(part);
    RE_TexAnim* idleAnim = new RE_TexAnim(
		m_animator, "Idle", part
	);
    idleAnim->SetCycleCount(0);
}

void Heart::Start()
{
    SetToRespawn(true);
    m_animator.PlayAnimation("Idle");

    // Crée le corps
    PE_World &world = m_scene.GetWorld();
    PE_BodyDef bodyDef;
    bodyDef.type = PE_BodyType::DYNAMIC;
    bodyDef.position = GetStartPosition();
    bodyDef.name = "Heart";
    bodyDef.damping.SetZero();
    PE_Body *body = world.CreateBody(bodyDef);
    SetBody(body);

    // Crée le collider
    PE_CircleShape circle(PE_Vec2(0.0f, 0.40f), 0.40f);
    PE_ColliderDef colliderDef;
    colliderDef.friction = 0.000f;
    colliderDef.filter.categoryBits = CATEGORY_COLLECTABLE;
    colliderDef.filter.maskBits = CATEGORY_COLLECTABLE | CATEGORY_TERRAIN | CATEGORY_PLAYER;
    colliderDef.shape = &circle;
    PE_Collider *collider = body->CreateCollider(colliderDef);

    PE_Vec2 pos;
    /*pos.x = .5f;
    pos.y = 10.f;
    body->SetVelocity(pos);*/
    body->SetVelocity(PE_Vec2(5.0f, 8.0f));
}

void Heart::Render()
{
    SDL_Renderer* renderer = m_scene.GetRenderer();
    Camera* camera = m_scene.GetActiveCamera();

    m_animator.Update(m_scene.GetTime());

    float scale = camera->GetWorldToViewScale();
    SDL_FRect rect = { 0 };
    rect.h = 1.0f * scale;
    rect.w = 1.0f * scale;
    camera->WorldToView(GetPosition(), rect.x, rect.y);
    m_animator.RenderCopyF(&rect, RE_Anchor::SOUTH);
}

void Heart::OnRespawn()
{
    SetEnabled(false);
}

void Heart::Update()
{
}

void Heart::FixedUpdate()
{
    PE_Body* body = GetBody();
    PE_Vec2 position = body->GetPosition();
    PE_Vec2 velocity = body->GetLocalVelocity();

    if (position.y < -2.0f)
    {
        SetEnabled(false);
        SetToRespawn(false);
        return;
    }

    PE_Vec2 originL = position + PE_Vec2(-0.35f, +0.5f);
    PE_Vec2 originR = position + PE_Vec2(+0.35f, +0.5f);

    // Les rayons ne touchent que des colliders solides (non trigger)
    // ayant la catégorie FILTER_TERRAIN
    RayHit hitL = m_scene.RayCast(originL, PE_Vec2::left, 0.2f, CATEGORY_TERRAIN, true);
    RayHit hitR = m_scene.RayCast(originR, PE_Vec2::right, 0.2f, CATEGORY_TERRAIN, true);

    // hitL.normal;

    if (hitL.collider != NULL)
    {
        // Le rayon gauche à touché le sol
        body->SetVelocity(PE_Vec2(5.0f, 0.f));
    }
    if (hitR.collider != NULL)
    {
        // Le rayon droit à touché le sol
        body->SetVelocity(PE_Vec2(-5.0f, 0.f));
    }
}

void Heart::OnCollisionEnter(GameCollision &collision)
{

}

void Heart::Collect(GameBody *collector)
{
    LevelScene* levelScene = (LevelScene*)&m_scene;

    /*if (levelScene->IsCreative())
        return;*/

    Player* player = dynamic_cast<Player*>(collector);
    if (player == nullptr)
    {
        assert(false);
        return;
    }

    SetToRespawn(false);
    SetEnabled(false);
    player->AddHeart();
}
