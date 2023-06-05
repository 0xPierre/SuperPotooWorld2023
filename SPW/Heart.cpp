#include "Heart.h"
#include "Scene.h"
#include "Camera.h"
#include "Player.h"
#include "Camera.h"

Heart::Heart(Scene &scene) :
    Collectable(scene, Layer::COLLECTABLE)
{
    m_name = "Heart";
}

void Heart::Start()
{
}

void Heart::Render()
{
}

void Heart::OnRespawn()
{
}

void Heart::Update()
{
}

void Heart::OnCollisionEnter(GameCollision &collision)
{
}

void Heart::Collect(GameBody *collector)
{
}
