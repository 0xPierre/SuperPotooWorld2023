#include "DebugCamera.h"
#include "LevelScene.h"

DebugCamera::DebugCamera(LevelScene &scene, const Camera &trackedCam) :
    Camera(scene), m_trackedCam(trackedCam), m_track(true)
{
    m_name = "DebugCamera";

    m_worldView = trackedCam.GetWorldView();
    m_worldView.Scale(m_worldView.GetCenter(), 2.0f);
}

void DebugCamera::Update()
{
    if (m_scene.GetActiveCamera() != this)
        return;

    InputManager &inputManager = m_scene.GetInputManager();
    MouseInput &mouse = inputManager.GetMouse();
    DebugInput &debug = inputManager.GetDebug();

    PE_Vec2 mousePos, mouseDelta;
    PE_AABB trackedView = m_trackedCam.GetWorldView();

    ViewToWorld(mouse.viewPos.x, mouse.viewPos.y, mousePos);
    ViewToWorld(mouse.lastViewPos.x, mouse.lastViewPos.y, mouseDelta);
    mouseDelta -= mousePos;

    if ((mouse.wheel > 0) && (m_worldView.upper.x - m_worldView.lower.x > 1.0f))
    {
        m_worldView.Scale(mousePos, 0.8f);
    }
    else if (mouse.wheel < 0)
    {
        m_worldView.Scale(mousePos, 1.0f / 0.8f);
    }

    if (mouse.rightDown)
    {
        m_worldView.Translate(mouseDelta);
    }

    if (debug.trackPressed)
    {
        m_track = !(m_track);
        m_trackedLower = trackedView.lower;
    }

    if (m_track)
    {
        m_worldView.Translate(trackedView.lower - m_trackedLower);
        m_trackedLower = trackedView.lower;
    }

    if (mouse.leftClickCount > 1)
    {
        LevelScene *levelScene = dynamic_cast<LevelScene *>(&m_scene);
        Player *player = levelScene->GetPlayer();
        PE_Body *body = player->GetBody();
        body->SetPosition(mousePos);
        body->SetVelocity(PE_Vec2::zero);
    }

    if (mouse.middleClick)
    {
        m_worldView = trackedView;
        m_worldView.Scale(m_worldView.GetCenter(), 2.0f);
    }
}
