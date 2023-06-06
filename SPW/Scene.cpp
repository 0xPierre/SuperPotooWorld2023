#include "Scene.h"
#include "ObjectManager.h"
#include "GameObject.h"
#include "GameBody.h"
#include "Settings.h"
#include "Camera.h"

#define TIME_STEP 1.0f / 60.0f

GameCollision::GameCollision(PE_CollisionPair &collision, bool first) :
    m_collision(collision), m_first(first)
{
    manifold = collision.GetManifold();
    if (first)
    {
        collider = collision.GetColliderA();
        otherCollider = collision.GetColliderB();
    }
    else
    {
        manifold.Reverse();
        collider = collision.GetColliderB();
        otherCollider = collision.GetColliderA();
    }
    gameBody = GameBody::GetFromBody(otherCollider->GetBody());
}

Scene::Scene(SDL_Renderer *renderer, RE_Timer &mainTime, ThemeID themeID) :
    m_accu(0.0f), m_alpha(0.0f), m_drawPhysics(false), m_drawGizmos(false), m_drawID(false),
    m_makeStep(false), m_mode(UpdateMode::REALTIME),
    m_objectManager(), m_quit(false), m_renderer(renderer), m_timeStep(TIME_STEP),
    m_respawn(false), m_time(mainTime), m_assetManager(renderer, themeID),
    m_collisionListener(), m_world(PE_Vec2(0.0f, -40.0f), TIME_STEP)
{
    m_world.SetCollisionListener(&m_collisionListener);
    m_activeCam = nullptr;
}

Scene::~Scene()
{
    m_objectManager.DeleteObjects();
}

class DrawCallback : public PE_QueryCallback
{
public:
    DrawCallback() {}
    virtual ~DrawCallback() {}

    virtual bool ReportCollider(PE_Collider *collider)
    {
        GameBody *object = GameBody::GetFromBody(collider->GetBody());
        object->DrawCollider(collider);

        // On continue la recherche
        return true;
    }
};

void Scene::Render()
{
    // Détermine les objets visibles par la caméra
    if (m_activeCam != nullptr)
    {
        PE_AABB worldView = m_activeCam->GetWorldView();
        worldView.Extend(2.0f);
        m_objectManager.AddVisibleBodies(m_world, worldView);
    }

    // Dessine les objets visibles par la caméra
    // Message : Render()
    m_objectManager.ProcessVisibleObjects();
    for (auto it = m_objectManager.visibleObjectsBegin();
         it != m_objectManager.visibleObjectsEnd(); ++it)
    {
        GameObject *object = *it;
        object->Render();
    }

    // Dessine les corps présents dans le moteur physique
    // Messages : DrawCollider(), DrawBody()
    if (m_drawPhysics && m_activeCam != nullptr)
    {
        DrawCallback callback;
        PE_AABB worldView = m_activeCam->GetWorldView();
        m_world.QueryAABB(callback, worldView);

        for (auto it = m_objectManager.visibleObjectsBegin();
             it != m_objectManager.visibleObjectsEnd(); ++it)
        {
            GameBody *gameBody = dynamic_cast<GameBody *>(*it);
            if (gameBody)
            {
                gameBody->DrawBody(gameBody->GetBody());
            }
        }
    }

    // Dessine les gizmos
    // Message : DrawGizmos()
    if (m_drawGizmos)
    {
        for (auto it = m_objectManager.visibleObjectsBegin();
             it != m_objectManager.visibleObjectsEnd(); ++it)
        {
            GameObject *object = *it;
            object->DrawGizmos();
        }
    }

    // Dessine les ID
    if (m_drawID)
    {
        for (auto it = m_objectManager.visibleObjectsBegin();
             it != m_objectManager.visibleObjectsEnd(); ++it)
        {
            GameBody *gameBody = dynamic_cast<GameBody *>(*it);
            if (gameBody)
            {
                gameBody->DrawID();
            }
        }
    }
}

bool Scene::Update()
{
    // Appelle les méthodes asynchrones
    if (m_respawn)
    {
        // Réinitialise la scene
        // Message : OnRespawn()
        m_objectManager.RespawnObjects();
        m_respawn = false;
        OnRespawn();
    }

    // Messages : Start(), OnEnable(), OnDisable(), Delete()
    m_objectManager.ProcessObjects();

    // Met à jour les entrées de l'utilisateur
    m_inputManager.ProcessEvents();

    // Met à jour les objets
    // Messages : Update(), FixedUpdate()
    UpdateGameObjects();

    if (m_inputManager.GetApplication().printPressed)
    {
        m_objectManager.PrintObjects();
    }

    if (m_quit)
    {
        return true;
    }

    return false;
}

void Scene::OnRespawn()
{
}

void Scene::UpdateGameObjects()
{
    // Appelle la méthode FixedUpdate de chaque GameObject
    if (m_mode == UpdateMode::REALTIME)
    {
        // Mode temps réel
        m_accu += m_time.GetDelta();
        while (m_accu >= m_timeStep)
        {
            m_world.FixedUpdate();

            for (auto it = m_objectManager.begin(); it != m_objectManager.end(); ++it)
            {
                GameObject *object = *it;
                if (object->IsEnabled())
                {
                    object->FixedUpdate();
                }
            }
            m_accu -= m_timeStep;
        }
        m_alpha = m_accu / m_timeStep;
    }
    else
    {
        // Mode pas à pas
        if (m_makeStep)
        {
            m_world.FixedUpdate();

            for (auto it = m_objectManager.begin(); it != m_objectManager.end(); ++it)
            {
                GameObject *object = *it;
                if (object->IsEnabled())
                {
                    object->FixedUpdate();
                }
            }
        }
        m_makeStep = false;
        m_alpha = 1.0f;
    }

    // Appelle la méthode Update de chaque GameObject
    int enabledCount = 0;
    for (auto it = m_objectManager.begin();
         it != m_objectManager.end(); ++it)
    {
        GameObject *object = *it;
        if (object->IsEnabled())
        {
            object->Update();
            enabledCount++;
        }
    }
}

void SceneListener::OnCollisionEnter(PE_CollisionPair &collision)
{
    GameBody *gameBodyA = GameBody::GetFromBody(collision.GetColliderA()->GetBody());
    GameBody *gameBodyB = GameBody::GetFromBody(collision.GetColliderB()->GetBody());
    assert(gameBodyA && gameBodyB);

    GameCollision gameCollisionA(collision, true);
    GameCollision gameCollisionB(collision, false);

    gameBodyA->OnCollisionEnter(gameCollisionA);
    gameBodyB->OnCollisionEnter(gameCollisionB);
}

void SceneListener::OnCollisionStay(PE_CollisionPair &collision)
{
    GameBody *gameBodyA = GameBody::GetFromBody(collision.GetColliderA()->GetBody());
    GameBody *gameBodyB = GameBody::GetFromBody(collision.GetColliderB()->GetBody());
    assert(gameBodyA && gameBodyB);

    GameCollision gameCollisionA(collision, true);
    GameCollision gameCollisionB(collision, false);

    gameBodyA->OnCollisionStay(gameCollisionA);
    gameBodyB->OnCollisionStay(gameCollisionB);
}

void SceneListener::OnCollisionExit(PE_CollisionPair &collision)
{
    GameBody *gameBodyA = GameBody::GetFromBody(collision.GetColliderA()->GetBody());
    GameBody *gameBodyB = GameBody::GetFromBody(collision.GetColliderB()->GetBody());
    assert(gameBodyA && gameBodyB);

    GameCollision gameCollisionA(collision, true);
    GameCollision gameCollisionB(collision, false);

    gameBodyA->OnCollisionExit(gameCollisionA);
    gameBodyB->OnCollisionExit(gameCollisionB);
}


class SceneRayCastCallback : public PE_RayCastCallback
{
public:
    SceneRayCastCallback(int maskBits, bool solidOnly) :
        m_maskBits(maskBits), m_solidOnly(solidOnly)
    {
        firstHit.gameBody = nullptr;
        firstHit.collider = nullptr;
    }
    virtual ~SceneRayCastCallback() {}


    virtual bool ReportCollider(PE_Collider *collider, const PE_RayCastHit &hit)
    {
        if (firstHit.collider != nullptr && firstHit.fraction < hit.fraction)
        {
            return true;
        }
        if (m_solidOnly && collider->IsTrigger())
        {
            return true;
        }

        if (collider->CheckCategory(m_maskBits))
        {
            firstHit.collider = collider;
            firstHit.fraction = hit.fraction;
            firstHit.gameBody = GameBody::GetFromBody(collider->GetBody());
            firstHit.hitPoint = hit.hitPoint;
            firstHit.normal = hit.normal;
        }

        return true;
    }

    RayHit firstHit;

private:
    int m_maskBits;
    bool m_solidOnly;
};

RayHit Scene::RayCast(PE_Vec2 origin, PE_Vec2 direction, float distance, int maskBits, bool solidOnly)
{
    SceneRayCastCallback callback(maskBits, solidOnly);
    m_world.RayCast(callback, origin, direction, distance);

    return callback.firstHit;
}

class SceneAreaCallback : public PE_QueryCallback
{
public:
    SceneAreaCallback(int maskBits) : m_gameBody(nullptr), m_maskBits(maskBits) {}

    virtual bool ReportCollider(PE_Collider *collider)
    {
        if (collider->CheckCategory(m_maskBits) == false)
            return true;

        GameBody *gameBody = GameBody::GetFromBody(collider->GetBody());
        if (gameBody == nullptr)
            return true;

        m_gameBody = gameBody;
        return false;
    }

    GameBody *m_gameBody;

private:
    int m_maskBits;
};

GameBody *Scene::OverlapArea(const PE_AABB &area, int maskBits)
{
    SceneAreaCallback callback(maskBits);
    m_world.QueryAABB(callback, area);
    return callback.m_gameBody;
}
