#include "MainCamera.h"
#include "LevelScene.h"

MainCamera::MainCamera(LevelScene &scene) :
    Camera(scene), m_center()
{
    m_name = "MainCamera";

    m_center.SetSmoothTime(0.15f);
    m_center.SetMaxSpeed(1000.0f);
}

MainCamera::~MainCamera()
{
}

void MainCamera::Update()
{
    LevelScene &levelScene = dynamic_cast<LevelScene &>(m_scene);
    PE_Body *playerBody = levelScene.GetPlayer()->GetBody();
    Camera *camera = m_scene.GetActiveCamera();
    Creative * creative;

    m_center.SetTarget(playerBody->GetPosition());
    m_center.Update(levelScene.GetTime());

    PE_Vec2 startPosition(levelScene.GetStartingPoint().x + 11.0f, m_center.y);

    PE_Vec2 cameraPos = m_center;

    if (camera->GetWorldView().lower.x <= 0 && m_center.x < camera->GetWorldView().upper.x / 2)
    {
        cameraPos = startPosition;
    }
    else if (m_center.x > camera->GetWorldView().upper.x / 2)
    {
        cameraPos = m_center;
    }
    
    TranslateWorldView(cameraPos - m_worldView.GetCenter());

    // Fixe la position verticale minimale de la caméra pour ne pas voir sous le sol
    float dispLowerY = m_worldBounds.lower.y - m_worldView.lower.y;
    if (dispLowerY > 0.0f)
    {
        TranslateWorldView(PE_Vec2(0.0f, dispLowerY));
    }
}
