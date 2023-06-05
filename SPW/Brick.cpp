#include "Brick.h"
#include "Scene.h"
#include "Camera.h"

Brick::Brick(Scene &scene) :
    GameBody(scene, Layer::TERRAIN), m_animator()
{
    m_name = "Brick";
}

Brick::~Brick()
{
}

void Brick::Start()
{
}

void Brick::Render()
{
}

void Brick::OnCollisionEnter(GameCollision &collision)
{
}

void Brick::OnRespawn()
{
}
