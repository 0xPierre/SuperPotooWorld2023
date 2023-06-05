#include "Bonus.h"
#include "Scene.h"
#include "Camera.h"
#include "Heart.h"

Bonus::Bonus(Scene &scene) :
    GameBody(scene, Layer::TERRAIN_FOREGROUND), m_animator(), m_hit(false)
{
    m_name = "Bonus";
}

void Bonus::Start()
{
}

void Bonus::Render()
{
}

void Bonus::OnRespawn()
{
}

void Bonus::OnCollisionEnter(GameCollision &collision)
{
}
