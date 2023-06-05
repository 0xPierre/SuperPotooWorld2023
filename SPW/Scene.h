#pragma once

#include "Settings.h"
#include "inputManager.h"
#include "ObjectManager.h"
#include "AssetManager.h"

class Camera;

struct SceneStats
{
    /// @brief Nombre d'objets visibles par la caméra active.
    int visibleCount;

    /// @brief Nombre d'objets présents dans la scène.
    int objectCount;

    /// @brief Nombre d'objets inactifs.
    int disabledCount;

    /// @brief Nombre d'objets à réinitialiser quand le joueur meurt.
    int toRespawnCount;

    /// @brief Nombre de corps présents dans le moteur physique.
    int bodyCount;
};

class Scene;
class SceneListener : public PE_CollisionListener
{
public:

    virtual void OnCollisionEnter(PE_CollisionPair &collision) override;
    virtual void OnCollisionStay(PE_CollisionPair &collision) override;
    virtual void OnCollisionExit(PE_CollisionPair &collision) override;
};

class GameCollision
{
public:
    GameCollision(PE_CollisionPair &collision, bool first);

    PE_Manifold manifold;
    PE_Collider *collider;
    PE_Collider *otherCollider;
    GameBody *gameBody;

    void ResolveUp();
    bool IsEnabled() const;
    void SetEnabled(bool enabled);

private:
    PE_CollisionPair &m_collision;
    bool m_first;
};

inline void GameCollision::ResolveUp()
{
    if (m_first)
        m_collision.ResolveUpA();
    else
        m_collision.ResolveUpB();
}

inline bool GameCollision::IsEnabled() const
{
    return m_collision.IsEnabled();
}

inline void GameCollision::SetEnabled(bool enabled)
{
    m_collision.SetEnabled(enabled);
}

class RayHit
{
public:
    PE_Collider *collider;
    GameBody *gameBody;
    PE_Vec2 normal;
    PE_Vec2 hitPoint;
    float fraction;
};

class Scene
{
public:

    Scene(SDL_Renderer *renderer, RE_Timer &mainTime, ThemeID themeID);
    Scene(Scene const&) = delete;
    Scene& operator=(Scene const&) = delete;
    virtual ~Scene();

    virtual void Render();
    virtual bool Update();
    virtual void OnRespawn();

    SDL_Renderer *GetRenderer();

    InputManager &GetInputManager();
    AssetManager &GetAssetManager();
    PE_World &GetWorld();

    void Respawn();

    void Quit();

    float GetAlpha();

    float GetFixedTimeStep();

    Camera *GetActiveCamera();

    const RE_Timer &GetTime() const;

    RayHit RayCast(
        PE_Vec2 origin, PE_Vec2 direction, float distance,
        int maskBits, bool solidOnly
    );
    GameBody *OverlapArea(const PE_AABB &area, int maskBits);

protected:
    friend class GameObject;

    void UpdateGameObjects();

    enum class UpdateMode
    {
        REALTIME,
        STEP_BY_STEP
    };
    /// @brief Mode de mise à jour du moteur physique de la scène (temps réel ou pas-à-pas).
    UpdateMode m_mode;

    RE_Timer &m_time;

    /// @brief Moteur de rendu.
    SDL_Renderer *m_renderer;

    ///// @brief Moteur physique.
    PE_World m_world;

    ///// @brief Gestionnaire des instances de GameObject.
    ObjectManager m_objectManager;

    ///// @brief Gestionnaire des entrées utilisateur.
    InputManager m_inputManager;

    ///// @brief Gestionnaire des ressources de la scène.
    AssetManager m_assetManager;

    ///// @brief Pointeur vers la caméra utilisée pour rendre la scène.
    Camera *m_activeCam;

    /// @brief Pas de temps fixe.
    float m_timeStep;

    /// @brief Accumulateur pour la mise à jour à pas de temps fixe.
    float m_accu;

    /// @brief Paramètre d'interpolation pour les positions des corps physiques.
    float m_alpha;

    /// @brief Booléen indiquant s'il faut afficher les "gizmos" (debug).
    bool m_drawGizmos;

    /// @brief Booléen indiquant s'il faut afficher corps (debug).
    bool m_drawPhysics;

    bool m_drawID;

    /// @brief Booléen indiquant s'il faut effectuer un pas fixe en mode pas-à-pas.
    bool m_makeStep;

    /// @brief Booléen indiquant s'il faut réinitialiser la scène.
    bool m_respawn;

    /// @brief Booléen indiquant s'il faut quitter la scène.
    bool m_quit;

    SceneListener m_collisionListener;
};

inline SDL_Renderer *Scene::GetRenderer()
{
    return m_renderer;
}

inline InputManager &Scene::GetInputManager()
{
    return m_inputManager;
}

inline AssetManager &Scene::GetAssetManager()
{
    return m_assetManager;
}

inline PE_World &Scene::GetWorld()
{
    return m_world;
}

inline void Scene::Respawn()
{
    m_respawn = true;
}

inline void Scene::Quit()
{
    m_quit = true;
}

inline float Scene::GetAlpha()
{
    return m_alpha;
}

inline float Scene::GetFixedTimeStep()
{
    return m_timeStep;
}

inline Camera *Scene::GetActiveCamera()
{
    return m_activeCam;
}

inline const RE_Timer &Scene::GetTime() const
{
    return m_time;
}
