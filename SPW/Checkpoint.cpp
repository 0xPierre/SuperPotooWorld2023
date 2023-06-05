#include "Checkpoint.h"
#include "Scene.h"
#include "Camera.h"

Checkpoint::Checkpoint(Scene &scene) :
    GameBody(scene, Layer::TERRAIN_BACKGROUND), m_animator(), m_isActive(false)
{
    m_name = "Checkpoint";
}

void Checkpoint::Start()
{
}

void Checkpoint::Render()
{
}

void Checkpoint::OnCollisionEnter(GameCollision &collision)
{
}
