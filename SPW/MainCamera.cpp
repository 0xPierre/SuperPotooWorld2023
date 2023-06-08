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
    
    m_center.SetTarget(playerBody->GetPosition());
    m_center.Update(levelScene.GetTime());

    PE_Vec2 cameraPos = m_center;
 
    TranslateWorldView(cameraPos - m_worldView.GetCenter());

    float dispLowerX = m_worldBounds.lower.x - m_worldView.lower.x;
    if (dispLowerX > 0.0f)
    {
        TranslateWorldView(PE_Vec2(dispLowerX, 0.0f));
    }

    // Fixe la position verticale minimale de la caméra pour ne pas voir sous le sol
    float dispLowerY = m_worldBounds.lower.y - m_worldView.lower.y;
    if (dispLowerY > 0.0f)
    {
        TranslateWorldView(PE_Vec2(0.0f, dispLowerY));
    }
}
