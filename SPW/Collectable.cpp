#include "Collectable.h"
#include "Scene.h"

Collectable::Collectable(Scene &scene, Layer layer) :
    GameBody(scene, layer)
{
}

Collectable::~Collectable()
{
}

void Collectable::Collect(GameBody *collector)
{
}
