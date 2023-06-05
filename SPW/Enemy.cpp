#include "Enemy.h"

Enemy::Enemy(Scene &scene) :
    GameBody(scene, Layer::ENEMY)
{
}

Enemy::~Enemy()
{
}

void Enemy::Damage(GameBody *damager)
{
    SetEnabled(false);
}
