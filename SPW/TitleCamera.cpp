#include "TitleCamera.h"

TitleCamera::TitleCamera(Scene &scene) :
    Camera(scene)
{
    m_name = "TitleCamera";
}

TitleCamera::~TitleCamera()
{
}

void TitleCamera::Update()
{
    float dx = 0.3f * m_scene.GetTime().GetDelta();
    TranslateWorldView(PE_Vec2(dx, 0.0f));
}
